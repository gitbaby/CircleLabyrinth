//
//  Shape.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 19.02.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_Shape_hpp
#define CircleLabyrinth_Shape_hpp

#include <OpenGLES/ES1/gl.h>
#include <vector>
#include "Matrix.hpp"

template <typename T, size_t N>
std::vector<T> makeVector(const T (&data)[N])
{
    return std::vector<T>(data, data+N);
}

using namespace std;

struct Shape {
    void setOffset(const vec2 &offset) { this->offset = offset; }
    virtual void createVertices(vector<vec2> &vertices, vector<vec4> &vertexColors, vector<GLushort> &vertexIndices) const = 0;
    vec2 offset;
};

struct ArcParams {
    void setVertexDistance(const float vertexDistance, const int onEdge = 0)
    {
        const int radius = edgeCenterDistances[onEdge];
        // According to the Law of cosines
        nSectors = angle/acos(1-vertexDistance*vertexDistance/2/radius/radius);
    }
    int nSectors;
    float angle;
    float startAngle;
    vector<float> edgeCenterDistances;
    vector<vec4> edgeColors;
};

struct ArcShape : Shape {
    ArcShape() { offset.x = 0; offset.y = 0; }
    ArcShape(const ArcParams &params) : params(params) { ArcShape::ArcShape(); }
    void setParams(const ArcParams &params) { this->params = params; }
    void createVertices(vector<vec2> &vertices, vector<vec4> &vertexColors, vector<GLushort> &vertexIndices) const;
    ArcParams params;
};

struct LineParams {
    vec2 start;
    vec2 end;
    vector<float> edgeCenterDistances;
    vector<vec4> edgeColors;
};

struct LineShape : Shape {
    LineShape() { offset.x = 0; offset.y = 0; }
    LineShape(const LineParams &params) : params(params) { LineShape::LineShape(); }
    void setParams(const LineParams &params) { this->params = params; }
    void createVertices(vector<vec2> &vertices, vector<vec4> &vertexColors, vector<GLushort> &vertexIndices) const;
    LineParams params;
};

class Drawable {
public:
    virtual void evaluate() = 0;
    void draw() const;
protected:
    vector<vec2> vertices;
    vector<vec4> vertexColors;
    vector<GLushort> vertexIndices;
};

class DrawableArc : public Drawable {
public:
    DrawableArc() {}
    void setParams(const ArcParams &params) { arc.setParams(params); }
    void evaluate();
private:
    ArcShape arc;
};

struct MazeParams {
    vector<float> radiuses;
    float thickness;
    float passWidth;
    int nPasses;
    float vertexDistance;
    vec2 offset;
};

class DrawableMaze : public Drawable {
public:
    DrawableMaze() {}
    void setParams(const MazeParams &params) { this->params = params; }
    void evaluate();
private:
    MazeParams params;
};

class DrawableHorizontalBar : public Drawable {
public:
    DrawableHorizontalBar() {
        leftCurve.params.nSectors = rightCurve.params.nSectors = 6;
        leftCurve.params.angle = rightCurve.params.angle = Pi;
        leftCurve.params.startAngle = Pi/2;
        rightCurve.params.startAngle = -Pi/2;
    }
    void setParams(const LineParams &params) {
        line.setParams(params);
        leftCurve.setOffset(params.start);
        rightCurve.setOffset(params.end);
        const vector<float> &edgeCenterDistances = params.edgeCenterDistances;
        leftCurve.params.edgeCenterDistances = rightCurve.params.edgeCenterDistances = edgeCenterDistances;
        const vector<vec4> &edgeColors = params.edgeColors;
        leftCurve.params.edgeColors = rightCurve.params.edgeColors = edgeColors;
    }
    void evaluate();
private:
    LineShape line;
    ArcShape leftCurve;
    ArcShape rightCurve;
};

#endif
