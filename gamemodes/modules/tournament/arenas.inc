#if defined _tourney_arenas_included
    #endinput
#endif
#define _tourney_arenas_included

stock GetTourneyCustomArenaCoords(id, &Float:x1, &Float:y1, &Float:z1, &Float:a1, &Float:x2, &Float:y2, &Float:z2, &Float:a2, &interior)
{
    interior = 0;
    switch(id)
    {
        case 0: // Chinese Garden
        {
            x1 = 3470.0; y1 = 5.0; z1 = 1001.5; a1 = 270.0;
            x2 = 3520.0; y2 = 5.0; z2 = 1001.5; a2 = 90.0;
        }
        case 1: // Desert
        {
            x1 = 3450.0; y1 = 970.0; z1 = 1001.5; a1 = 45.0;
            x2 = 3515.0; y2 = 1025.0; z2 = 1001.5; a2 = 225.0;
        }
        case 2: // Egypt
        {
            x1 = 3445.0; y1 = 1935.0; z1 = 1004.5; a1 = 45.0;
            x2 = 3540.0; y2 = 2035.0; z2 = 1004.5; a2 = 225.0;
        }
        case 3: // Urbex
        {
            x1 = -45.0; y1 = 2485.0; z1 = 993.5; a1 = 45.0;
            x2 = -5.0;  y2 = 2505.0; z2 = 993.5; a2 = 225.0;
        }
        case 4: // Zoo
        {
            x1 = 3480.0; y1 = -1005.0; z1 = 1001.5; a1 = 45.0;
            x2 = 3525.0; y2 = -985.0;  z2 = 1001.5; a2 = 225.0;
        }
        default:
        {
            x1 = 3470.0; y1 = 5.0; z1 = 1001.5; a1 = 270.0;
            x2 = 3520.0; y2 = 5.0; z2 = 1001.5; a2 = 90.0;
        }
    }
}

stock GetTourneyServerArenaCoords(id, &Float:x1, &Float:y1, &Float:z1, &Float:a1, &Float:x2, &Float:y2, &Float:z2, &Float:a2, &interior)
{
    if(id < 0 || id >= MAX_TOURNEY_SERVER_ARENAS) id = 0;

    if(IsCustomArena(id)) id = 0;

    interior = arenaData[id][arenaInterior];

    new Float:minX = arenaData[id][arenaPos][0];
    new Float:minY = arenaData[id][arenaPos][1];
    new Float:maxX = arenaData[id][arenaPos][2];
    new Float:maxY = arenaData[id][arenaPos][3];

    x1 = minX + (maxX - minX) * 0.35;
    y1 = minY + (maxY - minY) * 0.35;
    x2 = minX + (maxX - minX) * 0.65;
    y2 = minY + (maxY - minY) * 0.65;

    CA_RayCastLine(x1, y1, 1000.0, x1, y1, -1000.0, x1, y1, z1);
    CA_RayCastLine(x2, y2, 1000.0, x2, y2, -1000.0, x2, y2, z2);

    z1 += 1.0;
    z2 += 1.0;

    a1 = 45.0;
    a2 = 225.0;
}

stock GetTourneyDuelArenaCoords(id, &Float:x1, &Float:y1, &Float:z1, &Float:a1, &Float:x2, &Float:y2, &Float:z2, &Float:a2, &interior)
{
    if(id < 0 || id >= sizeof(DuelS_ArenaNames)) id = 0;

    interior = DuelS_ArenaInteriors[id];
    x1 = DuelS_Pos[id][0][0];
    y1 = DuelS_Pos[id][0][1];
    z1 = DuelS_Pos[id][0][2];
    a1 = DuelS_Pos[id][0][3];

    x2 = DuelS_Pos[id][1][0];
    y2 = DuelS_Pos[id][1][1];
    z2 = DuelS_Pos[id][1][2];
    a2 = DuelS_Pos[id][1][3];
}

stock GetTourneyArenaDisplayName(arenaCat, arenaid, dest[], size = sizeof(dest))
{
    switch(arenaCat)
    {
        case TOURNEY_ARENA_CAT_RANDOM:
        {
            strcopy(dest, "Random (Any Category)", size);
        }
        case TOURNEY_ARENA_CAT_DUEL:
        {
            if(arenaid >= 0 && arenaid < sizeof(DuelS_ArenaNames))
                format(dest, size, "Duel: %s", DuelS_ArenaNames[arenaid]);
            else
                strcopy(dest, "Duel: Random Arena", size);
        }
        case TOURNEY_ARENA_CAT_SERVER:
        {
            if(arenaid >= 0 && arenaid < MAX_TOURNEY_SERVER_ARENAS)
                format(dest, size, "Server: %s", arenaData[arenaid][arenaName]);
            else
                strcopy(dest, "Server: Random Arena", size);
        }
        case TOURNEY_ARENA_CAT_CUSTOM:
        {
            if(arenaid >= 0 && arenaid < MAX_TOURNEY_CUSTOM_ARENAS)
                format(dest, size, "Custom: %s", g_TourneyCustomArenaNames[arenaid]);
            else
                strcopy(dest, "Custom: Random Arena", size);
        }
        default:
        {
            strcopy(dest, "Random Arena", size);
        }
    }
    return 1;
}

stock PickTourneyMatchArena(matchid)
{
    new cat = g_TourneyData[t_ArenaCategory];
    new id = g_TourneyData[t_ArenaID];

    if(cat == TOURNEY_ARENA_CAT_RANDOM)
    {
        new r = random(3);
        if(r == 0)
        {
            g_TourneyMatches[matchid][tm_ArenaCategory] = TOURNEY_ARENA_CAT_DUEL;
            g_TourneyMatches[matchid][tm_ArenaID] = random(sizeof(DuelS_ArenaNames));
        }
        else if(r == 1)
        {
            g_TourneyMatches[matchid][tm_ArenaCategory] = TOURNEY_ARENA_CAT_SERVER;
            g_TourneyMatches[matchid][tm_ArenaID] = random(MAX_TOURNEY_SERVER_ARENAS);
        }
        else
        {
            g_TourneyMatches[matchid][tm_ArenaCategory] = TOURNEY_ARENA_CAT_CUSTOM;
            g_TourneyMatches[matchid][tm_ArenaID] = random(MAX_TOURNEY_CUSTOM_ARENAS);
        }
    }
    else
    {
        g_TourneyMatches[matchid][tm_ArenaCategory] = cat;
        if(id != -1)
        {
            g_TourneyMatches[matchid][tm_ArenaID] = id;
        }
        else
        {
            switch(cat)
            {
                case TOURNEY_ARENA_CAT_DUEL: g_TourneyMatches[matchid][tm_ArenaID] = random(sizeof(DuelS_ArenaNames));
                case TOURNEY_ARENA_CAT_SERVER: g_TourneyMatches[matchid][tm_ArenaID] = random(MAX_TOURNEY_SERVER_ARENAS);
                case TOURNEY_ARENA_CAT_CUSTOM: g_TourneyMatches[matchid][tm_ArenaID] = random(MAX_TOURNEY_CUSTOM_ARENAS);
            }
        }
    }
}
