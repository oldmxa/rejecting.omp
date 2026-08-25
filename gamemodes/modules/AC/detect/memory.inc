#if defined _ac_memory_included
    #endinput
#endif
#define _ac_memory_included

#define AC_PRESSED(%0) (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

#include <YSI_Coding\y_hooks>

static enum E_AC_PROBE
{
    E_AC_PROBE_MEMORY_ADDRESS,
    E_AC_PROBE_BYTE_COUNT,
    E_AC_PROBE_RETN_DATA,

    E_AC_PROBE_CODE,
    E_AC_PROBE_FALLBACK
}

static const AC_PROBE[][E_AC_PROBE] =
{
    {0x5E8606,  2,  204,    E_REJECTING_AC_S0BEIT,          E_REJECTING_AC_UNKNOWN},
    {0x520190,  4,  8,      E_REJECTING_AC_SAMPFUNCS,       E_REJECTING_AC_UNKNOWN},
    {0x4667DB,  4,  192,    E_REJECTING_AC_CLEO,            E_REJECTING_AC_UNKNOWN},
    {0x743C60,  2,  196,    E_REJECTING_AC_IMPROVEDDEAGLE,  E_REJECTING_AC_UNKNOWN},

    {0x5407F0,  4,  64,     E_REJECTING_AC_SPRINTHOOK,      E_REJECTING_AC_UNKNOWN},

    {0x4B3FC0,  4,  8,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x521500,  4,  4,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x712E40,  4,  8,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x541E2C,  2,  8,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_UNKNOWN},

    // {0x6FF420,  4,  204,    E_REJECTING_AC_STEALTH},
    {0x73FF11,  4,  200,    E_REJECTING_AC_STEALTH,         E_REJECTING_AC_UNKNOWN},

    {0x60F289,  2,  0,      E_REJECTING_AC_BYPASS,          E_REJECTING_AC_UNKNOWN},
    {0x60F29D,  2,  0,      E_REJECTING_AC_BYPASS,          E_REJECTING_AC_UNKNOWN},

    {0x58A330,  2,  76,     E_REJECTING_AC_FAKE_FPS,        E_REJECTING_AC_UNKNOWN},

    {0x6E9E23,  2,  128,    E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_UNKNOWN},
    {0x6EFBD8,  4,  68,     E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_UNKNOWN},

    {0x60B437,  4,  4,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x60B448,  4,  4,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x5231A6,  2,  4,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x52322D,  2,  0,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x5233BA,  2,  68,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},

    {0x679D3A,  2,  76,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x524013,  2,  140,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},

    {0x637590,  2,  64,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x63070C,  2,  64,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x4B331F,  2,  12,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x4B3395,  2,  4,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x64159F,  2,  136,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},

    {0x5E91CE,  2,  128,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},
    {0x544CFE,  2,  204,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_OTHER_CHEATS},

    // --- Probes (Modloader & Plugins) ---
    {0x5B8760,  4,  140,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_UNKNOWN},
    {0x53BC70,  4,  128,      E_REJECTING_AC_UNKNOWN,         E_REJECTING_AC_UNKNOWN},
    {0x609B10,  4,  200,      E_REJECTING_AC_RUN_EVERYWHERE,  E_REJECTING_AC_UNKNOWN}, // runeverywhere.asi ainda estou pegando as memorias corretas, falta calibrar!!
    {0x6F65BF,  4,  0,        E_REJECTING_AC_CRASHERS_ASI,    E_REJECTING_AC_UNKNOWN}, // crashers.asi
    {0x5DB1A0,  4,  132,      E_REJECTING_AC_SAMP_GRASS,      E_REJECTING_AC_UNKNOWN}, // samp_grass.asi (CPlantMgr)
    {0x5DD272,  4,  0,        E_REJECTING_AC_SAMP_GRASS,      E_REJECTING_AC_UNKNOWN}, // samp_grass.asi (Grass DFF hook)

    // --- Hooks Modloader 
    {0x5B8E1B,  4,  0,        E_REJECTING_AC_MODLOADER,       E_REJECTING_AC_UNKNOWN},
    {0x5BA865,  4,  0,        E_REJECTING_AC_MODLOADER,       E_REJECTING_AC_UNKNOWN},
    {0x5BF8B7,  4,  0,        E_REJECTING_AC_MODLOADER,       E_REJECTING_AC_UNKNOWN},
    {0x40685F,  4,  0,        E_REJECTING_AC_MODLOADER,       E_REJECTING_AC_UNKNOWN} 
};

#define AC_PROBE_SPRINTHOOK       (4)
#define AC_PROBE_FIRST_INJECTION  (5)

static const AC_SIGNATURE[][3] =
{
    {0x4B3FC0, 128, E_REJECTING_AC_1337},
    {0x4B3FC0, 140, E_REJECTING_AC_1337},
    {0x4B3FC0, 204, E_REJECTING_AC_ARCANE},

    {0x521500,  76, E_REJECTING_AC_1337},
    {0x521500,  12, E_REJECTING_AC_ARCANE},
    {0x521500, 132, E_REJECTING_AC_ARCANE},
    {0x521500, 204, E_REJECTING_AC_UNKNOWN},

    {0x712E40, 136, E_REJECTING_AC_1337},
    {0x712E40, 128, E_REJECTING_AC_ARCANE},

    {0x541E2C,  72, E_REJECTING_AC_EXTREMECHEATS},
    {0x541E2C,  76, E_REJECTING_AC_EXTREMECHEATS},

    {0x6E9E23,  64, E_REJECTING_AC_VENOM},

    {0x6EFBD8,  12, E_REJECTING_AC_VENOM},
    {0x6EFBD8, 204, E_REJECTING_AC_VENOM}
};

static
    ACSweepTick[MAX_PLAYERS],
    ACFullSweepTick[MAX_PLAYERS],
    ACSprintProbeTick[MAX_PLAYERS],

    bool:ACProbeSupport[MAX_PLAYERS char],
    ACHasCleo[MAX_PLAYERS],
    ACHasSampfuncs[MAX_PLAYERS];

static ACProbeRand[MAX_PLAYERS][sizeof(AC_PROBE)];

static AC_SendProbe(playerid, index)
{
    new rand, base;

    for (new attempt; attempt < 8; attempt++)
    {
        new bool:clash = false;

        rand = random(256);
        base = AC_PROBE[index][E_AC_PROBE_MEMORY_ADDRESS] - rand;

        for (new i; i < sizeof(AC_PROBE); i++)
        {
            if (i == index || ACProbeRand[playerid][i] < 0) continue;
            if (AC_PROBE[i][E_AC_PROBE_MEMORY_ADDRESS] - ACProbeRand[playerid][i] != base) continue;

            clash = true;
            break;
        }

        if (!clash) break;
    }

    ACProbeRand[playerid][index] = rand;

    SendClientCheck(playerid, 5, base, rand, AC_PROBE[index][E_AC_PROBE_BYTE_COUNT]);
    return 1;
}

static AC_ProbeIndexFromAddress(playerid, memaddr)
{
    for (new i; i < sizeof(AC_PROBE); i++)
    {
        if (ACProbeRand[playerid][i] < 0) continue;
        if (AC_PROBE[i][E_AC_PROBE_MEMORY_ADDRESS] - ACProbeRand[playerid][i] != memaddr) continue;

        ACProbeRand[playerid][i] = -1;
        return i;
    }
    return -1;
}

static AC_ClearProbes(playerid)
{
    for (new i; i < sizeof(AC_PROBE); i++) ACProbeRand[playerid][i] = -1;

    return 1;
}

static bool:AC_SendProbes(playerid, bool:onlyInjections = false)
{
    if (!ACProbeSupport{playerid}) return false;

    new pending = 0, total = 0;

    for (new i; i < sizeof(AC_PROBE); i++)
    {
        if (ACProbeRand[playerid][i] < 0) continue;

        pending++;
    }

    for (new i = (onlyInjections ? AC_PROBE_FIRST_INJECTION : 0); i < sizeof(AC_PROBE); i++) total++;

    if (pending > 0)
    {
        AC_Debug(playerid, E_REJECTING_AC_BYPASS, "previous sweep: %d of %d probes never answered",
            pending, total);
    }

    for (new i = (onlyInjections ? AC_PROBE_FIRST_INJECTION : 0); i < sizeof(AC_PROBE); i++)
    {
        AC_SendProbe(playerid, i);
    }

    return true;
}

stock IsPlayerUsingCleo(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;
    return ACHasCleo[playerid];
}

stock IsPlayerUsingSampfuncs(playerid)
{
    if (!IsPlayerUsingCleo(playerid)) return 0;
    return ACHasSampfuncs[playerid];
}

hook OnPlayerConnect(playerid)
{
    ACHasCleo[playerid] =
    ACHasSampfuncs[playerid] = -1;

    ACSweepTick[playerid] =
    ACFullSweepTick[playerid] =
    ACSprintProbeTick[playerid] = GetTickCount();

    AC_ClearProbes(playerid);

    new version[24 + 1];
    GetPlayerVersion(playerid, version);
    #if defined IsPlayerUsingOmp
        ACProbeSupport{playerid} = !strequal(version, "0.3.7") || IsPlayerUsingOmp(playerid);
    #else
        ACProbeSupport{playerid} = !strequal(version, "0.3.7");
    #endif

    if (!ACProbeSupport{playerid})
    {
        new log[128];
        format(log, sizeof(log), "[RAC] %s[%d] runs client '%s', memory probes are disabled for him",
            User[playerid][userName], playerid, version);

        ConsoleLog(log);
    }

    AC_SendProbes(playerid);

    return 1;
}

static AC_ResolveProbe(index, retndata)
{
    new code = AC_PROBE[index][E_AC_PROBE_CODE];

    if (code != E_REJECTING_AC_UNKNOWN) return code;

    new address = AC_PROBE[index][E_AC_PROBE_MEMORY_ADDRESS];

    for (new i; i < sizeof(AC_SIGNATURE); i++)
    {
        if (AC_SIGNATURE[i][0] != address || AC_SIGNATURE[i][1] != retndata) continue;

        return AC_SIGNATURE[i][2];
    }

    return AC_PROBE[index][E_AC_PROBE_FALLBACK];
}

hook OnClientCheckResponse(playerid, actionid, memaddr, retndata)
{
    if (actionid != 5) return 1;

    new index = AC_ProbeIndexFromAddress(playerid, memaddr);

    if (index == -1) return 1;

    new bool:mismatch = (retndata != AC_PROBE[index][E_AC_PROBE_RETN_DATA]);
    new code = mismatch ? AC_ResolveProbe(index, retndata) : E_REJECTING_AC_UNKNOWN;

    if (AC_DebugActive())
    {
        new name[32];

        if (mismatch) AC_GetDetectionName(code, name, sizeof(name));
        else name = "match";

        AC_Debug(playerid, (code == E_REJECTING_AC_UNKNOWN) ? E_REJECTING_AC_OTHER_CHEATS : code,
            "probe [%02d] 0x%x returned %d, expected %d, %s",
            index, AC_PROBE[index][E_AC_PROBE_MEMORY_ADDRESS], retndata, AC_PROBE[index][E_AC_PROBE_RETN_DATA], name);
    }

    if (mismatch && code != E_REJECTING_AC_UNKNOWN)
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, code);

    switch (AC_PROBE[index][E_AC_PROBE_CODE])
    {
        case E_REJECTING_AC_CLEO:      ACHasCleo[playerid] = _:mismatch;
        case E_REJECTING_AC_SAMPFUNCS: ACHasSampfuncs[playerid] = _:mismatch;
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!ACProbeSupport{playerid}) return 1;

    new const tickCount = GetTickCount();

    if (AC_PRESSED(KEY_SPRINT))
    {
        if (tickCount - ACSprintProbeTick[playerid] > 5_000)
        {
            ACSprintProbeTick[playerid] = tickCount;

            AC_SendProbe(playerid, AC_PROBE_SPRINTHOOK);
        }
    }
    else if (tickCount - ACFullSweepTick[playerid] > 30_000)
    {
        ACFullSweepTick[playerid] = ACSweepTick[playerid] = tickCount;
        AC_SendProbes(playerid);
    }
    else if (tickCount - ACSweepTick[playerid] > 5_000)
    {
        ACSweepTick[playerid] = tickCount;
        AC_SendProbes(playerid, true);
    }

    return 1;
}
