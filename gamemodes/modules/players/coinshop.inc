#if defined _coinshop_included
    #endinput
#endif
#define _coinshop_included

#define COINSHOP_COINS (7000)
#define COINSHOP_MONEY (2000000)

// teto por compra: mantem coins * 2000 dentro da cell
#define COINSHOP_MAX   (100000)

enum
{
    COINS_ROW_SECTION_BALANCE,
    COINS_ROW_BALANCE,
    COINS_ROW_BUY,
    COINS_ROW_HISTORY,
    COINS_ROW_SECTION_VIP,
    COINS_ROW_VIP_PERKS,
    COINS_ROW_VIP_BUY
}

forward DIALOG_COINS_MENU(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_COINS_BUY(playerid, dialogid, response, listitem, string:inputtext[]);

stock GetCoinsPrice(coins)
{
    // divide antes de multiplicar, senao coins * 2000000 estoura a cell
    return (coins * (COINSHOP_MONEY / 1000)) / (COINSHOP_COINS / 1000);
}

stock ShowCoinsMenu(playerid)
{
    new body[1024], title[64] = "{FFFF00}Coins";

    strcat(body, "{A29582}Option\t{A29582}Detail\n");

    strcat(body, "{MAIN}\xBB Balance\t \n");

    strcat(body, fmat_return("{FFFFFF}Your Coins\t{FFFF00}%s\n", FormatCoins(User[playerid][playerCoins])));

    strcat(body, fmat_return("{FFFFFF}Buy Coins\t{BEBEBE}%s {B4B5B7}buys {FFFF00}%s\n",
        FormatMoney(COINSHOP_MONEY), FormatCoins(COINSHOP_COINS)));

    strcat(body, fmat_return("{FFFFFF}Transaction history\t{BEBEBE}Last %d movements\n", COINSLOG_ROWS));

    strcat(body, "{MAIN}\xBB VIP\t \n");

    if (IsPlayerVip(playerid))
        strcat(body, fmat_return("{FFFFFF}VIP benefits\t{23C552}Active until %s\n", returnDate(User[playerid][vipExpire] - 10800)));
    else
        strcat(body, "{FFFFFF}VIP benefits\t{BEBEBE}What VIP gives you\n");

    strcat(body, fmat_return("{FFFFFF}Buy VIP\t{BEBEBE}%d plans from {FFFF00}%s\n",
        GetVipPackageCount(), FormatCoins(GetVipPackageCoins(0))));

    format_MainColor(playerid, body);

    return Dialog_ShowCallback(playerid, using public DIALOG_COINS_MENU<iiiis>, DIALOG_STYLE_TABLIST_HEADERS,
        title, body, "Select", "Close");
}

public DIALOG_COINS_MENU(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response || !User[playerid][isLogged]) return 1;

    switch (listitem)
    {
        case COINS_ROW_BUY:
        {
            new body[288];
            format(body, sizeof(body),
                "{FFFFFF}\x95 {B4B5B7}Rate: {BEBEBE}%s {B4B5B7}buys {FFFF00}%s\n"\
                "{FFFFFF}\x95 {B4B5B7}Your cash: {BEBEBE}%s\n"\
                "{FFFFFF}\x95 {B4B5B7}Limit: {FFFF00}%s {B4B5B7}per order\n\n"\
                "{FFFFFF}How many Coins do you want to buy?",
                FormatMoney(COINSHOP_MONEY), FormatCoins(COINSHOP_COINS),
                FormatMoney(User[playerid][playerMoney]), FormatCoins(COINSHOP_MAX));

            return Dialog_ShowCallback(playerid, using public DIALOG_COINS_BUY<iiiis>, DIALOG_STYLE_INPUT,
                "{FFFF00}Buy Coins", body, "Buy", "Back");
        }

        case COINS_ROW_HISTORY: return ShowCoinsHistory(playerid);

        case COINS_ROW_VIP_PERKS:
        {
            ShowVipMenu(playerid);
            return 1;
        }

        case COINS_ROW_VIP_BUY:
        {
            new cmd[16];
            strcopy(cmd, "/buyvip");

            Command_ReProcess(playerid, cmd, 0);
            return 1;
        }
    }

    return ShowCoinsMenu(playerid);
}

stock DoBuyCoins(playerid, coins)
{
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

    va_SendClientMessage(playerid, -1, "{MAIN}[COINS] {FFFFFF}You exchanged {23C552}%s {FFFFFF}for {FFFF00}%s{FFFFFF}.",
        FormatMoney(price), FormatCoins(coins));

    PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);
    return 1;
}

public DIALOG_COINS_BUY(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response || !User[playerid][isLogged]) return ShowCoinsMenu(playerid);

    if (!IsNumeric(inputtext) || strlen(inputtext) > 6)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Enter a valid number.");
        return ShowCoinsMenu(playerid);
    }

    DoBuyCoins(playerid, strval(inputtext));
    return ShowCoinsMenu(playerid);
}

@cmd(.alts = {"comprarcoins"}) buycoins(playerid, params[], help)
{
    if (!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    new coins;

    if (sscanf(params, "d", coins))
        return ShowCoinsMenu(playerid);

    return DoBuyCoins(playerid, coins);
}
