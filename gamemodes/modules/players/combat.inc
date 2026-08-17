
rSpree(killerid, playerid)
{
	new Stringa[156], bool:firstBlood = false;

	new victimLoc = User[playerid][locationId];
	if(0 <= victimLoc < MAX_LOCATIONS)
		firstBlood = (arenaInfo[victimLoc][totalArenaKills] < 1);

    new spreeColor[20];
    new hudIdx = GetPlayerHudColorIndex(killerid);
    if (GetPlayerShadeIndex(killerid, false) != -1 || hudIdx != 0)
    {
        format(spreeColor, sizeof(spreeColor), "%s", GetPlayerHudTag(killerid, false));
    }
    else
    {
        spreeColor[0] = '\0';
    }

	switch(firstBlood)
	{
		case true:
		{
			format(Stringa, sizeof(Stringa), "%sFirst Blood!", (spreeColor[0] == '\0') ? ("~r~~h~") : (spreeColor));

			foreach(new i : Player)
			{
				if(User[i][locationId] == victimLoc)
					va_SendClientMessage(i, -1, FIRST_BLOOD, (GetPlayerColor(killerid) >>> 8), getPlayerName(killerid));
			}
	    }

		default:
		{
        switch (GetPVarInt(killerid, "Spree")) 
        {

            case 1: {
                format(Stringa, sizeof(Stringa), "%sKill_Streak!", (spreeColor[0] == '\0') ? ("~w~~h~") : (spreeColor));
            }
            case 2: {
                format(Stringa, sizeof(Stringa), "%sDouble_Kill!", (spreeColor[0] == '\0') ? ("~w~~h~") : (spreeColor));
            }
            case 3: {
                format(Stringa, sizeof(Stringa), "%sTriple_Kill!", (spreeColor[0] == '\0') ? ("~w~~h~") : (spreeColor));
            }
            case 4: {
                format(Stringa, sizeof(Stringa), "%sQuadro_Kill!", (spreeColor[0] == '\0') ? ("~w~~h~") : (spreeColor));
            }
            case 5: {
                format(Stringa, sizeof(Stringa), "%sUltra_Kill!!", (spreeColor[0] == '\0') ? ("~r~~h~") : (spreeColor));
            }
            case 6: {
                format(Stringa, sizeof(Stringa), "%sMega_Kill!", (spreeColor[0] == '\0') ? ("~r~~h~") : (spreeColor));
            }
            case 7: {
                format(Stringa, sizeof(Stringa), "%sMonster_Kill!", (spreeColor[0] == '\0') ? ("~r~~h~") : (spreeColor));
            }
            case 8: {
                format(Stringa, sizeof(Stringa), "%sRampage!", (spreeColor[0] == '\0') ? ("~r~~h~") : (spreeColor));
                va_SendClientMessageToAll(-1, "{MAIN}%s - RAMPAGE!", getPlayerName(killerid));
            }
            case 9: {
                format(Stringa, sizeof(Stringa), "%sDominating!", (spreeColor[0] == '\0') ? ("~r~~h~") : (spreeColor));
            }
            case 10: {
                format(Stringa, sizeof(Stringa), "%sUnstoppable!", (spreeColor[0] == '\0') ? ("~y~~h~") : (spreeColor));
            }
            case 11: {
                format(Stringa, sizeof(Stringa), "%sWicked Sick!", (spreeColor[0] == '\0') ? ("~y~~h~") : (spreeColor));
            }
            case 12: {
                format(Stringa, sizeof(Stringa), "%sGodlike!", (spreeColor[0] == '\0') ? ("~y~~h~") : (spreeColor));
            }
            case 13: {
                format(Stringa, sizeof(Stringa), "%sExcellent!", (spreeColor[0] == '\0') ? ("~y~~h~") : (spreeColor));
            }
            case 14: {
                format(Stringa, sizeof(Stringa), "%sGodfather!", (spreeColor[0] == '\0') ? ("~y~~h~") : (spreeColor));
            }
            case 15: {
                format(Stringa, sizeof(Stringa), "%sBeyond Godlike!", (spreeColor[0] == '\0') ? ("~y~~h~") : (spreeColor));
            }
            case 16: {
                format(Stringa, sizeof(Stringa), "%sHigh Roller", (spreeColor[0] == '\0') ? ("~y~~h~") : (spreeColor));
            }
            case 17: {
                format(Stringa, sizeof(Stringa), "%sPogghamp!", (spreeColor[0] == '\0') ? ("~y~~h~") : (spreeColor));
            }
            case 18: {
                format(Stringa, sizeof(Stringa), "%sUnbeatable!", (spreeColor[0] == '\0') ? ("~y~~h~") : (spreeColor));
            }
            default: {
                format(Stringa, sizeof(Stringa), "%sLegendary!", (spreeColor[0] == '\0') ? ("~y~~h~") : (spreeColor));
            }
				}
		}
	}
	PlayerTextDrawSetString(killerid, killsandexp[killerid], Stringa);
	if(User[killerid][showPlayerHud]) PlayerTextDrawShow(killerid, killsandexp[killerid]);
	SetTimerEx("hideKillTD", 3300, false, "i", killerid);
}

RandomDeathMessages(killerid, playerid, reason)
{
	new newline[64], fullLine[64*6];

	new playerColorOne[20], playerColorTwo[20];
	format(playerColorOne, sizeof playerColorOne, "%s", GetPlayerHudTag(killerid, false));
	format(playerColorTwo, sizeof playerColorTwo, "%s", GetPlayerHudTag(killerid, true));

	switch(reason)
	{
			case 8: format(newline, sizeof(newline), "~n~%sYou kataned %s%s ~n~",
			playerColorOne, playerColorTwo, getPlayerName(playerid));

			case 16: format(newline, sizeof(newline), "~n~%sYou bombed %s%s ~n~",
			playerColorOne, playerColorTwo, getPlayerName(playerid));

			default:
			{
					switch(random(20))
					{
							case 0: format(newline, sizeof(newline), "%sYou sent %s%s %sto cemetery~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid), playerColorOne);

							case 1: format(newline, sizeof(newline), "%sYou destroyed %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 2: format(newline, sizeof(newline), "%sYou dominated %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 3: format(newline, sizeof(newline), "%sYou killed %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 4: format(newline, sizeof(newline), "%sYou owned %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 5: format(newline, sizeof(newline), "%sYou took %s%s%s's life~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid), playerColorOne);

							case 6: format(newline, sizeof(newline), "%sYou eliminated %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 7: format(newline, sizeof(newline), "%sYou buried %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 8: format(newline, sizeof(newline), "%sYou smoked %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 9: format(newline, sizeof(newline), "%sYou erased %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 10: format(newline, sizeof(newline), "%sYou clapped %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 11: format(newline, sizeof(newline), "%sYou wrecked %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 12: format(newline, sizeof(newline), "%sYou annihilated %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 13: format(newline, sizeof(newline), "%sYou executed %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 14: format(newline, sizeof(newline), "%sYou humiliated %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 15: format(newline, sizeof(newline), "%sYou wiped out %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 16: format(newline, sizeof(newline), "%sYou crushed %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 17: format(newline, sizeof(newline), "%sYou terminated %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 18: format(newline, sizeof(newline), "%sYou deleted %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));

							case 19: format(newline, sizeof(newline), "%sYou dismantled %s%s~n~",
							playerColorOne, playerColorTwo, getPlayerName(playerid));
					}
			}
	}

	if(lines[killerid] >= MAX_LINES)
	{
		for(new i = 0; i < MAX_LINES - 1; i++)
			StringToCat[killerid][i] = StringToCat[killerid][i + 1];

		lines[killerid] = MAX_LINES - 1;
	}

	StringToCat[killerid][lines[killerid]][0] = EOS;
	strcat(StringToCat[killerid][lines[killerid]], newline, 64 + 1);
	lines[killerid]++;

	for(new i = 0; i < lines[killerid]; i++)
		strcat(fullLine, StringToCat[killerid][i], sizeof(fullLine));

	PlayerTextDrawSetString(killerid, DeathInfo[killerid][0], fullLine);

	if(User[killerid][showPlayerHud]) PlayerTextDrawShow(killerid, DeathInfo[killerid][0]);
	User[killerid][timerBox] = gettime()+8;
	User[killerid][isShowingKSTD] = true;
	return true;
}

UpdateKillstreakBox(playerid)
{
	if(lines[playerid] > 0)
	{
		new strTemp[64*6];

		for(new i = 0; i < MAX_LINES - 1; i++)
			StringToCat[playerid][i] = StringToCat[playerid][i+1];

		StringToCat[playerid][MAX_LINES - 1] = "_";
		lines[playerid]--;

		for(new i; i < lines[playerid]; i++)
			strcat(strTemp, StringToCat[playerid][i], sizeof(strTemp));

		PlayerTextDrawSetString(playerid, DeathInfo[playerid][0], strTemp);
		if(User[playerid][showPlayerHud]) PlayerTextDrawShow(playerid, DeathInfo[playerid][0]);
		User[playerid][timerBox] = gettime()+2;
		return true;
	}

	PlayerTextDrawHide(playerid, DeathInfo[playerid][0]);
	User[playerid][isShowingKSTD] = false;

	for(new i; i < MAX_LINES; i++)
		StringToCat[playerid][i] = "_";

	return true;
}

forward updateDynamicHitmark(issuerid, Float:amount, PlayerText3D:hitbar, Float:posX, Float:posY, Float:posZ, Float:pos);
public updateDynamicHitmark(issuerid, Float:amount, PlayerText3D:hitbar, Float:posX, Float:posY, Float:posZ, Float:pos)
{
	if(!IsPlayerConnected(issuerid))
	{
		DeletePlayer3DTextLabel(issuerid, hitbar);
		return true;
	}

	if(pos >= 0.45)
	{
		User[issuerid][hitmarkTimer] = -1;
		DeletePlayer3DTextLabel(issuerid, hitbar);
		return true;
	}

	DeletePlayer3DTextLabel(issuerid, hitbar);
	new string[32];
	format(string, sizeof(string), "%.0f", amount);
	hitbar = CreatePlayer3DTextLabel(issuerid, string, User[issuerid][hitmarkColor], posX, posY, posZ + pos, 70);
	User[issuerid][hitmarkTimer] = SetTimerEx("updateDynamicHitmark", 30, false, "dfdffff", issuerid, amount, _:hitbar, posX, posY, posZ, pos + 0.03);
	return true;
}

forward destroyHidBar(issuerid, PlayerText3D:hitbar);
public destroyHidBar(issuerid, PlayerText3D:hitbar)
{
	return DeletePlayer3DTextLabel(issuerid, hitbar);
}

forward DeathMessage(playerid);
public DeathMessage(playerid)
{
	PlayerTextDrawHide(playerid, DeathInfo[playerid][1]);
}
