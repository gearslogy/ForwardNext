//
// Created by father on 9/10/21.
//

#ifndef NODE_AND_ITEMS_PCG_ITEM_H
#define NODE_AND_ITEMS_PCG_ITEM_H

#include <vector>
#include <string>

#include "PCG_Detail.hpp"

struct PCG_Item {
    uint32_t idx;
    std::string name; // for identify
    PCG_Detail gdp;
};

using Item_Array = std::vector<PCG_Item>;

#endif //NODE_AND_ITEMS_PCG_ITEM_H
