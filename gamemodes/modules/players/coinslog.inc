#if defined _coinslog_included
    #endinput
#endif
#define _coinslog_included

#define COINSLOG_ROWS (15)

forward DIALOG_COINS_HISTORY(playerid, dialogid, response, listitem, string:inputtext[]);

stock ShowCoinsHistory(playerid)
{
    inline const OnCoinsLog()
    {
        new rows = cache_num_rows();

        if (!rows)
        {
            return Dialog_ShowCallback(playerid, using public DIALOG_COINS_HISTORY<iiiis>, DIALOG_STYLE_MSGBOX,
                "{FFFF00}Coins History",
                "{FFFFFF}You have no Coins movement recorded yet.\n\n{B4B5B7}Every purchase and reward shows up here from now on.",
                "Back", "");
        }

        new body[1600];
        strcat(body, "{A29582}Nick\t{A29582}Transaction\t{A29582}Coins\t{A29582}Date\n");

        for (new i; i < rows; i++)
        {
            new username[MAX_PLAYER_NAME + 1], reason[64], date[24], amount;

            cache_get_value_name(i, "username", username, sizeof(username));
            cache_get_value_name(i, "reason", reason, sizeof(reason));
            cache_get_value_name(i, "created_at", date, sizeof(date));
            cache_get_value_name_int(i, "amount", amount);

            strcat(body, fmat_return("{FFFFFF}%s\t{BEBEBE}%s\t%s\t{B4B5B7}%s\n",
                username, reason, FormatCoinsDelta(amount), date));
        }

        return Dialog_ShowCallback(playerid, using public DIALOG_COINS_HISTORY<iiiis>, DIALOG_STYLE_TABLIST_HEADERS,
            "{FFFF00}Coins History", body, "Back", "");
    }

    MySQL_TQueryInline(dbHandle, using inline OnCoinsLog,
        "SELECT `username`, `reason`, `amount`, `created_at` FROM `coins_logs` WHERE `user_id` = %d ORDER BY `id` DESC LIMIT %d",
        User[playerid][user_sqlid], COINSLOG_ROWS);

    return 1;
}

public DIALOG_COINS_HISTORY(playerid, dialogid, response, listitem, string:inputtext[])
{
    return ShowCoinsMenu(playerid);
}

@cmd(.alts = {"coinslog"}) logcoins(playerid, params[], help)
{
    if (!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    return ShowCoinsHistory(playerid);
}
