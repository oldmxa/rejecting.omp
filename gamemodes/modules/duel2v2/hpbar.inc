#if defined _duel2v2_hpbar_included
    #endinput
#endif
#define _duel2v2_hpbar_included

#include <YSI_Coding\y_hooks>

#define D2V2_HPBAR_X      (557.000000)
#define D2V2_HPBAR_Y      (418.000000)
#define D2V2_HPBAR_WIDTH  (80.500000)
#define D2V2_HPBAR_HEIGHT (9.190000)
#define D2V2_HPBAR_MAX    (200)

#define D2V2_HPTEXT_X     (559.000000)
#define D2V2_HPTEXT_Y     (418.000000)

#define D2V2_HPBAR_BG     (0x00000099)
#define D2V2_HPBAR_TEAM_A (0x7C6DE8FF)
#define D2V2_HPBAR_TEAM_B (0xFF6A56FF)

new PlayerText:g_Duel2v2HpBack[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new PlayerText:g_Duel2v2HpFill[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new PlayerText:g_Duel2v2HpText[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};

new Float:g_Duel2v2Damage[MAX_PLAYERS];

static g_Duel2v2HpShown[MAX_PLAYERS];

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

    if(g_Duel2v2HpBack[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, g_Duel2v2HpBack[playerid]);
        g_Duel2v2HpBack[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }

    if(g_Duel2v2HpFill[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, g_Duel2v2HpFill[playerid]);
        g_Duel2v2HpFill[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }

    if(g_Duel2v2HpText[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, g_Duel2v2HpText[playerid]);
        g_Duel2v2HpText[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }

    g_Duel2v2HpShown[playerid] = -1;
    return 1;
}

static BuildDuel2v2Fill(playerid, total)
{
    if(g_Duel2v2HpFill[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, g_Duel2v2HpFill[playerid]);
        g_Duel2v2HpFill[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }

    if(total <= 0) return 0;

    new const Float:span = D2V2_HPBAR_WIDTH - 2.4;
    new const Float:endX = D2V2_HPBAR_X + 1.2 + (span * (float(total) / float(D2V2_HPBAR_MAX)));

    g_Duel2v2HpFill[playerid] = CreatePlayerTextDraw(playerid, D2V2_HPBAR_X + 1.2, D2V2_HPBAR_Y + 2.15, "_");
    PlayerTextDrawUseBox(playerid, g_Duel2v2HpFill[playerid], true);
    PlayerTextDrawTextSize(playerid, g_Duel2v2HpFill[playerid], endX, 0.0);
    PlayerTextDrawLetterSize(playerid, g_Duel2v2HpFill[playerid], 1.0, (D2V2_HPBAR_HEIGHT / 10.0) - 0.35);
    PlayerTextDrawBoxColor(playerid, g_Duel2v2HpFill[playerid],
        (g_PlayerDuel2v2Team[playerid] == 0) ? D2V2_HPBAR_TEAM_A : D2V2_HPBAR_TEAM_B);
    PlayerTextDrawShow(playerid, g_Duel2v2HpFill[playerid]);
    return 1;
}

stock UpdateDuel2v2HpBar(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
    if(g_Duel2v2HpBack[playerid] == PlayerText:INVALID_TEXT_DRAW) return 0;

    new mate = GetDuel2v2Teammate(playerid);

    if(mate == INVALID_PLAYER_ID || !IsPlayerConnected(mate))
    {
        DestroyDuel2v2HpBar(playerid);
        return 0;
    }

    new total = 0;

    if(IsDuel2v2PlayerAlive(mate))
    {
        new Float:health, Float:armour;
        GetPlayerHealth(mate, health);
        GetPlayerArmour(mate, armour);

        total = floatround(health + armour);
        if(total < 0) total = 0;
        if(total > D2V2_HPBAR_MAX) total = D2V2_HPBAR_MAX;
    }

    if(total == g_Duel2v2HpShown[playerid]) return 1;

    g_Duel2v2HpShown[playerid] = total;
    BuildDuel2v2Fill(playerid, total);

    new text[128], damage = floatround(g_Duel2v2Damage[mate]);

    if(total > 0) format(text, sizeof(text), "~w~~h~%s  %d / %d", getPlayerName(mate), total, damage);
    else format(text, sizeof(text), "~w~~h~%s  ~r~~h~Dead ~w~~h~/ %d", getPlayerName(mate), damage);

    PlayerTextDrawSetString(playerid, g_Duel2v2HpText[playerid], text);
    PlayerTextDrawShow(playerid, g_Duel2v2HpText[playerid]);
    return 1;
}

stock CreateDuel2v2HpBar(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
    if(!IsPlayerConnected(playerid)) return 0;

    DestroyDuel2v2HpBar(playerid);

    if(GetDuel2v2Teammate(playerid) == INVALID_PLAYER_ID) return 0;

    g_Duel2v2HpBack[playerid] = CreatePlayerTextDraw(playerid, D2V2_HPBAR_X, D2V2_HPBAR_Y, "_");
    PlayerTextDrawUseBox(playerid, g_Duel2v2HpBack[playerid], true);
    PlayerTextDrawTextSize(playerid, g_Duel2v2HpBack[playerid], D2V2_HPBAR_X + D2V2_HPBAR_WIDTH, 0.0);
    PlayerTextDrawLetterSize(playerid, g_Duel2v2HpBack[playerid], 1.0, D2V2_HPBAR_HEIGHT / 10.0);
    PlayerTextDrawBoxColor(playerid, g_Duel2v2HpBack[playerid], D2V2_HPBAR_BG);
    PlayerTextDrawShow(playerid, g_Duel2v2HpBack[playerid]);

    g_Duel2v2HpText[playerid] = CreatePlayerTextDraw(playerid, D2V2_HPTEXT_X, D2V2_HPTEXT_Y, "_");
    PlayerTextDrawFont(playerid, g_Duel2v2HpText[playerid], TEXT_DRAW_FONT_1);
    PlayerTextDrawLetterSize(playerid, g_Duel2v2HpText[playerid], 0.160000, 1.100000);
    PlayerTextDrawSetOutline(playerid, g_Duel2v2HpText[playerid], 1);
    PlayerTextDrawSetShadow(playerid, g_Duel2v2HpText[playerid], 0);
    PlayerTextDrawAlignment(playerid, g_Duel2v2HpText[playerid], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, g_Duel2v2HpText[playerid], -1);
    PlayerTextDrawBackgroundColor(playerid, g_Duel2v2HpText[playerid], MAIN_BACKGROUND_COLOUR);
    PlayerTextDrawSetProportional(playerid, g_Duel2v2HpText[playerid], true);

    g_Duel2v2HpShown[playerid] = -1;
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
    g_Duel2v2HpBack[playerid] = PlayerText:INVALID_TEXT_DRAW;
    g_Duel2v2HpFill[playerid] = PlayerText:INVALID_TEXT_DRAW;
    g_Duel2v2HpText[playerid] = PlayerText:INVALID_TEXT_DRAW;
    g_Duel2v2HpShown[playerid] = -1;
    g_Duel2v2Damage[playerid] = 0.0;
    return 1;
}
