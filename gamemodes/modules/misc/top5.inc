CreateTOP5Kills()
{
    for(new i = 0; i < 5; i++)
    {
        if(IsValidDynamic3DTextLabel(Top5Kills[i])) DestroyDynamic3DTextLabel(Top5Kills[i]);
        if(IsValidDynamic3DTextLabel(Top5KillsNome[i])) DestroyDynamic3DTextLabel(Top5KillsNome[i]);

        Top5Kills[i] = CreateDynamic3DTextLabel("", -1, TOP5Kills[i][0], TOP5Kills[i][1], TOP5Kills[i][2], 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, LOUNGEVW, 0, -1, 25.0);
        Top5KillsNome[i] = CreateDynamic3DTextLabel("", -1, TOP5Kills[i][0], TOP5Kills[i][1], TOP5Kills[i][2] + 1.0, 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, LOUNGEVW, 0, -1, 25.0);
    }
    return true;
}

stock GetTopActorColorHex(t_clanid, t_clanColor, t_chatMainColor, t_hudShadeOne, t_hudColor, dest[], len = sizeof(dest))
{
    if(t_clanid >= 0 && t_clanColor != -1 && t_clanColor != 0)
    {
        new colorVal = (t_clanColor >>> 8) & 0xFFFFFF;
        if(colorVal != 0 && colorVal != 0x4e5458 && colorVal != 0xa4aba8)
        {
            format(dest, len, "%06x", colorVal);
            return 1;
        }
    }

    if(1 <= t_chatMainColor <= sizeof(hudShades))
    {
        format(dest, len, "%06x", hudShades[t_chatMainColor - 1][shadeHex]);
        return 1;
    }
    else if(t_chatMainColor & 0x10000000)
    {
        format(dest, len, "%06x", t_chatMainColor & 0xFFFFFF);
        return 1;
    }

    if(0 <= t_hudShadeOne < sizeof(hudShades))
    {
        format(dest, len, "%06x", hudShades[t_hudShadeOne][shadeHex]);
        return 1;
    }

    if(t_clanid >= 0 && t_clanColor != -1 && t_clanColor != 0)
    {
        new colorVal = (t_clanColor >>> 8) & 0xFFFFFF;
        if(colorVal != 0)
        {
            format(dest, len, "%06x", colorVal);
            return 1;
        }
    }

    if(0 < t_hudColor < sizeof(CoresDoBglh))
    {
        format(dest, len, "%s", CoresDoBglh[t_hudColor]);
        return 1;
    }

    format(dest, len, "B4B5B7");
    return 1;
}

UpdateTOP5Kills()
{
    inline const OnTop5KillsLoad()
    {
        new rows = cache_num_rows();
        if(rows > 0)
        {
            new nome[MAX_PLAYER_NAME + 1], t_skin, t_kills, t_clanid, t_chatMainColor, t_hudShadeOne, t_hudColor, t_clanColor;
            new pColorHex[10], string[128], string2[128];
            for(new i = 0; i < rows; i++)
            {
                cache_get_value_name(i, "username", nome, sizeof(nome));
                cache_get_value_name_int(i, "kills_semanais", t_kills);
                cache_get_value_name_int(i, "skin_id", t_skin);
                cache_get_value_name_int(i, "clanid", t_clanid);
                cache_get_value_name_int(i, "chat_main_color", t_chatMainColor);
                cache_get_value_name_int(i, "hud_shade_one", t_hudShadeOne);
                cache_get_value_name_int(i, "hudcolor", t_hudColor);
                t_clanColor = -1;
                cache_get_value_name_int(i, "clan_color", t_clanColor);

                GetTopActorColorHex(t_clanid, t_clanColor, t_chatMainColor, t_hudShadeOne, t_hudColor, pColorHex);

                format(string, sizeof(string), "{F3230E}%s {B4B5B7}Kills", FormatNumber(t_kills));
                UpdateDynamic3DTextLabelText(Top5Kills[i], -1, string);

                format(string2, sizeof(string2), "{B4B5B7}#%d {%s}%s", i + 1, pColorHex, nome);
                UpdateDynamic3DTextLabelText(Top5KillsNome[i], -1, string2);

                if(IsValidDynamicActor(Top5KillsActor[i]))
                    DestroyDynamicActor(Top5KillsActor[i]);
                Top5KillsActor[i] = CreateDynamicActor(t_skin, TOP5Kills[i][0], TOP5Kills[i][1], TOP5Kills[i][2], TOP5Kills[i][3]);
                SetDynamicActorVirtualWorld(Top5KillsActor[i], LOUNGEVW);
            }
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnTop5KillsLoad,
        "SELECT `accounts`.`username`, `accounts`.`kills_semanais`, `accounts`.`skin_id`, `accounts`.`clanid`, \
        `accounts`.`chat_main_color`, `accounts`.`hud_shade_one`, `accounts`.`hudcolor`, `clans`.`clan_color` \
        FROM `accounts` \
        LEFT JOIN `clans` ON `accounts`.`clanid` = `clans`.`clan_id` \
        WHERE NOT EXISTS(SELECT 1 FROM `banimentos` WHERE `accounts`.`username` = `banimentos`.`username`) \
        ORDER BY `accounts`.`kills_semanais` DESC LIMIT 5");

    return true;
}

CreateTOP5Damage()
{
    for(new i = 0; i < 5; i++)
    {
        if(IsValidDynamic3DTextLabel(Top5Damage[i])) DestroyDynamic3DTextLabel(Top5Damage[i]);
        if(IsValidDynamic3DTextLabel(Top5DamageNome[i])) DestroyDynamic3DTextLabel(Top5DamageNome[i]);

        Top5Damage[i] = CreateDynamic3DTextLabel("", -1, TOP5Damage[i][0], TOP5Damage[i][1], TOP5Damage[i][2], 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, LOUNGEVW, 0, -1, 25.0);
        Top5DamageNome[i] = CreateDynamic3DTextLabel("", -1, TOP5Damage[i][0], TOP5Damage[i][1], TOP5Damage[i][2] + 1.0, 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, LOUNGEVW, 0, -1, 25.0);
    }
    return true;
}

UpdateTOP5Damage()
{
    inline const OnTop5DamageLoad()
    {
        new rows = cache_num_rows();
        if(rows > 0)
        {
            new nome[MAX_PLAYER_NAME + 1], t_skin, Float:t_dmgFloat, t_damage, t_clanid, t_chatMainColor, t_hudShadeOne, t_hudColor, t_clanColor;
            new pColorHex[10], string[128], string2[128];
            for(new i = 0; i < rows; i++)
            {
                cache_get_value_name(i, "username", nome, sizeof(nome));
                cache_get_value_name_float(i, "damage_semanal", t_dmgFloat);
                t_damage = floatround(t_dmgFloat);
                cache_get_value_name_int(i, "skin_id", t_skin);
                cache_get_value_name_int(i, "clanid", t_clanid);
                cache_get_value_name_int(i, "chat_main_color", t_chatMainColor);
                cache_get_value_name_int(i, "hud_shade_one", t_hudShadeOne);
                cache_get_value_name_int(i, "hudcolor", t_hudColor);
                t_clanColor = -1;
                cache_get_value_name_int(i, "clan_color", t_clanColor);

                GetTopActorColorHex(t_clanid, t_clanColor, t_chatMainColor, t_hudShadeOne, t_hudColor, pColorHex);

                format(string, sizeof(string), "{F3230E}%s {B4B5B7}Damage", FormatNumber(t_damage));
                UpdateDynamic3DTextLabelText(Top5Damage[i], -1, string);

                format(string2, sizeof(string2), "{B4B5B7}#%d {%s}%s", i + 1, pColorHex, nome);
                UpdateDynamic3DTextLabelText(Top5DamageNome[i], -1, string2);

                if(IsValidDynamicActor(Top5DamageActor[i]))
                    DestroyDynamicActor(Top5DamageActor[i]);
                Top5DamageActor[i] = CreateDynamicActor(t_skin, TOP5Damage[i][0], TOP5Damage[i][1], TOP5Damage[i][2], TOP5Damage[i][3]);
                SetDynamicActorVirtualWorld(Top5DamageActor[i], LOUNGEVW);
            }
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnTop5DamageLoad,
        "SELECT `accounts`.`username`, `accounts`.`damage_semanal`, `accounts`.`skin_id`, `accounts`.`clanid`, \
        `accounts`.`chat_main_color`, `accounts`.`hud_shade_one`, `accounts`.`hudcolor`, `clans`.`clan_color` \
        FROM `accounts` \
        LEFT JOIN `clans` ON `accounts`.`clanid` = `clans`.`clan_id` \
        WHERE NOT EXISTS(SELECT 1 FROM `banimentos` WHERE `accounts`.`username` = `banimentos`.`username`) \
        ORDER BY `accounts`.`damage_semanal` DESC LIMIT 5");

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

static s_WeekClans[512];
static s_WeekKills[512];
static s_WeekDamage[512];
static s_WeekReports[512];
static s_WeekActions[512];

forward SendWeeklyResultsDiscord(bool:resetAfter);
public SendWeeklyResultsDiscord(bool:resetAfter)
{
    s_WeekClans[0] = EOS;
    s_WeekKills[0] = EOS;
    s_WeekDamage[0] = EOS;
    s_WeekReports[0] = EOS;
    s_WeekActions[0] = EOS;

    inline const OnWeekClans()
    {
        new rows = cache_num_rows();
        if(rows > 0)
        {
            for(new i = 0; i < rows; i++)
            {
                new cname[32], ctag[8], cpts;
                cache_get_value_name(i, "clan_name", cname, sizeof(cname));
                cache_get_value_name(i, "clan_tag", ctag, sizeof(ctag));
                cache_get_value_name_int(i, "clan_zone_points", cpts);

                format(s_WeekClans, sizeof(s_WeekClans), "%s`#%d` `[%s]` **%s** - %s pts\n",
                    s_WeekClans, i + 1, ctag, cname, FormatMoney(cpts));
            }
        }
        else
        {
            format(s_WeekClans, sizeof(s_WeekClans), "*No clan points recorded this week.*\n");
        }

        inline const OnWeekKills()
        {
            new krows = cache_num_rows();
            if(krows > 0)
            {
                for(new i = 0; i < krows; i++)
                {
                    new kname[MAX_PLAYER_NAME + 1], kills;
                    cache_get_value_name(i, "username", kname, sizeof(kname));
                    cache_get_value_name_int(i, "kills_semanais", kills);

                    format(s_WeekKills, sizeof(s_WeekKills), "%s`#%d` **%s** - %s kills\n",
                        s_WeekKills, i + 1, kname, FormatMoney(kills));
                }
            }
            else
            {
                format(s_WeekKills, sizeof(s_WeekKills), "*No kills recorded this week.*\n");
            }

            inline const OnWeekDamage()
            {
                new drows = cache_num_rows();
                if(drows > 0)
                {
                    for(new i = 0; i < drows; i++)
                    {
                        new dname[MAX_PLAYER_NAME + 1], Float:dmg;
                        cache_get_value_name(i, "username", dname, sizeof(dname));
                        cache_get_value_name_float(i, "damage_semanal", dmg);

                        format(s_WeekDamage, sizeof(s_WeekDamage), "%s`#%d` **%s** - %s dmg\n",
                            s_WeekDamage, i + 1, dname, FormatMoney(floatround(dmg)));
                    }
                }
                else
                {
                    format(s_WeekDamage, sizeof(s_WeekDamage), "*No damage recorded this week.*\n");
                }

                inline const OnWeekReports()
                {
                    new rrows = cache_num_rows();
                    if(rrows > 0)
                    {
                        for(new i = 0; i < rrows; i++)
                        {
                            new rname[MAX_PLAYER_NAME + 1], cnt;
                            cache_get_value_name(i, "username", rname, sizeof(rname));
                            cache_get_value_name_int(i, "cnt", cnt);

                            format(s_WeekReports, sizeof(s_WeekReports), "%s`#%d` **%s** - %d reports\n",
                                s_WeekReports, i + 1, rname, cnt);
                        }
                    }
                    else
                    {
                        format(s_WeekReports, sizeof(s_WeekReports), "*No reports recorded this week.*\n");
                    }

                    inline const OnWeekActions()
                    {
                        new arows = cache_num_rows();
                        if(arows > 0)
                        {
                            for(new i = 0; i < arows; i++)
                            {
                                new aname[MAX_PLAYER_NAME + 1], acnt;
                                cache_get_value_name(i, "username", aname, sizeof(aname));
                                cache_get_value_name_int(i, "acnt", acnt);

                                format(s_WeekActions, sizeof(s_WeekActions), "%s`#%d` **%s** - %d actions\n",
                                    s_WeekActions, i + 1, aname, acnt);
                            }
                        }
                        else
                        {
                            format(s_WeekActions, sizeof(s_WeekActions), "*No admin actions recorded this week.*\n");
                        }

                        if(ch_week_results == DCC_INVALID_CHANNEL)
                            ch_week_results = DCC_FindChannelById(CH_WEEK_RESULTS);

                        if(ch_week_results != DCC_INVALID_CHANNEL)
                        {
                            new DCC_Embed:w_embed = DCC_CreateEmbed(
                                .title = "NEW WEEK! - RESULTS",
                                .color = DISCORD_COLOR_INFO
                            );
                            DCC_SetEmbedFooter(w_embed, getDateStrTD());

                            new desc[2048];
                            format(desc, sizeof(desc),
                                "**BEST CLANS BY POINTS**\n%s\n"\
                                "**TOP KILLS:**\n%s\n"\
                                "**TOP DAMAGE:**\n%s\n"\
                                "**TOP GOOD REPORTS:**\n%s\n"\
                                "**TOP ADMIN ACTIONS:**\n%s",
                                s_WeekClans,
                                s_WeekKills,
                                s_WeekDamage,
                                s_WeekReports,
                                s_WeekActions
                            );

                            DCC_SetEmbedDescription(w_embed, desc);
                            DCC_SendChannelEmbedMessage(ch_week_results, w_embed);
                        }

                        if(resetAfter)
                        {
                            ResetWeeklyTOP();
                        }
                    }

                    MySQL_TQueryInline(dbHandle, using inline OnWeekActions,
                        "SELECT `username`, COUNT(*) AS `acnt` FROM `server_logs` \
                        WHERE `category` IN ('bans', 'unbans', 'kicks', 'warns', 'mutes', 'promotions', 'admin_cmd', 'security') \
                        AND `created_at` >= NOW() - INTERVAL 7 DAY \
                        GROUP BY `username` ORDER BY `acnt` DESC LIMIT 5");
                }

                MySQL_TQueryInline(dbHandle, using inline OnWeekReports,
                    "SELECT `username`, COUNT(*) AS `cnt` FROM `server_logs` \
                    WHERE `category` = 'reports' AND `action` = 'Report' \
                    AND `created_at` >= NOW() - INTERVAL 7 DAY \
                    GROUP BY `username` ORDER BY `cnt` DESC LIMIT 5");
            }

            MySQL_TQueryInline(dbHandle, using inline OnWeekDamage,
                "SELECT `username`, `damage_semanal` FROM `accounts` \
                WHERE `damage_semanal` > 0 \
                AND NOT EXISTS(SELECT 1 FROM `banimentos` WHERE `accounts`.`username` = `banimentos`.`username`) \
                ORDER BY `damage_semanal` DESC LIMIT 5");
        }

        MySQL_TQueryInline(dbHandle, using inline OnWeekKills,
            "SELECT `username`, `kills_semanais` FROM `accounts` \
            WHERE `kills_semanais` > 0 \
            AND NOT EXISTS(SELECT 1 FROM `banimentos` WHERE `accounts`.`username` = `banimentos`.`username`) \
            ORDER BY `kills_semanais` DESC LIMIT 5");
    }

    MySQL_TQueryInline(dbHandle, using inline OnWeekClans,
        "SELECT `clan_name`, `clan_tag`, `clan_zone_points` FROM `clans` \
        ORDER BY `clan_zone_points` DESC LIMIT 5");

    return 1;
}
