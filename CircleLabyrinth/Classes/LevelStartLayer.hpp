//
//  LevelStartLayer.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 03.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_LevelStartLayer_hpp
#define CircleLabyrinth_LevelStartLayer_hpp

#include "Scene.hpp"

class LevelStartLayer : public CCLayerColor {
public:
    bool init();
    LAYER_NODE_FUNC(LevelStartLayer);
    void update();
    void ccTouchesEnded(CCSet* touches, CCEvent* event);
private:
    CCLabelBMFont *difficultyLabel;
    CCLabelBMFont *tipLabel;
};

SCENE_CLASS(LevelStart)

#endif
