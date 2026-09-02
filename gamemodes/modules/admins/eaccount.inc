#if defined _eaccount_included
    #endinput
#endif
#define _eaccount_included

#include <YSI_Coding\y_hooks>

enum E_EACC_FIELD
{
    eaccLabel[22],
    eaccColumn[18],
    eaccMin,
    eaccMax
}

enum
{
    EACC_LEVEL,
    EACC_KILLS,
    EACC_WEEKKILLS,
    EACC_DEATHS,
    EACC_MAXDMG,
    EACC_WEEKDMG,
    EACC_STREAK,
    EACC_MONEY,
    EACC_COINS,
    EACC_ARENAS,
    EACC_DUELWINS,
    EACC_DUELLOSSES,
    EACC_D2V2WINS,
    EACC_D2V2LOSSES,
    EACC_WARNINGS,
    EACC_ADMINLEVEL,
    EACC_VIPLEVEL,
    EACC_VIPDAYS,

    EACC_FIELD_COUNT
}

static const EAccField[EACC_FIELD_COUNT][E_EACC_FIELD] =
{
    {"Level",            "level",          0, 1000},
    {"Kills",            "kills",          0, 9999999},
    {"Weekly Kills",     "kills_semanais", 0, 9999999},
    {"Deaths",           "deaths",         0, 9999999},
    {"Max Damage",       "max_damage",     0, 9999999},
    {"Weekly Damage",    "damage_semanal", 0, 9999999},
    {"Max Killstreak",   "max_streak",     0, 99999},
    {"Money",            "player_money",   0, 999999999},
    {"Coins",            "coins",          0, 999999999},
    {"Won Arenas",       "won_arenas",     0, 9999999},
    {"Duel Wins",        "duel_wins",      0, 9999999},
    {"Duel Losses",      "duel_losses",    0, 9999999},
    {"2v2 Wins",         "duel2v2_wins",   0, 9999999},
    {"2v2 Losses",       "duel2v2_losses", 0, 9999999},
    {"Warnings",         "warnings",       0, 100},
    {"Admin Level",      "adminlevel",     0, MAX_ADMIN_LEVEL},
    {"VIP Level",        "vip_level",      0, 3},
    {"VIP Days Left",    "vip_expire",     0, 1825}
};

enum
{
    ECLAN_KILLS,
    ECLAN_POINTS,

    ECLAN_FIELD_COUNT
}

static const EClanField[ECLAN_FIELD_COUNT][E_EACC_FIELD] =
{
    {"Clan Kills",       "", 0, 9999999},
    {"Clan Zone Points", "", 0, 9999999}
};

static
    EAccSqlID[MAX_PLAYERS],
    EAccName[MAX_PLAYERS][MAX_PLAYER_NAME + 1],
    EAccCache[MAX_PLAYERS][EACC_FIELD_COUNT],
    EAccClan[MAX_PLAYERS],
    EAccMemberClan[MAX_PLAYERS],
    EAccMemberRole[MAX_PLAYERS];

forward DIALOG_EACC_LIST(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EACC_INPUT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EACC_CLAN(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EACC_CLANINPUT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EACC_CONFIRM_DELETE(playerid, dialogid, response, listitem, string:inputtext[]);

static bool:EAcc_IsOffline(playerid)
{
    return (EAccSqlID[playerid] > 0);
}

static EAcc_OnlineTarget(playerid)
{
    new targetid = GetPVarInt(playerid, "EAccTarget") - 1;

    if (!(0 <= targetid < MAX_PLAYERS) || !IsPlayerLogged(targetid))
        return INVALID_PLAYER_ID;

    return targetid;
}

static EAcc_Clear(playerid)
{
    EAccSqlID[playerid] = 0;
    EAccClan[playerid] = -1;
    EAccMemberClan[playerid] = -1;
    EAccMemberRole[playerid] = -1;
    EAccName[playerid][0] = EOS;

    DeletePVar(playerid, "EAccTarget");
    DeletePVar(playerid, "EAccField");
    return 1;
}

hook OnPlayerConnect(playerid)
{
    EAcc_Clear(playerid);
    return 1;
}

static bool:EAcc_Ready(playerid)
{
    if (User[playerid][adminLevel] < MAX_ADMIN_LEVEL || !IsPlayerAdmin(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You no longer have permission to edit accounts.");
        EAcc_Clear(playerid);
        return false;
    }

    if (EAcc_IsOffline(playerid)) return true;

    if (EAcc_OnlineTarget(playerid) == INVALID_PLAYER_ID)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The target player is no longer connected.");
        EAcc_Clear(playerid);
        return false;
    }
    return true;
}

static EAcc_TargetName(playerid, dest[], len = sizeof(dest))
{
    if (EAcc_IsOffline(playerid)) format(dest, len, "%s", EAccName[playerid]);
    else format(dest, len, "%s", User[EAcc_OnlineTarget(playerid)][userName]);

    return 1;
}

static EAcc_Read(playerid, field)
{
    if (EAcc_IsOffline(playerid))
    {
        if (field == EACC_VIPDAYS)
        {
            new left = EAccCache[playerid][field] - gettime();
            return (left > 0) ? ((left + 86399) / 86400) : 0;
        }
        return EAccCache[playerid][field];
    }

    new targetid = EAcc_OnlineTarget(playerid);
    if (targetid == INVALID_PLAYER_ID) return 0;

    switch (field)
    {
        case EACC_LEVEL:       return User[targetid][playerLevel];
        case EACC_KILLS:       return User[targetid][playerKills];
        case EACC_WEEKKILLS:   return User[targetid][kills_semanais];
        case EACC_DEATHS:      return User[targetid][playerDeaths];
        case EACC_MAXDMG:      return User[targetid][maxDamage];
        case EACC_WEEKDMG:     return User[targetid][dmgSemanal];
        case EACC_STREAK:      return User[targetid][maxStreak];
        case EACC_MONEY:       return User[targetid][playerMoney];
        case EACC_COINS:       return User[targetid][playerCoins];
        case EACC_ARENAS:      return User[targetid][wonArenas];
        case EACC_DUELWINS:    return User[targetid][duelWins];
        case EACC_DUELLOSSES:  return User[targetid][duelLosses];
        case EACC_D2V2WINS:    return User[targetid][duel2v2Wins];
        case EACC_D2V2LOSSES:  return User[targetid][duel2v2Losses];
        case EACC_WARNINGS:    return User[targetid][warnings];
        case EACC_ADMINLEVEL:  return User[targetid][adminLevel];
        case EACC_VIPLEVEL:    return User[targetid][vipLevel];
        case EACC_VIPDAYS:
        {
            new left = User[targetid][vipExpire] - gettime();
            return (left > 0) ? ((left + 86399) / 86400) : 0;
        }
    }
    return 0;
}

static EAcc_Write(playerid, field, value)
{
    new stored = (field == EACC_VIPDAYS) ? ((value > 0) ? (gettime() + (value * 86400)) : 0) : value;

    if (EAcc_IsOffline(playerid))
    {
        EAccCache[playerid][field] = stored;

        new query[192];
        mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `%s` = %d WHERE `id` = %d",
            EAccField[field][eaccColumn], stored, EAccSqlID[playerid]);
        mysql_tquery(dbHandle, query);
        return 1;
    }

    new targetid = EAcc_OnlineTarget(playerid);
    if (targetid == INVALID_PLAYER_ID) return 0;

    if (field == EACC_COINS && value != User[targetid][playerCoins])
    {
        new eaccReason[64];
        format(eaccReason, sizeof(eaccReason), "Edited by %s", User[playerid][userName]);

        LogCoins(User[targetid][user_sqlid], User[targetid][userName], eaccReason,
            value - User[targetid][playerCoins], value);
    }

    switch (field)
    {
        case EACC_LEVEL:       User[targetid][playerLevel]    = value;
        case EACC_KILLS:       User[targetid][playerKills]    = value;
        case EACC_WEEKKILLS:   User[targetid][kills_semanais] = value;
        case EACC_DEATHS:      User[targetid][playerDeaths]   = value;
        case EACC_MAXDMG:      User[targetid][maxDamage]      = value;
        case EACC_WEEKDMG:     User[targetid][dmgSemanal]     = value;
        case EACC_STREAK:      User[targetid][maxStreak]      = value;
        case EACC_MONEY:       User[targetid][playerMoney]    = value;
        case EACC_COINS:       User[targetid][playerCoins]    = value;
        case EACC_ARENAS:      User[targetid][wonArenas]      = value;
        case EACC_DUELWINS:    User[targetid][duelWins]       = value;
        case EACC_DUELLOSSES:  User[targetid][duelLosses]     = value;
        case EACC_D2V2WINS:    User[targetid][duel2v2Wins]    = value;
        case EACC_D2V2LOSSES:  User[targetid][duel2v2Losses]  = value;
        case EACC_WARNINGS:    User[targetid][warnings]       = value;
        case EACC_ADMINLEVEL:  User[targetid][adminLevel]     = value;
        case EACC_VIPLEVEL:    User[targetid][vipLevel]       = value;
        case EACC_VIPDAYS:     User[targetid][vipExpire]      = stored;
    }

    UpdatePlayerData(targetid);
    return 1;
}

static EAcc_ClanID(playerid)
{
    if (EAcc_IsOffline(playerid)) return EAccClan[playerid];

    new targetid = EAcc_OnlineTarget(playerid);
    if (targetid == INVALID_PLAYER_ID) return -1;

    return (GetPlayerClanRank(targetid) == RANK_FOUNDER) ? GetPlayerClan(targetid) : -1;
}

static bool:EAcc_OwnsClan(playerid)
{
    new clanid = EAcc_ClanID(playerid);
    return (clanid != -1 && IsValidClanID(clanid));
}

static bool:EAcc_IsDeleteOption(playerid, listitem)
{
    if (EAcc_OwnsClan(playerid))
        return (listitem == EACC_FIELD_COUNT + 1);

    return (listitem == EACC_FIELD_COUNT);
}

stock ShowEAccountDeleteConfirmDialog(playerid)
{
    if (!EAcc_Ready(playerid)) return 0;

    new bool:isOffline = EAcc_IsOffline(playerid);
    new targetid = EAcc_OnlineTarget(playerid);
    new sqlid = isOffline ? EAccSqlID[playerid] : User[targetid][user_sqlid];

    if ((!isOffline && targetid == playerid) || sqlid == User[playerid][user_sqlid])
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot delete your own active account.");
        return ShowEAccountDialog(playerid);
    }

    new name[MAX_PLAYER_NAME + 1];
    EAcc_TargetName(playerid, name);

    new memberClanId = isOffline ? EAccMemberClan[playerid] : (targetid != INVALID_PLAYER_ID ? User[targetid][clanID] : -1);
    new bool:ownsClan = EAcc_OwnsClan(playerid);

    new targetClanName[MAX_CLAN_NAME + 32] = "None";
    if (ownsClan)
    {
        format(targetClanName, sizeof(targetClanName), "%s [Founder, ID: %d]", ClanInfo[EAcc_ClanID(playerid)][clanName], EAcc_ClanID(playerid));
    }
    else if (memberClanId != -1 && IsValidClanID(memberClanId))
    {
        format(targetClanName, sizeof(targetClanName), "%s [Member, ID: %d]", ClanInfo[memberClanId][clanName], memberClanId);
    }

    new msg[768];
    format(msg, sizeof(msg),
        "{FFFFFF}Are you sure you want to {FF3333}DELETE {FFFFFF}this account?\n\n"\
        "{FFFFFF}\x95 {B4B5B7}Username: {FFFFFF}%s\n"\
        "{FFFFFF}\x95 {B4B5B7}Session: {FFFFFF}%s\n"\
        "{FFFFFF}\x95 {B4B5B7}Clan Affiliation: {FFFFFF}%s\n\n"\
        "{FF3333}[WARNING] {FFFFFF}This action is {FF3333}permanent and irreversible{FFFFFF}.\n"\
        "All stats, currency and records will be deleted from database.\n",
        name,
        isOffline ? ("Offline") : ("Online"),
        targetClanName
    );

    if (ownsClan)
    {
        strcat(msg, "{E26D5B}Notice: This user is the founder of a Clan.\nThe clan will be disbanded, and its Safehouse & Territories released.\n");
    }
    else if (memberClanId != -1 && IsValidClanID(memberClanId))
    {
        strcat(msg, fmat_return("{E26D5B}Notice: This user is a member of clan %s.\nTheir member slot will be freed up.\n", ClanInfo[memberClanId][clanName]));
    }

    strcat(msg, "\n{FFFFFF}Click {FF3333}'Delete'{FFFFFF} to confirm or {B4B5B7}'Cancel'{FFFFFF} to go back.");

    Dialog_ShowCallback(playerid, using public DIALOG_EACC_CONFIRM_DELETE<iiiis>, DIALOG_STYLE_MSGBOX, "{FF3333}Confirm Account Deletion", msg, "Delete", "Cancel");
    return 1;
}

public DIALOG_EACC_CONFIRM_DELETE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response)
    {
        return ShowEAccountDialog(playerid);
    }

    if (!EAcc_Ready(playerid)) return 1;

    new name[MAX_PLAYER_NAME + 1];
    EAcc_TargetName(playerid, name);

    new bool:isOffline = EAcc_IsOffline(playerid);
    new targetid = EAcc_OnlineTarget(playerid);
    new sqlid = isOffline ? EAccSqlID[playerid] : User[targetid][user_sqlid];

    if ((!isOffline && targetid == playerid) || sqlid == User[playerid][user_sqlid])
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot delete your own active account.");
        return ShowEAccountDialog(playerid);
    }

    new ownedClanId = EAcc_ClanID(playerid);
    new memberClanId = isOffline ? EAccMemberClan[playerid] : (targetid != INVALID_PLAYER_ID ? User[targetid][clanID] : -1);

    new bool:clanDisbanded = false;
    new bool:clanSlotFreed = false;

    if (ownedClanId != -1 && IsValidClanID(ownedClanId))
    {
        DeleteClan(playerid, ownedClanId);
        clanDisbanded = true;
    }
    else if (memberClanId != -1 && IsValidClanID(memberClanId))
    {
        for (new i = 0; i < MAX_MEMBERS; i++)
        {
            if (ClanMembers[memberClanId][i][member_slotused] && ClanMembers[memberClanId][i][member_id] == sqlid)
            {
                ResetClanMemberSlot(memberClanId, i);
                if (ClanMembersCount[memberClanId] > 0) ClanMembersCount[memberClanId]--;
                clanSlotFreed = true;
                break;
            }
        }
        ClearClanMemberAccount(sqlid);
    }

    if (!isOffline && targetid != INVALID_PLAYER_ID)
    {
        if (User[targetid][clanID] != -1)
        {
            User[targetid][clanID] = -1;
            User[targetid][clanRole] = -1;
            User[targetid][spawnSafehouse] = false;
            SetClanColor(targetid);
        }

        SendClientMessage(targetid, -1, "{MAIN}[ADMIN] {FFFFFF}Your account has been deleted by an administrator.");
        KickEx(targetid, 1000);
    }

    inline const OnAccountDeletedFromDB()
    {
        UpdateTOP3Kills();
        UpdateTOP3Damage();
        updateKillRank();
        UpdateTOP10Clans();
        UpdateServerStats();
    }

    new query[256];
    mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `accounts` WHERE `id` = %d LIMIT 1", sqlid);
    MySQL_TQueryInline(dbHandle, using inline OnAccountDeletedFromDB, query);

    new details[160];
    format(details, sizeof(details), "Deleted account %s (SQL ID: %d, clan disbanded: %d, clan slot freed: %d)",
        name, sqlid, clanDisbanded ? 1 : 0, clanSlotFreed ? 1 : 0);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "admin_cmd", "/eaccount", details);

    new statusExtra[128];
    if (clanDisbanded) format(statusExtra, sizeof(statusExtra), " Associated clan was disbanded and Safehouse released.");
    else if (clanSlotFreed) format(statusExtra, sizeof(statusExtra), " Member slot was released from clan %s.", ClanInfo[memberClanId][clanName]);

    va_SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}Account {MAIN}%s {FFFFFF}(ID: %d) was permanently deleted.%s",
        name, sqlid, statusExtra);

    EAcc_Clear(playerid);
    return 1;
}

stock ShowEAccountDialog(playerid)
{
    if (!EAcc_Ready(playerid)) return 0;

    new body[2048], title[96], name[MAX_PLAYER_NAME + 1];

    EAcc_TargetName(playerid, name);

    strcat(body, "{A29582}Setting\t{A29582}Current Value\n");

    for (new i = 0; i < EACC_FIELD_COUNT; i++)
        strcat(body, fmat_return("{FFFFFF}\x95 %s\t{BEBEBE}%d\n", EAccField[i][eaccLabel], EAcc_Read(playerid, i)));

    if (EAcc_OwnsClan(playerid))
        strcat(body, fmat_return("{23C552}\x95 Clan Settings\t{BEBEBE}%s\n", ClanInfo[EAcc_ClanID(playerid)][clanName]));

    strcat(body, "{FF3333}\x95 Delete Account\t{BEBEBE}Permanently remove account\n");

    if (EAcc_IsOffline(playerid)) format(title, sizeof(title), "{MAIN}Edit Account: {FFFFFF}%s (offline)", name);
    else format(title, sizeof(title), "{MAIN}Edit Account: {FFFFFF}%s [%d]", name, EAcc_OnlineTarget(playerid));

    Dialog_ShowCallback(playerid, using public DIALOG_EACC_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Close");
    return 1;
}

stock ShowEAccountClanDialog(playerid)
{
    if (!EAcc_Ready(playerid)) return 0;

    if (!EAcc_OwnsClan(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}This player no longer owns a clan.");

    new clanid = EAcc_ClanID(playerid), body[512], title[96];

    strcat(body, "{BEBEBE}Setting\t{BEBEBE}Current Value\n");
    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}%s\t{BEBEBE}%d\n", EClanField[ECLAN_KILLS][eaccLabel], ClanInfo[clanid][clanKills]));
    strcat(body, fmat_return("{B4B5B7}\x95 {FFFFFF}%s\t{BEBEBE}%d\n", EClanField[ECLAN_POINTS][eaccLabel], ClanInfo[clanid][clanPoints]));

    format(title, sizeof(title), "{MAIN}Edit Clan: {FFFFFF}%s", ClanInfo[clanid][clanName]);

    Dialog_ShowCallback(playerid, using public DIALOG_EACC_CLAN<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
    return 1;
}

public DIALOG_EACC_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return EAcc_Clear(playerid);

    if (!EAcc_Ready(playerid)) return 1;

    if (EAcc_IsDeleteOption(playerid, listitem))
        return ShowEAccountDeleteConfirmDialog(playerid);

    if (listitem == EACC_FIELD_COUNT && EAcc_OwnsClan(playerid))
        return ShowEAccountClanDialog(playerid);

    if (!(0 <= listitem < EACC_FIELD_COUNT)) return ShowEAccountDialog(playerid);

    SetPVarInt(playerid, "EAccField", listitem);

    new body[320], name[MAX_PLAYER_NAME + 1];
    EAcc_TargetName(playerid, name);

    format(body, sizeof(body), "{FFFFFF}\x95 {B4B5B7}Player: "COL_WHITE"%s\n{FFFFFF}\x95 {B4B5B7}Setting: "COL_WHITE"%s\n{FFFFFF}\x95 {B4B5B7}Current: "COL_WHITE"%d\n\n{FFFFFF}\x95 {B4B5B7}Allowed range: "COL_WHITE"%d to %d",
        name, EAccField[listitem][eaccLabel], EAcc_Read(playerid, listitem),
        EAccField[listitem][eaccMin], EAccField[listitem][eaccMax]);

    Dialog_ShowCallback(playerid, using public DIALOG_EACC_INPUT<iiiis>, DIALOG_STYLE_INPUT, "{MAIN}Edit Account - New Value", body, "Confirm", "Back");
    return 1;
}

public DIALOG_EACC_INPUT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowEAccountDialog(playerid);

    if (!EAcc_Ready(playerid)) return 1;

    new field = GetPVarInt(playerid, "EAccField");
    if (!(0 <= field < EACC_FIELD_COUNT)) return ShowEAccountDialog(playerid);

    if (!IsNumeric(inputtext) || strlen(inputtext) > 10)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Enter a valid number.");
        return ShowEAccountDialog(playerid);
    }

    new value = strval(inputtext);

    if (!(EAccField[field][eaccMin] <= value <= EAccField[field][eaccMax]))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}%s must be between {MAIN}%d {FFFFFF}and {MAIN}%d{FFFFFF}.",
            EAccField[field][eaccLabel], EAccField[field][eaccMin], EAccField[field][eaccMax]);
        return ShowEAccountDialog(playerid);
    }

    new before = EAcc_Read(playerid, field), name[MAX_PLAYER_NAME + 1];
    EAcc_TargetName(playerid, name);

    EAcc_Write(playerid, field, value);

    va_SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}%s of {MAIN}%s {FFFFFF}changed from {MAIN}%d {FFFFFF}to {MAIN}%d{FFFFFF}.",
        EAccField[field][eaccLabel], name, before, value);

    new details[160];
    format(details, sizeof(details), "%s: %d -> %d (target %s%s)", EAccField[field][eaccLabel], before, value, name,
        EAcc_IsOffline(playerid) ? (", offline") : (""));
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "admin_cmd", "/eaccount", details);

    if (field == EACC_KILLS || field == EACC_WEEKKILLS || field == EACC_MAXDMG || field == EACC_WEEKDMG || field == EACC_DEATHS)
    {
        UpdateTOP3Kills();
        UpdateTOP3Damage();
        updateKillRank();
        UpdateServerStats();
    }

    return ShowEAccountDialog(playerid);
}

public DIALOG_EACC_CLAN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowEAccountDialog(playerid);

    if (!EAcc_Ready(playerid)) return 1;
    if (!(0 <= listitem < ECLAN_FIELD_COUNT)) return ShowEAccountClanDialog(playerid);
    if (!EAcc_OwnsClan(playerid)) return ShowEAccountDialog(playerid);

    SetPVarInt(playerid, "EAccField", listitem);

    new clanid = EAcc_ClanID(playerid);
    new current = (listitem == ECLAN_KILLS) ? ClanInfo[clanid][clanKills] : ClanInfo[clanid][clanPoints];

    new body[256];
    format(body, sizeof(body), "{FFFFFF}\x95 {B4B5B7}Clan: "COL_WHITE"%s\n{FFFFFF}\x95 {B4B5B7}Setting: "COL_WHITE"%s\n{FFFFFF}\x95 {B4B5B7}Current: "COL_WHITE"%d\n\n{FFFFFF}\x95 {B4B5B7}Allowed range: "COL_WHITE"%d to %d",
        ClanInfo[clanid][clanName], EClanField[listitem][eaccLabel], current,
        EClanField[listitem][eaccMin], EClanField[listitem][eaccMax]);

    Dialog_ShowCallback(playerid, using public DIALOG_EACC_CLANINPUT<iiiis>, DIALOG_STYLE_INPUT, "{MAIN}Edit Clan - New Value", body, "Confirm", "Back");
    return 1;
}

public DIALOG_EACC_CLANINPUT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowEAccountClanDialog(playerid);

    if (!EAcc_Ready(playerid)) return 1;

    if (!EAcc_OwnsClan(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}This player no longer owns a clan.");

    new field = GetPVarInt(playerid, "EAccField");
    if (!(0 <= field < ECLAN_FIELD_COUNT)) return ShowEAccountClanDialog(playerid);

    if (!IsNumeric(inputtext) || strlen(inputtext) > 10)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Enter a valid number.");
        return ShowEAccountClanDialog(playerid);
    }

    new value = strval(inputtext);

    if (!(EClanField[field][eaccMin] <= value <= EClanField[field][eaccMax]))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}%s must be between {MAIN}%d {FFFFFF}and {MAIN}%d{FFFFFF}.",
            EClanField[field][eaccLabel], EClanField[field][eaccMin], EClanField[field][eaccMax]);
        return ShowEAccountClanDialog(playerid);
    }

    new clanid = EAcc_ClanID(playerid);
    new before = (field == ECLAN_KILLS) ? ClanInfo[clanid][clanKills] : ClanInfo[clanid][clanPoints];

    if (field == ECLAN_KILLS) ClanInfo[clanid][clanKills] = value;
    else ClanInfo[clanid][clanPoints] = value;

    SaveClanData(clanid);
    UpdateTOP10Clans();

    va_SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}%s of clan {MAIN}%s {FFFFFF}changed from {MAIN}%d {FFFFFF}to {MAIN}%d{FFFFFF}.",
        EClanField[field][eaccLabel], ClanInfo[clanid][clanName], before, value);

    new details[160];
    format(details, sizeof(details), "%s: %d -> %d (clan %s)", EClanField[field][eaccLabel], before, value, ClanInfo[clanid][clanName]);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "admin_cmd", "/eaccount", details);

    return ShowEAccountClanDialog(playerid);
}

stock LoadEAccountOffline(playerid, const name[])
{
    new query[512];

    mysql_format(dbHandle, query, sizeof(query),
        "SELECT `id`, `username`, `level`, `kills`, `kills_semanais`, `deaths`, `max_damage`, `damage_semanal`, \
        `max_streak`, `player_money`, `coins`, `won_arenas`, `duel_wins`, `duel_losses`, `duel2v2_wins`, \
        `duel2v2_losses`, `warnings`, `adminlevel`, `vip_level`, `vip_expire`, `clanid`, `clanrole` \
        FROM `accounts` WHERE `username` = '%e' LIMIT 1", name);

    inline const OnAccountLoaded()
    {
        if (!cache_num_rows())
            return va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}No account found with the name {MAIN}%s{FFFFFF}.", name);

        EAcc_Clear(playerid);

        cache_get_value_int(0, "id", EAccSqlID[playerid]);
        cache_get_value_name(0, "username", EAccName[playerid], MAX_PLAYER_NAME + 1);

        for (new i = 0; i < EACC_FIELD_COUNT; i++)
            cache_get_value_int(0, EAccField[i][eaccColumn], EAccCache[playerid][i]);

        new clanid, clanrole;
        cache_get_value_int(0, "clanid", clanid);
        cache_get_value_int(0, "clanrole", clanrole);

        EAccMemberClan[playerid] = (IsValidClanID(clanid)) ? clanid : -1;
        EAccMemberRole[playerid] = clanrole;
        EAccClan[playerid] = (clanrole == RANK_FOUNDER && IsValidClanID(clanid)) ? clanid : -1;

        ShowEAccountDialog(playerid);
        return 1;
    }

    MySQL_TQueryInline(dbHandle, using inline OnAccountLoaded, query);
    return 1;
}

@cmd() eaccount(playerid, params[], help)
{
    if (!IsPlayerLogged(playerid)) return true;

    if (User[playerid][adminLevel] < MAX_ADMIN_LEVEL || !IsPlayerAdmin(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need Admin Level 5 and RCON access to use this command.");
        return true;
    }

    CheckAdminAuth(playerid);

    new target[MAX_PLAYER_NAME + 1];
    if (sscanf(params, "s[25]", target))
    {
        SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/eaccount [playerid/name] {B4B5B7}- offline accounts are looked up by exact name.");
        return true;
    }

    EAcc_Clear(playerid);

    new targetid = INVALID_PLAYER_ID;

    if (IsNumeric(target) && strlen(target) <= 4)
    {
        new id = strval(target);
        if (0 <= id < MAX_PLAYERS && IsPlayerLogged(id)) targetid = id;
    }

    if (targetid == INVALID_PLAYER_ID)
    {
        foreach (new i : Player)
        {
            if (IsPlayerLogged(i) && !strcmp(User[i][userName], target, true))
            {
                targetid = i;
                break;
            }
        }
    }

    if (targetid != INVALID_PLAYER_ID)
    {
        SetPVarInt(playerid, "EAccTarget", targetid + 1);
        ShowEAccountDialog(playerid);
        return true;
    }

    LoadEAccountOffline(playerid, target);
    return true;
}
