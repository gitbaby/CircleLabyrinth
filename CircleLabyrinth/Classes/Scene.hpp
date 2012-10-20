//
//  Scene.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 01.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_Scene_hpp
#define CircleLabyrinth_Scene_hpp

#include "cocos2d.h"

using namespace cocos2d;

#define SCENE_CLASS(Name)\
class Name##Scene : public CCScene {\
public:\
    Name##Scene() : layer(NULL) {}\
    ~Name##Scene()\
    {\
        if (layer != NULL) {\
            /*layer->release();*/\
            layer = NULL;\
        }\
    }\
    bool init()\
    {\
        if(CCScene::init()) {\
            layer = Name##Layer::node();\
            /*layer->retain();*/\
            addChild(layer);\
            return true;\
        }\
        return false;\
    }\
    SCENE_NODE_FUNC(Name##Scene);\
    CC_SYNTHESIZE_READONLY(Name##Layer*, layer, Layer);\
};

#endif
