
forward DIALOG_AMENU(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_AMENU(playerid, dialogid, response, listitem, string:inputtext[])
{
    new action = 0;

    if(response)
    {
        new pvar_name[32];
        format(pvar_name, sizeof(pvar_name), "acfg_action_%d", listitem);
        action = GetPVarInt(playerid, pvar_name);
    }

    new pvCount = GetPVarInt(playerid, "acfg_count");
    for(new i = 0; i < pvCount; i++)
    {
        new p_name[32];
        format(p_name, sizeof(p_name), "acfg_action_%d", i);
        DeletePVar(playerid, p_name);
    }
    DeletePVar(playerid, "acfg_count");

    if(response)
    {
        switch(action)
        {
            case 0:
            {
                ShowAdminConfig(playerid);
                return 1;
            }
            case 1: // GMX
            {
                if(!IsAdmin(playerid, 4)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ required to restart.");
                
                SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s is restarting the server.", User[playerid][userName]);
                RestartServerSafe();
            }
            case 2: // Ping
            {
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MAXPING<iiiis>, DIALOG_STYLE_INPUT, "{BEBEBE}Max Ping", "{BEBEBE}Enter the new maximum ping allowed:", "*", "<");
            }
            case 3: // PL
            {
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MAXPL<iiiis>, DIALOG_STYLE_INPUT, "{BEBEBE}Max Packetloss", "{BEBEBE}Enter the new maximum packet loss allowed (float):", "*", "<");
            }
            case 4: // Min FPS
            {
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MINFPS<iiiis>, DIALOG_STYLE_INPUT, "{BEBEBE}Min FPS", "{BEBEBE}Enter the new minimum FPS allowed:", "*", "<");
            }
            case 12: // Max FPS
            {
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MAXFPS<iiiis>, DIALOG_STYLE_INPUT, "{BEBEBE}Max FPS", "{BEBEBE}Enter the new maximum FPS allowed:", "*", "<");
            }
            case 5: // Server Lock
            {
                if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ required to lock/unlock.");
                
                if(ServerLocked)
                {
                    ServerLocked = false;
                    SendRconCommand("password 0");
                    va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s unlocked the server.", User[playerid][userName]);
                    ShowAdminConfig(playerid);
                }
                else
                {
                    Dialog_ShowCallback(playerid, using public DIALOG_ADM_LOCKPASS<iiiis>, DIALOG_STYLE_INPUT, "{BEBEBE}Server Lock", "{BEBEBE}Enter the password to lock the server:", "*", "<");
                }
            }
            case 13: // Fake Version server-wide
            {
                if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ required to change this.");

                R2KickEnabled = !R2KickEnabled;
                SaveServerConfig();

                va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s %s the Fake Version client check kick.",
                    User[playerid][userName], (R2KickEnabled) ? ("enabled") : ("disabled"));

                ShowAdminConfig(playerid);
                PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
            }
            case 25: // open.mp Launcher server-wide
            {
                if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ required to change this.");

                OMPKickEnabled = !OMPKickEnabled;
                SaveServerConfig();

                va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s %s the open.mp launcher check kick.",
                    User[playerid][userName], (OMPKickEnabled) ? ("enabled") : ("disabled"));

                ShowAdminConfig(playerid);
                PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
            }
            case 16: // Max Warnings
            {
                if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MAXWARN<iiiis>, DIALOG_STYLE_INPUT, "{BEBEBE}Max Warnings", "{BEBEBE}Warnings before an automatic ban:", "*", "<");
            }
            case 17: // Max Login Attempts
            {
                if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MAXLOGIN<iiiis>, DIALOG_STYLE_INPUT, "{BEBEBE}Max Login Attempts", "{BEBEBE}Wrong passwords before the account is locked:", "*", "<");
            }
            case 18: // Max Accounts per IP/GPCI
            {
                if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MAXACC<iiiis>, DIALOG_STYLE_INPUT, "{BEBEBE}Max Accounts", "{BEBEBE}Accounts allowed per IP or serial (0 = no limit):", "*", "<");
            }
            case 22: // AC detection cooldown
            {
                if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

                new acStr[160];
                format(acStr, sizeof(acStr), "{FFFFFF}Current: {23C552}%d seconds\n\n{BEBEBE}Seconds before the same detection counts again (1-60):", ServerACCooldown);

                Dialog_ShowCallback(playerid, using public DIALOG_ADM_ACCOOLDOWN<iiiis>, DIALOG_STYLE_INPUT, "{BEBEBE}Anti Cheat Cooldown", acStr, "*", "<");
            }
            case 19: // Anti Spam
            {
                if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

                ServerAntiSpam = !ServerAntiSpam;
                SaveServerConfig();

                va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s %s the anti spam.", User[playerid][userName], (ServerAntiSpam) ? ("enabled") : ("disabled"));

                ShowAdminConfig(playerid);
                PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
            }
            case 20: // Anti Capslock
            {
                if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

                ServerAntiCapslock = !ServerAntiCapslock;
                SaveServerConfig();

                va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s %s the anti capslock.", User[playerid][userName], (ServerAntiCapslock) ? ("enabled") : ("disabled"));

                ShowAdminConfig(playerid);
                PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
            }
            case 21: // /admins command
            {
                if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

                ServerAdminsCmd = !ServerAdminsCmd;
                SaveServerConfig();

                va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s %s the /admins command.", User[playerid][userName], (ServerAdminsCmd) ? ("enabled") : ("disabled"));

                ShowAdminConfig(playerid);
                PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
            }
            case 6: // See PMs
            {
                User[playerid][acfg_pm] = !User[playerid][acfg_pm];
                UpdatePlayerData(playerid);
                SendClientMessage(playerid, -1, "%s", INFO_ADMINCONFIG);
                ShowAdminConfig(playerid);
                PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
            }
            case 7: // See Clan Chat
            {
                User[playerid][acfg_clanpm] = !User[playerid][acfg_clanpm];
                UpdatePlayerData(playerid);
                SendClientMessage(playerid, -1, "%s", INFO_ADMINCONFIG);
                ShowAdminConfig(playerid);
                PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
            }
            case 8: // See Commands
            {
                User[playerid][acfg_cmds] = !User[playerid][acfg_cmds];
                UpdatePlayerData(playerid);
                SendClientMessage(playerid, -1, "%s", INFO_ADMINCONFIG);
                ShowAdminConfig(playerid);
                PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
            }
            case 9: // See Local Chat
            {
                User[playerid][acfg_local] = !User[playerid][acfg_local];
                UpdatePlayerData(playerid);
                SendClientMessage(playerid, -1, "%s", INFO_ADMINCONFIG);
                ShowAdminConfig(playerid);
                PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
            }
            case 10: // Hostname (Level 4+ & RCON)
            {
                if(User[playerid][adminLevel] < 4 || !IsPlayerAdmin(playerid))
                    return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ and RCON login required.");

                Dialog_ShowCallback(playerid, using public DIALOG_ADM_HOSTNAME<iiiis>, DIALOG_STYLE_INPUT, "{BEBEBE}Server Name", "{BEBEBE}Enter the new server name to display in the launcher:", "*", "<");
            }
            case 11:
            {
                if(User[playerid][adminLevel] < 4 || !IsPlayerAdmin(playerid))
                    return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ and RCON login required.");

                new infoStr[128];
                format(infoStr, sizeof(infoStr), "{FFFFFF}Current Max Players: {23C552}%d\n\n{BEBEBE}Enter the new maximum player limit (1-1000):", GetMaxPlayers());
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MAXPLAYERS<iiiis>, DIALOG_STYLE_INPUT, "{BEBEBE}Max Slots", infoStr, "*", "<");
            }
            case 14: // Default Server HUD
            {
                if(User[playerid][adminLevel] < 5)
                    return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 5+ required to change default HUD style.");

                new string[512];
                strcopy(string, "{BEBEBE}ID\t{BEBEBE}HUD Style\n");
                for (new i = 0; i < sizeof(HUD_TYPES); i++)
                {
                    new bool:isSelected = (ServerDefaultHud == i);
                    strcat(string, fmat_return("%d\t%s%s\n", i, isSelected ? ("{23C552}> {FFFFFF}") : ("{B4B5B7}\x95 {FFFFFF}"), HUD_TYPES[i]));
                }
                format_MainColor(playerid, string);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_DEFAULTHUD<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "{BEBEBE}Default Server HUD", string, "*", "<");
            }
            case 15: // Default HUD Color
            {
                if(User[playerid][adminLevel] < 5)
                    return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 5+ required to change default HUD color.");

                new body[2048];
                BuildHudColorDialogList(body, sizeof(body), ServerDefaultHudColor);
                format_MainColor(playerid, body);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_DEFAULTHUDCOLOR<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "{BEBEBE}Default HUD Color", body, "*", "<");
            }
            case 23: // Default HUD Primary
            {
                if(User[playerid][adminLevel] < 5)
                    return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 5+ required to change the default HUD primary color.");

                new presetColor = (0 <= ServerDefaultHudColor < sizeof(hudPersonalization)) ? ServerDefaultHudColor : 0;
                new hex1[7];
                hex1 = GetColorHexTag(hudPersonalization[presetColor][colorOne]);

                new body[3500] = "{BEBEBE}Name\t{BEBEBE}Preview\n";
                strcat(body, fmat_return("%s{BEBEBE}Use interface preset\t{%s}Preview\n",
                    (ServerDefaultHudShadeOne == -1) ? ("{23C552}> ") : ("{B4B5B7}\x95 "), hex1), sizeof(body));

                for(new i; i < sizeof(hudShades); i++)
                {
                    new bool:isSelected = (ServerDefaultHudShadeOne == i);
                    strcat(body, fmat_return("%s{%06x}%s\t{%06x}Preview\n",
                        isSelected ? ("{23C552}> ") : ("{B4B5B7}\x95 "), hudShades[i][shadeHex], hudShades[i][shadeName], hudShades[i][shadeHex]), sizeof(body));
                }
                format_MainColor(playerid, body);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_DEFAULTSHADE1<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "{BEBEBE}Default HUD Primary", body, "*", "<");
            }
            case 24: // Default HUD Secondary
            {
                if(User[playerid][adminLevel] < 5)
                    return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 5+ required to change the default HUD secondary color.");

                new presetColor = (0 <= ServerDefaultHudColor < sizeof(hudPersonalization)) ? ServerDefaultHudColor : 0;
                new hex2[7];
                hex2 = GetColorHexTag(hudPersonalization[presetColor][colorTwo]);

                new body[3500] = "{BEBEBE}Name\t{BEBEBE}Preview\n";
                strcat(body, fmat_return("%s{BEBEBE}Use interface preset\t{%s}Preview\n",
                    (ServerDefaultHudShadeTwo == -1) ? ("{23C552}> ") : ("{B4B5B7}\x95 "), hex2), sizeof(body));

                for(new i; i < sizeof(hudShades); i++)
                {
                    new bool:isSelected = (ServerDefaultHudShadeTwo == i);
                    strcat(body, fmat_return("%s{%06x}%s\t{%06x}Preview\n",
                        isSelected ? ("{23C552}> ") : ("{B4B5B7}\x95 "), hudShades[i][shadeHex], hudShades[i][shadeName], hudShades[i][shadeHex]), sizeof(body));
                }
                format_MainColor(playerid, body);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_DEFAULTSHADE2<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "{BEBEBE}Default HUD Secondary", body, "*", "<");
            }
            case 26: // Default Chat Color
            {
                if(User[playerid][adminLevel] < 5)
                    return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 5+ required to change default chat color.");

                new body[3500] = "{BEBEBE}Name\t{BEBEBE}Preview\n";
                new bool:isDef = (ServerDefaultChatMainColor <= 0);
                strcat(body, fmat_return("%s{FFFFFF}Use Interface Color (Default)\t{BEBEBE}Follows the HUD color\n",
                    isDef ? ("{23C552}> ") : ("{B4B5B7}\x95 ")), sizeof(body));

                for(new i; i < sizeof(hudShades); i++)
                {
                    new bool:isSelected = (ServerDefaultChatMainColor == (i + 1));
                    strcat(body, fmat_return("%s{%06x}%s\t{%06x}[MAIN] Message\n",
                        isSelected ? ("{23C552}> ") : ("{B4B5B7}\x95 "), hudShades[i][shadeHex], hudShades[i][shadeName], hudShades[i][shadeHex]), sizeof(body));
                }
                format_MainColor(playerid, body);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_DEFAULTCHATCOLOR<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "{BEBEBE}Default Chat Color", body, "*", "<");
            }
        }
    }

    return true;
}
forward DIALOG_ADM_MAXWARN(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_ADM_MAXPING(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_MAXWARN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !isnull(inputtext))
    {
        new value = strval(inputtext);
        if(value < 1 || value > 50)
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid value. Choose between 1 and 50.");
            ShowAdminConfig(playerid);
            return 1;
        }
        ServerMaxWarnings = value;
        SaveServerConfig();
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Max Warnings updated to %d.", ServerMaxWarnings);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_ACCOOLDOWN(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_ACCOOLDOWN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !isnull(inputtext))
    {
        new value = strval(inputtext);
        if(value < 1 || value > 60)
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid value. Choose between 1 and 60.");
            ShowAdminConfig(playerid);
            return 1;
        }
        ServerACCooldown = value;
        SaveServerConfig();
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Anti Cheat cooldown updated to %d seconds.", ServerACCooldown);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_MAXLOGIN(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_MAXLOGIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !isnull(inputtext))
    {
        new value = strval(inputtext);
        if(value < 1 || value > 20)
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid value. Choose between 1 and 20.");
            ShowAdminConfig(playerid);
            return 1;
        }
        ServerMaxLoginAttempts = value;
        SaveServerConfig();
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Max Login Attempts updated to %d.", ServerMaxLoginAttempts);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_MAXACC(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_MAXACC(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !isnull(inputtext))
    {
        new value = strval(inputtext);
        if(value < 0 || value > 50)
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid value. Choose between 0 and 50.");
            ShowAdminConfig(playerid);
            return 1;
        }
        ServerMaxAccountsPerIP = value;
        SaveServerConfig();
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Max Accounts per IP/GPCI updated to %d.", ServerMaxAccountsPerIP);
    }
    ShowAdminConfig(playerid);
    return 1;
}

public DIALOG_ADM_MAXPING(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !isnull(inputtext))
    {
        new ping_val = strval(inputtext);
        if(ping_val < 50 || ping_val > 2000)
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid Max Ping value. Choose between 50 and 2000.");
            ShowAdminConfig(playerid);
            return 1;
        }
        ServerMaxPing = ping_val;
        SaveServerConfig();
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Max Ping updated to %d ms.", ServerMaxPing);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_MAXPL(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_MAXPL(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !isnull(inputtext))
    {
        new Float:pl_val = floatstr(inputtext);
        if(pl_val <= 0.0 || pl_val > 100.0)
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid Max Packetloss value. Choose between 0.01 and 100.00.");
            ShowAdminConfig(playerid);
            return 1;
        }
        ServerMaxPacketLoss = pl_val;
        SaveServerConfig();
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Max Packetloss updated to %.2f%%.", ServerMaxPacketLoss);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_MINFPS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_MINFPS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !isnull(inputtext))
    {
        new min_fps_val = strval(inputtext);
        if(min_fps_val < 1 || min_fps_val > ServerMaxFPS)
        {
            va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid Min FPS value. Choose between 1 and %d (current Max FPS).", ServerMaxFPS);
            ShowAdminConfig(playerid);
            return 1;
        }
        ServerMinFPS = min_fps_val;
        SaveServerConfig();
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Min FPS updated to %d.", ServerMinFPS);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_MAXFPS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_MAXFPS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !isnull(inputtext))
    {
        new max_fps_val = strval(inputtext);
        if(max_fps_val < 30 || max_fps_val > 500)
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid Max FPS value. Choose between 30 and 500.");
            ShowAdminConfig(playerid);
            return 1;
        }
        ServerMaxFPS = max_fps_val;
        SaveServerConfig();
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Max FPS updated to %d.", ServerMaxFPS);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_LOCKPASS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_LOCKPASS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !isnull(inputtext))
    {
        new str[64];
        format(str, sizeof(str), "password %s", inputtext);
        SendRconCommand(str);
        ServerLocked = true;
        va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s locked the server.", User[playerid][userName]);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_HOSTNAME(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_HOSTNAME(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !isnull(inputtext))
    {
        if(User[playerid][adminLevel] < 4 || !IsPlayerAdmin(playerid))
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ and RCON login required.");

        UpdateServerName(inputtext);

        va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s updated the server name to: %s", User[playerid][userName], inputtext);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_MAXPLAYERS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_MAXPLAYERS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !isnull(inputtext))
    {
        if(User[playerid][adminLevel] < 4 || !IsPlayerAdmin(playerid))
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ and RCON login required.");

        new slots = strval(inputtext);
        if(slots < 1 || slots > MAX_PLAYERS)
        {
            va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid slots amount. Please enter a value between 1 and %d.", MAX_PLAYERS);
            ShowAdminConfig(playerid);
            return 1;
        }

        new str[64];
        format(str, sizeof(str), "maxplayers %d", slots);
        SendRconCommand(str);

        va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s updated server max players to %d.", User[playerid][userName], slots);
    }
    ShowAdminConfig(playerid);
    return 1;
}


forward DIALOG_ADM_MUTE_REASON(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_MUTE_REASON(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        new targetid = GetPVarInt(playerid, "profile_target");
        if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
        if(isnull(inputtext)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Reason cannot be empty.");

        new cmd[150];
        format(cmd, sizeof(cmd), "/mute %d 60 %s", targetid, inputtext); // Default 60 minutes
        Command_ReProcess(playerid, cmd, 0);
    }
    return 1;
}

forward DIALOG_ADM_KICK_REASON(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_KICK_REASON(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        new targetid = GetPVarInt(playerid, "profile_target");
        if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
        if(isnull(inputtext)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Reason cannot be empty.");

        new cmd[150];
        format(cmd, sizeof(cmd), "/kick %d %s", targetid, inputtext);
        Command_ReProcess(playerid, cmd, 0);
    }
    return 1;
}

forward DIALOG_ADM_BAN_REASON(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_BAN_REASON(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        new targetid = GetPVarInt(playerid, "profile_target");
        if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
        if(isnull(inputtext)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Reason cannot be empty.");

        new cmd[150];
        format(cmd, sizeof(cmd), "/ban %d 9999 %s", targetid, inputtext); // Default 9999 hours
        Command_ReProcess(playerid, cmd, 0);
    }
    return 1;
}

forward DIALOG_ADM_DEFAULTHUD(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_DEFAULTHUD(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && 0 <= listitem < sizeof(HUD_TYPES))
    {
        if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 5+ required.");

        ServerDefaultHud = listitem;
        SaveServerConfig();

        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Server default HUD style updated to: %s (applies to new accounts).", HUD_TYPES[ServerDefaultHud]);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_DEFAULTHUDCOLOR(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_DEFAULTHUDCOLOR(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && 0 <= listitem < sizeof(hudPersonalization))
    {
        if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 5+ required.");

        ServerDefaultHudColor = listitem;
        SaveServerConfig();

        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Server default HUD color updated to: %s (applies to new accounts).", hudPersonalization[ServerDefaultHudColor][lineInfo]);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_DEFAULTSHADE1(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_DEFAULTSHADE1(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 5+ required.");

        new value = (listitem == 0) ? -1 : (listitem - 1);
        if(value < -1 || value >= sizeof(hudShades))
        {
            ShowAdminConfig(playerid);
            return 1;
        }

        ServerDefaultHudShadeOne = value;
        SaveServerConfig();

        new shadeName_[16];
        if(value == -1) format(shadeName_, sizeof(shadeName_), "Interface preset");
        else format(shadeName_, sizeof(shadeName_), "%s", hudShades[value][shadeName]);

        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Server default HUD primary color updated to: %s (applies to new accounts).", shadeName_);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_DEFAULTSHADE2(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_DEFAULTSHADE2(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 5+ required.");

        new value = (listitem == 0) ? -1 : (listitem - 1);
        if(value < -1 || value >= sizeof(hudShades))
        {
            ShowAdminConfig(playerid);
            return 1;
        }

        ServerDefaultHudShadeTwo = value;
        SaveServerConfig();

        new shadeName_[16];
        if(value == -1) format(shadeName_, sizeof(shadeName_), "Interface preset");
        else format(shadeName_, sizeof(shadeName_), "%s", hudShades[value][shadeName]);

        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Server default HUD secondary color updated to: %s (applies to new accounts).", shadeName_);
    }
    ShowAdminConfig(playerid);
    return 1;
}

forward DIALOG_ADM_DEFAULTCHATCOLOR(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ADM_DEFAULTCHATCOLOR(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 5+ required.");

        new value = listitem;
        if(value < 0 || value > sizeof(hudShades))
        {
            ShowAdminConfig(playerid);
            return 1;
        }

        ServerDefaultChatMainColor = value;
        SaveServerConfig();

        new cName[32];
        if(value == 0) format(cName, sizeof(cName), "Interface preset");
        else format(cName, sizeof(cName), "%s", hudShades[value - 1][shadeName]);

        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Server default chat color ({MAIN}) updated to: %s (applies to new accounts).", cName);
    }
    ShowAdminConfig(playerid);
    return 1;
}

