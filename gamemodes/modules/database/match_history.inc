
#if defined _match_history_included
    #endinput
#endif
#define _match_history_included

#define MATCH_HISTORY_ROWS_PER_QUERY (8)

#define MATCH_HISTORY_INSERT_HEAD "INSERT INTO `player_match_history` \
(`user_id`, `arena_id`, `arena_name`, `kills`, `deaths`, `headshots`, `damage`, \
`shots_fired`, `shots_hit`, `exp_gained`, `placement`, `participants`, \
`player_level`, `played_at`) VALUES "

stock bool:IsArenaMatchParticipant(playerid, location)
{
    if(!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) return false;
    if(!IsPlayerLogged(playerid)) return false;
    if(User[playerid][user_sqlid] <= 0) return false;
    if(User[playerid][locationId] != location) return false;
    if(IsPlayerAFK(playerid)) return false;
    if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE) return false;

    return true;
}

SaveArenaMatchHistory(location, finishingArenaId, const sortedPlayers[], sortedCount)
{
    if(location < 0 || location >= MAX_LOCATIONS) return 0;

    new participants = 0;
    for(new i = 0; i < sortedCount; i ++)
    {
        if(IsArenaMatchParticipant(sortedPlayers[i], location))
            participants++;
    }

    if(participants == 0) return 0;

    new arena_name[64];
    format(arena_name, sizeof(arena_name), "%s", arenaData[finishingArenaId][arenaName]);

    new timestamp = gettime();
    new query[2048], row[256], rows_in_batch = 0;

    for(new i = 0, placement = 0; i < sortedCount; i ++)
    {
        new playerid = sortedPlayers[i];

        if(!IsArenaMatchParticipant(playerid, location)) continue;

        placement++;

        if(rows_in_batch == 0)
        {
            query[0] = EOS;
            strcat(query, MATCH_HISTORY_INSERT_HEAD);
        }
        else
        {
            strcat(query, ",");
        }

        // mysql_format (not format) so the arena name goes through %e.
        mysql_format(dbHandle, row, sizeof(row), "(%d,%d,'%e',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
            User[playerid][user_sqlid],
            finishingArenaId,
            arena_name,
            playerArenaInfo[location][playerid][arenaKills],
            playerArenaInfo[location][playerid][arenaDeaths],
            playerArenaInfo[location][playerid][arenaHeadshots],
            floatround(playerArenaInfo[location][playerid][arenaDMG]),
            playerArenaInfo[location][playerid][firedShots],
            playerArenaInfo[location][playerid][hitedShots],
            playerArenaInfo[location][playerid][arenaExp],
            placement,
            participants,
            User[playerid][playerLevel],
            timestamp
        );

        strcat(query, row);
        rows_in_batch++;

        if(rows_in_batch >= MATCH_HISTORY_ROWS_PER_QUERY)
        {
            mysql_tquery(dbHandle, query);
            rows_in_batch = 0;
        }
    }

    if(rows_in_batch > 0)
        mysql_tquery(dbHandle, query);

    return participants;
}
