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
#include "magic_enum.hpp"
#include "PCG_Types.h"

using PCG_AttributeValueType = std::tuple<std::string , std::any, PCG_AttributeTypeInfo >;

class PCG_Detail{
private:
    std::vector<PCG_AttributeValueType> dataHandle;
public:
    template<typename T>
    static auto createAttribute(std::string_view name, T && value, PCG_AttributeTypeInfo info){
        return std::make_tuple(std::string(name), std::make_any<T>(std::forward<T>(value)), info );
    }

    void appendAttrib( PCG_AttributeValueType && attrib){
        dataHandle.emplace_back(std::forward<PCG_AttributeValueType>(attrib ));
    }

    void removeAttrib(std::string_view queryName){
        dataHandle.erase(std::find_if(dataHandle.begin(), dataHandle.end(), [&](auto &&var)
        {
            auto name = std::get<std::string>(var);
            if(name == queryName) return true;
            return false;
        }), std::end(dataHandle));
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

    bool hasAttrib(std::string_view queryName){
        return std::ranges::any_of(dataHandle.begin(), dataHandle.end(), [&](auto && attrib){
            auto && [name, any_var, ati_info] = attrib;
            return name == queryName;
        });
    }
    auto getAttribType(std::string_view queryName){
        std::optional<PCG_AttributeTypeInfo> ret{};
        for(auto &&[name, any_var, ati_info] : dataHandle){
            if(name == queryName) ret =  ati_info;
        }
        return ret;
    }

    // get attrib optional value
    template<typename T>
    auto getAttribValue(std::string_view queryName){
        std::optional<T> ret;
        for(auto &&[name, any_var, ati_info] : dataHandle){
            if(name == queryName) ret = std::any_cast<T>(any_var);
        }
        return ret;
    }

    // return optional ref
    template<typename T>
    auto getRefAttribValue(std::string_view queryName){
        std::optional<std::reference_wrapper<T> > ret;
        for(auto && attrib : dataHandle){
            if(queryName != std::get<std::string>(attrib)) continue;
            auto &any_var = std::get<std::any>(attrib);
            ret = std::ref(std::any_cast<T&>(any_var));
        }
        return ret;
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












#endif //NODE_AND_ITEMS_PCG_DETAIL_HPP
