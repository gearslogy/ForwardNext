//
// Created by star on 9/29/2021.
//

//
// Created by star on 9/29/2021.
//

#ifndef NODE_AND_ITEMS_UNIT_TEST_NODE_CPP
#define NODE_AND_ITEMS_UNIT_TEST_NODE_CPP

#include "PCG_Detail.hpp"
#include <boost/test/unit_test.hpp>


#undef OUT
#undef IN

BOOST_AUTO_TEST_CASE( unit_test_detail )
{

    PCG_Detail attrib;
    attrib.appendAttrib(PCG_Detail::createAttrib("Houdini", 5555, PCG_AttributeTypeInfo::P_ATI_INT));
    attrib.appendAttrib(PCG_Detail::createAttrib("Nuke", 6666, PCG_AttributeTypeInfo::P_ATI_INT));
    std::cout << attrib.hasAttrib("faster") << std::endl;

    // get by value
    auto ret = attrib.getAttribValue<int>("Houdini");
    std::cout << ret << std::endl;

    // get by ref
    auto &retRef = attrib.getAttribValue<int>("Houdini");
    retRef = 1000;
    std::cout << attrib.getAttribValue<int>("Houdini") << std::endl;

    // functional programming
    auto &refRef1 = PCG_GetAttribValue<int>(attrib,"Houdini") ;
    retRef = 2000;

    std::cout << PCG_GetAttribValue<int>(attrib, "Houdini") << std::endl;
    PCG_SetAttribValue<int>(attrib, "Houdini", 4321);
    std::cout << PCG_GetAttribValue<int>(attrib, "Houdini") << std::endl;

    // dump attrib
    std::cout << attrib << std::endl;

    // support range-loop
    for(auto &&r : attrib){
        r.getName() = std::string("xxx");
    }
    //
    for(auto &&r : attrib){
        std::cout << r.getName()<< std::endl;
    }
    attrib.clear();

    // functional create vec3 attribute
    auto N = PCG_CreateAttrib("N", glm::vec3{1,2,3}); // PCG_AttributeTypeInfo::P_ATI_VEC3
    attrib.appendAttrib(std::move(N));

    // function create the string attrib
    auto path = PCG_CreateAttrib("path", "/geometry/path");   // PCG_AttributeTypeInfo::P_ATI_STR
    attrib.appendAttrib(std::move(path));

    // create vec3 attribute, but as P_ATI_RGB
    auto Cd = PCG_Detail::createAttrib("Cd", glm::vec3{1,2,3}, PCG_AttributeTypeInfo::P_ATI_COLOR_RGB);
    attrib.appendAttrib(std::move(Cd));

    // create function attribute
    std::function<int(int,int)> func = [](auto x, auto y){return x+y;};
    auto addFuncAttrib = PCG_Detail::createAttrib("add", std::move(func),  PCG_AttributeTypeInfo::P_ATI_FUNCTION);
    attrib.appendAttrib(std::move(addFuncAttrib));

    // retrive add function
    auto &refFunc = attrib.getAttribValue<std::function<int(int,int) > >("add");
    auto evalRefFunc = refFunc(1,2);
    BOOST_CHECK_EQUAL(evalRefFunc, 3);
    BOOST_CHECK_EQUAL(attrib.numAttribs(), 4);
    std::cout << attrib << std::endl;

}

#endif //NODE_AND_ITEMS_UNIT_TEST_NODE_CPP
