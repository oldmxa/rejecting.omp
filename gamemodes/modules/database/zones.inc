forward ZoneColours();
public ZoneColours()
{
	new rows = cache_num_rows();

	for (new i; i < rows; i ++)
	{
		new zoneid, clanColour, ownerClan = -1;

		cache_get_value_int(i, "zoneid", zoneid);
		cache_get_value_int(i, "clancolor", clanColour);
		cache_get_value_int(i, "clanid", ownerClan);

		if(zoneid < 0 || zoneid >= MAX_ZONES) continue;
		if(ownerClan < 0 || ownerClan >= MAX_CLANS) ownerClan = -1;

		ZoneOwnerClan[zoneid] = ownerClan;
		ZoneColour[zoneid] = SetAlpha(clanColour, 75);
		ShowZoneForAll(zoneid, ZoneColour[zoneid], 0xFFFFFFFF, 0xFFFFFFFF);
	}

	return true;
}

forward ZoneCaptured(zoneid, clanid, dmgArena, location, killsArena);
public ZoneCaptured(zoneid, clanid, dmgArena, location, killsArena)
{
	new rows = cache_num_rows();

	if(zoneid < 0 || zoneid >= MAX_ZONES) return false;
	if(clanid < 0 || clanid >= MAX_CLANS) return false;

	new color = DEFAULT_ZONE_COLOUR;
    new clanNameColor[100];

    if(ClanInfo[clanid][clanIDUsed])
    {
        color = SetAlpha(ClanInfo[clanid][clanColor], 75);
        format(clanNameColor, sizeof(clanNameColor), "{%06x}[%s] %s{FFFFFF}", ClanInfo[clanid][clanColor] >>> 8, ClanInfo[clanid][clanTag], ClanInfo[clanid][clanName]);
    }
    else
    {
        format(clanNameColor, sizeof(clanNameColor), "%s", ClanInfo[clanid][clanName]);
    }

	if(rows)
	{
		new dmgSQL;
		cache_get_value_int(0, "dmg", dmgSQL);

		if(dmgArena > dmgSQL)
		{
			mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `zones` SET `clanid` = %d, `dmg` = %d, `clancolor` = %d WHERE `zoneid` = %d", clanid, dmgArena, color, zoneid);
			mysql_tquery(dbHandle, queryGlobal);

            ZoneOwnerClan[zoneid] = clanid;

			foreach(new i : Player)
			{
				if(User[i][locationId] == location)
				{
					if(!User[i][hideGlobalMsg])
					{
						va_SendClientMessage(i, -1, CLAN_ROUND_DOMINATED_ARENA,
							clanNameColor,
							zoneid,
							killsArena,
							dmgArena
						);
					}
				}
			}

			ZoneColour[zoneid] = color;
			ShowZoneForAll(zoneid, color, 0xFFFFFFFF, 0xFFFFFFFF);
		}
		else
		{
			new remainingDMG = dmgSQL - dmgArena;
			foreach(new i : Player)
			{
				if(User[i][locationId] == location)
				{
					if(!User[i][hideGlobalMsg])
					{
						va_SendClientMessage(i, -1, CLAN_ROUND_WON,	clanNameColor,	killsArena,	dmgArena, remainingDMG);
					}
				}
			}
		}
	}
	else
	{
		mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "INSERT INTO `zones` (`zoneid`, `clanid`, `dmg`, `clancolor`) VALUES(%d, %d, %d, %d)", zoneid, clanid, dmgArena, color);
		mysql_tquery(dbHandle, queryGlobal);

        ZoneOwnerClan[zoneid] = clanid;

		foreach(new i : Player)
		{
			if(User[i][locationId] == location)
			{
				if(!User[i][hideGlobalMsg])
				{
					va_SendClientMessage(i, -1, CLAN_ROUND_DOMINATED_ARENA,
						clanNameColor,
						zoneid,
						killsArena,
						dmgArena
					);
				}
			}
		}

		ShowZoneForAll(zoneid, color, 0xFFFFFFFF, 0xFFFFFFFF);
	}

	return true;
}
