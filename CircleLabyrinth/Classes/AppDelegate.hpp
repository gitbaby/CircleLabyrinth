//
//  AppDelegate.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 09.12.11.
//  Copyright 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef  _APP_DELEGATE_HPP_
#define  _APP_DELEGATE_HPP_

#include "CCApplication.h"
#include "cocos2d.h"

/**
@brief	The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class AppDelegate : private cocos2d::CCApplication
{
public:
	AppDelegate();
	virtual ~AppDelegate();

    /**
    @brief	Implement for initialize OpenGL instance, set source path, etc...
    */
    virtual bool initInstance();

    /**
    @brief	Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
};

#endif
