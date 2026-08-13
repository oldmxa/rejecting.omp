#if defined _outposts_included
    #endinput
#endif
#define _outposts_included

#include <YSI_Coding/y_hooks>

#define MAX_OUTPOSTS_PER_ARENA (2)
#define OUTPOST_CAPTURE_RADIUS (45.0)
#define OUTPOST_CAPTURE_TIME   (30)
#define OUTPOST_REWARD_TIME    (5 * 60 * 1000) // 5m
#define OUTPOST_REWARD_POINTS  (50)
#define OUTPOST_REWARD_MONEY   (5000)
#define OUTPOST_LOCK_TIME      (10 * 60) // 10m
#define OUTPOST_HIDDEN_VW      (8888)
#define OUTPOST_OBJECT_MODEL   (3461)
#define OUTPOST_OBJECT_HEIGHT  (0.0)

enum E_OUTPOST_DATA {
    oObject,
    Text3D:oLabel,
    oMapIcon,
    oOwnerClan,
    oLockTime,
    Float:oPos[3],
    oCapturingClan,
    oCaptureProgress
};

new OutpostInfo[MAX_ZONES][MAX_OUTPOSTS_PER_ARENA][E_OUTPOST_DATA];

forward CheckOutpostCapture();
forward InitOutposts();
forward OnOutpostsLoad();
forward RewardOutpostOwners();
forward UpdateOutpostsVisibility();
forward ResetPlayerOutpostCapture(playerid);

stock GetClanOwnedOutposts(clanid)
{
    if(clanid == -1) return 0;
    new count = 0;
    for(new i = 0; i < MAX_ZONES; i++)
    {
        for(new j = 0; j < MAX_OUTPOSTS_PER_ARENA; j++)
        {
            if(OutpostInfo[i][j][oOwnerClan] == clanid)
            {
                count++;
            }
        }
    }
    return count;
}

stock bool:IsClanCapturingAnyOutpost(clanid)
{
    if(clanid == -1) return false;
    for(new z = 0; z < MAX_ZONES; z++)
    {
        for(new o = 0; o < MAX_OUTPOSTS_PER_ARENA; o++)
        {
            if(OutpostInfo[z][o][oCapturingClan] == clanid) return true;
        }
    }
    return false;
}

stock AnnounceOutpostCaptureStart(zoneid, outpost_idx, clanid)
{
    new colorStr[10], clanNameColor[100];
    format(colorStr, sizeof(colorStr), "%06x", ClanInfo[clanid][clanColor] >>> 8);
    format(clanNameColor, sizeof(clanNameColor), "{%s}[%s] %s", colorStr, ClanInfo[clanid][clanTag], ClanInfo[clanid][clanName]);

    foreach(new i : Player)
    {
        if(User[i][locationId] == -1 || User[i][locationId] == ARENA_AFK) continue;
        if(arenaInfo[User[i][locationId]][arenaId] != zoneid) continue;
        if(User[i][hideGlobalMsg]) continue;

        va_SendClientMessage(i, -1, "{MAIN}[OUTPOST] %s {FFFFFF}is capturing Outpost #%d! Defend it or claim it yourself.", clanNameColor, outpost_idx + 1);
    }
    return 1;
}

stock AnnounceOutpostCaptureLost(zoneid, outpost_idx, clanid, progress)
{
    if(clanid == -1 || !ClanInfo[clanid][clanIDUsed]) return 0;

    new pct = (progress * 100) / OUTPOST_CAPTURE_TIME;
    if(pct > 100) pct = 100;

    foreach(new i : Player)
    {
        if(!User[i][isLogged] || User[i][clanID] != clanid) continue;

        new loc = User[i][locationId];
        if(loc < 0 || loc >= MAX_LOCATIONS || loc == ARENA_AFK) continue;
        if(arenaInfo[loc][arenaId] != zoneid) continue;

        va_SendClientMessage(i, -1, "{MAIN}[OUTPOST] {FFFFFF}Capture of Outpost #%d stopped: no clan member inside the area. {F84F31}%d%% {FFFFFF}lost.", outpost_idx + 1, pct);
        PlayerPlaySound(i, 1085, 0.0, 0.0, 0.0);
    }
    return 1;
}

stock Float:GetOutpostRespectMultiplier(clanid)
{
    if(clanid == -1 || !ClanInfo[clanid][clanIDUsed]) return 1.0;
    new count = GetClanOwnedOutposts(clanid);
    if(count == 1) return 1.3;
    if(count >= 2) return 1.7;
    return 1.0;
}

hook OnGameModeInit()
{
    SetTimer("InitOutposts", 2000, false);
    SetTimer("CheckOutpostCapture", 1000, true);
    SetTimer("RewardOutpostOwners", OUTPOST_REWARD_TIME, true);
    SetTimer("UpdateOutpostsVisibility", 5000, true);
    return 1;
}

hook OnPlayerConnect(playerid)
{
    HideOutpostBar(playerid);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    HideOutpostBar(playerid);
    return 1;
}

public InitOutposts()
{
    for(new i = 0; i < MAX_ZONES; i++)
    {
        CreateOutpost(i, 0, arenaData[i][arenaPos][0] + 15.0, arenaData[i][arenaPos][1] + 15.0);
        CreateOutpost(i, 1, arenaData[i][arenaPos][2] - 15.0, arenaData[i][arenaPos][3] - 15.0);
    }
    
    mysql_tquery(dbHandle, "SELECT * FROM `outposts`", "OnOutpostsLoad");
    UpdateOutpostsVisibility();
    return 1;
}

public OnOutpostsLoad()
{
    new rows = cache_num_rows();
	if(rows > 0)
	{
		new zoneid, outpostid, clanid;
		for(new i = 0; i < rows; i++)
		{
			cache_get_value_name_int(i, "zoneid", zoneid);
			cache_get_value_name_int(i, "outpostid", outpostid);
			cache_get_value_name_int(i, "clanid", clanid);

			if(clanid < 0 || clanid >= MAX_CLANS) clanid = -1;

			if(zoneid >= 0 && zoneid < MAX_ZONES && outpostid >= 0 && outpostid < MAX_OUTPOSTS_PER_ARENA)
			{
				SetOutpostOwner(zoneid, outpostid, clanid, false);
			}
		}
	}
    return 1;
}

SaveOutpost(arenaid, outpost_idx)
{
    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `outposts` (zoneid, outpostid, clanid) VALUES (%d, %d, %d) ON DUPLICATE KEY UPDATE clanid = %d",
        arenaid, outpost_idx, OutpostInfo[arenaid][outpost_idx][oOwnerClan], OutpostInfo[arenaid][outpost_idx][oOwnerClan]);
    mysql_tquery(dbHandle, query);
    return 1;
}

CreateOutpost(arenaid, outpost_idx, Float:x, Float:y)
{
    new Float:z;
    CA_FindZ_For2DCoord(x, y, z);
    z += 1.5;

    OutpostInfo[arenaid][outpost_idx][oPos][0] = x;
    OutpostInfo[arenaid][outpost_idx][oPos][1] = y;
    OutpostInfo[arenaid][outpost_idx][oPos][2] = z;
    OutpostInfo[arenaid][outpost_idx][oOwnerClan] = -1;
    OutpostInfo[arenaid][outpost_idx][oLockTime] = 0;
    OutpostInfo[arenaid][outpost_idx][oCapturingClan] = -1;
    OutpostInfo[arenaid][outpost_idx][oCaptureProgress] = 0;

    OutpostInfo[arenaid][outpost_idx][oObject] = CreateDynamicObject(OUTPOST_OBJECT_MODEL, x, y, z + OUTPOST_OBJECT_HEIGHT, 0.0, 0.0, 0.0, OUTPOST_HIDDEN_VW, -1, -1, 300.0);
    SetDynamicObjectNoCameraCol(OutpostInfo[arenaid][outpost_idx][oObject]);

    new str[200];
    format(str, sizeof(str), "{FFFFFF}Location #%d (%s)\n{FFFFFF}Outpost #%d\n{FFFFFF}Owner: {F3230E}None", arenaid, arenaData[arenaid][arenaName], outpost_idx + 1);
    
    OutpostInfo[arenaid][outpost_idx][oLabel] = CreateDynamic3DTextLabel(str, 0xFFFFFFFF, x, y, z + 0.5, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, OUTPOST_HIDDEN_VW, -1, -1, 20.0);
    
    OutpostInfo[arenaid][outpost_idx][oMapIcon] = CreateDynamicMapIcon(x, y, z, 48, 0, OUTPOST_HIDDEN_VW, -1, -1, 500.0, MAPICON_LOCAL);

    return 1;
}

public UpdateOutpostsVisibility()
{
    new zoneVWs[MAX_ZONES][MAX_LOCATIONS + 1];
    new zoneVWCount[MAX_ZONES];

    for(new i=0; i<MAX_ZONES; i++) zoneVWCount[i] = 0;

    for(new loc=0; loc < MAX_LOCATIONS; loc++)
    {
        if(loc == ARENA_AFK) continue;
        new aid = arenaInfo[loc][arenaId];
        if(aid >= 0 && aid < MAX_ZONES)
        {
            zoneVWs[aid][zoneVWCount[aid]++] = loc * 99;
        }
    }

    for(new z=0; z<MAX_ZONES; z++)
    {
        for(new o=0; o<MAX_OUTPOSTS_PER_ARENA; o++)
        {
            if(zoneVWCount[z] > 0)
            {
                Streamer_SetArrayData(STREAMER_TYPE_OBJECT, OutpostInfo[z][o][oObject], E_STREAMER_WORLD_ID, zoneVWs[z], zoneVWCount[z]);
                Streamer_SetArrayData(STREAMER_TYPE_3D_TEXT_LABEL, OutpostInfo[z][o][oLabel], E_STREAMER_WORLD_ID, zoneVWs[z], zoneVWCount[z]);
                Streamer_SetArrayData(STREAMER_TYPE_MAP_ICON, OutpostInfo[z][o][oMapIcon], E_STREAMER_WORLD_ID, zoneVWs[z], zoneVWCount[z]);
            }
            else
            {
                Streamer_SetIntData(STREAMER_TYPE_OBJECT, OutpostInfo[z][o][oObject], E_STREAMER_WORLD_ID, OUTPOST_HIDDEN_VW);
                Streamer_SetIntData(STREAMER_TYPE_3D_TEXT_LABEL, OutpostInfo[z][o][oLabel], E_STREAMER_WORLD_ID, OUTPOST_HIDDEN_VW);
                Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, OutpostInfo[z][o][oMapIcon], E_STREAMER_WORLD_ID, OUTPOST_HIDDEN_VW);
            }
        }
    }
    return 1;
}

public ResetPlayerOutpostCapture(playerid)
{
    HideOutpostBar(playerid);
    return 1;
}

public CheckOutpostCapture()
{
    for(new z = 0; z < MAX_ZONES; z++)
    {
        for(new o = 0; o < MAX_OUTPOSTS_PER_ARENA; o++)
        {
            new capClan = OutpostInfo[z][o][oCapturingClan];
            if(capClan == -1) continue;

            new capturingCount = 0;
            new rivalCount = 0;

            foreach(new i : Player)
            {
                if(!User[i][isLogged] || User[i][isAfk]) continue;
                new loc = User[i][locationId];
                if(loc < 0 || loc >= MAX_LOCATIONS || loc == ARENA_AFK) continue;
                if(arenaInfo[loc][arenaId] != z) continue;

                if(IsPlayerInRangeOfPoint(i, OUTPOST_CAPTURE_RADIUS, OutpostInfo[z][o][oPos][0], OutpostInfo[z][o][oPos][1], OutpostInfo[z][o][oPos][2]))
                {
                    new pClan = User[i][clanID];
                    if(pClan == capClan)
                    {
                        capturingCount++;
                    }
                    else if(pClan != -1)
                    {
                        rivalCount++;
                    }
                }
            }

            if(capturingCount == 0)
            {
                AnnounceOutpostCaptureLost(z, o, capClan, OutpostInfo[z][o][oCaptureProgress]);

                OutpostInfo[z][o][oCapturingClan] = -1;
                OutpostInfo[z][o][oCaptureProgress] = 0;

                foreach(new i : Player)
                {
                    new loc = User[i][locationId];
                    if(loc >= 0 && loc < MAX_LOCATIONS && arenaInfo[loc][arenaId] == z)
                    {
                        if(IsPlayerInRangeOfPoint(i, OUTPOST_CAPTURE_RADIUS, OutpostInfo[z][o][oPos][0], OutpostInfo[z][o][oPos][1], OutpostInfo[z][o][oPos][2]))
                        {
                            HideOutpostBar(i);
                        }
                    }
                }
                continue;
            }

            if(rivalCount > 0)
            {
                foreach(new i : Player)
                {
                    if(!User[i][isLogged] || User[i][isAfk]) continue;
                    new loc = User[i][locationId];
                    if(loc < 0 || loc >= MAX_LOCATIONS || arenaInfo[loc][arenaId] != z) continue;

                    if(IsPlayerInRangeOfPoint(i, OUTPOST_CAPTURE_RADIUS, OutpostInfo[z][o][oPos][0], OutpostInfo[z][o][oPos][1], OutpostInfo[z][o][oPos][2]))
                    {
                        if(User[i][clanID] == capClan)
                        {
                            new Float:dist = GetPlayerDistanceFromPoint(i, OutpostInfo[z][o][oPos][0], OutpostInfo[z][o][oPos][1], OutpostInfo[z][o][oPos][2]);
                            UpdateOutpostBar(i, OutpostInfo[z][o][oCaptureProgress], dist, capturingCount, true);
                        }
                    }
                }
                continue;
            }

            OutpostInfo[z][o][oCaptureProgress] += capturingCount;

            if(OutpostInfo[z][o][oCaptureProgress] >= OUTPOST_CAPTURE_TIME)
            {
                SetOutpostOwner(z, o, capClan, true);
                OutpostInfo[z][o][oCapturingClan] = -1;
                OutpostInfo[z][o][oCaptureProgress] = 0;

                foreach(new i : Player)
                {
                    new loc = User[i][locationId];
                    if(loc >= 0 && loc < MAX_LOCATIONS && arenaInfo[loc][arenaId] == z)
                    {
                        if(IsPlayerInRangeOfPoint(i, OUTPOST_CAPTURE_RADIUS, OutpostInfo[z][o][oPos][0], OutpostInfo[z][o][oPos][1], OutpostInfo[z][o][oPos][2]))
                        {
                            if(User[i][clanID] == capClan)
                            {
                                PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
                                Ach_OnOutpostCaptured(i, capturingCount);
                            }
                            HideOutpostBar(i);
                        }
                    }
                }
            }
            else
            {
                foreach(new i : Player)
                {
                    if(!User[i][isLogged] || User[i][isAfk]) continue;
                    new loc = User[i][locationId];
                    if(loc < 0 || loc >= MAX_LOCATIONS || arenaInfo[loc][arenaId] != z) continue;

                    if(IsPlayerInRangeOfPoint(i, OUTPOST_CAPTURE_RADIUS, OutpostInfo[z][o][oPos][0], OutpostInfo[z][o][oPos][1], OutpostInfo[z][o][oPos][2]))
                    {
                        if(User[i][clanID] == capClan)
                        {
                            new Float:dist = GetPlayerDistanceFromPoint(i, OutpostInfo[z][o][oPos][0], OutpostInfo[z][o][oPos][1], OutpostInfo[z][o][oPos][2]);
                            UpdateOutpostBar(i, OutpostInfo[z][o][oCaptureProgress], dist, capturingCount, false);
                        }
                    }
                }
            }
        }
    }

    foreach(new i : Player)
    {
        if(!User[i][isLogged] || User[i][isAfk]) { HideOutpostBar(i); continue; }
        new loc = User[i][locationId];
        if(loc < 0 || loc >= MAX_LOCATIONS || loc == ARENA_AFK) { HideOutpostBar(i); continue; }
        new z = arenaInfo[loc][arenaId];
        if(z < 0 || z >= MAX_ZONES) { HideOutpostBar(i); continue; }

        new bool:inActiveCap = false;
        for(new o = 0; o < MAX_OUTPOSTS_PER_ARENA; o++)
        {
            if(OutpostInfo[z][o][oCapturingClan] != -1 && OutpostInfo[z][o][oCapturingClan] == User[i][clanID])
            {
                if(IsPlayerInRangeOfPoint(i, OUTPOST_CAPTURE_RADIUS, OutpostInfo[z][o][oPos][0], OutpostInfo[z][o][oPos][1], OutpostInfo[z][o][oPos][2]))
                {
                    inActiveCap = true;
                    break;
                }
            }
        }
        if(!inActiveCap) HideOutpostBar(i);
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(newkeys & KEY_YES)
    {
        if(!User[playerid][isLogged] || User[playerid][isAfk]) return 1;
        
        new location = User[playerid][locationId];
        if(location == ARENA_AFK || location < 0 || location >= MAX_LOCATIONS) return 1;

        new arena = arenaInfo[location][arenaId];
        
        if(arena >= 0 && arena < MAX_ZONES)
        {
            for(new o = 0; o < MAX_OUTPOSTS_PER_ARENA; o++)
            {
                if(IsPlayerInRangeOfPoint(playerid, OUTPOST_CAPTURE_RADIUS, OutpostInfo[arena][o][oPos][0], OutpostInfo[arena][o][oPos][1], OutpostInfo[arena][o][oPos][2]))
                {
                    new clan = User[playerid][clanID];
                    if(clan == -1) {
                         if(gettime() > User[playerid][outpostWarnCooldown]) {
                             SendClientMessage(playerid, -1, "{MAIN}[OUTPOST] {FFFFFF}You need a clan to capture this outpost!");
                             User[playerid][outpostWarnCooldown] = gettime() + 10;
                         }
                         return 1;
                    }
                    if(OutpostInfo[arena][o][oOwnerClan] == clan) {
                         GameTextForPlayer(playerid, "~g~Already owned!", 1000, 4);
                         return 1;
                    }
                    
                    if(gettime() < OutpostInfo[arena][o][oLockTime])
                    {
                        new remaining = OutpostInfo[arena][o][oLockTime] - gettime();
                        new str[64];
                        format(str, sizeof(str), "~r~Locked %02d:%02d", remaining / 60, remaining % 60);
                        GameTextForPlayer(playerid, str, 1000, 4);
                        return 1;
                    }

                    if(OutpostInfo[arena][o][oCapturingClan] != clan && IsClanCapturingAnyOutpost(clan))
                    {
                        if(gettime() > User[playerid][outpostWarnCooldown]) {
                            SendClientMessage(playerid, -1, "{MAIN}[OUTPOST] {FFFFFF}Your clan is already capturing another outpost!");
                            User[playerid][outpostWarnCooldown] = gettime() + 10;
                        }
                        return 1;
                    }

                    if(OutpostInfo[arena][o][oCapturingClan] == -1)
                    {
                        OutpostInfo[arena][o][oCapturingClan] = clan;
                        OutpostInfo[arena][o][oCaptureProgress] = 1;
                        PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

                        AnnounceOutpostCaptureStart(arena, o, clan);
                    }
                    else if(OutpostInfo[arena][o][oCapturingClan] == clan)
                    {
                        if(gettime() > User[playerid][outpostWarnCooldown])
                        {
                            va_SendClientMessage(playerid, -1, "{MAIN}[OUTPOST] {FFFFFF}You are helping to capture Outpost #%d.", o + 1);
                            User[playerid][outpostWarnCooldown] = gettime() + 10;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

ShowOutpostBar(playerid)
{
    for(new i = 0; i < 5; i++) PlayerTextDrawShow(playerid, OutpostBar[playerid][i]);
    return 1;
}

HideOutpostBar(playerid)
{
    for(new i = 0; i < 5; i++) PlayerTextDrawHide(playerid, OutpostBar[playerid][i]);
    return 1;
}

UpdateOutpostBar(playerid, progress, Float:dist, membersCount = 1, bool:paused = false)
{
    ShowOutpostBar(playerid);
    
    new pct = (progress * 100) / OUTPOST_CAPTURE_TIME;
    if(pct > 100) pct = 100;

    new Float:width = (float(progress) * 150.0 / float(OUTPOST_CAPTURE_TIME));
    if(width < 2.0) width = 2.0;
    if(width > 150.0) width = 150.0;

    PlayerTextDrawTextSize(playerid, OutpostBar[playerid][2], width, 6.0);
    PlayerTextDrawShow(playerid, OutpostBar[playerid][2]);

    new str[128], subStr[128];
    new cOne[20], cTwo[20];
    format(cOne, sizeof(cOne), "%s", GetPlayerHudTag(playerid, false));
    format(cTwo, sizeof(cTwo), "%s", GetPlayerHudTag(playerid, true));

    if(paused)
    {
        format(str, sizeof(str), "%sCAPTURING OUTPOST %s[ ~r~PAUSED - RIVAL IN AREA %s]", cOne, cTwo, cTwo);
    }
    else
    {
        if(membersCount > 1)
            format(str, sizeof(str), "%sCAPTURING OUTPOST %s[ %s%d%% %s] ~g~x%d", cOne, cTwo, cOne, pct, cTwo, membersCount);
        else
            format(str, sizeof(str), "%sCAPTURING OUTPOST %s[ %s%d%% %s]", cOne, cTwo, cOne, pct, cTwo);
    }
    PlayerTextDrawSetString(playerid, OutpostBar[playerid][3], str);
    PlayerTextDrawShow(playerid, OutpostBar[playerid][3]);

    format(subStr, sizeof(subStr), "%sDISTANCE: %s%.1fM %s/ %.0fM", cTwo, cOne, dist, cTwo, OUTPOST_CAPTURE_RADIUS);
    PlayerTextDrawSetString(playerid, OutpostBar[playerid][4], subStr);
    PlayerTextDrawShow(playerid, OutpostBar[playerid][4]);
    return 1;
}

SetOutpostOwner(arenaid, outpost_idx, clanid, bool:save = true, bool:announce = true)
{
    OutpostInfo[arenaid][outpost_idx][oOwnerClan] = clanid;
    OutpostInfo[arenaid][outpost_idx][oCapturingClan] = -1;
    OutpostInfo[arenaid][outpost_idx][oCaptureProgress] = 0;
    
    new str[200], ownerClanName[100];
    
    if(clanid != -1) {
        new colorStr[10];
        format(colorStr, sizeof(colorStr), "%06x", ClanInfo[clanid][clanColor] >>> 8);

        format(ownerClanName, sizeof(ownerClanName), "{%s}[%s] %s", colorStr, ClanInfo[clanid][clanTag], ClanInfo[clanid][clanName]);
        
        OutpostInfo[arenaid][outpost_idx][oLockTime] = gettime() + OUTPOST_LOCK_TIME;
        
    } else {
        format(ownerClanName, sizeof(ownerClanName), "{F3230E}None");
        OutpostInfo[arenaid][outpost_idx][oLockTime] = 0;
    }

    format(str, sizeof(str), "{FFFFFF}Location #%d (%s)\n{FFFFFF}Outpost #%d\n{FFFFFF}Owner: %s", 
        arenaid, 
        arenaData[arenaid][arenaName], 
        outpost_idx + 1, 
        ownerClanName
    );
    
    UpdateDynamic3DTextLabelText(OutpostInfo[arenaid][outpost_idx][oLabel], 0xFFFFFFFF, str);
    
    if(IsValidDynamicMapIcon(OutpostInfo[arenaid][outpost_idx][oMapIcon]))
        DestroyDynamicMapIcon(OutpostInfo[arenaid][outpost_idx][oMapIcon]);
    
    new iconId = (clanid != -1) ? 19 : 48;
    OutpostInfo[arenaid][outpost_idx][oMapIcon] = CreateDynamicMapIcon(
        OutpostInfo[arenaid][outpost_idx][oPos][0], 
        OutpostInfo[arenaid][outpost_idx][oPos][1], 
        OutpostInfo[arenaid][outpost_idx][oPos][2], 
        iconId, 0, OUTPOST_HIDDEN_VW, -1, -1, 500.0, MAPICON_LOCAL
    );
    
    if(save) SaveOutpost(arenaid, outpost_idx);
    
    UpdateOutpostsVisibility();

    if(announce)
    {
        foreach(new i : Player) {
            if(User[i][locationId] != -1 && User[i][locationId] != ARENA_AFK) {
                 if(arenaInfo[User[i][locationId]][arenaId] == arenaid) {
                     if(!User[i][hideGlobalMsg]) {
                        va_SendClientMessage(i, -1, "{MAIN}[OUTPOST] {FFFFFF}Outpost #%d captured by %s!", outpost_idx+1, ownerClanName);
                     }
                 }
            }
        }
    }
    return 1;
}

ReleaseClanOutposts(clanid)
{
    if(clanid < 0 || clanid >= MAX_CLANS) return 0;

    new released = 0;
    for(new z = 0; z < MAX_ZONES; z++)
    {
        for(new o = 0; o < MAX_OUTPOSTS_PER_ARENA; o++)
        {
            if(OutpostInfo[z][o][oCapturingClan] == clanid)
            {
                OutpostInfo[z][o][oCapturingClan] = -1;
                OutpostInfo[z][o][oCaptureProgress] = 0;
            }

            if(OutpostInfo[z][o][oOwnerClan] != clanid) continue;

            SetOutpostOwner(z, o, -1, true, false);
            released++;
        }
    }
    return released;
}

public RewardOutpostOwners()
{
    new points[MAX_CLANS];
    for(new i = 0; i < MAX_ZONES; i++)
    {
        for(new j = 0; j < MAX_OUTPOSTS_PER_ARENA; j++)
        {
            new clan = OutpostInfo[i][j][oOwnerClan];
            if(clan != -1 && ClanInfo[clan][clanIDUsed])
            {
                points[clan] += OUTPOST_REWARD_POINTS;
            }
        }
    }

    new bool:rewarded = false;

    for(new c = 0; c < MAX_CLANS; c++)
    {
        if(points[c] > 0)
        {
            ClanInfo[c][clanPoints] += points[c];
            SaveClanData(c);
            rewarded = true;

            new moneyReward = (points[c] / OUTPOST_REWARD_POINTS) * OUTPOST_REWARD_MONEY;

            foreach(new i : Player) {
                if(User[i][clanID] == c) {
                    GivePlayerMoney(i, moneyReward);
                    User[i][playerMoney] += moneyReward;
                    if(!User[i][hideGlobalMsg]) {
                        va_SendClientMessage(i, -1, "{MAIN}[OUTPOST] {FFFFFF}Your clan received "COL_BEIGE"%d{FFFFFF} points and you got "COL_BEIGE"%s {FFFFFF}for holding outposts!", points[c], FormatMoney(moneyReward));
                    }
                }
            }
        }
    }

    if(rewarded) UpdateTOP10Clans();
    return 1;
}
