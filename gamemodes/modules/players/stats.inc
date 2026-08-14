Respawn(playerid, timestamp)
{
    if(User[playerid][isAfk])
        return SendClientMessage(playerid, -1, "%s", AFK_ERROR_MSG);

    new duelState = GetPVarInt(playerid, "DuelS_State");
    if(duelState != DUELS_STATE_NONE)
        return SendClientMessage(playerid, -1, "%s", ERROR_RESPAWN_DUEL);

    if(User[playerid][adminLevel] >= 4)
    {
        SetPlayerPos(playerid, 0, 0, 0);
        SpawnPlayerEx(playerid);
        return true;
    }

    if(User[playerid][timerRespawn] > timestamp)
        return SendClientMessage(playerid, -1, "%s", ERROR_RESPAWN);

    if(GetPVarInt(playerid, "isSpec") != 0)
        return SendClientMessage(playerid, -1, "%s", IN_SPEC_MODE);

    if(User[playerid][isCreatingArena])
        return SendClientMessage(playerid, -1, "%s", IN_ARENA_EDITOR);

    new Float:Armour;
    GetPlayerArmour(playerid, Armour);
    if(Armour < 20.0) return SendClientMessage(playerid, -1, "%s", CANNOT_RESPAWN);

    SetPlayerPos(playerid, 0, 0, 0);
    SpawnPlayerEx(playerid);

    User[playerid][timerRespawn] = gettime() + 2;

    return true;
}

ShowMenu(playerid)
{
    new strb[1536];

    // Header
    format(strb, sizeof(strb), "{F3230E}Preference\t{F3230E}Status / Value\n");
    

    format(strb, sizeof(strb), "%s{B4B5B7}World Time\t{FFFFFF}%02d:00\n", strb, User[playerid][time]);
    format(strb, sizeof(strb), "%s{B4B5B7}Weather ID\t{FFFFFF}%d\n", strb, User[playerid][weather]);
    format(strb, sizeof(strb), "%s{B4B5B7}Player HUD\t%s\n", strb, (User[playerid][showPlayerHud]) ? ("{23C552}Visible") : ("{F84F31}Hidden"));
    format(strb, sizeof(strb), "%s{B4B5B7}Damage Informer\t{447DBF}%s\n", strb, GetDamageStyleName(User[playerid][dmgInformerStyle]));
    format(strb, sizeof(strb), "%s{B4B5B7}Hitmarker Color\t{%06x}######\n", strb, (User[playerid][hitmarkColor] >>> 8));
    format(strb, sizeof(strb), "%s{B4B5B7}Interface Color\t"COL_WHITE"%s\n", strb, strtoupper(hudPersonalization[GetPlayerHudColorIndex(playerid)][lineInfo]));

    new shadeOne = GetPlayerShadeIndex(playerid, false), shadeTwo = GetPlayerShadeIndex(playerid, true);

    if(shadeOne == -1) format(strb, sizeof(strb), "%s{B4B5B7}Primary Color\t{BEBEBE}Interface preset\n", strb);
    else format(strb, sizeof(strb), "%s{B4B5B7}Primary Color\t{%06x}%s\n", strb, hudShades[shadeOne][shadeHex], hudShades[shadeOne][shadeName]);

    if(shadeTwo == -1) format(strb, sizeof(strb), "%s{B4B5B7}Secondary Color\t{BEBEBE}Interface preset\n", strb);
    else format(strb, sizeof(strb), "%s{B4B5B7}Secondary Color\t{%06x}%s\n", strb, hudShades[shadeTwo][shadeHex], hudShades[shadeTwo][shadeName]);

    new idHexM[7], textHexM[7];
    GetPlayerChatIDColorHex(playerid, idHexM);
    GetPlayerChatTextColorHex(playerid, textHexM);
    format(strb, sizeof(strb), "%s{B4B5B7}Chat Color\t{%s}ID: #%s {FFFFFF}/ {%s}Text: #%s\n", strb, idHexM, idHexM, textHexM, textHexM);

    format(strb, sizeof(strb), "%s{B4B5B7}Character Skin\t{FFFFFF}ID:%d\n", strb, GetPlayerSkin(playerid));
    format(strb, sizeof(strb), "%s{B4B5B7}Fighting Style\t{FFFFFF}ID:%d\n", strb, _:User[playerid][fightStyle]);
    format(strb, sizeof(strb), "%s{B4B5B7}Weapon Set\t{FFFFFF}Change", strb);

    Dialog_ShowCallback(playerid, using public DIALOG_MENU<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "{F3230E}Settings Menu", strb, "Confirm", "Exit");
}

ShowAdminConfig(playerid)
{
    new strb[3072], count = 0, actions[32];

    format(strb, sizeof(strb), "{A29582}Setting\t{A29582}Status\t{A29582}Info\n");

    if(User[playerid][adminLevel] >= 4)
    {
        strcat(strb, "{MAIN}\xBB Performance\t \t \n");
        actions[count++] = 0;

        format(strb, sizeof(strb), "%s{FFFFFF}Max Ping\t{BEBEBE}%d ms\t{BEBEBE}Kicks anyone above this ping\n", strb, ServerMaxPing);
        actions[count++] = 2; // Max Ping

        format(strb, sizeof(strb), "%s{FFFFFF}Max Packetloss\t{BEBEBE}%.2f%%\t{BEBEBE}Kicks anyone above this loss\n", strb, ServerMaxPacketLoss);
        actions[count++] = 3; // Max PL

        format(strb, sizeof(strb), "%s{FFFFFF}Min FPS\t{BEBEBE}%d FPS\t{BEBEBE}Kicks anyone below this framerate\n", strb, ServerMinFPS);
        actions[count++] = 4; // Min FPS

        format(strb, sizeof(strb), "%s{FFFFFF}Max FPS\t{BEBEBE}%d FPS\t{BEBEBE}Kicks anyone above this framerate\n", strb, ServerMaxFPS);
        actions[count++] = 12; // Max FPS

        if(User[playerid][adminLevel] >= 5)
        {
            strcat(strb, "{MAIN}\xBB Gameplay & Interface\t \t \n");
            actions[count++] = 0;

            new hudName[32];
            format(hudName, sizeof(hudName), "%s", (0 <= ServerDefaultHud < MAX_HUD_TYPES) ? HUD_TYPES[ServerDefaultHud] : "Unknown");
            format(strb, sizeof(strb), "%s{FFFFFF}Default Server HUD\t{BEBEBE}%s\t{BEBEBE}HUD new players start with\n", strb, hudName);
            actions[count++] = 14; // Default Server HUD

            new colorName[32];
            if(0 <= ServerDefaultHudColor < sizeof(hudPersonalization))
                format(colorName, sizeof(colorName), "%s", hudPersonalization[ServerDefaultHudColor][lineInfo]);
            else
                format(colorName, sizeof(colorName), "Default");
            format(strb, sizeof(strb), "%s{FFFFFF}Default HUD Color\t{BEBEBE}%s\t{BEBEBE}Colour new players start with\n", strb, colorName);
            actions[count++] = 15; // Default HUD Color

            strcat(strb, "{MAIN}\xBB Moderation\t \t \n");
            actions[count++] = 0;

            format(strb, sizeof(strb), "%s{FFFFFF}Max. Warnings\t{BEBEBE}%d warnings\t{BEBEBE}Warnings before an automatic ban\n", strb, ServerMaxWarnings);
            actions[count++] = 16;

            format(strb, sizeof(strb), "%s{FFFFFF}Max. Login Attempts\t{BEBEBE}%d attempts\t{BEBEBE}Wrong passwords before locking the account\n", strb, ServerMaxLoginAttempts);
            actions[count++] = 17;

            format(strb, sizeof(strb), "%s{FFFFFF}Max. Accounts IP-GPCI\t{BEBEBE}%d accounts\t{BEBEBE}Accounts allowed per IP or serial\n", strb, ServerMaxAccountsPerIP);
            actions[count++] = 18;

            format(strb, sizeof(strb), "%s{FFFFFF}Anti Cheat Cooldown\t{BEBEBE}%d s\t{BEBEBE}Wait before the same detection counts again\n", strb, ServerACCooldown);
            actions[count++] = 22;

            format(strb, sizeof(strb), "%s{FFFFFF}Toggle Anti Spam\t%s\t{BEBEBE}Kicks chat and command flood\n", strb, (ServerAntiSpam) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
            actions[count++] = 19;

            format(strb, sizeof(strb), "%s{FFFFFF}Toggle Anti Capslock\t%s\t{BEBEBE}Lowercases messages shouted in caps\n", strb, (ServerAntiCapslock) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
            actions[count++] = 20;

            format(strb, sizeof(strb), "%s{FFFFFF}Toggle '/Admins' Command\t%s\t{BEBEBE}Lets players list the online staff\n", strb, (ServerAdminsCmd) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
            actions[count++] = 21;
        }

        strcat(strb, "{MAIN}\xBB Server & Security\t \t \n");
        actions[count++] = 0;

        format(strb, sizeof(strb), "%s{FFFFFF}R2 Client Kick\t%s\t{BEBEBE}Kicks clients faking their version\n", strb, (R2KickEnabled) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
        actions[count++] = 13; // R2 Kick

        format(strb, sizeof(strb), "%s{FFFFFF}Server Lock\t%s\t{BEBEBE}Requires a password to join\n", strb, (ServerLocked) ? ("{F84F31}Locked") : ("{23C552}Unlocked"));
        actions[count++] = 5; // Lock

        if(IsPlayerAdmin(playerid))
        {
            format(strb, sizeof(strb), "%s{FFFFFF}Server Name\t{BEBEBE}%s\t{BEBEBE}Name shown in the launcher\n", strb, ServerHostname);
            actions[count++] = 10; // Hostname

            format(strb, sizeof(strb), "%s{FFFFFF}Max Slots\t{BEBEBE}%d slots\t{BEBEBE}How many players fit at once\n", strb, GetMaxPlayers());
            actions[count++] = 11; // Max Players
        }

        format(strb, sizeof(strb), "%s{FFFFFF}Restart Server\t{F84F31}Reboot\t{BEBEBE}Reloads the gamemode right now\n", strb);
        actions[count++] = 1; // Restart
    }

    strcat(strb, "{MAIN}\xBB Chat & Spy Permissions\t \t \n");
    actions[count++] = 0;

    format(strb, sizeof(strb), "%s{FFFFFF}See PM's\t%s\t{BEBEBE}Watch private messages\n", strb, (User[playerid][acfg_pm]) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
    actions[count++] = 6;

    format(strb, sizeof(strb), "%s{FFFFFF}See Clan Chat\t%s\t{BEBEBE}Watch clan chat\n", strb, (User[playerid][acfg_clanpm]) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
    actions[count++] = 7;

    format(strb, sizeof(strb), "%s{FFFFFF}See Commands\t%s\t{BEBEBE}Watch commands players run\n", strb, (User[playerid][acfg_cmds]) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
    actions[count++] = 8;

    format(strb, sizeof(strb), "%s{FFFFFF}See Local Chat\t%s\t{BEBEBE}Watch local chat", strb, (User[playerid][acfg_local]) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
    actions[count++] = 9;

    for(new i = 0; i < count; i++)
    {
        new pvar[32];
        format(pvar, sizeof(pvar), "acfg_action_%d", i);
        SetPVarInt(playerid, pvar, actions[i]);
    }
    SetPVarInt(playerid, "acfg_count", count);

    new title[64] = "{MAIN}Admin Configuration";
    format_MainColor(playerid, title);
    format_MainColor(playerid, strb, sizeof(strb));
    Dialog_ShowCallback(playerid, using public DIALOG_AMENU<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, strb, "Confirm", "Exit");
}

ShowStats(playerid, targetid)
{
    new strstts[3500], sampVERSION[24], ontime = (User[targetid][totalOnTime]+(gettime()-User[targetid][loginTime]));
    GetPlayerVersion(targetid, sampVERSION, sizeof(sampVERSION));

    if(ontime < 0) ontime = 0;

    new hour = ontime / 3600;
    new minute = (ontime % 3600) / 60;
    new second = ontime % 60;

    new dc_str[64];
    if(strequal(User[targetid][dc_username], "-1", true)) dc_str = "{F84F31}No Linked";
    else format(dc_str, sizeof(dc_str), "{7289DA}%s#%s", User[targetid][dc_username], User[targetid][dc_discriminator]);

    new regdate_str[32];
    if(isnull(User[targetid][user_regdate])) format(regdate_str, sizeof(regdate_str), "{BEBEBE}N/A");
    else format(regdate_str, sizeof(regdate_str), "{FFFFFF}%s", User[targetid][user_regdate]);

    format(strstts, sizeof(strstts), "{MAIN}Parameter\t{MAIN}Data\n");

    // Section 1: Overall Information
    strcat(strstts, "{MAIN}> Overall & Connection\t \n");
    format(strstts, sizeof(strstts), "%s{BEBEBE}Account ID\t{FFFFFF}#%d\n", strstts, User[targetid][user_sqlid]);
    format(strstts, sizeof(strstts), "%s{BEBEBE}Created At\t%s\n", strstts, regdate_str);
    format(strstts, sizeof(strstts), "%s{BEBEBE}Discord\t%s\n", strstts, dc_str);
    format(strstts, sizeof(strstts), "%s{BEBEBE}Client SA-MP\t{FFFFFF}%s\n", strstts, sampVERSION);
    format(strstts, sizeof(strstts), "%s{BEBEBE}Mobile Client\t%s\n", strstts, (User[targetid][isMobile]) ? ("{328E0A}Yes") : ("{F84F31}No"));
    format(strstts, sizeof(strstts), "%s{BEBEBE}FPS / Ping / Loss\t{FFFFFF}%d FPS {B4B5B7}| {FFFFFF}%d ms {B4B5B7}| {FFFFFF}%.2f%%\n", strstts, User[targetid][FPS], GetPlayerPing(targetid), NetStats_PacketLossPercent(targetid));
    format(strstts, sizeof(strstts), "%s{BEBEBE}Skin ID\t{FFFFFF}#%d\n", strstts, GetPlayerSkin(targetid));
    format(strstts, sizeof(strstts), "%s{BEBEBE}Admin Rank\t{FFFFFF}%s\n", strstts, adminnames[(User[targetid][adminCamu] ? 0 : User[targetid][adminLevel])][ingame]);
    if(IsPlayerVip(targetid))
    {
        new tagColor[7] = "FFD700";
        new tagIdx = User[targetid][vipHeadTag];
        if(0 <= tagIdx < 10)
            format(tagColor, sizeof(tagColor), "%s", vipColorListStr[tagIdx]);
        else if(tagIdx >= 0x10000000)
            format(tagColor, sizeof(tagColor), "%06x", tagIdx & 0xFFFFFF);

        if(User[targetid][vipExpire] == 0)
            format(strstts, sizeof(strstts), "%s{BEBEBE}VIP Status\t{%s}VIP Permanent\n", strstts, tagColor);
        else
        {
            new rem = User[targetid][vipExpire] - gettime();
            format(strstts, sizeof(strstts), "%s{BEBEBE}VIP Status\t{%s}VIP {FFFFFF}%dd %dh left\n", strstts, tagColor, rem / 86400, (rem % 86400) / 3600);
        }
    }
    else strcat(strstts, "{BEBEBE}VIP Status\t{BEBEBE}None\n");
    format(strstts, sizeof(strstts), "%s{BEBEBE}Online Time\t{FFFFFF}%02dh %02dm %02ds\n", strstts, hour, minute, second);
    format(strstts, sizeof(strstts), "%s{BEBEBE}Money\t{23C552}%s\n", strstts, FormatMoney(User[targetid][playerMoney]));
    format(strstts, sizeof(strstts), "%s{BEBEBE}Coins\t{FFFF00}%s\n", strstts, FormatCoins(User[targetid][playerCoins]));

    // Section 2: Global Records
    strcat(strstts, "{MAIN}> Combat Records\t \n");
    format(strstts, sizeof(strstts), "%s{BEBEBE}Level\t{FFFFFF}%d\n", strstts, User[targetid][playerLevel]);
    format(strstts, sizeof(strstts), "%s{BEBEBE}Total Kills / Deaths\t{23C552}%d {FFFFFF}/ {F84F31}%d\n", strstts, User[targetid][playerKills], User[targetid][playerDeaths]);
    format(strstts, sizeof(strstts), "%s{BEBEBE}K/D Ratio\t{23C552}%.2f\n", strstts, ratio(User[targetid][playerKills], User[targetid][playerDeaths]));
    format(strstts, sizeof(strstts), "%s{BEBEBE}Best Killstreak\t{23C552}%d kills\n", strstts, User[targetid][maxStreak]);
    format(strstts, sizeof(strstts), "%s{BEBEBE}Best Arena Damage\t{23C552}%d DMG\n", strstts, User[targetid][maxDamage]);
    format(strstts, sizeof(strstts), "%s{BEBEBE}Arenas Won (MVP)\t{23C552}%d arenas\n", strstts, User[targetid][wonArenas]);
    format(strstts, sizeof(strstts), "%s{BEBEBE}Duel Record (W/L)\t{23C552}%d W {FFFFFF}/ {F84F31}%d L {B4B5B7}(%.2f W/L)\n", strstts, User[targetid][duelWins], User[targetid][duelLosses], ratio(User[targetid][duelWins], User[targetid][duelLosses]));
    format(strstts, sizeof(strstts), "%s{BEBEBE}2v2 Record (W/L)\t{23C552}%d W {FFFFFF}/ {F84F31}%d L {B4B5B7}(%.2f W/L)\n", strstts, User[targetid][duel2v2Wins], User[targetid][duel2v2Losses], ratio(User[targetid][duel2v2Wins], User[targetid][duel2v2Losses]));

    // Section 3: Clan
    strcat(strstts, "{MAIN}> Clan Information\t \n");
    if(User[targetid][clanID] != -1)
    {
        new cid = User[targetid][clanID];
        new clanColorHex[7];
        format(clanColorHex, sizeof(clanColorHex), "%06x", ClanInfo[cid][clanColor] >>> 8);
        format(strstts, sizeof(strstts), "%s{BEBEBE}Clan Name & Tag\t{%s}%s {FFFFFF}[%s]\n", strstts, clanColorHex, ClanInfo[cid][clanName], ClanInfo[cid][clanTag]);
        format(strstts, sizeof(strstts), "%s{BEBEBE}Clan Kills / Points\t{FFFFFF}%d kills {B4B5B7}| {FFFFFF}%d pts\n", strstts, ClanInfo[cid][clanKills], ClanInfo[cid][clanPoints]);
    }
    else strcat(strstts, "{BEBEBE}Clan Status\t{F84F31}No Clan\n");

    // Section 4: Current Session
    strcat(strstts, "{MAIN}> Current Session\t \n");
    if (User[targetid][locationId] != -1)
    {
        new loc = User[targetid][locationId];
        format(strstts, sizeof(strstts), "%s{BEBEBE}Current Arena\t{FFFFFF}Arena #%d\n", strstts, arenaInfo[loc][arenaId]);
        format(strstts, sizeof(strstts), "%s{BEBEBE}Session K / D / DMG\t{23C552}%d K {FFFFFF}/ {F84F31}%d D {FFFFFF}/ {23C552}%.0f DMG\n", strstts, playerArenaInfo[loc][targetid][arenaKills], playerArenaInfo[loc][targetid][arenaDeaths], playerArenaInfo[loc][targetid][arenaDMG]);
        format(strstts, sizeof(strstts), "%s{BEBEBE}Session Ratio\t{23C552}%.2f\n", strstts, ratio(playerArenaInfo[loc][targetid][arenaKills], playerArenaInfo[loc][targetid][arenaDeaths]));
        format(strstts, sizeof(strstts), "%s{BEBEBE}Weapon Accuracy\t{23C552}%d%%\n", strstts, floatround(GetPlayerAccuracy(targetid, loc)));
    }
    else strcat(strstts, "{BEBEBE}Current Location\t{FFFFFF}Lounge / Lobby\n");

    new title[64];
    format(title, sizeof(title), "{MAIN}Profile Stats: {FFFFFF}%s", getPlayerName(targetid));
    format_MainColor(playerid, title);
    format_MainColor(playerid, strstts);

    Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, strstts, "Close", "");
    return 1;
}

ShowPlayerClanInfo(playerid, targetid)
{
    new clanid = User[targetid][clanID];
    if(clanid == -1) return 0;

    new infoStr[1024], createdAt[32], clanColorHex[10];
    new onlineMembers = GetClanOnlineMembers(clanid);
    new ownedOutposts = GetClanOwnedOutposts(clanid);
    
    if(ClanInfo[clanid][clanCreatedAt] > 0)
        format(createdAt, sizeof(createdAt), "%s", returnDate(ClanInfo[clanid][clanCreatedAt], 1));
    else
        format(createdAt, sizeof(createdAt), "N/A");

    format(clanColorHex, sizeof(clanColorHex), "%06x", ClanInfo[clanid][clanColor] >>> 8);

    format(infoStr, sizeof(infoStr), "{A29582}Parameter\t{A29582}Value\n");
    strcat(infoStr, fmat_return("Clan Name\t{%s}%s\n", clanColorHex, ClanInfo[clanid][clanName]));
    strcat(infoStr, fmat_return("Clan ID\t%d\n", ClanInfo[clanid][clanID]));
    strcat(infoStr, fmat_return("Clan Tag\t%s\n", ClanInfo[clanid][clanTag]));
    strcat(infoStr, fmat_return("Created At\t%s\n", createdAt));
    strcat(infoStr, fmat_return("Total Members\t%d / %d\n", ClanMembersCount[clanid], MAX_MEMBERS));
    strcat(infoStr, fmat_return("Online Members\t%d\n", onlineMembers));
    strcat(infoStr, fmat_return("Territories / Points\t%d / %d", ownedOutposts, ClanInfo[clanid][clanPoints]));

    new title[64];
    format(title, sizeof(title), "{MAIN}Clan Info: {FFFFFF}%s", ClanInfo[clanid][clanName]);
    format_MainColor(playerid, title);

    Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, infoStr, "Close", "");
    return 1;
}



ShowBanDialog(playerid, const adminname[], const reason[], const banDate[], const unbanDate[])
{
    #pragma unused unbanDate
    
    new safe_admin[25], safe_reason[120], safe_date[50];
    strcopy(safe_admin, adminname);
    strcopy(safe_reason, reason);
    strcopy(safe_date, banDate);

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), 
        "SELECT \
        (SELECT COUNT(*) FROM `banimentos` WHERE `gpci` = '%e') as gpci_ban, \
        (SELECT COUNT(*) FROM `ispsbanidas` WHERE `isp` = '%e') as isp_ban",
        User[playerid][user_gpci], User[playerid][Isp]);

    inline OnBanDetailsCheck()
    {
        new gpci_ban_count = 0, isp_ban_count = 0;
        cache_get_value_name_int(0, "gpci_ban", gpci_ban_count);
        cache_get_value_name_int(0, "isp_ban", isp_ban_count);

        new strban[2048];
        format(strban, sizeof(strban),"{FFFFFF}This account is banned from the server!\n\n");
        strcat(strban, "{FFD500}      BAN DETAILS:\n\n");
        format(strban, sizeof(strban), "%s{FF644D}Account banned: {FFFFFF}%s\n", strban, User[playerid][userName]);
        format(strban, sizeof(strban), "%s{FF644D}Admin: {FFFFFF}%s\n", strban, safe_admin);
        format(strban, sizeof(strban), "%s{FF644D}Reason: {FFFFFF}%s\n", strban, safe_reason);
        format(strban, sizeof(strban), "%s{FF644D}Date: {FFFFFF}%s\n", strban, safe_date);
        
        format(strban, sizeof(strban), "%s{FF644D}GPCI: %s\n", strban, (gpci_ban_count > 0) ? ("{FF0000}Banned") : ("{00FF00}Clean"));
        format(strban, sizeof(strban), "%s{FF644D}ISP: %s\n\n", strban, (isp_ban_count > 0) ? ("{FF0000}Banned") : ("{00FF00}Clean"));

        strcat(strban, fmat_return("{FFFFFF}Site({4282b3}%s {FFFFFF})", ServerHostname));

        SendClientMessage(playerid, 0xF3230EFF, "{MAIN}[BAN] {FFFFFF}This account is banned!");
        Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "{E63114}BANNED!!!", strban, "Close", "");
    }
    MySQL_TQueryInline(dbHandle, using inline OnBanDetailsCheck, queryGlobal);
    return 1;
}

ChangeTime(playerid, timeid)
{
	if(timeid < 0 || timeid > 23) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid world time. Use a value between 0 and 23.");

	SetPlayerTime(playerid, timeid, 0);
	va_SendClientMessage(playerid, 0xebebebFF, TIME_INFO, timeid);
    User[playerid][time] = timeid;

	return true;
}

ChangeWeather(playerid, weatherid)
{
	if(weatherid < 0 || weatherid > 45) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid weather ID. Use a value between 0 and 45.");

	SetPlayerWeather(playerid, weatherid);
    va_SendClientMessage(playerid, 0xebebebFF, WEATHER_INFO, weatherid);
    User[playerid][weather] = weatherid;

    return true;
}

ChangeSkin(playerid, skinid)
{
	if(skinid < 2 || skinid > 311) return SendClientMessage(playerid,-1, "%s", SKIN_HELP2);

    if(skinid == 74 || skinid == 86 || skinid == 271 || skinid == 270)
    {
    	va_SendClientMessage(playerid, COLOR_ABACATE, "{MAIN}[ERROR] {FFFFFF}Skin ID %d isn't valid.", skinid);
    	skinid = GetPlayerSkin(playerid);
    }

    SetPlayerSkin(playerid, skinid);
    User[playerid][playerSkin] = skinid;

    // mantem o ponto de spawn do lounge: passar 0,0,0 aqui jogava o respawn na origem do mapa
    SetSpawnInfo(playerid, 0, User[playerid][playerSkin],
        User[playerid][loungeX], User[playerid][loungeY], User[playerid][loungeZ], User[playerid][loungeA],
        WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);

    va_SendClientMessage(playerid, 0xebebebFF, SKIN_INFO, GetPlayerSkin(playerid));
    return true;
}

ChangeFight(playerid)
{
	new fightstr[512];
	fightstr[0] = '\0';

    for(new i; i < sizeof(fightStyles); i++)
        AppendStyleRow(fightstr, fightStyles[i][fightname], (User[playerid][fightStyle] == fightStyles[i][styleid]));

    new title[64];
    format(title, sizeof(title), "{MAIN}Select Fighting Style");
    format_MainColor(playerid, title);
    format_MainColor(playerid, fightstr);

    Dialog_ShowCallback(playerid, using public FIGHT<iiiis>, DIALOG_STYLE_LIST, title, fightstr, "Select", "Cancel");
}

showMapListDialog(playerid)
{
    new dialogStr[1024], currentPage, Float:arenaarea, arenaiddd;
    static const categoryNames[][] = {"Small", "Medium", "Big"};

    currentPage = GetPVarInt(playerid, "playerCurrentPage");

    strcat(dialogStr, "ID\tArena Name\tArea\tSize\n");
    for(new i; i < 16; i++)
    {
        arenaiddd = i+(currentPage*16);

        if(arenaiddd >= MAX_ZONES || IsCustomArena(arenaiddd))
        {
            SetPVarInt(playerid, "pageItems", i);
            break;
        }
        SetPVarInt(playerid, "pageItems", i+1);
        
        arenaarea = (arenaData[arenaiddd][arenaPos][0] - arenaData[arenaiddd][arenaPos][2]) * (arenaData[arenaiddd][arenaPos][1] - arenaData[arenaiddd][arenaPos][3]);
        if(arenaarea < 0) arenaarea *= -1;

        new catID = _:arenaData[arenaiddd][category];
        strcat(dialogStr,
            sprintf("%d\t%s\t%0.1fm\t%s\n", 
                arenaiddd, 
                arenaData[arenaiddd][arenaName], 
                arenaarea, 
                (catID >= 0 && catID < sizeof(categoryNames)) ? categoryNames[catID] : "N/A"
            )
        );
    }
    strcat(dialogStr, "{00ff00}>>>\tNext Page\t \t \n{ff0000}<<<\tPrevious Page\t \t ");

    Dialog_ShowCallback(playerid, using public DIALOG_MAPLIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "SELECT MAP", dialogStr, "Select", "Close");
}

ShowGunMenu(playerid)
{
	if(GetPVarInt(playerid, "isSpec") != 0)
		return SendClientMessage(playerid, -1, "%s", IN_SPEC_MODE);

    if(User[playerid][isCreatingArena])
        return SendClientMessage(playerid, -1, "%s", IN_ARENA_EDITOR);

    if(User[playerid][isAfk])
        return SendClientMessage(playerid, 0xFFFFFFFF, "%s", AFK_ERROR_MSG);

    new Float:Armour;
    GetPlayerArmour(playerid, Armour);
    if(Armour < 20.0)
        return SendClientMessage(playerid, -1, "%s", CANNOT_DO_THIS);

    gunMenu(playerid);
    return true;
}

forward MSGVALIDATION(playerid, const code[]);
public MSGVALIDATION(playerid, const code[])
{
	PMPlayer = DCC_GetCreatedPrivateChannel();
    
    if(PMPlayer == DCC_INVALID_CHANNEL)
    {
        SendClientMessage(playerid, 0xFFFFFFFF, "{MAIN}[ERROR] {FFFFFF}Could not send the code to your Discord DM. Are your DMs open?");
        return 0;
    }

	new DCC_Embed:v_embed = DCC_CreateEmbed(.title = ServerHostname, .color = DISCORD_COLOR_INFO);
    
    new footerStr[64];
    format(footerStr, sizeof(footerStr), "%s • %s", ServerHostname, getDateStrTD());
    DCC_SetEmbedFooter(v_embed, footerStr);

	new tmpStr[500];
	format(tmpStr, sizeof(tmpStr), "%s", getPlayerName(playerid));

	DCC_AddEmbedField(v_embed, "Player", tmpStr, true);
	DCC_AddEmbedField(v_embed, "Code", code, true);
	DCC_SetEmbedImage(v_embed, "https://i.imgur.com/hib1Fwk.png");
    DCC_SendChannelEmbedMessage(PMPlayer, v_embed);

    SendClientMessage(playerid, 0xFFFFFFFF, "{MAIN}[DISCORD] {FFFFFF}Validation code sent to your Discord DM.");
    return 1;
}

stock DCC_GetISOTimestamp()
{
    new timestamp[25], year, month, day, hour, minute, second;
    getdate(year, month, day);
    gettime(hour, minute, second);
    format(timestamp, sizeof(timestamp), "%04d-%02d-%02dT%02d:%02d:%02dZ", year, month, day, hour, minute, second);
    return timestamp;
}