//
//  Globals.hpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 03.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#pragma once
#ifndef CircleLabyrinth_Globals_hpp
#define CircleLabyrinth_Globals_hpp

#include "cocos2d.h"
#include "MainMenuLayer.hpp"
#include "NewGameLayer.hpp"
#include "LevelStartLayer.hpp"
#include "LevelCompletedLayer.hpp"
#include "GameOverLayer.hpp"
#include "AbandonLayer.hpp"
#include "GamePlayLayer.hpp"

#define TIME_LIMIT_SEC 180

using namespace cocos2d;

struct LevelParams {
    unsigned int circles;
    unsigned int arcs;
    unsigned int timeDrains;
    unsigned int balls;
};

class Globals {
public:
    static Globals &sharedGlobals()
    {
        static Globals theSingleInstance;
        return theSingleInstance;
    }
    void init();
    const bool getGameStarted() const { return gameStarted; }
    void setGameStarted(const bool gameStarted);
    const unsigned int getLevel() const { return level; }
    void setLevel(const unsigned int level);
    const unsigned int getLevelAchieved() const { return levelAchieved; }
    void setLevelAchieved(const unsigned int levelAchieved);
/*
    const unsigned int getSublevel() const { return sublevel; }
    void setSublevel(const unsigned int sublevel);
*/
    const unsigned int getScore() const { return score; }
    void setScore(const unsigned int score);
    const unsigned int getCoins() const { return coins; }
    void setCoins(const unsigned int coins);
    void addCoins(const unsigned int coins);
    void loadLevelParams();
    const unsigned int getNumCircles() const;
    const unsigned int getNumArcs() const;
    const unsigned int getNumTimeDrains() const;
    const unsigned int getNumBalls() const;
    CCScene *getInitialScene() const { return mainMenuScene; }
    void loadMainMenuScene() const;
    void loadNewGameScene();
    void loadLevelStartScene();
    void loadLevelCompletedScene(const float seconds, const float secondsLeft);
    void loadGameOverScene() const;
    void loadAbandonScene() const;
    void loadGamePlayScene();
private:        
    Globals() {}
    Globals(Globals &root) {}
    Globals &operator=(Globals &root) { return root; }
	inline const char *valueForKey(const char *key, CCDictionary<string, CCObject *> *dict)
	{
		if (dict)
		{
			CCString *pString = (CCString *)dict->objectForKey(key);
			return pString != NULL ? pString->m_sString.c_str() : "";
		}
		return "";
	}
    bool gameStarted;
    unsigned int level;
    unsigned int levelAchieved;
    LevelParams levelParams;
//    unsigned int sublevel;
    unsigned int score;
    unsigned int coins;
    MainMenuScene *mainMenuScene;
    NewGameScene *newGameScene;
    LevelStartScene *levelStartScene;
    LevelCompletedScene *levelCompletedScene;
    GameOverScene *gameOverScene;
    AbandonScene *abandonScene;
    GamePlayScene *gamePlayScene;
};

#endif
