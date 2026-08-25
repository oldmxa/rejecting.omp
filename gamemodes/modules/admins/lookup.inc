
stock PerformGPCILookup(playerid, const targetname[], const serial[])
{
    inline OnGPCILookup()
    {
        new rows = cache_num_rows();
        new aka_list[1024] = "";
        if(rows)
        {
            for(new i = 0; i < rows; i++)
            {
                new name[MAX_PLAYER_NAME + 1], ban_id;
                cache_get_value_name(i, "username", name, sizeof(name));
                cache_get_value_name_int(i, "ban_id", ban_id);

                strcat(aka_list, fmat_return("%s%s\n",
                    (ban_id > 0) ? ("{F3230E}* {FFFFFF}") : (""), name), sizeof(aka_list));
            }
        }
        else
        {
            aka_list = "No other accounts found.";
        }

        new dialog_body[2048];
        format(dialog_body, sizeof(dialog_body),
            "{FFFFFF}Target: {F3230E}%s\n{FFFFFF}GPCI: {F3230E}%s\n\n{FFFFFF}Accounts on this GPCI:\n%s\n\n{FFFF00}INFO: {FFFFFF}GPCI matches are not 100%% reliable.\n{F3230E}* {FFFFFF}= Banned account",
            targetname, serial, aka_list);

        Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "{FFFFFF}GPCI Lookup", dialog_body, "Close", "");
    }
    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT a.username, b.id as ban_id FROM accounts a LEFT JOIN banimentos b ON a.username = b.username WHERE a.gpci = '%e' LIMIT 20", serial);
    MySQL_TQueryInline(dbHandle, using inline OnGPCILookup, queryGlobal);
    return 1;
}

stock PerformIPLookup(playerid, const targetname[], const ip[], const gpci[] = "")
{
    new query[512], lookup_gpci[42];
    strcopy(lookup_gpci, (gpci[0] == '\0') ? ("N/A") : (gpci), sizeof(lookup_gpci));

    inline OnIPLookup()
    {
        new rows = cache_num_rows();

        new aka_list[2048];
        strcat(aka_list, "Account\tLast IP\tStatus\n");

        if(rows)
        {
            for(new i = 0; i < rows; i++)
            {
                new name[MAX_PLAYER_NAME + 1], rowip[18], ban_id;
                cache_get_value_name(i, "username", name, sizeof(name));
                cache_get_value_name(i, "lastip", rowip, sizeof(rowip));
                cache_get_value_name_int(i, "ban_id", ban_id);

                strcat(aka_list, fmat_return("{FFFFFF}%s\t{BEBEBE}%s\t%s\n",
                    name,
                    (rowip[0] == '\0') ? ("-") : (rowip),
                    (ban_id > 0) ? ("{F3230E}Banned") : ("{23C552}Clean")), sizeof(aka_list));
            }
        }
        else
        {
            strcat(aka_list, "{BEBEBE}No other accounts found.\t \t \n", sizeof(aka_list));
        }

        new title[128];
        format(title, sizeof(title), "{MAIN}AKA: {FFFFFF}%s {BEBEBE}(%s | %s)", targetname, ip, lookup_gpci);

        format_MainColor(playerid, title);
        Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, aka_list, "Close", "");
    }

    if(gpci[0] == '\0')
    {
        mysql_format(dbHandle, query, sizeof(query), "SELECT a.username, a.lastip, b.id as ban_id FROM accounts a LEFT JOIN banimentos b ON a.username = b.username WHERE a.regip = '%e' OR a.lastip = '%e' LIMIT 20", ip, ip);
    }
    else
    {
        mysql_format(dbHandle, query, sizeof(query), "SELECT a.username, a.lastip, b.id as ban_id FROM accounts a LEFT JOIN banimentos b ON a.username = b.username WHERE a.regip = '%e' OR a.lastip = '%e' OR a.gpci = '%e' LIMIT 20", ip, ip, gpci);
    }
    MySQL_TQueryInline(dbHandle, using inline OnIPLookup, query);
    return 1;
}
