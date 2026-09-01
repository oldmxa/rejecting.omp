#if defined _duel2v2_hpbar_included
    #endinput
#endif
#define _duel2v2_hpbar_included

#include <YSI_Coding\y_hooks>

#define D2V2_HPLIST_X     (628.000000)
#define D2V2_HPLIST_Y     (300.000000)
#define D2V2_HPLIST_MAX   (200)

#define D2V2_COL_NAME     "~w~~h~"
#define D2V2_COL_ALLY     "~b~~h~~h~"
#define D2V2_COL_ENEMY    "~r~~h~~h~"
#define D2V2_COL_DMG      "~y~~h~"
#define D2V2_COL_DEAD     "~r~~h~"

new PlayerText:g_Duel2v2HpList[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new Float:g_Duel2v2Damage[MAX_PLAYERS];

static g_Duel2v2HpHash[MAX_PLAYERS];

forward CheckDuel2v2HpBarOnUpdate(playerid);

stock GetDuel2v2Teammate(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return INVALID_PLAYER_ID;

    new matchid = g_PlayerDuel2v2ID[playerid];
    if(matchid < 0 || matchid >= MAX_DUELS_2V2 || !g_Duel2v2Data[matchid][d2_Active]) return INVALID_PLAYER_ID;

    if(playerid == g_Duel2v2Data[matchid][d2_TeamA_Player1]) return g_Duel2v2Data[matchid][d2_TeamA_Player2];
    if(playerid == g_Duel2v2Data[matchid][d2_TeamA_Player2]) return g_Duel2v2Data[matchid][d2_TeamA_Player1];
    if(playerid == g_Duel2v2Data[matchid][d2_TeamB_Player1]) return g_Duel2v2Data[matchid][d2_TeamB_Player2];
    if(playerid == g_Duel2v2Data[matchid][d2_TeamB_Player2]) return g_Duel2v2Data[matchid][d2_TeamB_Player1];

    return INVALID_PLAYER_ID;
}

stock DestroyDuel2v2HpBar(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;

    if(g_Duel2v2HpList[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, g_Duel2v2HpList[playerid]);
        g_Duel2v2HpList[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }

    g_Duel2v2HpHash[playerid] = 0;
    return 1;
}

static Duel2v2ListRow(dest[], len, targetid, bool:ally, bool:alive)
{
    new damage = floatround(g_Duel2v2Damage[targetid]);

    if(!alive)
    {
        #pragma unused ally
        return format(dest, len, "%s%s  %sDead %s- %s%d~n~",
            D2V2_COL_NAME, getPlayerName(targetid),
            D2V2_COL_DEAD, D2V2_COL_NAME, D2V2_COL_DMG, damage);
    }

    new Float:health, Float:armour;
    GetPlayerHealth(targetid, health);
    GetPlayerArmour(targetid, armour);

    new total = floatround(health + armour);
    if(total < 0) total = 0;
    if(total > D2V2_HPLIST_MAX) total = D2V2_HPLIST_MAX;

    return format(dest, len, "%s%s  %s%d %s- %s%d~n~",
        D2V2_COL_NAME, getPlayerName(targetid),
        ally ? (D2V2_COL_ALLY) : (D2V2_COL_ENEMY), total, D2V2_COL_NAME, D2V2_COL_DMG, damage);
}

stock UpdateDuel2v2HpBar(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
    if(g_Duel2v2HpList[playerid] == PlayerText:INVALID_TEXT_DRAW) return 0;

    new text[512], row[160], hash = 0;

    new matchid = g_PlayerDuel2v2ID[playerid];
    if(matchid < 0 || matchid >= MAX_DUELS_2V2 || !g_Duel2v2Data[matchid][d2_Active])
    {
        DestroyDuel2v2HpBar(playerid);
        return 0;
    }

    new players[4];
    GetDuel2v2Players(matchid, players);

    new myTeam = g_PlayerDuel2v2Team[playerid];

    for(new i = 0; i < 4; i++)
    {
        new pid = players[i];
        if(pid < 0 || pid >= MAX_PLAYERS || !IsPlayerConnected(pid)) continue;

        Duel2v2ListRow(row, sizeof(row), pid,
            (g_PlayerDuel2v2Team[pid] == myTeam), IsDuel2v2PlayerAlive(pid));

        strcat(text, row, sizeof(text));
    }

    for(new i = 0; text[i] != EOS; i++) hash = hash * 31 + text[i];

    if(hash == g_Duel2v2HpHash[playerid]) return 1;

    g_Duel2v2HpHash[playerid] = hash;
    PlayerTextDrawSetString(playerid, g_Duel2v2HpList[playerid], text);
    PlayerTextDrawShow(playerid, g_Duel2v2HpList[playerid]);
    return 1;
}

stock CreateDuel2v2HpBar(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
    if(!IsPlayerConnected(playerid)) return 0;

    DestroyDuel2v2HpBar(playerid);

    if(GetDuel2v2Teammate(playerid) == INVALID_PLAYER_ID) return 0;

    g_Duel2v2HpList[playerid] = CreatePlayerTextDraw(playerid, D2V2_HPLIST_X, D2V2_HPLIST_Y, "_");
    PlayerTextDrawFont(playerid, g_Duel2v2HpList[playerid], TEXT_DRAW_FONT_1);
    PlayerTextDrawLetterSize(playerid, g_Duel2v2HpList[playerid], 0.190000, 1.100000);
    PlayerTextDrawAlignment(playerid, g_Duel2v2HpList[playerid], TEXT_DRAW_ALIGN_RIGHT);
    PlayerTextDrawColor(playerid, g_Duel2v2HpList[playerid], -1);
    PlayerTextDrawSetOutline(playerid, g_Duel2v2HpList[playerid], 1);
    PlayerTextDrawSetShadow(playerid, g_Duel2v2HpList[playerid], 0);
    PlayerTextDrawSetProportional(playerid, g_Duel2v2HpList[playerid], true);
    PlayerTextDrawTextSize(playerid, g_Duel2v2HpList[playerid], 1280.000000, 1280.000000);
    PlayerTextDrawBackgroundColor(playerid, g_Duel2v2HpList[playerid], MAIN_BACKGROUND_COLOUR);

    g_Duel2v2HpHash[playerid] = 0;
    UpdateDuel2v2HpBar(playerid);
    return 1;
}

public CheckDuel2v2HpBarOnUpdate(playerid)
{
    UpdateDuel2v2HpBar(playerid);
    return 1;
}

hook OnPlayerConnect(playerid)
{
    g_Duel2v2HpList[playerid] = PlayerText:INVALID_TEXT_DRAW;
    g_Duel2v2HpHash[playerid] = 0;
    g_Duel2v2Damage[playerid] = 0.0;
    return 1;
}
