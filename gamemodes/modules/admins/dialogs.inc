
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
                if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ required to restart.");
                
                SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s is restarting the server.", User[playerid][userName]);
                foreach(new i : Player) { if(User[i][isLogged]) UpdatePlayerData(i); }
                SendRconCommand("gmx");
            }
            case 2: // Ping
            {
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MAXPING<iiiis>, DIALOG_STYLE_INPUT, "{418cb0}Max Ping", "{FFFFFF}Enter the new maximum ping allowed:", "Update", "Back");
            }
            case 3: // PL
            {
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MAXPL<iiiis>, DIALOG_STYLE_INPUT, "{418cb0}Max Packetloss", "{FFFFFF}Enter the new maximum packet loss allowed (float):", "Update", "Back");
            }
            case 4: // Min FPS
            {
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MINFPS<iiiis>, DIALOG_STYLE_INPUT, "{418cb0}Min FPS", "{FFFFFF}Enter the new minimum FPS allowed:", "Update", "Back");
            }
            case 12: // Max FPS
            {
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MAXFPS<iiiis>, DIALOG_STYLE_INPUT, "{418cb0}Max FPS", "{FFFFFF}Enter the new maximum FPS allowed:", "Update", "Back");
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
                    Dialog_ShowCallback(playerid, using public DIALOG_ADM_LOCKPASS<iiiis>, DIALOG_STYLE_INPUT, "{418cb0}Server Lock", "{FFFFFF}Enter the password to lock the server:", "Lock", "Back");
                }
            }
            case 13: // R2 Kick (server-wide)
            {
                if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ required to change this.");

                R2KickEnabled = !R2KickEnabled;
                SaveServerConfig();

                va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s %s the R2 client check kick.",
                    User[playerid][userName], (R2KickEnabled) ? ("enabled") : ("disabled"));

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

                Dialog_ShowCallback(playerid, using public DIALOG_ADM_HOSTNAME<iiiis>, DIALOG_STYLE_INPUT, "{418cb0}Server Name", "{FFFFFF}Enter the new server name to display in the launcher:", "Update", "Back");
            }
            case 11:
            {
                if(User[playerid][adminLevel] < 4 || !IsPlayerAdmin(playerid))
                    return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ and RCON login required.");

                new infoStr[128];
                format(infoStr, sizeof(infoStr), "{FFFFFF}Current Max Players: {418cb0}%d\n\n{FFFFFF}Enter the new maximum player limit (1-1000):", GetMaxPlayers());
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MAXPLAYERS<iiiis>, DIALOG_STYLE_INPUT, "{418cb0}Max Players", infoStr, "Update", "Back");
            }
            case 14: // Default Server HUD
            {
                if(User[playerid][adminLevel] < 5)
                    return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 5+ required to change default HUD style.");

                new string[512];
                strcopy(string, "ID\tHUD Style\tStatus\n");
                for (new i = 0; i < sizeof(HUD_TYPES); i++)
                {
                    strcat(string, fmat_return("%d\t%s\t%s\n", i, HUD_TYPES[i], (ServerDefaultHud == i) ? ("{00FF00}Selected") : ("{FFFFFF}Select")));
                }
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_DEFAULTHUD<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "{418cb0}Default Server HUD", string, "Select", "Back");
            }
            case 15: // Default HUD Color
            {
                if(User[playerid][adminLevel] < 5)
                    return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 5+ required to change default HUD color.");

                new body[2048] = "Name\tColor 1\tColor 2\tStatus\n";
                for(new i; i < sizeof(hudPersonalization); i++)
                {
                    new name[24], hex1[7], hex2[7];
                    format(name, sizeof(name), "%s", hudPersonalization[i][lineInfo]);
                    if(name[0] >= 'a' && name[0] <= 'z') name[0] -= 32;
                    new space = strfind(name, " ");
                    if(space != -1 && name[space+1] >= 'a' && name[space+1] <= 'z') name[space+1] -= 32;

                    hex1 = GetColorHexTag(hudPersonalization[i][colorOne]);
                    hex2 = GetColorHexTag(hudPersonalization[i][colorTwo]);

                    strcat(body, fmat_return("{%s}%s\t{%s}Preview\t{%s}Preview\t%s\n",
                        CoresDoBglh[i], name, hex1, hex2,
                        (ServerDefaultHudColor == i) ? ("{00FF00}Selected") : ("{FFFFFF}Select")), sizeof(body));
                }
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_DEFAULTHUDCOLOR<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "{418cb0}Default HUD Color", body, "Select", "Back");
            }
        }
    }

    return true;
}

forward DIALOG_ADM_MAXPING(playerid, dialogid, response, listitem, string:inputtext[]);
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
        if(slots < 1 || slots > 1000)
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid slots amount. Please enter a value between 1 and 1000.");
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

