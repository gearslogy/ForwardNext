//
// Created by father on 9/8/21.
//

#ifndef NODE_AND_ITEMS_PCG_TYPES_H
#define NODE_AND_ITEMS_PCG_TYPES_H


enum class PCG_AttributeTypeInfo{
    P_ATI_INVALID,

    P_ATI_FLT,
    P_ATI_INT,
    P_ATI_STR,
    P_ATI_DOUBLE,

    P_ATI_VEC2,
    P_ATI_VEC3,
    P_ATI_VEC4,
    P_ATI_COLOR_RGB,
    P_ATI_COLOR_RGBA,

    P_ATI_MAT22,
    P_ATI_MAT33,
    P_ATI_MAT44,

    P_ATI_ARRAY_FLT,
    P_ATI_ARRAY_STR,
    P_ATI_ARRAY_VEC,

    P_ATI_FILE,
    P_ATI_DOCUMENT,

    P_ATI_DROP_ARRAY_STRING,
    P_ATI_DROP_ARRAY_REAL,

    P_ATI_FUNCTION
};



enum class PCG_PortPlugType {
    IN = 0,
    OUT = 1
};



#endif //NODE_AND_ITEMS_PCG_TYPES_H
