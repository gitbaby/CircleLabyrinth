//
//  LevelStartLayer.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 03.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#include "LevelStartLayer.hpp"
#include "Globals.hpp"
#include "Shape.hpp"
#include "SimpleAudioEngine.h"

static const char *difficulties[] = { "Tutorial", "Intermediate", "Advanced" };

static const vector<const char *> tips = makeVector((const char *[]){
    "The more tilt you apply,\nthe faster the ball will roll.",
    "The faster you finish a level,\nthe more points you earn.",
    "Avoid Time Drains.",
    "Use + button to get additional time\nin exchange to the points.",
    "See the HELP section for more\ninformation how to play."
});

bool LevelStartLayer::init()
{
    if (CCLayerColor::initWithColor(ccc4(0,0,0,255))) {

        setIsTouchEnabled(true);

        const char *difficultyFont = "neon32.fnt";
        const char *tipFont = "neon14.fnt";

        const CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

        difficultyLabel = CCLabelBMFont::labelWithString("", difficultyFont);
        difficultyLabel->setPosition(CCPoint(screenSize.width/2, screenSize.height/2));
        addChild(difficultyLabel);
        
        tipLabel = CCLabelBMFont::labelWithString("", tipFont);
        tipLabel->setPosition(CCPoint(screenSize.width/2, screenSize.height/4));
        addChild(tipLabel);
        
        return true;
    }
    return false;
}

void LevelStartLayer::update()
{
    const unsigned int level = Globals::sharedGlobals().getLevel();
    const string difficulty = "Difficulty: " + string(difficulties[level]) + "\nTime Limit: 3:00";
    difficultyLabel->setString(difficulty.c_str());
    
/*
    const unsigned int sublevel = Globals::sharedGlobals().getSublevel();
    const unsigned int iTip = sublevel % tips.size();
    const string tip = "Tip: " + string(tips[iTip]);
    tipLabel->setString(tip.c_str());
*/
}

void LevelStartLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    Globals::sharedGlobals().loadGamePlayScene();
}
