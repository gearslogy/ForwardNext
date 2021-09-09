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
        }), std::end(dataHandle));
    }


    auto attribNames(){
        std::vector<std::string> ret;
        std::transform(dataHandle.begin(), dataHandle.end(), std::back_inserter(ret), [](auto &&attrib){
            return std::get<std::string> (attrib);
        });
        return ret;
    }



    bool hasAttrib(std::string_view queryName){
        return std::ranges::any_of(dataHandle.begin(), dataHandle.end(), [&](auto && attrib){
            auto && [name, any_var, ati_info] = attrib;
            return name == queryName;
        });
    }

    // if attrib exist, return copy of attrib value, or return constructor of T()
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
        std::optional<std::reference_wrapper<T>> ret;
        for(auto && attrib : dataHandle){
            if(queryName != std::get<std::string>(attrib)) continue;
            auto &any_var = std::get<std::any>(attrib);
            ret = std::ref(std::any_cast<T&>(any_var));
        }
        return ret;
    }

};


inline std::ostream &operator <<( std::ostream &os, const PCG_Detail &&rh){

    return os;
}












#endif //NODE_AND_ITEMS_PCG_DETAIL_HPP