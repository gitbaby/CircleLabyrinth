//
//  GameCenterManager.m
//  DefenderGame
//
//  Created by Oleksii Kozlov on 13.11.11.
//  Copyright (c) 2012 Oleksii Kozlov. All rights reserved.
//

#import "GameCenterManager.h"

@implementation GameCenterManager

@synthesize hasGameCenter;
@synthesize unsentScores;

SYNTHESIZE_SINGLETON_FOR_CLASS(GameCenterManager);

- (id)init 
{
    if ((self = [super init])) {
        hasGameCenter = [self isGameCenterAPIAvailable];
    }
    return self;
}

- (BOOL)isGameCenterAPIAvailable
{
    // Check for presence of GKLocalPlayer class
    BOOL localPlayerClassAvailable = (NSClassFromString(@"GKLocalPlayer")) != nil;

    // Device must be running 4.1 or later
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);

    return (localPlayerClassAvailable && osVersionSupported);
}

- (void)authenticateLocalPlayer
{
    if (hasGameCenter) {
        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        [localPlayer authenticateWithCompletionHandler:^(NSError *error) {
            if (localPlayer.isAuthenticated) {
                // If unsent scores array has length > 0, try to send saved scores
                if ([unsentScores count] > 0) {
                    // Create new array to help remove successfully sent scores
                    NSMutableArray *removedScores = [NSMutableArray array];
                    for (GKScore *score in unsentScores) {
                        [score reportScoreWithCompletionHandler:^(NSError *error) {
                            if (error != nil) {
                                // If there's an error reporting the score (again!), leave the score in the array
                            } else {
                                // If success, mark score for removal
                                [removedScores addObject:score];
                            }
                        }];
                    }
                    
                    // Remove successfully sent scores from stored array
                    [unsentScores removeObjectsInArray:removedScores];
                }
            }
        }];
    }
}

- (BOOL)isAuthenticated
{
    return hasGameCenter && [[GKLocalPlayer localPlayer] isAuthenticated];
}

- (void)reportScore:(int64_t)score forCategory:(NSString *)category
{
    GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:category] autorelease];
    scoreReporter.value = score;
    [scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
        if (error != nil) {
            // Handle reporting error here by adding object to a serializable array, to be sent again later
            [unsentScores addObject:scoreReporter];
        }
    }];
}

- (void)retrieveTopTenScoresForCategory:(NSString *)category
                  withCompletionHandler:(void (^)(NSArray *scores, NSError *error))completionHandler
{
    GKLeaderboard *leaderboardRequest = [[GKLeaderboard alloc] init];
    if (leaderboardRequest != nil) {
        leaderboardRequest.playerScope = GKLeaderboardPlayerScopeGlobal;
        leaderboardRequest.category = category;
        leaderboardRequest.timeScope = GKLeaderboardTimeScopeAllTime;
        leaderboardRequest.range = NSMakeRange(1,10);
        [leaderboardRequest loadScoresWithCompletionHandler:completionHandler];
        [leaderboardRequest release];
    }
}

#pragma mark -
#pragma mark Loading/Saving State

+ (void)loadState
{
    @synchronized([GameCenterManager class]) 
    {
        // just in case loadState is called before GameCenterManager inits
        if (!sharedGameCenterManager) [GameCenterManager sharedGameCenterManager];
        
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        NSString *file = [documentsDirectory stringByAppendingPathComponent:@"GameCenterManager.bin"];
        Boolean saveFileExists = [[NSFileManager defaultManager] fileExistsAtPath:file];
        
        if (saveFileExists) {
            // don't need to set the result to anything here since we're just getting initwithCoder to be called.
            // if you try to overwrite sharedGameCenterManager here, an assert will be thrown.
            [NSKeyedUnarchiver unarchiveObjectWithFile:file];
        }
    }
}

+ (void)saveState
{
    @synchronized([GameCenterManager class]) 
    {  
        GameCenterManager *state = [GameCenterManager sharedGameCenterManager];
        
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        NSString *saveFile = [documentsDirectory stringByAppendingPathComponent:@"GameCenterManager.bin"];
        [NSKeyedArchiver archiveRootObject:state toFile:saveFile];
    }
}

#pragma mark -
#pragma mark NSCoding Protocol Methods

- (void)encodeWithCoder:(NSCoder *)coder
{
    [coder encodeBool:self.hasGameCenter forKey:@"hasGameCenter"];
    [coder encodeObject:self.unsentScores forKey:@"unsentScores"];
}

- (id)initWithCoder:(NSCoder *)coder
{
    if ((self = [super init])) {
        self.hasGameCenter = [coder decodeBoolForKey:@"hasGameCenter"];
        self.unsentScores = [coder decodeObjectForKey:@"unsentScores"];
    }
    return self;
}

@end
