#if defined _safehouse_config_included
    #endinput
#endif
#define _safehouse_config_included

// one safehouse per map zone declared in arenaData
#define MAX_SAFEHOUSES          MAX_ZONES

#define SAFEHOUSE_NAME_LEN      (32)

#define SAFEHOUSE_PASS_LEN      (32)
#define SAFEHOUSE_PASS_MIN      (3)
#define SAFEHOUSE_PASS_MAX      (16)

#define SAFEHOUSE_BUY_PRICE     (1000000)
#define SAFEHOUSE_BUY_COINS     (68000)

#define SAFEHOUSE_DURATION_DAYS (15)
#define SAFEHOUSE_CHARGE_DAYS   (4)
#define SAFEHOUSE_CHARGE_VALUE  (50000)

// gives setAfk and the gamemode spawn time to finish before the base move
#define SAFEHOUSE_SPAWN_DELAY   (500)

// how often the server looks for due charges and expirations
#define SAFEHOUSE_TICK_INTERVAL (60000)

// property_green / property_locked
#define SAFEHOUSE_PICKUP_FREE   (1273)
#define SAFEHOUSE_PICKUP_OWNED  (1272)
#define SAFEHOUSE_VW_BASE       (70000)

// radar_propertyG (green house) / radar_propertyR (red house)
#define SAFEHOUSE_ICON_FREE     (31)
#define SAFEHOUSE_ICON_OWNED    (32)


#define SAFEHOUSE_MAX_LEVEL     (5)


#define SAFEHOUSE_HISTORY_ROWS  (15)

#define SAFEHOUSE_REFUND_PCT    (50)

#define SAFEHOUSE_RENEW_DAYS    (7)
#define SAFEHOUSE_RENEW_COINS   (34000)

enum
{
    SAFEHOUSE_STATUS_AVAILABLE = 0,
    SAFEHOUSE_STATUS_OWNED
}

enum
{
    SHCFG_ICON_DISTANCE,
    SHCFG_LABEL_DISTANCE,
    SHCFG_BUY_PRICE,
    SHCFG_BUY_COINS,
    SHCFG_DURATION_DAYS,
    SHCFG_CHARGE_DAYS,
    SHCFG_CHARGE_VALUE,
    SHCFG_REFUND_PCT,
    SHCFG_RENEW_DAYS,
    SHCFG_RENEW_COINS,
    SHCFG_MAX
}

new g_SafehouseConfig[SHCFG_MAX];

new const g_SafehouseConfigKey[SHCFG_MAX][] =
{
    "icon_distance", "label_distance", "buy_price", "buy_coins",
    "duration_days", "charge_days", "charge_value", "refund_pct",
    "renew_days", "renew_coins"
};

new const g_SafehouseConfigLabel[SHCFG_MAX][] =
{
    "Icon view distance",
    "Label draw distance",
    "Purchase price",
    "Purchase coins",
    "Contract duration (days)",
    "Maintenance interval (days)",
    "Maintenance charge",
    "Refund on sell (%)",
    "Renewal length (days)",
    "Renewal cost (coins)"
};

new const g_SafehouseConfigDefault[SHCFG_MAX] =
{
    500, 15, SAFEHOUSE_BUY_PRICE, SAFEHOUSE_BUY_COINS,
    SAFEHOUSE_DURATION_DAYS, SAFEHOUSE_CHARGE_DAYS, SAFEHOUSE_CHARGE_VALUE, SAFEHOUSE_REFUND_PCT,
    SAFEHOUSE_RENEW_DAYS, SAFEHOUSE_RENEW_COINS
};

new const g_SafehouseConfigMin[SHCFG_MAX] = { 10,   5,  0, 0, 1, 1, 0, 0, 1, 0 };
new const g_SafehouseConfigMax[SHCFG_MAX] = { 6000, 300, 2000000000, 2000000000, 365, 365, 2000000000, 100, 365, 2000000000 };

#define SH_ICON_DISTANCE    (float(g_SafehouseConfig[SHCFG_ICON_DISTANCE]))
#define SH_LABEL_DISTANCE   (float(g_SafehouseConfig[SHCFG_LABEL_DISTANCE]))
#define SH_BUY_PRICE        (g_SafehouseConfig[SHCFG_BUY_PRICE])
#define SH_BUY_COINS        (g_SafehouseConfig[SHCFG_BUY_COINS])
#define SH_DURATION         (g_SafehouseConfig[SHCFG_DURATION_DAYS] * 86400)
#define SH_CHARGE_EVERY     (g_SafehouseConfig[SHCFG_CHARGE_DAYS] * 86400)
#define SH_CHARGE_VALUE     (g_SafehouseConfig[SHCFG_CHARGE_VALUE])
#define SH_REFUND_PCT       (g_SafehouseConfig[SHCFG_REFUND_PCT])
#define SH_RENEW_DAYS       (g_SafehouseConfig[SHCFG_RENEW_DAYS])
#define SH_RENEW_COINS      (g_SafehouseConfig[SHCFG_RENEW_COINS])

#define SAFEHOUSE_VAULT_RANK    RANK_COFOUNDER
