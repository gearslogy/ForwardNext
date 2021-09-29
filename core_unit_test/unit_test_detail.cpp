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

    auto ret = attrib.getAttribValue<int>("Houdini");
    std::cout << ret << std::endl;

    auto &retRef = attrib.getAttribValue<int>("Houdini");
    retRef = 1000;
    std::cout << attrib.getAttribValue<int>("Houdini") << std::endl;

    auto &refRef1 = PCG_GetAttribValue<int>(attrib,"Houdini") ;
    retRef = 2000;

    std::cout << PCG_GetAttribValue<int>(attrib, "Houdini") << std::endl;
    PCG_SetAttribValue<int>(attrib, "Houdini", 4321);
    std::cout << PCG_GetAttribValue<int>(attrib, "Houdini") << std::endl;

    std::cout << attrib << std::endl;

    for(auto &&r : attrib){
        r.getName() = std::string("xxx");
    }

    for(auto &&r : attrib){
        std::cout << r.getName()<< std::endl;
    }
    std::cout << "**************end of PCG_Detail**************\n";
}

#endif //NODE_AND_ITEMS_UNIT_TEST_NODE_CPP
