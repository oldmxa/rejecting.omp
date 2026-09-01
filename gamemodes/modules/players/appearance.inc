
@cmd() piss(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION:68);
	return true;
}

@cmd() dance(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

	new danceid;
	if(sscanf(params, "d", danceid))
		return SendClientMessage(playerid, -1, "%s", DANCE_HELP);
	if(danceid < 1 || danceid > 4)
		return SendClientMessage(playerid, -1, "%s", DANCE_HELP);

	switch(danceid)
	{
		case 1: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE1);
		case 2: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE2);
		case 3: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE3);
		case 4: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE4);
	}
	return true;
}

@cmd() stopanim(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

	if(GetPlayerSpecialAction(playerid) == 	SPECIAL_ACTION_NONE)
		return true;
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	return true;
}


@cmd(.alts = {"ss", "skin"}) changeskin(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(IsNull(params)) return ShowSkinDialog(playerid, APPEARANCE_SRC_NONE);
    if(!IsNumeric(params)) return SendClientMessage(playerid,-1, "%s", IS_NOT_NUMERIC);

    ChangeSkin(playerid, strval(params));
    return true;
}

@cmd(.alts = {"fightstyle"}) fight(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    ChangeFight(playerid);
    return true;
}

@cmd() flash(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    new option[4], optionid;
    if(sscanf(params, "s[4]", option))
        return SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/flash [on/off]");

    if(!strcmp(option, "on", false, strlen(option)))
        optionid = 1;
    else if (!strcmp(option, "off", false, strlen(option)))
        optionid = 2;

    switch(optionid)
    {
        case 1:
        {
            if(piscaTimer[playerid])
            {
                KillTimer(piscaTimer[playerid]);
                piscaTimer[playerid] = 0;
            }
            SetPlayerColor(playerid, (randomcolor[random(sizeof(randomcolor))] & 0xFFFFFF00) | 0x65);
            piscaTimer[playerid] = SetTimerEx("updateColor", 1000, true, "i", playerid);
            User[playerid][isPiscando] = true;
            SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Flashing colors enabled.");
        }
        case 2:
        {
            if(piscaTimer[playerid])
            {
                KillTimer(piscaTimer[playerid]);
                piscaTimer[playerid] = 0;
            }
            SetClanColor(playerid);
            User[playerid][isPiscando] = false;
            SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Flashing colors disabled.");
        }
    }

    return true;
}


forward DIALOG_HELP_PERSONALIZATION(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HELP_PERSONALIZATION(playerid, dialogid, response, listitem, string:inputtext[])
{
    new cmd[32];
    if(!response)
    {
        return ShowMyAccountMenu(playerid);
    }

    switch(listitem)
    {
        case 0:
        {
            strcopy(cmd, "/changenick");
            Command_ReProcess(playerid, cmd, 0);
        }
        case 1:
        {
            SetPVarInt(playerid, "changepass_source", 0);
            new passBody[192];
            format(passBody, sizeof(passBody),
                "{FFFFFF}Enter your new password below:\n\n{B4B5B7}Only letters and numbers, between %d and %d characters.",
                MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH);

            Dialog_ShowCallback(playerid, using public DIALOG_CHANGE_PASSWORD<iiiis>, DIALOG_STYLE_PASSWORD, "Change Password", passBody, "Change", "Cancel");
        }
        case 2:
        {
            User[playerid][blockPM] = !User[playerid][blockPM];
            if(User[playerid][blockPM])
                SendClientMessage(playerid, -1, "{MAIN}[PM] {FFFFFF}You turned off incoming messages.");
            else
                SendClientMessage(playerid, -1, "{MAIN}[PM] {FFFFFF}You turned on incoming messages.");
            
            ShowPersonalizationMenu(playerid);
        }
        case 3:
        {
            User[playerid][showConnectMsg] = !User[playerid][showConnectMsg];
            ShowPersonalizationMenu(playerid);
        }
        case 4:
        {
            User[playerid][hideGlobalMsg] = !User[playerid][hideGlobalMsg];
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
        case 5:
        {
            User[playerid][hideWelcomeMsg] = !User[playerid][hideWelcomeMsg];
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
        case 6: ShowPersonalTimeDialog(playerid);
        case 7: ShowPersonalWeatherDialog(playerid);
        case 8: {
            User[playerid][showPlayerHud] = !User[playerid][showPlayerHud];
            if(User[playerid][showPlayerHud]) {
                hidePlayerTextdraw(playerid);
                showPlayerTextDraw(playerid);
            } else {
                hidePlayerTextdraw(playerid);
            }
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
        case 9: {
            User[playerid][showClanHud] = !User[playerid][showClanHud];
            if(User[playerid][showPlayerHud]) {
                updatePlayerHud(playerid);
            }
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
        case 10: ShowDamageStyleDialog(playerid, APPEARANCE_SRC_PERSONAL);
        case 11: ShowHudColorDialog(playerid, APPEARANCE_SRC_PERSONAL);
        case 12: ShowHudShadeDialog(playerid, APPEARANCE_SRC_PERSONAL, false);
        case 13: ShowHudShadeDialog(playerid, APPEARANCE_SRC_PERSONAL, true);
        case 14: ShowChatColorDialog(playerid, APPEARANCE_SRC_PERSONAL);
        case 15: ShowSkinDialog(playerid, APPEARANCE_SRC_PERSONAL);
        case 16: { strcopy(cmd, "/fight"); Command_ReProcess(playerid, cmd, 0); }
        case 17: {
            if(!IsPlayerInLounge(playerid))
            {
                SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}You can only configure weapon loadout via menu while in the Lounge.");
                ShowPersonalizationMenu(playerid);
                return 1;
            }
            SetPVarInt(playerid, "gunmenu_from_personalization", 1);
            gunMenu(playerid);
        }
        case 18: { ShowPlayerHudTypesList(playerid); }
        case 19: {
            User[playerid][show3DDeathTag] = !User[playerid][show3DDeathTag];
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
        case 20: {
            User[playerid][showRandomDeathTags] = !User[playerid][showRandomDeathTags];
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
        case 21: {
            User[playerid][showHeadshot] = !User[playerid][showHeadshot];
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
        case 22: { ShowHitSoundDialog(playerid); }
        case 23: { ShowKillSoundDialog(playerid); }
        case 24: { ShowPlayerLifeTypesList(playerid); }
        case 25: ShowDamageFeedMainMenu(playerid, APPEARANCE_SRC_PERSONAL);
        case 26: {
            if(!ServerHpArmourText)
            {
                SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}The HP and armour text is disabled server-wide.");
                ShowPersonalizationMenu(playerid);
                return 1;
            }

            User[playerid][showHpArmour] = !User[playerid][showHpArmour];
            RefreshHpArmourText(playerid);
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
    }
    return 1;
}

forward ShowPersonalTimeDlg(playerid, source);
public ShowPersonalTimeDlg(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new title[64] = "{MAIN}Personalization - World Time";
    format_MainColor(playerid, title);

    new body[256];
    format(body, sizeof(body),
        "{FFFFFF}Enter the desired world time below:\n\n\
        {B4B5B7}Hours: 0 - 23 (Current: {FFFFFF}%02d:00{B4B5B7})",
        User[playerid][time]);

    Dialog_ShowCallback(playerid, using public DIALOG_PERSONAL_TIME<iiiis>, DIALOG_STYLE_INPUT, title, body, "Apply", "Back");
    return 1;
}

forward DIALOG_PERSONAL_TIME(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_PERSONAL_TIME(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        return ReturnToAppearanceSource(playerid);
    }

    if(!IsPlayerLogged(playerid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(isnull(inputtext) || !IsNumeric(inputtext))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Please enter a valid numeric hour (0-23).");
        return ShowPersonalTimeDialog(playerid, GetAppearanceSource(playerid));
    }

    new t = strval(inputtext);
    if(t < 0 || t > 23)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}World time must be between 0 and 23.");
        return ShowPersonalTimeDialog(playerid, GetAppearanceSource(playerid));
    }

    ChangeTime(playerid, t);
    ReturnToAppearanceSource(playerid);
    return 1;
}

forward ShowPersonalWeatherDlg(playerid, source);
public ShowPersonalWeatherDlg(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new title[64] = "{MAIN}Personalization - Weather";
    format_MainColor(playerid, title);

    new body[256];
    format(body, sizeof(body),
        "{FFFFFF}Enter the desired weather ID below:\n\n\
        {B4B5B7}Weather ID: 0 - 45 (Current ID: {FFFFFF}%d{B4B5B7})",
        User[playerid][weather]);

    Dialog_ShowCallback(playerid, using public DIALOG_PERSONAL_WEATHER<iiiis>, DIALOG_STYLE_INPUT, title, body, "Apply", "Back");
    return 1;
}

forward DIALOG_PERSONAL_WEATHER(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_PERSONAL_WEATHER(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        return ReturnToAppearanceSource(playerid);
    }

    if(!IsPlayerLogged(playerid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(isnull(inputtext) || !IsNumeric(inputtext))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Please enter a valid numeric weather ID (0-45).");
        return ShowPersonalWeatherDialog(playerid, GetAppearanceSource(playerid));
    }

    new w = strval(inputtext);
    if(w < 0 || w > 45)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Weather ID must be between 0 and 45.");
        return ShowPersonalWeatherDialog(playerid, GetAppearanceSource(playerid));
    }

    ChangeWeather(playerid, w);
    ReturnToAppearanceSource(playerid);
    return 1;
}

stock GetSoundName(sound_id)
{
    new name[32] = "Custom/Unknown";
    for(new i = 0; i < sizeof(g_SoundList); i++)
    {
        if(g_SoundList[i][soundID] == sound_id)
        {
            format(name, sizeof(name), "%s", g_SoundList[i][soundName]);
            break;
        }
    }
    return name;
}

stock ShowHitSoundDialog(playerid, source = APPEARANCE_SRC_PERSONAL)
{
    SetAppearanceSource(playerid, source);

    new title[64] = "{MAIN}Personalization - Hit Sound";

    format_MainColor(playerid, title);
    new str[3500] = "{MAIN}ID\t{MAIN}Sound Name\t{MAIN}Preview / Status\n";
    for(new i = 0; i < sizeof(g_SoundList); i++)
    {
        new bool:isSelected = (User[playerid][hitSoundID] == g_SoundList[i][soundID]);
        format(str, sizeof(str), "%s{FFFFFF}%d\t%s{FFFFFF}%s\t%s\n",
            str,
            g_SoundList[i][soundID],
            STYLE_MARK(isSelected),
            g_SoundList[i][soundName],
            isSelected ? ("{23C552}Active (Playing)") : ("{BEBEBE}[Click to Test]")
        );
    }

    format_MainColor(playerid, str);
    Dialog_ShowCallback(playerid, using public DIALOG_HIT_SOUND_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, str, "Test / Select", "Close");
    return 1;
}

forward DIALOG_HIT_SOUND_LIST(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HIT_SOUND_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
        return ReturnToAppearanceSource(playerid);

    if(listitem >= 0 && listitem < sizeof(g_SoundList))
    {
        User[playerid][hitSoundID] = g_SoundList[listitem][soundID];
        if(User[playerid][hitSoundID] != 0)
        {
            PlayerPlaySound(playerid, User[playerid][hitSoundID], 0.0, 0.0, 0.0);
        }
        ShowHitSoundDialog(playerid, GetPVarInt(playerid, "appearance_source"));
    }
    return 1;
}

stock ShowKillSoundDialog(playerid, source = APPEARANCE_SRC_PERSONAL)
{
    SetAppearanceSource(playerid, source);

    new title[64] = "{MAIN}Personalization - Kill Sound";

    new str[3500] = "{MAIN}ID\t{MAIN}Sound Name\t{MAIN}Preview / Status\n";
    for(new i = 0; i < sizeof(g_SoundList); i++)
    {
        new bool:isSelected = (User[playerid][killSoundID] == g_SoundList[i][soundID]);
        format(str, sizeof(str), "%s{FFFFFF}%d\t%s{FFFFFF}%s\t%s\n",
            str,
            g_SoundList[i][soundID],
            STYLE_MARK(isSelected),
            g_SoundList[i][soundName],
            isSelected ? ("{23C552}Active (Playing)") : ("{BEBEBE}[Click to Test]")
        );
    }

    format_MainColor(playerid, str);
    Dialog_ShowCallback(playerid, using public DIALOG_KILL_SOUND_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, str, "Test / Select", "Close");
    return 1;
}

forward DIALOG_KILL_SOUND_LIST(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_KILL_SOUND_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
        return ReturnToAppearanceSource(playerid);

    if(listitem >= 0 && listitem < sizeof(g_SoundList))
    {
        User[playerid][killSoundID] = g_SoundList[listitem][soundID];
        if(User[playerid][killSoundID] != 0)
        {
            PlayerPlaySound(playerid, User[playerid][killSoundID], 0.0, 0.0, 0.0);
        }
        ShowKillSoundDialog(playerid, GetPVarInt(playerid, "appearance_source"));
    }
    return 1;
}

@cmd(.alts = {"hudstyle", "hudtype"}) customhud(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    ShowPlayerHudTypesList(playerid);
    return 1;
}

@cmd(.alts = {"maincolor", "corchat", "chatcor"}) chatcolor(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    ShowChatColorDialog(playerid, APPEARANCE_SRC_NONE);
    return 1;
}

@cmd(.alts = {"damageinformer", "informerstyle", "hitmarker"}) dmginformer(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    ShowDamageStyleDialog(playerid, APPEARANCE_SRC_NONE);
    return 1;
}

@cmd(.alts = {"damagefeed", "feedstyle"}) dmgfeed(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    ShowDamageFeedMainMenu(playerid, APPEARANCE_SRC_NONE);
    return 1;
}

@cmd(.alts = {"feedcolor", "corfeed", "feedcol"}) dmgfeedcolor(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    ShowDamageFeedColorMenu(playerid, APPEARANCE_SRC_NONE);
    return 1;
}

@cmd(.alts = {"hitmarkcolor", "hitcolor", "dmginformercolor", "cordano"}) hitmarkcol(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    ShowHitmarkColorDialog(playerid, APPEARANCE_SRC_NONE);
    return 1;
}

