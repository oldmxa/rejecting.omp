#define ANTIVPN_API_KEY "1694f401ab4943d98ffd129e419b0d0c"

new Request:LoadPlayerIDToRequest[MAX_PLAYERS];

stock CheckISP(playerid)
{
    new playerIP[16];
    GetPlayerIp(playerid, playerIP, sizeof(playerIP));

    if(strequal(playerIP, "127.0.0.1"))
        return true;

    // um unico client por sessao: recriar a cada conexao vaza handles do plugin
    if(client == RequestsClient:0)
        client = RequestsClient("https://vpnapi.io/api/");

    inline const OnWhitelistCheck()
    {
        new currentIP[16];

        if(!cache_num_rows() && IsPlayerConnected(playerid)
            && GetPlayerIp(playerid, currentIP, sizeof(currentIP))
            && strequal(currentIP, playerIP))
        {
            new format_antivpn[250];
            format(format_antivpn, sizeof(format_antivpn), "%s?key=" ANTIVPN_API_KEY, playerIP);

            LoadPlayerIDToRequest[playerid] = RequestJSON(
                client,
                format_antivpn,
                HTTP_METHOD_GET,
                "OnGetAntiVPN"
            );
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnWhitelistCheck,
        "SELECT `ip` FROM `ip_whitelist` WHERE `ip` = '%e' LIMIT 1", playerIP);

    return true;
}

forward OnGetAntiVPN(Request:id, E_HTTP_STATUS:status, Node:node);
public OnGetAntiVPN(Request:id, E_HTTP_STATUS:status, Node:node)
{
    switch(status)
    {
         case HTTP_STATUS_OK:
         {
            new Node:location, Node:security, Node:network, playerid = INVALID_PLAYER_ID;

            for (new i; i < sizeof(LoadPlayerIDToRequest); i ++)
            {
                if (LoadPlayerIDToRequest[i] == id)
                {
                    playerid = i;
                    LoadPlayerIDToRequest[i] = Request:0;

                    break;
                }
            }

            if (playerid == INVALID_PLAYER_ID || !IsPlayerConnected(playerid))
                return 1;

            JsonToggleGC(node, false);

            JsonGetObject(node, "security", security);
            JsonToggleGC(security, false);

            JsonGetBool(security, "vpn", User[playerid][VPN]);
            JsonGetBool(security, "proxy", User[playerid][Proxy]);

            JsonGetObject(node, "location", location);
            JsonToggleGC(location, false);

            JsonGetString(location, "city", User[playerid][City]);
            JsonGetString(location, "region", User[playerid][Region]);
            JsonGetString(location, "region_code", User[playerid][RegionCode]);
            JsonGetString(location, "country", User[playerid][Country]);
            JsonGetString(location, "country_code", User[playerid][CountryCode]);
            JsonGetString(location, "latitude", User[playerid][Latitude]);
            JsonGetString(location, "longitude", User[playerid][Longitude]);
            JsonGetString(location, "time_zone", User[playerid][TimeZone]);

            JsonGetObject(node, "network", network);
            JsonToggleGC(network, false);

            JsonGetString(network, "network", User[playerid][Network]);
            JsonGetString(network, "autonomous_system_number", User[playerid][IspNumber]);
            JsonGetString(network, "autonomous_system_organization", User[playerid][Isp]);

            JsonToggleGC(security, true);
            JsonToggleGC(location, true);
            JsonToggleGC(network, true);
            JsonToggleGC(node, true); // ativa todos


            inline const CountryBanned()
            {
                new rows = cache_num_rows();
                if(rows)
                {
                    new msgCountry[250];
                    format(msgCountry, sizeof (msgCountry), ""COL_ABACATE"Your country is banned from this server.");
                    SendClientMessage(playerid, -1, "%s", msgCountry);

                    KickEx(playerid, 2500);
                    return true;
                }
            }

            inline const IspBanned()
            {
                new rows = cache_num_rows();
                if(rows)
                {
                    new msgISP[250];
                    format(msgISP, sizeof (msgISP), ""COL_ABACATE"Your ISP: %s is banned in this server.",  User[playerid][Isp]);
                    SendClientMessage(playerid, -1, "%s", msgISP);

                    KickEx(playerid, 2500);
                    return true;
                }
            }
            MySQL_TQueryInline(dbHandle, using inline IspBanned, "SELECT `isp` FROM `ispsbanidas` WHERE `isp`='%e'", User[playerid][Isp]);
            MySQL_TQueryInline(dbHandle, using inline CountryBanned, "SELECT `country_name` FROM `paisesbanidos` WHERE `country_name`='%e'", User[playerid][Country]);

            if(User[playerid][Proxy] || User[playerid][VPN])
            {
                mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `warnings` FROM `accounts` WHERE `username` = '%e' LIMIT 1", User[playerid][userName]);
                mysql_tquery(dbHandle, queryGlobal, "OnAntiVpnWarn", "i", playerid);
            }
        }
        default:
        {
            new log[128];
            format(log, sizeof(log), "[ANTIVPN] lookup rejected by the API, HTTP status %d", _:status);

            ConsoleLog(log);
        }
    }
    return 1;
}

forward OnAntiVpnWarn(playerid);
public OnAntiVpnWarn(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 1;

    if(!cache_num_rows())
        return SendClientMessage(playerid, -1, ""COL_ABACATE"You are using a Proxy/VPN, disable it to avoid punishment.");

    new warns;
    cache_get_value_int(0, "warnings", warns);
    warns++;

    new bool:reachedLimit = (warns >= 5);

    va_SendClientMessage(playerid, -1, ""COL_ABACATE"You are using a Proxy/VPN, disable it. Warning [%d/5], on the fifth you will be banned.", warns);

    foreach(new i : Player)
    {
        if(User[i][adminLevel] > 0 && User[i][isLogged] && i != playerid)
            va_SendClientMessage(i, -1, ADM_TAG_PUN"Anti-VPN: %s[%d] connected using Proxy/VPN (ISP: %s). Warning [%d/5].", User[playerid][userName], playerid, User[playerid][Isp], warns);
    }

    new log_details[256];
    format(log_details, sizeof(log_details), "Proxy/VPN detected on %s (ISP: %s). Warning [%d/5]", User[playerid][userName], User[playerid][Isp], warns);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "warns", "Anti-VPN", log_details);

    User[playerid][warnings] = reachedLimit ? 0 : warns;

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `accounts` SET `warnings` = %d WHERE `username` = '%e'", User[playerid][warnings], User[playerid][userName]);
    mysql_tquery(dbHandle, queryGlobal);

    if(reachedLimit)
    {
        new hours = 9999, bantimestamp = gettime(), unbants = bantimestamp + hours * 3600, pip[18];
        GetPlayerIp(playerid, pip, sizeof(pip));

        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
            "INSERT INTO `banimentos` (`username`, `ip`, `admin`, `reason`, `country`, `isp`, `ban_timestamp`, `unban_timestamp`, `gpci`) VALUES('%e', '%e', '%e', '%e', '%e', '%e', %d, %d, '%e')",
            User[playerid][userName], pip, "Anti-VPN", "Exceeded warning limit. (5/5) Proxy/VPN", User[playerid][Country], User[playerid][Isp], bantimestamp, unbants, User[playerid][user_gpci]);
        mysql_tquery(dbHandle, queryGlobal);

        va_SendClientMessageToAll(-1, COL_ADM_PUNISH"[Admin] Anti-VPN "COL_ADM_PUNISH"banned player %s (Reason: Exceeded warning limit. (5/5) Proxy/VPN)", User[playerid][userName]);

        GameTextForPlayer(playerid, "~r~BANNED~w~!", 2500, 3);
        SendBanNotice(playerid, ban_message_color"[BANNED] Exceeded the Proxy/VPN warning limit (5/5).");

        new string[1024];
        format(string, sizeof(string), "**Admin:** Anti-VPN\n**Target:** %s\n**Reason:** Exceeded warning limit (5/5) - Proxy/VPN\n**ISP:** %s\n**Unban Date:** %s",
            User[playerid][userName], User[playerid][Isp], returnDate(unbants-10800));

        SendDiscordEmbed(logs_ban, "New Ban (Anti-VPN)", string, DISCORD_COLOR_ERROR);

        KickEx(playerid, 2500);
    }

    return 1;
}



