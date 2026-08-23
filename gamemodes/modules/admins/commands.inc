
enum E_ADMIN_CMD_DATA {
    cmd_name[64],
    cmd_desc[128],
    cmd_level
}

static const AdminCmdsList[][E_ADMIN_CMD_DATA] = {
    {"/alogin [code]", "Authenticate as admin (Use without code to generate).", 0},
    {"/a [message]", "Global administrative message.", 1},
    {"/aka [id/name]", "Check possible accounts of a player, online or offline.", 1},
    {"/al [id] [message]", "Send an anonymous alert to a player.", 1},
    {"/avtn [message]", "Send an on-screen notification to everyone.", 1},
    {"/baninfo [nick/ip]", "Check details of a banned account or IP.", 1},
    {"/banomp [id]", "Force open.mp launcher on a player.", 1},
    {"/cnn [text]", "Send an on-screen announcement (# ~p~Nick~p~: ~w~Text #).", 1},
    {"/nickcolor [color]", "Change nickname color.", 1},
    {"/count", "Global administrative message.", 1},
    {"/d [id] [message]", "Answer a player question (/ask).", 1},
    {"/eject [id]", "To eject the player from the vehicle.", 1},
    {"/exitint", "Exit interior viewing mode.", 1},
    {"/inject [id]", "Giving a ride to a player (Put in vehicle).", 1},
    {"/interiors", "Interiors viewer.", 1},
    {"/kick [id] [reason]", "Kick a player.", 1},
    {"/lc", "Clear the chat.", 1},
    {"/mute [id] [reason]", "Mute a player.", 1},
    {"/names [id]", "Check a player's name history.", 1},
    {"/players", "List online players stats.", 1},
    {"/r [id] [message]", "Reply to a player report (/report).", 1},
    {"/recentips", "To see the list of most recently connected ips on the server.", 1},
    {"/reports", "List the latest reports with the accused player's live status.", 1},
    {"/saveint", "Save current interior coordinates.", 1},
    {"/setlounge [id]", "Set player or lounge position.", 1},
    {"/slap [id]", "Slap a player (don't abuse).", 1},
    {"/suspects", "Players flagged by the anti-cheat, plus banned/camouflaged IPs.", 1},
    {"/temprec [id]", "Demand a player to record.", 1},
    {"/unmute [id]", "Unmute a player.", 1},
    {"/acinfo [id/nickname]", "Anti cheat report of a player, with history.", 2},
    {"/ban [id] [hours] [reason]", "Ban a player.", 2},
    {"/changearena [id]", "Change one to a specified arena id.", 2},
    {"/clist", "Map list of the Custom Room (used in place of /maplist there).", 2},
    {"/freeze [id]", "Freezes a player.", 2},
    {"/logins", "View the list of most recent player logins.", 2},
    {"/lookup [id/nickname/ip]", "IP and ISP info of a player, online or offline.", 2},
    {"/maplist", "List of the current arenas.", 2},
    {"/offban [id] [hours] [reason]", "Ban a offline player.", 2},
    {"/randomchangearena", "Change to a random id arena.", 2},
    {"/setmoney [id] [money]", "Set player's money.", 2},
    {"/spawn [id]", "Spawn a player.", 2},
    {"/unban [player/ip]", "Unban a player.", 2},
    {"/unbanomp [name]", "Unban a forced open.mp player.", 2},
    {"/unfreeze [id]", "Unfreeze a player.", 2},
    {"/aconfig", "Admin settings.", 3},
    {"/fakeban [id/nickname] [hours] [reason]", "Fake ban a player.", 3},
    {"/get [id]", "Pull a player in their position.", 3},
    {"/go [id]", "Go on a player position.", 3},
    {"/banisp [id]", "Ban a player's ISP.", 4},
    {"/bannick [nick] [reason]", "Ban a specific nick.", 4},
    {"/banserial [id/name] [reason]", "Ban a player's serial number (GPCI).", 4},
    {"/camouflage", "Camouflage admin tag in stats.", 4},
    {"/changebanreason [name] [reason]", "Changes the ban reason.", 4},
    {"/changebantime [name] [amount] [h/d]", "Changes the ban expiry time.", 4},
    {"/kickall", "Disconnect all players except executing admin.", 4},
    {"/fake [nick]", "Change your nick to a fake one.", 4},
    {"/fakecmd [id/name] [cmd]", "Force a player to execute a command.", 4},
    {"/forcetext [id] [text]", "Force a player to say something.", 4},
    {"/giveach [id] [ach id]", "Grant an exclusive achievement (server owner only).", 4},
    {"/givegun [id] [weapon] [ammo]", "Give a weapon to a player (/giveweapon).", 4},
    {"/givevip [id] [days]", "Grant VIP status to a player (0 = permanent).", 4},
    {"/gmx", "Restart server.", 4},
    {"/gpciaka [id/name]", "Check accounts related to a player's GPCI.", 4},
    {"/hidetab", "Hide or show yourself on the TAB list and radar.", 4},
    {"/hostname [new name]", "Changes the server name.", 4},
    {"/lockserver [pass]", "Locks the server with a password.", 4},
    {"/penalties [id]", "Displays penalty history.", 4},
    {"/removevip [id]", "Remove VIP status from a player.", 4},
    {"/resetaccount [id/name]", "Reset all account data.", 4},
    {"/saveaccounts", "Save all accounts.", 4},
    {"/setadmin [id] [level]", "Promote/Demote a player.", 4},
    {"/setinterior [id] [interior]", "Set player's interior.", 4},
    {"/setnick [id] [new_nick]", "Change a player's nickname without cost.", 4},
    {"/setvw [id] [vw]", "Set player's virtual world.", 4},
    {"/slimban [id/name] [hours]", "Ban a player as Anti-Cheat (Rejecting-AC).", 4},
    {"/unbangpci [serial]", "Unban a specific GPCI serial.", 4},
    {"/unbanisp [name]", "Unban an ISP name.", 4},
    {"/unbannick [nick]", "Unban a specific nick.", 4},
    {"/unlockserver", "Unlocks the server.", 4},
    {"/viewpass [id/name]", "Show player password (hash).", 4},
    {"/warn [id] [reason]", "Give a warning to a player. (5 = ban).", 4},
    {"/weekresults", "Broadcast weekly results to Discord.", 4},
    {"/ac", "Anti cheat settings: actions, punishment and flagged players.", 5},
    {"/actest", "Anti cheat tests: simulation of cheat detections.", 5},
    {"/acdebug", "Anti cheat debug.", 5},
    {"/eaccount [id/name]", "Account Editor.", 5},
    {"/editsafehouse", "Open the safehouse panel: create, place, edit and configure bases.", 5},
    {"/givecustomtag [id]", "Toggle custom tag permission for a player.", 5},
    {"/lifez [height]", "Calibrate the height of the custom life bar above the head.", 5},
    {"/setcoins [id/name] [amount]", "Set the Coins balance of an account (works offline).", 5}
};

@cmd() alogin(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    if(User[playerid][isAdminLogged])
        return SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}You are already logged in as an administrator!");

    if(User[playerid][adminAuthType] == 1)
    {
        StartAdminPasswordAuth(playerid);
        return true;
    }

    new code;
    if(sscanf(params, "d", code))
    {
        GenerateAdminAuthCode(playerid);
        return true;
    }

    if(code != User[playerid][adminAuthCode] || code == 0)
        return SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}Incorrect authentication code!");

    if(gettime() > User[playerid][adminAuthTimer])
    {
        User[playerid][adminAuthCode] = 0;
        return SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}Your code has expired! Generate a new one using {00EEFE}/alogin{FFFFFF}.");
    }

    User[playerid][isAdminLogged] = true;
    User[playerid][adminAuthTimer] = 0;
    User[playerid][adminAuthCode] = 0;
    SendServerInfoMessage(playerid, ADMIN_LOGIN_WELCOME, User[playerid][userName]);
    
    new log_details[128];
    format(log_details, sizeof(log_details), "Admin %s logged in successfully (2FA)", User[playerid][userName]);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "auth", "Admin Login", log_details);
    return true;
}

forward DIALOG_ADMINCMDS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADMINCMDS(playerid, dialogid, response, listitem, string:inputtext[])
{
    new current_page = GetPVarInt(playerid, "acmds_page");
    if(!response)
    {
        if(current_page > 0)
        {
            SetPVarInt(playerid, "acmds_page", current_page - 1);
            ShowAdminCmds(playerid);
        }
        return 1;
    }

    if(strfind(inputtext, "Next Page") != -1)
    {
        SetPVarInt(playerid, "acmds_page", current_page + 1);
        ShowAdminCmds(playerid);
    }
    return 1;
}

ShowAdminCmds(playerid)
{
    new cmdsstr[3000];
    strcat(cmdsstr, "Command\tDescription\n");

    new page = GetPVarInt(playerid, "acmds_page");
    new items_per_page = 15;
    new start_index = page * items_per_page;
    new count = 0;
    new shown_count = 0;
    new total_available = 0;

    for(new i = 0; i < sizeof(AdminCmdsList); i++)
    {
        if(User[playerid][adminLevel] >= AdminCmdsList[i][cmd_level])
        {
            total_available++;
        }
    }

    for(new i = 0; i < sizeof(AdminCmdsList); i++)
    {
        if(User[playerid][adminLevel] >= AdminCmdsList[i][cmd_level])
        {
            if(count >= start_index && shown_count < items_per_page)
            {
                strcat(cmdsstr, "{36cf85}");
                strcat(cmdsstr, AdminCmdsList[i][cmd_name]);
                strcat(cmdsstr, "\t{BEBEBE}");
                strcat(cmdsstr, AdminCmdsList[i][cmd_desc]);
                strcat(cmdsstr, "\n");
                shown_count++;
            }
            count++;
        }
    }

    if(start_index + items_per_page < total_available)
    {
        strcat(cmdsstr, ""COL_KGREEN">>> Next Page\tGo next\n");
    }

    Dialog_ShowCallback(playerid, using public DIALOG_ADMINCMDS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, sprintf("{FF4500}Commands Admin - Page {23C552}%d", page + 1), cmdsstr, "Select", (page > 0) ? ("<") : ("Close"));
    return 1;
}

@cmd(.alts = {"aa"}) acmds(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    SetPVarInt(playerid, "acmds_page", 0);
    ShowAdminCmds(playerid);
    return true;
}

@cmd(.alts = {"ans", "askreply"}) d(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    new targetid, message[128];
    if(sscanf(params, "us[128]", targetid, message))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}/d [id] [message]");

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Player not connected or not logged in!");

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot reply to yourself!");

    if(strlen(message) > 100)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Response too long!");

    new targetIsVip = IsPlayerVip(targetid);
    new askTag[16] = "[ASK]", askColor[10] = "{MAIN}";
    if(targetIsVip)
    {
        strcopy(askTag, "[VIP ASK]");
        strcopy(askColor, "{FFD700}");
    }

    va_SendClientMessage(playerid, -1, "{MAIN}%s %sResponse sent to %s[%d]: %s", askTag, askColor, User[targetid][userName], targetid, message);
    va_SendClientMessage(targetid, -1, "{MAIN}%s %sAdmin %s[%d] answered: %s", askTag, askColor, User[playerid][userName], playerid, message);

    GameTextForPlayer(targetid, "~p~~h~Ask Answered", 1500, 1);
    GameTextForPlayer(playerid, "~g~~h~Ask Answered", 1500, 1);
    PlayerPlaySound(targetid, 1085, 0.0, 0.0, 10.0);

    foreach(new i : Player)
    {
        if(User[i][adminLevel] > 0 && i != playerid && i != targetid)
        {
            va_SendClientMessage(i, -1, "{MAIN}%s %s%s answered %s: %s", askTag, askColor, User[playerid][userName], User[targetid][userName], message);
        }
    }

    format(msgdiscord, sizeof(msgdiscord), "**Admin:** %s\n**Target:** %s\n**Message:** %s", User[playerid][userName], User[targetid][userName], message);
    utf8encode(msgencoded, msgdiscord, sizeof(msgencoded));
    SendDiscordEmbed(logs_ask, "Question Reply", msgencoded, DISCORD_COLOR_INFO);

    return true;
}

@cmd(.alts = {"rep", "reply"}) r(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    new targetid, message[128];
    if(sscanf(params, "us[128]", targetid, message))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}/r [id] [message]");

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Player not connected or not logged in!");

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot reply to yourself!");

    if(strlen(message) > 100)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Response too long!");

    new targetIsVip = IsPlayerVip(targetid);
    new repTag[16] = "[REPORT]", repColor[10] = "{MAIN}";
    if(targetIsVip)
    {
        strcopy(repTag, "[VIP REPORT]");
        strcopy(repColor, "{FFD700}");
    }

    va_SendClientMessage(playerid, -1, "{MAIN}%s %sReply sent to %s[%d]: %s", repTag, repColor, User[targetid][userName], targetid, message);
    va_SendClientMessage(targetid, -1, "{MAIN}%s %sAdmin %s[%d] replied: %s", repTag, repColor, User[playerid][userName], playerid, message);

    GameTextForPlayer(targetid, "~b~~h~Report Answered", 1500, 1);
    GameTextForPlayer(playerid, "~g~~h~Report Answered", 1500, 1);
    PlayerPlaySound(targetid, 1085, 0.0, 0.0, 10.0);

    foreach(new i : Player)
    {
        if(User[i][adminLevel] > 0 && i != playerid && i != targetid)
        {
            va_SendClientMessage(i, -1, "{MAIN}%s %s%s replied to %s: %s", repTag, repColor, User[playerid][userName], User[targetid][userName], message);
        }
    }

    format(msgdiscord, sizeof(msgdiscord), "**Admin:** %s\n**Target:** %s\n**Message:** %s", User[playerid][userName], User[targetid][userName], message);
    utf8encode(msgencoded, msgdiscord, sizeof(msgencoded));
    SendDiscordEmbed(logs_report, "Report Reply", msgencoded, DISCORD_COLOR_INFO);

    return true;
}

@cmd() setadmin(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 4)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    new idplayer, level, Stringa[128], string[128];

    if (sscanf(params, "ud", idplayer, level))
        return SendClientMessage(playerid, -1, "%s", CREATE_ADMIN_HELP);
    if (!IsPlayerConnected(idplayer) || !User[idplayer][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(idplayer == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");
    if (User[idplayer][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);
    if (level < 0 || level > 5)
        return SendClientMessage(playerid, -1, "%s", CREATE_ADMIN_HELP2);
    if (level == 5 && !IsPlayerAdmin(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Only RCON Administrators can promote players to Level 5 (Developer).");
    if (level > User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", CREATE_ADMIN_HELP3);

    new adminName[MAX_PLAYER_NAME + 1];
    strcopy(adminName, User[playerid][userName], sizeof(adminName));

    new targetName[MAX_PLAYER_NAME + 1];
    strcopy(targetName, User[idplayer][userName], sizeof(targetName));

    if (level == 0)
    {
        User[idplayer][adminLevel] = 0;
        SyncPlayerDiscordRoles(idplayer);

        format(Stringa, sizeof(Stringa), REMOVED_ADMIN_MSG, User[idplayer][userName]);
        SendClientMessage(playerid, -1, "%s", Stringa);

        format(Stringa, sizeof(Stringa), REMOVED_ADMIN_MSG_PLAYER, User[playerid][userName]);
        SendClientMessage(idplayer, -1, "%s", Stringa);

        foreach(new i : Player)
        {
            if(User[i][adminLevel] > 0 && i != idplayer)
            {
                format(string, sizeof(string), ADM_TAG_NORM"Administrator %s demoted %s to player!", adminName, targetName);
                SendClientMessage(i, -1, "%s", string);
            }
        }
        return true;
    }

    foreach(new i : Player)
    {
        if(User[i][adminLevel] > 0 && i != idplayer)
        {
            format(string, sizeof(string), ADM_TAG_NORM"Administrator %s %s %s to %s.",
                adminName, (User[idplayer][adminLevel] < level) ? ("promoted") : ("demoted"), targetName, adminnames[level][ingame]);
            SendClientMessage(i, -1, "%s", string);
        }
    }

    User[idplayer][adminLevel] = level;
    SyncPlayerDiscordRoles(idplayer);

    new log_details[256];
    format(log_details, sizeof(log_details), "%s player %s to Admin Level %d", (level > 0 ? "Promoted" : "Demoted"), targetName, level);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "promotions", (level > 0 ? "Promotion" : "Demotion"), log_details);

    format(Stringa, sizeof(Stringa), CREATED_ADMIN_MSG , targetName, level);
    SendClientMessage(playerid, -1, "%s", Stringa);

    format(Stringa, sizeof(Stringa), CREATED_ADMIN_MSG_PLAYER, adminName, level);
    SendClientMessage(idplayer, -1, "%s", Stringa);

    new logString[1024];
    format(logString, sizeof(logString), "**Admin:** %s\n**Target:** %s\n**Level:** %d", adminName, targetName, level);
    
    SendDiscordEmbed(logs_promotions, "Admin Promotion", logString, DISCORD_COLOR_ADMIN);

    return true;
}

@cmd(.alts = {"adminconfig"}) aconfig(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 3)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    ShowAdminConfig(playerid);
    return true;
}

@cmd() a(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 1) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new text[128];
    if(sscanf(params, "s[128]", text)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/a [text]");
    va_SendClientMessageToAll(-1, ADM_TAG_NORM"%s: %s", User[playerid][userName], text);
    return true;
}

@cmd() lc(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 1) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    foreach(new i : Player)
        ClearChat(i, 100);

    GameTextForPlayer(playerid, " ~n~~b~~h~Chat clean", 2000, 1);
    return true;
}

@cmd() givecustomtag(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;

    if(User[playerid][adminLevel] < 5)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need Admin Level 5 to use this command.");
        return true;
    }

    CheckAdminAuth(playerid);

    new targetid;
    if(sscanf(params, "r", targetid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/givecustomtag [playerid/name]");
        return true;
    }

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Target player is not connected or logged in.");
        return true;
    }

    User[targetid][customTagPerm] = !User[targetid][customTagPerm];

    new query[144];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET custom_tag_perm = %d WHERE id = %d", User[targetid][customTagPerm], User[targetid][user_sqlid]);
    mysql_tquery(dbHandle, query);

    if(User[targetid][customTagPerm])
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}You granted Custom Tag permission to %s.", getPlayerName(targetid));
        va_SendClientMessage(targetid, -1, "{MAIN}[TAG] {FFFFFF}Admin %s granted you permission to use Custom Tags! Type /tag.", getPlayerName(playerid));
    }
    else
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}You revoked Custom Tag permission from %s.", getPlayerName(targetid));
        va_SendClientMessage(targetid, -1, "{MAIN}[TAG] {FFFFFF}Your Custom Tag permission was revoked by Admin %s.", getPlayerName(playerid));
    }
    return true;
}

@cmd(.alts = {"sendweekresults", "weeklyresults"}) weekresults(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ADMIN] {FFFFFF}You do not have permission to use this command.");
    CheckAdminAuth(playerid);

    SendWeeklyResultsDiscord(false);
    SendClientMessage(playerid, COLOR_KGREEN, "{MAIN}[DISCORD] {FFFFFF}Weekly results have been broadcasted to the Discord results channel!");
    return true;
}


