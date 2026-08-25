#if defined _initgame_included
    #endinput
#endif
#define _initgame_included

#include <YSI_Coding\y_hooks>

const RPC_INIT_GAME = 139;

#define IG_BIT_CJWALK (1)

new BitStream:g_InitGame[MAX_PLAYERS];
new bool:g_InitGameOk[MAX_PLAYERS];

ORPC:RPC_INIT_GAME(playerid, BitStream:bs)
{
    if (!(0 <= playerid < MAX_PLAYERS)) return 1;

    if (g_InitGame[playerid] != BitStream:0) BS_Delete(g_InitGame[playerid]);

    g_InitGame[playerid] = BS_NewCopy(bs);
    g_InitGameOk[playerid] = (g_InitGame[playerid] != BitStream:0);

    return 1;
}

forward RestorePlayerEnv(playerid);
public RestorePlayerEnv(playerid)
{
    if (IsPlayerLogged(playerid))
    {
        SetPlayerTime(playerid, User[playerid][time], 0);
        SetPlayerWeather(playerid, User[playerid][weather]);
    }
    return 1;
}

stock bool:ResendInitGame(playerid, bool:pedAnims)
{
    if (!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) return false;
    if (!User[playerid][isLogged]) return false;
    if (g_InitGame[playerid] == BitStream:0 || !g_InitGameOk[playerid]) return false;

    if (User[playerid][isSpectating] || playerSpecID[playerid] != INVALID_PLAYER_ID) return false;

    new BitStream:bs = BS_NewCopy(g_InitGame[playerid]);
    if (bs == BitStream:0) return false;

    new bits;
    BS_GetNumberOfBitsUsed(bs, bits);

    new bool:head[8];
    BS_SetReadOffset(bs, 0);
    for (new i; i < 8; i++) BS_ReadBool(bs, head[i]);
    BS_SetReadOffset(bs, 0);

    head[IG_BIT_CJWALK] = pedAnims;

    BS_SetWriteOffset(bs, 0);
    for (new i; i < 8; i++) BS_WriteBool(bs, head[i]);
    BS_SetWriteOffset(bs, bits);

    PR_SendRPC(bs, playerid, RPC_INIT_GAME);
    BS_Delete(bs);

    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && User[playerid][playerSkin] > 0)
    {
        SetPlayerSkin(playerid, User[playerid][playerSkin]);
    }

    SetPlayerTime(playerid, User[playerid][time], 0);
    SetPlayerWeather(playerid, User[playerid][weather]);
    SetTimerEx("RestorePlayerEnv", 50, false, "i", playerid);

    return true;
}

stock bool:IsRpWalkArena(location)
{
    return (location == ARENA_DM_1 || location == ARENA_DM_2);
}

stock ApplyPedAnimsToAll(bool:pedAnims)
{
    new done = 0;

    foreach (new i : Player)
    {
        new bool:targetAnim = IsRpWalkArena(User[i][locationId]) ? false : pedAnims;
        if (ResendInitGame(i, targetAnim)) done++;
    }

    return done;
}

stock ApplyArenaRpWalk(playerid, oldLocation, newLocation)
{
    new bool:wasRpWalk = IsRpWalkArena(oldLocation);
    new bool:isRpWalk = IsRpWalkArena(newLocation);

    if (wasRpWalk == isRpWalk) return 0;

    return ResendInitGame(playerid, !isRpWalk);
}

hook OnPlayerDisconnect(playerid, reason)
{
    if (g_InitGame[playerid] != BitStream:0)
    {
        BS_Delete(g_InitGame[playerid]);
        g_InitGame[playerid] = BitStream:0;
    }

    g_InitGameOk[playerid] = false;
    return 1;
}
