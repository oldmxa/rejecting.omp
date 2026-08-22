
stock ShowDiscordLinkDialog(playerid)
{
    new strC[512], title[64] = "{MAIN}Link Discord Account";
    format_MainColor(playerid, title);

    format(strC, sizeof(strC),
"{FFFFFF}Join our Discord server: {MAIN}%s{FFFFFF}\n\n\
Enter your Discord username or ID below to receive a verification code:\n\n\
{BEBEBE}Examples: {MAIN}username{BEBEBE} or {MAIN}username#0000{BEBEBE} or {MAIN}Discord ID", ServerHostname);
    format_MainColor(playerid, strC);

    return Dialog_ShowCallback(playerid, using public DIALOG_LINK_DISCORD<iiiis>, DIALOG_STYLE_INPUT, title, strC, "Confirm", "Cancel");
}

forward DIALOG_LINK_DISCORD(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_LINK_DISCORD(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!User[playerid][isLogged]) return 1;

    if(response)
    {
        if(IsNull(inputtext))
		{
			SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}Username cannot be empty. Please enter your Discord username.");
            ShowDiscordLinkDialog(playerid);
			return true;
		}

        new output[2][100];
        strexplode(output, inputtext, "#");

        new DCC_User:user, userid[DCC_ID_SIZE];

        if(IsNull(output[1]) || !strcmp(output[1], "0") || !strcmp(output[1], "0000"))
        {
             user = DCC_FindUserById(output[0]);
             if(user == DCC_INVALID_USER) 
             {
                 user = DCC_FindUserByName(output[0], "0");
                 if(user == DCC_INVALID_USER)
                 {
                     user = DCC_FindUserByName(output[0], "0000");
                     if(user == DCC_INVALID_USER)
                     {
                         new lowerName[100];
                         for(new i=0; i < sizeof(lowerName) && output[0][i] != '\0'; i++) lowerName[i] = tolower(output[0][i]);
                         user = DCC_FindUserByName(lowerName, "0");
                     }
                 }
             }
        }
        else
        {
             user = DCC_FindUserByName(output[0], output[1]);
             if(user == DCC_INVALID_USER)
             {
                 new lowerName[100];
                 for(new i=0; i < sizeof(lowerName) && output[0][i] != '\0'; i++) lowerName[i] = tolower(output[0][i]);
                 user = DCC_FindUserByName(lowerName, output[1]);
             }
        }

        if (user != DCC_INVALID_USER)
		{
            DCC_GetUserId(user, userid);

            inline const OnDiscordAvailable()
            {
                format(User[playerid][dc_code], 100, "%d%d%d%d%d", random(10), random(10), random(10), random(10), random(10));
                format(User[playerid][dc_username], 100, "%s", output[0]);
                format(User[playerid][dc_discriminator], 100, "%s", output[1]);
                format(User[playerid][dc_uid], 100, "%s", userid);

                DCC_CreatePrivateChannel(user, "MSGVALIDATION", "is", playerid, User[playerid][dc_code]);
                User[playerid][dc_codetimer] = gettime()+(2*60);

                new strC[512], title[64] = "{MAIN}Discord Verification";
                format_MainColor(playerid, title);

                format(strC, sizeof(strC),
"{FFFFFF}A 5-digit verification code was sent to your Discord PM.\n\n\
Discord Account: {MAIN}%s#%s{FFFFFF}\n\n\
Enter the code below to complete linking:",
                    User[playerid][dc_username], User[playerid][dc_discriminator]);
                format_MainColor(playerid, strC);

                Dialog_ShowCallback(playerid, using public DIALOG_VALI_DISCORD<iiiis>, DIALOG_STYLE_INPUT, title, strC, "Confirm", "Cancel");
            }
            CheckDiscordUser(playerid, output[0], userid, using inline OnDiscordAvailable);

		    return true;
		}
		else
		{
            SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}This Discord user was not found on our Discord server.");
            ShowDiscordLinkDialog(playerid);
		    return true;
		}
    }
    else
    {
        format(User[playerid][dc_username], 100, "-1");
        format(User[playerid][dc_discriminator], 100, "-1");
        format(User[playerid][dc_uid], 100, "-1");
        format(User[playerid][dc_code], 100, "-1");
    }
    return true;
}

forward DIALOG_VALI_DISCORD(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_VALI_DISCORD(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!User[playerid][isLogged]) return 1;

    if(response)
    {
        if(IsNull(inputtext))
		{
            FailValidation(playerid);
            return 1;
        }

        if(gettime() > User[playerid][dc_codetimer])
        {
            FailValidation(playerid);
            return 1;
        }

        new DCC_User:user;
        user = DCC_FindUserById(User[playerid][dc_uid]);

        if (user != DCC_INVALID_USER)
		{
		    if(strequal(inputtext, User[playerid][dc_code]))
            {
                SendClientMessage(playerid, COLOR_KGREEN, "%s", DC_LINKSUCCESS);

			    mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "UPDATE `accounts` SET `dc_username` = '%e', `dc_discriminator` = '%e', `dc_uid` = '%e' WHERE `id` = %d", User[playerid][dc_username], User[playerid][dc_discriminator], User[playerid][dc_uid], User[playerid][user_sqlid]);
	            mysql_tquery(dbHandle, queryGlobal);

                SyncPlayerDiscordRoles(playerid);

                DCC_SetGuildMemberNickname(guildaid, user, getPlayerName(playerid));

                LogDiscordVerification(playerid);

                CallLocalFunction("Ach_OnDiscordLink", "i", playerid);
            }
            else
            {
                FailValidation(playerid);
            }
		    return true;
		}
		else
		{
            FailValidation(playerid);
		    return true;
		}
    }
    else
    {
        FailValidation(playerid);
    }
    return true;
}

forward DIALOG_UNLINK_DISCORD(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_UNLINK_DISCORD(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!User[playerid][isLogged]) return 1;

    if(response)
    {
        if(!strcmp(User[playerid][dc_username], "-1", true))
            return SendClientMessage(playerid, COLOR_TOMATO, "%s", DC_NOTLINKED);

        mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "UPDATE `accounts` SET `dc_username` = '-1', `dc_discriminator` = '-1', `dc_uid` = '-1' WHERE `id` = %d", User[playerid][user_sqlid]);
        mysql_tquery(dbHandle, queryGlobal);

        ClearPlayerDiscordRoles(playerid);
        SendClientMessage(playerid, COLOR_KGREEN, "%s", DC_UNLINKSUCESS);

        format(User[playerid][dc_username], 100, "-1");
        format(User[playerid][dc_discriminator], 100, "-1");
        format(User[playerid][dc_uid], 100, "-1");
        format(User[playerid][dc_code], 100, "-1");
    }
    return 1;
}
