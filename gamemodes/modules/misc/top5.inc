CreateTOP5Kills()
{
    Top5Kills[0] = CreateDynamic3DTextLabel("", -1, 1184.2400,-2032.3656,69.0078,123.0535, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);
    Top5KillsNome[0] = CreateDynamic3DTextLabel("", -1, 1184.2400,-2032.3656,70.0000,123.0535, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);

    Top5Kills[1] = CreateDynamic3DTextLabel("", -1, 1182.5326,-2029.8873,69.0078,123.0753, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);
    Top5KillsNome[1] = CreateDynamic3DTextLabel("", -1, 1182.5326,-2029.8873,70.0000,123.0753, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);

    Top5Kills[2] = CreateDynamic3DTextLabel("", -1, 1180.1112,-2027.8939,69.0078,148.8109, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);
    Top5KillsNome[2] = CreateDynamic3DTextLabel("", -1, 1180.1112,-2027.8939,70.0000,148.8109, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);

    Top5Kills[3] = CreateDynamic3DTextLabel("", -1, 1169.2753,-2028.4543,69.0006,230.1996, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);
    Top5KillsNome[3] = CreateDynamic3DTextLabel("", -1, 1169.2753,-2028.4543,70.0000,230.1996, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);

    Top5Kills[4] = CreateDynamic3DTextLabel("", -1, 1165.3832,-2032.8639,69.0078,235.5392, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);
    Top5KillsNome[4] = CreateDynamic3DTextLabel("", -1, 1165.3832,-2032.8639,70.0000,235.5392, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);

    return true;
}

UpdateTOP5Kills()
{
    inline const OnTop5KillsLoad()
    {
        new rows = cache_num_rows();
        if(rows > 0)
        {
            new nome[MAX_PLAYER_NAME + 1], skin, kills, string[96], string2[96];
            for(new i = 0; i < rows; i++)
            {
                cache_get_value_name(i, "username", nome, sizeof(nome));
                cache_get_value_int(i, "kills_semanais", kills);
                cache_get_value_int(i, "skin_id", skin);

                format(string, sizeof(string), "%d Kills", kills);
                UpdateDynamic3DTextLabelText(Top5Kills[i], 0x9b2f25ff, string);
                format(string2, sizeof(string2), "%d. %s", i+1, nome);
                UpdateDynamic3DTextLabelText(Top5KillsNome[i], 0x9b2f25ff, string2);

                if(IsValidDynamicActor(Top5KillsActor[i]))
                 DestroyDynamicActor(Top5KillsActor[i]);
                Top5KillsActor[i] = CreateDynamicActor(skin, TOP5Kills[i][0], TOP5Kills[i][1], TOP5Kills[i][2], TOP5Kills[i][3]);
                SetDynamicActorVirtualWorld(Top5KillsActor[i], LOUNGEVW);
            }
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnTop5KillsLoad,
        "SELECT `username`, `kills_semanais`, `skin_id` FROM `accounts` \
        WHERE NOT EXISTS(SELECT 1 FROM `banimentos` WHERE `accounts`.`username` = `banimentos`.`username`) \
        ORDER BY `kills_semanais` DESC LIMIT 5");

    return true;
}

CreateTOP5Damage()
{
    Top5Damage[0] = CreateDynamic3DTextLabel("", -1, 1184.4551,-2041.2717,69.0078,46.4216, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);
    Top5DamageNome[0] = CreateDynamic3DTextLabel("", -1, 1184.4551,-2041.2717,70.0000,46.4216, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);

    Top5Damage[1] = CreateDynamic3DTextLabel("", -1, 1182.9928,-2043.0415,69.0078,56.8577, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);
    Top5DamageNome[1] = CreateDynamic3DTextLabel("", -1, 1182.9928,-2043.0415,70.0000,56.8577, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);

    Top5Damage[2] = CreateDynamic3DTextLabel("", -1, 1180.6578,-2045.7148,69.0078,34.9736, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);
    Top5DamageNome[2] = CreateDynamic3DTextLabel("", -1, 1180.6578,-2045.7148,70.0000,34.9736, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);

    Top5Damage[3] = CreateDynamic3DTextLabel("", -1, 1169.7711,-2045.6019,69.0006,346.1958, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);
    Top5DamageNome[3] = CreateDynamic3DTextLabel("", -1, 1169.7711,-2045.6019,70.0000,346.1958, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);

    Top5Damage[4] = CreateDynamic3DTextLabel("", -1, 1165.5803,-2041.0651,69.0078,320.7744, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);
    Top5DamageNome[4] = CreateDynamic3DTextLabel("", -1, 1165.5803,-2041.0651,70.0000,320.7744, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, LOUNGEVW, 0, -1, 25);

    return true;
}

UpdateTOP5Damage()
{
    inline const OnTop5DamageLoad()
    {
        new rows = cache_num_rows();
        if(rows > 0)
        {
            new nome[MAX_PLAYER_NAME + 1], skin, damage, string[96], string2[96];
            for(new i = 0; i < rows; i++)
            {
                cache_get_value_name(i, "username", nome, sizeof(nome));
                cache_get_value_int(i, "damage_semanal", damage);
                cache_get_value_int(i, "skin_id", skin);

                format(string, sizeof(string), "Damage %d", damage);
                UpdateDynamic3DTextLabelText(Top5Damage[i], 0x9b2f25ff, string);
                format(string2, sizeof(string2), "%d. %s", i+1, nome);
                UpdateDynamic3DTextLabelText(Top5DamageNome[i], 0x9b2f25ff, string2);

                if(IsValidDynamicActor(Top5DamageActor[i]))
                    DestroyDynamicActor(Top5DamageActor[i]);
                Top5DamageActor[i] = CreateDynamicActor(skin, TOP5Damage[i][0], TOP5Damage[i][1], TOP5Damage[i][2], TOP5Damage[i][3]);
                SetDynamicActorVirtualWorld(Top5DamageActor[i], LOUNGEVW);
            }
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnTop5DamageLoad,
        "SELECT `username`, `damage_semanal`, `skin_id` FROM `accounts` \
        WHERE NOT EXISTS(SELECT 1 FROM `banimentos` WHERE `accounts`.`username` = `banimentos`.`username`) \
        ORDER BY `damage_semanal` DESC LIMIT 5");

    return true;
}

CheckTOP1Kills()
{
    inline const OnTop1KillsLoad()
    {
        if(cache_num_rows())
        {
            new nome[MAX_PLAYER_NAME + 1], kills;
            cache_get_value_name(0, "username", nome, sizeof(nome));
            cache_get_value_int(0, "kills_semanais", kills);

            va_SendClientMessageToAll(-1, WEEKLY_KILLS, nome, kills);
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnTop1KillsLoad,
        "SELECT `username`, `kills_semanais` FROM `accounts` \
        WHERE NOT EXISTS(SELECT 1 FROM `banimentos` WHERE `accounts`.`username` = `banimentos`.`username`) \
        ORDER BY `kills_semanais` DESC LIMIT 1");

    return true;
}

CheckTOP1Damage()
{
    inline const OnTop1DamageLoad()
    {
        if(cache_num_rows())
        {
            new nome[MAX_PLAYER_NAME + 1], damage;
            cache_get_value_name(0, "username", nome, sizeof(nome));
            cache_get_value_int(0, "damage_semanal", damage);

            va_SendClientMessageToAll(-1, WEEKLY_DAMAGE, nome, damage);
        }

        ResetWeeklyTOP();
    }
    MySQL_TQueryInline(dbHandle, using inline OnTop1DamageLoad,
        "SELECT `username`, `damage_semanal` FROM `accounts` \
        WHERE NOT EXISTS(SELECT 1 FROM `banimentos` WHERE `accounts`.`username` = `banimentos`.`username`) \
        ORDER BY `damage_semanal` DESC LIMIT 1");

    return true;
}

ResetWeeklyTOP()
{
	mysql_tquery(dbHandle, "UPDATE `accounts` SET `kills_semanais` = 0, `damage_semanal` = 0 WHERE `kills_semanais` > 0 OR `damage_semanal` > 0");

    foreach(new playerid: Player)
    {
        User[playerid][kills_semanais] = 0;
        User[playerid][dmgSemanal] = 0;
    }

    UpdateServerStats();
    va_SendClientMessageToAll(-1, WEEKLY_RESET);
    return true;
}
