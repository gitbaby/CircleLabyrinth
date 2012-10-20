//
//  MainMenuLayer.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 01.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#include "MainMenuLayer.hpp"
#include "Globals.hpp"
#include "SimpleAudioEngine.h"

bool MainMenuLayer::init()
{
    if (CCLayerColor::initWithColor(ccc4(0,0,0,255))) {

        const char *menuFont = "neon32.fnt";

        CCLabelBMFont *newGameLabel = CCLabelBMFont::labelWithString("New Game", menuFont);
        CCLabelBMFont *resumeLabel = CCLabelBMFont::labelWithString("Resume", menuFont);
        CCLabelBMFont *shopLabel = CCLabelBMFont::labelWithString("Buy Coins", menuFont);
        CCLabelBMFont *scoreLabel = CCLabelBMFont::labelWithString("High Scores", menuFont);
        CCLabelBMFont *optionsLabel = CCLabelBMFont::labelWithString("Options", menuFont);
        CCLabelBMFont *helpLabel = CCLabelBMFont::labelWithString("Help", menuFont);
        CCLabelBMFont *abandonLabel = CCLabelBMFont::labelWithString("Abandon", menuFont);
        abandonLabelColor = abandonLabel->getColor();

        newGameItem = CCMenuItemLabel::itemWithLabel(newGameLabel, this, menu_selector(MainMenuLayer::newGameCallback));
        resumeItem = CCMenuItemLabel::itemWithLabel(resumeLabel, this, menu_selector(MainMenuLayer::resumeCallback));
        CCMenuItemLabel *shopItem = CCMenuItemLabel::itemWithLabel(shopLabel);
        CCMenuItemLabel *scoreItem = CCMenuItemLabel::itemWithLabel(scoreLabel);
        CCMenuItemLabel *optionsItem = CCMenuItemLabel::itemWithLabel(optionsLabel);
        CCMenuItemLabel *helpItem = CCMenuItemLabel::itemWithLabel(helpLabel);
        abandonItem = CCMenuItemLabel::itemWithLabel(abandonLabel, this, menu_selector(MainMenuLayer::abandonCallback));

        CCMenu *menu = CCMenu::menuWithItems(newGameItem, resumeItem, shopItem, scoreItem, optionsItem, helpItem, abandonItem, 0);

        newGameItem->setPosition(CCPoint(0, 100));
        resumeItem->setPosition(CCPoint(0, 100));
        shopItem->setPosition(CCPoint(0, 60));
        scoreItem->setPosition(CCPoint(0, 20));
        optionsItem->setPosition(CCPoint(0, -20));
        helpItem->setPosition(CCPoint(0, -60));
        abandonItem->setPosition(CCPoint(0, -100));

        addChild(menu);

        return true;
    }
    return false;
}

void MainMenuLayer::update()
{
    if (Globals::sharedGlobals().getGameStarted()) {
        newGameItem->setIsVisible(false);
        resumeItem->setIsVisible(true);
        ((CCLabelBMFont *)abandonItem->getLabel())->setColor(abandonLabelColor);
    } else {
        resumeItem->setIsVisible(false);
        newGameItem->setIsVisible(true);
        ((CCLabelBMFont *)abandonItem->getLabel())->setColor(ccc3(128, 128, 128));
    }
}

void MainMenuLayer::newGameCallback(const CCObject *target) const
{
    Globals::sharedGlobals().loadNewGameScene();
}

void MainMenuLayer::resumeCallback(const CCObject *target) const
{
    Globals::sharedGlobals().loadLevelStartScene();
}

void MainMenuLayer::abandonCallback(const CCObject *target) const
{
    const Globals &globals = Globals::sharedGlobals();
    if (globals.getGameStarted()) {
        globals.loadAbandonScene();
    }
}
