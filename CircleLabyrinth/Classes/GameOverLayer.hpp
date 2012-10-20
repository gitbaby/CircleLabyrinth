//
//  GameOverLayer.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 04.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_GameOverLayer_hpp
#define CircleLabyrinth_GameOverLayer_hpp

#include "Scene.hpp"

class GameOverLayer : public CCLayerColor {
public:
    bool init();
    LAYER_NODE_FUNC(GameOverLayer);
private:
    void yesCallback(const CCObject *target) const;
    void noCallback(const CCObject *target) const;
};

SCENE_CLASS(GameOver)

#endif
