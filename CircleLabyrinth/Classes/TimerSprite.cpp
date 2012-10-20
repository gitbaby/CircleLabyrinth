//
//  TimerSprite.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 28.02.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#include "TimerSprite.hpp"
#include "SimpleAudioEngine.h"

TimerSprite::TimerSprite(const float seconds) : seconds(seconds), secondsLeft(seconds), on(true), penalty(false)
{
    const float scale = CC_CONTENT_SCALE_FACTOR();
	const CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    width = screenSize.width*3/5;

    const LineParams backgroundParams = {
        .start = vec2(-width/2*scale, 0),
        .end = vec2(width/2*scale, 0),
        .edgeCenterDistances = makeVector((float[]){scale, 3*scale + 1}),
        .edgeColors = makeVector((vec4[]){vec4(1,1,1,0), vec4(1,1,1,0.2f)})
    };
    background.setParams(backgroundParams);
    background.evaluate();

    timeBarParams = (LineParams){
        .start = vec2(-width/2*scale, 0),
        .end = vec2(width/2*scale, 0),
        .edgeCenterDistances = makeVector((float[]){0, 3*scale, 4*scale, 20*scale}),
        .edgeColors = makeVector((vec4[]){vec4(0,1,0,1), vec4(0,1,0,0.5f), vec4(0,1,0,0.2f), vec4(1,1,1,0)})
    };
    timeBar.setParams(timeBarParams);
    timeBar.evaluate();

    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("beep.caf");
}

const CCSize TimerSprite::getContentSize() const
{
    return CCSize(width, 40);
}

void TimerSprite::start()
{
    schedule(schedule_selector(TimerSprite::tick), 0.1f);
}

void TimerSprite::stop()
{
    unscheduleAllSelectors();
    on = true;
    penalty = false;
}

void TimerSprite::tick(ccTime dt)
{
    static float previousPercentage = 1;

    if (secondsLeft > 0) {
        secondsLeft -= dt;
        if (secondsLeft < 0) secondsLeft = 0;
        const float percentage = secondsLeft / seconds;
        const unsigned int currentWidth = width * percentage;
        if (currentWidth != int(width * previousPercentage)) {
            timeBarParams.end = vec2((-width/2 + currentWidth) * CC_CONTENT_SCALE_FACTOR(), 0);
            const float red = percentage <= 0.3f || penalty ? 1 : 0;
            const float green = percentage > 0.1f && !penalty ? 1 : 0;
            timeBarParams.edgeColors = makeVector((vec4[]){
                vec4(red, green, 0.6f, (currentWidth > 0 ? 1 : 0)),
                vec4(red, green, 0.6f, (currentWidth > 0 ? 0.5f : 0)),
                vec4(red, green, 0.6f, (currentWidth > 0 ? 0.2f : 0)),
                vec4(1,1,1,0)
            });
            timeBar.setParams(timeBarParams);
            timeBar.evaluate();
            if (percentage <= 0.3f && previousPercentage > 0.3f) {
                startBlinking(8);
            } else if (percentage <= 0.1f && previousPercentage > 0.1f) {
                startBlinking(8, true);
            }
            previousPercentage = percentage;
        }
    } else {
        stop();
    }
}

void TimerSprite::startBlinking(const unsigned int blinks, const bool beep)
{
    this->beep = beep;
    blinksLeft = blinks * 2 - 1;
    blink(0);
    schedule(schedule_selector(TimerSprite::blink), 0.1f);
}

void TimerSprite::stopBlinking()
{
    unschedule(schedule_selector(TimerSprite::blink));
    on = true;
    penalty = false;
}

void TimerSprite::blink(ccTime dt)
{
    if (blinksLeft > 0) {
        --blinksLeft;
        on = !on;
        if (beep && !on) CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("beep.caf");
    } else {
        stopBlinking();
    }
}

void TimerSprite::setPenalty(const float seconds)
{
    if (!penalty) {
        if (secondsLeft > seconds) {
            secondsLeft -= seconds;
            penalty = true;
            startBlinking(8);
        } else {
            secondsLeft = 0.1f;
        }
    }
}

void TimerSprite::reset()
{
    stop();
    secondsLeft = seconds;
}

void TimerSprite::draw()
{
    background.draw();
    if (on) timeBar.draw();
}
