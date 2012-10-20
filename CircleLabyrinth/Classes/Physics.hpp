//
//  Physics.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 23.02.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_Physics_hpp
#define CircleLabyrinth_Physics_hpp

#include "cocos2d.h"
#include "PhysicsShape.hpp"
#include "GLES-Render.h"

using namespace cocos2d;

class Physics {
public:
    Physics();
    ~Physics();
    void setParams(const MazeParams &params) { this->params = params; }
    void evaluate();
    void createStaticBody(const vector<b2Vec2> &vertices, const bool loop, const vec2 &position, void *userData = NULL);
    void createBall(const float radius, const vec2 &position, void *userData = NULL);
    void clear();
    void setAcceleration(const CCAcceleration *acceleration);
    void tick(ccTime dt);
    void debugDraw() const;
private:
    b2World *world;
    MazeParams params;
    vector<b2Body *> staticBodies;
    vector<b2Body *> dynamicBodies;
    GLESDebugDraw *m_debugDraw;
};

#endif
