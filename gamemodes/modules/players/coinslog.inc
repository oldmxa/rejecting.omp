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
            new title[64] = "{MAIN}Coins History";
            new msg[256] = "{FFFFFF}You have no Coins movement recorded yet.\n\n{B4B5B7}Every purchase and reward shows up here from now on.";
            format_MainColor(playerid, title);
            format_MainColor(playerid, msg);
            return Dialog_ShowCallback(playerid, using public DIALOG_COINS_HISTORY<iiiis>, DIALOG_STYLE_MSGBOX,
                title, msg, "Back", "");
        }

        new body[1600], title[64] = "{MAIN}Coins History";
        strcat(body, "{MAIN}Nick\t{MAIN}Transaction\t{MAIN}Coins\t{MAIN}Date\n");

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

        format_MainColor(playerid, title);
        format_MainColor(playerid, body);
        return Dialog_ShowCallback(playerid, using public DIALOG_COINS_HISTORY<iiiis>, DIALOG_STYLE_TABLIST_HEADERS,
            title, body, "Back", "");
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
    if (!IsPlayerLogged(playerid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    return ShowCoinsHistory(playerid);
}
