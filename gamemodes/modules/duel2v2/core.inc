#if defined _duel2v2_core_included
    #endinput
#endif
#define _duel2v2_core_included

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    if(IsPlayerInDuel2v2(playerid))
    {
        OnDuel2v2DeathCheck(playerid, killerid);
    }

    #if defined D2C_OnPlayerDeath
        return D2C_OnPlayerDeath(playerid, killerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath D2C_OnPlayerDeath
#if defined D2C_OnPlayerDeath
    forward D2C_OnPlayerDeath(playerid, killerid, WEAPON:reason);
#endif

public OnPlayerDisconnect(playerid, reason)
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

    #if defined D2C_OnPlayerDisconnect
        return D2C_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect D2C_OnPlayerDisconnect
#if defined D2C_OnPlayerDisconnect
    forward D2C_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if(hittype == BULLET_HIT_TYPE_PLAYER)
    {
        if(IsPlayerInDuel2v2(playerid) && IsPlayerInDuel2v2(hitid))
        {
            if(g_PlayerDuel2v2ID[playerid] == g_PlayerDuel2v2ID[hitid])
            {
                if(g_PlayerDuel2v2Team[playerid] == g_PlayerDuel2v2Team[hitid]) return 0;
                if(!IsDuel2v2PlayerAlive(playerid) || !IsDuel2v2PlayerAlive(hitid)) return 0;
            }
        }
    }

    #if defined D2C_OnPlayerWeaponShot
        return D2C_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot D2C_OnPlayerWeaponShot
#if defined D2C_OnPlayerWeaponShot
    forward D2C_OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

public OnPlayerSpawn(playerid)
{
    if(IsPlayerInDuel2v2(playerid))
    {
        new matchid = g_PlayerDuel2v2ID[playerid];
        if(matchid >= 0 && matchid < MAX_DUELS_2V2 && g_Duel2v2Data[matchid][d2_Active])
        {
            switch(g_Duel2v2Data[matchid][d2_State])
            {
                case DUEL2V2_STATE_PREPARING:
                {
                    ApplyDuel2v2RoundSpawn(playerid);
                }
                case DUEL2V2_STATE_ACTIVE, DUEL2V2_STATE_ROUND_END:
                {
                    if(IsDuel2v2PlayerAlive(playerid)) SetPlayerVirtualWorld(playerid, DUEL2V2_VW_BASE + matchid);
                    else ApplyDuel2v2DeadSpawn(playerid);
                }
            }
        }
    }

    #if defined D2C_OnPlayerSpawn
        return D2C_OnPlayerSpawn(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn D2C_OnPlayerSpawn
#if defined D2C_OnPlayerSpawn
    forward D2C_OnPlayerSpawn(playerid);
#endif
