//
//  ArcNode.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 15.01.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#include "ArcNode.hpp"

ArcNode::ArcNode(const ArcParams &params)
{
    circle.setParams(params);
    circle.evaluate();
}

void ArcNode::draw()
{
    circle.draw();
}
