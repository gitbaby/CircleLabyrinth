//
//  MainMenuLayer.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 01.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_MainMenuLayer_hpp
#define CircleLabyrinth_MainMenuLayer_hpp

#include "Scene.hpp"

class MainMenuLayer : public CCLayerColor {
public:
    bool init();
    LAYER_NODE_FUNC(MainMenuLayer);
    void update();
private:
    void newGameCallback(const CCObject *target) const;
    void resumeCallback(const CCObject *target) const;
    void abandonCallback(const CCObject *target) const;
    CCMenuItemLabel *newGameItem;
    CCMenuItemLabel *resumeItem;
    CCMenuItemLabel *abandonItem;
    ccColor3B abandonLabelColor;
};

SCENE_CLASS(MainMenu)

#endif
