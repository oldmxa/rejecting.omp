#if defined _practice_npc_incl
    #endinput
#endif
#define _practice_npc_incl

forward OnPracticeNPCRespawn(playerid);

stock PracticeNPCOwner(npcid)
{
    if (npcid < 0 || npcid >= MAX_NPCS) return INVALID_PLAYER_ID;

    new owner = g_PracticeNPCOwner[npcid];

    if (owner < 0 || owner >= MAX_PLAYERS) return INVALID_PLAYER_ID;
    if (!g_Practice[owner][pracActive] || g_Practice[owner][pracNpcId] != npcid) return INVALID_PLAYER_ID;

    return owner;
}

stock DestroyPracticeNPC(playerid)
{
    new npcid = g_Practice[playerid][pracNpcId];
    if (npcid == INVALID_NPC_ID) return 0;

    if (0 <= npcid < MAX_NPCS) g_PracticeNPCOwner[npcid] = INVALID_PLAYER_ID;

    NPC_Destroy(npcid);
    g_Practice[playerid][pracNpcId] = INVALID_NPC_ID;
    return 1;
}

static PracticeNPCMoveStyle(playerid)
{
    new idx = g_Practice[playerid][pracMoveStyle];
    if (idx < 0 || idx >= sizeof(g_PracticeStyles)) idx = 0;

    return g_PracticeStyles[idx][psType];
}

stock PracticeNPCApplyCombat(playerid)
{
    new npcid = g_Practice[playerid][pracNpcId];
    if (npcid == INVALID_NPC_ID) return 0;

    if (!g_Practice[playerid][pracNpcFight])
    {
        NPC_SetWeapon(npcid, WEAPON_FIST);
        NPC_StopAim(npcid);
        return 1;
    }

    new idx = g_Practice[playerid][pracNpcWeapon];
    if (idx < 0 || idx >= sizeof(g_PracticeWeapons)) idx = 0;

    new WEAPON:weapon = WEAPON:g_PracticeWeapons[idx][pwId];

    NPC_SetWeapon(npcid, weapon);
    NPC_SetAmmo(npcid, 9999);
    NPC_SetWeaponAccuracy(npcid, weapon, PRACTICE_NPC_ACCURACY);
    NPC_AimAtPlayer(npcid, playerid, true, PRACTICE_NPC_SHOOT_DELAY);
    return 1;
}

stock PracticeNPCStartMove(playerid)
{
    if (!PracticeCanMove(playerid)) return 0;

    new npcid = g_Practice[playerid][pracNpcId];
    if (npcid == INVALID_NPC_ID) return 0;

    new dest = g_Practice[playerid][pracWaypoint] ? 0 : 1;
    g_Practice[playerid][pracWaypoint] = dest;

    new Float:x, Float:y, Float:z;
    GetPracticePoint(playerid, dest ? 2 : 1, x, y, z);

    NPC_Move(npcid, x, y, z, NPC_MOVE_TYPE:PracticeNPCMoveStyle(playerid));
    return 1;
}

stock PracticeNPCApply(playerid)
{
    new npcid = g_Practice[playerid][pracNpcId];
    if (npcid == INVALID_NPC_ID) return 0;

    new Float:x, Float:y, Float:z;
    GetPracticeStartPos(playerid, x, y, z);

    new skin = g_Practice[playerid][pracSkin];
    if (skin < PRACTICE_SKIN_MIN || skin > PRACTICE_SKIN_MAX) skin = PRACTICE_SKIN_DEFAULT;

    NPC_SetSkin(npcid, skin);
    NPC_SetVirtualWorld(npcid, PRACTICE_VW_BASE + playerid);
    NPC_SetInterior(npcid, GetPlayerInterior(playerid));
    NPC_SetPos(npcid, x, y, z);

    NPC_SetInvulnerable(npcid, false);
    NPC_SetHealth(npcid, PRACTICE_NPC_HP);

    NPC_SetAngleToPlayer(npcid, playerid);
    PracticeNPCApplyCombat(playerid);

    g_Practice[playerid][pracWaypoint] = 0;

    // a gravacao tem prioridade sobre a patrulha entre os dois pontos
    if (PracticeHasRecord(playerid)) PracticePlayRecord(playerid);
    else if (PracticeCanMove(playerid)) PracticeNPCStartMove(playerid);

    return 1;
}

stock EnsurePracticeNPC(playerid)
{
    new npcid = g_Practice[playerid][pracNpcId];

    if (npcid != INVALID_NPC_ID && NPC_IsValid(npcid))
    {
        if (NPC_IsDead(npcid))
        {
            NPC_Respawn(npcid);
            return 1;
        }

        PracticeNPCApply(playerid);
        return 1;
    }

    return CreatePracticeNPC(playerid);
}

stock CreatePracticeNPC(playerid)
{
    DestroyPracticeNPC(playerid);

    new name[MAX_PLAYER_NAME + 1], nick[MAX_PLAYER_NAME + 1];

    format(nick, sizeof(nick), "%s", User[playerid][userName]);
    if (strlen(nick) > 15) nick[15] = EOS;

    g_PracticeNameSeq[playerid] = (g_PracticeNameSeq[playerid] + 1) % 1000;

    format(name, sizeof(name), "npc_%s_%d", nick, g_PracticeNameSeq[playerid]);

    new npcid = NPC_Create(name);

    if (npcid == INVALID_NPC_ID || !(0 <= npcid < MAX_NPCS))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}No NPC slot available right now. Try the ball target.");
        return 0;
    }

    g_Practice[playerid][pracNpcId] = npcid;
    g_PracticeNPCOwner[npcid] = playerid;

    NPC_Spawn(npcid);
    return 1;
}

public OnPracticeNPCRespawn(playerid)
{
    if (!IsPlayerConnected(playerid) || !g_Practice[playerid][pracActive]) return 1;
    if (g_Practice[playerid][pracTargetType] != PRACTICE_TYPE_NPC) return 1;

    return EnsurePracticeNPC(playerid);
}

public OnNPCPlaybackEnd(npcid, recordid)
{
    new owner = PracticeNPCOwner(npcid);
    if (owner == INVALID_PLAYER_ID) return 1;

    // reinicia fora do callback para nao chamar playback dentro do proprio fim
    SetTimerEx("OnPracticeRecordLoop", 100, false, "i", owner);
    return 1;
}

public OnNPCFinishMove(npcid)
{
    new owner = PracticeNPCOwner(npcid);
    if (owner == INVALID_PLAYER_ID) return 1;

    PracticeNPCStartMove(owner);
    return 1;
}

public OnNPCTakeDamage(npcid, issuerid, Float:amount, WEAPON:weaponid, bodypart)
{
    new owner = PracticeNPCOwner(npcid);
    if (owner == INVALID_PLAYER_ID || owner != issuerid) return 1;

    g_Practice[owner][pracHits]++;

    NPC_SetHealth(npcid, PRACTICE_NPC_HP);
    return 1;
}

public OnNPCDeath(npcid, killerid, WEAPON:reason)
{
    new owner = PracticeNPCOwner(npcid);
    if (owner == INVALID_PLAYER_ID) return 1;

    SetTimerEx("OnPracticeNPCRespawn", PRACTICE_RESPAWN_DELAY, false, "i", owner);
    return 1;
}

public OnNPCSpawn(npcid)
{
    new owner = PracticeNPCOwner(npcid);
    if (owner == INVALID_PLAYER_ID) return 1;

    PracticeNPCApply(owner);
    return 1;
}

public OnNPCRespawn(npcid)
{
    new owner = PracticeNPCOwner(npcid);
    if (owner == INVALID_PLAYER_ID) return 1;

    PracticeNPCApply(owner);
    return 1;
}
