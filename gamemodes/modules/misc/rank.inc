
forward initTop5();
public initTop5()
{
	CreateTOP5Kills();
	UpdateTOP5Kills();
	CreateTOP5Damage();
	UpdateTOP5Damage();
	updateKillRank();
	return true;
}

forward updateKillRank();
public updateKillRank()
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

		format(g_LoungeRankStr, sizeof(g_LoungeRankStr), "%s", str);

		new Float:frontX = g_LoungeBoardPos[0] + 0.015 * floatsin(-g_LoungeBoardPos[3], degrees);
		new Float:frontY = g_LoungeBoardPos[1] + 0.015 * floatcos(-g_LoungeBoardPos[3], degrees);

		if (!IsValidDynamicObject(g_BoardBaseObject))
		{
			g_BoardBaseObject = CreateDynamicObject(3077, g_LoungeBoardPos[0], g_LoungeBoardPos[1], g_LoungeBoardPos[2], 0.0000, 0.0000, g_LoungeBoardPos[3], LOUNGEVW, 0, -1, 300.0, 300.0);
		}
		else
		{
			SetDynamicObjectPos(g_BoardBaseObject, g_LoungeBoardPos[0], g_LoungeBoardPos[1], g_LoungeBoardPos[2]);
			SetDynamicObjectRot(g_BoardBaseObject, 0.0000, 0.0000, g_LoungeBoardPos[3]);
		}

		if (!IsValidDynamicObject(g_Object))
		{
			g_Object = CreateDynamicObject(3077, frontX, frontY, g_LoungeBoardPos[2], 0.0000, 0.0000, g_LoungeBoardPos[3], LOUNGEVW, 0, -1, 300.0, 300.0);
		}
		else
		{
			SetDynamicObjectPos(g_Object, frontX, frontY, g_LoungeBoardPos[2]);
			SetDynamicObjectRot(g_Object, 0.0000, 0.0000, g_LoungeBoardPos[3]);
		}
		SetDynamicObjectMaterialText(g_Object, 0, str, OBJECT_MATERIAL_SIZE_512x256, "Lucida Console", 18, false, 0xFFD9A0A9, 0, OBJECT_MATERIAL_TEXT_ALIGN_LEFT);
	}
	MySQL_TQueryInline(dbHandle, using inline OnKillRankLoad,
		"SELECT `username`, `kills`, `deaths` FROM `accounts` ORDER BY `kills` DESC LIMIT 10");
}
