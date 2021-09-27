#include <iostream>
#include <vector>



#include "PCG_Detail.hpp"
#include "PCG_Node.hpp"



int main() {
    PCG_Node node1;
    node1.appendOutPort(PCG_NodePort{PCG_PortPlugType::OUT, "out0"});

    PCG_Node node2;
    node2.appendInPort(PCG_NodePort{PCG_PortPlugType::IN, "in_master"});
    node2.appendOutPort(PCG_NodePort{PCG_PortPlugType::OUT, "out_master"});

    PCG_Node node3;
    node3.appendInPort(PCG_NodePort{PCG_PortPlugType::IN, "input"});

    node2.setInput(0, node1,0);
    node3.setInput(0, node2, 0);

    PCG_Detail attrib;
    attrib.appendAttrib(PCG_Detail::createAttribute("Houdini", 5555, PCG_AttributeTypeInfo::P_ATI_INT));
    attrib.appendAttrib(PCG_Detail::createAttribute("Nuke", 6666, PCG_AttributeTypeInfo::P_ATI_INT));
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
    return 0;
}
