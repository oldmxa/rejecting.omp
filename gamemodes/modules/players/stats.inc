Respawn(playerid, timestamp)
{
    if(IsPlayerAFK(playerid))
        return SendClientMessage(playerid, -1, "%s", AFK_ERROR_MSG);

    if(IsPlayerInMatch(playerid) || IsPlayerInTraining(playerid) || IsPlayerInPractice(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use respawn while in a duel, match, training, practice or safehouse.");

    new duelState = GetPVarInt(playerid, "DuelS_State");
    if(duelState != DUELS_STATE_NONE)
        return SendClientMessage(playerid, -1, "%s", ERROR_RESPAWN_DUEL);

    new loc = User[playerid][locationId];
    if(loc == ARENA_AFK)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use respawn in the lounge.");

    if(loc < 0 || loc >= MAX_LOCATIONS || !arenaInfo[loc][isRunningArena])
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot respawn while the round has ended.");

    if(IsPlayerInCombat(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot respawn while in combat or having taken damage recently.");

    if(IsAdmin(playerid, 4))
    {
        SetPlayerPos(playerid, 0, 0, 0);
        SpawnPlayerEx(playerid);
        return true;
    }

    if(User[playerid][timerRespawn] > timestamp)
        return SendClientMessage(playerid, -1, "%s", ERROR_RESPAWN);

    if(IsPlayerSpectating(playerid))
        return SendClientMessage(playerid, -1, "%s", IN_SPEC_MODE);

    new Float:Armour;
    GetPlayerArmour(playerid, Armour);
    if(Armour < 20.0) return SendClientMessage(playerid, -1, "%s", CANNOT_RESPAWN);

    SetPlayerPos(playerid, 0, 0, 0);
    SpawnPlayerEx(playerid);

    User[playerid][timerRespawn] = gettime() + 2;

    return true;
}

stock ShowMenu(playerid)
{
    return ShowPersonalizationMenu(playerid);
}

forward Stats_ShowAdminConfig(playerid);
public Stats_ShowAdminConfig(playerid)
{
    new strb[3072], count = 0, actions[48];

    format(strb, sizeof(strb), "{BEBEBE}Setting\t{BEBEBE}Status\t{BEBEBE}Info\n");

    if(IsAdmin(playerid, 4))
    {
        strcat(strb, "{MAIN}\xA4 Performance\t \t \n");
        actions[count++] = 0;

        format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Max Ping\t{BEBEBE}%d ms\t{BEBEBE}Kicks anyone above this ping\n", strb, ServerMaxPing);
        actions[count++] = 2; // Max Ping

        format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Max Packetloss\t{BEBEBE}%.2f%%\t{BEBEBE}Kicks anyone above this loss\n", strb, ServerMaxPacketLoss);
        actions[count++] = 3; // Max PL

        format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Min FPS\t{BEBEBE}%d FPS\t{BEBEBE}Kicks anyone below this framerate\n", strb, ServerMinFPS);
        actions[count++] = 4; // Min FPS

        format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Max FPS\t{BEBEBE}%d FPS\t{BEBEBE}Kicks anyone above this framerate\n", strb, ServerMaxFPS);
        actions[count++] = 12; // Max FPS

        if(IsAdmin(playerid, 5))
        {
            strcat(strb, "{MAIN}\xA4 Gameplay & Interface\t \t \n");
            actions[count++] = 0;

            new hudName[32];
            format(hudName, sizeof(hudName), "%s", (0 <= ServerDefaultHud < MAX_HUD_TYPES) ? HUD_TYPES[ServerDefaultHud] : "Unknown");
            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Default Server HUD\t{BEBEBE}%s\t{BEBEBE}HUD new players start with\n", strb, hudName);
            actions[count++] = 14; // Default Server HUD

            new colorName[40];
            if(0 <= ServerDefaultHudColor < sizeof(hudPersonalization))
                format(colorName, sizeof(colorName), "{%s}%s", GetColorHexTag(hudPersonalization[ServerDefaultHudColor][colorOne]), hudPersonalization[ServerDefaultHudColor][lineInfo]);
            else
                format(colorName, sizeof(colorName), "{BEBEBE}Default");
            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Default HUD Color\t%s\t{BEBEBE}Colour new players start with\n", strb, colorName);
            actions[count++] = 15; // Default HUD Color

            new primaryName[32];
            if(0 <= ServerDefaultHudShadeOne < sizeof(hudShades))
                format(primaryName, sizeof(primaryName), "{%06x}%s", hudShades[ServerDefaultHudShadeOne][shadeHex], hudShades[ServerDefaultHudShadeOne][shadeName]);
            else
                format(primaryName, sizeof(primaryName), "{BEBEBE}Interface preset");
            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Default HUD Primary\t%s\t{BEBEBE}Overrides the preset's primary color\n", strb, primaryName);
            actions[count++] = 23; // Default HUD Primary

            new secondaryName[32];
            if(0 <= ServerDefaultHudShadeTwo < sizeof(hudShades))
                format(secondaryName, sizeof(secondaryName), "{%06x}%s", hudShades[ServerDefaultHudShadeTwo][shadeHex], hudShades[ServerDefaultHudShadeTwo][shadeName]);
            else
                format(secondaryName, sizeof(secondaryName), "{BEBEBE}Interface preset");
            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Default HUD Secondary\t%s\t{BEBEBE}Overrides the preset's secondary color\n", strb, secondaryName);
            actions[count++] = 24; // Default HUD Secondary

            new defChatColorName[40];
            if(1 <= ServerDefaultChatMainColor <= sizeof(hudShades))
                format(defChatColorName, sizeof(defChatColorName), "{%06x}%s", hudShades[ServerDefaultChatMainColor - 1][shadeHex], hudShades[ServerDefaultChatMainColor - 1][shadeName]);
            else if(ServerDefaultChatMainColor & 0x10000000)
                format(defChatColorName, sizeof(defChatColorName), "{%06x}#%06x", ServerDefaultChatMainColor & 0xFFFFFF, ServerDefaultChatMainColor & 0xFFFFFF);
            else
                format(defChatColorName, sizeof(defChatColorName), "{BEBEBE}Interface preset");
            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Default Chat Color\t%s\t{BEBEBE}Main accent color new players start with\n", strb, defChatColorName);
            actions[count++] = 26; // Default Chat Color

            new defBarColorName[40];
            if(0 <= ServerDefaultHpBarColor < sizeof(hudShades))
                format(defBarColorName, sizeof(defBarColorName), "{%06x}%s", hudShades[ServerDefaultHpBarColor][shadeHex], hudShades[ServerDefaultHpBarColor][shadeName]);
            else
                format(defBarColorName, sizeof(defBarColorName), "{BEBEBE}Default (Red)");
            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Default HP Bar Color\t%s\t{BEBEBE}Health bar color new players start with\n", strb, defBarColorName);
            actions[count++] = 30; // Default HP Bar Color

            strcat(strb, "{MAIN}\xA4 Moderation\t \t \n");
            actions[count++] = 0;

            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Max. Warnings\t{BEBEBE}%d warnings\t{BEBEBE}Warnings before an automatic ban\n", strb, ServerMaxWarnings);
            actions[count++] = 16;

            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Max. Login Attempts\t{BEBEBE}%d attempts\t{BEBEBE}Wrong passwords before locking the account\n", strb, ServerMaxLoginAttempts);
            actions[count++] = 17;

            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Max. Accounts IP-GPCI\t{BEBEBE}%d accounts\t{BEBEBE}Accounts allowed per IP or serial\n", strb, ServerMaxAccountsPerIP);
            actions[count++] = 18;

            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Anti Cheat Cooldown\t{BEBEBE}%d s\t{BEBEBE}Wait before the same detection counts again\n", strb, ServerACCooldown);
            actions[count++] = 22;

            format(strb, sizeof(strb), "%s{B4B5B7}\x95 {FFFFFF}Toggle Anti Spam\t%s\t{BEBEBE}Kicks chat and command flood\n", strb, (ServerAntiSpam) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
            actions[count++] = 19;

            format(strb, sizeof(strb), "%s{B4B5B7}\x95 {FFFFFF}Toggle Anti Capslock\t%s\t{BEBEBE}Lowercases messages shouted in caps\n", strb, (ServerAntiCapslock) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
            actions[count++] = 20;

            format(strb, sizeof(strb), "%s{B4B5B7}\x95 {FFFFFF}Toggle '/Admins' Command\t%s\t{BEBEBE}Lets players list the online staff\n", strb, (ServerAdminsCmd) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
            actions[count++] = 21;

            format(strb, sizeof(strb), "%s{B4B5B7}\x95 {FFFFFF}Toggle Anti Sprint Macro\t%s\t{BEBEBE}Stops players tapping sprint faster than humanly possible\n", strb, (ACSprintMacro) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
            actions[count++] = 27; // Anti Sprint Macro

            format(strb, sizeof(strb), "%s{B4B5B7}\x95 {FFFFFF}Toggle HP & Armour Text\t%s\t{BEBEBE}Shows the health and armour numbers on the bars\n", strb, (ServerHpArmourText) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
            actions[count++] = 29; // HP & Armour Text

            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Max. Accuracy Limit\t%s\t{BEBEBE}Limit of weapon hit accuracy\n", strb, (ServerMaxAccuracy > 0) ? fmat_return("{BEBEBE}%d%%", ServerMaxAccuracy) : ("{F84F31}Disabled"));
            actions[count++] = 28; // Max Accuracy Limit
        }

        strcat(strb, "{MAIN}\xA4 Server & Security\t \t \n");
        actions[count++] = 0;

        format(strb, sizeof(strb), "%s{B4B5B7}\x95 {FFFFFF}Fake Version Kick\t%s\t{BEBEBE}Kicks clients faking their version\n", strb, (R2KickEnabled) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
        actions[count++] = 13; // Fake Version Kick

        format(strb, sizeof(strb), "%s{B4B5B7}\x95 {FFFFFF}open.mp Only Kick\t%s\t{BEBEBE}Kicks clients not using open.mp launcher\n", strb, (OMPKickEnabled) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
        actions[count++] = 25; // OMP Kick

        format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Server Lock\t%s\t{BEBEBE}Requires a password to join\n", strb, (ServerLocked) ? ("{F84F31}Locked") : ("{23C552}Unlocked"));
        actions[count++] = 5; // Lock

        if(IsPlayerAdmin(playerid))
        {
            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Server Name\t{BEBEBE}%s\t{BEBEBE}Name shown in the launcher\n", strb, ServerHostname);
            actions[count++] = 10; // Hostname

            format(strb, sizeof(strb), "%s{B4B5B7}\xBB {FFFFFF}Max Slots\t{BEBEBE}%d slots\t{BEBEBE}How many players fit at once\n", strb, GetMaxPlayers());
            actions[count++] = 11; // Max Players
        }

        format(strb, sizeof(strb), "%s{B4B5B7}\x95 {FFFFFF}Restart Server\t{F84F31}GMX\t{BEBEBE}Reloads the gamemode right now\n", strb);
        actions[count++] = 1; // Restart
    }

    strcat(strb, "{MAIN}\xA4 Chat & Spy Permissions\t \t \n");
    actions[count++] = 0;

    format(strb, sizeof(strb), "%s{B4B5B7}\x95 {FFFFFF}See PM's\t%s\t{BEBEBE}Watch private messages\n", strb, (User[playerid][acfg_pm]) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
    actions[count++] = 6;

    format(strb, sizeof(strb), "%s{B4B5B7}\x95 {FFFFFF}See Clan Chat\t%s\t{BEBEBE}Watch clan chat\n", strb, (User[playerid][acfg_clanpm]) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
    actions[count++] = 7;

    format(strb, sizeof(strb), "%s{B4B5B7}\x95 {FFFFFF}See Commands\t%s\t{BEBEBE}Watch commands players run\n", strb, (User[playerid][acfg_cmds]) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
    actions[count++] = 8;

    format(strb, sizeof(strb), "%s{B4B5B7}\x95 {FFFFFF}See Local Chat\t%s\t{BEBEBE}Watch local chat", strb, (User[playerid][acfg_local]) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
    actions[count++] = 9;

    for(new i = 0; i < count; i++)
    {
        new pvar[32];
        format(pvar, sizeof(pvar), "acfg_action_%d", i);
        SetPVarInt(playerid, pvar, actions[i]);
    }
    SetPVarInt(playerid, "acfg_count", count);

    new title[64] = "{MAIN}Admin Configuration";
    format_MainColor(playerid, strb, sizeof(strb));
    format_MainColor(playerid, title);
    format_MainColor(playerid, strb, sizeof(strb));
    Dialog_ShowCallback(playerid, using public DIALOG_AMENU<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, strb, "*", "Exit");
}

forward Stats_ShowStats(playerid, targetid);
public Stats_ShowStats(playerid, targetid)
{
    new strstts[4096], sampVERSION[24], ontime = (User[targetid][totalOnTime]+(gettime()-User[targetid][loginTime]));
    GetPlayerVersion(targetid, sampVERSION, sizeof(sampVERSION));

    if(ontime < 0) ontime = 0;

    new hour = ontime / 3600;
    new minute = (ontime % 3600) / 60;
    new second = ontime % 60;

    new dc_str[64];
    if(strequal(User[targetid][dc_username], "-1", true) || isnull(User[targetid][dc_username])) dc_str = "{F84F31}No Linked";
    else if(isnull(User[targetid][dc_discriminator]) || !strcmp(User[targetid][dc_discriminator], "0") || !strcmp(User[targetid][dc_discriminator], "0000") || !strcmp(User[targetid][dc_discriminator], "-1"))
        format(dc_str, sizeof(dc_str), "{7289DA}%s", User[targetid][dc_username]);
    else
        format(dc_str, sizeof(dc_str), "{7289DA}%s#%s", User[targetid][dc_username], User[targetid][dc_discriminator]);

    new regdate_str[32];
    if(isnull(User[targetid][user_regdate])) format(regdate_str, sizeof(regdate_str), "{BEBEBE}N/A");
    else format(regdate_str, sizeof(regdate_str), "{FFFFFF}%s", User[targetid][user_regdate]);

    new fightStyleName[16] = "Normal";
    for(new i = 0; i < sizeof(fightStyles); i++)
    {
        if(User[targetid][fightStyle] == fightStyles[i][styleid])
        {
            format(fightStyleName, sizeof(fightStyleName), "%s", fightStyles[i][fightname]);
            break;
        }
    }

    new achUnlocked = 0;
    for(new i = 0; i < ACH_COUNT; i++)
    {
        if(g_UserAchUnlocked[targetid][i]) achUnlocked++;
    }

    format(strstts, sizeof(strstts), "{MAIN}Parameter\t{MAIN}Data\n");
    strcat(strstts, "{MAIN}> Overall & Connection\t \n");
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Account ID\t{FFFFFF}#%d\n", strstts, User[targetid][user_sqlid]);
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Created At\t%s\n", strstts, regdate_str);
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Discord\t%s\n", strstts, dc_str);
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Client SA-MP\t{FFFFFF}%s\n", strstts, sampVERSION);
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Mobile Client\t%s\n", strstts, (User[targetid][isMobile]) ? ("{328E0A}Yes") : ("{F84F31}No"));
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}FPS / Ping / Loss\t{FFFFFF}%d FPS {B4B5B7}| {FFFFFF}%d ms {B4B5B7}| {FFFFFF}%.2f%%\n", strstts, User[targetid][FPS], GetPlayerPing(targetid), NetStats_PacketLossPercent(targetid));
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Skin ID\t{FFFFFF}#%d\n", strstts, GetPlayerSkin(targetid));
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Fighting Style\t{FFFFFF}%s\n", strstts, fightStyleName);
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Admin Rank\t{FFFFFF}%s\n", strstts, adminnames[(IsAdminCamouflaged(targetid) ? 0 : User[targetid][adminLevel])][ingame]);
    if(IsPlayerVip(targetid))
    {
        new tagColor[7] = "FFD700";
        new tagIdx = User[targetid][vipHeadTag];
        if(0 <= tagIdx < 10)
            format(tagColor, sizeof(tagColor), "%s", vipColorListStr[tagIdx]);
        else if(tagIdx >= 0x10000000)
            format(tagColor, sizeof(tagColor), "%06x", tagIdx & 0xFFFFFF);

        if(User[targetid][vipExpire] == 0)
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}VIP Status\t{%s}VIP Permanent\n", strstts, tagColor);
        else
        {
            new rem = User[targetid][vipExpire] - gettime();
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}VIP Status\t{%s}VIP {FFFFFF}%dd %dh left\n", strstts, tagColor, rem / 86400, (rem % 86400) / 3600);
        }
    }
    else strcat(strstts, "{FFFFFF}\x95 {B4B5B7}VIP Status\t{BEBEBE}None\n");

    if(User[targetid][customTag][0] != '\0')
    {
        if(!User[targetid][hideCustomTag])
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Custom Title\t{FF4500}%s\n", strstts, User[targetid][customTag]);
        else
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Custom Title\t{BEBEBE}%s {FF6347}[Hidden]\n", strstts, User[targetid][customTag]);
    }

    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Online Time\t{FFFFFF}%02dh %02dm %02ds\n", strstts, hour, minute, second);
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Money\t{23C552}%s\n", strstts, FormatMoney(User[targetid][playerMoney]));
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Coins\t{FFFF00}%s\n", strstts, FormatCoins(User[targetid][playerCoins]));

    if(User[targetid][warnings] > 0)
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Warnings\t{F84F31}%d {FFFFFF}/ {BEBEBE}%d\n", strstts, User[targetid][warnings], ServerMaxWarnings);
    else
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Warnings\t{FFFFFF}0 / %d\n", strstts, ServerMaxWarnings);

    if(IsPlayerMuted(targetid))
    {
        new remMute = GetPlayerMuteRemaining(targetid);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Mute Status\t{F84F31}Muted (%02dm %02ds left)\n", strstts, remMute / 60, remMute % 60);
    }
    else
    {
        strcat(strstts, "{FFFFFF}\x95 {B4B5B7}Mute Status\t{23C552}Clean\n");
    }
    strcat(strstts, "{MAIN}> Combat Records\t \n");
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Level\t{FFFFFF}%d\n", strstts, User[targetid][playerLevel]);
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Total Kills / Deaths\t{23C552}%d {FFFFFF}/ {F84F31}%d\n", strstts, User[targetid][playerKills], User[targetid][playerDeaths]);
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}K/D Ratio\t{23C552}%.2f\n", strstts, GetPlayerKDR(targetid));
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Weekly Kills / DMG\t{23C552}%d kills {B4B5B7}| {23C552}%d DMG\n", strstts, User[targetid][kills_semanais], User[targetid][dmgSemanal]);
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Best Killstreak\t{23C552}%d kills\n", strstts, User[targetid][maxStreak]);
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Best Arena Damage\t{23C552}%d DMG\n", strstts, User[targetid][maxDamage]);
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Arenas Won (MVP)\t{23C552}%d arenas\n", strstts, User[targetid][wonArenas]);
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Duel Record (W/L)\t{23C552}%d W {FFFFFF}/ {F84F31}%d L {B4B5B7}(%.2f W/L)\n", strstts, User[targetid][duelWins], User[targetid][duelLosses], ratio(User[targetid][duelWins], User[targetid][duelLosses]));
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}2v2 Record (W/L)\t{23C552}%d W {FFFFFF}/ {F84F31}%d L {B4B5B7}(%.2f W/L)\n", strstts, User[targetid][duel2v2Wins], User[targetid][duel2v2Losses], ratio(User[targetid][duel2v2Wins], User[targetid][duel2v2Losses]));
    format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Achievements\t{23C552}%d {FFFFFF}/ {BEBEBE}%d {B4B5B7}(%d%%)\n", strstts, achUnlocked, ACH_COUNT, (achUnlocked * 100) / ACH_COUNT);
    strcat(strstts, "{MAIN}> Clan Information\t \n");
    if(IsPlayerInClan(targetid))
    {
        new cid = User[targetid][clanID];
        new clanColorHex[7];
        format(clanColorHex, sizeof(clanColorHex), "%06x", ClanInfo[cid][clanColor] >>> 8);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Clan Name & Tag\t{%s}%s {FFFFFF}[%s]\n", strstts, clanColorHex, ClanInfo[cid][clanName], ClanInfo[cid][clanTag]);
        
        new roleIdx = User[targetid][clanRole];
        new roleName[CLAN_ROLE_LEN];
        if(IsValidClanRole(roleIdx))
            format(roleName, sizeof(roleName), "%s", ClanRoles[cid][roleIdx]);
        else
            format(roleName, sizeof(roleName), "Member");

        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Clan Role\t{MAIN}%s {B4B5B7}(Level %d)\n", strstts, roleName, roleIdx);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Clan Kills / Points\t{FFFFFF}%d kills {B4B5B7}| {FFFFFF}%d pts\n", strstts, ClanInfo[cid][clanKills], ClanInfo[cid][clanPoints]);
    }
    else strcat(strstts, "{FFFFFF}\x95 {B4B5B7}Clan Status\t{F84F31}No Clan\n");
    strcat(strstts, "{MAIN}> Current Session\t \n");
    if (User[targetid][locationId] != -1)
    {
        new loc = User[targetid][locationId];
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Current Arena\t{FFFFFF}Arena #%d\n", strstts, arenaInfo[loc][arenaId]);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Session K / D / DMG\t{23C552}%d K {FFFFFF}/ {F84F31}%d D {FFFFFF}/ {23C552}%.0f DMG\n", strstts, playerArenaInfo[loc][targetid][arenaKills], playerArenaInfo[loc][targetid][arenaDeaths], playerArenaInfo[loc][targetid][arenaDMG]);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Session Ratio\t{23C552}%.2f\n", strstts, ratio(playerArenaInfo[loc][targetid][arenaKills], playerArenaInfo[loc][targetid][arenaDeaths]));
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Weapon Accuracy\t{23C552}%d%%\n", strstts, floatround(GetPlayerAccuracy(targetid, loc)));
    }
    else strcat(strstts, "{FFFFFF}\x95 {B4B5B7}Current Location\t{FFFFFF}Lounge / Lobby\n");

    new title[64];
    format(title, sizeof(title), "{MAIN}Profile Stats: {FFFFFF}%s", getPlayerName(targetid));
    format_MainColor(playerid, title);
    format_MainColor(playerid, strstts, sizeof(strstts));
    Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, strstts, "Close", "");
    return 1;
}

stock CountUnlockedAchFromStr(const data[])
{
    if(isnull(data)) return 0;
    new count = 0, pos = 0, len = strlen(data), entry[64];
    while(pos < len)
    {
        new epos = pos;
        while(epos < len && data[epos] != ';') epos++;
        strmid(entry, data, pos, epos, sizeof(entry));
        pos = epos + 1;
        if(!entry[0]) continue;

        new tpos = 0, elen = strlen(entry), field = 0;
        new achid = -1, date[ACH_DATE_LEN];
        date[0] = '\0';

        while(tpos <= elen && field < 3)
        {
            new tend = tpos;
            while(tend < elen && entry[tend] != '|') tend++;
            new token[32];
            strmid(token, entry, tpos, tend, sizeof(token));
            tpos = tend + 1;

            switch(field++)
            {
                case 0: achid = strval(token);
                case 2: strcopy(date, token, sizeof(date));
            }
        }
        if(0 <= achid < ACH_COUNT && date[0] != '\0') count++;
    }
    return count;
}

forward LookupOfflineStats(playerid, const targetName[]);
public LookupOfflineStats(playerid, const targetName[])
{
    if(!IsPlayerLogged(playerid)) return 0;

    new query[512];
    mysql_format(dbHandle, query, sizeof(query),
        "SELECT a.*, c.clan_name, c.clan_tag, c.clan_color, c.clan_zone_points, c.clan_kills \
        FROM `accounts` a \
        LEFT JOIN `clans` c ON a.clanid = c.clan_id \
        WHERE a.`username` = '%e' LIMIT 1", targetName);

    inline OnOfflineStatsQuery()
    {
        if(!IsPlayerLogged(playerid)) return 1;

        if(!cache_num_rows())
        {
            va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Player account '{MAIN}%s{FFFFFF}' was not found.", targetName);
            return 1;
        }

        new strstts[4096], off_user[MAX_PLAYER_NAME + 1], off_regdate[32], off_dc_user[100], off_dc_disc[100], off_dc_str[64];
        new off_sqlid, off_level, off_money, off_coins, off_kills, off_kills_sem, off_max_dmg, off_dmg_sem, off_streak, off_deaths;
        new off_admin, off_clanid, off_clanrole, off_vip_lvl, off_vip_exp, off_vip_tag, off_fstyle, off_skin;
        new off_lastlogin, off_won_arenas, off_duel_w, off_duel_l, off_duel2_w, off_duel2_l, off_warns, off_custom_tag[33];
        new bool:off_hide_tag, off_unmute_ts, off_played_time, off_achData[ACH_SERIAL_BUF_SIZE];
        new off_clan_name[MAX_CLAN_NAME + 1], off_clan_tag[MAX_CLAN_TAG + 1], off_clan_color, off_clan_pts, off_clan_kills;

        cache_get_value_name_int(0, "id", off_sqlid);
        cache_get_value_name(0, "username", off_user, sizeof(off_user));
        cache_get_value_name(0, "created_at", off_regdate, sizeof(off_regdate));
        if(isnull(off_regdate)) format(off_regdate, sizeof(off_regdate), "{BEBEBE}N/A");
        else format(off_regdate, sizeof(off_regdate), "{FFFFFF}%s", off_regdate);

        cache_get_value_name(0, "dc_username", off_dc_user, sizeof(off_dc_user));
        cache_get_value_name(0, "dc_discriminator", off_dc_disc, sizeof(off_dc_disc));
        if(strequal(off_dc_user, "-1", true) || isnull(off_dc_user)) format(off_dc_str, sizeof(off_dc_str), "{F84F31}No Linked");
        else if(isnull(off_dc_disc) || !strcmp(off_dc_disc, "0") || !strcmp(off_dc_disc, "0000") || !strcmp(off_dc_disc, "-1"))
            format(off_dc_str, sizeof(off_dc_str), "{7289DA}%s", off_dc_user);
        else
            format(off_dc_str, sizeof(off_dc_str), "{7289DA}%s#%s", off_dc_user, off_dc_disc);

        cache_get_value_name_int(0, "level", off_level);
        cache_get_value_name_int(0, "player_money", off_money);
        cache_get_value_name_int(0, "coins", off_coins);
        cache_get_value_name_int(0, "kills", off_kills);
        cache_get_value_name_int(0, "kills_semanais", off_kills_sem);
        cache_get_value_name_int(0, "max_damage", off_max_dmg);
        cache_get_value_name_int(0, "damage_semanal", off_dmg_sem);
        cache_get_value_name_int(0, "max_streak", off_streak);
        cache_get_value_name_int(0, "deaths", off_deaths);
        cache_get_value_name_int(0, "adminlevel", off_admin);
        cache_get_value_name_int(0, "clanid", off_clanid);
        cache_get_value_name_int(0, "clanrole", off_clanrole);
        cache_get_value_name_int(0, "vip_level", off_vip_lvl);
        cache_get_value_name_int(0, "vip_expire", off_vip_exp);
        cache_get_value_name_int(0, "vip_head_tag", off_vip_tag);
        cache_get_value_name_int(0, "fighting_style", off_fstyle);
        cache_get_value_name_int(0, "skin_id", off_skin);
        cache_get_value_name_int(0, "last_login", off_lastlogin);
        cache_get_value_name_int(0, "won_arenas", off_won_arenas);
        cache_get_value_name_int(0, "duel_wins", off_duel_w);
        cache_get_value_name_int(0, "duel_losses", off_duel_l);
        cache_get_value_name_int(0, "duel2v2_wins", off_duel2_w);
        cache_get_value_name_int(0, "duel2v2_losses", off_duel2_l);
        cache_get_value_name_int(0, "warnings", off_warns);
        cache_get_value_name(0, "custom_tag", off_custom_tag, sizeof(off_custom_tag));
        cache_get_value_name_bool(0, "hide_custom_tag", off_hide_tag);
        cache_get_value_name_int(0, "unmute_timestamp", off_unmute_ts);
        cache_get_value_name_int(0, "played_time", off_played_time);
        cache_get_value_name(0, "achievements_data", off_achData, sizeof(off_achData));

        if(off_clanid != -1)
        {
            cache_get_value_name(0, "clan_name", off_clan_name, sizeof(off_clan_name));
            cache_get_value_name(0, "clan_tag", off_clan_tag, sizeof(off_clan_tag));
            cache_get_value_name_int(0, "clan_color", off_clan_color);
            cache_get_value_name_int(0, "clan_zone_points", off_clan_pts);
            cache_get_value_name_int(0, "clan_kills", off_clan_kills);
        }

        if(off_played_time < 0) off_played_time = 0;
        new hour = off_played_time / 3600;
        new minute = (off_played_time % 3600) / 60;
        new second = off_played_time % 60;

        new fightStyleName[16] = "Normal";
        for(new i = 0; i < sizeof(fightStyles); i++)
        {
            if(off_fstyle == _:fightStyles[i][styleid])
            {
                format(fightStyleName, sizeof(fightStyleName), "%s", fightStyles[i][fightname]);
                break;
            }
        }

        new achUnlocked = CountUnlockedAchFromStr(off_achData);

        format(strstts, sizeof(strstts), "{MAIN}Parameter\t{MAIN}Data\n");
        strcat(strstts, "{MAIN}> Overall & Status (Offline)\t \n");
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Account ID\t{FFFFFF}#%d\n", strstts, off_sqlid);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Status\t{F84F31}Offline\n", strstts);
        if(off_lastlogin > 0)
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Last Seen\t{FFFFFF}%s\n", strstts, returnDate(off_lastlogin, 1));
        else
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Last Seen\t{BEBEBE}Never\n", strstts);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Created At\t%s\n", strstts, off_regdate);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Discord\t%s\n", strstts, off_dc_str);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Skin ID\t{FFFFFF}#%d\n", strstts, off_skin);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Fighting Style\t{FFFFFF}%s\n", strstts, fightStyleName);
        if(0 <= off_admin < sizeof(adminnames))
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Admin Rank\t{FFFFFF}%s\n", strstts, adminnames[off_admin][ingame]);
        else
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Admin Rank\t{FFFFFF}Player\n", strstts);

        if(off_vip_lvl > 0 && (off_vip_exp == 0 || off_vip_exp > gettime()))
        {
            new tagColor[7] = "FFD700";
            if(0 <= off_vip_tag < 10)
                format(tagColor, sizeof(tagColor), "%s", vipColorListStr[off_vip_tag]);
            else if(off_vip_tag >= 0x10000000)
                format(tagColor, sizeof(tagColor), "%06x", off_vip_tag & 0xFFFFFF);

            if(off_vip_exp == 0)
                format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}VIP Status\t{%s}VIP Permanent\n", strstts, tagColor);
            else
            {
                new rem = off_vip_exp - gettime();
                format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}VIP Status\t{%s}VIP {FFFFFF}%dd %dh left\n", strstts, tagColor, rem / 86400, (rem % 86400) / 3600);
            }
        }
        else strcat(strstts, "{FFFFFF}\x95 {B4B5B7}VIP Status\t{BEBEBE}None\n");

        if(off_custom_tag[0] != '\0')
        {
            if(!off_hide_tag)
                format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Custom Title\t{FF4500}%s\n", strstts, off_custom_tag);
            else
                format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Custom Title\t{BEBEBE}%s {FF6347}[Hidden]\n", strstts, off_custom_tag);
        }

        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Online Time\t{FFFFFF}%02dh %02dm %02ds\n", strstts, hour, minute, second);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Money\t{23C552}%s\n", strstts, FormatMoney(off_money));
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Coins\t{FFFF00}%s\n", strstts, FormatCoins(off_coins));

        if(off_warns > 0)
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Warnings\t{F84F31}%d {FFFFFF}/ {BEBEBE}%d\n", strstts, off_warns, ServerMaxWarnings);
        else
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Warnings\t{FFFFFF}0 / %d\n", strstts, ServerMaxWarnings);

        if(off_unmute_ts > gettime())
        {
            new remMute = off_unmute_ts - gettime();
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Mute Status\t{F84F31}Muted (%02dm %02ds left)\n", strstts, remMute / 60, remMute % 60);
        }
        else
        {
            strcat(strstts, "{FFFFFF}\x95 {B4B5B7}Mute Status\t{23C552}Clean\n");
        }
        strcat(strstts, "{MAIN}> Combat Records\t \n");
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Level\t{FFFFFF}%d\n", strstts, off_level);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Total Kills / Deaths\t{23C552}%d {FFFFFF}/ {F84F31}%d\n", strstts, off_kills, off_deaths);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}K/D Ratio\t{23C552}%.2f\n", strstts, ratio(off_kills, off_deaths));
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Weekly Kills / DMG\t{23C552}%d kills {B4B5B7}| {23C552}%d DMG\n", strstts, off_kills_sem, off_dmg_sem);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Best Killstreak\t{23C552}%d kills\n", strstts, off_streak);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Best Arena Damage\t{23C552}%d DMG\n", strstts, off_max_dmg);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Arenas Won (MVP)\t{23C552}%d arenas\n", strstts, off_won_arenas);
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Duel Record (W/L)\t{23C552}%d W {FFFFFF}/ {F84F31}%d L {B4B5B7}(%.2f W/L)\n", strstts, off_duel_w, off_duel_l, ratio(off_duel_w, off_duel_l));
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}2v2 Record (W/L)\t{23C552}%d W {FFFFFF}/ {F84F31}%d L {B4B5B7}(%.2f W/L)\n", strstts, off_duel2_w, off_duel2_l, ratio(off_duel2_w, off_duel2_l));
        format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Achievements\t{23C552}%d {FFFFFF}/ {BEBEBE}%d {B4B5B7}(%d%%)\n", strstts, achUnlocked, ACH_COUNT, (achUnlocked * 100) / ACH_COUNT);
        strcat(strstts, "{MAIN}> Clan Information\t \n");
        if(off_clanid != -1 && !isnull(off_clan_name))
        {
            new clanColorHex[7];
            format(clanColorHex, sizeof(clanColorHex), "%06x", off_clan_color >>> 8);
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Clan Name & Tag\t{%s}%s {FFFFFF}[%s]\n", strstts, clanColorHex, off_clan_name, off_clan_tag);
            
            new roleName[CLAN_ROLE_LEN];
            if(IsValidClanRole(off_clanrole) && 0 <= off_clanid < MAX_CLANS)
                format(roleName, sizeof(roleName), "%s", ClanRoles[off_clanid][off_clanrole]);
            else
                format(roleName, sizeof(roleName), "Member");

            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Clan Role\t{MAIN}%s {B4B5B7}(Level %d)\n", strstts, roleName, off_clanrole);
            format(strstts, sizeof(strstts), "%s{FFFFFF}\x95 {B4B5B7}Clan Kills / Points\t{FFFFFF}%d kills {B4B5B7}| {FFFFFF}%d pts\n", strstts, off_clan_kills, off_clan_pts);
        }
        else strcat(strstts, "{FFFFFF}\x95 {B4B5B7}Clan Status\t{F84F31}No Clan\n");

        new title[64];
        format(title, sizeof(title), "{MAIN}Profile Stats: {FFFFFF}%s {BEBEBE}(Offline)", off_user);
        format_MainColor(playerid, strstts, sizeof(strstts));

        Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, strstts, "Close", "");
        return 1;
    }
    MySQL_TQueryInline(dbHandle, using inline OnOfflineStatsQuery, query);
    return 1;
}

forward Stats_ShowPlayerClanInfo(playerid, targetid);
public Stats_ShowPlayerClanInfo(playerid, targetid)
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

    format(infoStr, sizeof(infoStr), "{MAIN}Parameter\t{MAIN}Value\n");
    strcat(infoStr, fmat_return("{FFFFFF}\x95 {B4B5B7}Clan Name\t{%s}%s\n", clanColorHex, ClanInfo[clanid][clanName]));
    strcat(infoStr, fmat_return("{FFFFFF}\x95 {B4B5B7}Clan ID\t{FFFFFF}#%d\n", ClanInfo[clanid][clanID]));
    strcat(infoStr, fmat_return("{FFFFFF}\x95 {B4B5B7}Clan Tag\t{FFFFFF}%s\n", ClanInfo[clanid][clanTag]));
    strcat(infoStr, fmat_return("{FFFFFF}\x95 {B4B5B7}Created At\t{FFFFFF}%s\n", createdAt));
    strcat(infoStr, fmat_return("{FFFFFF}\x95 {B4B5B7}Total Members\t{FFFFFF}%d / %d\n", ClanMembersCount[clanid], MAX_MEMBERS));
    strcat(infoStr, fmat_return("{FFFFFF}\x95 {B4B5B7}Online Members\t{23C552}%d\n", onlineMembers));
    strcat(infoStr, fmat_return("{FFFFFF}\x95 {B4B5B7}Territories / Points\t{FFFFFF}%d / %d", ownedOutposts, ClanInfo[clanid][clanPoints]));

    new title[64];
    format(title, sizeof(title), "{MAIN}Clan Info: {FFFFFF}%s", ClanInfo[clanid][clanName]);
    format_MainColor(playerid, infoStr, sizeof(infoStr));

    format_MainColor(playerid, title);
    format_MainColor(playerid, infoStr, sizeof(infoStr));
    Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, infoStr, "Close", "");
    return 1;
}



forward Stats_ShowBanDialog(playerid, const adminname[], const reason[], const banDate[], const unbanDate[]);
public Stats_ShowBanDialog(playerid, const adminname[], const reason[], const banDate[], const unbanDate[])
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
        
        format(strban, sizeof(strban), "%s{FF644D}GPCI: %s\n", strban, (gpci_ban_count > 0) ? ("{F84F31}Banned") : ("{23C552}Clean"));
        format(strban, sizeof(strban), "%s{FF644D}ISP: %s\n\n", strban, (isp_ban_count > 0) ? ("{F84F31}Banned") : ("{23C552}Clean"));

        strcat(strban, fmat_return("{FFFFFF}Site({4282b3}%s {FFFFFF})", ServerHostname));

        SendClientMessage(playerid, 0xF3230EFF, "{MAIN}[BAN] {FFFFFF}This account is banned!");
        Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "{E63114}BANNED!!!", strban, "Close", "");
    }
    MySQL_TQueryInline(dbHandle, using inline OnBanDetailsCheck, queryGlobal);
    return 1;
}

forward Stats_ChangeTime(playerid, timeid);
public Stats_ChangeTime(playerid, timeid)
{
	if(timeid < 0 || timeid > 23) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid world time. Use a value between 0 and 23.");

	SetPlayerTime(playerid, timeid, 0);
	va_SendClientMessage(playerid, 0xebebebFF, TIME_INFO, timeid);
    User[playerid][time] = timeid;

	return true;
}

forward Stats_ChangeWeather(playerid, weatherid);
public Stats_ChangeWeather(playerid, weatherid)
{
	if(weatherid < 0 || weatherid > 45) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid weather ID. Use a value between 0 and 45.");

	SetPlayerWeather(playerid, weatherid);
    va_SendClientMessage(playerid, 0xebebebFF, WEATHER_INFO, weatherid);
    User[playerid][weather] = weatherid;

    return true;
}

forward Stats_ChangeSkin(playerid, skinid);
public Stats_ChangeSkin(playerid, skinid)
{
	if(skinid < 2 || skinid > 311) return SendClientMessage(playerid,-1, "%s", SKIN_HELP2);

    if(skinid == 74 || skinid == 86 || skinid == 271 || skinid == 270)
    {
    	va_SendClientMessage(playerid, COLOR_ABACATE, "{MAIN}[ERROR] {FFFFFF}Skin ID %d isn't valid.", skinid);
    	skinid = GetPlayerSkin(playerid);
    }

    SetPlayerSkin(playerid, skinid);
    User[playerid][playerSkin] = skinid;

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
    {
        if (User[playerid][fightStyle] == fightStyles[i][styleid])
            strcat(fightstr, fmat_return("{MAIN}> {FFFFFF}%s\n", fightStyles[i][fightname]), sizeof(fightstr));
        else
            strcat(fightstr, fmat_return("{MAIN}\xA4 {FFFFFF}%s\n", fightStyles[i][fightname]), sizeof(fightstr));
    }

    new title[64] = "{MAIN}\xA4 {FFFFFF}Select Fighting Style";

    format_MainColor(playerid, title);
    format_MainColor(playerid, fightstr);
    Dialog_ShowCallback(playerid, using public FIGHT<iiiis>, DIALOG_STYLE_LIST, title, fightstr, "Select", "Back");
}

forward Stats_showMapListDialog(playerid);
public Stats_showMapListDialog(playerid)
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
        new ownerClan = ZoneOwnerClan[arenaiddd];
        new arenaNameStr[96];

        if(IsValidClanID(ownerClan))
        {
            format(arenaNameStr, sizeof(arenaNameStr), "{%06x}%s", ClanInfo[ownerClan][clanColor] >>> 8, arenaData[arenaiddd][arenaName]);
        }
        else
        {
            format(arenaNameStr, sizeof(arenaNameStr), "{FFFFFF}%s", arenaData[arenaiddd][arenaName]);
        }

        strcat(dialogStr,
            sprintf("%d\t%s\t%0.1fm\t%s\n", 
                arenaiddd, 
                arenaNameStr, 
                arenaarea, 
                (catID >= 0 && catID < sizeof(categoryNames)) ? categoryNames[catID] : "N/A"
            )
        );
    }

    if((currentPage + 1) * 16 < MAX_ZONES)
    {
        strcat(dialogStr, "{00ff00}>>>\tNext Page\t \t \n");
    }

    Dialog_ShowCallback(playerid, using public DIALOG_MAPLIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "SELECT MAP", dialogStr, "Select", (currentPage > 0) ? ("<") : ("Close"));
    return 1;
}

ShowGunMenu(playerid)
{
	if(IsPlayerSpectating(playerid))
		return SendClientMessage(playerid, -1, "%s", IN_SPEC_MODE);

    new bool:isLoungeOrAfk = (IsPlayerAFK(playerid) || IsPlayerInLounge(playerid));
    if(!isLoungeOrAfk)
    {
        new Float:Armour;
        GetPlayerArmour(playerid, Armour);
        if(Armour < 20.0)
            return SendClientMessage(playerid, -1, "%s", CANNOT_DO_THIS);
    }

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
    
    DCC_SetEmbedFooter(v_embed, getDateStrTD());

	new tmpStr[500];
	format(tmpStr, sizeof(tmpStr), "%s", getPlayerName(playerid));

	DCC_AddEmbedField(v_embed, "Player", tmpStr, true);
	DCC_AddEmbedField(v_embed, "Code", code, true);
	DCC_SetEmbedImage(v_embed, "https://i.imgur.com/hib1Fwk.png");
    DCC_SendChannelEmbedMessage(PMPlayer, v_embed);

    SendClientMessage(playerid, 0xFFFFFFFF, "{MAIN}[DISCORD] {FFFFFF}Validation code sent to your Discord DM.");
    return 1;
}