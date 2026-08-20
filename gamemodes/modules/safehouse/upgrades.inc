#if defined _safehouse_upgrades_included
    #endinput
#endif
#define _safehouse_upgrades_included

new const g_SafehouseLevels[SAFEHOUSE_MAX_LEVEL][E_SAFEHOUSE_LEVEL] =
{
    {"Angel Pine Trailer", 0,       0,     5,  2, 1.1853,   -3.2387,    999.4284,  87.5718},
    {"Burglary House 1",   500000,  11000, 10, 3, 234.6087, 1187.8195,  1080.2578, 349.4844},
    {"Burglary House 2",   1000000, 22000, 15, 2, 225.5707, 1240.0643,  1082.1406, 96.2852},
    {"Burglary House 14",  1500000, 45000, 20, 6, 234.2826, 1065.2290,  1084.2101, 4.3864},
    {"Below The Belt Gym", 3000000, 90000, 25, 7, 773.7318, -74.6957,   1000.6542, 5.2304}
};

#assert SAFEHOUSE_MAX_LEVEL == sizeof(g_SafehouseLevels)

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
