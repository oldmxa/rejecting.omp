#if defined _safehouse_functions_included
    #endinput
#endif
#define _safehouse_functions_included

stock IsValidSafehouseIndex(index)
{
    return (0 <= index < MAX_SAFEHOUSES) ? 1 : 0;
}

stock SafehouseExists(index)
{
    if (!IsValidSafehouseIndex(index)) return 0;
    return (SafehouseData[index][sh_SQLID] > 0) ? 1 : 0;
}

stock GetSafehouseZoneAtPosition(Float:x, Float:y)
{
    for (new i = 0; i < MAX_SAFEHOUSES; i++)
    {
        if (!IsSafehouseZone(i)) continue;

        if (x < arenaData[i][arenaPos][0] || x > arenaData[i][arenaPos][2]) continue;
        if (y < arenaData[i][arenaPos][1] || y > arenaData[i][arenaPos][3]) continue;

        return i;
    }
    return -1;
}

stock ClampSafehouseConfig(setting, value)
{
    if (setting < 0 || setting >= SHCFG_MAX) return value;

    if (value < g_SafehouseConfigMin[setting]) return g_SafehouseConfigMin[setting];
    if (value > g_SafehouseConfigMax[setting]) return g_SafehouseConfigMax[setting];
    return value;
}

stock IsSafehouseZone(zoneid)
{
    if (!IsValidSafehouseIndex(zoneid)) return 0;

    switch (arenaData[zoneid][category])
    {
        case ARENA_CATEGORY_MEDIUM, ARENA_CATEGORY_BIG: return 1;
    }
    return 0;
}

stock bool:SanitizeSafehouseText(const input[], dest[], size = sizeof(dest), minLen = 2)
{
    return SanitizeString(input, dest, size, minLen);
}

stock FormatSafehouseTime(seconds, dest[], size = sizeof(dest))
{
    if (seconds <= 0)
    {
        format(dest, size, "expired");
        return 0;
    }

    new days = seconds / 86400;
    new hours = (seconds % 86400) / 3600;
    new minutes = (seconds % 3600) / 60;

    if (days > 0)
        format(dest, size, "%dd %dh", days, hours);
    else if (hours > 0)
        format(dest, size, "%dh %dm", hours, minutes);
    else
        format(dest, size, "%dm", minutes);

    return 1;
}

stock GetSafehouseTimeLeft(index)
{
    if (!IsValidSafehouseIndex(index)) return 0;

    new left = SafehouseData[index][sh_ExpiresAt] - gettime();
    return (left > 0) ? left : 0;
}

stock GetSafehouseNextChargeIn(index)
{
    if (!IsValidSafehouseIndex(index)) return 0;

    new left = SafehouseData[index][sh_NextCharge] - gettime();
    return (left > 0) ? left : 0;
}

stock RenewSafehouse(playerid, index)
{
    if (!IsValidSafehouseIndex(index)) return 0;

    if (!IsSafehouseMember(playerid, index))
        return SendClientMessage(playerid, -1, "%s", SH_NOT_MEMBER);

    if (User[playerid][playerCoins] < SH_RENEW_COINS)
        return va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You need %s to extend the contract.", FormatCoins(SH_RENEW_COINS));

    new extra = SH_RENEW_DAYS * 86400;
    if (GetSafehouseTimeLeft(index) + extra > SH_DURATION)
        return va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"The contract cannot go past %d days. Renew closer to the expiry.", SH_DURATION / 86400);

    GivePlayerCoins(playerid, -SH_RENEW_COINS, "Safehouse renew");
    UpdatePlayerData(playerid);

    SafehouseData[index][sh_ExpiresAt] += extra;
    SaveSafehouse(index);

    new detail[96];
    format(detail, sizeof(detail), "+%d days for C$%d", SH_RENEW_DAYS, SH_RENEW_COINS);
    LogSafehouse(index, playerid, "renew", detail);

    new leftStr[24];
    FormatSafehouseTime(GetSafehouseTimeLeft(index), leftStr);

    SendSafehouseClanMessage(index, SAFEHOUSE_PREFIX"%s extended the contract by {MAIN}%d {FFFFFF}day(s). Time left: {MAIN}%s{FFFFFF}.",
        User[playerid][userName], SH_RENEW_DAYS, leftStr);
    return 1;
}

stock GetSafehouseByClan(clanid)
{
    if (!IsValidClanID(clanid)) return -1;

    for (new i = 0; i < MAX_SAFEHOUSES; i++)
    {
        if (SafehouseData[i][sh_Status] == SAFEHOUSE_STATUS_OWNED &&
            SafehouseData[i][sh_ClanID] == clanid)
            return i;
    }
    return -1;
}

public OnClanDeleted(clanid)
{
    new index = GetSafehouseByClan(clanid);
    if (index != -1) ReleaseSafehouse(index, "owner clan was deleted");
    return 1;
}

stock GetPlayerSafehouse(playerid)
{
    return GetSafehouseByClan(User[playerid][clanID]);
}

stock bool:IsSafehouseMember(playerid, index)
{
    if (!IsValidSafehouseIndex(index)) return false;
    if (SafehouseData[index][sh_Status] != SAFEHOUSE_STATUS_OWNED) return false;
    if (!IsClanMember(playerid)) return false;

    return (User[playerid][clanID] == SafehouseData[index][sh_ClanID]);
}

stock bool:IsSafehouseLeader(playerid, index)
{
    if (!IsSafehouseMember(playerid, index)) return false;
    return IsClanLeader(playerid);
}

stock bool:CanUseSafehouseVault(playerid, index)
{
    if (!IsSafehouseMember(playerid, index)) return false;
    return HasClanRank(playerid, SAFEHOUSE_VAULT_RANK);
}

stock ResetSafehouse(index)
{
    if (!IsValidSafehouseIndex(index)) return 0;

    DestroySafehousePickup(index);

    SafehouseData[index][sh_SQLID] = 0;
    SafehouseData[index][sh_ClanID] = -1;
    format(SafehouseData[index][sh_Name], SAFEHOUSE_NAME_LEN, "%s", arenaData[index][arenaName]);
    SafehouseData[index][sh_Password][0] = '\0';
    SafehouseData[index][sh_Level] = 1;
    SafehouseData[index][sh_Interior] = g_SafehouseLevels[0][lvl_Interior];
    SafehouseData[index][sh_VirtualWorld] = SAFEHOUSE_VW_BASE + index;

    SafehouseData[index][sh_ExtX] = 0.0;
    SafehouseData[index][sh_ExtY] = 0.0;
    SafehouseData[index][sh_ExtZ] = 0.0;

    SafehouseData[index][sh_IntX] = g_SafehouseLevels[0][lvl_IntX];
    SafehouseData[index][sh_IntY] = g_SafehouseLevels[0][lvl_IntY];
    SafehouseData[index][sh_IntZ] = g_SafehouseLevels[0][lvl_IntZ];
    SafehouseData[index][sh_IntA] = g_SafehouseLevels[0][lvl_IntA];

    SafehouseData[index][sh_ExitX] = 0.0;
    SafehouseData[index][sh_ExitY] = 0.0;
    SafehouseData[index][sh_ExitZ] = 0.0;

    SafehouseData[index][sh_Vault] = 0;
    SafehouseData[index][sh_BoughtAt] = 0;
    SafehouseData[index][sh_ExpiresAt] = 0;
    SafehouseData[index][sh_NextCharge] = 0;
    SafehouseData[index][sh_LastCharge] = 0;
    SafehouseData[index][sh_Status] = SAFEHOUSE_STATUS_AVAILABLE;
    SafehouseData[index][sh_ExpiringShown] = false;
    return 1;
}

stock DestroySafehousePickup(index)
{
    if (SafehouseData[index][sh_Pickup] != STREAMER_TAG_PICKUP:0 &&
        IsValidDynamicPickup(SafehouseData[index][sh_Pickup]))
    {
        DestroyDynamicPickup(SafehouseData[index][sh_Pickup]);
    }
    SafehouseData[index][sh_Pickup] = STREAMER_TAG_PICKUP:0;

    if (SafehouseData[index][sh_MapIcon] != STREAMER_TAG_MAP_ICON:0 &&
        IsValidDynamicMapIcon(SafehouseData[index][sh_MapIcon]))
    {
        DestroyDynamicMapIcon(SafehouseData[index][sh_MapIcon]);
    }
    SafehouseData[index][sh_MapIcon] = STREAMER_TAG_MAP_ICON:0;

    if (SafehouseData[index][sh_Label] != Text3D:INVALID_3DTEXT_ID)
    {
        DestroyDynamic3DTextLabel(SafehouseData[index][sh_Label]);
        SafehouseData[index][sh_Label] = Text3D:INVALID_3DTEXT_ID;
    }
    return 1;
}

stock bool:IsSafehouseExpiring(index)
{
    if (!IsValidSafehouseIndex(index)) return false;
    if (SafehouseData[index][sh_Status] != SAFEHOUSE_STATUS_OWNED) return false;
    if (SafehouseData[index][sh_ExpiresAt] <= 0) return false;

    new left = SafehouseData[index][sh_ExpiresAt] - gettime();
    return (left > 0 && left <= SAFEHOUSE_EXPIRING_WINDOW);
}

stock GetSafehousePickupModel(index)
{
    if (SafehouseData[index][sh_Status] != SAFEHOUSE_STATUS_OWNED)
        return SAFEHOUSE_PICKUP_FREE;

    if (IsSafehouseExpiring(index))
        return SAFEHOUSE_PICKUP_EXPIRING;

    return SAFEHOUSE_PICKUP_OWNED;
}

stock GetSafehouseMapIcon(index)
{
    if (SafehouseData[index][sh_Status] != SAFEHOUSE_STATUS_OWNED)
        return SAFEHOUSE_ICON_FREE;

    if (IsSafehouseExpiring(index))
        return SAFEHOUSE_ICON_EXPIRING;

    return SAFEHOUSE_ICON_OWNED;
}

stock RefreshSafehousePickup(index)
{
    if (!IsValidSafehouseIndex(index)) return 0;

    DestroySafehousePickup(index);

    SafehouseData[index][sh_ExpiringShown] = IsSafehouseExpiring(index);

    if (!IsSafehouseZone(index) || !SafehouseExists(index)) return 0;

    new Float:x = SafehouseData[index][sh_ExtX];
    new Float:y = SafehouseData[index][sh_ExtY];
    new Float:z = SafehouseData[index][sh_ExtZ];

    if (z == 0.0) return 0;

    new bool:owned = (SafehouseData[index][sh_Status] == SAFEHOUSE_STATUS_OWNED);

    SafehouseData[index][sh_Pickup] = CreateDynamicPickup(
        GetSafehousePickupModel(index), 1, x, y, z, -1);
    SafehouseData[index][sh_MapIcon] = CreateDynamicMapIcon(x, y, z,
        GetSafehouseMapIcon(index),
        0, -1, -1, -1, SH_ICON_DISTANCE, MAPICON_LOCAL);

    new interiorName[24];
    GetSafehouseLevelName(SafehouseData[index][sh_Level], interiorName);

    // FormatMoney returns an array, so each value needs its own buffer
    new upkeepStr[36];
    format(upkeepStr, sizeof(upkeepStr), "%s", FormatMoney(SH_CHARGE_VALUE));

    new label[320];

    if (owned)
    {
        new clanid = SafehouseData[index][sh_ClanID];
        new ownerName[MAX_CLAN_NAME + 1] = "?";

        if (IsValidClanID(clanid))
            format(ownerName, sizeof(ownerName), "%s", ClanInfo[clanid][clanName]);

        format(label, sizeof(label),
            "{E67E22}%s's Safehouse (ID: %d)\n\n{FFFFFF}%s\n{FFFFFF}Interior: {3498DB}%s\n\
{FFFFFF}Members: {2ECC71}%d{FFFFFF}/{2ECC71}%d\n\
{FFFFFF}Upkeep {2ECC71}%s {FFFFFF}every {2ECC71}%d {FFFFFF}days.",
            ownerName, index,
            SafehouseData[index][sh_Name],
            interiorName,
            IsValidClanID(clanid) ? ClanMembersCount[clanid] : 0,
            GetClanMemberSlots(clanid),
            upkeepStr,
            SH_CHARGE_EVERY / 86400);
    }
    else
    {
        new priceStr[36], coinStr[36];
        format(priceStr, sizeof(priceStr), "%s", FormatMoney(SH_BUY_PRICE));
        format(coinStr, sizeof(coinStr), "%s", FormatCoins(SH_BUY_COINS));

        format(label, sizeof(label),
            "{2ECC71}Safehouse For Sale (ID: %d)\n\n{FFFFFF}%s\n{FFFFFF}Interior: {3498DB}%s\n\
{2ECC71}%s {FFFFFF}+ {2ECC71}%s\n\
{FFFFFF}Upkeep {2ECC71}%s {FFFFFF}every {2ECC71}%d {FFFFFF}days.",
            index,
            SafehouseData[index][sh_Name],
            interiorName,
            priceStr, coinStr,
            upkeepStr,
            SH_CHARGE_EVERY / 86400);
    }

    SafehouseData[index][sh_Label] = CreateDynamic3DTextLabel(
        label, -1, x, y, z + 0.7, SH_LABEL_DISTANCE,
        .testlos = 1
    );

    foreach (new i : Player)
    {
        if (!IsPlayerInRangeOfPoint(i, SH_ICON_DISTANCE, x, y, z)) continue;

        Streamer_Update(i, STREAMER_TYPE_PICKUP);
        Streamer_Update(i, STREAMER_TYPE_MAP_ICON);
        Streamer_Update(i, STREAMER_TYPE_3D_TEXT_LABEL);
    }

    return 1;
}

stock RefreshAllSafehousePickups()
{
    new placed = 0;

    for (new i = 0; i < MAX_SAFEHOUSES; i++)
    {
        if (RefreshSafehousePickup(i)) placed++;
    }
    return placed;
}

stock GetSafehouseAtPosition(playerid, Float:range = 2.0)
{
    for (new i = 0; i < MAX_SAFEHOUSES; i++)
    {
        if (SafehouseData[i][sh_ExtZ] == 0.0) continue;

        if (IsPlayerInRangeOfPoint(playerid, range,
            SafehouseData[i][sh_ExtX], SafehouseData[i][sh_ExtY], SafehouseData[i][sh_ExtZ]))
            return i;
    }
    return -1;
}

stock BuySafehouse(playerid, index)
{
    if (!IsValidSafehouseIndex(index)) return 0;

    if (SafehouseData[index][sh_Status] == SAFEHOUSE_STATUS_OWNED)
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"This safehouse already belongs to a clan.");

    new clanid = User[playerid][clanID];
    if (!IsValidClanID(clanid))
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You need to be in a clan to buy a safehouse.");

    if (!IsClanLeader(playerid))
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Only the clan leader can buy a safehouse.");

    if (GetSafehouseByClan(clanid) != -1)
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Your clan already owns a safehouse.");

    if (!HasPlayerCash(playerid, SH_BUY_PRICE))
        return va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You need %s to buy this safehouse.", FormatMoney(SH_BUY_PRICE));

    if (!HasPlayerCoins(playerid, SH_BUY_COINS))
        return va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You need %s to buy this safehouse.", FormatCoins(SH_BUY_COINS));

    GivePlayerCash(playerid, -SH_BUY_PRICE);
    GivePlayerCoins(playerid, -SH_BUY_COINS, "Safehouse purchase");

    UpdatePlayerData(playerid);

    new timestamp = gettime();

    SafehouseData[index][sh_ClanID] = clanid;
    SafehouseData[index][sh_Status] = SAFEHOUSE_STATUS_OWNED;
    SafehouseData[index][sh_BoughtAt] = timestamp;
    SafehouseData[index][sh_ExpiresAt] = timestamp + SH_DURATION;
    SafehouseData[index][sh_NextCharge] = timestamp + SH_CHARGE_EVERY;
    SafehouseData[index][sh_LastCharge] = 0;
    SafehouseData[index][sh_Vault] = 0;
    SafehouseData[index][sh_Password][0] = '\0';
    ApplySafehouseLevel(index, 1);

    SaveSafehouse(index);
    RefreshSafehousePickup(index);

    new detail[96];
    format(detail, sizeof(detail), "price $%d + C$%d", SH_BUY_PRICE, SH_BUY_COINS);
    LogSafehouse(index, playerid, "buy", detail);

    SendSafehouseClanMessage(index, SAFEHOUSE_PREFIX"Your clan bought the safehouse at {MAIN}%s{FFFFFF}!", SafehouseData[index][sh_Name]);
    return 1;
}

stock SellSafehouse(playerid, index)
{
    if (!IsValidSafehouseIndex(index)) return 0;
    if (!IsSafehouseLeader(playerid, index))
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Only the clan leader can sell the safehouse.");

    new refund = (SH_BUY_PRICE / 100) * SH_REFUND_PCT + ((SH_BUY_PRICE % 100) * SH_REFUND_PCT) / 100;
    refund += SafehouseData[index][sh_Vault];

    GivePlayerCash(playerid, refund);
    UpdatePlayerData(playerid);

    new detail[96];
    format(detail, sizeof(detail), "refund %d (vault %d)", refund, SafehouseData[index][sh_Vault]);
    LogSafehouse(index, playerid, "sell", detail);

    SendSafehouseClanMessage(index, SAFEHOUSE_PREFIX"Your clan sold the safehouse at {MAIN}%s{FFFFFF}.", SafehouseData[index][sh_Name]);

    va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You received {23C552}%s{FFFFFF} back.", FormatMoney(refund));

    ReleaseSafehouse(index, "sold by leader");
    return 1;
}
stock ReleaseSafehouse(index, const reason[])
{
    if (!IsValidSafehouseIndex(index)) return 0;

    new oldClanID = SafehouseData[index][sh_ClanID];

    foreach (new i : Player)
    {
        if (g_PlayerInSafehouse[i] == index)
            ExitSafehouse(i);

        if (oldClanID != -1 && IsPlayerLogged(i) && User[i][clanID] == oldClanID)
        {
            if (User[i][spawnSafehouse])
            {
                User[i][spawnSafehouse] = false;
                UpdatePlayerData(i);
            }
        }
    }

    if (oldClanID != -1)
    {
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `accounts` SET `spawn_safehouse` = 0 WHERE `clanID` = %d", oldClanID);
        mysql_tquery(dbHandle, queryGlobal);
    }

    SafehouseData[index][sh_ClanID] = -1;
    format(SafehouseData[index][sh_Name], SAFEHOUSE_NAME_LEN, "%s", arenaData[index][arenaName]);
    SafehouseData[index][sh_Password][0] = '\0';
    SafehouseData[index][sh_Vault] = 0;
    SafehouseData[index][sh_BoughtAt] = 0;
    SafehouseData[index][sh_ExpiresAt] = 0;
    SafehouseData[index][sh_NextCharge] = 0;
    SafehouseData[index][sh_LastCharge] = 0;
    SafehouseData[index][sh_Status] = SAFEHOUSE_STATUS_AVAILABLE;
    SafehouseData[index][sh_ExpiringShown] = false;
    ApplySafehouseLevel(index, 1);

    SaveSafehouse(index);
    RefreshSafehousePickup(index);
    LogSafehouse(index, INVALID_PLAYER_ID, "release", reason);
    return 1;
}

stock IsPlayerInSafehouseCombat(playerid)
{
    return (gettime() < User[playerid][lastDamageTime]) ? 1 : 0;
}

stock EnterSafehouse(playerid, index)
{
    if (!IsValidSafehouseIndex(index)) return 0;

    new loc = User[playerid][locationId];

    if (loc != -1 && loc != ARENA_AFK)
    {
        ApplyArenaRpWalk(playerid, loc, ARENA_AFK);

        User[playerid][isAfk] = true;
        User[playerid][locationId] = ARENA_AFK;

        GangZoneStopPulseForPlayer(playerid, e_ZONE_PULSE_ZONE, arenaInfo[loc][arenaId]);
        HideRoundStatsTD(playerid);
        updatePlayerHud(playerid);
        CallLocalFunction("ResetPlayerOutpostCapture", "i", playerid);
    }

    SetPlayerInterior(playerid, SafehouseData[index][sh_Interior]);
    SetPlayerVirtualWorld(playerid, SafehouseData[index][sh_VirtualWorld]);
    SetPlayerPos(playerid,
        SafehouseData[index][sh_IntX],
        SafehouseData[index][sh_IntY],
        SafehouseData[index][sh_IntZ]);
    SetPlayerFacingAngle(playerid, SafehouseData[index][sh_IntA]);
    SetCameraBehindPlayer(playerid);

    g_PlayerInSafehouse[playerid] = index;

    if (IsSafehouseMember(playerid, index))
        va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You entered {MAIN}%s{FFFFFF}. Type {MAIN}/safemenu{FFFFFF} to manage it.", SafehouseData[index][sh_Name]);
    else
        va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You entered {MAIN}%s{FFFFFF} as a guest. Type {MAIN}/exitsafehouse{FFFFFF} to leave.", SafehouseData[index][sh_Name]);

    return 1;
}

stock ExitSafehouse(playerid)
{
    new index = g_PlayerInSafehouse[playerid];
    if (!IsValidSafehouseIndex(index)) return 0;

    g_PlayerInSafehouse[playerid] = -1;

    if (!IsPlayerConnected(playerid)) return 1;

    new Float:x = SafehouseData[index][sh_ExitX];
    new Float:y = SafehouseData[index][sh_ExitY];
    new Float:z = SafehouseData[index][sh_ExitZ];

    if (z == 0.0)
    {
        x = SafehouseData[index][sh_ExtX];
        y = SafehouseData[index][sh_ExtY];
        z = SafehouseData[index][sh_ExtZ];
    }

    new world = 0;
    if (IsPlayerAFK(playerid) && User[playerid][locationId] == ARENA_AFK)
        world = User[playerid][loungeVW];

    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, world);
    SetPlayerPos(playerid, x, y, z);
    SetCameraBehindPlayer(playerid);
    return 1;
}

stock SendSafehouseClanMessage(index, const message[], va_args<>)
{
    if (!IsValidSafehouseIndex(index)) return 0;

    new clanid = SafehouseData[index][sh_ClanID];
    if (!IsValidClanID(clanid)) return 0;

    new formatted[192];
    va_format(formatted, sizeof(formatted), message, va_start<2>);

    foreach (new i : Player)
    {
        if (IsPlayerLogged(i) && User[i][clanID] == clanid)
            SendClientMessage(i, -1, "%s", formatted);
    }
    return 1;
}

stock SafehouseVaultDeposit(playerid, index, amount)
{
    if (amount <= 0) return SendClientMessage(playerid, -1, "%s", SH_INVALID_AMOUNT);

    if (amount > 100000000)
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Maximum deposit limit per transaction is $100,000,000.");

    if (!HasPlayerCash(playerid, amount))
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You do not have that much money.");

    if (SafehouseData[index][sh_Vault] + amount > 2000000000)
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"The vault cannot hold any more money (capacity reached).");

    GivePlayerCash(playerid, -amount);

    SafehouseData[index][sh_Vault] += amount;
    SaveSafehouse(index);

    new detail[64];
    format(detail, sizeof(detail), "amount %d, vault now %d", amount, SafehouseData[index][sh_Vault]);
    LogSafehouse(index, playerid, "vault_deposit", detail);

    SendSafehouseClanMessage(index, SAFEHOUSE_PREFIX"%s deposited {23C552}%s{FFFFFF} into the vault.",
        User[playerid][userName], FormatMoney(amount));
    return 1;
}

stock SafehouseVaultWithdraw(playerid, index, amount)
{
    if (amount <= 0) return SendClientMessage(playerid, -1, "%s", SH_INVALID_AMOUNT);

    if (amount > 100000000)
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Maximum withdrawal limit per transaction is $100,000,000.");

    if (SafehouseData[index][sh_Vault] < amount)
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"The vault does not have that much money.");

    if (User[playerid][playerMoney] + amount > 2000000000)
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Your wallet cannot hold any more money.");

    SafehouseData[index][sh_Vault] -= amount;
    SaveSafehouse(index);

    GivePlayerCash(playerid, amount);

    new detail[64];
    format(detail, sizeof(detail), "amount %d, vault now %d", amount, SafehouseData[index][sh_Vault]);
    LogSafehouse(index, playerid, "vault_withdraw", detail);

    SendSafehouseClanMessage(index, SAFEHOUSE_PREFIX"%s withdrew {F3230E}%s{FFFFFF} from the vault.",
        User[playerid][userName], FormatMoney(amount));
    return 1;
}

stock UpgradeSafehouse(playerid, index)
{
    if (!IsSafehouseLeader(playerid, index))
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Only the clan leader can upgrade the safehouse.");

    new next = SafehouseData[index][sh_Level] + 1;
    if (!IsValidSafehouseLevel(next))
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Your safehouse is already at the maximum level.");

    new price, coins;
    GetSafehouseUpgradePrice(next, price, coins);

    if (!HasPlayerCash(playerid, price))
        return va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You need %s for this upgrade.", FormatMoney(price));

    if (!HasPlayerCoins(playerid, coins))
        return va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You need %s for this upgrade.", FormatCoins(coins));

    GivePlayerCash(playerid, -price);
    GivePlayerCoins(playerid, -coins, "Safehouse upgrade");

    UpdatePlayerData(playerid);

    ApplySafehouseLevel(index, next);
    SaveSafehouse(index);

    foreach (new i : Player)
    {
        if (g_PlayerInSafehouse[i] == index)
            EnterSafehouse(i, index);
    }

    new levelName[24];
    GetSafehouseLevelName(next, levelName);

    new detail[96];
    format(detail, sizeof(detail), "level %d (%s), cost $%d + C$%d", next, levelName, price, coins);
    LogSafehouse(index, playerid, "upgrade", detail);

    SendSafehouseClanMessage(index, SAFEHOUSE_PREFIX"The safehouse was upgraded to {MAIN}%s{FFFFFF} ({MAIN}%d{FFFFFF} member slots).",
        levelName, GetClanMemberSlots(SafehouseData[index][sh_ClanID]));
    return 1;
}

stock ProcessSafehouseCharge(index)
{
    if (SafehouseData[index][sh_Status] != SAFEHOUSE_STATUS_OWNED) return 0;

    if (SafehouseData[index][sh_Vault] < SH_CHARGE_VALUE)
    {
        SendSafehouseClanMessage(index, SAFEHOUSE_PREFIX"The vault could not cover the {F3230E}%s{FFFFFF} maintenance. The safehouse was lost.",
            FormatMoney(SH_CHARGE_VALUE));

        LogSafehouse(index, INVALID_PLAYER_ID, "expire", "vault could not cover maintenance");
        ReleaseSafehouse(index, "unpaid maintenance");
        return 1;
    }

    new timestamp = gettime();

    SafehouseData[index][sh_Vault] -= SH_CHARGE_VALUE;
    SafehouseData[index][sh_LastCharge] = timestamp;
    SafehouseData[index][sh_NextCharge] = timestamp + SH_CHARGE_EVERY;
    SaveSafehouse(index);

    new detail[80];
    format(detail, sizeof(detail), "charged %d, vault now %d", SH_CHARGE_VALUE, SafehouseData[index][sh_Vault]);
    LogSafehouse(index, INVALID_PLAYER_ID, "maintenance", detail);

    SendSafehouseClanMessage(index, SAFEHOUSE_PREFIX"Maintenance of {F3230E}%s{FFFFFF} charged. Vault: {MAIN}%s",
        FormatMoney(SH_CHARGE_VALUE), FormatMoney(SafehouseData[index][sh_Vault]));
    return 1;
}

public SafehouseTick()
{
    new timestamp = gettime();

    for (new i = 0; i < MAX_SAFEHOUSES; i++)
    {
        if (SafehouseData[i][sh_Status] != SAFEHOUSE_STATUS_OWNED) continue;

        if (SafehouseData[i][sh_ExpiresAt] > 0 && timestamp >= SafehouseData[i][sh_ExpiresAt])
        {
            SendSafehouseClanMessage(i, SAFEHOUSE_PREFIX"The safehouse contract expired and the base was released.");
            LogSafehouse(i, INVALID_PLAYER_ID, "expire", "contract reached its end date");
            ReleaseSafehouse(i, "contract expired");
            continue;
        }

        if (SafehouseData[i][sh_NextCharge] > 0 && timestamp >= SafehouseData[i][sh_NextCharge])
        {
            ProcessSafehouseCharge(i);
            continue;
        }

        if (IsSafehouseExpiring(i) != SafehouseData[i][sh_ExpiringShown])
        {
            RefreshSafehousePickup(i);

            if (SafehouseData[i][sh_ExpiringShown])
            {
                SendSafehouseClanMessage(i, SAFEHOUSE_PREFIX"Less than {F3230E}24 hours{FFFFFF} left on the safehouse contract. Renew it before it expires.");
                LogSafehouse(i, INVALID_PLAYER_ID, "expiring", "less than 24h left on the contract");
            }
        }
    }
    return 1;
}
