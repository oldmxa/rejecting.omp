
@cmd() piss(playerid, params[], help)
{
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION:68);
	return true;
}

@cmd() dance(playerid, params[], help)
{
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
	if(GetPlayerSpecialAction(playerid) == 	SPECIAL_ACTION_NONE)
		return true;
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	return true;
}


@cmd(.alts = {"ss", "skin"}) changeskin(playerid, params[], help)
{
    if(IsNull(params)) return ShowSkinDialog(playerid, APPEARANCE_SRC_NONE);
    if(!IsNumeric(params)) return SendClientMessage(playerid,-1, "%s", IS_NOT_NUMERIC);

    ChangeSkin(playerid, strval(params));
    return true;
}

@cmd(.alts = {"fightstyle"}) fight(playerid, params[], help)
{
    ChangeFight(playerid);
    return true;
}

@cmd() flash(playerid, params[], help)
{
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
            SetPlayerColor(playerid, (randomcolor[random(sizeof(randomcolor))] & 0xFFFFFF00) | 0x65);
            piscaTimer[playerid] = SetTimerEx("updateColor", 1000, true, "i", playerid);
            User[playerid][isPiscando] = true;
            SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Flashing colors enabled.");
        }
        case 2:
        {
            KillTimer(piscaTimer[playerid]);
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
        case 0: { strcopy(cmd, "/time"); Command_ReProcess(playerid, cmd, 0); }
        case 1: { strcopy(cmd, "/weather"); Command_ReProcess(playerid, cmd, 0); }
        case 2: {
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
        case 3: {
            User[playerid][showClanHud] = !User[playerid][showClanHud];
            if(User[playerid][showPlayerHud]) {
                updatePlayerHud(playerid);
            }
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
        case 4: ShowDamageStyleDialog(playerid, APPEARANCE_SRC_PERSONAL);
        case 5: ShowHitmarkColorDialog(playerid, APPEARANCE_SRC_PERSONAL);
        case 6: ShowHudColorDialog(playerid, APPEARANCE_SRC_PERSONAL);
        case 7: ShowHudShadeDialog(playerid, APPEARANCE_SRC_PERSONAL, false);
        case 8: ShowHudShadeDialog(playerid, APPEARANCE_SRC_PERSONAL, true);
        case 9: ShowChatColorDialog(playerid, APPEARANCE_SRC_PERSONAL);
        case 10: ShowSkinDialog(playerid, APPEARANCE_SRC_PERSONAL);
        case 11: { strcopy(cmd, "/fight"); Command_ReProcess(playerid, cmd, 0); }
        case 12: { strcopy(cmd, "/gunmenu"); Command_ReProcess(playerid, cmd, 0); }
        case 13: { ShowPlayerHudTypesList(playerid); }
        case 14: {
            User[playerid][show3DDeathTag] = !User[playerid][show3DDeathTag];
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
        case 15: {
            User[playerid][showRandomDeathTags] = !User[playerid][showRandomDeathTags];
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
        case 16: {
            User[playerid][showHeadshot] = !User[playerid][showHeadshot];
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
        case 17: { ShowHitSoundDialog(playerid); }
        case 18: { ShowKillSoundDialog(playerid); }
        case 19: {
            User[playerid][hideWelcomeMsg] = !User[playerid][hideWelcomeMsg];
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowPersonalizationMenu(playerid);
        }
        case 20: { ShowPlayerLifeTypesList(playerid); }
    }
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

stock ShowHitSoundDialog(playerid)
{
    new title[64] = "{MAIN}Personalization - Hit Sound Selection";
    format_MainColor(playerid, title);

    new str[1200] = "{MAIN}ID\t{MAIN}Sound Name\n";
    for(new i = 0; i < sizeof(g_SoundList); i++)
    {
        format(str, sizeof(str), "%s{FFFFFF}%d\t%s{FFFFFF}%s\n",
            str,
            g_SoundList[i][soundID],
            STYLE_MARK(User[playerid][hitSoundID] == g_SoundList[i][soundID]),
            g_SoundList[i][soundName]
        );
    }
    format_MainColor(playerid, str);

    Dialog_ShowCallback(playerid, using public DIALOG_HIT_SOUND_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, str, "Select", "Back");
    return 1;
}

forward DIALOG_HIT_SOUND_LIST(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HIT_SOUND_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
        return ShowPersonalizationMenu(playerid);

    if(listitem >= 0 && listitem < sizeof(g_SoundList))
    {
        User[playerid][hitSoundID] = g_SoundList[listitem][soundID];
        if(User[playerid][hitSoundID] != 0)
        {
            PlayerPlaySound(playerid, User[playerid][hitSoundID], 0.0, 0.0, 0.0);
        }
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
        ShowHitSoundDialog(playerid);
    }
    return 1;
}

stock ShowKillSoundDialog(playerid)
{
    new title[64] = "{MAIN}Personalization - Kill Sound Selection";
    format_MainColor(playerid, title);

    new str[1200] = "{MAIN}ID\t{MAIN}Sound Name\n";
    for(new i = 0; i < sizeof(g_SoundList); i++)
    {
        format(str, sizeof(str), "%s{FFFFFF}%d\t%s{FFFFFF}%s\n",
            str,
            g_SoundList[i][soundID],
            STYLE_MARK(User[playerid][killSoundID] == g_SoundList[i][soundID]),
            g_SoundList[i][soundName]
        );
    }
    format_MainColor(playerid, str);

    Dialog_ShowCallback(playerid, using public DIALOG_KILL_SOUND_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, str, "Select", "Back");
    return 1;
}

forward DIALOG_KILL_SOUND_LIST(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_KILL_SOUND_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
        return ShowPersonalizationMenu(playerid);

    if(listitem >= 0 && listitem < sizeof(g_SoundList))
    {
        User[playerid][killSoundID] = g_SoundList[listitem][soundID];
        if(User[playerid][killSoundID] != 0)
        {
            PlayerPlaySound(playerid, User[playerid][killSoundID], 0.0, 0.0, 0.0);
        }
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
        ShowKillSoundDialog(playerid);
    }
    return 1;
}

@cmd(.alts = {"hudstyle", "hudtype"}) customhud(playerid, params[], help)
{
    ShowPlayerHudTypesList(playerid);
    return 1;
}
