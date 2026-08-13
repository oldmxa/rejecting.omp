
#if defined _random_death_tag_included
    #endinput
#endif
#define _random_death_tag_included

#include <a_samp>

new PlayerText3D:DeadPlayerTag[MAX_PLAYERS] = {PlayerText3D:INVALID_3DTEXT_ID, ...};
new Timer3DText[MAX_PLAYERS];

new Text3D:BodyDeathTag[MAX_PLAYERS] = {Text3D:INVALID_3DTEXT_ID, ...};
new BodyDeathTimer[MAX_PLAYERS];

forward Hide3DText(playerid);
public Hide3DText(playerid)
{
    if(Timer3DText[playerid])
    {
        KillTimer(Timer3DText[playerid]);
        Timer3DText[playerid] = 0;
    }
    if(DeadPlayerTag[playerid] != PlayerText3D:INVALID_3DTEXT_ID)
    {
        DeletePlayer3DTextLabel(playerid, DeadPlayerTag[playerid]);
        DeadPlayerTag[playerid] = PlayerText3D:INVALID_3DTEXT_ID;
    }
    return 1;
}

forward HideBodyDeathTag(playerid);
public HideBodyDeathTag(playerid)
{
    if(BodyDeathTimer[playerid])
    {
        KillTimer(BodyDeathTimer[playerid]);
        BodyDeathTimer[playerid] = 0;
    }
    if(BodyDeathTag[playerid] != Text3D:INVALID_3DTEXT_ID)
    {
        DestroyDynamic3DTextLabel(BodyDeathTag[playerid]);
        BodyDeathTag[playerid] = Text3D:INVALID_3DTEXT_ID;
    }
    return 1;
}

stock ShowBody3DDeathTag(killerid, playerid, givemoney)
{
    if(killerid == INVALID_PLAYER_ID || !IsPlayerConnected(killerid)) return 0;
    if(playerid == INVALID_PLAYER_ID || !IsPlayerConnected(playerid)) return 0;
    if(!User[killerid][show3DDeathTag]) return 0;

    HideBodyDeathTag(playerid);

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    new vsKills = g_PlayerKillsAgainst[killerid][playerid];
    new vsDeaths = g_PlayerKillsAgainst[playerid][killerid];

    new strTag[128];
    format(strTag, sizeof(strTag), "{FF0000}DEAD\n{00FF00}+%s\n{00FF00}%d {FFFFFF}x {FF0000}%d", 
        FormatMoney(givemoney), 
        vsKills, 
        vsDeaths
    );

    BodyDeathTag[playerid] = CreateDynamic3DTextLabel(strTag, -1, x, y, z - 0.25, 12.0, .worldid = GetPlayerVirtualWorld(playerid), .interiorid = GetPlayerInterior(playerid), .testlos = 0);
    BodyDeathTimer[playerid] = SetTimerEx("HideBodyDeathTag", 3500, false, "i", playerid);
    return 1;
}

stock RandomDeathTags(killerid, playerid, death_mode = 0, const victim_name[] = "")
{	 
    if(killerid == INVALID_PLAYER_ID || !IsPlayerConnected(killerid)) return 0;
    if(!User[killerid][showRandomDeathTags]) return 0;

    Hide3DText(killerid);

    new name[MAX_PLAYER_NAME + 1];
    if(IsPlayerConnected(playerid))
    {
        GetPlayerName(playerid, name, sizeof(name));
    }
    else if(strlen(victim_name) > 0)
    {
        format(name, sizeof(name), "%s", victim_name);
    }
    else
    {
        format(name, sizeof(name), "Enemy");
    }

    new mode = (death_mode <= 0) ? random(8) : (death_mode - 1);
    new Strange[128];

    switch(mode)
    {
        case 0: format(Strange, sizeof(Strange), "{F3230E}Murdered: {FFFFFF}%s", name);
        case 1: format(Strange, sizeof(Strange), "{F3230E}Fucked: {FFFFFF}%s", name);
        case 2: format(Strange, sizeof(Strange), "{F3230E}Eaten: {FFFFFF}%s", name);
        case 3: format(Strange, sizeof(Strange), "{F3230E}Reaped: {FFFFFF}%s", name);
        case 4: format(Strange, sizeof(Strange), "{F3230E}You fucked {FFFFFF}%s", name);
        case 5: format(Strange, sizeof(Strange), "{F3230E}Sent {FFFFFF}%s {F3230E}to the cemetery!", name);
        case 6: format(Strange, sizeof(Strange), "{F3230E}Destroyed: {FFFFFF}%s", name);
        case 7: format(Strange, sizeof(Strange), "{F3230E}Eliminated: {FFFFFF}%s", name);
        default: format(Strange, sizeof(Strange), "{F3230E}Killed: {FFFFFF}%s", name);
    }

    DeadPlayerTag[killerid] = CreatePlayer3DTextLabel(killerid, Strange, -1, 0.0, 0.0, 0.7, 40.0, playerid, INVALID_VEHICLE_ID, false);

    Timer3DText[killerid] = SetTimerEx("Hide3DText", 2000, false, "i", killerid);
    return 1;
}

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    if(killerid != INVALID_PLAYER_ID && IsPlayerConnected(killerid))
    {
        RandomDeathTags(killerid, playerid);
    }

    #if defined RDT_OnPlayerDeath
        return RDT_OnPlayerDeath(playerid, killerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath RDT_OnPlayerDeath
#if defined RDT_OnPlayerDeath
    forward RDT_OnPlayerDeath(playerid, killerid, WEAPON:reason);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    Hide3DText(playerid);
    HideBodyDeathTag(playerid);

    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        g_PlayerKillsAgainst[playerid][i] = 0;
        g_PlayerKillsAgainst[i][playerid] = 0;
    }

    #if defined RDT_OnPlayerDisconnect
        return RDT_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect RDT_OnPlayerDisconnect
#if defined RDT_OnPlayerDisconnect
    forward RDT_OnPlayerDisconnect(playerid, reason);
#endif
