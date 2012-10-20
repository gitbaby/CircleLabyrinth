//
//  NewGameLayer.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 04.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#include "NewGameLayer.hpp"
#include "Globals.hpp"
#include "SimpleAudioEngine.h"

bool NewGameLayer::init()
{
    if (CCLayerColor::initWithColor(ccc4(0,0,0,255))) {
        
        const char *menuFont = "neon32.fnt";
        const char *iconsPng = "level-icons.png";
        const char *iconsPlist = "level-icons.plist";

        const CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

        CCLabelBMFont *selectLevelLabel = CCLabelBMFont::labelWithString("Select Level", menuFont);
        selectLevelLabel->setPosition(CCPoint(screenSize.width/2, screenSize.height/3));
        addChild(selectLevelLabel);

        CCSpriteBatchNode *batchNode = CCSpriteBatchNode::batchNodeWithFile(iconsPng);
        addChild(batchNode);
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(iconsPlist);

        CCSprite *tutorial = CCSprite::spriteWithSpriteFrameName("tutorial-icon.png");
        CCSprite *tutorialSelected = CCSprite::spriteWithSpriteFrameName("tutorial-icon.png");
        CCSprite *intermediate = CCSprite::spriteWithSpriteFrameName("intermediate-icon.png");
        CCSprite *intermediateSelected = CCSprite::spriteWithSpriteFrameName("intermediate-icon.png");
        CCSprite *intermediateDisabled = CCSprite::spriteWithSpriteFrameName("intermediate-locked-icon.png");
        CCSprite *advanced = CCSprite::spriteWithSpriteFrameName("advanced-icon.png");
        CCSprite *advancedSelected = CCSprite::spriteWithSpriteFrameName("advanced-icon.png");
        CCSprite *advancedDisabled = CCSprite::spriteWithSpriteFrameName("advanced-locked-icon.png");

        CCMenuItemSprite *tutorialItem = CCMenuItemSprite::itemFromNormalSprite(tutorial, tutorialSelected, this, menu_selector(NewGameLayer::tutorialCallback));
        intermediateItem = CCMenuItemSprite::itemFromNormalSprite(intermediate, intermediateSelected, intermediateDisabled, this, menu_selector(NewGameLayer::intermediateCallback));
        advancedItem = CCMenuItemSprite::itemFromNormalSprite(advanced, advancedSelected, advancedDisabled, this, menu_selector(NewGameLayer::advancedCallback));
        
        CCMenu *menu = CCMenu::menuWithItems(tutorialItem, intermediateItem, advancedItem, 0);
        
        tutorialItem->setPosition(CCPoint(0, 100));
        intermediateItem->setPosition(CCPoint(0, 0));
        advancedItem->setPosition(CCPoint(0, -100));
        
        addChild(menu);

        return true;
    }
    return false;
}

void NewGameLayer::update()
{
    const unsigned int levelAchieved = Globals::sharedGlobals().getLevelAchieved();
    if (levelAchieved > 0) {
        intermediateItem->setIsEnabled(true);
    } else {
        intermediateItem->setIsEnabled(false);
    }
    if (levelAchieved > 1) {
        advancedItem->setIsEnabled(true);
    } else {
        advancedItem->setIsEnabled(false);
    }
}

void NewGameLayer::tutorialCallback(const CCObject *target) const
{
    Globals &globals = Globals::sharedGlobals();
    globals.setLevel(0);
    globals.setScore(0);
}

void NewGameLayer::intermediateCallback(const CCObject *target) const
{
    Globals &globals = Globals::sharedGlobals();
    if (globals.getLevelAchieved() > 0) {
        globals.setLevel(1);
        globals.setScore(0);
    }
}

void NewGameLayer::advancedCallback(const CCObject *target) const
{
    Globals &globals = Globals::sharedGlobals();
    if (globals.getLevelAchieved() > 1) {
        globals.setLevel(2);
        globals.setScore(0);
    }
}
