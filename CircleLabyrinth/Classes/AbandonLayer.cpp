//
//  AbandonLayer.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 04.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#include "AbandonLayer.hpp"
#include "Globals.hpp"
#include "SimpleAudioEngine.h"

bool AbandonLayer::init()
{
    if (CCLayerColor::initWithColor(ccc4(0,0,0,255))) {
        
        const char *menuFont = "neon32.fnt";
        
        const CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        
        CCLabelBMFont *abandonLabel = CCLabelBMFont::labelWithString("Abandon Game?", menuFont);
        abandonLabel->setPosition(CCPoint(screenSize.width/2, screenSize.height*2/3));
        addChild(abandonLabel);
        
        CCLabelBMFont *yesLabel = CCLabelBMFont::labelWithString("Yes", menuFont);
        CCLabelBMFont *noLabel = CCLabelBMFont::labelWithString("No", menuFont);
        
        CCMenuItemLabel *yesItem = CCMenuItemLabel::itemWithLabel(yesLabel, this, menu_selector(AbandonLayer::yesCallback));
        CCMenuItemLabel *noItem = CCMenuItemLabel::itemWithLabel(noLabel, this, menu_selector(AbandonLayer::noCallback));
        
        CCMenu *menu = CCMenu::menuWithItems(yesItem, noItem, 0);
        menu->setPosition(CCPoint(screenSize.width/2, screenSize.height/3));
        
        yesItem->setPosition(CCPoint(-50, 0));
        noItem->setPosition(CCPoint(50, 0));
        
        addChild(menu);
        
        return true;
    }
    return false;
}

void AbandonLayer::yesCallback(const CCObject *target) const
{
    Globals &globals = Globals::sharedGlobals();
    globals.setGameStarted(false);
    globals.loadMainMenuScene();
}

void AbandonLayer::noCallback(const CCObject *target) const
{
    Globals::sharedGlobals().loadMainMenuScene();
}
