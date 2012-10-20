//
//  GameCenterManager.h
//  DefenderGame
//
//  Created by Oleksii Kozlov on 13.11.11.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#import <GameKit/GameKit.h>
#import "SynthesizeSingleton.h"

// Subclass our object from NSObject - allow it to be serialized
@interface GameCenterManager : NSObject <NSCoding>
{
	// Boolean that is set to true if device supports Game Center
	BOOL hasGameCenter;
	
	// An array that holds scores that couldn't be sent to Game Center (network timeout, etc.)
	NSMutableArray *unsentScores;
	
	// The view that shows the default Game Center leaderboards
	UIViewController *myViewController;
}

@property (readwrite) BOOL hasGameCenter;
@property (readwrite, retain) NSMutableArray *unsentScores;

// Time-saving singleton generator - see http://cocoawithlove.com/2008/11/singletons-appdelegates-and-top-level.html
SYNTHESIZE_SINGLETON_FOR_CLASS_HEADER(GameCenterManager);

- (BOOL)isGameCenterAPIAvailable;
- (void)authenticateLocalPlayer;
- (BOOL)isAuthenticated;
- (void)reportScore:(int64_t)score forCategory:(NSString *)category;
- (void)retrieveTopTenScoresForCategory:(NSString *)category
                  withCompletionHandler:(void (^)(NSArray *scores, NSError *error))completionHandler;

// Serialize/store the variables in this singleton
+ (void)loadState;
+ (void)saveState;

@end
