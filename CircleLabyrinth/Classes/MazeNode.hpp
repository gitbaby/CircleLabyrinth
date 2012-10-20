//
//  MazeNode.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 07.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#ifndef CircleLabyrinth_MazeNode_hpp
#define CircleLabyrinth_MazeNode_hpp

#include "cocos2d.h"
#include "Shape.hpp"

using namespace cocos2d;

class MazeNode : public CCNode {
public:
    MazeNode(const MazeParams &params);
    void draw();
private:
    DrawableMaze maze;
};

#endif
