#if defined _clan_functions_included
    #endinput
#endif
#define _clan_functions_included

stock bool:IsValidClanID(clanid)
{
    if (!(0 <= clanid < MAX_CLANS)) return false;
    return ClanInfo[clanid][clanIDUsed];
}

stock bool:IsValidClanSlot(slot)
{
    return (0 <= slot < MAX_MEMBERS);
}

stock bool:IsValidClanRole(role)
{
    return (0 <= role < MAX_ROLES);
}

stock bool:IsClanMember(playerid)
{
    return IsValidClanID(User[playerid][clanID]);
}

stock bool:IsClanLeader(playerid)
{
    if (!IsClanMember(playerid)) return false;
    return (User[playerid][clanRole] == RANK_FOUNDER);
}

stock bool:HasClanRank(playerid, rank)
{
    if (!IsClanMember(playerid)) return false;
    return (User[playerid][clanRole] >= rank);
}

stock bool:SanitizeClanText(const input[], dest[], size = sizeof(dest), minLen = CLAN_NAME_MIN)
{
    return SanitizeString(input, dest, size, minLen);
}

stock GetFreeClanID()
{
    for (new i = 0; i < MAX_CLANS; i++)
    {
        if (!ClanInfo[i][clanIDUsed]) return i;
    }
    return -1;
}

stock GetFreeClanSlotID(clanid)
{
    if (!IsValidClanID(clanid)) return -1;

    for (new i = 0; i < MAX_MEMBERS; i++)
    {
        if (!ClanMembers[clanid][i][member_slotused]) return i;
    }
    return -1;
}

stock GetPlayerClanSlotID(playerid, clanid)
{
    if (!IsValidClanID(clanid)) return -1;

    for (new i = 0; i < MAX_MEMBERS; i++)
    {
        if (ClanMembers[clanid][i][member_id] == User[playerid][user_sqlid]) return i;
    }
    return -1;
}

stock GetClanMemberSlots(clanid)
{
    new slots = CLAN_BASE_SLOTS + GetSafehouseSlotBonus(clanid);

    if (slots > MAX_MEMBERS) slots = MAX_MEMBERS;
    return slots;
}

stock GetClanOnlineMembers(clanid)
{
    if (!IsValidClanID(clanid)) return 0;

    new count = 0;
    for (new i = 0; i < MAX_MEMBERS; i++)
    {
        if (!ClanMembers[clanid][i][member_slotused]) continue;
        if (IsAccountOnline(ClanMembers[clanid][i][member_id]) != 0) count++;
    }
    return count;
}

stock GetClanMemberCount(clanid)
{
    if (!IsValidClanID(clanid)) return 0;

    new count = 0;
    for (new i = 0; i < MAX_MEMBERS; i++)
    {
        if (ClanMembers[clanid][i][member_slotused]) count++;
    }
    return count;
}

stock GetClanLeaderName(clanid, dest[], size = sizeof(dest))
{
    dest[0] = '\0';

    if (!IsValidClanID(clanid))
    {
        format(dest, size, "N/A");
        return 0;
    }

    if (ClanInfo[clanid][clanLeader][0] != '\0' && strlen(ClanInfo[clanid][clanLeader]) > 1)
    {
        format(dest, size, "%s", ClanInfo[clanid][clanLeader]);
        return 1;
    }

    for (new i = 0; i < MAX_MEMBERS; i++)
    {
        if (ClanMembers[clanid][i][member_slotused] && ClanMembers[clanid][i][member_role] == RANK_FOUNDER)
        {
            format(dest, size, "%s", ClanMembers[clanid][i][member_name]);
            format(ClanInfo[clanid][clanLeader], MAX_PLAYER_NAME + 1, "%s", ClanMembers[clanid][i][member_name]);
            return 1;
        }
    }

    format(dest, size, "N/A");
    return 0;
}

public GetPlayerIDBySQLID(sqlid)
{
    foreach (new i : Player)
    {
        if (User[i][user_sqlid] == sqlid && User[i][isLogged]) return i;
    }
    return INVALID_PLAYER_ID;
}

stock ResetClanMemberSlot(clanid, slot)
{
    if (!IsValidClanID(clanid) || !IsValidClanSlot(slot)) return 0;

    ClanMembers[clanid][slot][member_slotused] = false;
    ClanMembers[clanid][slot][member_role] = -1;
    ClanMembers[clanid][slot][member_id] = 0;
    format(ClanMembers[clanid][slot][member_name], MAX_PLAYER_NAME + 1, " ");
    return 1;
}

stock AddClanMember(clanid, slot, playerid, role)
{
    if (!IsValidClanID(clanid) || !IsValidClanSlot(slot)) return 0;

    ClanMembers[clanid][slot][member_slotused] = true;
    ClanMembers[clanid][slot][member_id] = User[playerid][user_sqlid];
    ClanMembers[clanid][slot][member_role] = role;
    format(ClanMembers[clanid][slot][member_name], MAX_PLAYER_NAME + 1, "%s", User[playerid][userName]);
    ClanMembersCount[clanid]++;
    return 1;
}

stock CreateClan(playerid, const tag[], const name[])
{
    new clanid = GetFreeClanID();
    if (clanid == -1) return -1;

    ClanInfo[clanid][clanIDUsed] = true;

    new slot = GetFreeClanSlotID(clanid);
    if (slot == -1)
    {
        ClanInfo[clanid][clanIDUsed] = false;
        return -1;
    }

    new timestamp = gettime();

    ClanInfo[clanid][clanID] = clanid;
    ClanInfo[clanid][clanPoints] = 0;
    ClanInfo[clanid][clanKills] = 0;
    ClanInfo[clanid][clanColor] = CLAN_COLOR_DEFAULT;
    ClanInfo[clanid][clanCreatedAt] = timestamp;

    for (new i = 0; i < MAX_LOCATIONS; i++)
    {
        ClanInfo[clanid][clanArenaKills][i] = 0;
        ClanInfo[clanid][clanArenaDMG][i] = 0;
    }

    format(ClanInfo[clanid][clanLeader], MAX_PLAYER_NAME + 1, "%s", User[playerid][userName]);
    format(ClanInfo[clanid][clanTag], CLAN_TAG_LEN, "%s", tag);
    format(ClanInfo[clanid][clanName], CLAN_NAME_LEN, "%s", name);

    for (new i = 0; i < MAX_ROLES; i++)
        format(ClanRoles[clanid][i], CLAN_ROLE_LEN, "%s", ClanDefaultRoles[i]);

    GivePlayerCash(playerid, -CLAN_PRICE);
    User[playerid][clanID] = clanid;
    User[playerid][clanRole] = RANK_FOUNDER;

    AddClanMember(clanid, slot, playerid, RANK_FOUNDER);
    InsertClan(clanid, User[playerid][user_sqlid], name, tag, timestamp);

    SetClanColor(playerid);
    UpdatePlayerData(playerid);
    return clanid;
}

stock LeaveClan(playerid)
{
    if (!IsClanMember(playerid)) return SendClientMessage(playerid, -1, "%s", CLAN_NOTAMEMBER);
    if (IsClanLeader(playerid)) return SendClientMessage(playerid, -1, "%s", CLAN_ERROR1);

    new clanid = User[playerid][clanID];

    foreach (new i : Player)
    {
        if (User[i][isLogged] && User[i][clanID] == clanid)
        {
            va_SendClientMessage(i, -1, CLAN_LEFT, GetPlayerColor(playerid) >>> 8, User[playerid][userName]);
            PlaySuccessSound(i);
        }
    }

    new slot = GetPlayerClanSlotID(playerid, clanid);
    if (slot != -1)
    {
        ResetClanMemberSlot(clanid, slot);
        if (ClanMembersCount[clanid] > 0) ClanMembersCount[clanid]--;
    }

    User[playerid][clanID] = -1;
    User[playerid][clanRole] = -1;
    User[playerid][spawnSafehouse] = false;

    SetClanColor(playerid);
    UpdatePlayerData(playerid);
    return 1;
}

stock ChangePlayerRank(playerid, clanid, slot, rank)
{
    if (!IsValidClanID(clanid) || !IsValidClanSlot(slot)) return 0;

    SaveClanMemberRank(ClanMembers[clanid][slot][member_id], rank);

    new targetid = GetPlayerIDBySQLID(ClanMembers[clanid][slot][member_id]);
    if (targetid != INVALID_PLAYER_ID)
    {
        if (rank != -1)
        {
            User[targetid][clanRole] = rank;
            UpdatePlayerData(targetid);
            va_SendClientMessage(targetid, -1, CLAN_EDITRANK1, getPlayerName(playerid), ClanRoles[clanid][rank]);
        }
        else
        {
            va_SendClientMessage(targetid, -1, CLAN_KICKED, ClanInfo[clanid][clanName]);
            User[targetid][clanID] = -1;
            User[targetid][clanRole] = -1;
            User[targetid][spawnSafehouse] = false;
            SetClanColor(targetid);
            PlaySuccessSound(targetid);
            UpdatePlayerData(targetid);
        }
    }

    if (rank != -1)
    {
        va_SendClientMessage(playerid, -1, CLAN_EDITRANK, ClanMembers[clanid][slot][member_name], ClanRoles[clanid][rank]);
        ClanMembers[clanid][slot][member_role] = rank;
        return 1;
    }

    va_SendClientMessage(playerid, -1, CLAN_KICK, ClanMembers[clanid][slot][member_name]);

    ClearClanMemberAccount(ClanMembers[clanid][slot][member_id]);
    ResetClanMemberSlot(clanid, slot);
    if (ClanMembersCount[clanid] > 0) ClanMembersCount[clanid]--;
    return 1;
}

stock EditRoleName(playerid, clanid, roleid, const rolename[])
{
    if (!IsValidClanID(clanid) || !IsValidClanRole(roleid)) return 0;
    if (!HasClanRank(playerid, RANK_COFOUNDER)) return SendClientMessage(playerid, -1, "%s", CLAN_LOWRANK);
    if (roleid == MAX_ROLES - 1 && !IsClanLeader(playerid)) return SendClientMessage(playerid, -1, "%s", CLAN_LOWRANK);

    new clean[CLAN_ROLE_LEN];
    if (!SanitizeClanText(rolename, clean, sizeof(clean), CLAN_ROLE_MIN))
        return SendClientMessage(playerid, -1, "%s", CLAN_ERROR4);

    format(ClanRoles[clanid][roleid], CLAN_ROLE_LEN, "%s", clean);
    UpdateRoles(clanid);
    va_SendClientMessage(playerid, -1, CLAN_ROLE_CHANGED, clean);
    return 1;
}

stock EditClanName(playerid, clanid, const name[])
{
    if (!IsValidClanID(clanid)) return 0;
    if (!HasClanRank(playerid, RANK_COFOUNDER)) return SendClientMessage(playerid, -1, "%s", CLAN_LOWRANK);

    new clean[CLAN_NAME_LEN];
    if (!SanitizeClanText(name, clean, sizeof(clean), CLAN_NAME_MIN))
        return SendClientMessage(playerid, -1, "%s", CLAN_NAMEFAIL);

    inline const OnNameChecked(taken)
    {
        if (taken == CLAN_TEXT_FREE && IsValidClanID(clanid))
        {
            format(ClanInfo[clanid][clanName], CLAN_NAME_LEN, "%s", clean);
            UpdateClan(clanid);
            RefreshClanColor(clanid);
            va_SendClientMessage(playerid, -1, CLAN_NAME_CHANGED, clean);
        }
    }
    CheckClanText(playerid, "", clean, clanid, using inline OnNameChecked);

    return 1;
}

stock EditClanTag(playerid, clanid, const tag[])
{
    if (!IsValidClanID(clanid)) return 0;
    if (!HasClanRank(playerid, RANK_COFOUNDER)) return SendClientMessage(playerid, -1, "%s", CLAN_LOWRANK);

    new clean[CLAN_TAG_LEN];
    if (!SanitizeClanText(tag, clean, sizeof(clean), CLAN_TAG_MIN))
        return SendClientMessage(playerid, -1, "%s", CLAN_TAGFAIL);

    inline const OnTagChecked(taken)
    {
        if (taken == CLAN_TEXT_FREE && IsValidClanID(clanid))
        {
            format(ClanInfo[clanid][clanTag], CLAN_TAG_LEN, "%s", clean);
            UpdateClan(clanid);
            RefreshClanColor(clanid);
            va_SendClientMessage(playerid, -1, CLAN_TAG_CHANGED, clean);
        }
    }
    CheckClanText(playerid, clean, "", clanid, using inline OnTagChecked);

    return 1;
}

stock SetClanColorValue(playerid, clanid, color)
{
    if (!IsValidClanID(clanid)) return 0;

    ClanInfo[clanid][clanColor] = color;
    UpdateClan(clanid);

    foreach (new i : Player)
    {
        if (User[i][clanID] != clanid) continue;

        SetClanColor(i);
        va_SendClientMessage(i, -1, CLAN_PREFIX"Clan color updated by %s!", User[playerid][userName]);
    }
    return 1;
}

stock RefreshClanColor(clanid)
{
    foreach (new i : Player)
    {
        if (User[i][isLogged] && User[i][clanID] == clanid) SetClanColor(i);
    }
    return 1;
}

stock ReleaseClanZones(clanid)
{
    if (!(0 <= clanid < MAX_CLANS)) return 0;

    new released = 0;
    for (new i = 0; i < MAX_ZONES; i++)
    {
        if (ZoneOwnerClan[i] != clanid) continue;

        ZoneOwnerClan[i] = -1;
        ZoneColour[i] = DEFAULT_ZONE_COLOUR;
        ShowZoneForAll(i, DEFAULT_ZONE_COLOUR, 0xFFFFFFFF, 0xFFFFFFFF);
        released++;
    }

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "DELETE FROM `zones` WHERE `clanid` = %d", clanid);
    mysql_tquery(dbHandle, queryGlobal);
    return released;
}

stock DeleteClan(playerid, clanid)
{
    if (!IsValidClanID(clanid)) return 0;

    OnClanDeleted(clanid);
    ReleaseClanZones(clanid);
    ReleaseClanOutposts(clanid);

    foreach (new i : Player)
    {
        if (!User[i][isLogged] || User[i][clanID] != clanid) continue;

        User[i][clanID] = -1;
        User[i][clanRole] = -1;
        SetClanColor(i);
        UpdatePlayerData(i);
        va_SendClientMessage(i, -1, CLAN_DELETED, getPlayerName(playerid));
    }

    for (new i = 0; i < MAX_MEMBERS; i++)
        ResetClanMemberSlot(clanid, i);

    ClanInfo[clanid][clanIDUsed] = false;
    ClanInfo[clanid][clanPoints] = 0;
    ClanInfo[clanid][clanKills] = 0;
    ClanMembersCount[clanid] = 0;

    for (new i = 0; i < MAX_LOCATIONS; i++)
    {
        ClanInfo[clanid][clanArenaKills][i] = 0;
        ClanInfo[clanid][clanArenaDMG][i] = 0;
    }

    DeleteClanRows(clanid);
    return 1;
}

stock SendMessageToClan(clanid, color, const message[])
{
    foreach (new i : Player)
    {
        if (User[i][clanID] == clanid) SendClientMessage(i, color, "%s", message);
    }
    return 1;
}

stock GetPlayerDisplayColour(playerid)
{
    if (IsValidClanID(User[playerid][clanID]))
        return (ClanInfo[User[playerid][clanID]][clanColor] >>> 8);

    return 0xB2ACAC;
}

stock SetClanColor(playerid)
{
    if (GetPVarInt(playerid, "HideTab") == 1)
    {
        SetPlayerColor(playerid, 0x00000000);
        return 1;
    }

    if (ClanLabel[playerid] != Text3D:INVALID_3DTEXT_ID)
    {
        DestroyDynamic3DTextLabel(ClanLabel[playerid]);
        ClanLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
    }

    if (VipLabel[playerid] != Text3D:INVALID_3DTEXT_ID)
    {
        DestroyDynamic3DTextLabel(VipLabel[playerid]);
        VipLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
    }

    if (User[playerid][clanID] != -1)
        SetPlayerColor(playerid, (ClanInfo[User[playerid][clanID]][clanColor] & 0xFFFFFF00) | 0x65);
    else
        SetPlayerColor(playerid, 0xB2ACAC65);

    if (GetPlayerState(playerid) != PLAYER_STATE_WASTED)
    {
        new bubble_text[256];
        bubble_text[0] = '\0';

        if (IsAdmin(playerid) && !User[playerid][hideAdminTag] && !IsAdminCamouflaged(playerid))
            format(bubble_text, sizeof(bubble_text), "{FF4500}%s", adminnames[User[playerid][adminLevel]][ingame]);

        if (!User[playerid][hideCustomTag] && User[playerid][customTag][0] != '\0')
        {
            if (bubble_text[0] != '\0')
                format(bubble_text, sizeof(bubble_text), "%s {FF4500}%s", bubble_text, User[playerid][customTag]);
            else
                format(bubble_text, sizeof(bubble_text), "{FF4500}%s", User[playerid][customTag]);
        }
        else
        {
            new vipStr[32];
            GetVipTagStringEx(playerid, vipStr, sizeof(vipStr));
            if (vipStr[0] != '\0')
            {
                new vlen = strlen(vipStr);
                if (vlen > 0 && vipStr[vlen - 1] == ' ') vipStr[vlen - 1] = '\0';

                if (bubble_text[0] != '\0')
                    format(bubble_text, sizeof(bubble_text), "%s %s", bubble_text, vipStr);
                else
                    format(bubble_text, sizeof(bubble_text), "%s", vipStr);
            }

            if (IsPlayerInClan(playerid) && !User[playerid][hideClanTag])
            {
                new clanid = GetPlayerClan(playerid);

                if (bubble_text[0] != '\0')
                    format(bubble_text, sizeof(bubble_text), "%s {%06x}[%s]", bubble_text, (ClanInfo[clanid][clanColor] >>> 8), ClanInfo[clanid][clanName]);
                else
                    format(bubble_text, sizeof(bubble_text), "{%06x}[%s]", (ClanInfo[clanid][clanColor] >>> 8), ClanInfo[clanid][clanName]);
            }
        }

        if (bubble_text[0] != '\0')
            ClanLabel[playerid] = CreateDynamic3DTextLabel(bubble_text, -1, 0.0, 0.0, 0.5, 30.0, .attachedplayer = playerid, .testlos = 0);
    }

    return 1;
}

stock GetRoundRank(location)
{
    if (location < 0 || location >= MAX_LOCATIONS) return 0;

    new bestDamage = 0, bestClan = -1;

    for (new i = 0; i < MAX_CLANS; i++)
    {
        if (!ClanInfo[i][clanIDUsed]) continue;

        if (ClanInfo[i][clanArenaDMG][location] > bestDamage)
        {
            bestDamage = ClanInfo[i][clanArenaDMG][location];
            bestClan = i;
        }
    }

    if (bestClan == -1) return 1;

    ClanInfo[bestClan][clanPoints] += CLAN_ROUND_POINTS;
    UpdateTOP10Clans();

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT * FROM `zones` WHERE `zoneid` = %d", arenaInfo[location][arenaId]);
    mysql_tquery(dbHandle, queryGlobal, "ZoneCaptured", "ddddd", arenaInfo[location][arenaId], bestClan, bestDamage, location, ClanInfo[bestClan][clanArenaKills][location]);
    return 1;
}

stock UpdateTOP10Clans()
{
    for (new i = 0; i < MAX_CLANS; i++)
    {
        if (ClanInfo[i][clanIDUsed])
        {
            ClanRank[i][rank_clanID] = ClanInfo[i][clanID];
            ClanRank[i][rank_clanPoints] = ClanInfo[i][clanPoints];
        }
        else
        {
            ClanRank[i][rank_clanID] = -1;
            ClanRank[i][rank_clanPoints] = -1;
        }
    }

    SortDeepArray(ClanRank, rank_clanPoints, .order = SORT_DESC);
    return 1;
}

public RewardClanTerritories()
{
    new clanReward[MAX_CLANS], clanTerritories[MAX_CLANS];

    for (new i = 0; i < MAX_ZONES; i++)
    {
        if (ZoneOwnerClan[i] == -1 || ZoneOwnerClan[i] >= MAX_CLANS) continue;

        clanTerritories[ZoneOwnerClan[i]]++;

        switch (arenaData[i][category])
        {
            case ARENA_CATEGORY_BIG:    clanReward[ZoneOwnerClan[i]] += CLAN_REWARD_BIG;
            case ARENA_CATEGORY_MEDIUM: clanReward[ZoneOwnerClan[i]] += CLAN_REWARD_MEDIUM;
            case ARENA_CATEGORY_SMALL:  clanReward[ZoneOwnerClan[i]] += CLAN_REWARD_SMALL;
        }
    }

    foreach (new i : Player)
    {
        if (!User[i][isLogged] || User[i][clanID] == -1) continue;

        new reward = clanReward[User[i][clanID]];
        if (reward <= 0) continue;

        new count = clanTerritories[User[i][clanID]];
        if (IsPlayerVip(i)) reward *= 2;

        GivePlayerCash(i, reward);

        if (!User[i][hideGlobalMsg])
        {
            va_SendClientMessage(i, -1, "{MAIN}[TERRITORIES] {FFFFFF}You received "COL_MONEY"%s {FFFFFF}for your clan holding %d territories! %s",
                FormatMoney(reward), count, (IsPlayerVip(i)) ? (""COL_MONEY"(2x VIP Bonus!)") : (""));
        }
    }
    return 1;
}
