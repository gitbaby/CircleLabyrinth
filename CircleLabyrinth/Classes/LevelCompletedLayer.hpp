//
//  LevelCompletedLayer.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 03.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_LevelCompletedLayer_hpp
#define CircleLabyrinth_LevelCompletedLayer_hpp

#include "Scene.hpp"

class LevelCompletedLayer : public CCLayerColor {
public:
    bool init();
    LAYER_NODE_FUNC(LevelCompletedLayer);
    void update(const float seconds, const float secondsLeft);
    void ccTouchesEnded(CCSet* touches, CCEvent* event);
private:
    CCLabelBMFont *resultsLabel;
};

SCENE_CLASS(LevelCompleted)

#endif
