//
// Created by liuyangping on 2021/9/27.
//

#ifndef NODE_AND_ITEMS_PCG_NODE_H
#define NODE_AND_ITEMS_PCG_NODE_H

#include "PCG_NodePort.h"

struct PCG_Node {
    // ports
    PCG_NodePortArray inPorts;
    PCG_NodePortArray outPorts;


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
    void setInput(uint32_t fromIdx, const PCG_Node &fromNode, uint32_t toIdx) {
        auto &fromPort = fromNode.getOutPort(fromIdx);
        auto &toThisPort = getInPort(toIdx);
        toThisPort.setInPort(fromPort);
    }
};




#endif //NODE_AND_ITEMS_PCG_NODE_H
