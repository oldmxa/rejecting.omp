#if defined _clan_commands_included
    #endinput
#endif
#define _clan_commands_included

@cmd() createclan(playerid, params[], help)
{
    if (!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if (User[playerid][clanID] != -1) return SendClientMessage(playerid, -1, "%s", CLAN_INACLAN);
    if (User[playerid][playerMoney] < CLAN_PRICE) return SendClientMessage(playerid, -1, "%s", CLAN_NOMONEY);

    new rawTag[100], rawName[100];
    if (sscanf(params, "s[100]s[100]", rawTag, rawName))
        return SendClientMessage(playerid, COLOR_RED, CLAN_PREFIX"Usage: /createclan [tag] [name]");

    new tag[CLAN_TAG_LEN], name[CLAN_NAME_LEN];
    if (!SanitizeClanText(rawTag, tag, sizeof(tag), CLAN_TAG_MIN)) return SendClientMessage(playerid, -1, "%s", CLAN_TAGFAIL);
    if (!SanitizeClanText(rawName, name, sizeof(name), CLAN_NAME_MIN)) return SendClientMessage(playerid, -1, "%s", CLAN_NAMEFAIL);

    inline const OnCreateChecked(taken)
    {
        if (taken == CLAN_TEXT_FREE)
        {
            if (User[playerid][clanID] != -1)
                SendClientMessage(playerid, -1, "%s", CLAN_INACLAN);
            else if (User[playerid][playerMoney] < CLAN_PRICE)
                SendClientMessage(playerid, -1, "%s", CLAN_NOMONEY);
            else
            {
                new clanid = CreateClan(playerid, tag, name);

                if (clanid == -1)
                    SendClientMessage(playerid, -1, CLAN_PREFIX"The server reached the clan limit. Contact the administration.");
                else
                    va_SendClientMessageToAll(-1, CLAN_CREATED, ClanInfo[clanid][clanTag], ClanInfo[clanid][clanName]);
            }
        }
    }
    CheckClanText(playerid, tag, name, -1, using inline OnCreateChecked);

    return 1;
}

@cmd() clan(playerid, params[], help)
{
    if (!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if (User[playerid][clanID] == -1) return SendClientMessage(playerid, -1, "%s", CLAN_NOTAMEMBER);

    new option[10], value[10];
    if (!sscanf(params, "s[10]S()[10]", option, value) && !strcmp(option, "color", true))
    {
        if (!HasClanRank(playerid, RANK_COFOUNDER)) return SendClientMessage(playerid, -1, "%s", CLAN_LOWRANK);

        if (User[playerid][playerMoney] < CLAN_COLOR_PRICE)
            return va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need %s to change the clan color.", FormatMoney(CLAN_COLOR_PRICE));

        if (isnull(value)) return SendClientMessage(playerid, -1, CLAN_PREFIX"Usage: /clan color [HEX] (e.g., FF0000)");

        new colorVal;
        if (sscanf(value, "x", colorVal)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid HEX color.");

        GivePlayerCash(playerid, -CLAN_COLOR_PRICE);

        SetClanColorValue(playerid, User[playerid][clanID], (colorVal << 8) | CLAN_COLOR_ALPHA_CMD);
        return 1;
    }

    ShowClanMenu(playerid);
    return 1;
}

@cmd(.alts = {"inviteclan"}) invite(playerid, params[], help)
{
    if (!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if (User[playerid][clanID] == -1) return SendClientMessage(playerid, -1, "%s", CLAN_NOTAMEMBER);
    if (!HasClanRank(playerid, RANK_RECRUTER)) return SendClientMessage(playerid, -1, "%s", CLAN_NOPERMISSION);

    new targetid;
    if (sscanf(params, "u", targetid)) return SendClientMessage(playerid, -1, "%s", CLAN_INVITE);
    if (!IsPlayerLogged(targetid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if (targetid == playerid) return SendClientMessage(playerid, -1, "%s", CLAN_INYOURCLAN);
    if (User[targetid][clanID] == User[playerid][clanID]) return SendClientMessage(playerid, -1, "%s", CLAN_INYOURCLAN);
    if (User[targetid][clanID] != -1) return SendClientMessage(playerid, -1, "%s", CLAN_INACLAN1);

    new clanid = User[playerid][clanID];

    // slots come from the base allowance plus whatever the safehouse level adds
    if (ClanMembersCount[clanid] >= GetClanMemberSlots(clanid))
        return va_SendClientMessage(playerid, -1, CLAN_PREFIX"Your clan reached its %d member slots. Buy or upgrade a safehouse to get more.", GetClanMemberSlots(clanid));

    User[targetid][clanIDInvite] = clanid;
    User[targetid][clanInviteTime] = gettime();

    va_SendClientMessage(playerid, -1, CLAN_INVITESENT, User[targetid][userName]);
    va_SendClientMessage(targetid, -1, CLAN_INVITERECEIVED, User[playerid][userName], ClanInfo[clanid][clanName]);
    PlayerPlaySound(targetid, 5203, 0.0, 0.0, 0.0);
    return 1;
}

@cmd() accept(playerid, params[], help)
{
    if (!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if (User[playerid][clanID] != -1) return SendClientMessage(playerid, -1, "%s", CLAN_INACLAN);
    if (User[playerid][clanIDInvite] == -1) return SendClientMessage(playerid, -1, "%s", CLAN_NOINVITE);

    new clanid = User[playerid][clanIDInvite];

    if (!IsValidClanID(clanid))
    {
        User[playerid][clanIDInvite] = -1;
        return SendClientMessage(playerid, -1, "%s", CLAN_NOINVITE);
    }

    if (gettime() >= User[playerid][clanInviteTime] + CLAN_INVITE_TIMEOUT)
    {
        User[playerid][clanIDInvite] = -1;
        return SendClientMessage(playerid, -1, "%s", CLAN_NOINVITE);
    }

    if (ClanMembersCount[clanid] >= GetClanMemberSlots(clanid))
    {
        User[playerid][clanIDInvite] = -1;
        return SendClientMessage(playerid, -1, "%s", CLAN_FULL);
    }

    new slot = GetFreeClanSlotID(clanid);
    if (slot == -1)
    {
        User[playerid][clanIDInvite] = -1;
        return SendClientMessage(playerid, -1, "%s", CLAN_ERROR);
    }

    User[playerid][clanID] = clanid;
    User[playerid][clanRole] = RANK_TRIAL;
    User[playerid][clanIDInvite] = -1;

    AddClanMember(clanid, slot, playerid, RANK_TRIAL);

    foreach (new i : Player)
    {
        if (!IsPlayerLogged(i) || User[i][clanID] != clanid) continue;

        va_SendClientMessage(i, -1, CLAN_JOINED, GetPlayerColor(playerid) >>> 8, User[playerid][userName], GetPlayerColor(playerid) >>> 8, ClanInfo[clanid][clanName]);
        PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
    }

    SetClanColor(playerid);
    UpdatePlayerData(playerid);
    return 1;
}

@cmd() leaveclan(playerid, params[], help)
{
    if (!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    LeaveClan(playerid);
    return 1;
}

@cmd(.alts = {"clanslist"}) clans(playerid, params[], help)
{
    if (!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    ShowClansListDialog(playerid);
    return 1;
}

@cmd() territories(playerid, params[], help)
{
    if (!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    SetPVarInt(playerid, "terr_page", 0);
    ShowTerritories(playerid);
    return 1;
}

@cmd() clancmds(playerid, params[], help)
{
    if (!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    new title[64] = "{MAIN}Clan Commands";

    format_MainColor(playerid, title);
    new body[768];
    strcat(body, "{A29582}Command\t{A29582}Description\n");

    strcat(body, "{FFFFFF}/clan\t{BEBEBE}Open Clan management dashboard\n");
    strcat(body, fmat_return("{FFFFFF}/createclan [tag] [name]\t{BEBEBE}Create a new Clan (%s)\n", FormatMoney(CLAN_PRICE)));
    strcat(body, "{FFFFFF}/invite [id/name]\t{BEBEBE}Invite a player to your clan\n");
    strcat(body, "{FFFFFF}/accept\t{BEBEBE}Accept a clan invitation\n");
    strcat(body, "{FFFFFF}/leaveclan\t{BEBEBE}Leave your current clan\n");
    strcat(body, "{FFFFFF}/safemenu\t{BEBEBE}Clan safehouse & vault menu\n");
    strcat(body, "{FFFFFF}/exitsafehouse\t{BEBEBE}Leave safehouse interior\n");
    strcat(body, "{FFFFFF}/clans\t{BEBEBE}Global clan rankings & stats\n");
    strcat(body, "{FFFFFF}/territories\t{BEBEBE}Outposts & territories status\n");

    Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Close", "");
    return 1;
}
