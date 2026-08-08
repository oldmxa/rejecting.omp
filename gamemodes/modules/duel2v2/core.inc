#if defined _duel2v2_core_included
    #endinput
#endif
#define _duel2v2_core_included

#include <YSI_Coding/y_hooks>

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(IsPlayerInDuel2v2(playerid))
    {
        OnDuel2v2DeathCheck(playerid, killerid);
    }
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(IsPlayerInDuel2v2(playerid))
    {
        new matchid = g_PlayerDuel2v2ID[playerid];
        if(matchid != -1 && g_Duel2v2Data[matchid][d2_Active])
        {
            new disconnectMsg[96];
            format(disconnectMsg, sizeof(disconnectMsg), "%s disconnected during the 2v2 match.", getPlayerName(playerid));
            ForfeitDuel2v2Session(matchid, playerid, disconnectMsg);
        }

        g_PlayerDuel2v2ID[playerid] = -1;
        g_PlayerDuel2v2Team[playerid] = -1;
    }
    return 1;
}

hook OnPlayerWeaponShot(playerid, weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if(hittype != BULLET_HIT_TYPE_PLAYER) return 1;
    if(!IsPlayerInDuel2v2(playerid) || !IsPlayerInDuel2v2(hitid)) return 1;
    if(g_PlayerDuel2v2ID[playerid] != g_PlayerDuel2v2ID[hitid]) return 1;

    if(g_PlayerDuel2v2Team[playerid] == g_PlayerDuel2v2Team[hitid]) return 0;

    return 1;
}

hook OnPlayerSpawn(playerid)
{
    if(!IsPlayerInDuel2v2(playerid)) return 1;

    new matchid = g_PlayerDuel2v2ID[playerid];
    if(matchid < 0 || matchid >= MAX_DUELS_2V2 || !g_Duel2v2Data[matchid][d2_Active]) return 1;

    switch(g_Duel2v2Data[matchid][d2_State])
    {
        case DUEL2V2_STATE_PREPARING:
        {
            ApplyDuel2v2RoundSpawn(playerid);
        }
        case DUEL2V2_STATE_ACTIVE, DUEL2V2_STATE_ROUND_END:
        {
            ApplyDuel2v2DeadSpawn(playerid);
        }
    }
    return 1;
}
