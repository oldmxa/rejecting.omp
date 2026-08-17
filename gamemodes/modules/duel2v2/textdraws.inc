#if defined _duel2v2_textdraws_included
    #endinput
#endif
#define _duel2v2_textdraws_included

new PlayerText:TD2v2MatchInfo[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};

stock CreateDuel2v2PlayerTextDraws(playerid)
{
    if(TD2v2MatchInfo[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, TD2v2MatchInfo[playerid]);
        TD2v2MatchInfo[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }

    TD2v2MatchInfo[playerid] = CreatePlayerTextDraw(playerid, 320.000000, 415.000000, "~w~DUEL 2v2~n~~b~~h~Team A: 0 ~w~: ~r~~h~Team B: 0");
    PlayerTextDrawFont(playerid, TD2v2MatchInfo[playerid], TEXT_DRAW_FONT_1);
    PlayerTextDrawLetterSize(playerid, TD2v2MatchInfo[playerid], 0.220000, 0.950000);
    PlayerTextDrawSetOutline(playerid, TD2v2MatchInfo[playerid], 1);
    PlayerTextDrawSetShadow(playerid, TD2v2MatchInfo[playerid], 0);
    PlayerTextDrawAlignment(playerid, TD2v2MatchInfo[playerid], TEXT_DRAW_ALIGN_CENTRE);
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

public OnPlayerConnect(playerid)
{
    g_PlayerDuel2v2ID[playerid] = -1;
    g_PlayerDuel2v2Team[playerid] = -1;
    TD2v2MatchInfo[playerid] = PlayerText:INVALID_TEXT_DRAW;

    g_Duel2v2Wiz_Arena[playerid] = 0;
    g_Duel2v2Wiz_Weapons[playerid] = 0;
    g_Duel2v2Wiz_Rounds[playerid] = 1;

    #if defined D2TD_OnPlayerConnect
        return D2TD_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect D2TD_OnPlayerConnect
#if defined D2TD_OnPlayerConnect
    forward D2TD_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    DestroyDuel2v2PlayerTextDraws(playerid);

    #if defined D2TD_OnPlayerDisconnect
        return D2TD_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect D2TD_OnPlayerDisconnect
#if defined D2TD_OnPlayerDisconnect
    forward D2TD_OnPlayerDisconnect(playerid, reason);
#endif
