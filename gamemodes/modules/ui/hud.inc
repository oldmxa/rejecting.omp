
forward hideKillTD(playerid);
public hideKillTD(playerid)
{
	PlayerTextDrawHide(playerid, killsandexp[playerid]);
}

forward hideMoneyTD(playerid);
public hideMoneyTD(playerid)
{
	PlayerTextDrawHide(playerid, killsandexpii[playerid]);
}

stock ShowMoneyTextDraw(playerid, const text[], Float:x = 615.0, Float:y = 56.0)
{
    if(killsandexpii[playerid] != PlayerText:INVALID_TEXT_DRAW)
        PlayerTextDrawDestroy(playerid, killsandexpii[playerid]);

    killsandexpii[playerid] = CreatePlayerTextDraw(playerid, x, y, text);
    PlayerTextDrawFont(playerid, killsandexpii[playerid], TEXT_DRAW_FONT_1);
    PlayerTextDrawLetterSize(playerid, killsandexpii[playerid], 0.179167, 0.750000);
    PlayerTextDrawSetOutline(playerid, killsandexpii[playerid], 1);
    PlayerTextDrawAlignment(playerid, killsandexpii[playerid], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, killsandexpii[playerid], -1);
    PlayerTextDrawBackgroundColor(playerid, killsandexpii[playerid], 80);
    PlayerTextDrawBoxColor(playerid, killsandexpii[playerid], 50);
    PlayerTextDrawSetProportional(playerid, killsandexpii[playerid], 1);

    if(User[playerid][showPlayerHud]) PlayerTextDrawShow(playerid, killsandexpii[playerid]);
    SetTimerEx("hideMoneyTD", 3300, false, "i", playerid);
    return 1;
}

forward updateColor(playerid);
public updateColor(playerid)
{
	SetPlayerColor(playerid, (randomcolor[random(sizeof(randomcolor))] & 0xFFFFFF00) | 0x65);
}

hidePlayerTextdraw(playerid)
{
	PlayerTextDrawHide(playerid, FPSPingPacket[playerid]);
	for(new i; i < 3; i++) PlayerTextDrawHide(playerid, InfoStats[playerid][i]);
	for(new i; i < 4; i++) PlayerTextDrawHide(playerid, ClanHUD[playerid][i]);
	for(new i; i < 5; i++) PlayerTextDrawHide(playerid, OutpostBar[playerid][i]);
	PlayerTextDrawHide(playerid, speedometer[playerid]);
	PlayerTextDrawHide(playerid, killsandexp[playerid]);
	if(killsandexpii[playerid] != PlayerText:INVALID_TEXT_DRAW) PlayerTextDrawHide(playerid, killsandexpii[playerid]);
	PlayerTextDrawHide(playerid, DeathInfo[playerid][0]);
	PlayerTextDrawHide(playerid, DeathInfo[playerid][1]);

	User[playerid][isShowingHud] = false;
}

showPlayerTextDraw(playerid)
{
	if(User[playerid][isSpectating] || playerSpecID[playerid] != INVALID_PLAYER_ID) return 0;

    for(new i; i < 3; i++)
			PlayerTextDrawShow(playerid, InfoStats[playerid][i]);
    PlayerTextDrawShow(playerid, FPSPingPacket[playerid]);

	User[playerid][isShowingHud] = true;
	return 1;
}

stock updatePlayerHud(playerid)
{
	if(User[playerid][isSpectating] || playerSpecID[playerid] != INVALID_PLAYER_ID) return 1;

	if(User[playerid][locationId] != -1)
	{
		new fString[640], cString[300];
		new gMins, gSecs;

		new roundLeft = arenaInfo[User[playerid][locationId]][roundMints];
		if(roundLeft < 1) roundLeft = 0;

		gMins = roundLeft / 60;
		gSecs = roundLeft % 60;

		new playerColorOne[20], playerColorTwo[20];
		format(playerColorOne, sizeof playerColorOne, "%s", GetPlayerHudTag(playerid, false));
		format(playerColorTwo, sizeof playerColorTwo, "%s", GetPlayerHudTag(playerid, true));

		if (User[playerid][hudType] == HUD_UNDERLINE)
		{
			if (User[playerid][isAfk] || User[playerid][locationId] == -1 || User[playerid][locationId] == ARENA_AFK)
			{
				format(fString, sizeof(fString), "%s~n~%sFPS: %s%d %sPL: %s%.1f~n~%sArena Time: %slounge",
					getDateStrTDInfo(playerColorOne, playerColorTwo),
					playerColorOne, playerColorTwo, User[playerid][FPS],
					playerColorOne, playerColorTwo, NetStats_PacketLossPercent(playerid),
					playerColorOne, playerColorTwo
				);
			}
			else
			{
				new bool:lowTime = (gMins < 1 && gSecs <= 30);

				format(fString, sizeof(fString), "%s~n~%sFPS: %s%d %sPL: %s%.1f~n~%sArena Time: %s%d:%s%02d",
					getDateStrTDInfo(playerColorOne, playerColorTwo),
					playerColorOne, playerColorTwo, User[playerid][FPS],
					playerColorOne, playerColorTwo, NetStats_PacketLossPercent(playerid),
					playerColorOne,
					lowTime ? ("~r~~h~") : (playerColorOne), gMins,
					lowTime ? ("~r~~h~") : (playerColorTwo), gSecs
				);
			}
			PlayerTextDrawSetString(playerid, FPSPingPacket[playerid], fString);
		}
		else
		{
			FormatNetworkDataForHud(User[playerid][hudType], playerColorOne, playerColorTwo, User[playerid][FPS], GetPlayerPing(playerid), NetStats_PacketLossPercent(playerid), fString, sizeof(fString));
			PlayerTextDrawSetString(playerid, FPSPingPacket[playerid], fString);
		}

		new clockStr[128];
		format(clockStr, sizeof(clockStr), "%s", GetCustomClockString(User[playerid][hudType], playerColorOne, playerColorTwo, 0, -3, true));
		if (clockStr[0] != '\0')
			PlayerTextDrawSetString(playerid, InfoStats[playerid][2], clockStr);
		else
			PlayerTextDrawSetString(playerid, InfoStats[playerid][2], " ");

		new colorTime[10];
		if(gMins < 1 && gSecs <= 30)
		{
			format(colorTime, 10, "~r~~h~");
		}
		else{
			format(colorTime, 10, "~w~~h~");
		}

		#if !defined GANG
			if (GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE)
			{
				PlayerTextDrawSetString(playerid, InfoStats[playerid][1], " ");
			}
			else if (User[playerid][isAfk])
			{
				if (User[playerid][hudType] == HUD_UNDERLINE)
				{
					PlayerTextDrawSetString(playerid, InfoStats[playerid][1], " ");
				}
				else
				{
					format(fString, sizeof(fString), "~n~%sLOUNGE", playerColorTwo);
					PlayerTextDrawSetString(playerid, InfoStats[playerid][1], fString);
				}
			}
			else
			{
				new intDmg = floatround(playerArenaInfo[User[playerid][locationId]][playerid][arenaDMG]);
				new kills = playerArenaInfo[User[playerid][locationId]][playerid][arenaKills];
				new deaths = playerArenaInfo[User[playerid][locationId]][playerid][arenaDeaths];
				new spree = GetPVarInt(playerid, "Spree");

				if (User[playerid][hudType] == HUD_UNDERLINE)
				{
					format(fString, sizeof(fString),
					"%sStreak:_%s%d~n~\
					%sKills:_%s%d~n~\
					%sDeaths:_%s%d~n~\
					%sDamage:_%s%d~n~\
					%sRatio:_%s%0.2f~n~\
					%sAccuracy:_%s%d%%~n~",
					playerColorOne, playerColorTwo, spree,
					playerColorOne, playerColorTwo, kills,
					playerColorOne, playerColorTwo, deaths,
					playerColorOne, playerColorTwo, intDmg,
					playerColorOne, playerColorTwo, ratio(kills, deaths),
					playerColorOne, playerColorTwo, floatround(GetPlayerAccuracy(playerid, User[playerid][locationId]), floatround_round)
					);
				}
				else
				{
					format(fString, sizeof(fString),
					"%sTime:_%s%d:%s%02d~n~\
					%sKills:_%s%d~n~\
					%sDeaths:_%s%d~n~\
					%sDamage:_%s%d~n~\
					%sRatio:_%s%0.2f~n~\
					%sStreak:_%s%d~n~",
					playerColorOne, playerColorTwo, gMins, colorTime, gSecs,
					playerColorOne, playerColorTwo, kills,
					playerColorOne, playerColorTwo, deaths,
					playerColorOne, playerColorTwo, intDmg,
					playerColorOne, playerColorTwo, ratio(kills, deaths),
					playerColorOne, playerColorTwo, spree
					);
				}
				PlayerTextDrawSetString(playerid, InfoStats[playerid][1], fString);
			}
		#endif

		format(fString, sizeof(fString),
		"%sK: %s%d~n~\
		%sD: %s%d~n~\
		%sRatio: %s%0.2f~n~\
		%sDuel W:%s%d %sL:%s%d~n~\
		%sDuel 2v2 W:%s%d %sL:%s%d~n~\
		%sLevel: %s%d (%d)",
		playerColorOne, playerColorTwo, User[playerid][playerKills],
		playerColorOne, playerColorTwo, User[playerid][playerDeaths],
		playerColorOne, playerColorTwo, GetPlayerKDR(playerid),
		playerColorOne, playerColorTwo, User[playerid][duelWins], playerColorOne, playerColorTwo, User[playerid][duelLosses],
		playerColorOne, playerColorTwo, User[playerid][duel2v2Wins], playerColorOne, playerColorTwo, User[playerid][duel2v2Losses],
		playerColorOne, playerColorTwo, User[playerid][playerLevel], levelGoal(User[playerid][playerLevel]));
		PlayerTextDrawSetString(playerid, InfoStats[playerid][0],fString);

		if(IsPlayerInClan(playerid) && User[playerid][showPlayerHud] && User[playerid][showClanHud])
		{
			for(new i; i < 4; i++)
				PlayerTextDrawShow(playerid, ClanHUD[playerid][i]);

			format(cString, sizeof(cString), "%sClan:_%s%s__%sKills:_%s%d",
				playerColorOne, playerColorTwo, ClanInfo[User[playerid][clanID]][clanName],
				playerColorOne, playerColorTwo, ClanInfo[User[playerid][clanID]][clanKills]);
			PlayerTextDrawSetString(playerid, ClanHUD[playerid][0],cString);

			format(cString, sizeof(cString), "%sRespects:_%s%d",//Zone_Points
				playerColorOne, playerColorTwo, ClanInfo[User[playerid][clanID]][clanPoints]);
			PlayerTextDrawSetString(playerid, ClanHUD[playerid][1],cString);

			new roleIdx = User[playerid][clanRole];
			if(roleIdx < 0 || roleIdx >= MAX_ROLES) roleIdx = 0;

			format(cString, sizeof(cString), "%sRank:_%s%s",
				playerColorOne, playerColorTwo, ClanRoles[User[playerid][clanID]][roleIdx]);
			PlayerTextDrawSetString(playerid, ClanHUD[playerid][2],cString);

			new count = 0, strr[200];
			strr[0] = EOS;

			foreach(new i: Player)
			{
				if(User[i][clanID] != User[playerid][clanID]) continue;

				strcat(strr, fmat_return("%s%s_(%d)~n~", playerColorTwo, getPlayerName(i), i), sizeof(strr));
				count++;
			}

			new cCCString[190];
		
			format(cCCString, sizeof(cCCString), "%sOnline_(%d|%d)~n~~w~%s", 
				playerColorOne, count, ClanMembersCount[User[playerid][clanID]], strr);
			PlayerTextDrawSetString(playerid, ClanHUD[playerid][3], cCCString);
		}
		else
		{
			for(new i; i < 4; i++)
				PlayerTextDrawHide(playerid, ClanHUD[playerid][i]);
		}
	}
	CallLocalFunction("CheckTrainingHudOnUpdate", "d", playerid);
	return 1;
}


forward UpdateFastHUD();
public UpdateFastHUD()
{
    foreach(new i : Player)
    {
        if(IsPlayerNPC(i)) continue;
        if(!User[i][isLogged] || !User[i][isAlreadySpawned]) continue;
        if(!User[i][showPlayerHud]) continue;
        if(User[i][isSpectating] || playerSpecID[i] != INVALID_PLAYER_ID) continue;

        if(User[i][locationId] != -1 && User[i][isShowingHud])
        {
            updatePlayerHud(i);
        }
    }
    return 1;
}
