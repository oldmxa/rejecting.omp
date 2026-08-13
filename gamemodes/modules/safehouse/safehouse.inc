#if defined _safehouse_module_included
    #endinput
#endif
#define _safehouse_module_included

#include <YSI_Coding/y_hooks>

#include "modules/safehouse/config.inc"
#include "modules/safehouse/messages.inc"
#include "modules/safehouse/enums.inc"
#include "modules/safehouse/upgrades.inc"
#include "modules/safehouse/functions.inc"
#include "modules/safehouse/database.inc"
#include "modules/safehouse/dialogs.inc"
#include "modules/safehouse/commands.inc"

hook OnGameModeInit()
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
    return 1;
}

hook OnPlayerConnect(playerid)
{
    g_PlayerInSafehouse[playerid] = -1;
    g_PlayerSafehousePrompt[playerid] = -1;
    g_PlayerSafehouseEdit[playerid] = -1;
    g_PlayerSafehouseCfg[playerid] = -1;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    g_PlayerInSafehouse[playerid] = -1;
    g_PlayerSafehousePrompt[playerid] = -1;
    g_PlayerSafehouseEdit[playerid] = -1;
    return 1;
}

hook OnPlayerSpawn(playerid)
{
    new index = g_PlayerInSafehouse[playerid];
    if (IsValidSafehouseIndex(index) &&
        GetPlayerVirtualWorld(playerid) != SafehouseData[index][sh_VirtualWorld])
    {
        g_PlayerInSafehouse[playerid] = -1;
    }

    if (User[playerid][spawnSafehouse])
        SetTimerEx("RestoreSafehouseSpawn", SAFEHOUSE_SPAWN_DELAY, false, "i", playerid);

    return 1;
}

public RestoreSafehouseSpawn(playerid)
{
    if (!IsPlayerConnected(playerid)) return 1;
    if (!User[playerid][spawnSafehouse]) return 1;
    if (!User[playerid][isLogged] || !User[playerid][isAfk]) return 1;
    if (User[playerid][locationId] != ARENA_AFK) return 1;
    if (GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE) return 1;
    if (IsValidSafehouseIndex(g_PlayerInSafehouse[playerid])) return 1;

    new home = GetPlayerSafehouse(playerid);
    if (IsValidSafehouseIndex(home))
        EnterSafehouse(playerid, home);

    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!PRESSED(KEY_SECONDARY_ATTACK)) return 1;
    if (!User[playerid][isLogged]) return 1;

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

    new index = GetSafehouseAtPosition(playerid);
    if (!IsValidSafehouseIndex(index)) return 1;

    if (SafehouseData[index][sh_Status] != SAFEHOUSE_STATUS_OWNED)
        return ShowSafehouseBuyDialog(playerid, index);

    new bool:isMember = IsSafehouseMember(playerid, index);

    if (!isMember && SafehouseData[index][sh_Password][0] == '\0')
        return SendClientMessage(playerid, -1, "%s", SH_LOCKED);

    if (IsPlayerInSafehouseCombat(playerid))
        return SendClientMessage(playerid, -1, "%s", SH_IN_COMBAT);

    if (!isMember && SafehouseData[index][sh_Password][0] != '\0')
        return ShowSafehousePasswordPrompt(playerid, index);

    return EnterSafehouse(playerid, index);
}
