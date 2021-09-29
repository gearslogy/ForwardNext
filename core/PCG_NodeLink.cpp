//
// Created by father on 9/8/21.
//

#include "PCG_NodeLink.h"


PCG_NodeLink::PCG_NodeLink(const PCG_NodePort &src, const PCG_NodePort &to): src{&src}, des{&to} {
}

void PCG_NodeLink::setTo(const PCG_NodePort &rh) {
    des = &rh;
}

void PCG_NodeLink::setFrom(const PCG_NodePort &rh) {
    src = &rh;
}