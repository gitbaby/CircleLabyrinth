//
//  Physics.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 23.02.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#include "Physics.hpp"

#define PTM_RATIO 32

Physics::Physics()
{
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);

	world = new b2World(gravity);
    world->SetAllowSleeping(false);    
	world->SetContinuousPhysics(true);
    
    m_debugDraw = new GLESDebugDraw(PTM_RATIO);
    world->SetDebugDraw(m_debugDraw);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    //flags += b2Draw::e_jointBit;
    //flags += b2Draw::e_aabbBit;
    //flags += b2Draw::e_pairBit;
    //flags += b2Draw::e_centerOfMassBit;
    m_debugDraw->SetFlags(flags);
}

Physics::~Physics()
{
	delete world;
	world = NULL;

	delete m_debugDraw;
    m_debugDraw = NULL;
}

void Physics::evaluate()
{
    const float thick = params.thickness / PTM_RATIO;
    const float halfThick = thick / 2;
    const float passWidth = params.passWidth / PTM_RATIO;

    PhysicsArcShape arcShape;
    vector<b2Vec2> vertices;

    for (int iCircle = 0; iCircle < params.radiuses.size() - 1; ++iCircle) {
        const float radius = params.radiuses[iCircle] / PTM_RATIO;
        const float innerRadius = radius - thick;
        
        // According to the Law of cosines
        const float passAngle = acos(1-passWidth*passWidth/2/innerRadius/innerRadius);
        
        const int nArcs = min(int(Pi/passAngle), params.nPasses);
        const float angle = TwoPi/nArcs - passAngle;
        
        arcShape.params.angle = angle;
        arcShape.params.edgeCenterDistances = makeVector((float[]){radius - halfThick});
        arcShape.params.setVertexDistance(params.vertexDistance / PTM_RATIO);
        
        float startAngle = iCircle*Pi/nArcs + passAngle/2;
        for (int iArc = 0; iArc < nArcs; ++iArc) {
            arcShape.params.startAngle = startAngle;
            arcShape.createVertices(vertices);
            createStaticBody(vertices, false, params.offset, NULL);
            vertices.resize(0);
            startAngle += angle + passAngle;
        }
    }
    
    // Border circle
    const float borderRadius = params.radiuses.back() / PTM_RATIO;
    ArcParams borderParams = {
        .angle = TwoPi,
        .startAngle = 0,
        .edgeCenterDistances = makeVector((float[]){borderRadius - halfThick})
    };
    borderParams.setVertexDistance(params.vertexDistance / PTM_RATIO, 0);
    arcShape.setParams(borderParams);
    arcShape.createVertices(vertices);
    createStaticBody(vertices, true, params.offset, NULL);
}

void Physics::createStaticBody(const vector<b2Vec2> &vertices, const bool loop, const vec2 &position, void *userData)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(params.offset.x/PTM_RATIO, params.offset.y/PTM_RATIO);
    b2Body *body = world->CreateBody(&bodyDef);
    b2ChainShape chainShape;
    if (loop) {
        chainShape.CreateLoop(&vertices[0], vertices.size());
    } else {
        chainShape.CreateChain(&vertices[0], vertices.size());
    }
    body->CreateFixture(&chainShape, 0.0f);
    body->SetUserData(userData);
    staticBodies.push_back(body);
}

void Physics::createBall(const float radius, const vec2 &position, void *userData)
{
    b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
	b2Body *body = world->CreateBody(&bodyDef);
    b2CircleShape shape;
    shape.m_radius = radius/PTM_RATIO;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;	
    fixtureDef.density = 7870.0f; // Iron
    fixtureDef.friction = 0.4f; // Less friction - harder
    fixtureDef.restitution = 0.4f; // Bouncy
	body->CreateFixture(&fixtureDef);
    body->SetUserData(userData);
    dynamicBodies.push_back(body);
}

void Physics::clear()
{
    staticBodies.resize(0);
    dynamicBodies.resize(0);
	for (b2Body* body = world->GetBodyList(); body; body = body->GetNext()) {
        world->DestroyBody(body);
    }
}

void Physics::setAcceleration(const CCAcceleration *acceleration)
{
    world->SetGravity(b2Vec2(acceleration->x * PTM_RATIO, acceleration->y * PTM_RATIO));
}

void Physics::tick(ccTime dt)
{
    const float scale = CC_CONTENT_SCALE_FACTOR();
    
	const int velocityIterations = 8;
	const int positionIterations = 1;
	world->Step(dt, velocityIterations, positionIterations);
	
    vector<b2Body *>::const_iterator body;
    for (body = staticBodies.begin(); body != staticBodies.end(); ++body) {
        CCNode *node = (CCNode *)(*body)->GetUserData();
		if (node != NULL) {
			// Synchronize body position and rotation with node
            const CCPoint &nodePosition = node->getPosition();
            (*body)->SetTransform(b2Vec2(nodePosition.x * scale/PTM_RATIO, nodePosition.y * scale/PTM_RATIO),
                                  -1 * CC_DEGREES_TO_RADIANS(node->getRotation()));
        }
    }
    for (body = dynamicBodies.begin(); body != dynamicBodies.end(); ++body) {
        CCNode *node = (CCNode *)(*body)->GetUserData();
		if (node != NULL) {
			// Synchronize node position and rotation with body
            const b2Vec2 &bodyPosition = (*body)->GetPosition();
            node->setPosition(CCPointMake(bodyPosition.x * PTM_RATIO/scale, bodyPosition.y * PTM_RATIO/scale));
			node->setRotation(-1 * CC_RADIANS_TO_DEGREES((*body)->GetAngle()));
        }
    }
}

void Physics::debugDraw() const
{
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	world->DrawDebugData();
	
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}
