
forward DIALOG_HISTORY_NAV(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HISTORY_NAV(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        new page = GetPVarInt(playerid, "hist_page");
        new type = GetPVarInt(playerid, "hist_type");
        
        if(strfind(inputtext, "Next Page") != -1)
        {
            if(type == 1) ShowNameHistory(playerid, GetPVarInt(playerid, "hist_target"), page + 1);
            else {
                new name[MAX_PLAYER_NAME + 1];
                GetPVarString(playerid, "hist_target_name", name, sizeof(name));
                ShowPenaltyHistory(playerid, name, page + 1);
            }
        }
        else if(strfind(inputtext, "Previous Page") != -1)
        {
            if(type == 1) ShowNameHistory(playerid, GetPVarInt(playerid, "hist_target"), page - 1);
            else {
                new name[MAX_PLAYER_NAME + 1];
                GetPVarString(playerid, "hist_target_name", name, sizeof(name));
                ShowPenaltyHistory(playerid, name, page - 1);
            }
        }
    }
    return 1;
}

@cmd() aka(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    new target[MAX_PLAYER_NAME + 1];
    if (sscanf(params, "s[25]", target))
        return SendClientMessage(playerid, -1, ADM_TAG_NORM"/aka [id/name]");

    new targetid = ResolvePlayerTarget(target);

    if(targetid != INVALID_PLAYER_ID)
    {
        PerformIPLookup(playerid, User[targetid][userName], User[targetid][user_regip], User[targetid][user_gpci]);
        return true;
    }

    new query[256];
    mysql_format(dbHandle, query, sizeof(query), "SELECT `regip`, `gpci`, `username` FROM `accounts` WHERE `username` = '%e' LIMIT 1", target);

    inline OnOfflineAKA()
    {
        if(cache_num_rows() > 0)
        {
            new ip[18], targetGpci[42], realname[MAX_PLAYER_NAME + 1];
            cache_get_value_name(0, "regip", ip, sizeof(ip));
            cache_get_value_name(0, "gpci", targetGpci, sizeof(targetGpci));
            cache_get_value_name(0, "username", realname, sizeof(realname));
            PerformIPLookup(playerid, realname, ip, targetGpci);
        }
        else
        {
            SendClientMessage(playerid, -1, ADM_TAG_NORM"Account not found.");
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnOfflineAKA, query);
    return true;
}

// anti-VPN
static stock ShowOnlineLookup(playerid, targetid)
{
    new strlookup[2048], title[64];

    format(title, sizeof(title), "{MAIN}Lookup: {FFFFFF}%s", getPlayerName(targetid));
    format_MainColor(playerid, title);

    if (isnull(User[targetid][Isp]))
    {
        new why[240];

        format(why, sizeof(why),
            "{FFFFFF}No network data was collected for this player.\n\n"\
            "{B4B5B7}IP: {FFFFFF}%s\n"\
            "{B4B5B7}GPCI / Serial: {FFFFFF}%s\n\n"\
            "{B4B5B7}Local addresses are skipped on purpose. Otherwise check the console for {FFFFFF}Status VPN{B4B5B7}, which reports a rejected lookup.",
            getPlayerIp(targetid), User[targetid][user_gpci]);

        return Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, title, why, "Close", "");
    }

    format(strlookup, sizeof(strlookup),
        "{MAIN}Field\t{MAIN}Value\n"\
        "{B4B5B7}IP\t{FFFFFF}%s\n"\
        "{B4B5B7}GPCI / Serial\t{FFFFFF}%s\n"\
        "{B4B5B7}ISP\t{FFFFFF}%s\n"\
        "{B4B5B7}Network\t{FFFFFF}%s\n"\
        "{B4B5B7}ISP Number\t{FFFFFF}%s\n"\
        "{B4B5B7}City\t{FFFFFF}%s\n"\
        "{B4B5B7}Region\t{FFFFFF}%s (%s)\n"\
        "{B4B5B7}Country\t{FFFFFF}%s (%s)\n"\
        "{B4B5B7}Coordinates\t{FFFFFF}%s, %s\n"\
        "{B4B5B7}Time Zone\t{FFFFFF}%s\n"\
        "{B4B5B7}VPN\t%s\n"\
        "{B4B5B7}Proxy\t%s",
        getPlayerIp(targetid),
        User[targetid][user_gpci],
        User[targetid][Isp],
        User[targetid][Network],
        User[targetid][IspNumber],
        User[targetid][City],
        User[targetid][Region], User[targetid][RegionCode],
        User[targetid][Country], User[targetid][CountryCode],
        User[targetid][Latitude], User[targetid][Longitude],
        User[targetid][TimeZone],
        User[targetid][VPN] == true ? (""COL_KGREEN"Yes") : (""COL_RED"No"),
        User[targetid][Proxy] == true ? (""COL_KGREEN"Yes") : (""COL_RED"No")
    );

    format_MainColor(playerid, strlookup);
    Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, strlookup, "Close", "");
    return true;
}

@cmd() lookup(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 2)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    new target[MAX_PLAYER_NAME + 1];
    if(sscanf(params, "s[25]", target))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}/lookup [id/nickname/ip]");

    new targetid = ResolvePlayerTarget(target);

    if(targetid != INVALID_PLAYER_ID)
    {
        if(targetid == playerid)
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

        return ShowOnlineLookup(playerid, targetid);
    }

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "SELECT * FROM `accounts` WHERE `username` = '%e' OR `regip` = '%e' OR `lastip` = '%e' LIMIT 1",
        target, target, target);
    mysql_tquery(dbHandle, queryGlobal, "OffDox", "i", playerid);

    return true;
}

stock ShowNameHistory(playerid, targetid, page = 0)
{
    if (!IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return false;

    SetPVarInt(playerid, "hist_target", targetid);
    SetPVarInt(playerid, "hist_page", page);
    SetPVarInt(playerid, "hist_type", 1);

    new limit = 15;
    new offset = page * limit;

    inline const OnNameChangesSelect()
    {
        new rows = cache_num_rows();
        if (rows > 0)
        {
            new chadString[3000] = "No.\tOld name\tNew name\tTimestamp";
            new count = (rows > limit) ? limit : rows;

            for (new i = 0; i < count; i++)
            {
                new oldName[20 + 1], newName[20 + 1], timestamp[32];
                cache_get_value(i, "old_name", oldName);
                cache_get_value(i, "new_name", newName);
                cache_get_value(i, "timestamp", timestamp);

                strcat(chadString, fmat_return("\n%d.\t%s\t%s\t%s", offset + i + 1, oldName, newName, timestamp), sizeof(chadString));
            }

            if (page > 0)
            {
                strcat(chadString, "\n"COL_RED"<<< Previous Page\t\t\tGo back");
            }
            if (rows > limit)
            {
                strcat(chadString, "\n"COL_KGREEN">>> Next Page\t\t\tGo next");
            }

            Dialog_ShowCallback(playerid, using public DIALOG_HISTORY_NAV<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, sprintf("{FF4500}%s's name changes - Page %d", User[targetid][userName], page + 1), chadString, "Select", "Close");
        }
        else
        {
            if(page > 0) ShowNameHistory(playerid, targetid, page - 1);
            else Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "Information", "This player has never changed his name.", "Close", "");
        }
    }
    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT old_name, new_name, timestamp FROM name_changes WHERE uid = %d ORDER BY timestamp DESC LIMIT %d OFFSET %d", User[targetid][user_sqlid], limit + 1, offset);
    MySQL_TQueryInline(dbHandle, using inline OnNameChangesSelect, queryGlobal);
    return true;
}

@cmd(.alts = {"nicks"}) names(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new id;
    if(sscanf(params, "u", id))
        return SendClientMessage(playerid, -1, "%s", NAMES_HELP);
    if(id == INVALID_PLAYER_ID)
        return SendClientMessage(playerid,-1, "%s", IS_NOT_CONNECTED);
    if (User[id][isLogged] == false)
        return SendClientMessage(playerid, -1, "%s", IS_NOT_LOGGED);

    ShowNameHistory(playerid, id);
    return true;
}

@cmd() inject(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 1) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new targetid;
    if(sscanf(params, "u", targetid)) return SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/inject [id/name]");
    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged]) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(targetid == playerid) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");
    if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be in a vehicle.");
    
    new vehicleid = GetPlayerVehicleID(playerid);
    PutPlayerInVehicle(targetid, vehicleid, 1); 
    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You gave a ride to %s.", User[targetid][userName]);
    va_SendClientMessage(targetid, -1, ADM_TAG_NORM"The admin %s gave you a ride.", User[playerid][userName]);
    return true;
}

@cmd() eject(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 1) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new targetid;
    if(sscanf(params, "u", targetid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}/eject [id] ou /ejetar [id]");
    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged]) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(targetid == playerid) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");
    if(!IsPlayerInAnyVehicle(targetid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The player is not in a vehicle.");
    
    RemovePlayerFromVehicle(targetid);
    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You ejected %s from the vehicle.", User[targetid][userName]);
    va_SendClientMessage(targetid, -1, ADM_TAG_NORM"The admin %s ejected you from the vehicle.", User[playerid][userName]);
    return true;
}

@cmd(.alts = {"gpci"}) gpciaka(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    new targetid;
    if (!sscanf(params, "u", targetid) && IsPlayerConnected(targetid))
    {
        new serial[65];
        format(serial, sizeof(serial), "%s", User[targetid][user_gpci]);
        PerformGPCILookup(playerid, User[targetid][userName], serial);
        return true;
    }

    if(isnull(params))
        return SendClientMessage(playerid, -1, ADM_TAG_NORM"/gpciaka [id/name]");

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `gpci`, `username` FROM `accounts` WHERE `username` = '%e' LIMIT 1", params);
    inline OnOfflineGPCI()
    {
        if(cache_num_rows() > 0)
        {
            new serial[65], realname[MAX_PLAYER_NAME + 1];
            cache_get_value_name(0, "gpci", serial, sizeof(serial));
            cache_get_value_name(0, "username", realname, sizeof(realname));
            PerformGPCILookup(playerid, realname, serial);
        }
        else
        {
            SendClientMessage(playerid, -1, ADM_TAG_NORM"Account not found.");
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnOfflineGPCI, queryGlobal);
    return true;
}

@cmd() recentips(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    inline OnRecentIPs()
    {
        if(cache_num_rows() > 0)
        {
            new str[2048], username[MAX_PLAYER_NAME + 1], ip[18], date_str[32];
            strcat(str, "Player\tIP\tDate\n");
            for(new i = 0; i < cache_num_rows(); i++)
            {
                cache_get_value_name(i, "username", username);
                cache_get_value_name(i, "lastip", ip);
                cache_get_value_name(i, "date_str", date_str);
                strcat(str, fmat_return("{FFFFFF}%s\t{BEBEBE}%s\t{BEBEBE}%s\n", username, ip, date_str));
            }
            Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Recent Connections", str, "Close", "");
        }
        else
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}No recent connections found.");
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnRecentIPs, "SELECT username, lastip, FROM_UNIXTIME(last_login, '%%d/%%m/%%Y %%H:%%i') as date_str FROM accounts ORDER BY last_login DESC LIMIT 20");
    return true;
}

@cmd() logins(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 2)
        return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    inline OnLoginsQuery()
    {
        new rows = cache_num_rows();
        if(rows > 0)
        {
            new str[4096], username[MAX_PLAYER_NAME + 1], ip[18], date_str[32], acc_id;
            strcat(str, "ID\tName\tIP\tStatus\tLast Login\n");

            for(new i = 0; i < rows; i++)
            {
                cache_get_value_int(i, "id", acc_id);
                cache_get_value_name(i, "username", username);
                cache_get_value_name(i, "lastip", ip);
                cache_get_value_name(i, "date_str", date_str);

                new online_id = GetOnlinePlayerIDByName(username);

                if(online_id != INVALID_PLAYER_ID)
                {
                    strcat(str, fmat_return("{36cf85}#%d\t{FFFFFF}%s\t{BEBEBE}%s\t{36cf85}Online [%d]\t{BEBEBE}%s\n", acc_id, username, ip, online_id, date_str));
                }
                else
                {
                    strcat(str, fmat_return("{BEBEBE}#%d\t{FFFFFF}%s\t{BEBEBE}%s\t{E74C3C}Offline\t{BEBEBE}%s\n", acc_id, username, ip, date_str));
                }
            }

            Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, sprintf("{FF4500}Recent Player Logins ({23C552}%d{FF4500})", rows), str, "Close", "");
        }
        else
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}No recent logins found.");
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnLoginsQuery, "SELECT id, username, lastip, FROM_UNIXTIME(last_login, '%%d/%%m/%%Y %%H:%%i') as date_str FROM accounts ORDER BY last_login DESC LIMIT 30");
    return true;
}

static stock AppendACSuspects(dest[], size)
{
    new added = 0, reason[32], details[96], ageStr[16];

    foreach(new i : Player)
    {
        if(!User[i][isLogged]) continue;
        if(ACSuspect[i][ac_rejectedHits] < 1 && ACSuspect[i][ac_weaponFlags] < 1 && ACSuspect[i][ac_cheatFlags] < 1) continue;

        new elapsed = gettime() - ((ACSuspect[i][ac_cheatFlags] > 0) ? ACSuspect[i][ac_lastCheatTime] : ACSuspect[i][ac_lastTime]);
        if(elapsed < 60) format(ageStr, sizeof(ageStr), "%ds ago", elapsed);
        else format(ageStr, sizeof(ageStr), "%dm ago", elapsed / 60);

        if(ACSuspect[i][ac_cheatFlags] > 0)
        {
            format(reason, sizeof(reason), "{F84F31}Client");
            format(details, sizeof(details), "{BEBEBE}%dx %s (%s)",
                ACSuspect[i][ac_cheatFlags], ACSuspect[i][ac_lastCheat], ageStr);
        }
        else if(ACSuspect[i][ac_weaponFlags] > 0)
        {
            format(reason, sizeof(reason), "{FF0000}Weapon");

            if(ACSuspect[i][ac_rejectedHits] > 0)
            {
                format(details, sizeof(details), "{BEBEBE}%dx %s +%d rejected (%s)",
                    ACSuspect[i][ac_weaponFlags], ACSuspect[i][ac_lastWeapon], ACSuspect[i][ac_rejectedHits], ageStr);
            }
            else
            {
                format(details, sizeof(details), "{BEBEBE}%dx %s (%s)",
                    ACSuspect[i][ac_weaponFlags], ACSuspect[i][ac_lastWeapon], ageStr);
            }
        }
        else
        {
            format(reason, sizeof(reason), "{FFA500}Rejected Hits");
            format(details, sizeof(details), "{BEBEBE}%dx %s (%s)",
                ACSuspect[i][ac_rejectedHits], ACSuspect[i][ac_lastReason], ageStr);
        }

        strcat(dest, fmat_return("{FFFFFF}%s\t%s\t%s\n", User[i][userName], reason, details), size);
        added++;
    }
    return added;
}

@cmd() suspects(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    inline OnSuspectsQuery()
    {
        new str[2048], listed = 0;
        strcat(str, "Player\tReason\tDetails\n");

        if(cache_num_rows() > 0)
        {
            new username[MAX_PLAYER_NAME + 1], reason[32], details[64], bool:vpn, bool:proxy, lastip[18], banned_name[MAX_PLAYER_NAME + 1];

            for(new i = 0; i < cache_num_rows(); i++)
            {
                cache_get_value_name(i, "username", username);
                cache_get_value_name(i, "lastip", lastip);
                cache_get_value_int(i, "vpn", _:vpn);
                cache_get_value_int(i, "proxy", _:proxy);

                new bool:is_banned = false, bool:is_null = false;
                cache_is_value_name_null(i, "banned_username", is_null);

                if(!is_null)
                {
                    cache_get_value_name(i, "banned_username", banned_name);
                    is_banned = true;
                }

                if(is_banned)
                {
                    format(reason, sizeof(reason), "{FF0000}Ban Evader");
                    format(details, sizeof(details), "{BEBEBE}Linked to: %s", banned_name);
                }
                else if(vpn || proxy)
                {
                    format(reason, sizeof(reason), "{FFFF00}VPN/Proxy");
                    format(details, sizeof(details), "{BEBEBE}IP: %s", lastip);
                }
                else continue;

                strcat(str, fmat_return("{FFFFFF}%s\t%s\t%s\n", username, reason, details), sizeof(str));
                listed++;
            }
        }

        listed += AppendACSuspects(str, sizeof(str));

        if(!listed)
            return SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}No suspicious players online.");

        Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Suspects Online", str, "Close", "");
    }

    new query[512];
    mysql_format(dbHandle, query, sizeof(query),
        "SELECT a.username, a.lastip, a.vpn, a.proxy, MAX(b.username) AS banned_username \
        FROM accounts a \
        LEFT JOIN banimentos b ON ((a.lastip <> '' AND a.lastip = b.ip) OR (a.gpci <> '' AND a.gpci = b.gpci)) \
        WHERE a.is_online = 1 AND (a.vpn = 1 OR a.proxy = 1 OR b.id IS NOT NULL) \
        GROUP BY a.id, a.username, a.lastip, a.vpn, a.proxy"
    );
    MySQL_TQueryInline(dbHandle, using inline OnSuspectsQuery, query);
    return true;
}

#define MAX_REPORTS_LIST      (30)
#define REPORT_REASON_PREVIEW (44)

new g_ReportListNames[MAX_PLAYERS][MAX_REPORTS_LIST][MAX_PLAYER_NAME + 1];
new g_ReportListCount[MAX_PLAYERS];
new g_reportsDialogBuf[3072];

forward DIALOG_REPORTS_LIST(playerid, dialogid, response, listitem, string:inputtext[]);

static stock ReportSanitize(text[], maxlen = 0)
{
    new len = strlen(text);

    for(new i = 0; i < len; i++)
    {
        switch(text[i])
        {
            case '\t', '\n', '\r': text[i] = ' ';
            case '{': text[i] = '(';
            case '}': text[i] = ')';
        }
    }

    if(maxlen >= 4 && len > maxlen)
    {
        text[maxlen - 3] = '.';
        text[maxlen - 2] = '.';
        text[maxlen - 1] = '.';
        text[maxlen] = '\0';
    }
    return 1;
}

@cmd(.alts = {"reportlist"}) reports(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    inline OnReportsList()
    {
        new rows = cache_num_rows();
        if(!rows)
        {
            return SendClientMessage(playerid, -1, "{MAIN}[REPORT] {FFFFFF}No reports registered yet.");
        }

        if(rows > MAX_REPORTS_LIST) rows = MAX_REPORTS_LIST;

        g_ReportListCount[playerid] = 0;
        g_reportsDialogBuf[0] = '\0';
        strcat(g_reportsDialogBuf, "{MAIN}ID\t{MAIN}Player\t{MAIN}Status\t{MAIN}Reason\n", sizeof(g_reportsDialogBuf));

        for(new i = 0; i < rows; i++)
        {
            new id, accused[MAX_PLAYER_NAME + 1], reason[256];
            new line[256], statusStr[32];

            cache_get_value_name_int(i, "id", id);
            cache_get_value_name(i, "accused_name", accused, sizeof(accused));
            cache_get_value_name(i, "description", reason, sizeof(reason));

            ReportSanitize(reason, REPORT_REASON_PREVIEW);

            if(GetOnlinePlayerIDByName(accused) == INVALID_PLAYER_ID)
                format(statusStr, sizeof(statusStr), "{F84F31}Offline");
            else
                format(statusStr, sizeof(statusStr), "{23C552}Online");

            format(line, sizeof(line), "{BEBEBE}#%d\t{FFFFFF}%s\t%s\t{BEBEBE}%s\n", id, accused, statusStr, reason);
            strcat(g_reportsDialogBuf, line, sizeof(g_reportsDialogBuf));

            strcopy(g_ReportListNames[playerid][g_ReportListCount[playerid]], accused, MAX_PLAYER_NAME + 1);
            g_ReportListCount[playerid]++;
        }

        new title[64];
        format(title, sizeof(title), "{MAIN}Reports {FFFFFF}(%d)", g_ReportListCount[playerid]);
        format_MainColor(playerid, title);
        format_MainColor(playerid, g_reportsDialogBuf, sizeof(g_reportsDialogBuf));

        Dialog_ShowCallback(playerid, using public DIALOG_REPORTS_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, g_reportsDialogBuf, "View", "Close");
        return 1;
    }

    new listQuery[192];
    format(listQuery, sizeof(listQuery),
        "SELECT `id`, `accused_name`, `description` FROM `tickets` WHERE `ticket_type` = 'report' ORDER BY `id` DESC LIMIT %d", MAX_REPORTS_LIST);

    MySQL_TQueryInline(dbHandle, using inline OnReportsList, listQuery);
    return true;
}

public DIALOG_REPORTS_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return 1;
    if(listitem < 0 || listitem >= g_ReportListCount[playerid]) return 1;

    new accused[MAX_PLAYER_NAME + 1];
    strcopy(accused, g_ReportListNames[playerid][listitem], sizeof(accused));

    new targetid = GetOnlinePlayerIDByName(accused);
    if(targetid == INVALID_PLAYER_ID)
    {
        return va_SendClientMessage(playerid, -1, "{MAIN}[REPORT] {FFFFFF}%s {BEBEBE}is offline - stats are only available while the player is connected.", accused);
    }
    
    ShowStats(playerid, targetid);
    return 1;
}
