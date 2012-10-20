//
//  MazeNode.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 07.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#include "MazeNode.hpp"

MazeNode::MazeNode(const MazeParams &params)
{
    maze.setParams(params);
    maze.evaluate();
}

void MazeNode::draw()
{
    maze.draw();
}
