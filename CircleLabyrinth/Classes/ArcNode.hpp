//
//  ArcNode.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 15.01.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_ArcNode_hpp
#define CircleLabyrinth_ArcNode_hpp

#include "cocos2d.h"
#include "Shape.hpp"

using namespace cocos2d;

class ArcNode : public CCNode {
public:
    ArcNode(const ArcParams &params);
    void draw();
private:
    DrawableArc circle;
};

#endif
