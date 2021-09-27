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

    auto ret = attrib.getRefAttribValue<int>("Houdini");
    if(ret.has_value()){
        std::cout << ret->get() << std::endl;
        ret->get() = 4321; // change houdini attrib
    }
    std::cout << attrib.getAttribValue<int>("Houdini").value() << std::endl;
    std::cout << attrib << std::endl;

    PCG_Detail attrib2 = attrib;
    PCG_Detail attrib3 {std::move(attrib2)};

    return 0;
}
