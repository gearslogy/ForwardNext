//
// Created by star on 9/29/2021.
//

#ifndef NODE_AND_ITEMS_UNIT_TEST_NODE_CPP
#define NODE_AND_ITEMS_UNIT_TEST_NODE_CPP


#include <boost/test/unit_test.hpp>
#include "PCG_Node.hpp"

BOOST_AUTO_TEST_CASE( unit_test_node )
{

    PCG_Node node1;

    node1.appendOutPort(PCG_NodePort{PCG_PortPlugType::OUT, "out0"});

    PCG_Node node2;
    node2.appendInPort(PCG_NodePort{PCG_PortPlugType::IN, "in_master"});
    node2.appendOutPort(PCG_NodePort{PCG_PortPlugType::OUT, "out_master"});

    PCG_Node node3;
    node3.appendInPort(PCG_NodePort{PCG_PortPlugType::IN, "input"});

    node2.setInput(0, node1,0);
    node3.setInput(0, node2, 0);
    std::cout << "**************end of node test**************\n";
}

#endif //NODE_AND_ITEMS_UNIT_TEST_NODE_CPP
