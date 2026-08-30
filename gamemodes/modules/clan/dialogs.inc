#if defined _clan_dialogs_included
    #endinput
#endif
#define _clan_dialogs_included

enum
{
    CLAN_MENU_INFO,
    CLAN_MENU_MEMBERS,
    CLAN_MENU_SETTINGS,
    CLAN_MENU_LOGS,
    CLAN_MENU_SPAWN
};

enum
{
    CLAN_SET_ROLES,
    CLAN_SET_NAME,
    CLAN_SET_TAG,
    CLAN_SET_COLOR,
    CLAN_SET_LEAVE
};

stock ShowClanMenu(playerid)
{
    new clanid = User[playerid][clanID];
    if (!IsValidClanID(clanid)) return SendClientMessage(playerid, -1, "%s", CLAN_NOTAMEMBER);

    new body[1024];
    format(body, sizeof(body), "{MAIN}Option\t{MAIN}Description\n");
    strcat(body, "{MAIN}Information\t{BEBEBE}View clan stats, ID, tag, outposts & creation date\n");
    strcat(body, fmat_return("{MAIN}Members\t{BEBEBE}View & manage members ({23C552}%d Online{BEBEBE})\n", GetClanOnlineMembers(clanid)));
    strcat(body, "{MAIN}Settings\t{BEBEBE}Configure rank titles, name, tag & color\n");
    strcat(body, "{MAIN}Logs\t{BEBEBE}View recent match history and victories\n");

    new safehouseStatus[64];
    if (GetSafehouseByClan(clanid) == -1)
        format(safehouseStatus, sizeof(safehouseStatus), "{BEBEBE}Clan has no safehouse");
    else if (User[playerid][spawnSafehouse])
        format(safehouseStatus, sizeof(safehouseStatus), "{23C552}Enabled{BEBEBE} - spawn at the safehouse");
    else
        format(safehouseStatus, sizeof(safehouseStatus), "{F3230E}Disabled{BEBEBE} - spawn at the lounge");

    strcat(body, fmat_return("{MAIN}Safehouse Spawn\t%s", safehouseStatus));

    new title[128];
    format(title, sizeof(title), "{MAIN}Clan Dashboard: {FFFFFF}%s [%s]", ClanInfo[clanid][clanName], ClanInfo[clanid][clanTag]);

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_CLANMENU<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Cancel");
    return 1;
}

public DIALOG_CLANMENU(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;

    new clanid = User[playerid][clanID];
    if (!IsValidClanID(clanid)) return SendClientMessage(playerid, -1, "%s", CLAN_NOTAMEMBER);

    switch (listitem)
    {
        case CLAN_MENU_INFO:     ShowClanInfoDialog(playerid, clanid);
        case CLAN_MENU_MEMBERS:  ShowClanMembersDialog(playerid, 0);
        case CLAN_MENU_SETTINGS: ShowClanSettingsDialog(playerid, clanid);
        case CLAN_MENU_LOGS:     ShowClanLogsDialog(playerid, clanid);
        case CLAN_MENU_SPAWN:    ToggleClanSafehouseSpawn(playerid, clanid);
    }
    return 1;
}

stock ShowClanInfoDialog(playerid, clanid)
{
    if (!IsValidClanID(clanid)) return 0;

    new infoStr[1024], createdAt[32], clanColorHex[10];

    if (ClanInfo[clanid][clanCreatedAt] > 0)
        format(createdAt, sizeof(createdAt), "%s", returnDate(ClanInfo[clanid][clanCreatedAt], 1));
    else
        format(createdAt, sizeof(createdAt), "N/A");

    format(clanColorHex, sizeof(clanColorHex), "%06x", ClanInfo[clanid][clanColor] >>> 8);

    format(infoStr, sizeof(infoStr), "{MAIN}Parameter\t{MAIN}Value\n");
    strcat(infoStr, fmat_return("Clan Name\t{%s}%s\n", clanColorHex, ClanInfo[clanid][clanName]));
    strcat(infoStr, fmat_return("Clan ID\t{FFFFFF}%d\n", ClanInfo[clanid][clanID]));
    strcat(infoStr, fmat_return("Clan Tag\t{FFFFFF}[%s]\n", ClanInfo[clanid][clanTag]));
    strcat(infoStr, fmat_return("Created At\t{FFFFFF}%s\n", createdAt));
    strcat(infoStr, fmat_return("Total Members\t{FFFFFF}%d / %d\n", ClanMembersCount[clanid], GetClanMemberSlots(clanid)));
    strcat(infoStr, fmat_return("Online Members\t{23C552}%d Online\n", GetClanOnlineMembers(clanid)));
    strcat(infoStr, fmat_return("Territories / Points\t{FFD700}%d Outposts {FFFFFF}/ {FFD700}%d Points", GetClanOwnedOutposts(clanid), ClanInfo[clanid][clanPoints]));

    new title[128];
    format(title, sizeof(title), "{MAIN}Clan Info: {FFFFFF}%s [%s]", ClanInfo[clanid][clanName], ClanInfo[clanid][clanTag]);

    format_MainColor(playerid, title);
    format_MainColor(playerid, infoStr);
    Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, infoStr, "Close", "");
    return 1;
}

stock ShowClanSettingsDialog(playerid, clanid)
{
    if (!IsValidClanID(clanid)) return 0;

    new body[1024];
    format(body, sizeof(body), "{BEBEBE}Setting\t{BEBEBE}Permission / Cost\n");

    if (HasClanRank(playerid, RANK_COFOUNDER))
    {
        strcat(body, "{B4B5B7}\x95 {FFFFFF}Manage Roles\t{BEBEBE}Configure rank titles & permissions\n");
        strcat(body, "{B4B5B7}\x95 {FFFFFF}Change Clan Name\t{BEBEBE}Co-Founder+ Permission\n");
        strcat(body, "{B4B5B7}\x95 {FFFFFF}Change Clan Tag\t{BEBEBE}Co-Founder+ Permission\n");
        strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}Change Clan Color\t{BEBEBE}Default or Custom (%s)\n", FormatMoney(CLAN_COLOR_PRICE)));
    }
    else
    {
        strcat(body, "{808080}\x95 Manage Roles\t{808080}Co-Founder+ Required\n");
        strcat(body, "{808080}\x95 Change Clan Name\t{808080}Co-Founder+ Required\n");
        strcat(body, "{808080}\x95 Change Clan Tag\t{808080}Co-Founder+ Required\n");
        strcat(body, "{808080}\x95 Change Clan Color\t{808080}Co-Founder+ Required\n");
    }

    if (IsClanLeader(playerid))
        strcat(body, "{F84F31}\x95 Delete Clan\t{F84F31}Permanent Action");
    else
        strcat(body, "{F84F31}\x95 Leave Clan\t{F84F31}Exit this clan");

    new title[128];
    format(title, sizeof(title), "{MAIN}Clan Settings: {FFFFFF}%s", ClanInfo[clanid][clanName]);

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_CLAN_SETTINGS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
    return 1;
}

stock ShowClanLogsDialog(playerid, clanid)
{
    if (!IsValidClanID(clanid)) return 0;

    inline ShowLogs()
    {
        new rows;
        cache_get_row_count(rows);

        if (!rows)
        {
            new empty[64] = "{MAIN}No match logs recorded for your clan.";

            format_MainColor(playerid, empty);
            Dialog_ShowCallback(playerid, using public DIALOG_CLAN_LOGS_BACK<iiiis>, DIALOG_STYLE_MSGBOX, "Clan Logs", empty, "Back", "");
            return 1;
        }

        new body[4096], arena[64], winner[MAX_PLAYER_NAME + 1], kills, dmg, date_str[32];
        format(body, sizeof(body), "{MAIN}Date\t{MAIN}Arena\t{MAIN}Winner\t{MAIN}Kills\t{MAIN}Damage\n");

        for (new i = 0; i < rows; i++)
        {
            cache_get_value_name(i, "arena_name", arena);
            cache_get_value_name(i, "winner_name", winner);
            cache_get_value_int(i, "total_kills", kills);
            cache_get_value_int(i, "total_damage", dmg);
            cache_get_value_name(i, "date_str", date_str);

            strcat(body, fmat_return("{BEBEBE}%s\t{FFFFFF}%s\t{90C3D4}%s\t{FFD700}%d Kills\t{FF4500}%d DMG\n", date_str, arena, winner, kills, dmg));
        }

        new title[128];
        format(title, sizeof(title), "{MAIN}Clan Match Logs: {FFFFFF}%s", ClanInfo[clanid][clanName]);

        format_MainColor(playerid, title);
        format_MainColor(playerid, body);
        Dialog_ShowCallback(playerid, using public DIALOG_CLAN_LOGS_BACK<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Back", "");
        return 1;
    }

    MySQL_TQueryInline(dbHandle, using inline ShowLogs,
        "SELECT arena_name, winner_name, total_kills, total_damage, FROM_UNIXTIME(timestamp, '%%d/%%m %%H:%%i') as date_str FROM `arena_logs` WHERE `winner_clan` = '%e' ORDER BY `timestamp` DESC LIMIT %d",
        ClanInfo[clanid][clanName], CLAN_LOG_ROWS);
    return 1;
}

public DIALOG_CLAN_LOGS_BACK(playerid, dialogid, response, listitem, string:inputtext[])
{
    return ShowClanMenu(playerid);
}

stock ToggleClanSafehouseSpawn(playerid, clanid)
{
    if (GetSafehouseByClan(clanid) == -1)
    {
        if (User[playerid][spawnSafehouse])
        {
            User[playerid][spawnSafehouse] = false;
            UpdatePlayerData(playerid);
            SendClientMessage(playerid, -1, "{MAIN}[SAFEHOUSE] {FFFFFF}Your clan does not own a safehouse. Safehouse spawn has been disabled.");
            return ShowClanMenu(playerid);
        }
        SendClientMessage(playerid, -1, "{MAIN}[SAFEHOUSE] {FFFFFF}Your clan does not own a safehouse.");
        return ShowClanMenu(playerid);
    }

    User[playerid][spawnSafehouse] = !User[playerid][spawnSafehouse];
    UpdatePlayerData(playerid);

    if (User[playerid][spawnSafehouse])
        SendClientMessage(playerid, -1, "{MAIN}[SAFEHOUSE] {FFFFFF}You will now spawn inside the clan safehouse instead of the lounge.");
    else
        SendClientMessage(playerid, -1, "{MAIN}[SAFEHOUSE] {FFFFFF}You will now spawn at the lounge.");

    return ShowClanMenu(playerid);
}

public DIALOG_CLAN_SETTINGS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowClanMenu(playerid);

    new clanid = User[playerid][clanID];
    if (!IsValidClanID(clanid)) return 1;

    if (listitem != CLAN_SET_LEAVE && !HasClanRank(playerid, RANK_COFOUNDER))
        return ShowClanMenu(playerid);

    switch (listitem)
    {
        case CLAN_SET_ROLES: ShowClanRolesDialog(playerid, clanid);
        case CLAN_SET_NAME:
        {
            new body[512];
            format(body, sizeof(body), "{B4B5B7}Current name: "COL_WHITE"%s\n\n{B4B5B7}Requirements: "COL_WHITE"min %d and max %d characters", ClanInfo[clanid][clanName], CLAN_NAME_MIN, MAX_CLAN_NAME);
            Dialog_ShowCallback(playerid, using public DIALOG_CLAN_EDIT_NAME<iiiis>, DIALOG_STYLE_INPUT, "Clan Settings - Edit Clan Name", body, "Confirm", "Cancel");
        }
        case CLAN_SET_TAG:
        {
            new body[512];
            format(body, sizeof(body), "{B4B5B7}Current tag: "COL_WHITE"%s\n\n{B4B5B7}Requirements: "COL_WHITE"min %d and max %d characters", ClanInfo[clanid][clanTag], CLAN_TAG_MIN, MAX_CLAN_TAG);
            Dialog_ShowCallback(playerid, using public DIALOG_CLAN_EDIT_TAG<iiiis>, DIALOG_STYLE_INPUT, "Clan Settings - Edit Clan Tag", body, "Confirm", "Cancel");
        }
        case CLAN_SET_COLOR:
        {
            new body[200];
            strcat(body, "Default Colors\n");
            strcat(body, fmat_return("Custom HEX Color (%s)\n", FormatMoney(CLAN_COLOR_PRICE)));
            Dialog_ShowCallback(playerid, using public DIALOG_CLAN_COLOR_TYPE<iiiis>, DIALOG_STYLE_LIST, "Clan Settings - Choose Color Type", body, "Select", "Back");
        }
        case CLAN_SET_LEAVE:
        {
            if (IsClanLeader(playerid))
                DeleteClan(playerid, clanid);
            else
                LeaveClan(playerid);
        }
    }
    return 1;
}

stock ShowClanRolesDialog(playerid, clanid)
{
    if (!IsValidClanID(clanid)) return 0;

    new maxRole = (IsClanLeader(playerid)) ? MAX_ROLES : (MAX_ROLES - 1);

    new body[2500];
    format(body, sizeof(body), "{BEBEBE}Level\t{BEBEBE}Role Title\n");

    for (new i = 0; i < maxRole; i++)
        strcat(body, fmat_return("{B4B5B7}\x95 Level %d\t{FFFFFF}%s\n", i, ClanRoles[clanid][i]));

    new title[128];
    format(title, sizeof(title), "{MAIN}Clan Roles: {FFFFFF}%s", ClanInfo[clanid][clanName]);

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_CLAN_ROLES<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Edit", "Back");
    return 1;
}

public DIALOG_CLAN_ROLES(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowClanMenu(playerid);

    new clanid = User[playerid][clanID];
    if (!IsValidClanID(clanid)) return 1;

    new maxRole = (IsClanLeader(playerid)) ? MAX_ROLES : (MAX_ROLES - 1);
    if (!(0 <= listitem < maxRole)) return ShowClanMenu(playerid);

    SetPVarInt(playerid, "EditingRoleID", listitem);

    new body[512];
    format(body, sizeof(body), "{B4B5B7}Current name: "COL_WHITE"%s\n\n{B4B5B7}Requirements: "COL_WHITE"min %d and max %d characters", ClanRoles[clanid][listitem], CLAN_ROLE_MIN, MAX_ROLE_NAME);

    Dialog_ShowCallback(playerid, using public DIALOG_CLAN_EDIT_ROLE<iiiis>, DIALOG_STYLE_INPUT, "{MAIN}Clan - Edit Role Name", body, "Confirm", "Cancel");
    return 1;
}

public DIALOG_CLAN_EDIT_ROLE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;

    EditRoleName(playerid, User[playerid][clanID], GetPVarInt(playerid, "EditingRoleID"), inputtext);
    return 1;
}

public DIALOG_CLAN_EDIT_NAME(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;

    EditClanName(playerid, User[playerid][clanID], inputtext);
    return 1;
}

public DIALOG_CLAN_EDIT_TAG(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;

    EditClanTag(playerid, User[playerid][clanID], inputtext);
    return 1;
}

public DIALOG_CLAN_COLOR_TYPE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowClanMenu(playerid);

    new clanid = User[playerid][clanID];
    if (!IsValidClanID(clanid)) return 1;

    if (listitem == 0)
    {
        new body[1024];
        format(body, sizeof(body), "{BEBEBE}Color Preview\t{BEBEBE}Color Name\n");

        for (new i = 0; i < sizeof(DefaultClanColors); i++)
            strcat(body, fmat_return("{%06x}==========\t{FFFFFF}%s\n", DefaultClanColors[i] >>> 8, DefaultClanColorNames[i]));

        new title[128];
        format(title, sizeof(title), "{MAIN}Default Clan Colors: {FFFFFF}%s", ClanInfo[clanid][clanName]);

        format_MainColor(playerid, title);
        format_MainColor(playerid, body);
        Dialog_ShowCallback(playerid, using public DIALOG_CLAN_DEFAULT_COLORS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
        return 1;
    }

    new body[512];
    format(body, sizeof(body), "{FFFFFF}Current Color: {%06x}#%06x\n\n{BEBEBE}Enter your new 6-digit or 8-digit HEX color code below (e.g. {MAIN}FF0000{BEBEBE} or {MAIN}FF0000AA{BEBEBE}).\n{FFD700}Cost: %s",
        ClanInfo[clanid][clanColor] >>> 8, ClanInfo[clanid][clanColor] >>> 8, FormatMoney(CLAN_COLOR_PRICE));

    new title[128];
    format(title, sizeof(title), "{MAIN}Custom Clan Color: {FFFFFF}%s", ClanInfo[clanid][clanName]);

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_CLAN_EDIT_COLOR<iiiis>, DIALOG_STYLE_INPUT, title, body, "Confirm", "Cancel");
    return 1;
}

public DIALOG_CLAN_DEFAULT_COLORS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowClanMenu(playerid);

    new clanid = User[playerid][clanID];
    if (!IsValidClanID(clanid)) return 1;
    if (!(0 <= listitem < sizeof(DefaultClanColors))) return ShowClanMenu(playerid);
    if (!HasClanRank(playerid, RANK_COFOUNDER)) return SendClientMessage(playerid, -1, "%s", CLAN_LOWRANK);

    SetClanColorValue(playerid, clanid, DefaultClanColors[listitem]);
    SendClientMessage(playerid, -1, CLAN_PREFIX"Default color applied successfully!");

    ShowClanMenu(playerid);
    return 1;
}

public DIALOG_CLAN_EDIT_COLOR(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;

    new clanid = User[playerid][clanID];
    if (!IsValidClanID(clanid)) return SendClientMessage(playerid, -1, "%s", CLAN_NOTAMEMBER);
    if (!HasClanRank(playerid, RANK_COFOUNDER)) return SendClientMessage(playerid, -1, "%s", CLAN_LOWRANK);

    if (User[playerid][playerMoney] < CLAN_COLOR_PRICE)
        return va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need %s to change the clan color.", FormatMoney(CLAN_COLOR_PRICE));

    new parsedColor;
    if (!ParseHexColorRGBA(inputtext, parsedColor, CLAN_COLOR_ALPHA))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid format. Use 6 (RRGGBB) or 8 (RRGGBBAA) characters (0-9, A-F).");

    GivePlayerCash(playerid, -CLAN_COLOR_PRICE);

    SetClanColorValue(playerid, clanid, parsedColor);
    SendClientMessage(playerid, -1, CLAN_PREFIX"Custom color applied successfully!");
    return 1;
}

stock ShowClanMembersDialog(playerid, page)
{
    new clanid = User[playerid][clanID];
    if (!IsValidClanID(clanid)) return SendClientMessage(playerid, -1, "%s", CLAN_NOTAMEMBER);
    if (page < 0) page = 0;

    new members[MAX_MEMBERS];
    new count = GetClanMemberList(clanid, members);

    new start = page * CLAN_MEMBERS_PER_PAGE;
    new end = start + CLAN_MEMBERS_PER_PAGE;

    new body[2048];
    format(body, sizeof(body), "{MAIN}Member\t{MAIN}Role\t{MAIN}Status\t{MAIN}Kills\n");

    if (page > 0) strcat(body, "{FF0000}<<<\tPrevious Page\t \t \n");

    for (new i = start; i < end && i < count; i++)
    {
        new slot = members[i];
        new roleIdx = ClanMembers[clanid][slot][member_role];
        if (!IsValidClanRole(roleIdx)) roleIdx = RANK_TRIAL;

        strcat(body, fmat_return("{FFFFFF}%s\t{90C3D4}%s\t%s\t{FFD700}%d Kills\n",
            ClanMembers[clanid][slot][member_name],
            ClanRoles[clanid][roleIdx],
            IsAccountOnline(ClanMembers[clanid][slot][member_id]) ? ("{23C552}Online") : ("{808080}Offline"),
            ClanMembers[clanid][slot][member_score]));
    }

    if (count > end) strcat(body, "{00FF00}>>>\tNext Page\t \t \n");

    SetPVarInt(playerid, "ClanMemPage", page);

    new title[128];
    format(title, sizeof(title), "{MAIN}Clan Members: {FFFFFF}%s (Page %d)", ClanInfo[clanid][clanName], page + 1);

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_CLANMEMBERS_PAGED<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
    return 1;
}

stock GetClanMemberList(clanid, members[], size = sizeof(members))
{
    new count = 0;

    for (new i = 0; i < MAX_MEMBERS && count < size; i++)
    {
        if (ClanMembers[clanid][i][member_slotused]) members[count++] = i;
    }
    return count;
}

public DIALOG_CLANMEMBERS_PAGED(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowClanMenu(playerid);

    new page = GetPVarInt(playerid, "ClanMemPage");

    if (strfind(inputtext, "Next Page") != -1) return ShowClanMembersDialog(playerid, page + 1);
    if (strfind(inputtext, "Previous Page") != -1) return ShowClanMembersDialog(playerid, page - 1);

    new clanid = User[playerid][clanID];
    if (!IsValidClanID(clanid)) return 1;

    new members[MAX_MEMBERS];
    new count = GetClanMemberList(clanid, members);

    new index = listitem;
    if (page > 0) index--;

    new real_index = (page * CLAN_MEMBERS_PER_PAGE) + index;
    if (real_index < 0 || real_index >= count) return ShowClanMembersDialog(playerid, page);

    new slot = members[real_index];

    if (!HasClanRank(playerid, RANK_COFOUNDER)) return ShowClanMembersDialog(playerid, page);
    if (User[playerid][user_sqlid] == ClanMembers[clanid][slot][member_id]) return SendClientMessage(playerid, -1, "%s", CLAN_ERROR2);
    if (User[playerid][clanRole] <= ClanMembers[clanid][slot][member_role]) return SendClientMessage(playerid, -1, "%s", CLAN_ERROR3);

    SetPVarInt(playerid, "EditingClanPlayer", slot);

    new body[256];
    format(body, sizeof(body), "{MAIN}Action\t{MAIN}Role Level\n");

    for (new i = 0; i < MAX_ROLES - 1; i++)
        strcat(body, fmat_return("{MAIN}Set Role: %s\tLevel %d\n", ClanRoles[clanid][i], i));

    strcat(body, "{FF0000}Kick Member from Clan\t{FF4500}Action");

    new title[128];
    format(title, sizeof(title), "{MAIN}Manage Member: {FFFFFF}%s", ClanMembers[clanid][slot][member_name]);

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_CLAN_EDITMEMBER<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Confirm", "Back");
    return 1;
}

public DIALOG_CLAN_EDITMEMBER(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;

    new clanid = User[playerid][clanID];
    if (!IsValidClanID(clanid)) return 1;

    new slot = GetPVarInt(playerid, "EditingClanPlayer");
    if (!IsValidClanSlot(slot)) return 1;

    if (listitem == MAX_ROLES - 1)
        ChangePlayerRank(playerid, clanid, slot, -1);
    else if (0 <= listitem < MAX_ROLES - 1)
        ChangePlayerRank(playerid, clanid, slot, listitem);

    return 1;
}

stock ShowClansListDialog(playerid)
{
    new count = 0, body[4096];
    format(body, sizeof(body), "{MAIN}Tag\t{MAIN}Clan Name\t{MAIN}Leader\t{MAIN}Members\n");

    for (new i = 0; i < MAX_CLANS; i++)
    {
        if (!ClanInfo[i][clanIDUsed]) continue;

        g_PlayerClansList[playerid][count] = i;

        new colorHex[10], leaderName[MAX_PLAYER_NAME + 1];
        format(colorHex, sizeof(colorHex), "%06x", ClanInfo[i][clanColor] >>> 8);
        GetClanLeaderName(i, leaderName);

        strcat(body, fmat_return("[%s]\t{%s}%s\t{FFFFFF}%s\t{FFFFFF}%d/%d {23C552}(%d Online)\n",
            ClanInfo[i][clanTag],
            colorHex,
            ClanInfo[i][clanName],
            leaderName,
            ClanMembersCount[i],
            GetClanMemberSlots(i),
            GetClanOnlineMembers(i)));

        count++;
    }

    if (count == 0) return SendClientMessage(playerid, -1, CLAN_PREFIX"No clans found on the server.");

    new title[64] = "{MAIN}Server Clans";

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_CLANS_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Close");
    return 1;
}

public DIALOG_CLANS_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;
    if (!(0 <= listitem < MAX_CLANS)) return 1;

    new clanid = g_PlayerClansList[playerid][listitem];
    if (!IsValidClanID(clanid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid or non-existent clan.");

    g_PlayerSelectedClan[playerid] = clanid;
    ShowClanRecruitDialog(playerid, clanid);
    return 1;
}

stock ShowClanRecruitDialog(playerid, clanid)
{
    if (!IsValidClanID(clanid)) return 0;

    new infoStr[1024], createdAt[32], clanColorHex[10], leaderName[MAX_PLAYER_NAME + 1];
    GetClanLeaderName(clanid, leaderName);

    if (ClanInfo[clanid][clanCreatedAt] > 0)
        format(createdAt, sizeof(createdAt), "%s", returnDate(ClanInfo[clanid][clanCreatedAt], 1));
    else
        format(createdAt, sizeof(createdAt), "N/A");

    format(clanColorHex, sizeof(clanColorHex), "%06x", ClanInfo[clanid][clanColor] >>> 8);

    format(infoStr, sizeof(infoStr), "{MAIN}Parameter\t{MAIN}Value\n");
    strcat(infoStr, fmat_return("Clan Name\t{%s}%s\n", clanColorHex, ClanInfo[clanid][clanName]));
    strcat(infoStr, fmat_return("Clan Tag\t{FFFFFF}[%s]\n", ClanInfo[clanid][clanTag]));
    strcat(infoStr, fmat_return("Clan Leader\t{FFFFFF}%s\n", leaderName));
    strcat(infoStr, fmat_return("Created At\t{FFFFFF}%s\n", createdAt));
    strcat(infoStr, fmat_return("Total Members\t{FFFFFF}%d / %d\n", ClanMembersCount[clanid], GetClanMemberSlots(clanid)));
    strcat(infoStr, fmat_return("Online Members\t{23C552}%d Online\n", GetClanOnlineMembers(clanid)));
    strcat(infoStr, fmat_return("Territories / Points\t{FFD700}%d Outposts {FFFFFF}/ {FFD700}%d Points\n", GetClanOwnedOutposts(clanid), ClanInfo[clanid][clanPoints]));
    strcat(infoStr, "{MAIN}> Request Recruitment\t{BEBEBE}Send a request to join this clan");

    new title[128];
    format(title, sizeof(title), "{MAIN}Clan Info: {FFFFFF}%s [%s]", ClanInfo[clanid][clanName], ClanInfo[clanid][clanTag]);

    format_MainColor(playerid, title);
    format_MainColor(playerid, infoStr);
    Dialog_ShowCallback(playerid, using public DIALOG_CLAN_RECRUIT<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, infoStr, "Select", "Back");
    return 1;
}

public DIALOG_CLAN_RECRUIT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowClansListDialog(playerid);

    new clanid = g_PlayerSelectedClan[playerid];
    if (!IsValidClanID(clanid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid or non-existent clan.");

    if (strfind(inputtext, "Request Recruitment") == -1) return ShowClanRecruitDialog(playerid, clanid);

    if (IsPlayerInClan(playerid)) return SendClientMessage(playerid, -1, CLAN_PREFIX"You are already in a clan!");

    if (GetClanOnlineMembers(clanid) == 0)
        return va_SendClientMessage(playerid, -1, CLAN_PREFIX"There are no online members in clan {MAIN}%s {FFFFFF}at the moment. Try again later.", ClanInfo[clanid][clanName]);

    new notifyMsg[144];
    format(notifyMsg, sizeof(notifyMsg), "{MAIN}[RECRUITMENT] {FFFFFF}%s [ID: %d] requested to join your clan! Use {MAIN}/invite %d", User[playerid][userName], playerid, playerid);

    foreach (new i : Player)
    {
        if (!IsPlayerLogged(i) || User[i][clanID] != clanid) continue;

        SendClientMessage(i, -1, "%s", notifyMsg);
        PlayerPlaySound(i, 1054, 0.0, 0.0, 0.0);
    }

    va_SendClientMessage(playerid, -1, CLAN_PREFIX"Your recruitment request has been sent to the online members of clan {MAIN}%s{FFFFFF}!", ClanInfo[clanid][clanName]);
    PlaySuccessSound(playerid);
    return 1;
}

stock ShowTerritories(playerid)
{
    new body[4096], shown = 0;
    new page = GetPVarInt(playerid, "terr_page");
    new start_index = page * CLAN_TERRITORY_PER_PAGE;

    strcat(body, "Name\tProfit\tLock\tClan\n");

    for (new i = start_index; i < MAX_ZONES && shown < CLAN_TERRITORY_PER_PAGE; i++)
    {
        new clanid = ZoneOwnerClan[i];
        new clanStr[64], profitStr[16];

        switch (arenaData[i][category])
        {
            case ARENA_CATEGORY_BIG:    strcopy(profitStr, FormatMoney(CLAN_REWARD_BIG), sizeof(profitStr));
            case ARENA_CATEGORY_MEDIUM: strcopy(profitStr, FormatMoney(CLAN_REWARD_MEDIUM), sizeof(profitStr));
            case ARENA_CATEGORY_SMALL:  strcopy(profitStr, FormatMoney(CLAN_REWARD_SMALL), sizeof(profitStr));
            default:                    strcopy(profitStr, "$0", sizeof(profitStr));
        }

        if (IsValidClanID(clanid))
            format(clanStr, sizeof(clanStr), "{%06x}%s", ClanInfo[clanid][clanColor] >>> 8, ClanInfo[clanid][clanName]);
        else
            format(clanStr, sizeof(clanStr), "-");

        strcat(body, fmat_return("{FFFFFF}%s\t{BEBEBE}%s\t{BEBEBE}-\t%s\n", arenaData[i][arenaName], profitStr, clanStr));
        shown++;
    }

    if (page > 0) strcat(body, ""COL_RED"<<< Previous Page\t\t\t\n");
    if (start_index + CLAN_TERRITORY_PER_PAGE < MAX_ZONES) strcat(body, ""COL_KGREEN">>> Next Page\t\t\t\n");

    Dialog_ShowCallback(playerid, using public DIALOG_TERRITORIES<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, sprintf("{FF4500}Territories - Page %d", page + 1), body, "Select", "Close");
    return 1;
}

public DIALOG_TERRITORIES(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;

    new page = GetPVarInt(playerid, "terr_page");

    if (strfind(inputtext, "Next Page") != -1)
    {
        SetPVarInt(playerid, "terr_page", page + 1);
        ShowTerritories(playerid);
    }
    else if (strfind(inputtext, "Previous Page") != -1)
    {
        SetPVarInt(playerid, "terr_page", page - 1);
        ShowTerritories(playerid);
    }
    return 1;
}
