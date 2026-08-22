#if defined _ac_detections_included
    #endinput
#endif
#define _ac_detections_included

stock AC_GetDetectionName(code, destination[], len = sizeof(destination))
{
    if (0 <= code < MAX_AC_DETECTIONS)
        format(destination, len, "%s", AC_DETECTION[code][acdName]);
    else
        format(destination, len, "Unknown (%d)", code);

    return 1;
}

static ACReportTick[MAX_PLAYERS][MAX_AC_DETECTIONS];
static ACDiscordCooldown[MAX_PLAYERS][MAX_AC_DETECTIONS];

#include <YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid)
{
    for (new c; c < MAX_AC_DETECTIONS; c++)
    {
        ACReportTick[playerid][c] = 0;
        ACDiscordCooldown[playerid][c] = 0;
    }

    ApplyAnimation(playerid, "PAULNMAC", "null", 0.0, 0, 0, 0, 0, 0);
    return 1;
}

static AC_GetAction(code)
{
    if (code < 0 || code >= MAX_AC_DETECTIONS) return AC_ACTION_REPORT;

    new action = AC_DETECTION_ACTION[code];

    if (!ACPunishEnabled && action > AC_ACTION_REPORT) return AC_ACTION_REPORT;

    return action;
}

static AC_Alert(playerid, code, const detection[], const action[])
{
    new WEAPON:weaponid = GetPlayerWeapon(playerid), weapon[24], context[40];

    if (weaponid != WEAPON_FIST)
    {
        GetWeaponName(weaponid, weapon, sizeof(weapon));
        format(context, sizeof(context), "{B4B5B7}with {3CCFFA}%s ", weapon);
    }
    else format(weapon, sizeof(weapon), "none");

    foreach (new i : Player)
    {
        if (User[i][adminLevel] > 0)
            va_SendClientMessage(i, -1, AC_TAG_PUN": %s[%d] {B4B5B7}suspect of using {3CCFFA}%s %s{B4B5B7}code: {3CCFFA}#%02d{B4B5B7}.",
                User[playerid][userName], playerid, detection, context, code + 1);
    }

    new now = gettime();
    if (now - ACDiscordCooldown[playerid][code] >= 5)
    {
        ACDiscordCooldown[playerid][code] = now;

        new pip[18], details[512];
        GetPlayerIp(playerid, pip, sizeof(pip));

        format(details, sizeof(details),
            "**Player:** %s [%d]\n**Detection:** %s (#%02d)\n**Weapon:** %s\n**Action:** %s\n**FPS:** %d  |  **Ping:** %d ms  |  **PL:** %.2f%%\n**IP:** %s\n**GPCI:** %s",
            User[playerid][userName], playerid, detection, code + 1, weapon, action,
            User[playerid][FPS], GetPlayerPing(playerid), NetStats_PacketLossPercent(playerid),
            pip, User[playerid][user_gpci]);

        SendDiscordEmbed(logs_ac, "Rejecting AC", details, DISCORD_COLOR_ERROR);
        LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "warns", "Rejecting AC", detection);
    }
    return 1;
}

static AC_BanPlayer(playerid, code, const detection[])
{
    new reason[64], pip[18];
    format(reason, sizeof(reason), "Rejecting AC: %s", detection);
    GetPlayerIp(playerid, pip, sizeof(pip));

    new bantimestamp = gettime(), unbants = bantimestamp + AC_BAN_HOURS * 3600;

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "INSERT INTO `banimentos` (`username`, `ip`, `admin`, `reason`, `country`, `isp`, `ban_timestamp`, `unban_timestamp`, `gpci`) VALUES('%e', '%e', '%e', '%e', '%e', '%e', %d, %d, '%e')",
        User[playerid][userName], pip, "Rejecting AC", reason, User[playerid][Country], User[playerid][Isp], bantimestamp, unbants, User[playerid][user_gpci]);
    mysql_tquery(dbHandle, queryGlobal);

    va_SendClientMessageToAll(-1, COL_ADM_PUNISH"[Admin] Rejecting AC "COL_ADM_PUNISH"banned player %s (Reason: %s)", User[playerid][userName], detection);

    GameTextForPlayer(playerid, "~r~BANNED~w~!", 2500, 3);
    SendBanNotice(playerid, ban_message_color"[BANNED] Rejecting AC detected a prohibited modification.");

    AC_ShowKickScreen(playerid, code);
    KickEx(playerid, AC_SCREEN_TIME);
    return 1;
}

static AC_GtempPlayer(playerid, const detection[])
{
    if (User[playerid][OnTempREC]) return 1;

    new gTempIP[18], gTempUser[MAX_PLAYER_NAME + 1], gTempCountry[50 + 1], gTempISP[100 + 1], gTempReason[64];

    GetPlayerIp(playerid, gTempIP, sizeof(gTempIP));
    format(gTempUser, sizeof(gTempUser), "%s", User[playerid][userName]);
    format(gTempCountry, sizeof(gTempCountry), "%s", User[playerid][Country]);
    format(gTempISP, sizeof(gTempISP), "%s", User[playerid][Isp]);
    format(gTempReason, sizeof(gTempReason), "review - AC: %s", detection);

    va_SendClientMessage(playerid, COLOR_LIGHT_BLUE,
        ADM_TAG_PUN"%s{FFFFFF}, you have 5 minutes to start recording your screen, after that time passes you will be banned from the server.", gTempUser);
    GameTextForPlayer(playerid, " ~n~~b~~h~start recording", 12000, 0);

    inline const gTempServer()
    {
        inline const OnACTempRecChecked()
        {
            if (cache_num_rows()) return 1;

            new bantimestamp = gettime(), unbants = bantimestamp + AC_BAN_HOURS * 3600;

            mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,
                "INSERT INTO `banimentos` (`username`, `ip`, `admin`, `reason`, `country`, `isp`, `ban_timestamp`, `unban_timestamp`) VALUES('%e', '%e', '%e', '%e', '%e', '%e', %d, %d)",
                gTempUser, gTempIP, "Rejecting AC", gTempReason, gTempCountry, gTempISP, bantimestamp, unbants);
            mysql_tquery(dbHandle, queryGlobal);

            va_SendClientMessageToAll(-1, BAN_TEMPREC_REASON, "Rejecting AC", gTempUser, AC_BAN_HOURS, gTempReason);

            if (IsPlayerConnected(playerid) && strequal(User[playerid][userName], gTempUser))
            {
                SendClientMessage(playerid, -1, "%s", BAN_TEMPREC_MINUTES);
                SendClientMessage(playerid, -1, BAN_TEMPREC_WHOBANNED, "Rejecting AC", gTempReason, gTempIP);
                User[playerid][OnTempREC] = false;
                KickEx(playerid, 2000);
            }

            new string[512];
            format(string, sizeof(string), "**Admin:** Rejecting AC\n**Target:** %s\n**Reason:** %s", gTempUser, gTempReason);
            SendDiscordEmbed(logs_ac, "New Ban - AC GTEMP", string, DISCORD_COLOR_ERROR);
        }
        MySQL_TQueryInline(dbHandle, using inline OnACTempRecChecked,
            "SELECT `id` FROM `banimentos` WHERE `username` = '%e' LIMIT 1", gTempUser);
    }

    Timer_CreateCallback(using inline gTempServer, 300000, 1);
    User[playerid][OnTempREC] = true;
    return 1;
}

static AC_ShowKickScreen(playerid, code)
{
    new Float:x, Float:y, Float:z, Float:angle;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, angle);

    ApplyAnimation(playerid, "PAULNMAC", "wank_loop", 4.1, 1, 0, 0, 1, 0, SYNC_ALL);

    SetPlayerCameraPos(playerid,
        x + AC_SCREEN_CAM_DIST * floatsin(-angle, degrees),
        y + AC_SCREEN_CAM_DIST * floatcos(-angle, degrees),
        z + AC_SCREEN_CAM_HEIGHT);
    SetPlayerCameraLookAt(playerid, x, y, z);

    new year, month, day, hour, minute, second;
    getdate(year, month, day);
    gettime(hour, minute, second);

    new body[512];
    format(body, sizeof(body),
        "{FFFFFF}%s, you have reason for suspected of cheating.\n\n\
        "COL_GRAY"Date & Time: {FFFFFF}%02d.%02d.%d - %02d:%02d:%02d\n\
        "COL_GRAY"FPS: {FFFFFF}%d   "COL_GRAY"PING: {FFFFFF}%d   "COL_GRAY"PL: {FFFFFF}%.2f%%\n\
        "COL_GRAY"Code: {FF0000}#%02d\n\n\
        "COL_GRAY"If you think this is an error, please contact us:\n{MAIN}%s",
        User[playerid][userName],
        day, month, year, hour, minute, second,
        User[playerid][FPS], GetPlayerPing(playerid), NetStats_PacketLossPercent(playerid),
        code + 1,
        SERVER_WEBSITE);

    new title[32] = "{MAIN}Anti Cheat";
    format_MainColor(playerid, title);
    format_MainColor(playerid, body, sizeof(body));

    ShowPlayerDialog(playerid, AC_DIALOG_KICKED, DIALOG_STYLE_MSGBOX, title, body, "Close", "");
    return 1;
}

static AC_KickPlayer(playerid, code, const detection[])
{
    va_SendClientMessage(playerid, -1, "{MAIN}[RAC] {FFFFFF}%s is prohibited on this server. Remove it to keep playing.", detection);

    AC_ShowKickScreen(playerid, code);
    KickEx(playerid, AC_SCREEN_TIME);
    return 1;
}

public OnRejectingACDetection(playerid, code)
{
    if (IsPlayerNPC(playerid)) return 1;
    if (!IsPlayerConnected(playerid) || !User[playerid][isLogged]) return 1;

    if (code < 0 || code >= MAX_AC_DETECTIONS) return 1;
    if (!AC_DETECTION[code][acdActive]) return 1;
    if (User[playerid][isSpectating] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
    {
        switch (code)
        {
            case AC_AIRBREAK, AC_CLICK_WARP, AC_NOP_CONTROLLABLE, AC_NOP_POS,
                 AC_INVISIBLE_UG, AC_INVISIBLE_SURF, AC_MAP_RUN, AC_NULL_VELOCITY, AC_NO_FALL,
                 AC_CAR_AIRBREAK, AC_CAR_SPEED, AC_MEGA_JUMP, AC_CAR_SLAPPER,
                 AC_NOP_ANIMATION, AC_NOP_ENGINE_STATE:
            {
                return 1;
            }
        }
    }
    if ((code == AC_BULLET_PROOF || code == AC_ARMOUR_PROOF) && GetPVarInt(playerid, "spawn_protect") == 1)
    {
        return 1;
    }

    new tick = GetTickCount();

    if (ACReportTick[playerid][code] && tick - ACReportTick[playerid][code] < ServerACCooldown * 1000) return 1;
    ACReportTick[playerid][code] = tick;

    new detection[32];
    AC_GetDetectionName(code, detection, sizeof(detection));

    ACSuspect[playerid][ac_cheatFlags]++;
    ACSuspect[playerid][ac_lastCheatTime] = gettime();
    format(ACSuspect[playerid][ac_lastCheat], AC_REASON_LEN, "%s", detection);

    new action = AC_GetAction(code);

    AC_LogDetection(playerid, code, action);

    switch (action)
    {
        case AC_ACTION_IGNORE: return 1;
        case AC_ACTION_REPORT: AC_Alert(playerid, code, detection, "reported");
        case AC_ACTION_KICK:
        {
            AC_Alert(playerid, code, detection, "kicked");
            AC_KickPlayer(playerid, code, detection);
        }
        case AC_ACTION_GTEMP:
        {
            AC_Alert(playerid, code, detection, "gtemp");
            AC_GtempPlayer(playerid, detection);
        }
        case AC_ACTION_BAN:
        {
            AC_Alert(playerid, code, detection, "banned");
            AC_BanPlayer(playerid, code, detection);
        }
    }
    return 1;
}

@cmd() actest(playerid, params[], help)
{
    if (User[playerid][adminLevel] < AC_MIN_ADMIN_LEVEL)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    new code;
    if (sscanf(params, "D(0)", code)) code = 0;
    if (code < 0 || code >= MAX_AC_DETECTIONS) code = 0;

    new detection[32];
    AC_GetDetectionName(code, detection, sizeof(detection));

    SetPVarInt(playerid, "ac_test", 1);
    AC_ShowKickScreen(playerid, code);

    va_SendClientMessage(playerid, -1, "{MAIN}[RAC] {FFFFFF}Preview of {MAIN}%s{FFFFFF}. Nobody was kicked. Use {MAIN}/actest [0-%d]{FFFFFF} for another one.",
        detection, MAX_AC_DETECTIONS - 1);
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (dialogid != AC_DIALOG_KICKED) return 0;

    if (GetPVarInt(playerid, "ac_test"))
    {
        DeletePVar(playerid, "ac_test");

        ClearAnimations(playerid);
        SetCameraBehindPlayer(playerid);
    }
    return 1;
}

