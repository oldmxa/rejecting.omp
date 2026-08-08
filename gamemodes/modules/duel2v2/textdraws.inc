#if defined _duel2v2_textdraws_included
    #endinput
#endif
#define _duel2v2_textdraws_included

#include <YSI_Coding/y_hooks>

new PlayerText:TD2v2MatchInfo[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};

stock CreateDuel2v2PlayerTextDraws(playerid)
{
    if(TD2v2MatchInfo[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, TD2v2MatchInfo[playerid]);
        TD2v2MatchInfo[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }

    TD2v2MatchInfo[playerid] = CreatePlayerTextDraw(playerid, 320.000000, 415.000000, "~w~DUEL 2v2~n~~b~~h~Team A: 0 ~w~: ~r~~h~Team B: 0");
    PlayerTextDrawFont(playerid, TD2v2MatchInfo[playerid], 1);
    PlayerTextDrawLetterSize(playerid, TD2v2MatchInfo[playerid], 0.220000, 0.950000);
    PlayerTextDrawSetOutline(playerid, TD2v2MatchInfo[playerid], 1);
    PlayerTextDrawSetShadow(playerid, TD2v2MatchInfo[playerid], 0);
    PlayerTextDrawAlignment(playerid, TD2v2MatchInfo[playerid], 2);
    PlayerTextDrawColor(playerid, TD2v2MatchInfo[playerid], -1);
    PlayerTextDrawBackgroundColor(playerid, TD2v2MatchInfo[playerid], MAIN_BACKGROUND_COLOUR);
    PlayerTextDrawSetProportional(playerid, TD2v2MatchInfo[playerid], true);
    return 1;
}

stock DestroyDuel2v2PlayerTextDraws(playerid)
{
    if(TD2v2MatchInfo[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, TD2v2MatchInfo[playerid]);
        TD2v2MatchInfo[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }
    return 1;
}

stock UpdateDuel2v2TextDrawString(playerid, const text[])
{
    if(TD2v2MatchInfo[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawSetString(playerid, TD2v2MatchInfo[playerid], text);
        PlayerTextDrawShow(playerid, TD2v2MatchInfo[playerid]);
    }
    return 1;
}

hook OnPlayerConnect(playerid)
{
    g_PlayerDuel2v2ID[playerid] = -1;
    g_PlayerDuel2v2Team[playerid] = -1;
    TD2v2MatchInfo[playerid] = PlayerText:INVALID_TEXT_DRAW;

    g_Duel2v2Wiz_Arena[playerid] = 0;
    g_Duel2v2Wiz_Weapons[playerid] = 0;
    g_Duel2v2Wiz_Rounds[playerid] = 1;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    DestroyDuel2v2PlayerTextDraws(playerid);
    return 1;
}
