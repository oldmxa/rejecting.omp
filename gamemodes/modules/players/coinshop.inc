#if defined _coinshop_included
    #endinput
#endif
#define _coinshop_included

#define COINSHOP_COINS (7000)
#define COINSHOP_MONEY (2000000)

#define COINSHOP_MAX   (100000)

enum
{
    COINS_ROW_BALANCE,
    COINS_ROW_BUY,
    COINS_ROW_VIP_BUY,
    COINS_ROW_VIP_PERKS,
    COINS_ROW_DISCORD,
    COINS_ROW_HISTORY
}

forward DIALOG_COINS_MENU(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_COINS_BUY(playerid, dialogid, response, listitem, string:inputtext[]);

stock GetCoinsPrice(coins)
{
    return (coins * (COINSHOP_MONEY / 1000)) / (COINSHOP_COINS / 1000);
}

forward Coinshop_ShowCoinsMenu(playerid);
public Coinshop_ShowCoinsMenu(playerid)
{
    new body[640], title[64] = "{MAIN}Coins";

    strcat(body, "{A29582}Option\t{A29582}Details\n");

    strcat(body, fmat_return("{FFFFFF}My Balance\t{FFFF00}%s\n", FormatCoins(User[playerid][playerCoins])));
    strcat(body, fmat_return("{FFFFFF}Buy Coins\t{23C552}%s {BEBEBE}\xbb {FFFF00}%s\n",
        FormatMoney(COINSHOP_MONEY), FormatCoins(COINSHOP_COINS)));
    strcat(body, fmat_return("{FFFFFF}Buy VIP\t{BEBEBE}From {FFFF00}%s\n", FormatCoins(GetVipPackageCoins(0))));

    if (IsPlayerVip(playerid))
        strcat(body, "{FFFFFF}VIP Perks\t{23C552}Active\n");
    else
        strcat(body, "{FFFFFF}VIP Perks\t{BEBEBE}View perks\n");

    if (strcmp(User[playerid][dc_username], "-1", true) != 0)
        strcat(body, "{FFFFFF}Discord Reward\t{23C552}Linked\n");
    else
        strcat(body, "{FFFFFF}Discord Reward\t{23C552}+$100k {B4B5B7}& {FFFF00}+5k Coins\n");

    strcat(body, "{FFFFFF}History\t{BEBEBE}Recent transactions\n");

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);

    return Dialog_ShowCallback(playerid, using public DIALOG_COINS_MENU<iiiis>, DIALOG_STYLE_TABLIST_HEADERS,
        title, body, "Select", "Close");
}

public DIALOG_COINS_MENU(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response || !User[playerid][isLogged]) return 1;

    switch (listitem)
    {
        case COINS_ROW_BALANCE:
        {
            va_SendClientMessage(playerid, -1, "{MAIN}[COINS] {FFFFFF}Your current balance is: {FFFF00}%s{FFFFFF}.", FormatCoins(User[playerid][playerCoins]));
            return ShowCoinsMenu(playerid);
        }

        case COINS_ROW_BUY:
        {
            new body[384], title[64] = "{MAIN}Buy Coins";
            format(body, sizeof(body),
                "{FFFFFF}\x95 {B4B5B7}Rate: {23C552}%s {B4B5B7}buys {FFFF00}%s\n"\
                "{FFFFFF}\x95 {B4B5B7}Your Cash: {23C552}%s\n"\
                "{FFFFFF}\x95 {B4B5B7}Order Limit: {FFFF00}%s\n\n"\
                "{FFFFFF}Enter the amount of {FFFF00}Coins {FFFFFF}you want to buy:",
                FormatMoney(COINSHOP_MONEY), FormatCoins(COINSHOP_COINS),
                FormatMoney(User[playerid][playerMoney]), FormatCoins(COINSHOP_MAX));

            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_COINS_BUY<iiiis>, DIALOG_STYLE_INPUT,
                title, body, "Buy", "Back");
        }

        case COINS_ROW_VIP_BUY:
        {
            new cmd[16];
            strcopy(cmd, "/buyvip");
            Command_ReProcess(playerid, cmd, 0);
            return 1;
        }

        case COINS_ROW_VIP_PERKS:
        {
            ShowVipMenu(playerid);
            return 1;
        }

        case COINS_ROW_DISCORD:
        {
            new cmd[16];
            strcopy(cmd, "/discord");
            Command_ReProcess(playerid, cmd, 0);
            return 1;
        }

        case COINS_ROW_HISTORY: return ShowCoinsHistory(playerid);
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

@cmd(.alts = {"coins"}) buycoins(playerid, params[], help)
{
    if (!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    new coins;

    if (sscanf(params, "d", coins))
        return ShowCoinsMenu(playerid);

    return DoBuyCoins(playerid, coins);
}
