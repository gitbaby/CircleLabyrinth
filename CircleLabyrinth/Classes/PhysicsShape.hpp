//
//  PhysicsShape.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 24.02.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_PhysicsShape_hpp
#define CircleLabyrinth_PhysicsShape_hpp

#include "Box2D.h"
#include "Shape.hpp"

struct PhysicsShape {
    virtual void createVertices(vector<b2Vec2> &vertices) const = 0;
};

struct PhysicsArcShape : PhysicsShape {
    PhysicsArcShape() { offset.x = 0; offset.y = 0; }
    PhysicsArcShape(const ArcParams &params) : params(params) { PhysicsArcShape::PhysicsArcShape(); }
    void setOffset(const vec2 &offset) { this->offset = offset; }
    void setParams(const ArcParams &params) { this->params = params; }
    void createVertices(vector<b2Vec2> &vertices) const;
    vec2 offset;
    ArcParams params;
};

#endif
