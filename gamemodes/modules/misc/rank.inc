
forward initTop3();
public initTop3()
{
	CreateTOP3Kills();
	UpdateTOP3Kills();
	CreateTOP3Damage();
	UpdateTOP3Damage();
	RespawnLoungeBoard();
	updateKillRank();
	updateDamageRank();
	return true;
}

forward updateDamageRank();
public updateDamageRank()
{
	inline const OnDamageRankLoad()
	{
		new str[1024];
		strcat(str, "\n{B2C3FF}TOP 10 DAMAGE\n\n");

		new rows = cache_num_rows();
		if(rows > 0)
		{
			new username[MAX_PLAYER_NAME + 1], dmg;

			for(new i; i < rows && i < 10; i++)
			{
				cache_get_value_name(i, "username", username, sizeof(username));
				cache_get_value_int(i, "dmg_val", dmg);

				if(dmg > 0)
				{
					strcat(str, sprintf("{B2C3FF}%d. {FFFFFF}%s: {FF6060}%s dmg\n", i + 1, username, FormatNumber(dmg)), sizeof(str));
				}
				else
				{
					strcat(str, "\n", sizeof(str));
				}
			}
		}

		format(g_LoungeDamageRankStr, sizeof(g_LoungeDamageRankStr), "%s", str);

		if(IsValidDynamicObject(g_BoardDamageFront))
		{
			SetDynamicObjectMaterialText(g_BoardDamageFront, 0, g_LoungeDamageRankStr, 130, "Arial", 18, 1, 0xFFCCCCCC, 0xFF111111, 1);
		}
	}
	MySQL_TQueryInline(dbHandle, using inline OnDamageRankLoad,
		"SELECT `username`, IF(`damage_semanal` > 0, `damage_semanal`, `max_damage`) AS `dmg_val` FROM `accounts` \
		WHERE (`damage_semanal` > 0 OR `max_damage` > 0) \
		AND NOT EXISTS(SELECT 1 FROM `banimentos` WHERE `accounts`.`username` = `banimentos`.`username`) \
		ORDER BY `dmg_val` DESC LIMIT 10");
}

forward updateKillRank();
public updateKillRank()
{
	inline const OnKillRankLoad()
	{
		new str[1024];
		strcat(str, "\n{B2C3FF}TOP 10 KILLS\n\n");

		new rows = cache_num_rows();
		if(rows > 0)
		{
			new username[MAX_PLAYER_NAME + 1], kills;

			for(new i; i < rows && i < 10; i++)
			{
				cache_get_value_name(i, "username", username, sizeof(username));
				cache_get_value_int(i, "kills", kills);

				if(kills > 0)
				{
					strcat(str, sprintf("{B2C3FF}%d. {FFFFFF}%s: {FF6060}%s kills\n", i + 1, username, FormatNumber(kills)), sizeof(str));
				}
				else
				{
					strcat(str, "\n", sizeof(str));
				}
			}
		}

		format(g_LoungeKillsRankStr, sizeof(g_LoungeKillsRankStr), "%s", str);
		format(g_LoungeRankStr, sizeof(g_LoungeRankStr), "%s", str);

		if(IsValidDynamicObject(g_BoardKillsFront))
		{
			SetDynamicObjectMaterialText(g_BoardKillsFront, 0, g_LoungeKillsRankStr, 130, "Arial", 18, 1, 0xFFCCCCCC, 0xFF111111, 1);
		}
		updateDamageRank();
	}
	MySQL_TQueryInline(dbHandle, using inline OnKillRankLoad,
		"SELECT `username`, `kills` FROM `accounts` \
		WHERE `kills` > 0 \
		AND NOT EXISTS(SELECT 1 FROM `banimentos` WHERE `accounts`.`username` = `banimentos`.`username`) \
		ORDER BY `kills` DESC LIMIT 10");
}

stock RespawnLoungeBoard()
{
	new Float:A = g_LoungeBoardPos[3];
	new Float:frontRot = A + 90.0;
	new Float:backRot = A - 90.0;
	new Float:frontDx = floatsin(-A, degrees);
	new Float:frontDy = floatcos(-A, degrees);
	new Float:rightDx = floatcos(-A, degrees);
	new Float:rightDy = -floatsin(-A, degrees);

	// Top 10 Kills
	new Float:b1_x = g_LoungeBoardPos[0] + g_LoungeBoardSpacing * rightDx;
	new Float:b1_y = g_LoungeBoardPos[1] + g_LoungeBoardSpacing * rightDy;
	new Float:b1_z = g_LoungeBoardPos[2];

	// Top 10 Damage
	new Float:b2_x = g_LoungeBoardPos[0] - g_LoungeBoardSpacing * rightDx;
	new Float:b2_y = g_LoungeBoardPos[1] - g_LoungeBoardSpacing * rightDy;
	new Float:b2_z = g_LoungeBoardPos[2];

	// TOP 10 KILLS
	if (!IsValidDynamicObject(g_BoardKillsFront))
	{
		g_BoardKillsFront = CreateDynamicObject(3354, b1_x + g_LoungeBoardDepth * frontDx, b1_y + g_LoungeBoardDepth * frontDy, b1_z + g_LoungeBoardHeight, 0.0, 0.0, frontRot, LOUNGEVW, 0, -1, 300.0, 300.0);
	}
	else
	{
		SetDynamicObjectPos(g_BoardKillsFront, b1_x + g_LoungeBoardDepth * frontDx, b1_y + g_LoungeBoardDepth * frontDy, b1_z + g_LoungeBoardHeight);
		SetDynamicObjectRot(g_BoardKillsFront, 0.0, 0.0, frontRot);
	}
	SetDynamicObjectMaterial(g_BoardKillsFront, 1, 19962, "samproadsigns", "materialtext1", 0);

	if (!IsValidDynamicObject(g_BoardKillsBack))
	{
		g_BoardKillsBack = CreateDynamicObject(3354, b1_x - g_LoungeBoardDepth * frontDx, b1_y - g_LoungeBoardDepth * frontDy, b1_z + g_LoungeBoardHeight, 0.0, 0.0, backRot, LOUNGEVW, 0, -1, 300.0, 300.0);
	}
	else
	{
		SetDynamicObjectPos(g_BoardKillsBack, b1_x - g_LoungeBoardDepth * frontDx, b1_y - g_LoungeBoardDepth * frontDy, b1_z + g_LoungeBoardHeight);
		SetDynamicObjectRot(g_BoardKillsBack, 0.0, 0.0, backRot);
	}
	SetDynamicObjectMaterial(g_BoardKillsBack, 1, 19962, "samproadsigns", "materialtext1", 0);
	SetDynamicObjectMaterialText(g_BoardKillsBack, 0, " ", 130, "Arial", 22, 1, 0xFFCCCCCC, 0xFF111111, 1);

	if (!IsValidDynamicObject(g_BoardKillsPillar))
	{
		g_BoardKillsPillar = CreateDynamicObject(3077, b1_x, b1_y, b1_z, 0.0, 0.0, A, LOUNGEVW, 0, -1, 300.0, 300.0);
	}
	else
	{
		SetDynamicObjectPos(g_BoardKillsPillar, b1_x, b1_y, b1_z);
		SetDynamicObjectRot(g_BoardKillsPillar, 0.0, 0.0, A);
	}
	SetDynamicObjectMaterial(g_BoardKillsPillar, 1, 3440, "airportpillar", "metalic_64", 0);
	SetDynamicObjectMaterial(g_BoardKillsPillar, 0, 3440, "airportpillar", "metalic_64", 0);

	// TOP 10 DAMAGE
	if (!IsValidDynamicObject(g_BoardDamageFront))
	{
		g_BoardDamageFront = CreateDynamicObject(3354, b2_x + g_LoungeBoardDepth * frontDx, b2_y + g_LoungeBoardDepth * frontDy, b2_z + g_LoungeBoardHeight, 0.0, 0.0, frontRot, LOUNGEVW, 0, -1, 300.0, 300.0);
	}
	else
	{
		SetDynamicObjectPos(g_BoardDamageFront, b2_x + g_LoungeBoardDepth * frontDx, b2_y + g_LoungeBoardDepth * frontDy, b2_z + g_LoungeBoardHeight);
		SetDynamicObjectRot(g_BoardDamageFront, 0.0, 0.0, frontRot);
	}
	SetDynamicObjectMaterial(g_BoardDamageFront, 1, 19962, "samproadsigns", "materialtext1", 0);

	if (!IsValidDynamicObject(g_BoardDamageBack))
	{
		g_BoardDamageBack = CreateDynamicObject(3354, b2_x - g_LoungeBoardDepth * frontDx, b2_y - g_LoungeBoardDepth * frontDy, b2_z + g_LoungeBoardHeight, 0.0, 0.0, backRot, LOUNGEVW, 0, -1, 300.0, 300.0);
	}
	else
	{
		SetDynamicObjectPos(g_BoardDamageBack, b2_x - g_LoungeBoardDepth * frontDx, b2_y - g_LoungeBoardDepth * frontDy, b2_z + g_LoungeBoardHeight);
		SetDynamicObjectRot(g_BoardDamageBack, 0.0, 0.0, backRot);
	}
	SetDynamicObjectMaterial(g_BoardDamageBack, 1, 19962, "samproadsigns", "materialtext1", 0);
	SetDynamicObjectMaterialText(g_BoardDamageBack, 0, " ", 130, "Arial", 22, 1, 0xFFCCCCCC, 0xFF111111, 1);

	if (!IsValidDynamicObject(g_BoardDamagePillar))
	{
		g_BoardDamagePillar = CreateDynamicObject(3077, b2_x, b2_y, b2_z, 0.0, 0.0, A, LOUNGEVW, 0, -1, 300.0, 300.0);
	}
	else
	{
		SetDynamicObjectPos(g_BoardDamagePillar, b2_x, b2_y, b2_z);
		SetDynamicObjectRot(g_BoardDamagePillar, 0.0, 0.0, A);
	}
	SetDynamicObjectMaterial(g_BoardDamagePillar, 1, 3440, "airportpillar", "metalic_64", 0);
	SetDynamicObjectMaterial(g_BoardDamagePillar, 0, 3440, "airportpillar", "metalic_64", 0);

	g_BoardBaseObject = g_BoardKillsPillar;
	g_Object = g_BoardKillsFront;

	if (g_LoungeKillsRankStr[0] != '\0')
	{
		SetDynamicObjectMaterialText(g_BoardKillsFront, 0, g_LoungeKillsRankStr, 130, "Arial", 18, 1, 0xFFCCCCCC, 0xFF111111, 1);
	}

	if (g_LoungeDamageRankStr[0] != '\0')
	{
		SetDynamicObjectMaterialText(g_BoardDamageFront, 0, g_LoungeDamageRankStr, 130, "Arial", 18, 1, 0xFFCCCCCC, 0xFF111111, 1);
	}
	return 1;
}

