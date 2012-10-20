//
//  GamePlayLayer.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 09.12.11.
//  Copyright 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_GamePlayLayer_hpp
#define CircleLabyrinth_GamePlayLayer_hpp

#include "Scene.hpp"
#include "Physics.hpp"
#include "MazeNode.hpp"
#include "ArcNode.hpp"
#include "TimerSprite.hpp"

class GamePlayLayer : public CCLayerColor {
public:
    GamePlayLayer() : timer(NULL) {}
    ~GamePlayLayer();
    bool init();
    LAYER_NODE_FUNC(GamePlayLayer);
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    void startGame();
    void stopGame();
    void levelCompleted();
    void gameOver();
    void prepareScene();
    void clearScene();
    void draw();
//    virtual void ccTouchesEnded(CCSet *touches, CCEvent *event);
    void tick(ccTime dt);
    void didAccelerate(CCAcceleration *acceleration);
private:
    CCSpriteBatchNode *batchNode;
    Physics physics;
    float targetRadius;
    float ballRadius;
    MazeNode *maze;
    vector<ArcNode *> balls;
    vector<ArcNode *> timeDrains;
    TimerSprite *timer;
    unsigned int numBalls;
};

SCENE_CLASS(GamePlay)

#endif
