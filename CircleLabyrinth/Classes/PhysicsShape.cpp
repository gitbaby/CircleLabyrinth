//
//  PhysicsShape.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 24.02.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#include "PhysicsShape.hpp"

void PhysicsArcShape::createVertices(vector<b2Vec2> &vertices) const
{
    const float stepAngle = params.angle/params.nSectors;
    mat2 rotationMatrices[2] = {mat2::CCWRotationMatrix(stepAngle), mat2::CWRotationMatrix(stepAngle)};
    const int nEdges = params.edgeCenterDistances.size();
    vector<vec2> edgeVertices(nEdges);
    for (int iEdge = 0; iEdge < nEdges; ++iEdge) {
        const float radius = params.edgeCenterDistances[iEdge];
        edgeVertices[iEdge] = vec2(radius * cosf(params.startAngle), radius * sinf(params.startAngle));
    }
    for (int iSection = 0; iSection < params.nSectors + 1; ++iSection) {
        for (int iEdge = 0; iEdge < nEdges; ++iEdge) {
            const vec2 vertex = offset + edgeVertices[iEdge];
            vertices.push_back(b2Vec2(vertex.x, vertex.y));
            edgeVertices[iEdge] = rotationMatrices[iEdge % 2] * edgeVertices[iEdge];
        }
    }
}
