
forward TIME(playerid, dialogid, response, listitem, string:inputtext[]);
public TIME(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
        return 1;
    else
    {
        new timeID = strval(inputtext);
        ChangeTime(playerid, timeID);
       	ShowMenu(playerid);
    }

    return true;
}

forward WEATHER(playerid, dialogid, response, listitem, string:inputtext[]);
public WEATHER(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
        return 1;
    else
    {
        new weatherID = strval(inputtext);
        ChangeWeather(playerid, weatherID);
        ShowMenu(playerid);
    }
    return true;
}

// The skin picker is a model-preview grid: modules/ui/skin_picker.inc

// Dialogs /menu, Settings e Personalization.

SetAppearanceSource(playerid, source)
{
    SetPVarInt(playerid, "appearance_source", source);
    return 1;
}

GetAppearanceSource(playerid)
{
    return GetPVarInt(playerid, "appearance_source");
}

ReturnToAppearanceSource(playerid)
{
    switch(GetPVarInt(playerid, "appearance_source"))
    {
        case APPEARANCE_SRC_PERSONAL: ShowPersonalizationMenu(playerid);
        case APPEARANCE_SRC_SETTINGS: ShowSettingsMenu(playerid);
        case APPEARANCE_SRC_NONE: {} // opened from chat, nothing to reopen
        default: ShowMenu(playerid);
    }
    return 1;
}

ShowHudColorDialog(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new body[3500] = "Name\tColor 1\tColor 2\n";
    for(new i; i < sizeof(hudPersonalization); i++)
    {
        new name[24], hex1[7], hex2[7];
        format(name, sizeof(name), "%s", hudPersonalization[i][lineInfo]);
        if(name[0] >= 'a' && name[0] <= 'z') name[0] -= 32;
        new space = strfind(name, " ");
        if(space != -1 && name[space+1] >= 'a' && name[space+1] <= 'z') name[space+1] -= 32;

        hex1 = GetColorHexTag(hudPersonalization[i][colorOne]);
        hex2 = GetColorHexTag(hudPersonalization[i][colorTwo]);

        strcat(body, fmat_return("%s{%s}%s\t{%s}Preview\t{%s}Preview\n",
            STYLE_MARK(User[playerid][hudColor] == i), CoresDoBglh[i], name, hex1, hex2), sizeof(body));
    }

    new title[64] = "{MAIN}Personalization - HUD Color";
    format_MainColor(playerid, title);
    format_MainColor(playerid, body);

    return Dialog_ShowCallback(playerid, using public HUD_COLORS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Accept", "Cancel");
}

ShowChatColorDialog(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new idHex[7], textHex[7];
    GetPlayerChatIDColorHex(playerid, idHex);
    GetPlayerChatTextColorHex(playerid, textHex);

    new body[512];
    format(body, sizeof(body),
"Option\tPreview\n\
Color ID in chat\t{%s}[%d] Sample ID\n\
Text color in chat\t{%s}Sample Chat Text",
        idHex, playerid, textHex);

    new title[64] = "{MAIN}Personalization - Chat Color";
    format_MainColor(playerid, title);

    return Dialog_ShowCallback(playerid, using public CHAT_COLOR_MAIN<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
}

forward CHAT_COLOR_MAIN(playerid, dialogid, response, listitem, string:inputtext[]);
public CHAT_COLOR_MAIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response)
        return ReturnToAppearanceSource(playerid);

    if(listitem == 0)
    {
        ShowChatColorSelectionDialog(playerid, 0, GetAppearanceSource(playerid));
    }
    else if(listitem == 1)
    {
        ShowChatColorSelectionDialog(playerid, 1, GetAppearanceSource(playerid));
    }

    return 1;
}

ShowChatColorSelectionDialog(playerid, color_type, source)
{
    SetAppearanceSource(playerid, source);
    SetPVarInt(playerid, "ChatColorType", color_type);

    new curHex[7];
    if(color_type == 0) GetPlayerChatIDColorHex(playerid, curHex);
    else GetPlayerChatTextColorHex(playerid, curHex);

    new body[2048];
    format(body, sizeof(body), "Color Name\tSample Text\n");

    strcat(body, fmat_return("{MAIN}Custom HEX Color\t{%s}#%s\n", curHex, curHex), sizeof(body));

    for(new i; i < sizeof(chatPersonalization); i++)
    {
        new bool:isSelected = false;
        if(color_type == 0)
        {
            if(User[playerid][chatIDColor] == i) isSelected = true;
        }
        else
        {
            if(User[playerid][chatColor] == i) isSelected = true;
        }

        strcat(body, fmat_return("%s{%s}%s\t{%s}Sample Chat Text\n",
            STYLE_MARK(isSelected), chatPersonalization[i][chatColorHex], chatPersonalization[i][chatColorName],
            chatPersonalization[i][chatColorHex]), sizeof(body));
    }

    new title[64];
    format(title, sizeof(title), "{MAIN}Chat Color - %s", (color_type == 0) ? ("Color ID") : ("Text Color"));
    format_MainColor(playerid, title);
    format_MainColor(playerid, body);

    return Dialog_ShowCallback(playerid, using public CHAT_COLORS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
}

ShowChatColorCustomHexDialog(playerid, color_type, source)
{
    SetAppearanceSource(playerid, source);
    SetPVarInt(playerid, "ChatColorType", color_type);

    new curHex[7];
    if(color_type == 0) GetPlayerChatIDColorHex(playerid, curHex);
    else GetPlayerChatTextColorHex(playerid, curHex);

    new body[512];
    format(body, sizeof(body),
"{FFFFFF}Current %s Color: {%s}#%s\n\n\
{BEBEBE}Enter your new 6-digit HEX color code below (e.g. {MAIN}FF0000{BEBEBE} for Red, {MAIN}00FF00{BEBEBE} for Green).\n\
{BEBEBE}Valid characters: 0-9, A-F.",
        (color_type == 0) ? ("ID") : ("Text"), curHex, curHex);

    new title[64];
    format(title, sizeof(title), "{MAIN}Custom HEX Color - %s", (color_type == 0) ? ("Color ID") : ("Text Color"));
    format_MainColor(playerid, title);
    format_MainColor(playerid, body);

    return Dialog_ShowCallback(playerid, using public CHAT_COLOR_CUSTOM_HEX<iiiis>, DIALOG_STYLE_INPUT, title, body, "Confirm", "Back");
}

ShowDamageStyleDialog(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new const damageStyles[][] = {
        "Dynamic (Moving numbers)",
        "Numbers (Static numbers)",
        "Dot (*)",
        "X (X)"
    };

    new body[384];
    body[0] = '\0';

    for(new i; i < sizeof(damageStyles); i++)
        AppendStyleRow(body, damageStyles[i], (User[playerid][dmgInformerStyle] == i));

    new title[64] = "{MAIN}Damage Informer Style";
    format_MainColor(playerid, title);
    format_MainColor(playerid, body);

    return Dialog_ShowCallback(playerid, using public DIALOG_DMG_STYLE<iiiis>, DIALOG_STYLE_LIST, title, body, "Select", "Back");
}

ShowHitmarkColorDialog(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new body[512];
    format(body, sizeof body, ""COL_WHITE"Change the color of your hitmark.\n\
Your current color: {%06x}#%x\n\
"COL_WHITE"Follow this model: \n{CC0000}FF{00CC00}FF{0000CC}FF"COL_WHITE"FF\n\
Visit this site if you prefer >> https://g.co/kgs/A2aHcY\n\
This is the order >> {CC0000}Red, {00CC00}Green {0000CC}Blue "COL_WHITE"and Alpha (Transparency)",
        User[playerid][hitmarkColor] >>> 8, User[playerid][hitmarkColor]);

    new title[64] = "{MAIN}Change hitmark color";
    format_MainColor(playerid, title);

    return Dialog_ShowCallback(playerid, using public DIALOG_HITMARK<iiiis>, DIALOG_STYLE_INPUT, title, body, "Confirm", "Cancel");
}

forward HUD_COLORS(playerid, dialogid, response, listitem, string:inputtext[]);
public HUD_COLORS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response)
        return ReturnToAppearanceSource(playerid);

    if(0 <= listitem < sizeof(hudPersonalization))
    {
        User[playerid][hudColor] = listitem;
        UpdateRoundStatsColors(playerid);
        updatePlayerHud(playerid);
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);

        if(User[playerid][showPlayerHud])
        {
            PlayerTextDrawShow(playerid, FPSPingPacket[playerid]);
            PlayerTextDrawShow(playerid, InfoStats[playerid][0]);
            PlayerTextDrawShow(playerid, InfoStats[playerid][1]);
        }
    }

    return ReturnToAppearanceSource(playerid);
}

forward CHAT_COLORS(playerid, dialogid, response, listitem, string:inputtext[]);
public CHAT_COLORS(playerid, dialogid, response, listitem, string:inputtext[])
{
    new color_type = GetPVarInt(playerid, "ChatColorType");
    if (!response)
    {
        return ShowChatColorDialog(playerid, GetAppearanceSource(playerid));
    }

    if(listitem == 0)
    {
        return ShowChatColorCustomHexDialog(playerid, color_type, GetAppearanceSource(playerid));
    }

    new selectedIdx = listitem - 1;
    if(0 <= selectedIdx < sizeof(chatPersonalization))
    {
        if(color_type == 0)
        {
            User[playerid][chatIDColor] = selectedIdx;
        }
        else
        {
            User[playerid][chatColor] = selectedIdx;
        }
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
    }

    return ShowChatColorSelectionDialog(playerid, color_type, GetAppearanceSource(playerid));
}

forward CHAT_COLOR_CUSTOM_HEX(playerid, dialogid, response, listitem, string:inputtext[]);
public CHAT_COLOR_CUSTOM_HEX(playerid, dialogid, response, listitem, string:inputtext[])
{
    new color_type = GetPVarInt(playerid, "ChatColorType");
    if (!response)
    {
        return ShowChatColorSelectionDialog(playerid, color_type, GetAppearanceSource(playerid));
    }

    new hexInput[16];
    format(hexInput, sizeof(hexInput), "%s", inputtext);

    if(hexInput[0] == '#')
    {
        strdel(hexInput, 0, 1);
    }

    new len = strlen(hexInput);
    if(len != 6)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid HEX format! Please enter 6 characters (e.g. FF0000).");
        return ShowChatColorCustomHexDialog(playerid, color_type, GetAppearanceSource(playerid));
    }

    new colorVal;
    if(sscanf(hexInput, "x", colorVal))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid HEX color code.");
        return ShowChatColorCustomHexDialog(playerid, color_type, GetAppearanceSource(playerid));
    }

    new storedVal = 0x10000000 | (colorVal & 0xFFFFFF);
    if(color_type == 0)
    {
        User[playerid][chatIDColor] = storedVal;
    }
    else
    {
        User[playerid][chatColor] = storedVal;
    }

    SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
    return ShowChatColorSelectionDialog(playerid, color_type, GetAppearanceSource(playerid));
}

forward DIALOG_DMG_STYLE(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_DMG_STYLE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && 0 <= listitem <= 3)
    {
        User[playerid][dmgInformerStyle] = listitem;
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
    }

    return ReturnToAppearanceSource(playerid);
}

forward DIALOG_HITMARK(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HITMARK(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
        return ReturnToAppearanceSource(playerid);

    if(strlen(inputtext) != 8)
        return SendClientMessage(playerid, -1, "%s", HITMARK_HELP);

    if(!IsHex(inputtext))
        return SendClientMessage(playerid, -1, "%s", HITMARK_HELP_2);

    User[playerid][hitmarkColor] = HexToInt(inputtext);
    SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);

    return ReturnToAppearanceSource(playerid);
}

forward DIALOG_MENU(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_MENU(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        switch(listitem)
        {
            case 0:
            {
                Dialog_ShowCallback(playerid, using public TIME<iiiis>, DIALOG_STYLE_INPUT, "Time", "Select time", "Continue", "Cancel");
            }
            case 1:
            {
                Dialog_ShowCallback(playerid, using public WEATHER<iiiis>, DIALOG_STYLE_INPUT, "Weather", "Select weather", "Continue", "Cancel");
            }
            case 2:
            {
                if(User[playerid][showPlayerHud] == false)
                {
                    hidePlayerTextdraw(playerid);
                    showPlayerTextDraw(playerid);
                    User[playerid][showPlayerHud] = true;
                }
                else{
                    User[playerid][showPlayerHud] = false;
                    hidePlayerTextdraw(playerid);
                }
                SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
                ShowMenu(playerid);
            }
            case 3: ShowDamageStyleDialog(playerid, APPEARANCE_SRC_MENU);
            case 4: ShowHitmarkColorDialog(playerid, APPEARANCE_SRC_MENU);
            case 5: ShowHudColorDialog(playerid, APPEARANCE_SRC_MENU);
            case 6: ShowChatColorDialog(playerid, APPEARANCE_SRC_MENU);
            case 7: ShowSkinDialog(playerid, APPEARANCE_SRC_MENU);
            case 8: ChangeFight(playerid);
            case 9: ShowGunMenu(playerid);
        }
    }

    return true;
}

forward CHANGE(playerid, dialogid, response, listitem, string:inputtext[]);
public CHANGE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        if(User[playerid][playerWeapons][0] == 0 && User[playerid][playerWeapons][1] == 0 && User[playerid][playerWeapons][2] == 0 && User[playerid][playerWeapons][3] == 0 && User[playerid][isAlreadySpawned])
        {
            SendClientMessage(playerid, -1, "%s", CHOOSE_WEAPON_MSG);
            gunMenu(playerid);
            DeletePVar(playerid, "slotName");
            return false;
        }

        if(GetPVarInt(playerid, "showGunmenuFirst") == 1)
        {
        	SetPlayerVirtualWorld(playerid, User[playerid][locationId]*99);
        	DeletePVar(playerid, "showGunmenuFirst");
        }

        givePlayerWeapons(playerid);

        if(GetPVarInt(playerid, "gunmenu_world"))
        {
        	DeletePVar(playerid, "gunmenu_world");
        	if(User[playerid][locationId] != -1)
        		SetPlayerVirtualWorld(playerid, User[playerid][locationId]*99);
        }

        return true;
    }

    switch(listitem)
    {
        case 0:
        {
            Dialog_ShowCallback(playerid, using public CHANGE1<iiiis>, DIALOG_STYLE_LIST, "Select weapon for first slot", showAvailableWeapons(), "Select", "Back");
            SetPVarInt(playerid, "slotId", listitem);
        }


        case 1:
        {
            Dialog_ShowCallback(playerid, using public CHANGE1<iiiis>, DIALOG_STYLE_LIST, "Select weapon for second slot", showAvailableWeapons(), "Select", "Back");
            SetPVarInt(playerid, "slotId", listitem);
        }

        case 2:
        {
            if(User[playerid][playerLevel] < 5)
                return SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}The third slot will be available only after reaching level {MAIN}5{FFFFFF}."), gunMenu(playerid);
            Dialog_ShowCallback(playerid, using public CHANGE1<iiiis>, DIALOG_STYLE_LIST, "Select weapon for third slot", showAvailableWeapons(), "Select", "Back");
            SetPVarInt(playerid, "slotId", listitem);
        }
        case 3:
        {
            if(User[playerid][playerLevel] < 10) return SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}The fourth slot will be available only after reaching level {MAIN}10{FFFFFF}."), gunMenu(playerid);
            Dialog_ShowCallback(playerid, using public CHANGE1<iiiis>, DIALOG_STYLE_LIST, "Select weapon for slot four", showAvailableWeapons(), "Select", "Back");
            SetPVarInt(playerid, "slotId", listitem);
        }

        default:
            gunMenu(playerid);
    }

    givePlayerWeapons(playerid);
    return true;
}

forward CHANGE1(playerid, dialogid, response, listitem, string:inputtext[]);
public CHANGE1(playerid, dialogid, response, listitem, string:inputtext[])
{
	if(!response || !(0 <= listitem < sizeof(gunmenuWInfo)))
	{
		return gunMenu(playerid);
	}

	new slotid = GetPVarInt(playerid, "slotId");
	new listitemm = listitem+1;
	new slotNum[4][6] =
	{
		"one",
		"two",
		"three",
		"four"
	};
	switch(listitem)
	{
		case 0, 1: User[playerid][playerWeapons][slotid] = listitemm;
		case 2:
		{
			if(User[playerid][playerLevel] < 5)
				return Dialog_ShowCallback(playerid, using public CHANGE1<iiiis>, DIALOG_STYLE_LIST, fmat_return("Select weapon for slot %s", slotNum[slotid]), showAvailableWeapons(), "Select", "Back"), SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}This weapon will be available upon reaching level {MAIN}5{FFFFFF}.");
			User[playerid][playerWeapons][slotid] = listitemm;
		}
		case 3:
		{
			if(User[playerid][playerLevel] < 10)
				return Dialog_ShowCallback(playerid, using public CHANGE1<iiiis>, DIALOG_STYLE_LIST, fmat_return("Select weapon for slot %s", slotNum[slotid]), showAvailableWeapons(), "Select", "Back"), SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}This weapon will be available upon reaching level {MAIN}10{FFFFFF}.");

			User[playerid][playerWeapons][slotid] = listitemm;
		}
		case 4:	User[playerid][playerWeapons][slotid] = 0;
	}

	givePlayerWeapons(playerid);
	gunMenu(playerid);

    return true;
}

forward FIGHT(playerid, dialogid, response, listitem, string:inputtext[]);
public FIGHT(playerid, dialogid, response, listitem, string:inputtext[])
{
	if(response)
    {
		if(0 <= listitem < sizeof(fightStyles))
        {
            User[playerid][fightStyle] = fightStyles[listitem][styleid];
            SetPlayerFightingStyle(playerid, User[playerid][fightStyle]);
            UpdatePlayerData(playerid);
            va_SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Your fighting style has been updated to: {MAIN}%s", fightStyles[listitem][fightname]);
        }
	}

	return true;
}

forward CHANGE_NICK(playerid, dialogid, response, listitem, string:inputtext[]);
public CHANGE_NICK(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response)
    {
        return true;
    }
    if (!(3 <= strlen(inputtext) <= 20))
    {
        return SendClientMessage(playerid, -1, "{MAIN}**{FFFFFF} Invalid nickname entered, try another one");
    }

    if(IsPlayerFakingNick(playerid))
        return SendClientMessage(playerid, -1, "%s", NICK_CHANGE_WHILE_FAKE);

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `id` FROM `accounts` WHERE `username` = '%e'", inputtext);
    mysql_tquery(dbHandle, queryGlobal, "OnPlayerTryChangeNick", "iss", playerid, inputtext, User[playerid][userName]);

	return true;
}

forward DIALOG_RANK(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_RANK(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        // training challenges: served from the training module caches, no query
        if(listitem == 10)
        {
            CallLocalFunction("ShowTrainingChallengeDialog", "i", playerid);
            return true;
        }

        new query[256], header[128];
        switch(listitem)
        {
            case 0: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `played_time` FROM `accounts` ORDER BY `played_time` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}> {FFFFFF}Top 10: Played Time"); }
            case 1: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `kills` FROM `accounts` ORDER BY `kills` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}> {FFFFFF}Top 10: Total Kills"); }
            case 2: { mysql_format(dbHandle, query, sizeof(query), "SELECT `clan_name`, `clan_kills`, `clan_zone_points` FROM `clans` ORDER BY `clan_zone_points` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}> {FFFFFF}Top 10: Dominant Clans"); }
            case 3: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `kills_semanais` FROM `accounts` ORDER BY `kills_semanais` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}> {FFFFFF}Top 10: Weekly Kills"); }
            case 4: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `damage_semanal` FROM `accounts` ORDER BY `damage_semanal` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}> {FFFFFF}Top 10: Weekly Damage"); }
            case 5: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `deaths` FROM `accounts` ORDER BY `deaths` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}> {FFFFFF}Top 10: Total Deaths"); }
            case 6: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `max_streak` FROM `accounts` ORDER BY `max_streak` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}> {FFFFFF}Top 10: Highest Streaks"); }
            case 7: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `max_damage` FROM `accounts` ORDER BY `max_damage` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}> {FFFFFF}Top 10: Session Peak Damage"); }
            case 8: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `won_arenas` FROM `accounts` ORDER BY `won_arenas` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}> {FFFFFF}Top 10: Most Wins"); }
            case 9: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `duel_wins` FROM `accounts` ORDER BY `duel_wins` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}> {FFFFFF}Top 10: Duel Victories"); }
        }

        inline const OnRankLoad()
        {
            new liststr[2048], usernamee[24], value, clanName_[20], clankills, clanzp;
            new rows = cache_num_rows();
            
            if(rows)
            {
                if(listitem == 2) format(liststr, sizeof(liststr), "{BEBEBE}#\t{BEBEBE}Clan Name\t{BEBEBE}Score Points\t{BEBEBE}Frags\n");
                else if(listitem == 0) format(liststr, sizeof(liststr), "{BEBEBE}#\t{BEBEBE}Player\t{BEBEBE}Time Spent Online\n");
                else format(liststr, sizeof(liststr), "{BEBEBE}#\t{BEBEBE}Player\t{BEBEBE}Total Records\n");

                for(new i; i < rows; i++)
                {
                    new pos_color[16], trophy[8];
                    switch(i)
                    {
                        case 0: { pos_color = "{FFFF00}"; trophy = " (G)"; } // Gold
                        case 1: { pos_color = "{C0C0C0}"; trophy = " (S)"; } // Silver
                        case 2: { pos_color = "{CD7F32}"; trophy = " (B)"; } // Bronze
                        default: { pos_color = "{MAIN}"; trophy = ""; }
                    }

                    if(listitem == 2) // Clan Rank
                    {
                        cache_get_value_name(i, "clan_name", clanName_);
                        cache_get_value_name_int(i, "clan_zone_points", clanzp);
                        cache_get_value_name_int(i, "clan_kills", clankills);
                        strcat(liststr, fmat_return("%s%02d.\t{FFFFFF}%s%s\t{BEBEBE}%d pts\t{BEBEBE}%d kills\n", pos_color, i+1, clanName_, trophy, clanzp, clankills));
                    }
                    else // Player Ranks
                    {
                        cache_get_value_name(i, "username", usernamee);
                        
                        if(listitem == 0) // Played Time
                        {
                            cache_get_value_name_int(i, "played_time", value);
                            new hour = value/3600, minute = (value%3600)/60, second = value%60;
                            strcat(liststr, fmat_return("%s%02d.\t{FFFFFF}%s%s\t{BEBEBE}%02dh %02dm %02ds\n", pos_color, i+1, usernamee, trophy, hour, minute, second));
                        }
                        else
                        {
                            new col_name[32], unit[16] = "";
                            switch(listitem)
                            {
                                case 1: col_name = "kills";
                                case 3: col_name = "kills_semanais";
                                case 4: { col_name = "damage_semanal"; unit = " dmg"; }
                                case 5: col_name = "deaths";
                                case 6: col_name = "max_streak";
                                case 7: { col_name = "max_damage"; unit = " dmg"; }
                                case 8: { col_name = "won_arenas"; unit = " wins"; }
                                case 9: { col_name = "duel_wins"; unit = " wins"; }
                            }
                            cache_get_value_name_int(i, col_name, value);
                            strcat(liststr, fmat_return("%s%02d.\t{FFFFFF}%s%s\t{BEBEBE}%d%s\n", pos_color, i+1, usernamee, trophy, value, unit));
                        }
                    }
                }
                
                format_MainColor(playerid, header);
                format_MainColor(playerid, liststr);

                Dialog_ShowCallback(playerid, using public DIALOG_RANK1<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, header, liststr, "< Back", "Exit");
            }
            else
            {
                SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}No data found for this rank.");
                Dialog_ShowCallback(playerid, using public DIALOG_RANK1<iiiis>, DIALOG_STYLE_MSGBOX, "Rankings", "No records found yet.", "Back", "");
            }
        }
        MySQL_TQueryInline(dbHandle, using inline OnRankLoad, query);
    }
    return true;
}

forward DIALOG_RANK1(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_RANK1(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        new rankstr[1024];
        strcat(rankstr, "{BEBEBE}Category\t{BEBEBE}Objective Description\n");
        strcat(rankstr, "{FFFFFF}Played Time\t{A2A2A2}Ranking of the most active community members\n");
        strcat(rankstr, "{FFFFFF}Total Kills\t{A2A2A2}Global statistics of confirmed frags\n");
        strcat(rankstr, "{FFFFFF}Best Clans\t{A2A2A2}Clans with highest points and dominance\n");
        strcat(rankstr, "{FFFFFF}Weekly Kills\t{A2A2A2}Top killers of the current week\n");
        strcat(rankstr, "{FFFFFF}Weekly Damage\t{A2A2A2}Top damage dealers of the current week\n");
        strcat(rankstr, "{FFFFFF}Total Deaths\t{A2A2A2}Statistics of players with most casualties\n");
        strcat(rankstr, "{FFFFFF}Killstreak\t{A2A2A2}Best combat records without dying\n");
        strcat(rankstr, "{FFFFFF}Peak Damage\t{A2A2A2}Highest damage achieved in a single session\n");
        strcat(rankstr, "{FFFFFF}Arenas Won\t{A2A2A2}Elite players with most round victories\n");
        strcat(rankstr, "{FFFFFF}Duel Victories\t{A2A2A2}Top duel champions of the server\n");
        strcat(rankstr, "{FFFFFF}Challenges\t{A2A2A2}Training arena records: yours and the server's");

        // o buffer precisa de folga: "{MAIN}" (6) vira "{RRGGBB}" (8) na substituicao
        new header[64] = "{MAIN}> {FFFFFF}Server Statistics & Rankings";
        format_MainColor(playerid, header);

        Dialog_ShowCallback(playerid, using public DIALOG_RANK<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, header, rankstr, "View Rank", "Close");
    }
    return true;
}

forward DIALOG_MAPLIST(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_MAPLIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    new maxitems, listitemm, currentpage;
    currentpage = GetPVarInt(playerid, "playerCurrentPage");
    maxitems = GetPVarInt(playerid, "pageItems");
    listitemm = listitem+(currentpage*16);

    if(response)
    {
        if(listitem < maxitems)
        {
            if(changeArenaDelay > gettime() &&  User[playerid][adminLevel] < 4)
                return va_SendClientMessage(playerid, -1, CHANGEARENA_DELAY, convertTime(changeArenaDelay - gettime()));

            new location = User[playerid][locationId];
            if(location < 0 || location >= MAX_LOCATIONS || location == ARENA_AFK)
                return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be inside an arena to use this command.");

            if(listitemm < 0 || listitemm >= MAX_ZONES)
                return va_SendClientMessage(playerid, -1, NOT_VALID_ARENA, MAX_ZONES-1);

            foreach(new i : Player)
            {
                if(User[i][locationId] != location || GetPVarInt(i, "DuelS_State") != DUELS_STATE_NONE) continue;
                GangZoneStopPulseForPlayer(i, e_ZONE_PULSE_ZONE, arenaInfo[location][arenaId]);
            }

            GetRoundRank(location);
            new oldArenaId = arenaInfo[location][arenaId];
            arenaInfo[location][arenaId] = listitemm;
            PrepareChangeArena(location, oldArenaId);

            foreach(new i : Player)
            {
                if((User[i][locationId] == location && GetPVarInt(i, "DuelS_State") == DUELS_STATE_NONE) || User[i][adminLevel] > 0)
                    va_SendClientMessage(i, -1, CHANGED_ARENA_MSG , User[playerid][userName] , arenaData[listitemm][arenaName], listitemm);
            }

            if(User[playerid][adminLevel] < 4)
                changeArenaDelay = gettime()+(3*60);
        }
        else if (listitem == maxitems)
        {
            if((currentpage + 1) * 16 >= MAX_ZONES)
            {
                showMapListDialog(playerid);
                return true;
            }
            SetPVarInt(playerid, "playerCurrentPage", currentpage+1);
            showMapListDialog(playerid);
        }
        else if (listitem == maxitems+1)
        {
            if(currentpage <= 0)
            {
                showMapListDialog(playerid);
                return true;
            }
            SetPVarInt(playerid, "playerCurrentPage", currentpage-1);
            showMapListDialog(playerid);
        }
    }
    return true;
}

// Clan

forward DIALOG_PROFILE_MENU(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_PROFILE_MENU(playerid, dialogid, response, listitem, string:inputtext[])
{
    new action = 0, targetid = INVALID_PLAYER_ID;

    if(response)
    {
        targetid = GetPVarInt(playerid, "profile_target");

        new pvar[20];
        format(pvar, sizeof(pvar), "profile_act_%d", listitem);
        action = GetPVarInt(playerid, pvar);
    }

    new act_count = GetPVarInt(playerid, "profile_act_count");
    for(new i = 0; i < act_count; i++)
    {
        new pvar_clean[20];
        format(pvar_clean, sizeof(pvar_clean), "profile_act_%d", i);
        DeletePVar(playerid, pvar_clean);
    }
    DeletePVar(playerid, "profile_act_count");

    if(response)
    {
        if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

        switch(action)
        {
            case 1: // Geral stats
            {
                ShowStats(playerid, targetid);
            }
            case 2: // Reports
            {
                if(targetid == playerid) return SendClientMessage(playerid, -1, "%s", CANNOT_REPORT_SELF);
                if(User[targetid][adminLevel] > 0) return SendClientMessage(playerid, -1, "%s", CANNOT_REPORT_ADMIN);

                new header[64];
                format(header, sizeof(header), "{MAIN}Submit Report: {FFFFFF}%s", User[targetid][userName]);
                format_MainColor(playerid, header);
                Dialog_ShowCallback(playerid, using public DIALOG_REPORT_SUBMIT<iiiis>, DIALOG_STYLE_INPUT, header, "{FFFFFF}Please enter the reason for reporting this player:", "Submit", "Cancel");
            }
            case 3: // Clan Information
            {
                if(User[targetid][clanID] == -1) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}This player is no longer in a clan.");
                ShowPlayerClanInfo(playerid, targetid);
            }
            case 4: // Names changes
            {
                if(User[playerid][adminLevel] < 1) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);
                ShowNameHistory(playerid, targetid);
            }
            case 5: // Penality historico
            {
                if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);
                ShowPenaltyHistory(playerid, User[targetid][userName]);
            }
            case 6: // Go
            {
                new cmd[32];
                format(cmd, sizeof(cmd), "/go %d", targetid);
                Command_ReProcess(playerid, cmd, 0);
            }
            case 7: // Get
            {
                new cmd[32];
                format(cmd, sizeof(cmd), "/get %d", targetid);
                Command_ReProcess(playerid, cmd, 0);
            }
            case 8: // Slap
            {
                new cmd[32];
                format(cmd, sizeof(cmd), "/slap %d", targetid);
                Command_ReProcess(playerid, cmd, 0);
            }
            case 9: // Mute
            {
                new header[64];
                format(header, sizeof(header), "{MAIN}Mute Player: {FFFFFF}%s", User[targetid][userName]);
                format_MainColor(playerid, header);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MUTE_REASON<iiiis>, DIALOG_STYLE_INPUT, header, "{FFFFFF}Please enter the reason for muting this player:", "Submit", "Cancel");
            }
            case 10: // Unmute
            {
                new cmd[32];
                format(cmd, sizeof(cmd), "/unmute %d", targetid);
                Command_ReProcess(playerid, cmd, 0);
            }
            case 11: // Kick
            {
                new header[64];
                format(header, sizeof(header), "{MAIN}Kick Player: {FFFFFF}%s", User[targetid][userName]);
                format_MainColor(playerid, header);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_KICK_REASON<iiiis>, DIALOG_STYLE_INPUT, header, "{FFFFFF}Please enter the reason for kicking this player:", "Submit", "Cancel");
            }
            case 12: // Ban
            {
                new header[64];
                format(header, sizeof(header), "{MAIN}Ban Player: {FFFFFF}%s", User[targetid][userName]);
                format_MainColor(playerid, header);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_BAN_REASON<iiiis>, DIALOG_STYLE_INPUT, header, "{FFFFFF}Please enter the reason for banning this player:", "Submit", "Cancel");
            }
            case 13: // Spec
            {
                new cmd[32];
                format(cmd, sizeof(cmd), "/spec %d", targetid);
                Command_ReProcess(playerid, cmd, 0);
            }
            case 14: // Gtemp
            {
                new cmd[32];
                format(cmd, sizeof(cmd), "/gtemp %d", targetid);
                Command_ReProcess(playerid, cmd, 0);
            }
            case 15: // Ask Question
            {
                new header[64] = "{MAIN}Submit Question";
                format_MainColor(playerid, header);
                Dialog_ShowCallback(playerid, using public DIALOG_ASK_SUBMIT<iiiis>, DIALOG_STYLE_INPUT, header, "{FFFFFF}Please enter your question for the administration:", "Submit", "Cancel");
            }
            case 16: // Achievements
            {
                ShowAchievementsMainMenu(playerid, targetid);
            }
        }
    }

    return 1;
}

forward DIALOG_ASK_SUBMIT(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ASK_SUBMIT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        if(GetPVarInt(playerid, "ask_cooldown"))
            return SendClientMessage(playerid, -1, "{MAIN}[ASK] {FFFFFF}Please wait 30 seconds before sending another question.");

        if(isnull(inputtext)) 
        {
            new header[64] = "{MAIN}Submit Question";
            format_MainColor(playerid, header);
            return Dialog_ShowCallback(playerid, using public DIALOG_ASK_SUBMIT<iiiis>, DIALOG_STYLE_INPUT, header, "{F3230E}Question cannot be empty!{FFFFFF}\n\nPlease enter your question for the administration:", "Submit", "Cancel");
        }

        new cmd[256];
        format(cmd, sizeof(cmd), "/ask %s", inputtext);
        Command_ReProcess(playerid, cmd, 0);
    }
    return 1;
}

forward DIALOG_REPORT_SUBMIT(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_REPORT_SUBMIT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        if(GetPVarInt(playerid, "report_cooldown"))
            return SendClientMessage(playerid, -1, "{MAIN}[REPORT] {FFFFFF}Please wait 30 seconds before sending another report.");

        new targetid = GetPVarInt(playerid, "profile_target");
        if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
        if(targetid == playerid) return SendClientMessage(playerid, -1, "%s", CANNOT_REPORT_SELF);
        if(User[targetid][adminLevel] > 0) return SendClientMessage(playerid, -1, "%s", CANNOT_REPORT_ADMIN);
        if(isnull(inputtext)) 
        {
            new header[64];
            format(header, sizeof(header), "{MAIN}Submit Report: {FFFFFF}%s", User[targetid][userName]);
            format_MainColor(playerid, header);
            return Dialog_ShowCallback(playerid, using public DIALOG_REPORT_SUBMIT<iiiis>, DIALOG_STYLE_INPUT, header, "{F3230E}Reason cannot be empty!{FFFFFF}\n\nPlease enter the reason for reporting this player:", "Submit", "Cancel");
        }

        SetPVarInt(playerid, "report_cooldown", 1);
        SetTimerEx("ReleaseReport", 30000, false, "i", playerid);

        foreach(new i : Player)
        {
            if(User[i][adminLevel] > 0)
            {
                if(IsPlayerVip(playerid))
                {
                    va_SendClientMessage(i, 0xFF7700FF, "{MAIN}[VIP REPORT] {FFFFFF}Player %s[%d] reported: %s[%d] | Reason: %s", User[playerid][userName], playerid, User[targetid][userName], targetid, inputtext);
                    GameTextForPlayer(i, "~n~~o~VIP Report", 3000, 3);
                }
                else
                {
                    va_SendClientMessage(i, 0xEDA909AA, "{MAIN}[REPORT] {FFFFFF}Player %s[%d] reported: %s[%d] | Reason: %s", User[playerid][userName], playerid, User[targetid][userName], targetid, inputtext);
                    GameTextForPlayer(i, "~n~~r~Review", 3000, 3);
                }
                PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
            }
        }
        va_SendClientMessage(playerid, -1, "{MAIN}[REPORT] {FFFFFF}Your report against {MAIN}%s {FFFFFF}has been sent successfully to the Administration!", User[targetid][userName]);
        PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
        
        LogTicket(User[playerid][user_sqlid], User[targetid][userName], inputtext);

        new log_details[256];
        format(log_details, sizeof(log_details), "Player %s reported %s. Reason: %s", User[playerid][userName], User[targetid][userName], inputtext);
        LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "reports", "Report", log_details);

        new string[1024];
        format(string, sizeof(string), "**Reporter:** %s\n**Target:** %s\n**Reason:** %s",
            User[playerid][userName],
            User[targetid][userName],
            inputtext);

        SendDiscordEmbed(logs_report, "New Report", string, DISCORD_COLOR_WARNING);
    }
    return 1;
}

enum
{
    TAG_TYPE_VIP,
    TAG_TYPE_CLAN,
    TAG_TYPE_ADMIN,
    TAG_TYPE_CUSTOM,
    TAG_TYPE_EDIT_CUSTOM
};

new const g_PredefinedTags[][] = {
    "PRO PLAYER",
    "LEGEND",
    "LEGENDARY",
    "DM KING",
    "MONSTER",
    "HEADSHOOTER",
    "MYTH",
    "BAD BOY",
    "SUPREME",
    "DOMINATOR",
    "IMMORTAL",
    "EXECUTOR"
};

static g_PlayerTagMenu[MAX_PLAYERS][6];

static g_PlayerTagMenuCount[MAX_PLAYERS];

#define CUSTOM_TAG_MIN_LEN  (2)
#define CUSTOM_TAG_MAX_LEN  (16)
#define CUSTOM_TAG_NO_PERM  "{MAIN}[ERROR] {FFFFFF}You must be VIP, Admin Level 5 or have special permission to use Custom Tags."

stock ResetPlayerTagMenu(playerid)
{
    g_PlayerTagMenuCount[playerid] = 0;
    return 1;
}

stock CanPlayerUseCustomTag(playerid)
{
    if(IsPlayerVip(playerid) || User[playerid][adminLevel] >= 5 || User[playerid][customTagPerm]) return true;
    return false;
}

stock bool:SanitizeCustomTag(const input[], dest[], size = sizeof(dest))
{
    dest[0] = '\0';

    new end = strlen(input) - 1, start = 0;
    while(start <= end && input[start] == ' ') start++;
    while(end >= start && input[end] == ' ') end--;

    new outLen = end - start + 1;
    if(outLen < CUSTOM_TAG_MIN_LEN || outLen > CUSTOM_TAG_MAX_LEN) return false;
    if(outLen >= size) return false;

    for(new i = start; i <= end; i++)
    {
        new c = input[i];
        if(c < ' ' || c > '~') return false;
        if(c == '{' || c == '}') return false;
        if(c == '~') return false;
    }

    strmid(dest, input, start, end + 1, size);
    return true;
}

stock SaveCustomTag(playerid)
{
    new query[192];
    mysql_format(dbHandle, query, sizeof(query),
        "UPDATE accounts SET custom_tag = '%e', hide_custom_tag = %d WHERE id = %d",
        User[playerid][customTag], User[playerid][hideCustomTag], User[playerid][user_sqlid]);
    mysql_tquery(dbHandle, query);
    return 1;
}

stock ApplyCustomTag(playerid, const tag[])
{
    format(User[playerid][customTag], 32, "%s", tag);
    User[playerid][hideCustomTag] = false;

    SaveCustomTag(playerid);
    SetClanColor(playerid);
    va_SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}Your Custom Tag has been set to {FF4500}%s{FFFFFF}!", User[playerid][customTag]);
    return 1;
}

stock ClearCustomTag(playerid)
{
    User[playerid][customTag][0] = '\0';
    User[playerid][hideCustomTag] = true;

    SaveCustomTag(playerid);
    SetClanColor(playerid);
    SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}Your Custom Tag has been removed.");
    return 1;
}

stock ShowTagMenu(playerid)
{
    if(!User[playerid][isLogged]) return 0;

    new tagList[1024];
    format(tagList, sizeof(tagList), "{MAIN}Tag\t{MAIN}Display\t{MAIN}Status\n");

    new count = 0;

    // 1. VIP Tag
    if(IsPlayerVip(playerid))
    {
        g_PlayerTagMenu[playerid][count++] = TAG_TYPE_VIP;
        new vipPreview[32] = "{FFD700}[VIP]";
        new tagIdx = User[playerid][vipHeadTag];
        if(0 <= tagIdx < 10) format(vipPreview, sizeof(vipPreview), "{%s}[VIP]", vipColorListStr[tagIdx]);
        else if(tagIdx >= 0x10000000) format(vipPreview, sizeof(vipPreview), "{%06x}[VIP]", tagIdx & 0xFFFFFF);

        format(tagList, sizeof(tagList), "%s{FFFFFF}VIP Tag\t%s\t%s\n",
            tagList,
            vipPreview,
            User[playerid][hideVipTag] ? "{FF6347}[Hidden]" : "{23C552}[Visible]");
    }

    // 2. Clan Tag
    if(User[playerid][clanID] != -1)
    {
        g_PlayerTagMenu[playerid][count++] = TAG_TYPE_CLAN;
        new clanid = User[playerid][clanID];
        format(tagList, sizeof(tagList), "%s{FFFFFF}Clan Tag\t{%06x}[%s]\t%s\n",
            tagList,
            (ClanInfo[clanid][clanColor] >>> 8),
            ClanInfo[clanid][clanName],
            User[playerid][hideClanTag] ? "{FF6347}[Hidden]" : "{23C552}[Visible]");
    }

    // 3. Admin Tag
    if(User[playerid][adminLevel] > 0)
    {
        g_PlayerTagMenu[playerid][count++] = TAG_TYPE_ADMIN;
        format(tagList, sizeof(tagList), "%s{FFFFFF}Admin Tag\t{FFFFFF}[{FF4500}%s{FFFFFF}]\t%s\n",
            tagList,
            adminnames[User[playerid][adminLevel]][ingame],
            User[playerid][hideAdminTag] ? "{FF6347}[Hidden]" : "{23C552}[Visible]");
    }

    // 4. Custom Tag
    g_PlayerTagMenu[playerid][count++] = TAG_TYPE_CUSTOM;
    new customPreview[64];
    if(User[playerid][customTag][0] != '\0')
        format(customPreview, sizeof(customPreview), "{FF4500}%s", User[playerid][customTag]);
    else
        format(customPreview, sizeof(customPreview), "{B4B5B7}(None)");

    new customStatus[32];
    if(!CanPlayerUseCustomTag(playerid))
        format(customStatus, sizeof(customStatus), "{B4B5B7}[No Access]");
    else
        format(customStatus, sizeof(customStatus), "%s", User[playerid][hideCustomTag] ? "{FF6347}[Hidden]" : "{23C552}[Visible]");

    format(tagList, sizeof(tagList), "%s{FFFFFF}Custom Tag\t%s\t%s\n",
        tagList,
        customPreview,
        customStatus);

    // 5. Configure Custom Tag
    if(CanPlayerUseCustomTag(playerid))
    {
        g_PlayerTagMenu[playerid][count++] = TAG_TYPE_EDIT_CUSTOM;
        format(tagList, sizeof(tagList), "%s{MAIN}Configure Custom Tag\t{FFFFFF}Preset or Custom\t{90C3D4}[Configure]\n", tagList);
    }

    g_PlayerTagMenuCount[playerid] = count;

    new title[64] = "{MAIN}Tag Manager";
    format_MainColor(playerid, title);
    format_MainColor(playerid, tagList);

    return Dialog_ShowCallback(playerid, using public DIALOG_TAG_MANAGER<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, tagList, "Select", "Close");
}

forward DIALOG_TAG_MANAGER(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_TAG_MANAGER(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return 1;

    if(listitem < 0 || listitem >= g_PlayerTagMenuCount[playerid]) return 1;
    new tagType = g_PlayerTagMenu[playerid][listitem];

    switch(tagType)
    {
        case TAG_TYPE_VIP:
        {
            if(!IsPlayerVip(playerid)) return ShowTagMenu(playerid);

            User[playerid][hideVipTag] = !User[playerid][hideVipTag];
            if(User[playerid][hideVipTag])
            {
                SetPVarInt(playerid, "LastVipHeadTag", User[playerid][vipHeadTag]);
                User[playerid][vipHeadTag] = 10;
            }
            else
            {
                new lastTag = GetPVarInt(playerid, "LastVipHeadTag");
                if(lastTag == 10) lastTag = 0;
                User[playerid][vipHeadTag] = lastTag;
            }

            SetClanColor(playerid);

            new query[144];
            mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET hide_vip_tag = %d, vip_head_tag = %d WHERE id = %d", User[playerid][hideVipTag], User[playerid][vipHeadTag], User[playerid][user_sqlid]);
            mysql_tquery(dbHandle, query);

            va_SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}VIP Tag is now %s{FFFFFF}.", User[playerid][hideVipTag] ? "{FF6347}Hidden" : "{23C552}Visible");
            ShowTagMenu(playerid);
        }
        case TAG_TYPE_CLAN:
        {
            if(User[playerid][clanID] == -1) return ShowTagMenu(playerid);

            User[playerid][hideClanTag] = !User[playerid][hideClanTag];
            SetClanColor(playerid);

            new query[144];
            mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET hide_clan_tag = %d WHERE id = %d", User[playerid][hideClanTag], User[playerid][user_sqlid]);
            mysql_tquery(dbHandle, query);

            va_SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}Clan Tag is now %s{FFFFFF}.", User[playerid][hideClanTag] ? "{FF6347}Hidden" : "{23C552}Visible");
            ShowTagMenu(playerid);
        }
        case TAG_TYPE_ADMIN:
        {
            if(User[playerid][adminLevel] <= 0) return ShowTagMenu(playerid);

            User[playerid][hideAdminTag] = !User[playerid][hideAdminTag];
            SetClanColor(playerid);

            new query[144];
            mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET hide_admin_tag = %d WHERE id = %d", User[playerid][hideAdminTag], User[playerid][user_sqlid]);
            mysql_tquery(dbHandle, query);

            va_SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}Admin Tag is now %s{FFFFFF}.", User[playerid][hideAdminTag] ? "{FF6347}Hidden" : "{23C552}Visible");
            ShowTagMenu(playerid);
        }
        case TAG_TYPE_CUSTOM:
        {
            if(!CanPlayerUseCustomTag(playerid))
            {
                SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
                return ShowTagMenu(playerid);
            }

            if(User[playerid][customTag][0] == '\0')
            {
                return ShowCustomTagOptionsMenu(playerid);
            }

            User[playerid][hideCustomTag] = !User[playerid][hideCustomTag];
            SaveCustomTag(playerid);
            SetClanColor(playerid);

            va_SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}Custom Tag is now %s{FFFFFF}.", User[playerid][hideCustomTag] ? "{FF6347}Hidden" : "{23C552}Visible");
            ShowTagMenu(playerid);
        }
        case TAG_TYPE_EDIT_CUSTOM:
        {
            if(!CanPlayerUseCustomTag(playerid))
            {
                SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
                return ShowTagMenu(playerid);
            }
            return ShowCustomTagOptionsMenu(playerid);
        }
    }
    return 1;
}

stock ShowCustomTagOptionsMenu(playerid)
{
    if(!CanPlayerUseCustomTag(playerid))
    {
        SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
        return ShowTagMenu(playerid);
    }

    new options[256];
    format(options, sizeof(options), "1. Choose from Predefined List\n2. Enter Custom Tag Text (Max %d Chars)\n3. Remove Custom Tag", CUSTOM_TAG_MAX_LEN);

    new title[64] = "{MAIN}Custom Tag Options";
    format_MainColor(playerid, title);

    return Dialog_ShowCallback(playerid, using public DIALOG_CUSTOM_TAG_OPTIONS<iiiis>, DIALOG_STYLE_LIST, title, options, "Select", "Back");
}

static stock HandleCustomTagRemoval(playerid)
{
    if(User[playerid][customTag][0] == '\0')
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You do not have a Custom Tag set to remove.");
        return ShowTagMenu(playerid);
    }

    ClearCustomTag(playerid);
    return ShowTagMenu(playerid);
}

forward DIALOG_CUSTOM_TAG_OPTIONS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_CUSTOM_TAG_OPTIONS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowTagMenu(playerid);

    if(!CanPlayerUseCustomTag(playerid))
    {
        SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
        return ShowTagMenu(playerid);
    }

    switch(listitem)
    {
        case 0: // Predefined list
        {
            new listStr[512] = "";
            for(new i = 0; i < sizeof(g_PredefinedTags); i++)
            {
                format(listStr, sizeof(listStr), "%s%d. %s\n", listStr, i + 1, g_PredefinedTags[i]);
            }
            new title[64] = "{MAIN}Predefined Tags";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_PREDEFINED_TAGS<iiiis>, DIALOG_STYLE_LIST, title, listStr, "Select", "Back");
        }
        case 1: // Custom input
        {
            new title[64] = "{MAIN}Custom Tag";
            format_MainColor(playerid, title);

            new body[192];
            format(body, sizeof(body), "{FFFFFF}Enter your Custom Tag text (%d to %d characters):\n{B4B5B7}(Letters, numbers and basic symbols only - no brackets [] required)",
                CUSTOM_TAG_MIN_LEN, CUSTOM_TAG_MAX_LEN);

            return Dialog_ShowCallback(playerid, using public DIALOG_EDIT_CUSTOM_TAG<iiiis>, DIALOG_STYLE_INPUT, title, body, "Save", "Back");
        }
        case 2: // Remove
        {
            return HandleCustomTagRemoval(playerid);
        }
    }
    return 1;
}

forward DIALOG_PREDEFINED_TAGS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_PREDEFINED_TAGS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowCustomTagOptionsMenu(playerid);

    if(!CanPlayerUseCustomTag(playerid))
    {
        SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
        return ShowTagMenu(playerid);
    }

    if(listitem < 0 || listitem >= sizeof(g_PredefinedTags)) return ShowTagMenu(playerid);

    ApplyCustomTag(playerid, g_PredefinedTags[listitem]);
    return ShowTagMenu(playerid);
}

forward DIALOG_EDIT_CUSTOM_TAG(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_EDIT_CUSTOM_TAG(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowTagMenu(playerid);

    if(!CanPlayerUseCustomTag(playerid))
    {
        SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
        return ShowTagMenu(playerid);
    }

    if(isnull(inputtext) || strcmp(inputtext, "none", true) == 0)
    {
        return HandleCustomTagRemoval(playerid);
    }

    new cleanTag[32];
    if(!SanitizeCustomTag(inputtext, cleanTag, sizeof(cleanTag)))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid Custom Tag. Use %d to %d normal characters - colour codes and special symbols are not allowed.",
            CUSTOM_TAG_MIN_LEN, CUSTOM_TAG_MAX_LEN);
        return ShowTagMenu(playerid);
    }

    ApplyCustomTag(playerid, cleanTag);
    return ShowTagMenu(playerid);
}

