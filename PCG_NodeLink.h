//
// Created by father on 9/8/21.
//

#ifndef NODE_AND_ITEMS_PCG_NODELINK_H
#define NODE_AND_ITEMS_PCG_NODELINK_H

#include <vector>

class PCG_NodePort;
struct PCG_NodeLink {
    PCG_NodeLink(const PCG_NodePort &src, const PCG_NodePort&to);
    void setFrom(const PCG_NodePort &rh);
    void setTo(const PCG_NodePort &rh);

    const PCG_NodePort *src = nullptr;
    const PCG_NodePort *des = nullptr;
};

using PCG_NodeLink_Array = std::vector<PCG_NodePort>;


#endif //NODE_AND_ITEMS_PCG_NODELINK_H
