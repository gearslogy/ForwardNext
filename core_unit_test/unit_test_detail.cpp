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

    PCG_Detail gdp;
    gdp.appendAttrib(PCG_Detail::createAttrib("Houdini", 5555, PCG_AttributeTypeInfo::P_ATI_INT));
    gdp.appendAttrib(PCG_Detail::createAttrib("Nuke", 6666, PCG_AttributeTypeInfo::P_ATI_INT));
    std::cout << gdp.hasAttrib("faster") << std::endl;

    // get by value
    auto ret = gdp.getAttribValue<int>("Houdini");
    std::cout << ret << std::endl;

    // get by ref
    auto &retRef = gdp.getAttribValue<int>("Houdini");
    retRef = 1000;
    std::cout << gdp.getAttribValue<int>("Houdini") << std::endl;

    // functional programming
    auto &refRef1 = PCG_GetAttribValue<int>(gdp, "Houdini") ;
    retRef = 2000;

    std::cout << PCG_GetAttribValue<int>(gdp, "Houdini") << std::endl;
    PCG_SetAttribValue<int>(gdp, "Houdini", 4321);
    std::cout << PCG_GetAttribValue<int>(gdp, "Houdini") << std::endl;

    // dump attrib
    std::cout << gdp << std::endl;

    // support range-loop
    for(auto &&r : gdp){
        r.getName() = std::string("xxx");
    }
    //
    for(auto &&r : gdp){
        std::cout << r.getName()<< std::endl;
    }
    gdp.clear();

    // functional create vec3 attribute
    auto N = PCG_CreateAttrib("N", glm::vec3{1,2,3}); // PCG_AttributeTypeInfo::P_ATI_VEC3
    gdp.appendAttrib(std::move(N));

    // function create the string attrib
    auto path = PCG_CreateAttrib("path", "/geometry/path");   // PCG_AttributeTypeInfo::P_ATI_STR
    gdp.appendAttrib(std::move(path));

    std::string stringVar{"hello world"};
    gdp.appendAttrib(PCG_CreateAttrib("path1", stringVar));


    // create vec3 attribute, but as P_ATI_RGB
    auto Cd = PCG_Detail::createAttrib("Cd", glm::vec3{1,2,3}, PCG_AttributeTypeInfo::P_ATI_COLOR_RGB);
    gdp.appendAttrib(std::move(Cd));

    // create function attribute
    std::function<int(int,int)> func = [](auto x, auto y){return x+y;};
    auto addFuncAttrib = PCG_Detail::createAttrib("add", std::move(func),  PCG_AttributeTypeInfo::P_ATI_FUNCTION);
    gdp.appendAttrib(std::move(addFuncAttrib));

    // retrive add function
    auto &refFunc = gdp.getAttribValue < std::function < int(int, int) > > ("add");
    auto evalRefFunc = refFunc(1,2);
    BOOST_CHECK_EQUAL(evalRefFunc, 3);
    BOOST_CHECK_EQUAL(gdp.numAttribs(), 5);

    // array test
    gdp.appendAttrib(PCG_CreateAttrib("array_float", std::vector<float>{1,2,3,4,5}));
    gdp.appendAttrib(PCG_CreateAttrib("array_int", std::vector<int>{1,2,3,4,5}));


    std::cout << gdp << std::endl;

}

#endif //NODE_AND_ITEMS_UNIT_TEST_NODE_CPP
