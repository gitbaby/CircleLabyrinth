//
//  GamePlayLayer.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 09.12.11.
//  Copyright 2012 Oleksii Kozlov. All rights reserved.
//

#include "GamePlayLayer.hpp"
#include "Globals.hpp"
#include "SimpleAudioEngine.h"

bool GamePlayLayer::init()
{
    if (CCLayerColor::initWithColor(ccc4(0,0,0,255))) {

        setIsTouchEnabled(true);
        setIsAccelerometerEnabled(true);
    
        const CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
/*
	CCSpriteBatchNode *batchNode = CCSpriteBatchNode::batchNodeWithFile("sprites.png");
	addChild(batchNode, 1);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprites.plist");

    goldIndicator = CCSprite::spriteWithSpriteFrameName("coin1.png");
    CCSize goldIndicatorSize = goldIndicator->getContentSize();
    goldIndicator->setPosition(ccp(goldIndicatorSize.width/2+3.0f, screenSize.height-goldIndicatorSize.height/2-3.0f));
    batchNode->addChild(goldIndicator);
*/

/*
        const char *messageFont = "neon14.fnt";

        message = CCLabelBMFont::labelWithString("0", messageFont);
        message->setAnchorPoint(CCPoint(0.0f, 0.5f));
 */

        CCSprite *background = CCSprite::spriteWithFile("background.png");
        background->setPosition(CCPoint(screenSize.width/2, screenSize.height/2));
//        addChild(background);

        timer = new TimerSprite(TIME_LIMIT_SEC);
        timer->setPosition(CCPoint(screenSize.width/2, timer->getContentSize().height/2));
        timer->setIsVisible(false);
        addChild(timer);

        return true;
    }
    return false;
}

GamePlayLayer::~GamePlayLayer()
{
    for (vector<ArcNode *>::const_iterator ball = balls.begin(); ball != balls.end(); ++ball) {
        delete *ball;
    }

    for (vector<ArcNode *>::const_iterator timeDrain = timeDrains.begin(); timeDrain != timeDrains.end(); ++timeDrain) {
        delete *timeDrain;
    }

    delete timer;

	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
}

void GamePlayLayer::onEnter()
{
    CCLayerColor::onEnter();
    prepareScene();
}

void GamePlayLayer::onEnterTransitionDidFinish()
{
    CCLayerColor::onEnterTransitionDidFinish();
    startGame();
}

void GamePlayLayer::onExit()
{
    clearScene();
    CCLayerColor::onExit();
}

void GamePlayLayer::startGame()
{
    schedule(schedule_selector(GamePlayLayer::tick));
    timer->start();
}

void GamePlayLayer::stopGame()
{
    timer->stop();
    unscheduleAllSelectors();
}

void GamePlayLayer::levelCompleted()
{
    stopGame();
    Globals::sharedGlobals().loadLevelCompletedScene(timer->getSeconds(), timer->getSecondsLeft());
}

void GamePlayLayer::gameOver()
{
    stopGame();
    Globals::sharedGlobals().loadGameOverScene();
}

void GamePlayLayer::prepareScene()
{
    const float scale = CC_CONTENT_SCALE_FACTOR();

    const Globals &globals = Globals::sharedGlobals();
    const unsigned int nCircles = globals.getNumCircles();
    const float nArcs = globals.getNumArcs();
    const float nTimeDrains = globals.getNumTimeDrains();
    numBalls = globals.getNumBalls();

	const CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    const CCPoint screenCenter(screenSize.width/2, screenSize.height/2);

    const float radiusStep = min(screenSize.height,screenSize.width) / ((nCircles+1)*2+1);
    const float circleThickness = radiusStep/6;
    const float scaledCircleThickness = circleThickness * scale;
    const float firstCircleRadius = radiusStep * 3/2;
    targetRadius = firstCircleRadius - circleThickness * 3/2;
    ballRadius = radiusStep/2 - circleThickness;
    const float scaledBallRadius = ballRadius * scale;

    vector<float> radiuses(nCircles + 1);
    for (int iCircle = 0; iCircle < nCircles; ++iCircle) {
        radiuses[iCircle] = (firstCircleRadius + radiusStep * iCircle) * scale;
    }
    radiuses[nCircles] = (firstCircleRadius + radiusStep * nCircles) * scale;
    
    const MazeParams mazeParams = {
        .radiuses = radiuses,
        .thickness = scaledCircleThickness,
        .passWidth = scaledBallRadius * 2.4f,
        .nPasses = nArcs,
        .vertexDistance = 5.0f * scale,
        .offset = vec2(screenCenter.x * scale, screenCenter.y * scale)
    };

    physics.setParams(mazeParams);
    physics.evaluate();
    
    maze = new MazeNode(mazeParams);
    maze->setPosition(CCPoint(10,10));
    addChild(maze);

    const ArcParams timeDrainParams = {
        .nSectors = 16,
        .angle = TwoPi,
        .startAngle = 0,
        .edgeCenterDistances = makeVector((float[]){
            scaledBallRadius - scaledCircleThickness/2 * 3,
            scaledBallRadius - scaledCircleThickness,
            scaledBallRadius - scaledCircleThickness/2,
            scaledBallRadius,
            scaledBallRadius + scaledCircleThickness/2
        }),
        .edgeColors = makeVector((vec4[]){
            vec4(0,0,0,1),
            vec4(0.5f,0,0,1),
            vec4(1,0.8f,0.8f,1),
            vec4(0.5f,0,0,1),
            vec4(0,0,0,1)
        })
    };

    timeDrains.resize(nTimeDrains);
    for (int itd = 0, iCircle = 2; itd < nTimeDrains && iCircle < nCircles; ++itd, iCircle += nCircles/nTimeDrains) {
        ArcNode *timeDrain = new ArcNode(timeDrainParams);
        addChild(timeDrain);
        const float radius = radiusStep*(iCircle+1) - circleThickness/2;
        const float angle = (2*iCircle+1)*Pi/nArcs/2;
        const float x = screenCenter.x + radius * cosf(angle);
        const float y = screenCenter.y + radius * sinf(angle);
        timeDrain->setPosition(CCPoint(x, y));
        timeDrains[itd] = timeDrain;
    }

    const float outerRadius = radiusStep*(nCircles+1);
    const float ballDiameter = ballRadius*2;
    // According to the Law of cosines
    const float ballAngle = acos(1-ballDiameter*ballDiameter/2/outerRadius/outerRadius);

    const ArcParams ballParams = {
        .nSectors = 16,
        .angle = TwoPi,
        .startAngle = 0,
        .edgeCenterDistances = makeVector((float[]){
            0,
            scaledBallRadius/2,
            scaledBallRadius,
            scaledBallRadius*2,
            scaledBallRadius*6
        }),
        .edgeColors = makeVector((vec4[]){
            vec4(1,1,1,1),
            vec4(1,0.7f,1,0.5f),
            vec4(1,0,1,0.3f),
            vec4(1,0,1,0.2f),
            vec4(1,1,1,0)
        })
    };

    balls.resize(numBalls);
    for (int iBall = 0; iBall < numBalls; ++iBall) {
        ArcNode *ball = new ArcNode(ballParams);
        addChild(ball);
        const float angle = Pi * 3/2 + ballAngle * iBall;
        const float x = screenCenter.x + outerRadius * cosf(angle);
        const float y = screenCenter.y + outerRadius * sinf(angle);
        ball->setPosition(CCPoint(x, y));
        balls[iBall] = ball;
        physics.createBall(scaledBallRadius, vec2(x*scale, y*scale), ball);
    }

    timer->reset();
    timer->setIsVisible(true);
}

void GamePlayLayer::clearScene()
{
    removeChild(batchNode, true);

    physics.clear();

    removeChild(maze, true);
    delete maze;

    for (vector<ArcNode *>::const_iterator ball = balls.begin(); ball != balls.end(); ++ball) {
        removeChild(*ball, true);
        delete *ball;
    }
    balls.resize(0);

    for (vector<ArcNode *>::const_iterator timeDrain = timeDrains.begin(); timeDrain != timeDrains.end(); ++timeDrain) {
        removeChild(*timeDrain, true);
        delete *timeDrain;
    }
    timeDrains.resize(0);

    timer->setIsVisible(false);
}

void GamePlayLayer::draw()
{
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states:  GL_VERTEX_ARRAY, 
	// Unneeded states: GL_TEXTURE_2D, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY

    physics.debugDraw();

}

void GamePlayLayer::tick(ccTime dt)
{
	physics.tick(dt);

    if (timer->getSecondsLeft() > 0) {
        const CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        const CCPoint screenCenter(screenSize.width/2, screenSize.height/2);
        int ballsIn = 0;
        vector<ArcNode *>::const_iterator ball;
        for (ball = balls.begin(); ball != balls.end(); ++ball) {
            const CCPoint ballPosition = (*ball)->getPosition();
            // Check collisions with time drains
            vector<ArcNode *>::const_iterator timeDrain;
            for (timeDrain = timeDrains.begin(); timeDrain != timeDrains.end(); ++timeDrain) {
                const float distance = ccpDistance(ballPosition, (*timeDrain)->getPosition());
                if (distance <= ballRadius) {
                    timer->setPenalty(TIME_LIMIT_SEC/10);
                }
            }
            if (ccpDistance(screenCenter, ballPosition) <= targetRadius) ballsIn++;
        }
        if (ballsIn >= numBalls) {
            // All balls reached center of the screen
            levelCompleted();
        }
    } else {
        gameOver();
    }
    
    /*
     // Update labels only when they change for efficiency
     if (gold != previousGold) {
     char buffer[11];
     sprintf(buffer, "%i", gold);
     goldLabel->setString(buffer);
     previousGold = gold;
     }
     */
}

/*
void GamePlayLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	//Add a new body/atlas sprite at the touched location
	CCSetIterator it;
	CCTouch* touch;
    
	for( it = touches->begin(); it != touches->end(); it++) 
	{
		touch = (CCTouch*)(*it);
        
		if(!touch)
			break;
        
		CCPoint location = touch->locationInView(touch->view());
		
		location = CCDirector::sharedDirector()->convertToGL(location);
        
		addNewSpriteWithCoords( location );
	}
}
*/

void GamePlayLayer::didAccelerate(CCAcceleration *acceleration)
{
    physics.setAcceleration(acceleration);
}
