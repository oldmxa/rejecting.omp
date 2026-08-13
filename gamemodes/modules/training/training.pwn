#if defined _training_module_included
    #endinput
#endif
#define _training_module_included

/*
    Main Entry
*/

#include <YSI_Coding/y_hooks>

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
hook OnGameModeInit()
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
    return 1;
}

hook OnPlayerConnect(playerid)
{
    g_TrainingTimerTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
    g_TrainingCountdownTD[playerid] = PlayerText:INVALID_TEXT_DRAW;

    ClearPlayerChallengeRecords(playerid);
    ResetPlayerTrainingData(playerid);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    EndTrainingSession(playerid);
    ClearPlayerChallengeRecords(playerid);
    return 1;
}

hook OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    if (g_PlayerTraining[playerid][pTrain_Active])
    {
        CancelTrainingChallenge(playerid, "you died");
    }
    return 1;
}

hook OnPlayerSpawn(playerid)
{
    if (g_PlayerTraining[playerid][pTrain_Active])
    {
        ApplyTrainingEnvironment(playerid);

        SetTimerEx("RestoreTrainingSpawn", TRAINING_RESPAWN_DELAY, false, "i", playerid);
    }
    return 1;
}

hook OnDynamicObjectMoved(STREAMER_TAG_OBJECT:objectid)
{
    HandleTrainingTargetMove(_:objectid);
    return 1;
}

hook OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    HandleTrainingWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
    return 1;
}

hook updatePlayerHud(playerid)
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
