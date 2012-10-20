//
//  NewGameLayer.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 04.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_NewGameLayer_hpp
#define CircleLabyrinth_NewGameLayer_hpp

#include "Scene.hpp"

class NewGameLayer : public CCLayerColor {
public:
    bool init();
    LAYER_NODE_FUNC(NewGameLayer);
    void update();
private:
    void tutorialCallback(const CCObject *target) const;
    void intermediateCallback(const CCObject *target) const;
    void advancedCallback(const CCObject *target) const;
    CCMenuItemSprite *intermediateItem;
    CCMenuItemSprite *advancedItem;
};

SCENE_CLASS(NewGame)

#endif
