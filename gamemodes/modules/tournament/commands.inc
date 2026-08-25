#if defined _tourney_cmds_included
    #endinput
#endif
#define _tourney_cmds_included

// Tournament Commands

@cmd(.alts = {"tourney", "tourneyadmin"}) tournament(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(User[playerid][adminLevel] >= 4)
    {
        CheckAdminAuth(playerid);
        return ShowTournamentAdminMenu(playerid);
    }

    return ShowTourneyBracketDialog(playerid);
}

@cmd(.alts = {"jointourney"}) jointournament(playerid, params[], help)
{
    if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}There are no open tournament registrations right now.");
        return 1;
    }

    if(g_PlayerTourneySlot[playerid] != -1)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You are already registered in the tournament!");
        return 1;
    }

    if(IsPlayerBusy(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You cannot join the tournament while busy in another activity.");
        return 1;
    }

    if(g_TourneyData[t_Mode] == TOURNAMENT_MODE_2V2)
    {
        new targetid = INVALID_PLAYER_ID;
        if(sscanf(params, "u", targetid))
        {
            SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Usage: /jointournament [teammate id/name]");
            return 1;
        }

        if(!IsPlayerLogged(targetid))
        {
            SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
            return 1;
        }

        if(targetid == playerid)
        {
            SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You cannot invite yourself as teammate.");
            return 1;
        }

        if(IsPlayerBusy(targetid))
        {
            SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your teammate is currently busy or in a duel.");
            return 1;
        }

        SendTourneyInvite(playerid, targetid);
        return 1;
    }
    else if(g_TourneyData[t_Mode] == TOURNAMENT_MODE_CLAN)
    {
        new clanid = User[playerid][clanID];
        if(clanid == -1)
        {
            SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You must be in a clan to register in Clan War tournament!");
            return 1;
        }

        if(!HasClanRank(playerid, RANK_COFOUNDER))
        {
            SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Only Clan Leader or Co-Founder can register the clan!");
            return 1;
        }

        if(GetClanMemberCount(clanid) < 2)
        {
            SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your clan must have at least 2 members to participate in Clan War!");
            return 1;
        }

        new targetid = INVALID_PLAYER_ID;
        if(!isnull(params))
        {
            sscanf(params, "u", targetid);
        }

        RegisterParticipant(playerid, targetid);
        return 1;
    }

    RegisterParticipant(playerid);
    return 1;
}

@cmd(.alts = {"accepttourney"}) accepttournament(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(IsPlayerBusy(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You cannot join the tournament while busy in another activity.");

    AcceptTourneyInvite(playerid);
    return 1;
}

@cmd(.alts = {"leavetourney"}) leavetournament(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    UnregisterParticipant(playerid);
    return 1;
}

@cmd(.alts = {"tourneybracket"}) bracket(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    ShowTourneyBracketDialog(playerid);
    return 1;
}

@cmd(.alts = {"tourneyspec", "spectourney"}) spectournament(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(g_TourneyData[t_State] != TOURNAMENT_STATE_IN_PROGRESS)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}There are no live tournament matches to spectate.");
        return 1;
    }

    new matchid = 0;
    if(!isnull(params))
    {
        matchid = strval(params) - 1;
    }
    else
    {
        // Find first active match
        for(new i = 0; i < MAX_TOURNEY_MATCHES; i++)
        {
            if(g_TourneyMatches[i][tm_Active] && g_TourneyMatches[i][tm_State] == MATCH_STATE_ACTIVE)
            {
                matchid = i;
                break;
            }
        }
    }

    SpectateTourneyMatch(playerid, matchid);
    return 1;
}

@cmd() starttourney(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);
    CheckAdminAuth(playerid);

    if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Tournament is not in registration phase.");
        return 1;
    }

    if(g_TourneyData[t_RegisteredCount] < 2)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}At least 2 participants are required to start!");
        return 1;
    }

    g_TourneyData[t_State] = TOURNAMENT_STATE_STARTING;
    StartTournamentCountdown(0);
    return 1;
}

@cmd() canceltourney(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);
    CheckAdminAuth(playerid);

    CancelTournament(playerid, "Cancelled via command");
    return 1;
}
