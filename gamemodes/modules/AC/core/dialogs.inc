#if defined _ac_dialogs_included
    #endinput
#endif
#define _ac_dialogs_included

#define AC_ROW_SECTION   (-1)
#define AC_ROW_PUNISH    (1)
#define AC_ROW_R2        (2)
#define AC_ROW_LIMITS    (3)
#define AC_ROW_FLAGGED   (4)
#define AC_ROW_VERBOSE   (5)
#define AC_ROW_RECENT    (6)
#define AC_ROW_FALLDMG   (7)
#define AC_ROW_FALLANIM  (8)
#define AC_ROW_CJWALK    (9)
#define AC_ROW_STUNS     (10)
#define AC_ROW_OMP       (11)
#define AC_ROW_CATEGORY  (100)

#define AC_RECENT_ROWS   (30)

#define AC_MAX_ROWS      (64)

static AC_StoreRows(playerid, const rows[], count)
{
    new pvar[24];

    for (new i; i < count; i++)
    {
        format(pvar, sizeof(pvar), "ac_row_%d", i);
        SetPVarInt(playerid, pvar, rows[i]);
    }

    SetPVarInt(playerid, "ac_rows", count);
    return 1;
}

static AC_TakeRow(playerid, listitem)
{
    new pvar[24], count = GetPVarInt(playerid, "ac_rows"), value = AC_ROW_SECTION;

    if (0 <= listitem < count)
    {
        format(pvar, sizeof(pvar), "ac_row_%d", listitem);
        value = GetPVarInt(playerid, pvar);
    }

    for (new i; i < count; i++)
    {
        format(pvar, sizeof(pvar), "ac_row_%d", i);
        DeletePVar(playerid, pvar);
    }

    DeletePVar(playerid, "ac_rows");
    return value;
}

static AC_CountByCategory(catid, &armed)
{
    new total = 0;
    armed = 0;

    for (new i; i < MAX_AC_DETECTIONS; i++)
    {
        if (AC_DETECTION[i][acdCategory] != catid) continue;
        if (!AC_DETECTION[i][acdName][0]) continue;

        total++;
        if (AC_DETECTION[i][acdActive]) armed++;
    }
    return total;
}

static AC_ToggleExtra(playerid, &bool:setting, const key[], const label[])
{
    setting = !setting;
    AC_SaveSetting(key, _:setting);

    va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s %s %s.",
        User[playerid][userName], setting ? ("enabled") : ("disabled"), label);

    PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
    return 1;
}

static AC_ToggleCJWalk(playerid)
{
    new bool:enabled = !GetConsoleVarAsBool("game.use_player_ped_anims");

    if (enabled) SendRconCommand("game.use_player_ped_anims 1");
    else SendRconCommand("game.use_player_ped_anims 0");

    new applied = ApplyPedAnimsToAll(enabled);

    va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s %s the CJ walk animations.",
        User[playerid][userName], enabled ? ("enabled") : ("disabled"));

    if (applied < 1)
        SendClientMessage(playerid, -1, ADM_TAG_NORM"Nobody could be updated live, it takes effect for players who connect from now on.");

    PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
    ShowACMenu(playerid);
    return 1;
}

static AC_CountFlagged()
{
    new count = 0;

    foreach (new i : Player)
        if (User[i][isLogged] && ACSuspect[i][ac_cheatFlags] > 0) count++;

    return count;
}

stock AC_ToggleVerbose(playerid)
{
    AC_SetDebug(playerid, !ACDebugOn[playerid]);

    if (!ACDebugOn[playerid])
    {
        SendClientMessage(playerid, -1, "{MAIN}[RAC] {FFFFFF}Debug off.");
        return 0;
    }

    SendClientMessage(playerid, -1, "{MAIN}[RAC] {FFFFFF}Debug on. Every measurement is shown, even below the threshold.");
    SendClientMessage(playerid, -1, "{B4B5B7}Also written to scriptfiles/ac_debug.log");

    new File:log = fopen(AC_DEBUG_FILE, io_write);
    if (log) fclose(log);

    AC_Debug(playerid, 0, "session started: afk %d, location %d, interior %d, state %d",
        _:User[playerid][isAfk], User[playerid][locationId], GetPlayerInterior(playerid), _:GetPlayerState(playerid));

    return 1;
}

ShowACMenu(playerid)
{
    new body[1600], title[64] = "{MAIN}RAC {B4B5B7}- {FFFFFF}Menu", rows[AC_MAX_ROWS], count = 0;

    format(body, sizeof(body), "{BEBEBE}Category / Setting\t{BEBEBE}Status / Value\n");

    strcat(body, "{MAIN}\xA4 Detections\t \n", sizeof(body));
    rows[count++] = AC_ROW_SECTION;

    for (new c; c < MAX_AC_CATEGORIES; c++)
    {
        new armed, total = AC_CountByCategory(c, armed);

        if (!total)
        {
            strcat(body, fmat_return("{B4B5B7}\xBB %s\t{B4B5B7}Not available yet\n", AC_CATEGORY_NAME[c]), sizeof(body));
            rows[count++] = AC_ROW_SECTION;
            continue;
        }

        strcat(body, fmat_return("{B4B5B7}\xBB {FFFFFF}%s\t%s%d{B4B5B7}/%d\n",
            AC_CATEGORY_NAME[c],
            armed ? ("{23C552}") : ("{F84F31}"), armed, total), sizeof(body));

        rows[count++] = AC_ROW_CATEGORY + c;
    }

    strcat(body, "{MAIN}\xA4 Enforcement\t \n", sizeof(body));
    rows[count++] = AC_ROW_SECTION;

    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}Punishment\t%s\n",
        ACPunishEnabled ? ("{23C552}Enabled") : ("{F84F31}Report only")), sizeof(body));
    rows[count++] = AC_ROW_PUNISH;

    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}Fake Version Kick\t%s\n",
        R2KickEnabled ? ("{23C552}Enabled") : ("{F84F31}Disabled")), sizeof(body));
    rows[count++] = AC_ROW_R2;

    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}open.mp Only Kick\t%s\n",
        OMPKickEnabled ? ("{23C552}Enabled") : ("{F84F31}Disabled")), sizeof(body));
    rows[count++] = AC_ROW_OMP;

    strcat(body, "{MAIN}\xA4 RAC - Extra\t \n", sizeof(body));
    rows[count++] = AC_ROW_SECTION;

    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}No Fall Damage\t%s\n", ACNoFallDamage ? ("{23C552}Enabled") : ("{F84F31}Disabled")), sizeof(body));
    rows[count++] = AC_ROW_FALLDMG;

    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}No Fall Animation\t%s\n", ACNoFallAnimation ? ("{23C552}Enabled") : ("{F84F31}Disabled")), sizeof(body));
    rows[count++] = AC_ROW_FALLANIM;

    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}UseCJWalk\t%s\n", GetConsoleVarAsBool("game.use_player_ped_anims") ? ("{23C552}Enabled") : ("{F84F31}Disabled")), sizeof(body));
    rows[count++] = AC_ROW_CJWALK;

    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}Fast Stuns\t%s\n", ACFastStuns ? ("{23C552}Enabled") : ("{F84F31}Disabled")), sizeof(body));
    rows[count++] = AC_ROW_STUNS;

    strcat(body, "{MAIN}\xA4 Monitoring\t \n", sizeof(body));
    rows[count++] = AC_ROW_SECTION;

    new flagged = AC_CountFlagged();

    strcat(body, fmat_return("{B4B5B7}\xBB {FFFFFF}Flagged Players\t%s\n",
        flagged ? (fmat_return("{F84F31}%d this session", flagged)) : ("{23C552}Nobody")), sizeof(body));
    rows[count++] = AC_ROW_FLAGGED;

    strcat(body, fmat_return("{B4B5B7}\xBB {FFFFFF}Recent Detections\t{B4B5B7}Last %d on record\n", AC_RECENT_ROWS), sizeof(body));
    rows[count++] = AC_ROW_RECENT;

    strcat(body, fmat_return("{B4B5B7}\xBB {FFFFFF}Connection Limits\t{FFFFFF}%d ms {B4B5B7}/ {FFFFFF}%.2f%% {B4B5B7}/ {FFFFFF}%d-%d FPS\n",
        ServerMaxPing, ServerMaxPacketLoss, ServerMinFPS, ServerMaxFPS), sizeof(body));
    rows[count++] = AC_ROW_LIMITS;

    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}Debug\t%s",
        ACDebugOn[playerid] ? ("{23C552}Enabled") : ("{F84F31}Disabled")), sizeof(body));
    rows[count++] = AC_ROW_VERBOSE;

    AC_StoreRows(playerid, rows, count);

    format_MainColor(playerid, body, sizeof(body));

    format_MainColor(playerid, title);
    format_MainColor(playerid, body, sizeof(body));
    Dialog_ShowCallback(playerid, using public DIALOG_AC_MENU<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "*", "Exit");
    return 1;
}

ShowACCategory(playerid, catid)
{
    if (catid < 0 || catid >= MAX_AC_CATEGORIES) return ShowACMenu(playerid);

    SetPVarInt(playerid, "ac_category", catid);

    new body[1800], title[64], rows[AC_MAX_ROWS], count = 0;
    format(title, sizeof(title), "{MAIN}RAC {B4B5B7}- {FFFFFF}%s", AC_CATEGORY_NAME[catid]);

    format(body, sizeof(body), "{BEBEBE}Category\t{BEBEBE}Code\t{BEBEBE}Status\n");

    for (new i; i < MAX_AC_DETECTIONS; i++)
    {
        if (AC_DETECTION[i][acdCategory] != catid) continue;
        if (!AC_DETECTION[i][acdName][0]) continue;

        new name[32], action = AC_DETECTION_ACTION[i];
        new bool:armed = AC_DETECTION[i][acdActive];
        AC_GetDetectionName(i, name, sizeof(name));

        strcat(body, fmat_return("{B4B5B7}\xBB {FFFFFF}%s\t{7F8C8D}#%02d\t%s%s\n",
            name, i + 1,
            armed ? (AC_ACTION_COLOR[action]) : ("{B4B5B7}"),
            armed ? (AC_ACTION_NAME[action]) : ("Pending")), sizeof(body));

        rows[count++] = i;
    }

    if (!count)
        strcat(body, "{B4B5B7}Nothing here yet\t{B4B5B7}-\t{B4B5B7}-\n", sizeof(body));

    AC_StoreRows(playerid, rows, count);

    format_MainColor(playerid, body, sizeof(body));

    format_MainColor(playerid, title);
    format_MainColor(playerid, body, sizeof(body));
    Dialog_ShowCallback(playerid, using public DIALOG_AC_CATEGORY<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "*", "<");
    return 1;
}

ShowACAction(playerid, detection)
{
    if (detection < 0 || detection >= MAX_AC_DETECTIONS) return ShowACMenu(playerid);

    SetPVarInt(playerid, "ac_detection", detection);

    new name[32], body[900], title[96], rows[AC_MAX_ROWS], count = 0;
    AC_GetDetectionName(detection, name, sizeof(name));

    format(title, sizeof(title), "{MAIN}RAC {B4B5B7}- {FFFFFF}%s {7F8C8D}#%02d", name, detection + 1);

    format(body, sizeof(body), "{BEBEBE}Action\t{BEBEBE}What happens\n");

    strcat(body, fmat_return("{MAIN}\xA4 %s\t{B4B5B7}%d hits on record\n",
        AC_DETECTION[detection][acdHelp], ACHitCount[detection]), sizeof(body));
    rows[count++] = AC_ROW_SECTION;

    for (new a; a < sizeof(AC_ACTION_NAME); a++)
    {
        new bool:isSelected = (AC_DETECTION_ACTION[detection] == a);
        strcat(body, fmat_return("%s%s%s\t{B4B5B7}%s\n",
            isSelected ? ("{MAIN}> ") : ("{B4B5B7}\x95 "),
            AC_ACTION_COLOR[a], AC_ACTION_NAME[a], AC_ACTION_HELP[a]), sizeof(body));

        rows[count++] = a;
    }

    AC_StoreRows(playerid, rows, count);

    format_MainColor(playerid, body, sizeof(body));

    format_MainColor(playerid, title);
    format_MainColor(playerid, body, sizeof(body));
    Dialog_ShowCallback(playerid, using public DIALOG_AC_ACTION<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "*", "<");
    return 1;
}

ShowACFlagged(playerid)
{
    new body[1600], title[64] = "{MAIN}RAC {B4B5B7}- {FFFFFF}Flagged Players";
    new rows[AC_MAX_ROWS], count = 0;

    format(body, sizeof(body), "{BEBEBE}Player\t{BEBEBE}Hits\t{BEBEBE}Last Detection\t{BEBEBE}Client\n");

    foreach (new i : Player)
    {
        if (!User[i][isLogged] || ACSuspect[i][ac_cheatFlags] < 1) continue;
        if (count >= AC_MAX_ROWS) break;

        new elapsed = gettime() - ACSuspect[i][ac_lastCheatTime], age[16];
        if (elapsed < 60) format(age, sizeof(age), "%ds", elapsed);
        else format(age, sizeof(age), "%dm", elapsed / 60);

        strcat(body, fmat_return("{B4B5B7}\xBB {FFFFFF}%s [%d]\t{F84F31}%d\t{FFFFFF}%s {B4B5B7}(%s)\t{B4B5B7}%s\n",
            User[i][userName], i, ACSuspect[i][ac_cheatFlags], ACSuspect[i][ac_lastCheat], age,
            IsPlayerAndroid(i) ? ("mobile") : ("desktop")), sizeof(body));

        rows[count++] = i;
    }

    if (!count)
        strcat(body, "{B4B5B7}Nobody flagged\t{B4B5B7}-\t{B4B5B7}-\t{B4B5B7}-\n", sizeof(body));

    AC_StoreRows(playerid, rows, count);

    format_MainColor(playerid, body, sizeof(body));

    format_MainColor(playerid, title);
    format_MainColor(playerid, body, sizeof(body));
    Dialog_ShowCallback(playerid, using public DIALOG_AC_FLAGGED<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "*", "<");
    return 1;
}

ShowACRecent(playerid)
{
    inline const OnACRecent()
    {
        new rows = cache_num_rows(), body[2200], title[64] = "{MAIN}RAC {B4B5B7}- {FFFFFF}Recent Detections";

        format(body, sizeof(body), "{BEBEBE}Player\t{BEBEBE}Detection\t{BEBEBE}Action\t{BEBEBE}When\n");

        if (!rows)
            strcat(body, "{B4B5B7}Nothing on record\t{B4B5B7}-\t{B4B5B7}-\t{B4B5B7}-\n", sizeof(body));

        for (new r; r < rows && r < AC_RECENT_ROWS; r++)
        {
            new code, action, stamp, name[MAX_PLAYER_NAME + 1], detection[32];

            cache_get_value_name(r, "username", name, sizeof(name));
            cache_get_value_name_int(r, "code", code);
            cache_get_value_name_int(r, "action", action);
            cache_get_value_name_int(r, "created_at", stamp);

            AC_GetDetectionName(code, detection, sizeof(detection));

            if (action < 0 || action >= sizeof(AC_ACTION_NAME)) action = AC_ACTION_IGNORE;

            strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}%s\t{B4B5B7}#%02d %s\t%s%s\t{B4B5B7}%s\n",
                name, code + 1, detection,
                AC_ACTION_COLOR[action], AC_ACTION_NAME[action], returnDate(stamp)), sizeof(body));
        }

        format_MainColor(playerid, body, sizeof(body));

        format_MainColor(playerid, title);
        format_MainColor(playerid, body, sizeof(body));
        Dialog_ShowCallback(playerid, using public DIALOG_AC_RECENT<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Close", "<");
    }

    MySQL_TQueryInline(dbHandle, using inline OnACRecent,
        "SELECT `username`, `code`, `action`, `created_at` FROM `ac_detections` ORDER BY `id` DESC LIMIT %d", AC_RECENT_ROWS);
    return 1;
}

forward DIALOG_AC_RECENT(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_AC_RECENT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowACMenu(playerid);
    return 1;
}

ShowACPlayer(playerid, targetid)
{
    if (!IsPlayerConnected(targetid)) return ShowACMenu(playerid);

    SetPVarInt(playerid, "ac_target", targetid);

    new version[24], body[1600], title[96];
    AC_GetClientVersion(targetid, version, sizeof(version));

    format(title, sizeof(title), "{MAIN}RAC {B4B5B7}- {FFFFFF}%s [%d]", User[targetid][userName], targetid);

    format(body, sizeof(body), "{BEBEBE}Item\t{BEBEBE}Value\n");

    strcat(body, "{MAIN}\xA4 Client\t \n", sizeof(body));

    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}Version\t{FFFFFF}%s\n", version), sizeof(body));
    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}Platform\t{FFFFFF}%s\n", IsPlayerAndroid(targetid) ? ("mobile") : ("desktop")), sizeof(body));

    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}CLEO\t%s\n",
        (IsPlayerUsingCleo(targetid) == 1) ? ("{F84F31}Yes") :
        ((IsPlayerUsingCleo(targetid) == 0) ? ("{23C552}No") : ("{B4B5B7}No answer"))), sizeof(body));

    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}SAMPFUNCS\t%s\n",
        (IsPlayerUsingSampfuncs(targetid) == 1) ? ("{F84F31}Yes") :
        ((IsPlayerUsingSampfuncs(targetid) == 0) ? ("{23C552}No") : ("{B4B5B7}No answer"))), sizeof(body));

    strcat(body, "{MAIN}\xA4 Connection\t \n", sizeof(body));

    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}Ping\t{FFFFFF}%d ms\n", GetPlayerPing(targetid)), sizeof(body));
    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}Packet Loss\t{FFFFFF}%.2f%%\n", NetStats_PacketLossPercent(targetid)), sizeof(body));
    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}FPS\t{FFFFFF}%d\n", User[targetid][FPS]), sizeof(body));

    strcat(body, "{MAIN}\xA4 This Session\t \n", sizeof(body));

    if (ACSuspect[targetid][ac_cheatFlags] > 0)
    {
        new elapsed = gettime() - ACSuspect[targetid][ac_lastCheatTime];

        strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}Detections\t{F84F31}%d\n", ACSuspect[targetid][ac_cheatFlags]), sizeof(body));
        strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}Last One\t{F84F31}%s {B4B5B7}(%ds ago)\n", ACSuspect[targetid][ac_lastCheat], elapsed), sizeof(body));
    }
    else strcat(body, "{B4B5B7}\x95 {FFFFFF}Detections\t{23C552}Clean\n", sizeof(body));

    format_MainColor(playerid, body, sizeof(body));

    format_MainColor(playerid, title);
    format_MainColor(playerid, body, sizeof(body));
    Dialog_ShowCallback(playerid, using public DIALOG_AC_PLAYER<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "*", "<");
    return 1;
}

ShowACHistory(playerid, targetid)
{
    new name[MAX_PLAYER_NAME + 1], userid = User[targetid][user_sqlid];
    format(name, sizeof(name), "%s", User[targetid][userName]);

    if (userid <= 0) return ShowACPlayer(playerid, targetid);

    inline const OnACHistory()
    {
        new rows = cache_num_rows(), body[1600], title[96];

        format(title, sizeof(title), "{MAIN}RAC {B4B5B7}- {FFFFFF}%s {B4B5B7}history", name);
        format(body, sizeof(body), "{BEBEBE}Detection\t{BEBEBE}Hits\t{BEBEBE}Last Time\n");

        if (!rows)
            strcat(body, "{B4B5B7}Nothing on record\t{B4B5B7}0\t{B4B5B7}-\n", sizeof(body));

        for (new r; r < rows && r < 25; r++)
        {
            new code, hits, last, detection[32];

            cache_get_value_name_int(r, "code", code);
            cache_get_value_name_int(r, "hits", hits);
            cache_get_value_name_int(r, "last", last);

            AC_GetDetectionName(code, detection, sizeof(detection));

            strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}#%02d %s\t{F84F31}%d\t{B4B5B7}%s\n",
                code + 1, detection, hits, returnDate(last)), sizeof(body));
        }

        format_MainColor(playerid, body, sizeof(body));

        format_MainColor(playerid, title);
        format_MainColor(playerid, body, sizeof(body));
        Dialog_ShowCallback(playerid, using public DIALOG_AC_HISTORY<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "<", "Close");
    }

    MySQL_TQueryInline(dbHandle, using inline OnACHistory,
        "SELECT `code`, `hits`, `last_seen` AS `last` FROM `ac_detection_totals` WHERE `user_id` = %d ORDER BY `hits` DESC", userid);
    return 1;
}

forward DIALOG_AC_MENU(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_AC_MENU(playerid, dialogid, response, listitem, string:inputtext[])
{
    new row = AC_TakeRow(playerid, listitem);

    if (!response) return 1;

    if (row >= AC_ROW_CATEGORY) return ShowACCategory(playerid, row - AC_ROW_CATEGORY);

    switch (row)
    {
        case AC_ROW_PUNISH:
        {
            ACPunishEnabled = !ACPunishEnabled;
            AC_SaveSetting("punish", _:ACPunishEnabled);

            va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s set the anti cheat to %s.",
                User[playerid][userName], ACPunishEnabled ? ("punish") : ("report only"));

            PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
        }
        case AC_ROW_R2:
        {
            R2KickEnabled = !R2KickEnabled;
            SaveServerConfig();

            va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s %s the Fake Version client check kick.",
                User[playerid][userName], R2KickEnabled ? ("enabled") : ("disabled"));

            PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
        }
        case AC_ROW_OMP:
        {
            OMPKickEnabled = !OMPKickEnabled;
            SaveServerConfig();

            va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s %s the open.mp launcher check kick.",
                User[playerid][userName], OMPKickEnabled ? ("enabled") : ("disabled"));

            PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
        }
        case AC_ROW_VERBOSE:
        {
            AC_ToggleVerbose(playerid);
            PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
        }
        case AC_ROW_LIMITS:
        {
            ShowAdminConfig(playerid);
            return 1;
        }
        case AC_ROW_FLAGGED: return ShowACFlagged(playerid);
        case AC_ROW_RECENT: return ShowACRecent(playerid);

        case AC_ROW_FALLDMG:  AC_ToggleExtra(playerid, ACNoFallDamage, "no_fall_damage", "no fall damage");
        case AC_ROW_FALLANIM: AC_ToggleExtra(playerid, ACNoFallAnimation, "no_fall_anim", "no fall animation");
        case AC_ROW_CJWALK:   AC_ToggleCJWalk(playerid);
        case AC_ROW_STUNS:    AC_ToggleExtra(playerid, ACFastStuns, "fast_stuns", "fast stuns");
    }

    return ShowACMenu(playerid);
}

forward DIALOG_AC_CATEGORY(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_AC_CATEGORY(playerid, dialogid, response, listitem, string:inputtext[])
{
    new detection = AC_TakeRow(playerid, listitem);

    if (!response) return ShowACMenu(playerid);
    if (detection < 0) return ShowACCategory(playerid, GetPVarInt(playerid, "ac_category"));

    return ShowACAction(playerid, detection);
}

forward DIALOG_AC_ACTION(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_AC_ACTION(playerid, dialogid, response, listitem, string:inputtext[])
{
    new detection = GetPVarInt(playerid, "ac_detection"), action = AC_TakeRow(playerid, listitem);

    if (!response) return ShowACCategory(playerid, GetPVarInt(playerid, "ac_category"));

    if (action < 0 || action >= sizeof(AC_ACTION_NAME)) return ShowACAction(playerid, detection);
    if (detection < 0 || detection >= MAX_AC_DETECTIONS) return ShowACMenu(playerid);

    AC_DETECTION_ACTION[detection] = action;

    new key[AC_KEY_LEN];
    format(key, sizeof(key), "code_%02d", detection + 1);
    AC_SaveSetting(key, action);

    new name[32];
    AC_GetDetectionName(detection, name, sizeof(name));

    va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s set the anti cheat action for %s to %s.",
        User[playerid][userName], name, AC_ACTION_NAME[action]);

    PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);

    return ShowACCategory(playerid, GetPVarInt(playerid, "ac_category"));
}

forward DIALOG_AC_FLAGGED(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_AC_FLAGGED(playerid, dialogid, response, listitem, string:inputtext[])
{
    new targetid = AC_TakeRow(playerid, listitem);

    if (!response) return ShowACMenu(playerid);
    if (targetid < 0) return ShowACFlagged(playerid);

    return ShowACPlayer(playerid, targetid);
}

forward DIALOG_AC_PLAYER(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_AC_PLAYER(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;
    return ShowACHistory(playerid, GetPVarInt(playerid, "ac_target"));
}

forward DIALOG_AC_HISTORY(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_AC_HISTORY(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (response) return ShowACPlayer(playerid, GetPVarInt(playerid, "ac_target"));
    return 1;
}

@cmd() ac(playerid, params[], help)
{
    if (User[playerid][adminLevel] < AC_MIN_ADMIN_LEVEL)
        return SendClientMessage(playerid, -1, "%s", UNKNOWN_COMMAND_MSG);

    CheckAdminAuth(playerid);

    return ShowACMenu(playerid);
}

@cmd(.alts = {"accheck"}) acinfo(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 2)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    new targetid;
    if (sscanf(params, "u", targetid))
        return SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/acinfo [id/nick]");

    if (!IsPlayerLogged(targetid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    return ShowACPlayer(playerid, targetid);
}

@cmd() acdebug(playerid, params[], help)
{
    if (User[playerid][adminLevel] < AC_MIN_ADMIN_LEVEL)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    return AC_ToggleVerbose(playerid);
}
