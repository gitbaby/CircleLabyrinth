//
//  TimerSprite.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 28.02.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_TimerSprite_hpp
#define CircleLabyrinth_TimerSprite_hpp

#include "cocos2d.h"
#include "Shape.hpp"

using namespace cocos2d;

class TimerSprite : public CCSprite {
public:
    TimerSprite(const float seconds);
    const CCSize getContentSize() const;
    void start();
    void stop();
    void tick(ccTime dt);
    void startBlinking(const unsigned int blinks, const bool beep = false);
    void stopBlinking();
    void blink(ccTime dt);
    void setPenalty(const float seconds);
    void reset();
    const float getSeconds() const { return seconds; }
    const float getSecondsLeft() const { return secondsLeft; }
    void draw();
private:
    float width;
    float seconds;
    float secondsLeft;
    bool on;
    bool beep;
    bool penalty;
    unsigned int blinksLeft;
    DrawableHorizontalBar background;
    LineParams timeBarParams;
    DrawableHorizontalBar timeBar;
};

#endif
