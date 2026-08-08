
forward initTop5();
public initTop5()
{
	CreateTOP5Kills();
	UpdateTOP5Kills();
	CreateTOP5Damage();
	UpdateTOP5Damage();
	return true;
}

updateKillRank()
{
	inline const OnKillRankLoad()
	{
		const NAME_COL = 26;
		const KILLS_COL = 10;

		new str[1000];

		strcat(str, "                  Top Kills\n\n");
		strcat(str, " Pos"); strcat(str, "  Name                     ");strcat(str, "Kills    "); strcat(str, "K/D\n");

		new rows = cache_num_rows();
		if(rows > 0)
		{
			new useername[MAX_PLAYER_NAME + 1], kills, deaths;
			new killsStr[KILLS_COL + 1], usernamee[NAME_COL + 1];

			for(new i; i < rows; i++)
			{
				cache_get_value_name(i, "username", useername, sizeof(useername));
				cache_get_value_int(i, "kills", kills);
				cache_get_value_int(i, "deaths", deaths);

				if(kills > 0)
				{
					format(killsStr, sizeof(killsStr), "%d", kills);
					format(usernamee, sizeof(usernamee), "%s", useername);

					for(new g = strlen(usernamee); g < NAME_COL; g++)
						strcat(usernamee, " ", sizeof(usernamee));

					for(new k = strlen(killsStr); k < KILLS_COL; k++)
						strcat(killsStr, " ", sizeof(killsStr));

					strcat(str, sprintf(" %d  ", i+1), sizeof(str));
					strcat(str, sprintf("%s%s", (i+1 > 9) ? (" ") : ("  "), usernamee), sizeof(str));
					strcat(str, killsStr, sizeof(str));
					strcat(str, sprintf("%0.2f\n", ratio(kills, deaths)), sizeof(str));
				}
				else strcat(str, "\n", sizeof(str));
			}
		}

		DestroyObject(g_Object);
		g_Object = CreateObject(3077, 1177.4472, -2037.1240, 67.8771, 0.0000, 0.0000, -97.4003);//painel de top kills
		SetObjectMaterialText(g_Object, str, 0, OBJECT_MATERIAL_SIZE_512x256, "Lucida Console", 18, false, 0xFFD9A0A9, 0, OBJECT_MATERIAL_TEXT_ALIGN_LEFT);
	}
	MySQL_TQueryInline(dbHandle, using inline OnKillRankLoad,
		"SELECT `username`, `kills`, `deaths` FROM `accounts` ORDER BY `kills` DESC LIMIT 10");
}
