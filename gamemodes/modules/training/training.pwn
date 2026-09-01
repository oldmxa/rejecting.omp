#if defined _training_module_included
    #endinput
#endif
#define _training_module_included

/*
    Main Entry
*/

#include "modules/training/definitions.inc"
#include "modules/training/variables.inc"
#include "modules/training/records.inc"
#include "modules/training/functions.inc"
#include "modules/training/dialogs.inc"
#include "modules/training/commands.inc"

// Arenas
#include "modules/training/arenas/skinhit_sf.inc"
#include "modules/training/arenas/skinhit_ls.inc"
#include "modules/training/arenas/skinhit_commerce.inc"
#include "modules/training/arenas/skinhit_marina.inc"
#include "modules/training/arenas/skinhit_lv.inc"
#include "modules/training/arenas/skinhit_ganton.inc"
#include "modules/training/arenas/reflex_bayside.inc"

// Hooks
public OnGameModeInit()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        g_TrainingTimerTD[i] = PlayerText:INVALID_TEXT_DRAW;
        g_TrainingCountdownTD[i] = PlayerText:INVALID_TEXT_DRAW;
    }

    Init_SkinhitSF();
    Init_SkinhitLS();
    Init_SkinhitCommerce();
    Init_SkinhitMarina();
    Init_SkinhitLV();
    Init_SkinhitGanton();
    Init_ReflexBayside();

    InitTrainingRecords();

    #if defined TRN_OnGameModeInit
        return TRN_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit TRN_OnGameModeInit
#if defined TRN_OnGameModeInit
    forward TRN_OnGameModeInit();
#endif

public OnPlayerConnect(playerid)
{
    g_TrainingTimerTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
    g_TrainingCountdownTD[playerid] = PlayerText:INVALID_TEXT_DRAW;

    ClearPlayerChallengeRecords(playerid);
    ResetPlayerTrainingData(playerid);

    #if defined TRN_OnPlayerConnect
        return TRN_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect TRN_OnPlayerConnect
#if defined TRN_OnPlayerConnect
    forward TRN_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    EndTrainingSession(playerid);
    ClearPlayerChallengeRecords(playerid);

    #if defined TRN_OnPlayerDisconnect
        return TRN_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect TRN_OnPlayerDisconnect
#if defined TRN_OnPlayerDisconnect
    forward TRN_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    if (g_PlayerTraining[playerid][pTrain_Active])
    {
        CancelTrainingChallenge(playerid, "you died");
    }

    #if defined TRN_OnPlayerDeath
        return TRN_OnPlayerDeath(playerid, killerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath TRN_OnPlayerDeath
#if defined TRN_OnPlayerDeath
    forward TRN_OnPlayerDeath(playerid, killerid, WEAPON:reason);
#endif

public OnPlayerSpawn(playerid)
{
    if (g_PlayerTraining[playerid][pTrain_Active])
    {
        ApplyTrainingEnvironment(playerid);

        SetTimerEx("RestoreTrainingSpawn", TRAINING_RESPAWN_DELAY, false, "i", playerid);
    }

    #if defined TRN_OnPlayerSpawn
        return TRN_OnPlayerSpawn(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn TRN_OnPlayerSpawn
#if defined TRN_OnPlayerSpawn
    forward TRN_OnPlayerSpawn(playerid);
#endif

public OnDynamicObjectMoved(STREAMER_TAG_OBJECT:objectid)
{
    HandleTrainingTargetMove(_:objectid);

    #if defined TRN_OnDynamicObjectMoved
        return TRN_OnDynamicObjectMoved(objectid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnDynamicObjectMoved
    #undef OnDynamicObjectMoved
#else
    #define _ALS_OnDynamicObjectMoved
#endif
#define OnDynamicObjectMoved TRN_OnDynamicObjectMoved
#if defined TRN_OnDynamicObjectMoved
    forward TRN_OnDynamicObjectMoved(STREAMER_TAG_OBJECT:objectid);
#endif

public OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    HandleTrainingWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);

    #if defined TRN_OnPlayerWeaponShot
        return TRN_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot TRN_OnPlayerWeaponShot
#if defined TRN_OnPlayerWeaponShot
    forward TRN_OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

forward CheckTrainingHudOnUpdate(playerid);
public CheckTrainingHudOnUpdate(playerid)
{
    if (g_PlayerTraining[playerid][pTrain_Active])
    {
        new secondsLeft = 0;
        if (g_PlayerTraining[playerid][pTrain_ChallengeOn])
        {
            new msLeft = GetTrainingMsLeft(playerid);
            if (msLeft > 0) secondsLeft = (msLeft + 999) / 1000;
        }
        else
        {
            secondsLeft = TRAINING_CHALLENGE_TIME;
        }
        UpdateTrainingHud(playerid, secondsLeft);
        UpdateChallengeTimerTD(playerid, secondsLeft);
    }
    return 1;
}
