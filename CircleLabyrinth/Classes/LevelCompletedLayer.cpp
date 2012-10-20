//
//  LevelCompletedLayer.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 03.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#include "LevelCompletedLayer.hpp"
#include "Globals.hpp"
#include "SimpleAudioEngine.h"
#include <sstream>

bool LevelCompletedLayer::init()
{
    if (CCLayerColor::initWithColor(ccc4(0,0,0,255))) {
        
        setIsTouchEnabled(true);
        
        const char *resultsFont = "neon32.fnt";
        
        const CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

        CCLabelBMFont *levelCompletedLabel = CCLabelBMFont::labelWithString("LEVEL COMPLETED!", resultsFont);
        levelCompletedLabel->setPosition(CCPoint(screenSize.width/2, screenSize.height*3/4));
        addChild(levelCompletedLabel);

        resultsLabel = CCLabelBMFont::labelWithString("", resultsFont);
        resultsLabel->setPosition(CCPoint(screenSize.width/2, screenSize.height/2));
        addChild(resultsLabel);

        return true;
    }
    return false;
}

void LevelCompletedLayer::update(const float seconds, const float secondsLeft)
{
    Globals &globals = Globals::sharedGlobals();
    stringstream ss;

    const unsigned int finishTime = seconds - secondsLeft;
    const unsigned int finishMinutes = finishTime / 60;
    const unsigned int finishSeconds = finishTime % 60;
    ss << "Finish Time: " << finishMinutes << ':' << finishSeconds;

    const unsigned int level = globals.getLevel();
    const unsigned int levelFactor = pow(10, (float)level);
    const unsigned int baseScore = (globals.getNumCircles() + globals.getNumArcs() - 1 + globals.getNumTimeDrains()) * globals.getNumBalls() * 10 * levelFactor;
    ss << "\nBase Score: " << baseScore;

    const unsigned int timeBonus = secondsLeft * levelFactor;
    ss << "\nTime Bonus: " << timeBonus;

    const unsigned int levelScore = baseScore + timeBonus;
    ss << "\nLevel Score: " << levelScore;

    const unsigned int score = globals.getScore() + levelScore;
    globals.setScore(score);
    ss << "\nOverall Score: " << score;

    globals.addCoins(timeBonus);
    ss << "\nCoins Earned: " << timeBonus;

    resultsLabel->setString(ss.str().c_str());
}

void LevelCompletedLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    Globals::sharedGlobals().loadLevelStartScene();
}
