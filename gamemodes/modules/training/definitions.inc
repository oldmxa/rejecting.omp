#if defined _training_definitions_included
    #endinput
#endif
#define _training_definitions_included

// Defines, enums and constants training system

forward Ach_OnTrainingChallengeFinish(playerid, Float:damage, Float:accuracy, recordType);

#define TRAINING_VW_BASE            (50000)
#define TRAINING_MAX_ARENAS         (32)
#define TRAINING_MAX_WEAPONS        (4)
#define TRAINING_NAME_LEN           (32)

#define TRAINING_TARGET_MODEL       (19059) // XmasOrb1
#define TRAINING_TARGET_HP          (20000.0)

#define TRAINING_TARGET_RADIUS_SCALE (1.05)
#define TRAINING_TARGET_RADIUS_MIN   (0.35) // floor, for models CA reports tiny
#define TRAINING_TARGET_RADIUS_FALL  (0.60) // used when CA cannot size the model

#define TRAINING_LAG_PING_CAP        (300)  // ms, ignores ping spikes above this
#define TRAINING_MAX_DISTANCE        (15.0) // max distance player can move from spawn point before challenge is cancelled

// hit feedback
#define TRAINING_HIT_SOUND          (17802)

// damage per weapon, mirroring the arena balance used in OnPlayerDamage
#define TRAINING_DMG_DEFAULT        (10.0)
#define TRAINING_DMG_DEAGLE         (35.0)
#define TRAINING_DMG_SHOTGUN        (15.0)
#define TRAINING_DMG_M4             (10.0)
#define TRAINING_DMG_SNIPER         (48.0)

#define TRAINING_RESPAWN_DELAY      (400)

#define TRAINING_CHALLENGE_TIME     (60)
#define TRAINING_CHALLENGE_TICK     (250)

#define TRAINING_PAUSE_TIMEOUT      (3)

// challenge result codes
#define TRAINING_RECORD_NONE        (0)
#define TRAINING_RECORD_PERSONAL    (1)
#define TRAINING_RECORD_SERVER      (2)

#define TRAINING_MAX_WAYPOINTS      (8)

enum E_TRAINING_ARENA
{
    tArena_ID,
    tArena_Name[TRAINING_NAME_LEN],
    tArena_Type[TRAINING_NAME_LEN],
    Float:tArena_SpawnX,
    Float:tArena_SpawnY,
    Float:tArena_SpawnZ,
    Float:tArena_SpawnA,
    Float:tArena_TargetStartX,
    Float:tArena_TargetStartY,
    Float:tArena_TargetStartZ,
    Float:tArena_TargetStartA,
    Float:tArena_TargetEndX,
    Float:tArena_TargetEndY,
    Float:tArena_TargetEndZ,
    Float:tArena_TargetEndA,
    Float:tArena_TargetSpeed,
    tArena_TargetModel,
    Float:tArena_TargetHP,
    Float:tArena_TargetRadius, // resolved from the model on first use

    tArena_Weapons[TRAINING_MAX_WEAPONS],
    tArena_WeaponCount,

    Float:tArena_WaypointX[TRAINING_MAX_WAYPOINTS],
    Float:tArena_WaypointY[TRAINING_MAX_WAYPOINTS],
    Float:tArena_WaypointZ[TRAINING_MAX_WAYPOINTS],
    tArena_WaypointCount,
    bool:tArena_IsRandomSpawning
}

enum E_PLAYER_TRAINING
{
    bool:pTrain_Active,
    pTrain_ArenaID,
    pTrain_TargetObject,
    Float:pTrain_TargetHealth,
    bool:pTrain_MovingToEnd,
    pTrain_CurrentWaypoint,
    pTrain_WaypointTimer,
    pTrain_Hits,
    pTrain_Shots,
    Float:pTrain_Damage,
    pTrain_CountdownTimer,
    bool:pTrain_ChallengeOn,
    pTrain_ChallengeEndTick,
    pTrain_ChallengeTimer,
    pTrain_SecondShown,
    bool:pTrain_IsOutOfBounds,
    pTrain_OutOfBoundsTime,
    pTrain_OutOfBoundsLastSec
}
