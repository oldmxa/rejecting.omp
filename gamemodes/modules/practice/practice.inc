#if defined _practice_module_incl
    #endinput
#endif
#define _practice_module_incl

#include "modules/practice/definitions.inc"
#include "modules/practice/core.inc"
#include "modules/practice/record.inc"
#include "modules/practice/npc.inc"
#include "modules/practice/dialogs.inc"
#include "modules/practice/commands.inc"

public OnPlayerConnect(playerid)
{
    if (IsPlayerNPC(playerid))
    {
        #if defined PRAC_OnPlayerConnect
            return PRAC_OnPlayerConnect(playerid);
        #else
            return 1;
        #endif
    }

    ResetPracticeState(playerid);
    PracticeDropRecordFile(playerid);

    #if defined PRAC_OnPlayerConnect
        return PRAC_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect PRAC_OnPlayerConnect
#if defined PRAC_OnPlayerConnect
    forward PRAC_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    if (IsPlayerNPC(playerid))
    {
        new owner = PracticeNPCOwner(playerid);
        if (owner != INVALID_PLAYER_ID && g_Practice[owner][pracActive] && g_Practice[owner][pracTargetType] == PRACTICE_TYPE_NPC)
        {
            g_Practice[owner][pracNpcId] = INVALID_NPC_ID;
            g_PracticeNPCOwner[playerid] = INVALID_PLAYER_ID;
            SetTimerEx("OnPracticeNPCRespawn", 500, false, "i", owner);
        }

        #if defined PRAC_OnPlayerDisconnect
            return PRAC_OnPlayerDisconnect(playerid, reason);
        #else
            return 1;
        #endif
    }

    if (g_Practice[playerid][pracRecording]) StopRecordingPlayerData(playerid);

    PracticeDropRecord(playerid);
    DestroyPracticeTarget(playerid);
    ResetPracticeState(playerid);

    #if defined PRAC_OnPlayerDisconnect
        return PRAC_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect PRAC_OnPlayerDisconnect
#if defined PRAC_OnPlayerDisconnect
    forward PRAC_OnPlayerDisconnect(playerid, reason);
#endif

public OnGameModeExit()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        PracticeDropRecordFile(i);
        DestroyPracticeTarget(i);
    }

    #if defined PRAC_OnGameModeExit
        return PRAC_OnGameModeExit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else
    #define _ALS_OnGameModeExit
#endif
#define OnGameModeExit PRAC_OnGameModeExit
#if defined PRAC_OnGameModeExit
    forward PRAC_OnGameModeExit();
#endif

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    if (g_Practice[playerid][pracActive])
        DestroyPracticeTarget(playerid);

    #if defined PRAC_OnPlayerDeath
        return PRAC_OnPlayerDeath(playerid, killerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath PRAC_OnPlayerDeath
#if defined PRAC_OnPlayerDeath
    forward PRAC_OnPlayerDeath(playerid, killerid, WEAPON:reason);
#endif

public OnPlayerSpawn(playerid)
{
    if (!g_Practice[playerid][pracActive])
    {
        #if defined PRAC_OnPlayerSpawn
            return PRAC_OnPlayerSpawn(playerid);
        #else
            return 1;
        #endif
    }

    if (!IsPlayerInLounge(playerid))
    {
        EndPracticeSession(playerid);
        #if defined PRAC_OnPlayerSpawn
            return PRAC_OnPlayerSpawn(playerid);
        #else
            return 1;
        #endif
    }

    SetPlayerVirtualWorld(playerid, PRACTICE_VW_BASE + playerid);
    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 100.0);

    CreatePracticeTarget(playerid);

    #if defined PRAC_OnPlayerSpawn
        return PRAC_OnPlayerSpawn(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn PRAC_OnPlayerSpawn
#if defined PRAC_OnPlayerSpawn
    forward PRAC_OnPlayerSpawn(playerid);
#endif

public OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if (!PracticeValidateWorld(playerid))
    {
        #if defined PRAC_OnPlayerWeaponShot
            return PRAC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
        #else
            return 1;
        #endif
    }

    g_Practice[playerid][pracShots]++;
    g_Practice[playerid][pracShotId]++;

    HandlePracticeShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);

    #if defined PRAC_OnPlayerWeaponShot
        return PRAC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot PRAC_OnPlayerWeaponShot
#if defined PRAC_OnPlayerWeaponShot
    forward PRAC_OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

public OnPlayerEditDynamicObject(playerid, STREAMER_TAG_OBJECT:objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    if (g_Practice[playerid][pracActive] && g_Practice[playerid][pracTargetType] == PRACTICE_TYPE_BALL && objectid == g_Practice[playerid][pracObject])
    {
        if (response == _:EDIT_RESPONSE_FINAL)
        {
            new Float:px, Float:py, Float:pz;
            GetPlayerPos(playerid, px, py, pz);

            new Float:baseZ = g_Practice[playerid][pracHasPos1] ? g_Practice[playerid][pracPos1][2] : pz;
            new Float:newHeight = z - baseZ - PRACTICE_BALL_LIFT;

            if (newHeight < PRACTICE_HEIGHT_MIN) newHeight = PRACTICE_HEIGHT_MIN;
            if (newHeight > PRACTICE_HEIGHT_MAX) newHeight = PRACTICE_HEIGHT_MAX;

            g_Practice[playerid][pracHeight] = newHeight;
            CreatePracticeTarget(playerid);

            va_SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Target height saved: {MAIN}%.2f", newHeight);
            ShowPracticeDialog(playerid);
        }
        else if (response == _:EDIT_RESPONSE_CANCEL)
        {
            CreatePracticeTarget(playerid);
            SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}3D editing canceled.");
            ShowPracticeDialog(playerid);
        }
        return 1;
    }

    #if defined PRAC_OnPlayerEditDynamicObject
        return PRAC_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerEditDynamicObject
    #undef OnPlayerEditDynamicObject
#else
    #define _ALS_OnPlayerEditDynamicObject
#endif
#define OnPlayerEditDynamicObject PRAC_OnPlayerEditDynamicObject
#if defined PRAC_OnPlayerEditDynamicObject
    forward PRAC_OnPlayerEditDynamicObject(playerid, STREAMER_TAG_OBJECT:objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);
#endif

