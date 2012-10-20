//
//  Shape.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 19.02.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#include "Shape.hpp"

void ArcShape::createVertices(vector<vec2> &vertices, vector<vec4> &vertexColors, vector<GLushort> &vertexIndices) const
{
    const mat2 rotationMatrix = mat2::CCWRotationMatrix(params.angle/params.nSectors);
    const int nSections = params.nSectors + 1;
    const int zeroEdge = params.edgeCenterDistances[0] > 0 ? 0 : 1;
    const int nEdges = params.edgeCenterDistances.size() - zeroEdge;
    const int centerVertex = vertices.size();
    const int zeroVertex = centerVertex + zeroEdge;
    if (zeroEdge > 0) {
        // Add center vertex
        vertices.push_back(offset);
        vertexColors.push_back(params.edgeColors[0]);
    }
    // Add arc vertices
    vector<vec2> edgeVertices(nEdges);
    for (int iEdge = 0; iEdge < nEdges; ++iEdge) {
        const float radius = params.edgeCenterDistances[iEdge + zeroEdge];
        edgeVertices[iEdge] = vec2(radius * cosf(params.startAngle), radius * sinf(params.startAngle));
    }
    for (int iSection = 0; iSection < nSections; ++iSection) {
        for (int iEdge = 0; iEdge < nEdges; ++iEdge) {
            vertices.push_back(offset + edgeVertices[iEdge]);
            vertexColors.push_back(params.edgeColors[iEdge + zeroEdge]);
            edgeVertices[iEdge] = rotationMatrix * edgeVertices[iEdge];
        }
    }
    // Make triangles
    if (zeroEdge > 0) {
        // Fill sector
        for (int iIndex = 0, iVertex = zeroVertex; iIndex < params.nSectors * 3; iIndex += 3) {
            vertexIndices.push_back(centerVertex);
            vertexIndices.push_back(iVertex);
            iVertex += nEdges;
            vertexIndices.push_back(iVertex);
        }
    }
    // Fill arc
    for (int iSection = 0; iSection < nSections - 1; ++iSection) {
        for (int iEdge = 0; iEdge < nEdges - 1; ++iEdge) {
            const int iVertex = zeroVertex + iSection * nEdges + iEdge;
            vertexIndices.push_back(iVertex);
            vertexIndices.push_back(iVertex + nEdges + 1);
            vertexIndices.push_back(iVertex + 1);
            vertexIndices.push_back(iVertex);
            vertexIndices.push_back(iVertex + nEdges + 1);
            vertexIndices.push_back(iVertex + nEdges);
        }
    }
}

void LineShape::createVertices(vector<vec2> &vertices, vector<vec4> &vertexColors, vector<GLushort> &vertexIndices) const
{
    const int zeroVertex = vertices.size();
    const int nEdges = params.edgeCenterDistances.size();
    for (int iEdge = 0; iEdge < nEdges; ++iEdge) {
        const vec2 start = offset + params.start;
        const vec2 end = offset + params.end;
        const vec2 edgeOffset = vec2(0, params.edgeCenterDistances[iEdge]);
        const vec4 color = params.edgeColors[iEdge];
        // Edge
        vertices.push_back(start + edgeOffset);
        vertexColors.push_back(color);
        vertices.push_back(end + edgeOffset);
        vertexColors.push_back(color);
        // Mirror edge
        vertices.push_back(start - edgeOffset);
        vertexColors.push_back(color);
        vertices.push_back(end - edgeOffset);
        vertexColors.push_back(color);
    }
    // Make triangles
    for (int iEdge = 0; iEdge < nEdges - 1; ++iEdge) {
        const int iVertex = zeroVertex + iEdge * 4;
        // Edge
        vertexIndices.push_back(iVertex);
        vertexIndices.push_back(iVertex + 5);
        vertexIndices.push_back(iVertex + 1);
        vertexIndices.push_back(iVertex);
        vertexIndices.push_back(iVertex + 5);
        vertexIndices.push_back(iVertex + 4);
        // Mirror edge
        vertexIndices.push_back(iVertex + 2);
        vertexIndices.push_back(iVertex + 7);
        vertexIndices.push_back(iVertex + 3);
        vertexIndices.push_back(iVertex + 2);
        vertexIndices.push_back(iVertex + 7);
        vertexIndices.push_back(iVertex + 6);
    }
}

void Drawable::draw() const
{
    glDisable(GL_TEXTURE_2D);
    //glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glVertexPointer(2, GL_FLOAT, 0, &vertices[0]);
    glColorPointer(4, GL_FLOAT, 0, &vertexColors[0]);
    //glDrawArrays(GL_POINTS, 0, vertices.size());
    glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_SHORT, &vertexIndices[0]);
    
    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
    
    // restore default GL states
    glEnable(GL_TEXTURE_2D);
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void DrawableArc::evaluate()
{
    vertices.resize(0);
    vertexColors.resize(0);
    vertexIndices.resize(0);
    arc.createVertices(vertices, vertexColors, vertexIndices);
}

void DrawableMaze::evaluate()
{
    vertices.resize(0);
    vertexColors.resize(0);
    vertexIndices.resize(0);

    const float thick = params.thickness;
    const float halfThick = thick / 2;

    const vector<vec4> arcColors = makeVector((vec4[]){
        vec4(0,0,0,1),
        vec4(0,0,0.5f,1),
        vec4(0.8f,0.8f,1,1),
        vec4(0,0,0.5f,1),
        vec4(0,0,0,1)
    });
    const vector<vec4> curveColors = makeVector((vec4[]){vec4(0.8f,0.8f,1,1), vec4(0,0,0.5f,1), vec4(0,0,0,1)});
    const vector<float> curveRadiuses = makeVector((float[]){0, halfThick, thick});

    ArcShape arcShape((ArcParams){
        .edgeColors = arcColors
    });
    arcShape.setOffset(params.offset);

    ArcShape leftCurveShape((ArcParams){
        .nSectors = 4,
        .angle = -Pi,
        .edgeCenterDistances = curveRadiuses,
        .edgeColors = curveColors
    });
    ArcShape rightCurveShape((ArcParams){
        .nSectors = 4,
        .angle = Pi,
        .edgeCenterDistances = curveRadiuses,
        .edgeColors = curveColors
    });

    for (int iCircle = 0; iCircle < params.radiuses.size() - 1; ++iCircle) {
        const float radius = params.radiuses[iCircle];
        const float innerRadius = radius - thick;
        
        // According to the Law of cosines
        const float passAngle = acos(1-params.passWidth*params.passWidth/2/innerRadius/innerRadius);
        
        const int nArcs = min(int(Pi/passAngle), params.nPasses);
        const float angle = TwoPi/nArcs - passAngle;

        arcShape.params.angle = angle;
        arcShape.params.edgeCenterDistances = makeVector((float[]){
            radius - halfThick*3,
            radius - thick,
            radius - halfThick,
            radius,
            radius + halfThick
        });
        arcShape.params.setVertexDistance(params.vertexDistance);

        float startAngle = iCircle*Pi/nArcs + passAngle/2;
        for (int iArc = 0; iArc < nArcs; ++iArc) {
            const int zeroVertex = vertices.size();
            arcShape.params.startAngle = startAngle;
            arcShape.createVertices(vertices, vertexColors, vertexIndices);
            const int lastVertex = vertices.size() - 1;
            leftCurveShape.params.startAngle = startAngle;
            leftCurveShape.setOffset(vertices[zeroVertex + 2]);
            leftCurveShape.createVertices(vertices, vertexColors, vertexIndices);
            rightCurveShape.params.startAngle = startAngle + angle;
            rightCurveShape.setOffset(vertices[lastVertex - 2]);
            rightCurveShape.createVertices(vertices, vertexColors, vertexIndices);
            startAngle += angle + passAngle;
        }
    }

    // Border circle
    const float borderRadius = params.radiuses.back();
    arcShape.params.angle = TwoPi;
    arcShape.params.startAngle = 0;
    arcShape.params.edgeCenterDistances = makeVector((float[]){
        borderRadius - halfThick*3,
        borderRadius - thick,
        borderRadius - halfThick,
        borderRadius,
        borderRadius + halfThick
    });
    arcShape.params.setVertexDistance(params.vertexDistance);
    arcShape.createVertices(vertices, vertexColors, vertexIndices);
}

void DrawableHorizontalBar::evaluate()
{
    vertices.resize(0);
    vertexColors.resize(0);
    vertexIndices.resize(0);
    line.createVertices(vertices, vertexColors, vertexIndices);
    leftCurve.createVertices(vertices, vertexColors, vertexIndices);
    rightCurve.createVertices(vertices, vertexColors, vertexIndices);
}
