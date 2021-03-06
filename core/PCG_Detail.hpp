//
// Created by liuyangping on 2021/3/24.
//

#pragma once
#include <vector>
#include <string>
#include <any>
#include <tuple>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <optional>
#include <utility>
#include "magic_enum.hpp"
#include <glm/glm.hpp>

template<typename X>
using access_value_type  =  typename X::value_type;
template<typename X>
using access_iterator_of =  typename X::iterator;

template< class T >
concept is_range = requires(T t) {
    typename access_value_type<T>;    // T must have value_type
    typename access_iterator_of<T>;   // T must have iterator
    t.begin() ;
    t.end();
};

template <class T>
concept is_not_range = not is_range<T> or std::is_same_v<T, std::string>; // do not support string type




enum class PCG_AttributeTypeInfo{
    P_ATI_INVALID,

    P_ATI_FLT,
    P_ATI_INT,
    P_ATI_STR,
    P_ATI_DOUBLE,

    P_ATI_VEC2,
    P_ATI_VEC3,
    P_ATI_VEC4,
    P_ATI_COLOR_RGB,
    P_ATI_COLOR_RGBA,

    P_ATI_MAT22,
    P_ATI_MAT33,
    P_ATI_MAT44,

    P_ATI_ARRAY_INT,
    P_ATI_ARRAY_FLT,
    P_ATI_ARRAY_DOUBLE,
    P_ATI_ARRAY_STR,
    P_ATI_ARRAY_VEC2,
    P_ATI_ARRAY_VEC3,
    P_ATI_ARRAY_VEC4,
    P_ATI_ARRAY_MAT22,
    P_ATI_ARRAY_MAT33,
    P_ATI_ARRAY_MAT44,

    P_ATI_FILE,
    P_ATI_DOCUMENT,

    P_ATI_DROP_ARRAY_STRING,
    P_ATI_DROP_ARRAY_REAL,

    P_ATI_FUNCTION,

    // the image data should 0-255 std::vector<char>
    P_ATI_UCHAR_RGB_IAMGE,
    P_ATI_UCHAR_RGBA_IMAGE,

    // the image data should 0-1 std::vector<float>
    P_ATI_FLT_RGB_IMAGE,
    P_ATI_FLT_RGBA_IMAGE,

    // this is user data, data should maintain by user
    P_ATI_USER
};


//! @std::string name, @std::any valueHandle, @ati is only register for meta information, this handle attrib
using PCG_AttribValueType = std::tuple<std::string , std::any, PCG_AttributeTypeInfo >;

class PCG_AttribHandle: public PCG_AttribValueType{
public:
    using PCG_AttribValueType::PCG_AttribValueType;
    constexpr auto &getName(){
        return std::get<std::string>(*this);
    }
    [[nodiscard]] constexpr const auto &getName() const{
        return std::get<std::string>(*this);
    }
    // eval value no check!
    template<typename T>
    constexpr auto &getValue() const{
        auto &handle = *this;
        auto &var = std::get<std::any>(handle);
        return std::any_cast<const T&>(var);
    }
    template<typename T>
    constexpr auto &getValue(){
        return const_cast<T&>(std::as_const(*this).getValue<T>());
    }

    std::any & getDataHandle(){
        return std::get<std::any>(*this);
    }

    [[nodiscard]] const std::any & getDataHandle() const{
        return std::get<std::any>(*this);
    }

    template<typename T>
    void setValue(const T &value){
        auto &handle = std::get<std::any>(*this);
        auto &var = std::any_cast<T&>(handle);
        var = value;
    }
    template<typename T>
    void setValue(T &&value){
        auto &handle = std::get<std::any>(*this);
        auto &var = std::any_cast<T&>(handle);
        var = std::move(value);
    }
    [[nodiscard]] constexpr auto getInfo() const noexcept{
        return std::get<PCG_AttributeTypeInfo>(*this);
    }
};

// struct binding for PCG_AttribHandle
template<> struct std::tuple_size<PCG_AttribHandle>{
    static constexpr int value = 3;
};
template<> struct std::tuple_element<0, PCG_AttribHandle>{
    using type = std::string; // for name
};
template<> struct std::tuple_element<1, PCG_AttribHandle>{
    using type = std::any;  // for value
};
template<> struct std::tuple_element<2, PCG_AttribHandle>{
    using type = PCG_AttributeTypeInfo;
};

template<std::size_t idx> decltype(auto) get(const PCG_AttribHandle & attrib){
    static_assert(idx < 3);
    if constexpr( idx == 0) return attrib.getName();
    else if constexpr(idx == 1) return attrib.getDataHandle();
    else{
        return attrib.getInfo();
    }
}

template<std::size_t idx> decltype(auto) get(PCG_AttribHandle & attrib){
    static_assert(idx < 3);
    if constexpr( idx == 0) return attrib.getName();
    else if constexpr(idx == 1) return attrib.getDataHandle();
    else{
        return attrib.getInfo();
    }
}
template<std::size_t idx> decltype(auto) get(PCG_AttribHandle && attrib){
    static_assert(idx < 3);
    if constexpr( idx == 0) return std::move(attrib.getName());
    else if constexpr(idx == 1) return std::move(attrib.getDataHandle());
    else{
        return attrib.getInfo();
    }
}



// attrib value handle, it's a container
class PCG_Detail{
private:
    std::vector<PCG_AttribHandle> dataHandle;
public:
    PCG_Detail() = default;
    template<typename T>
    PCG_Detail(std::string_view name, const T & value , PCG_AttributeTypeInfo info){
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
        return PCG_AttribHandle{std::string(name), std::make_any<T>(std::forward<T>(value)), info };
    }
    template<typename T>
    static auto createAttrib(std::string_view name, const T & value, PCG_AttributeTypeInfo info){
        return PCG_AttribHandle{std::string(name), std::make_any<T>(value), info };
    }

    void appendAttrib(PCG_AttribHandle && attrib){
        dataHandle.emplace_back(std::forward<PCG_AttribHandle>(attrib ));
    }
    void appendAttrib(const PCG_AttribHandle & attrib){
        dataHandle.emplace_back(attrib );
    }

    void removeAttrib(std::string_view queryName){
        dataHandle.erase(std::find_if(dataHandle.begin(), dataHandle.end(), [&](auto &&var)
        {
            auto name = std::get<std::string>(var);
            if(name == queryName) return true;
            return false;
        }), std::end(dataHandle));
    }

    void clear(){
        dataHandle.clear();
    }

    [[nodiscard]] size_t numAttribs() const{
        return dataHandle.size();
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
    auto &getAttrib(size_t index) {
        return dataHandle[index];
    }
    [[nodiscard]] const auto & getAttrib(size_t index) const{
        return dataHandle[index];
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
    bool hasAttrib(const PCG_AttribHandle & rh){
        auto name = std::get<std::string>(rh);
        return hasAttrib(name);
    }

    [[nodiscard]] auto size() const{
        return dataHandle.size();
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

template<typename C>
class PCG_Simple_Iterator{
public:
    PCG_Simple_Iterator(C &rh, size_t idx): detail(rh), index(idx){}
    bool operator!= (PCG_Simple_Iterator const & other) const{
        return index != other.index;
    }
    PCG_Simple_Iterator const & operator++ (){
        ++index;
        return *this;
    }

    const PCG_AttribHandle & operator* () const {
        return detail.getAttrib(index);
    }

    PCG_AttribHandle & operator* (){
        return detail.getAttrib(index);
    }

private:
    C &detail;
    int index{};
};


// --------- helper method for functional programming -------------------
inline auto begin(PCG_Detail& detail){
    return PCG_Simple_Iterator<PCG_Detail>(detail, 0);
}

inline auto end(PCG_Detail& detail){
    return PCG_Simple_Iterator<PCG_Detail>(detail, detail.size());
}

inline auto begin(const PCG_Detail& detail){
    return PCG_Simple_Iterator<const PCG_Detail>(detail, 0);
}

inline auto end(const PCG_Detail& detail)  {
    return PCG_Simple_Iterator<const PCG_Detail>(detail, detail.size());
}

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

// create attribute for all attribute
template<typename T>
inline auto PCG_CreateAttrib(std::string_view name, T &&value, PCG_AttributeTypeInfo ati){
    using vt = std::remove_cvref_t<T>;
    return PCG_Detail::createAttrib(name, std::forward<vt>(value), ati);
}
template<typename T>
inline auto PCG_CreateAttrib(std::string_view name, const T &value, PCG_AttributeTypeInfo ati){
    using vt = std::remove_cvref_t<T>;
    return PCG_Detail::createAttrib(name, std::forward<vt>(value), ati);
}

// create attribute, if value is custom user data, the ati of PCG_AttributeValueType is invalid,
// you can change the type  std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_INT;
template<is_not_range T>
inline auto PCG_CreateAttrib(std::string_view name, T && value){
    using rm_ref_type = std::remove_cvref_t<T>;
    using decay_type = std::decay_t<T>;
    using value_type = std::conditional_t< std::is_same_v<decay_type, const char *>, std::string, rm_ref_type >;

    auto ret = PCG_Detail::createAttrib(name, static_cast<value_type>(value),
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
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_USER;
    }
    return ret;
}

// create array attribute
template<typename T>
inline auto PCG_CreateAttrib(std::string_view name, std::vector<T> && value){
    using rm_ref_type = std::remove_cvref_t<T>;
    using decay_type = std::decay_t<T>;
    using value_type = std::conditional_t< std::is_same_v<decay_type, const char *>, std::string, rm_ref_type >;

    auto ret = PCG_Detail::createAttrib(name, std::move(value),
                                        PCG_AttributeTypeInfo::P_ATI_INVALID);
    if constexpr(std::is_same_v<value_type, int>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_ARRAY_INT;
    }
    else if constexpr(std::is_same_v<value_type, float>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_ARRAY_FLT;
    }
    else if constexpr(std::is_same_v<value_type, double>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_ARRAY_DOUBLE;
    }
    else if constexpr(std::is_same_v<value_type, std::string>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_ARRAY_STR;
    }
    else if constexpr(std::is_same_v<value_type, glm::vec2>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_ARRAY_VEC2;
    }
    else if constexpr(std::is_same_v<value_type, glm::vec3>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_ARRAY_VEC3;
    }
    else if constexpr(std::is_same_v<value_type, glm::vec4>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_ARRAY_VEC4;
    }
    else if constexpr(std::is_same_v<value_type, glm::mat2>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_ARRAY_MAT22;
    }
    else if constexpr(std::is_same_v<value_type, glm::mat3>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_ARRAY_MAT33;
    }
    else if constexpr(std::is_same_v<value_type, glm::mat4>){
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_ARRAY_MAT44;
    }
    else{
        std::get<PCG_AttributeTypeInfo>(ret) = PCG_AttributeTypeInfo::P_ATI_USER;
    }
    return ret;
}

// append attribute to gdp
inline void PCG_AppendAttrib(PCG_Detail &detail ,PCG_AttribHandle & attrib){
    detail.appendAttrib(attrib);
}
inline void PCG_AppendAttrib(PCG_Detail &detail , PCG_AttribHandle && attrib){
    detail.appendAttrib(std::move(attrib));
}



