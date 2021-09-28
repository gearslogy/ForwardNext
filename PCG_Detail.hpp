//
// Created by father on 9/9/21.
//

#ifndef NODE_AND_ITEMS_PCG_DETAIL_HPP
#define NODE_AND_ITEMS_PCG_DETAIL_HPP
#include <string>
#include <any>
#include <tuple>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <optional>
#include <utility>
#include "magic_enum.hpp"
#include "PCG_Types.h"
#include <glm/glm.hpp>
#include <concepts>


//! @std::string name, @std::any valueHandle, @ati is only register for meta information, this handle attrib
using PCG_AttribValueType = std::tuple<std::string , std::any, PCG_AttributeTypeInfo >;


// attrib value handle, it's a container
class PCG_Detail{
private:
    std::vector<PCG_AttribValueType> dataHandle;
public:
    PCG_Detail() = default;
    template<typename T>
    PCG_Detail(std::string_view name, const T && value , PCG_AttributeTypeInfo info){
        auto att = PCG_Detail::createAttrib(name, std::forward<T>(value), info);
        dataHandle.emplace_back(std::move(att));
    }

    template<typename T>
    PCG_Detail(std::string_view name, T && value , PCG_AttributeTypeInfo info){
        auto att = PCG_Detail::createAttrib(name, std::forward<T>(value), info);
        dataHandle.emplace_back(std::move(att));
    }

    template<typename T>
    static auto createAttrib(std::string_view name, T && value, PCG_AttributeTypeInfo info){
        return std::make_tuple(std::string(name), std::make_any<T>(std::forward<T>(value)), info );
    }

    void appendAttrib(PCG_AttribValueType && attrib){
        dataHandle.emplace_back(std::forward<PCG_AttribValueType>(attrib ));
    }

    void removeAttrib(std::string_view queryName){
        dataHandle.erase(std::find_if(dataHandle.begin(), dataHandle.end(), [&](auto &&var)
        {
            auto name = std::get<std::string>(var);
            if(name == queryName) return true;
            return false;
        }), std::end(dataHandle));
    }

    // only change attribute type info
    void setAttribTypeInfo(std::string_view queryName,PCG_AttributeTypeInfo newInfo ){
        for(auto &&[name, value, info] : dataHandle) {
            if(name == queryName ) info = newInfo;
        }
    }

    template<typename T>
    void setAttribValue(std::string_view nameToSet, T && value){
        getAttribValue<T>(nameToSet) = value;
    }

    [[nodiscard]] auto attribNames() const{
        std::vector<std::string> ret;
        std::transform(dataHandle.begin(), dataHandle.end(), std::back_inserter(ret), [](auto &&attrib){
            return std::get<std::string> (attrib);
        });
        return ret;
    }

    // return ref of attrib handles
    auto & getAttribs(){
        return dataHandle;
    }
    // return const ref of attrib handles
    [[nodiscard]] const auto & getAttribs() const{
        return dataHandle;
    }

    auto getAttribType(std::string_view queryName){
        std::optional<PCG_AttributeTypeInfo> ret{};
        for(auto &&[name, any_var, ati] : dataHandle){
            if(name == queryName) ret =  ati;
        }
        return ret;
    }

    // for attrib that was existed
    bool hasAttrib(std::string_view queryName){
        return std::ranges::any_of(dataHandle.begin(), dataHandle.end(), [&](auto && attrib){
            auto && [name, any_var, ati_info] = attrib;
            return name == queryName;
        });
    }
    // overload method for attrib that was existed
    bool hasAttrib(const PCG_AttribValueType & rh){
        auto name = std::get<std::string>(rh);
        return hasAttrib(name);
    }


    // get attrib optional value
    template<typename T>
    auto getAttribValueOpt(std::string_view queryName) const{
        std::optional<T> ret;
        for(auto &&[name, any_var, ati] : dataHandle){
            if(name == queryName) ret = std::any_cast<T>(any_var);
        }
        return ret;
    }

    // return optional ref , auto &val = getAttribRefValueOpt()->get(); val=x
    template<typename T>
    auto getAttribValueOpt(std::string_view queryName){
        std::optional<std::reference_wrapper<T> > ret;
        for(auto && attrib : dataHandle){
            if(queryName != std::get<std::string>(attrib)) continue;
            auto &any_var = std::get<std::any>(attrib);
            ret = std::ref(std::any_cast<T&>(any_var));
        }
        return ret;
    }

    // return value unchecked
    template<typename T>
    T getAttribValue(std::string_view queryName) const{
        return getAttribValueOpt<T>(queryName).value();
    }
    // return ref value unchecked
    template<typename T>
    T &getAttribValue(std::string_view queryName){
        return getAttribValueOpt<T>(queryName)->get();
    }

    // get value by index
    template<class T>
    const T & getAttribValue(size_t index) const {
        auto &handle = dataHandle[index];
        auto &var = std::get<std::any>(handle);
        return std::any_cast<const T&>(var);
    }
    template<class T>
    T & getAttribValue(size_t index){
        return const_cast<T&>(std::as_const(*this).getAttribValue<T>(index));
    }

};

// print all attribute names
inline std::ostream &operator <<( std::ostream &os, const PCG_Detail &rh){
    os << "gdp:" << &rh <<" : ";
    std::vector<std::string> infos;
    for(auto &&[name, value, ati] : rh.getAttribs()){
        auto strType = magic_enum::enum_name(ati);
        auto ret= name + std::string(": ") + std::string(strType);
        infos.emplace_back(std::move(ret));
    }

    std::copy(infos.begin(),
              infos.end(),
              std::ostream_iterator<std::string>( os, "\t"));
    return os;
}


// helper method for functional programming
template<typename T>
inline decltype(auto) PCG_GetAttribValue( PCG_Detail &rh, std::string_view name){
    return rh.getAttribValue<T>(name);
}
template<typename T>
inline decltype(auto) PCG_GetAttribValue( const PCG_Detail &rh, std::string_view name){
    return rh.getAttribValue<T>(name);
}
template<typename T>
inline void PCG_SetAttribValue(PCG_Detail &rh, std::string_view name,  T && value){
    rh.setAttribValue(name, std::forward<T>(value) );
}
// create attribute, if value is custom user data, the ati of PCG_AttributeValueType is invalid,
// you can change the type  std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_INT;
template<typename T>
inline auto PCG_CreateAttrib(std::string_view name, T && value){
    using value_type = std::remove_cvref_t<T>;
    PCG_AttribValueType ret = PCG_Detail::createAttrib(name, static_cast<value_type>(value),
                                                       PCG_AttributeTypeInfo::P_ATI_INVALID);
    if constexpr(std::is_same_v<value_type, int>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_INT;
    }
    else if constexpr(std::is_same_v<value_type, float>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_FLT;
    }
    else if constexpr(std::is_same_v<value_type, double>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_DOUBLE;
    }
    else if constexpr(std::is_same_v<value_type, std::string>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_STR;
    }
    else if constexpr(std::is_same_v<value_type, glm::vec2>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_VEC2;
    }
    else if constexpr(std::is_same_v<value_type, glm::vec3>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_VEC3;
    }
    else if constexpr(std::is_same_v<value_type, glm::vec4>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_VEC4;
    }
    else if constexpr(std::is_same_v<value_type, glm::mat2>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_MAT22;
    }
    else if constexpr(std::is_same_v<value_type, glm::mat3>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_MAT33;
    }
    else if constexpr(std::is_same_v<value_type, glm::mat4>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_MAT44;
    }
    else{
    }
    return ret;
}
#endif //NODE_AND_ITEMS_PCG_DETAIL_HPP
