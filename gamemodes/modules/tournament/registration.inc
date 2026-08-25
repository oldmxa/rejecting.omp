#if defined _tourney_registration_included
    #endinput
#endif
#define _tourney_registration_included

stock RegisterParticipant(playerid, teammateid = INVALID_PLAYER_ID)
{
    if(!IsPlayerLogged(playerid)) return -1;
    if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Registrations are currently closed!");
        return -1;
    }

    if(g_PlayerTourneySlot[playerid] != -1)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You are already registered in the tournament!");
        return -1;
    }

    if(g_TourneyData[t_RegisteredCount] >= g_TourneyData[t_MaxSlots])
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}The tournament registration slots are already full!");
        return -1;
    }

    if(g_TourneyData[t_RequireDiscord])
    {
        if(!IsPlayerRegisteredOnDiscord(playerid))
        {
            SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You must complete your application on Discord first!");
            SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Go to Discord channel {23C552}#" CH_TOURNEY_REGISTER " {FFFFFF}and use: {MAIN}/tournament <nick>");
            SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Without a Discord application, you can only spectate with {MAIN}/bracket {FFFFFF}or {MAIN}/spectournament{FFFFFF}.");
            return -1;
        }
    }

    if(g_TourneyData[t_EntryFee] > 0)
    {
        if(User[playerid][playerMoney] < g_TourneyData[t_EntryFee])
        {
            va_SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You need %s to pay the registration fee.", FormatMoney(g_TourneyData[t_EntryFee]));
            return -1;
        }
    }

    new slot = -1;
    for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
    {
        if(!g_TourneyParticipants[i][tp_Registered])
        {
            slot = i;
            break;
        }
    }

    if(slot == -1) return -1;

    switch(g_TourneyData[t_Mode])
    {
        case TOURNAMENT_MODE_1V1:
        {
            g_TourneyParticipants[slot][tp_Registered] = true;
            g_TourneyParticipants[slot][tp_Player1] = playerid;
            g_TourneyParticipants[slot][tp_Player1_SQL] = User[playerid][user_sqlid];
            strcopy(g_TourneyParticipants[slot][tp_Player1_Name], User[playerid][userName], MAX_PLAYER_NAME + 1);
            g_TourneyParticipants[slot][tp_Eliminated] = false;
            g_TourneyParticipants[slot][tp_Seed] = slot + 1;
            g_PlayerTourneySlot[playerid] = slot;

            if(g_TourneyData[t_EntryFee] > 0)
            {
                GivePlayerCash(playerid, -g_TourneyData[t_EntryFee]);
            }

            g_TourneyData[t_RegisteredCount]++;

            va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s joined the tournament! ({MAIN}%d/%d {FFFFFF}slots filled).",
                User[playerid][userName], g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        }
        case TOURNAMENT_MODE_2V2:
        {
            if(teammateid == INVALID_PLAYER_ID || !IsPlayerLogged(teammateid) || teammateid == playerid)
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Usage: /jointournament [teammate id/name]");
                return -1;
            }

            if(g_PlayerTourneySlot[teammateid] != -1)
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your teammate is already registered in another team!");
                return -1;
            }

            if(g_PlayerTourneyInvite[teammateid] != playerid)
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your teammate has not accepted the invite yet.");
                return -1;
            }

            if(g_TourneyData[t_RequireDiscord] && !IsPlayerRegisteredOnDiscord(teammateid))
            {
                va_SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your teammate %s has not completed their Discord application yet!", User[teammateid][userName]);
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}They must go to Discord channel {23C552}#" CH_TOURNEY_REGISTER " {FFFFFF}and use: {MAIN}/tournament <nick>");
                return -1;
            }

            if(g_TourneyData[t_EntryFee] > 0 && !HasPlayerCash(teammateid, g_TourneyData[t_EntryFee]))
            {
                va_SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your teammate does not have enough money for the registration fee (%s).", FormatMoney(g_TourneyData[t_EntryFee]));
                return -1;
            }

            g_TourneyParticipants[slot][tp_Registered] = true;
            g_TourneyParticipants[slot][tp_Player1] = playerid;
            g_TourneyParticipants[slot][tp_Player1_SQL] = User[playerid][user_sqlid];
            strcopy(g_TourneyParticipants[slot][tp_Player1_Name], User[playerid][userName], MAX_PLAYER_NAME + 1);

            g_TourneyParticipants[slot][tp_Player2] = teammateid;
            g_TourneyParticipants[slot][tp_Player2_SQL] = User[teammateid][user_sqlid];
            strcopy(g_TourneyParticipants[slot][tp_Player2_Name], User[teammateid][userName], MAX_PLAYER_NAME + 1);

            g_TourneyParticipants[slot][tp_Eliminated] = false;
            g_TourneyParticipants[slot][tp_Seed] = slot + 1;
            g_PlayerTourneySlot[playerid] = slot;
            g_PlayerTourneySlot[teammateid] = slot;

            g_PlayerTourneyInvite[teammateid] = -1;

            if(g_TourneyData[t_EntryFee] > 0)
            {
                GivePlayerCash(playerid, -g_TourneyData[t_EntryFee]);
                GivePlayerCash(teammateid, -g_TourneyData[t_EntryFee]);
            }

            g_TourneyData[t_RegisteredCount]++;

            va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}Team {MAIN}%s & %s {FFFFFF}joined the 2v2 tournament! ({MAIN}%d/%d {FFFFFF}slots).",
                User[playerid][userName], User[teammateid][userName], g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        }
        case TOURNAMENT_MODE_CLAN:
        {
            if(!IsPlayerInClan(playerid))
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You must be in a clan to register in Clan War tournament!");
                return -1;
            }

            new clanid = GetPlayerClan(playerid);
            if(!HasClanRank(playerid, RANK_COFOUNDER))
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Only Clan Leader or Co-Founder can register the clan!");
                return -1;
            }

            if(GetClanMemberCount(clanid) < 2)
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your clan must have at least 2 members to participate in Clan War!");
                return -1;
            }

            for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
            {
                if(g_TourneyParticipants[i][tp_Registered] && g_TourneyParticipants[i][tp_ClanID] == clanid)
                {
                    SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your clan is already registered!");
                    return -1;
                }
            }

            g_TourneyParticipants[slot][tp_Registered] = true;
            g_TourneyParticipants[slot][tp_Player1] = playerid;
            g_TourneyParticipants[slot][tp_Player1_SQL] = User[playerid][user_sqlid];
            strcopy(g_TourneyParticipants[slot][tp_Player1_Name], User[playerid][userName], MAX_PLAYER_NAME + 1);

            if(teammateid != INVALID_PLAYER_ID && teammateid != playerid && IsPlayerLogged(teammateid) && User[teammateid][clanID] == clanid && g_PlayerTourneySlot[teammateid] == -1)
            {
                g_TourneyParticipants[slot][tp_Player2] = teammateid;
                g_TourneyParticipants[slot][tp_Player2_SQL] = User[teammateid][user_sqlid];
                strcopy(g_TourneyParticipants[slot][tp_Player2_Name], User[teammateid][userName], MAX_PLAYER_NAME + 1);
                g_PlayerTourneySlot[teammateid] = slot;
            }
            else
            {
                g_TourneyParticipants[slot][tp_Player2] = INVALID_PLAYER_ID;
                g_TourneyParticipants[slot][tp_Player2_SQL] = 0;
                g_TourneyParticipants[slot][tp_Player2_Name][0] = '\0';
            }

            g_TourneyParticipants[slot][tp_ClanID] = clanid;
            strcopy(g_TourneyParticipants[slot][tp_ClanName], ClanInfo[clanid][clanName], CLAN_NAME_LEN);
            strcopy(g_TourneyParticipants[slot][tp_ClanTag], ClanInfo[clanid][clanTag], CLAN_TAG_LEN);
            g_TourneyParticipants[slot][tp_Eliminated] = false;
            g_TourneyParticipants[slot][tp_Seed] = slot + 1;
            g_PlayerTourneySlot[playerid] = slot;

            if(g_TourneyData[t_EntryFee] > 0)
            {
                GivePlayerCash(playerid, -g_TourneyData[t_EntryFee]);
            }

            g_TourneyData[t_RegisteredCount]++;

            va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}Clan {MAIN}[%s] %s {FFFFFF}registered for Clan War tournament! ({MAIN}%d/%d {FFFFFF}slots).",
                ClanInfo[clanid][clanTag], ClanInfo[clanid][clanName], g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        }
    }

    // Auto-start check
    if(g_TourneyData[t_AutoStart] && g_TourneyData[t_RegisteredCount] == g_TourneyData[t_MaxSlots])
    {
        SendTourneyBroadcast("All registration slots are filled! Tournament will begin in 10 seconds!");
        g_TourneyData[t_State] = TOURNAMENT_STATE_STARTING;
        SetTimerEx("StartTournamentCountdown", 10000, false, "d", 0);
    }

    return slot;
}

stock SendTourneyInvite(playerid, teammateid)
{
    if(teammateid == INVALID_PLAYER_ID || !IsPlayerLogged(teammateid) || teammateid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Usage: /jointournament [teammate id/name]");

    if(g_PlayerTourneySlot[playerid] != -1)
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You are already registered in the tournament!");

    if(g_PlayerTourneySlot[teammateid] != -1)
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}That player is already registered in another team!");

    if(g_TourneyData[t_RequireDiscord] && !IsPlayerRegisteredOnDiscord(teammateid))
        return va_SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}%s has not completed the Discord application yet.", User[teammateid][userName]);

    g_PlayerTourneyInvite[teammateid] = playerid;

    va_SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Invite sent to %s. Waiting for them to accept.", User[teammateid][userName]);
    va_SendClientMessage(teammateid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}%s invited you as their 2v2 teammate. Type {MAIN}/accepttourney {FFFFFF}to accept.", User[playerid][userName]);
    return 1;
}

stock AcceptTourneyInvite(playerid)
{
    new inviter = g_PlayerTourneyInvite[playerid];

    if(inviter == -1 || !IsPlayerLogged(inviter))
    {
        g_PlayerTourneyInvite[playerid] = -1;
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You have no pending tournament invite.");
    }

    if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION)
    {
        g_PlayerTourneyInvite[playerid] = -1;
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Registrations are currently closed!");
    }

    if(g_PlayerTourneySlot[inviter] != -1)
    {
        g_PlayerTourneyInvite[playerid] = -1;
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}The player who invited you already registered with someone else.");
    }

    return RegisterParticipant(inviter, playerid);
}

stock UnregisterParticipant(playerid)
{
    if(!IsPlayerInTournament(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You are not registered in the tournament!");
        return 0;
    }

    if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You cannot unregister once the tournament has started!");
        return 0;
    }

    new slot = g_PlayerTourneySlot[playerid];
    if(slot < 0 || slot >= MAX_TOURNEY_SLOTS) return 0;

    new p1 = g_TourneyParticipants[slot][tp_Player1];
    new p2 = g_TourneyParticipants[slot][tp_Player2];

    new bool:p1Valid = (p1 != INVALID_PLAYER_ID && 0 <= p1 < MAX_PLAYERS);
    new bool:p2Valid = (p2 != INVALID_PLAYER_ID && 0 <= p2 < MAX_PLAYERS && p2 != p1);

    if(g_TourneyData[t_EntryFee] > 0)
    {
        if(p1Valid && IsPlayerConnected(p1))
        {
            GivePlayerCash(p1, g_TourneyData[t_EntryFee]);
        }
        if(p2Valid && IsPlayerConnected(p2))
        {
            GivePlayerCash(p2, g_TourneyData[t_EntryFee]);
        }
    }

    if(p1Valid) g_PlayerTourneySlot[p1] = -1;
    if(p2Valid) g_PlayerTourneySlot[p2] = -1;

    g_PlayerTourneySlot[playerid] = -1;

    g_TourneyParticipants[slot][tp_Registered] = false;
    g_TourneyParticipants[slot][tp_Player1] = INVALID_PLAYER_ID;
    g_TourneyParticipants[slot][tp_Player1_SQL] = 0;
    g_TourneyParticipants[slot][tp_Player1_Name][0] = '\0';
    g_TourneyParticipants[slot][tp_Player2] = INVALID_PLAYER_ID;
    g_TourneyParticipants[slot][tp_Player2_SQL] = 0;
    g_TourneyParticipants[slot][tp_Player2_Name][0] = '\0';
    g_TourneyParticipants[slot][tp_ClanID] = -1;

    if(g_TourneyData[t_RegisteredCount] > 0) g_TourneyData[t_RegisteredCount]--;

    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s left the tournament registration. ({MAIN}%d/%d {FFFFFF}slots remaining).",
        User[playerid][userName], g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);

    return 1;
}

stock KickTourneyParticipant(slot, adminid = INVALID_PLAYER_ID)
{
    if(slot < 0 || slot >= MAX_TOURNEY_SLOTS || !g_TourneyParticipants[slot][tp_Registered]) return 0;
    if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION) return 0;

    new p1 = g_TourneyParticipants[slot][tp_Player1];
    new p2 = g_TourneyParticipants[slot][tp_Player2];

    if(g_TourneyData[t_EntryFee] > 0)
    {
        if(p1 != INVALID_PLAYER_ID && IsPlayerLogged(p1))
        {
            GivePlayerCash(p1, g_TourneyData[t_EntryFee]);
            SendClientMessage(p1, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You were removed from the tournament by staff and refunded.");
        }
        if(p2 != INVALID_PLAYER_ID && IsPlayerLogged(p2))
        {
            GivePlayerCash(p2, g_TourneyData[t_EntryFee]);
            SendClientMessage(p2, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You were removed from the tournament by staff and refunded.");
        }
    }

    if(p1 != INVALID_PLAYER_ID && 0 <= p1 < MAX_PLAYERS) g_PlayerTourneySlot[p1] = -1;
    if(p2 != INVALID_PLAYER_ID && 0 <= p2 < MAX_PLAYERS) g_PlayerTourneySlot[p2] = -1;

    new partName[64];
    GetParticipantDisplayName(slot, partName, sizeof(partName));

    g_TourneyParticipants[slot][tp_Registered] = false;
    g_TourneyParticipants[slot][tp_Player1] = INVALID_PLAYER_ID;
    g_TourneyParticipants[slot][tp_Player1_SQL] = 0;
    g_TourneyParticipants[slot][tp_Player1_Name][0] = '\0';
    g_TourneyParticipants[slot][tp_Player2] = INVALID_PLAYER_ID;
    g_TourneyParticipants[slot][tp_Player2_SQL] = 0;
    g_TourneyParticipants[slot][tp_Player2_Name][0] = '\0';
    g_TourneyParticipants[slot][tp_ClanID] = -1;

    if(g_TourneyData[t_RegisteredCount] > 0) g_TourneyData[t_RegisteredCount]--;

    if(adminid != INVALID_PLAYER_ID && IsPlayerConnected(adminid))
    {
        va_SendClientMessage(adminid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Participant %s was removed from slot #%d.", partName, slot + 1);
    }
    return 1;
}

stock StartTournamentRegistration(adminid)
{
    if(g_TourneyData[t_Active] || g_TourneyData[t_State] != TOURNAMENT_STATE_NONE)
    {
        if(IsPlayerConnected(adminid))
            SendClientMessage(adminid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}A tournament is already active or open!");
        return 0;
    }

    g_TourneyData[t_Active] = true;
    g_TourneyData[t_State] = TOURNAMENT_STATE_REGISTRATION;
    g_TourneyData[t_RegisteredCount] = 0;
    g_TourneyData[t_StartTime] = gettime();

    if(IsPlayerConnected(adminid))
        strcopy(g_TourneyData[t_StartedBy], User[adminid][userName], MAX_PLAYER_NAME + 1);

    new modeName[32];
    switch(g_TourneyData[t_Mode])
    {
        case TOURNAMENT_MODE_1V1: strcopy(modeName, "1v1 Elimination");
        case TOURNAMENT_MODE_2V2: strcopy(modeName, "2v2 Team Battle");
        case TOURNAMENT_MODE_CLAN: strcopy(modeName, "Clan War");
    }

    new feeStr[32];
    if(g_TourneyData[t_EntryFee] > 0) format(feeStr, sizeof(feeStr), "%s", FormatMoney(g_TourneyData[t_EntryFee]));
    else strcopy(feeStr, "FREE");

    new prizeStr[64];
    format(prizeStr, sizeof(prizeStr), "{FFD700}%d Coins {FFFFFF}+ {23C552}%s", g_TourneyData[t_PrizeCoins], FormatMoney(g_TourneyData[t_PrizeMoney]));

    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT OPEN] {FFFFFF}Administrator {MAIN}%s {FFFFFF}opened registrations for {MAIN}%s{FFFFFF}!", g_TourneyData[t_StartedBy], g_TourneyData[t_Name]);
    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {B4B5B7}For full information, schedule, prizes and registration, check our {00B6F6}Discord{B4B5B7} server!");

    new discordAnnounce[1024];
    format(discordAnnounce, sizeof(discordAnnounce),
        "**Event Information**\n"\
        "- **Tournament:** `%s`\n"\
        "- **Schedule:** `%s`\n"\
        "- **Modality:** `%s` (%d Slots)\n\n"\
        "**Prizes & Rewards**\n"\
        "- **Champion Tag:** `%s`\n"\
        "- **Grand Prize:** `%d Coins` + `%s`\n"\
        "- **Entry Fee:** `%s`\n"\
        "- **Weapon Loadout:** `%s`\n\n"\
        "**How to Participate**\n"\
        "Use `/tournament <InGame_Nick>` in <#" CH_TOURNEY_REGISTER "> to apply!",
        g_TourneyData[t_Name], g_TourneyData[t_Schedule], modeName, g_TourneyData[t_MaxSlots],
        g_TourneyData[t_AchievementTag], g_TourneyData[t_PrizeCoins], FormatMoney(g_TourneyData[t_PrizeMoney]),
        feeStr, g_TourneyWeaponSets[g_TourneyData[t_WeaponSet]][twi_Name]
    );
    utf8encode(msgencoded, discordAnnounce, sizeof(msgencoded));

    new discordTitle[128];
    format(discordTitle, sizeof(discordTitle), ":trophy: %s - Registrations Open", g_TourneyData[t_Name]);
    SendDiscordEmbed(ch_tourney_announce, discordTitle, msgencoded, DISCORD_COLOR_WARNING);

    new arenaNameStr[32];
    if(g_TourneyData[t_ArenaID] >= 0 && g_TourneyData[t_ArenaID] < sizeof(DuelS_ArenaNames)) strcopy(arenaNameStr, DuelS_ArenaNames[g_TourneyData[t_ArenaID]]);
    else strcopy(arenaNameStr, "Random");

    new adminLogEmbed[1024];
    format(adminLogEmbed, sizeof(adminLogEmbed),
        "**Overview**\n"\
        "- **Tournament:** `%s`\n"\
        "- **Created by:** `%s`\n"\
        "- **Scheduled for:** `%s`\n\n"\
        "**Match Configuration**\n"\
        "- **Modality:** `%s` (%d Slots)\n"\
        "- **Format:** `%s` (%d sec/round)\n"\
        "- **Weapon Loadout:** `%s`\n"\
        "- **Arena:** `%s`\n\n"\
        "**Rewards & Settings**\n"\
        "- **Prize:** `%d Coins` + `%s`\n"\
        "- **Achievement:** `%s`\n"\
        "- **Entry Fee:** `%s`\n"\
        "- **Auto-Start:** `%s`\n"\
        "- **Created At:** `%s`",
        g_TourneyData[t_Name], g_TourneyData[t_StartedBy], g_TourneyData[t_Schedule],
        modeName, g_TourneyData[t_MaxSlots],
        (g_TourneyData[t_RoundsPerMatch] == 1) ? "Best of 1" : "Best of 3 (Bo3)", g_TourneyData[t_RoundTimeLimit],
        g_TourneyWeaponSets[g_TourneyData[t_WeaponSet]][twi_Name], arenaNameStr,
        g_TourneyData[t_PrizeCoins], FormatMoney(g_TourneyData[t_PrizeMoney]),
        g_TourneyData[t_AchievementTag], feeStr,
        g_TourneyData[t_AutoStart] ? "Enabled" : "Disabled",
        returnDate(gettime())
    );
    utf8encode(msgencoded, adminLogEmbed, sizeof(msgencoded));
    SendDiscordEmbed(ch_tourney_admin_logs, ":hammer_and_wrench: Tournament Created", msgencoded, DISCORD_COLOR_ADMIN);

    return 1;
}

stock ProcessDiscordTourneyApp(DCC_User:user, DCC_Channel:channel, const params[])
{
    if(_:user == 0) return 0;

    new bool:isBot;
    DCC_IsUserBot(user, isBot);
    if(isBot) return 0;

    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));

    new username[DCC_USERNAME_SIZE];
    DCC_GetUserName(user, username, sizeof(username));

    if(!IsDiscordUserVerified(user))
    {
        new notVerifiedMsg[256];
        format(notVerifiedMsg, sizeof(notVerifiedMsg), "<@%s> :no_entry: Only **verified members** can register for tournaments!\nPlease link your account using `/verify` on Discord or `/linkdiscord` in-game to get verified.", userid);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, notVerifiedMsg);
        return 0;
    }

    if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION)
    {
        new failMsg[192];
        format(failMsg, sizeof(failMsg), "<@%s> :warning: There are no open tournament registrations right now.", userid);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, failMsg);
        return 0;
    }

    if(g_TourneyData[t_Mode] != TOURNAMENT_MODE_1V1)
    {
        new modeName[16], modeMsg[256];

        if(g_TourneyData[t_Mode] == TOURNAMENT_MODE_2V2) strcopy(modeName, "2v2", sizeof(modeName));
        else strcopy(modeName, "Clan War", sizeof(modeName));

        format(modeMsg, sizeof(modeMsg), "<@%s> :warning: This tournament is **%s**. Discord registration is only available for **1v1** - please register in-game with `/jointournament`.", userid, modeName);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, modeMsg);
        return 0;
    }

    new cleanParams[256];
    format(cleanParams, sizeof(cleanParams), "%s", params);
    cleanParams[sizeof(cleanParams) - 1] = '\0';

    if(!strcmp(cleanParams, "/tournament", true, 11) || !strcmp(cleanParams, ".tournament", true, 11) || !strcmp(cleanParams, "!tournament", true, 11))
    {
        strdel(cleanParams, 0, 11);
    }

    new startPos = 0;
    while(cleanParams[startPos] == ' ' || cleanParams[startPos] == '\t') startPos++;
    if(startPos > 0) strdel(cleanParams, 0, startPos);

    new nick[MAX_PLAYER_NAME + 1];
    if(sscanf(cleanParams, "s[25]", nick))
    {
        new usageMsg[256];
        format(usageMsg, sizeof(usageMsg), "<@%s> :x: Invalid format!\n**Usage:** `/tournament <InGame_Nick>`\n*Example:* `/tournament Nick_Silva`", userid);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, usageMsg);
        return 0;
    }

    for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
    {
        if(g_TourneyParticipants[i][tp_Registered])
        {
            if(!strcmp(g_TourneyParticipants[i][tp_Player1_Name], nick, true) || (g_TourneyParticipants[i][tp_Player2_Name][0] != '\0' && !strcmp(g_TourneyParticipants[i][tp_Player2_Name], nick, true)))
            {
                new alreadyMsg[256];
                format(alreadyMsg, sizeof(alreadyMsg), "<@%s> :warning: Nick **%s** is already registered in the tournament!", userid, nick);
                if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, alreadyMsg);
                return 0;
            }
        }
    }

    new slot = -1;
    for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
    {
        if(!g_TourneyParticipants[i][tp_Registered])
        {
            slot = i;
            break;
        }
    }

    if(slot == -1)
    {
        new fullMsg[192];
        format(fullMsg, sizeof(fullMsg), "<@%s> :warning: The tournament registration slots are currently full (%d/%d)!", userid, g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, fullMsg);
        return 0;
    }

    new onlinePlayerId = INVALID_PLAYER_ID;
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(IsPlayerLogged(i))
        {
            if(!strcmp(User[i][userName], nick, true))
            {
                onlinePlayerId = i;
                break;
            }
        }
    }

    if(onlinePlayerId != INVALID_PLAYER_ID)
    {
        if(g_TourneyData[t_EntryFee] > 0 && User[onlinePlayerId][playerMoney] < g_TourneyData[t_EntryFee])
        {
            new feeMsg[256];
            format(feeMsg, sizeof(feeMsg), "<@%s> :x: **%s** does not have enough money for the entry fee (%s).", userid, nick, FormatMoney(g_TourneyData[t_EntryFee]));
            if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, feeMsg);
            return 0;
        }

        if(IsPlayerBusy(onlinePlayerId))
        {
            new busyMsg[256];
            format(busyMsg, sizeof(busyMsg), "<@%s> :warning: **%s** is busy in another activity right now and cannot be registered.", userid, nick);
            if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, busyMsg);
            return 0;
        }

        g_TourneyParticipants[slot][tp_Registered] = true;
        g_TourneyParticipants[slot][tp_Eliminated] = false;
        g_TourneyParticipants[slot][tp_Seed] = slot + 1;
        strcopy(g_TourneyParticipants[slot][tp_Player1_Name], nick, MAX_PLAYER_NAME + 1);
        g_TourneyParticipants[slot][tp_Player1] = onlinePlayerId;
        g_TourneyParticipants[slot][tp_Player1_SQL] = User[onlinePlayerId][user_sqlid];
        g_PlayerTourneySlot[onlinePlayerId] = slot;
        if(g_TourneyData[t_EntryFee] > 0)
        {
            GivePlayerCash(onlinePlayerId, -g_TourneyData[t_EntryFee]);
        }

        g_TourneyData[t_RegisteredCount]++;

        new appIdx = -1;
        for(new i = 0; i < MAX_DISCORD_TOURNEY_APPS; i++)
        {
            if(g_TourneyDiscordApps[i][tda_Active] && !strcmp(g_TourneyDiscordApps[i][tda_Nick], nick, true))
            {
                appIdx = i;
                break;
            }
        }
        if(appIdx == -1)
        {
            for(new i = 0; i < MAX_DISCORD_TOURNEY_APPS; i++)
            {
                if(!g_TourneyDiscordApps[i][tda_Active])
                {
                    appIdx = i;
                    break;
                }
            }
        }

        if(appIdx != -1)
        {
            g_TourneyDiscordApps[appIdx][tda_Active] = true;
            strcopy(g_TourneyDiscordApps[appIdx][tda_Nick], nick, MAX_PLAYER_NAME + 1);
            strcopy(g_TourneyDiscordApps[appIdx][tda_UCP], "Verified Discord", 128);
            strcopy(g_TourneyDiscordApps[appIdx][tda_DiscordID], userid, DCC_ID_SIZE);
            strcopy(g_TourneyDiscordApps[appIdx][tda_DiscordTag], username, DCC_USERNAME_SIZE);
            g_TourneyDiscordApps[appIdx][tda_Timestamp] = gettime();
            g_TourneyDiscordAppCount++;
        }

        new appEmbed[1024];
        format(appEmbed, sizeof(appEmbed),
            "**Applicant Details**\n"\
            "- **In-Game Nick:** `%s`\n"\
            "- **Discord Member:** <@%s> (`%s`)\n"\
            "- **Account Status:** :white_check_mark: Verified Account (Online)\n\n"\
            "**Tournament Status**\n"\
            "- **Event:** `%s`\n"\
            "- **Slot Assigned:** `#%02d`\n"\
            "- **Registered Slots:** `%d / %d`\n"\
            "- **Application Time:** `%s`",
            nick, userid, username,
            g_TourneyData[t_Name],
            slot + 1,
            g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots],
            returnDate(gettime())
        );

        utf8encode(msgencoded, appEmbed, sizeof(msgencoded));
        SendDiscordEmbed(ch_tourney_applications, ":pencil: Tournament Registration Received", msgencoded, DISCORD_COLOR_SUCCESS);

        new confirmMsg[256];
        format(confirmMsg, sizeof(confirmMsg), ":white_check_mark: <@%s> Your registration with nick **%s** is confirmed! (Slot #%02d - **%d/%d** slots filled).", userid, nick, slot + 1, g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, confirmMsg);

        va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s joined the tournament via Discord! ({MAIN}%d/%d {FFFFFF}slots filled).",
            nick, g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);

        if(g_TourneyData[t_AutoStart] && g_TourneyData[t_RegisteredCount] >= g_TourneyData[t_MaxSlots])
        {
            g_TourneyData[t_State] = TOURNAMENT_STATE_STARTING;
            StartTournamentCountdown(0);
        }
        return 1;
    }

    inline const OnTourneyCheckAccount()
    {
        if(!cache_num_rows())
        {
            new notFoundMsg[256];
            format(notFoundMsg, sizeof(notFoundMsg), "<@%s> :x: The account **%s** does not exist on the server! Please make sure you registered this account in-game.", userid, nick);
            if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, notFoundMsg);
            return 1;
        }

        if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION)
        {
            new failMsg[192];
            format(failMsg, sizeof(failMsg), "<@%s> :warning: Tournament registrations closed in the meantime.", userid);
            if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, failMsg);
            return 1;
        }

        for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
        {
            if(g_TourneyParticipants[i][tp_Registered])
            {
                if(!strcmp(g_TourneyParticipants[i][tp_Player1_Name], nick, true) || (g_TourneyParticipants[i][tp_Player2_Name][0] != '\0' && !strcmp(g_TourneyParticipants[i][tp_Player2_Name], nick, true)))
                {
                    new alreadyMsg[256];
                    format(alreadyMsg, sizeof(alreadyMsg), "<@%s> :warning: Nick **%s** is already registered in the tournament!", userid, nick);
                    if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, alreadyMsg);
                    return 1;
                }
            }
        }

        new slotIdx = -1;
        for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
        {
            if(!g_TourneyParticipants[i][tp_Registered])
            {
                slotIdx = i;
                break;
            }
        }

        if(slotIdx == -1)
        {
            new fullMsg[192];
            format(fullMsg, sizeof(fullMsg), "<@%s> :warning: The tournament registration slots are currently full (%d/%d)!", userid, g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
            if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, fullMsg);
            return 1;
        }

        new acc_id = 0;
        new acc_name[MAX_PLAYER_NAME + 1];
        cache_get_value_int(0, "id", acc_id);
        cache_get_value_name(0, "username", acc_name, sizeof(acc_name));

        g_TourneyParticipants[slotIdx][tp_Registered] = true;
        g_TourneyParticipants[slotIdx][tp_Eliminated] = false;
        g_TourneyParticipants[slotIdx][tp_Seed] = slotIdx + 1;
        g_TourneyParticipants[slotIdx][tp_Player1] = INVALID_PLAYER_ID;
        g_TourneyParticipants[slotIdx][tp_Player1_SQL] = acc_id;
        strcopy(g_TourneyParticipants[slotIdx][tp_Player1_Name], acc_name, MAX_PLAYER_NAME + 1);

        g_TourneyData[t_RegisteredCount]++;

        new appIdx2 = -1;
        for(new i = 0; i < MAX_DISCORD_TOURNEY_APPS; i++)
        {
            if(g_TourneyDiscordApps[i][tda_Active] && !strcmp(g_TourneyDiscordApps[i][tda_Nick], acc_name, true))
            {
                appIdx2 = i;
                break;
            }
        }
        if(appIdx2 == -1)
        {
            for(new i = 0; i < MAX_DISCORD_TOURNEY_APPS; i++)
            {
                if(!g_TourneyDiscordApps[i][tda_Active])
                {
                    appIdx2 = i;
                    break;
                }
            }
        }

        if(appIdx2 != -1)
        {
            g_TourneyDiscordApps[appIdx2][tda_Active] = true;
            strcopy(g_TourneyDiscordApps[appIdx2][tda_Nick], acc_name, MAX_PLAYER_NAME + 1);
            strcopy(g_TourneyDiscordApps[appIdx2][tda_UCP], "Verified Discord", 128);
            strcopy(g_TourneyDiscordApps[appIdx2][tda_DiscordID], userid, DCC_ID_SIZE);
            strcopy(g_TourneyDiscordApps[appIdx2][tda_DiscordTag], username, DCC_USERNAME_SIZE);
            g_TourneyDiscordApps[appIdx2][tda_Timestamp] = gettime();
            g_TourneyDiscordAppCount++;
        }

        new appEmbed2[1024];
        format(appEmbed2, sizeof(appEmbed2),
            "**Applicant Details**\n"\
            "- **In-Game Nick:** `%s`\n"\
            "- **Discord Member:** <@%s> (`%s`)\n"\
            "- **Account Status:** :white_check_mark: Verified Account (ID: `%d`)\n\n"\
            "**Tournament Status**\n"\
            "- **Event:** `%s`\n"\
            "- **Slot Assigned:** `#%02d`\n"\
            "- **Registered Slots:** `%d / %d`\n"\
            "- **Application Time:** `%s`",
            acc_name, userid, username, acc_id,
            g_TourneyData[t_Name],
            slotIdx + 1,
            g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots],
            returnDate(gettime())
        );

        utf8encode(msgencoded, appEmbed2, sizeof(msgencoded));
        SendDiscordEmbed(ch_tourney_applications, ":pencil: Tournament Registration Received", msgencoded, DISCORD_COLOR_SUCCESS);

        new confirmMsg2[256];
        format(confirmMsg2, sizeof(confirmMsg2), ":white_check_mark: <@%s> Your registration with nick **%s** is confirmed! (Slot #%02d - **%d/%d** slots filled).", userid, acc_name, slotIdx + 1, g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, confirmMsg2);

        va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s joined the tournament via Discord! ({MAIN}%d/%d {FFFFFF}slots filled).",
            acc_name, g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);

        if(g_TourneyData[t_AutoStart] && g_TourneyData[t_RegisteredCount] >= g_TourneyData[t_MaxSlots])
        {
            g_TourneyData[t_State] = TOURNAMENT_STATE_STARTING;
            StartTournamentCountdown(0);
        }
        return 1;
    }

    new accQuery[128];
    mysql_format(dbHandle, accQuery, sizeof(accQuery), "SELECT `id`, `username` FROM `accounts` WHERE `username` = '%e' LIMIT 1", nick);
    MySQL_TQueryInline(dbHandle, using inline OnTourneyCheckAccount, accQuery);
    return 1;
}
