#if defined _safehouse_upgrades_included
    #endinput
#endif
#define _safehouse_upgrades_included

new const g_SafehouseLevels[SAFEHOUSE_MAX_LEVEL][E_SAFEHOUSE_LEVEL] =
{
    {"Burglary House 13", 0,       0,     5,  5,  140.3679,  1367.8837,  1083.8621, 0.0},
    {"Burglary House 7",  500000,  11000,  10, 5,  227.7559,  1114.3844,  1080.9922, 0.0},
    {"Safehouse 4",       1000000, 22000, 15, 10, 2261.0977, -1137.8833, 1050.6328, 0.0},
    {"Johnson House",     1500000, 45000, 20, 3,  2496.0549, -1695.1749, 1014.7422, 0.0},
    {"Unused Safe House", 3000000, 90000, 25, 12, 2323.7063, -1147.6509, 1050.7101, 0.0}
};

stock bool:IsValidSafehouseLevel(level)
{
    return (1 <= level <= SAFEHOUSE_MAX_LEVEL);
}

stock GetSafehouseLevelSlots(level)
{
    if (!IsValidSafehouseLevel(level)) return 0;
    return g_SafehouseLevels[level - 1][lvl_Slots];
}

stock GetSafehouseLevelName(level, dest[], size = sizeof(dest))
{
    if (!IsValidSafehouseLevel(level))
    {
        format(dest, size, "None");
        return 0;
    }

    format(dest, size, "%s", g_SafehouseLevels[level - 1][lvl_Name]);
    return 1;
}

stock GetSafehouseSlotBonus(clanid)
{
    new index = GetSafehouseByClan(clanid);
    if (index == -1) return 0;

    return GetSafehouseLevelSlots(SafehouseData[index][sh_Level]);
}

stock ApplySafehouseLevel(index, level)
{
    if (!IsValidSafehouseIndex(index) || !IsValidSafehouseLevel(level))
        return 0;

    SafehouseData[index][sh_Level] = level;
    SafehouseData[index][sh_Interior] = g_SafehouseLevels[level - 1][lvl_Interior];
    SafehouseData[index][sh_IntX] = g_SafehouseLevels[level - 1][lvl_IntX];
    SafehouseData[index][sh_IntY] = g_SafehouseLevels[level - 1][lvl_IntY];
    SafehouseData[index][sh_IntZ] = g_SafehouseLevels[level - 1][lvl_IntZ];
    SafehouseData[index][sh_IntA] = g_SafehouseLevels[level - 1][lvl_IntA];
    return 1;
}

stock GetSafehouseUpgradePrice(level, &price, &coins)
{
    price = 0;
    coins = 0;

    if (!IsValidSafehouseLevel(level)) return 0;

    price = g_SafehouseLevels[level - 1][lvl_Price];
    coins = g_SafehouseLevels[level - 1][lvl_Coins];
    return 1;
}
