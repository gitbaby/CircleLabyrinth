//
//  Globals.cpp
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 03.03.12.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#include "Globals.hpp"
#include "AppDelegate.hpp"
#include <sstream>

#define LEVELS 1

/*
#define LEVELS 3
#define SUBLEVELS 7

static const LevelParams levelParams[][SUBLEVELS] = {
    {
        (LevelParams){ .circles = 3, .arcs = 1, .timeDrains = 0, .balls = 1 },
        (LevelParams){ .circles = 4, .arcs = 2, .timeDrains = 1, .balls = 1 },
        (LevelParams){ .circles = 4, .arcs = 2, .timeDrains = 0, .balls = 2 },
        (LevelParams){ .circles = 5, .arcs = 3, .timeDrains = 1, .balls = 2 },
        (LevelParams){ .circles = 5, .arcs = 4, .timeDrains = 1, .balls = 2 },
        (LevelParams){ .circles = 5, .arcs = 5, .timeDrains = 1, .balls = 2 },
        (LevelParams){ .circles = 5, .arcs = 6, .timeDrains = 2, .balls = 2 }
    },
    {
        (LevelParams){ .circles = 10, .arcs = 3, .timeDrains = 3, .balls = 2 },
        (LevelParams){ .circles = 10, .arcs = 4, .timeDrains = 3, .balls = 2 },
        (LevelParams){ .circles = 10, .arcs = 4, .timeDrains = 3, .balls = 2 },
        (LevelParams){ .circles = 10, .arcs = 5, .timeDrains = 3, .balls = 2 },
        (LevelParams){ .circles = 10, .arcs = 5, .timeDrains = 3, .balls = 2 },
        (LevelParams){ .circles = 10, .arcs = 6, .timeDrains = 3, .balls = 2 },
        (LevelParams){ .circles = 10, .arcs = 6, .timeDrains = 3, .balls = 3 }
    },
    {
        (LevelParams){ .circles = 15, .arcs = 3, .timeDrains = 4, .balls = 3 },
        (LevelParams){ .circles = 15, .arcs = 4, .timeDrains = 4, .balls = 3 },
        (LevelParams){ .circles = 15, .arcs = 4, .timeDrains = 4, .balls = 3 },
        (LevelParams){ .circles = 15, .arcs = 5, .timeDrains = 4, .balls = 3 },
        (LevelParams){ .circles = 15, .arcs = 5, .timeDrains = 4, .balls = 3 },
        (LevelParams){ .circles = 15, .arcs = 6, .timeDrains = 4, .balls = 3 },
        (LevelParams){ .circles = 15, .arcs = 6, .timeDrains = 4, .balls = 4 }
    }
};
*/

void Globals::init()
{
    gameStarted = CCUserDefault::sharedUserDefault()->getBoolForKey("gameStarted", false);
    level = CCUserDefault::sharedUserDefault()->getIntegerForKey("level", 0);
    levelAchieved = CCUserDefault::sharedUserDefault()->getIntegerForKey("levelAchieved", 0);
//    sublevel = CCUserDefault::sharedUserDefault()->getIntegerForKey("sublevel", 0);
    score = CCUserDefault::sharedUserDefault()->getIntegerForKey("score", 0);
    coins = CCUserDefault::sharedUserDefault()->getIntegerForKey("coins", 0);

    mainMenuScene = MainMenuScene::node();
    mainMenuScene->retain();
    mainMenuScene->getLayer()->update();
    newGameScene = NewGameScene::node();
    newGameScene->retain();
    levelStartScene = LevelStartScene::node();
    levelStartScene->retain();
    levelCompletedScene = LevelCompletedScene::node();
    levelCompletedScene->retain();
    gameOverScene = GameOverScene::node();
    gameOverScene->retain();
    abandonScene = AbandonScene::node();
    abandonScene->retain();
    gamePlayScene = GamePlayScene::node();
    gamePlayScene->retain();
}

void Globals::setGameStarted(const bool gameStarted)
{
    this->gameStarted = gameStarted;
    CCUserDefault::sharedUserDefault()->setBoolForKey("gameStarted", gameStarted);
}

void Globals::setLevel(const unsigned int level)
{
    this->level = level;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("level", level);
    setLevelAchieved(level);
//    setSublevel(0);
}

void Globals::setLevelAchieved(const unsigned int levelAchieved)
{
    if (this->levelAchieved < levelAchieved) {
        this->levelAchieved = levelAchieved;
        CCUserDefault::sharedUserDefault()->setIntegerForKey("levelAchieved", levelAchieved);
    }
}

/*
void Globals::setSublevel(const unsigned int sublevel)
{
    this->sublevel = sublevel;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("sublevel", sublevel);
}
*/

void Globals::setScore(const unsigned int score)
{
    this->score = score;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("score", score);
}

void Globals::setCoins(const unsigned int coins)
{
    this->coins = coins;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("coins", coins);
}

void Globals::addCoins(const unsigned int coins)
{
    this->coins += coins;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("coins", coins);
}

void Globals::loadLevelParams()
{
    stringstream ss;
    ss << "level" << level << ".plist";
    const char *path = CCFileUtils::fullPathFromRelativePath(ss.str().c_str());
    CCDictionary<std::string, CCObject*> *plist = CCFileUtils::dictionaryWithContentsOfFileThreadSafe(path);
    levelParams.circles = atoi(valueForKey("circles", plist));
    levelParams.arcs = atoi(valueForKey("arcs", plist));
    levelParams.timeDrains = atoi(valueForKey("timeDrains", plist));
    levelParams.balls = atoi(valueForKey("balls", plist));
	plist->release();
}

const unsigned int Globals::getNumCircles() const
{
//    return levelParams[level][sublevel].circles;
    return levelParams.circles;
}

const unsigned int Globals::getNumArcs() const
{
//    return levelParams[level][sublevel].arcs;
    return levelParams.arcs;
}

const unsigned int Globals::getNumTimeDrains() const
{
//    return levelParams[level][sublevel].timeDrains;
    return levelParams.timeDrains;
}

const unsigned int Globals::getNumBalls() const
{
//    return levelParams[level][sublevel].balls;
    return levelParams.balls;
}

void Globals::loadMainMenuScene() const
{
    mainMenuScene->getLayer()->update();
    CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInL::transitionWithDuration(0.5f, mainMenuScene));
}

void Globals::loadNewGameScene()
{
    if (levelAchieved > 0) {
        newGameScene->getLayer()->update();
        CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInR::transitionWithDuration(0.5f, newGameScene));
    } else {
        setLevel(0);
        loadLevelStartScene();
    }
}

void Globals::loadLevelStartScene()
{
    setGameStarted(true);
    levelStartScene->getLayer()->update();
    CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInR::transitionWithDuration(0.5f, levelStartScene));
}

void Globals::loadLevelCompletedScene(const float seconds, const float secondsLeft)
{
    levelCompletedScene->getLayer()->update(seconds, secondsLeft);
    /*if (sublevel < SUBLEVELS - 1) {
        setSublevel(sublevel + 1);
    } else*/ if (level < LEVELS - 1) {
        setLevel(level + 1);
    }
    CCDirector::sharedDirector()->replaceScene(CCTransitionRotoZoom::transitionWithDuration(2.0f, levelCompletedScene));
}

void Globals::loadGameOverScene() const
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionRotoZoom::transitionWithDuration(2.0f, gameOverScene));
}

void Globals::loadAbandonScene() const
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInR::transitionWithDuration(0.5f, abandonScene));
}

void Globals::loadGamePlayScene()
{
    loadLevelParams();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(1.0f, gamePlayScene));
}
