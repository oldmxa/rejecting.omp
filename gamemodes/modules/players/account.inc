
@cmd() changenick(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", NOT_LOGGED_ERROR);

    if(IsPlayerFakingNick(playerid))
        return SendClientMessage(playerid, -1, "%s", NICK_CHANGE_WHILE_FAKE);

    Dialog_ShowCallback(playerid, using public CHANGE_NICK<iiiis>, DIALOG_STYLE_INPUT, "Changing Nickname", sprintf("{FFFFFF}Price: {007129}%s\n{FFFFFF}Put your new nickname below:", FormatMoney(300000)), "Change", "Exit");
    return true;
}

@cmd() changepass(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", NOT_LOGGED_ERROR);

    new pass[61 + 1];
    if(sscanf(params, "s[61]", pass))
        return SendClientMessage(playerid, -1, "%s", ERROR_CHANGEPASS);

    if(!IsValidPassword(pass))
    {
        return va_SendClientMessage(playerid, -1, ERROR_INVALIDPASS, MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH);
    }

    bcrypt_hash(playerid, "OnPlayerChangePass", pass, BCRYPT_COST);
    return true;
}
/*
@cmd(.alts = {"novaarena"}) newarena(playerid, params[], help)
{
    if (!User[playerid][isLogged])
    {
        return SendClientMessage(playerid, -1, "%s", NOT_LOGGED_ERROR);
    }
    if (!User[playerid][isAfk])
    {
        return SendClientMessage(playerid, -1, "%s", NEED_BE_AFK);
    }
    if (User[playerid][isCreatingArena])
    {
        User[playerid][isCreatingArena] = false;

        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, true);
        ClearPlayerNewArenaData(playerid);

        return SendClientMessage(playerid, -1, "%s", NEW_ARENA_EXIT);
    }

    User[playerid][isCreatingArena] = true;

    SendClientMessage(playerid, -1, "%s", NEW_ARENA_INIT);
    SendClientMessage(playerid, -1, "%s", NEW_ARENA_INFO);

    return true;
}
*/
@cmd(.alts = {"linkdiscord", "discord", "dc"}) dvincular(playerid, params[], help)
{
	if(playerid == INVALID_PLAYER_ID || !User[playerid][isLogged])
        return false;
        
    if(strcmp(User[playerid][dc_username], "-1", true))
        return va_SendClientMessage(playerid, COLOR_TOMATO, DC_LINKED, User[playerid][dc_username], User[playerid][dc_discriminator]);

    if(User[playerid][dc_codetimer] > gettime())
        return SendClientMessage(playerid, COLOR_TOMATO, "%s", DC_COOLDOWN);

    new strC[512], title[64] = "{MAIN}Link Discord Account";
    format_MainColor(playerid, title);

    format(strC, sizeof(strC),
"{FFFFFF}Join our Discord server: {MAIN}%s{FFFFFF}\n\n\
Enter your Discord username or ID below to receive a verification code:\n\n\
{BEBEBE}Examples: {MAIN}username{BEBEBE} or {MAIN}username#0000{BEBEBE} or {MAIN}Discord ID", ServerHostname);
    format_MainColor(playerid, strC);

    Dialog_ShowCallback(playerid, using public DIALOG_LINK_DISCORD<iiiis>, DIALOG_STYLE_INPUT, title, strC, "Confirm", "Cancel");
    return true;
}

@cmd(.alts = {"unlinkdiscord"}) udiscord(playerid, params[], help)
{
	if(playerid == INVALID_PLAYER_ID || !User[playerid][isLogged]) return false;

    if(!strcmp(User[playerid][dc_username], "-1", true))
        return SendClientMessage(playerid, COLOR_TOMATO, "%s", DC_NOTLINKED);

    new title[64] = "{MAIN}Unlink Discord Account";
    format_MainColor(playerid, title);

    new body[256];
    format(body, sizeof(body), "{FFFFFF}Are you sure you want to unlink your Discord account ({MAIN}%s#%s{FFFFFF})?", User[playerid][dc_username], User[playerid][dc_discriminator]);
    format_MainColor(playerid, body);

    Dialog_ShowCallback(playerid, using public DIALOG_UNLINK_DISCORD<iiiis>, DIALOG_STYLE_MSGBOX, title, body, "Unlink", "Cancel");
    return true;
}

ShowMyAccountMenu(playerid)
{
    new accStr[512], title[64];
    format(title, sizeof(title), "{MAIN}My Account");
    format_MainColor(playerid, title);

    format(accStr, sizeof(accStr), "{MAIN}Option\t{MAIN}Description\n");
    strcat(accStr, "Settings\t{FFFFFF}Configure preferences, sound & HUD toggles\n");
    strcat(accStr, "Personalization\t{FFFFFF}Customize skin, hitmark, colors & sounds\n");
    strcat(accStr, "My Stats\t{FFFFFF}View your detailed account statistics\n");
    strcat(accStr, "Discord ID\t{FFFFFF}View your linked Discord ID & status");
    format_MainColor(playerid, accStr);

    Dialog_ShowCallback(playerid, using public DIALOG_HELP_MYACCOUNT<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, accStr, "Select", "Back");
    return 1;
}

ShowSecurityMenu(playerid)
{
    new secStr[512];
    format(secStr, sizeof(secStr), "{MAIN}Option\t{MAIN}Status / Info\n");

    if(strcmp(User[playerid][dc_username], "-1", true)) 
        strcat(secStr, fmat_return("Discord Account\t{00FF00}%s\n", User[playerid][dc_username]));
    else 
        strcat(secStr, "Discord Account\t{FF0000}Not Linked\n");

    new bool:isLinked = (strcmp(User[playerid][dc_username], "-1", true) != 0);
    if(isLinked)
    {
        strcat(secStr, fmat_return("Auto-Login (IP & Serial)\t%s\n", (User[playerid][autoLogin]) ? ("{00FF00}ON") : ("{FF0000}OFF")));
    }
    else
    {
        strcat(secStr, "Auto-Login (IP & Serial)\t{BEBEBE}Requires Linked Discord\n");
    }

    if(User[playerid][adminLevel] > 0)
    {
        strcat(secStr, fmat_return("Admin Authentication\t%s\n", (User[playerid][adminAuthType] == 1) ? ("{00FF00}Password (Login)") : ("{00FF00}Code (Discord)")));
    }

    new title[64] = "{MAIN}Security Settings";
    format_MainColor(playerid, title);
    format_MainColor(playerid, secStr);

    Dialog_ShowCallback(playerid, using public DIALOG_HELP_SECURITY<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, secStr, "Select", "Back");
    return 1;
}

ShowSettingsMenu(playerid)
{
    new setStr[1024], title[64];
    format(title, sizeof(title), "{MAIN}Account & Gameplay Settings");
    format_MainColor(playerid, title);

    format(setStr, sizeof(setStr), "{MAIN}Option\t{MAIN}Status / Value\n");
    strcat(setStr, "Change my username\t \n");
    strcat(setStr, "Change my password\t \n");
    strcat(setStr, fmat_return("Incoming messages\t%s\n", (User[playerid][blockPM]) ? ("{FF0000}OFF") : ("{00FF00}ON")));
    strcat(setStr, fmat_return("Connect info\t%s\n", (User[playerid][showConnectMsg]) ? ("{00FF00}ON") : ("{FF0000}OFF")));
    strcat(setStr, fmat_return("Global messages\t%s", (User[playerid][hideGlobalMsg]) ? ("{F84F31}Disabled") : ("{23C552}Enabled")));
    format_MainColor(playerid, setStr);

    Dialog_ShowCallback(playerid, using public DIALOG_HELP_SETTINGS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, setStr, "Select", "Back");
    return 1;
}

forward DIALOG_HELP_SETTINGS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HELP_SETTINGS(playerid, dialogid, response, listitem, string:inputtext[])
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
            
            ShowSettingsMenu(playerid);
        }
        case 3:
        {
            User[playerid][showConnectMsg] = !User[playerid][showConnectMsg];
            ShowSettingsMenu(playerid);
        }
        case 4:
        {
            User[playerid][hideGlobalMsg] = !User[playerid][hideGlobalMsg];
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowSettingsMenu(playerid);
        }
    }
    return 1;
}

forward DIALOG_CHANGE_PASSWORD(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_CHANGE_PASSWORD(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowSettingsMenu(playerid);

    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", NOT_LOGGED_ERROR);

    if(isnull(inputtext))
        return SendClientMessage(playerid, -1, "%s", ERROR_CHANGEPASS);

    if(!IsValidPassword(inputtext))
        return va_SendClientMessage(playerid, -1, ERROR_INVALIDPASS, MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH);

    bcrypt_hash(playerid, "OnPlayerChangePass", inputtext, BCRYPT_COST);
    return 1;
}

forward DIALOG_HELP_DISCORDID(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HELP_DISCORDID(playerid, dialogid, response, listitem, string:inputtext[])
{
    return ShowMyAccountMenu(playerid);
}

ShowPersonalizationMenu(playerid)
{
    new pStr[2048];
    format(pStr, sizeof(pStr), "{MAIN}Preference\t{MAIN}Status / Value\n");

    format(pStr, sizeof(pStr), "%s{B4B5B7}World Time\t{FFFFFF}%02d:00\n", pStr, User[playerid][time]);
    format(pStr, sizeof(pStr), "%s{B4B5B7}Weather ID\t{FFFFFF}%d\n", pStr, User[playerid][weather]);
    format(pStr, sizeof(pStr), "%s{B4B5B7}Player HUD\t%s\n", pStr, (User[playerid][showPlayerHud]) ? ("{23C552}Visible") : ("{F84F31}Hidden"));
    format(pStr, sizeof(pStr), "%s{B4B5B7}Clan HUD\t%s\n", pStr, (User[playerid][showClanHud]) ? ("{23C552}Visible") : ("{F84F31}Hidden"));

    format(pStr, sizeof(pStr), "%s{B4B5B7}Damage Informer\t{447DBF}%s\n", pStr, GetDamageStyleName(User[playerid][dmgInformerStyle]));
    format(pStr, sizeof(pStr), "%s{B4B5B7}Hitmarker Color\t{%06x}######\n", pStr, (User[playerid][hitmarkColor] >>> 8));

    new hudIdx = GetPlayerHudColorIndex(playerid);

    new name[24];
    format(name, sizeof(name), "%s", hudPersonalization[hudIdx][lineInfo]);
    if(name[0] >= 'a' && name[0] <= 'z') name[0] -= 32;
    new space = strfind(name, " ");
    if(space != -1 && name[space+1] >= 'a' && name[space+1] <= 'z') name[space+1] -= 32;

    new idHex[7], textHex[7];
    GetPlayerChatIDColorHex(playerid, idHex);
    GetPlayerChatTextColorHex(playerid, textHex);

    format(pStr, sizeof(pStr), "%s{B4B5B7}Interface Color\t{%s}%s\n", pStr, GetColorHexTag(hudPersonalization[hudIdx][colorOne]), name);

    new shadeOne = GetPlayerShadeIndex(playerid, false), shadeTwo = GetPlayerShadeIndex(playerid, true);

    if(shadeOne == -1) format(pStr, sizeof(pStr), "%s{B4B5B7}Primary Color\t{BEBEBE}Interface preset\n", pStr);
    else format(pStr, sizeof(pStr), "%s{B4B5B7}Primary Color\t{%06x}%s\n", pStr, hudShades[shadeOne][shadeHex], hudShades[shadeOne][shadeName]);

    if(shadeTwo == -1) format(pStr, sizeof(pStr), "%s{B4B5B7}Secondary Color\t{BEBEBE}Interface preset\n", pStr);
    else format(pStr, sizeof(pStr), "%s{B4B5B7}Secondary Color\t{%06x}%s\n", pStr, hudShades[shadeTwo][shadeHex], hudShades[shadeTwo][shadeName]);

    format(pStr, sizeof(pStr), "%s{B4B5B7}Chat Color\t{%s}ID: #%s {FFFFFF}/ {%s}Text: #%s\n", pStr, idHex, idHex, textHex, textHex);

    format(pStr, sizeof(pStr), "%s{B4B5B7}Character Skin\t{FFFFFF}ID:%d\n", pStr, GetPlayerSkin(playerid));
    format(pStr, sizeof(pStr), "%s{B4B5B7}Fighting Style\t{FFFFFF}ID:%d\n", pStr, _:User[playerid][fightStyle]);
    format(pStr, sizeof(pStr), "%s{B4B5B7}Weapon Set\t{FFFFFF}Change\n", pStr);

    new hudTypeIdx = User[playerid][hudType];
    if(hudTypeIdx < 0 || hudTypeIdx >= MAX_HUD_TYPES) hudTypeIdx = HUD_REJECTING;

    format(pStr, sizeof(pStr), "%s{B4B5B7}HUD Style\t{FFFFFF}%s\n", pStr, HUD_TYPES[hudTypeIdx]);
    format(pStr, sizeof(pStr), "%s{B4B5B7}3D Death Tag\t%s\n", pStr, (User[playerid][show3DDeathTag]) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
    format(pStr, sizeof(pStr), "%s{B4B5B7}Random Death Tags\t%s\n", pStr, (User[playerid][showRandomDeathTags]) ? ("{23C552}Enabled") : ("{F84F31}Disabled"));
    format(pStr, sizeof(pStr), "%s{B4B5B7}Headshot Text\t%s\n", pStr, (User[playerid][showHeadshot]) ? ("{23C552}Visible") : ("{F84F31}Hidden"));
    format(pStr, sizeof(pStr), "%s{B4B5B7}Hit Sound\t{FFFFFF}ID:%d (%s)\n", pStr, User[playerid][hitSoundID], GetSoundName(User[playerid][hitSoundID]));
    format(pStr, sizeof(pStr), "%s{B4B5B7}Kill Sound\t{FFFFFF}ID:%d (%s)\n", pStr, User[playerid][killSoundID], GetSoundName(User[playerid][killSoundID]));
    format(pStr, sizeof(pStr), "%s{B4B5B7}Welcome Messages\t%s\n", pStr, (User[playerid][hideWelcomeMsg]) ? ("{F84F31}Hidden") : ("{23C552}Visible"));

    new lifeName[24];
    GetLifeStyleName(User[playerid][lifeType], lifeName);
    format(pStr, sizeof(pStr), "%s{B4B5B7}Life Style\t{FFFFFF}%s", pStr, lifeName);

    new pTitle[64] = "{MAIN}Personalization Menu";
    format_MainColor(playerid, pTitle);
    format_MainColor(playerid, pStr);

    Dialog_ShowCallback(playerid, using public DIALOG_HELP_PERSONALIZATION<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, pTitle, pStr, "Select", "Back");
    return 1;
}

forward DIALOG_HELP_MYACCOUNT(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HELP_MYACCOUNT(playerid, dialogid, response, listitem, string:inputtext[])
{
    new cmd[32];
    if(!response)
    {
        strcopy(cmd, "/help");
        return Command_ReProcess(playerid, cmd, 0);
    }

    switch(listitem)
    {
        case 0:
        {
            ShowSettingsMenu(playerid);
        }
        case 1:
        {
            ShowPersonalizationMenu(playerid);
        }
        case 2:
        {
            strcopy(cmd, "/stats");
            Command_ReProcess(playerid, cmd, 0);
        }
        case 3:
        {
            new dcIDStr[256];
            if(strcmp(User[playerid][dc_username], "-1", true))
            {
                format(dcIDStr, sizeof(dcIDStr), "Username: %s\nDiscriminator: %s\nID: %s", User[playerid][dc_username], User[playerid][dc_discriminator], User[playerid][dc_uid]);
            }
            else
            {
                format(dcIDStr, sizeof(dcIDStr), "Account not linked to Discord.");
            }
            new dcTitle[64] = "{MAIN}Discord ID Info";
            format_MainColor(playerid, dcTitle);
            Dialog_ShowCallback(playerid, using public DIALOG_HELP_DISCORDID<iiiis>, DIALOG_STYLE_MSGBOX, dcTitle, dcIDStr, "Close", "Back");
        }
    }
    return 1;
}

forward DIALOG_HELP_SECURITY(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HELP_SECURITY(playerid, dialogid, response, listitem, string:inputtext[])
{
    new cmd[32];
    if(!response)
    {
        strcopy(cmd, "/help");
        return Command_ReProcess(playerid, cmd, 0);
    }

    switch(listitem)
    {
        case 0:
        { 
            if(strcmp(User[playerid][dc_username], "-1", true)) strcopy(cmd, "/udiscord"); 
            else strcopy(cmd, "/dvincular");
            
            Command_ReProcess(playerid, cmd, 0); 
        }
        case 1:
        {
            if(!strcmp(User[playerid][dc_username], "-1", true))
            {
                SendClientMessage(playerid, -1, "{MAIN}[SECURITY] {FFFFFF}You must link your Discord account first to enable Auto-Login!");
                ShowSecurityMenu(playerid);
                return 1;
            }

            User[playerid][autoLogin] = !User[playerid][autoLogin];

            mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `accounts` SET `auto_login` = %d WHERE `id` = %d", User[playerid][autoLogin], User[playerid][user_sqlid]);
            mysql_tquery(dbHandle, queryGlobal);

            va_SendClientMessage(playerid, -1, "{MAIN}[SECURITY] {FFFFFF}Auto-Login (IP & Serial) is now %s{FFFFFF}.", (User[playerid][autoLogin]) ? ("{00FF00}ENABLED") : ("{FF0000}DISABLED"));
            ShowSecurityMenu(playerid);
        }
        case 2:
        {
            if(User[playerid][adminLevel] < 1)
            {
                ShowSecurityMenu(playerid);
                return 1;
            }

            ShowAdminAuthMethodDialog(playerid);
        }
    }
    return 1;
}

forward DIALOG_HELP_RULES(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HELP_RULES(playerid, dialogid, response, listitem, string:inputtext[])
{
    return ShowHelpMainMenu(playerid);
}

forward DIALOG_HELP_MAIN(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HELP_MAIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return 1;

    switch(listitem)
    {
        case 0:
        {
            ShowMyAccountMenu(playerid);
        }
        case 1:
        {
            ShowSecurityMenu(playerid);
        }
        case 2: StartChangeMode(playerid);
        case 3:
        {
            ShowTrainingHelpDialog(playerid);
        }
        case 4:
        {
            ShowVipMenu(playerid);
        }
        case 5: ShowRankingsMenu(playerid);
        case 6:
        {
            new rulesStr[2048];
            strcat(rulesStr, ""COL_RED"RULES\n\n"COL_WHITE"", sizeof(rulesStr));

            for(new r = 0; r < sizeof(g_ServerRules); r++)
                strcat(rulesStr, fmat_return("%d. %s\n", r + 1, g_ServerRules[r]), sizeof(rulesStr));

            strcat(rulesStr, "\n"COL_GRAY"Note:\n\n", sizeof(rulesStr));
            strcat(rulesStr, g_ServerRulesNote, sizeof(rulesStr));

            new rulesTitle[64] = "{MAIN}Server Rules & Guidelines";
            format_MainColor(playerid, rulesTitle);
            Dialog_ShowCallback(playerid, using public DIALOG_HELP_RULES<iiiis>, DIALOG_STYLE_MSGBOX, rulesTitle, rulesStr, "Back", "");
        }
        case 7: ShowPlayerCommands(playerid);
    }
    return 1;
}
