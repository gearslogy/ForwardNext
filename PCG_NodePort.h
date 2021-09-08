//
// Created by father on 9/8/21.
//

#ifndef NODE_AND_ITEMS_PCG_NODEPORT_H
#define NODE_AND_ITEMS_PCG_NODEPORT_H

#include <string>
#include "PCG_Types.h"
#include <vector>
#include <stdexcept>
#include "PCG_NodeLink.h"


class PCG_NodePort {
private:
    std::string name;
    PCG_PortPlugType plugType;
    std::vector<PCG_NodeLink> links;
public:
    PCG_NodePort() = delete;
    explicit PCG_NodePort(PCG_PortPlugType type, std::string_view display) : plugType{type}, name{display} {}
    void setInPort(const PCG_NodePort &from) {
        if(plugType != PCG_PortPlugType::IN){
            throw std::runtime_error("only support type is IN");
        }
        links.clear();
        links.emplace_back(PCG_NodeLink{from, *this});
    }
};

using PCG_NodePortArray = std::vector<PCG_NodePort>;

#endif //NODE_AND_ITEMS_PCG_NODEPORT_H

