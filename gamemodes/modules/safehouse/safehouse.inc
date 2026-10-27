#if defined _safehouse_module_included
    #endinput
#endif
#define _safehouse_module_included

#include "modules/safehouse/config.inc"
#include "modules/safehouse/messages.inc"
#include "modules/safehouse/enums.inc"
#include "modules/safehouse/upgrades.inc"
#include "modules/safehouse/functions.inc"
#include "modules/safehouse/database.inc"
#include "modules/safehouse/dialogs.inc"
#include "modules/safehouse/commands.inc"

public OnGameModeInit()
{
    for (new i = 0; i < SHCFG_MAX; i++)
        g_SafehouseConfig[i] = g_SafehouseConfigDefault[i];

    for (new i = 0; i < MAX_SAFEHOUSES; i++)
    {
        SafehouseData[i][sh_Pickup] = STREAMER_TAG_PICKUP:0;
        SafehouseData[i][sh_MapIcon] = STREAMER_TAG_MAP_ICON:0;
        SafehouseData[i][sh_Label] = Text3D:INVALID_3DTEXT_ID;
        ResetSafehouse(i);
    }

    InitSafehouseSystem();
    SetTimer("SafehouseTick", SAFEHOUSE_TICK_INTERVAL, true);

    #if defined SH_OnGameModeInit
        return SH_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit SH_OnGameModeInit
#if defined SH_OnGameModeInit
    forward SH_OnGameModeInit();
#endif

public OnPlayerConnect(playerid)
{
    g_PlayerInSafehouse[playerid] = -1;
    g_PlayerSafehousePrompt[playerid] = -1;
    g_PlayerSafehouseEdit[playerid] = -1;
    g_PlayerSafehouseCfg[playerid] = -1;
    g_PlayerSafehouseEditField[playerid] = -1;
    g_SafehouseEditCount[playerid] = 0;

    #if defined SH_OnPlayerConnect
        return SH_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect SH_OnPlayerConnect
#if defined SH_OnPlayerConnect
    forward SH_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    g_PlayerInSafehouse[playerid] = -1;
    g_PlayerSafehousePrompt[playerid] = -1;
    g_PlayerSafehouseEdit[playerid] = -1;
    g_PlayerSafehouseCfg[playerid] = -1;
    g_PlayerSafehouseEditField[playerid] = -1;
    g_SafehouseEditCount[playerid] = 0;

    #if defined SH_OnPlayerDisconnect
        return SH_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect SH_OnPlayerDisconnect
#if defined SH_OnPlayerDisconnect
    forward SH_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerSpawn(playerid)
{
    new index = g_PlayerInSafehouse[playerid];
    if (IsValidSafehouseIndex(index) &&
        GetPlayerVirtualWorld(playerid) != SafehouseData[index][sh_VirtualWorld])
    {
        g_PlayerInSafehouse[playerid] = -1;
    }

    if (User[playerid][spawnSafehouse])
        SetTimerEx("RestoreSafehouseSpawn", SAFEHOUSE_SPAWN_DELAY, false, "i", playerid);

    #if defined SH_OnPlayerSpawn
        return SH_OnPlayerSpawn(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn SH_OnPlayerSpawn
#if defined SH_OnPlayerSpawn
    forward SH_OnPlayerSpawn(playerid);
#endif

public RestoreSafehouseSpawn(playerid)
{
    if (!IsPlayerConnected(playerid)) return 1;
    if (!User[playerid][spawnSafehouse]) return 1;
    if (!IsPlayerLogged(playerid) || !IsPlayerAFK(playerid)) return 1;
    if (User[playerid][locationId] != ARENA_AFK) return 1;
    if (GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE || IsPlayerInDuel2v2(playerid)) return 1;
    if (g_PlayerTraining[playerid][pTrain_Active] || g_Practice[playerid][pracActive]) return 1;
    if (IsValidSafehouseIndex(g_PlayerInSafehouse[playerid])) return 1;

    new home = GetPlayerSafehouse(playerid);
    if (IsValidSafehouseIndex(home))
        EnterSafehouse(playerid, home);
    else
    {
        User[playerid][spawnSafehouse] = false;
        UpdatePlayerData(playerid);
    }

    return 1;
}

static stock HandleSafehouseAltKey(playerid)
{
    if (!IsPlayerLogged(playerid) || IsPlayerInAnyVehicle(playerid)) return 0;

    new PLAYER_STATE:pState = GetPlayerState(playerid);
    if (pState == PLAYER_STATE_NONE || pState == PLAYER_STATE_WASTED || pState == PLAYER_STATE_SPECTATING)
        return 0;

    if (IsValidSafehouseIndex(g_PlayerInSafehouse[playerid]))
    {
        new index = g_PlayerInSafehouse[playerid];
        if (IsPlayerInRangeOfPoint(playerid, 2.0,
            SafehouseData[index][sh_IntX], SafehouseData[index][sh_IntY], SafehouseData[index][sh_IntZ]))
        {
            ExitSafehouse(playerid);
        }
        return 1;
    }

    if (IsPlayerInMatch(playerid, false) || IsPlayerFrozen(playerid) || IsPlayerSpectating(playerid))
        return 0;

    new index = GetSafehouseAtPosition(playerid);
    if (!IsValidSafehouseIndex(index)) return 0;

    if (SafehouseData[index][sh_Status] != SAFEHOUSE_STATUS_OWNED)
    {
        ShowSafehouseBuyDialog(playerid, index);
        return 1;
    }

    new bool:isMember = IsSafehouseMember(playerid, index);

    if (!isMember && SafehouseData[index][sh_Password][0] == '\0')
    {
        SendClientMessage(playerid, -1, "%s", SH_LOCKED);
        return 1;
    }

    if (IsPlayerInSafehouseCombat(playerid))
    {
        SendClientMessage(playerid, -1, "%s", SH_IN_COMBAT);
        return 1;
    }

    if (!isMember && SafehouseData[index][sh_Password][0] != '\0')
    {
        ShowSafehousePasswordPrompt(playerid, index);
        return 1;
    }

    EnterSafehouse(playerid, index);
    return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (PRESSED(KEY_SECONDARY_ATTACK))
        HandleSafehouseAltKey(playerid);

    #if defined SH_OnPlayerKeyStateChange
        return SH_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange SH_OnPlayerKeyStateChange
#if defined SH_OnPlayerKeyStateChange
    forward SH_OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys);
#endif
