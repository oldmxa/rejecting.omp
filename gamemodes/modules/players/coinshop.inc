#if defined _coinshop_included
    #endinput
#endif
#define _coinshop_included

#define COINSHOP_COINS (7000)
#define COINSHOP_MONEY (2000000)

#define COINSHOP_MAX   (100000)

stock GetCoinsPrice(coins)
{
    return (coins * (COINSHOP_MONEY / 1000)) / (COINSHOP_COINS / 1000);
}

@cmd(.alts = {"comprarcoins"}) buycoins(playerid, params[], help)
{
    if (!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    new coins;

    if (sscanf(params, "d", coins))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/buycoins [amount of coins]");
        va_SendClientMessage(playerid, -1, "{MAIN}[SHOP] {FFFFFF}Rate: {FFFF00}%s {FFFFFF}costs {23C552}%s",
            FormatCoins(COINSHOP_COINS), FormatMoney(COINSHOP_MONEY));

        return va_SendClientMessage(playerid, -1, "{MAIN}[SHOP] {FFFFFF}You have {23C552}%s {FFFFFF}and {FFFF00}%s",
            FormatMoney(User[playerid][playerMoney]), FormatCoins(User[playerid][playerCoins]));
    }

    if (coins < 1 || coins > COINSHOP_MAX)
        return va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Buy between {FFFF00}1 {FFFFFF}and {FFFF00}%s {FFFFFF}per order.", FormatCoins(COINSHOP_MAX));

    new price = GetCoinsPrice(coins);

    if (price < 1)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}That amount is too small to charge for.");

    if (User[playerid][playerMoney] < price)
    {
        return va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need {23C552}%s {FFFFFF}more to buy {FFFF00}%s{FFFFFF}.",
            FormatMoney(price - User[playerid][playerMoney]), FormatCoins(coins));
    }

    User[playerid][playerMoney] -= price;
    GivePlayerMoney(playerid, -price);

    User[playerid][playerCoins] += coins;

    LogCoins(User[playerid][user_sqlid], User[playerid][userName], "Bought with cash", coins, User[playerid][playerCoins]);
    UpdatePlayerData(playerid);

    va_SendClientMessage(playerid, -1, "{MAIN}[SHOP] {FFFFFF}You exchanged {23C552}%s {FFFFFF}for {FFFF00}%s{FFFFFF}.",
        FormatMoney(price), FormatCoins(coins));

    va_SendClientMessage(playerid, -1, "{MAIN}[SHOP] {FFFFFF}Balance: {23C552}%s {FFFFFF}and {FFFF00}%s",
        FormatMoney(User[playerid][playerMoney]), FormatCoins(User[playerid][playerCoins]));

    PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);
    return true;
}
