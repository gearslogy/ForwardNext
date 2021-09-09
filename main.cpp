#include <iostream>
#include <vector>


#include "PCG_NodePort.h"
#include "PCG_Types.h"
#include "PCG_Detail.hpp"



struct Item {
    std::string name; // for identify
};

using Item_Array = std::vector<Item>;




struct PCG_Node {
    // ports
    PCG_NodePortArray inPorts;
    PCG_NodePortArray outPorts;

    // items handle
    Item_Array items;

    // malloc in ports
    void mallocInPorts(std::vector<PCG_NodePort> &&ports) { inPorts = std::forward<PCG_NodePortArray>(ports); }

    void appendInPort(PCG_NodePort &&port) { inPorts.emplace_back(port); }

    void mallocOutPorts(std::vector<PCG_NodePort> &&ports) { outPorts = std::forward<PCG_NodePortArray>(ports); }

    void appendOutPort(PCG_NodePort &&port) { outPorts.emplace_back(port); }

    PCG_NodePort &getInPort(size_t idx) {
        return inPorts[idx];
    }

    PCG_NodePort &getOutPort(size_t idx) {
        return outPorts[idx];
    }

    [[nodiscard]] const PCG_NodePort &getInPort(size_t idx) const {
        return inPorts[idx];
    }

    [[nodiscard]] const PCG_NodePort &getOutPort(size_t idx) const {
        return outPorts[idx];
    }

    // set input
    void setInputNode(uint32_t fromIdx, const PCG_Node &fromNode, uint32_t toIdx) {
        auto &fromPort = fromNode.getOutPort(fromIdx);
        auto &toThisPort = getInPort(toIdx);
        toThisPort.setInPort(fromPort);
    }
};




int main() {
    PCG_Node node1;
    node1.appendOutPort(PCG_NodePort{PCG_PortPlugType::OUT, "out0"});

    PCG_Node node2;
    node2.appendInPort(PCG_NodePort{PCG_PortPlugType::IN, "in_master"});
    node2.appendOutPort(PCG_NodePort{PCG_PortPlugType::OUT, "out_master"});

    PCG_Node node3;
    node3.appendInPort(PCG_NodePort{PCG_PortPlugType::IN, "input"});

    node2.setInputNode(0, node1,0);
    node3.setInputNode(0, node2, 0);

    PCG_Detail attrib;
    attrib.append(PCG_Detail::createAttribute("Houdini", 5555, PCG_AttributeTypeInfo::P_ATI_INT));
    attrib.append(PCG_Detail::createAttribute("Nuke", 6666, PCG_AttributeTypeInfo::P_ATI_INT));
    std::cout << attrib.hasAttrib("faster") << std::endl;

    auto ret = attrib.getRefAttribValue<int>("houdini");
    if(ret.has_value()){
        std::cout << ret->get() << std::endl;
        ret->get() = 4321; // change houdini attrib
    }
    std::cout << attrib.getAttribValue<int>("Houdini").value() << std::endl;

    return 0;
}
