#if defined _ac_crashers_included
    #endinput
#endif
#define _ac_crashers_included

#include <YSI_Coding\y_hooks>

#define AC_MAX_SEAT        (3)
#define AC_MIN_COMPONENT   (1000)
#define AC_MAX_COMPONENT   (1193)
#define AC_MAX_PAINTJOB    (3)
#define AC_MAX_LISTITEM    (255)

enum
{
    AC_CB_DIALOG,
    AC_CB_CLICK_PLAYER,
    AC_CB_CLICK_TEXTDRAW,
    AC_CB_CLICK_MAP,
    AC_CB_ENTER_VEHICLE,
    AC_CB_EXIT_VEHICLE,
    AC_CB_PICKUP,
    AC_CB_STATE_CHANGE,
    AC_CB_REQUEST_CLASS,
    AC_CB_MOD_SHOP,

    MAX_AC_CALLBACKS
}

static const AC_CB_LIMIT[MAX_AC_CALLBACKS][2] =
{
    {1000, 12},
    {1000, 10},
    {1000, 15},
    {1000,  6},
    {1000,  6},
    {1000,  6},
    {1000, 14},
    {1000, 10},
    {1000, 12},
    {2000,  4}
};

static
    ACCallbackTick[MAX_PLAYERS][MAX_AC_CALLBACKS],
    ACCallbackHits[MAX_PLAYERS][MAX_AC_CALLBACKS],
    ACDeathTick[MAX_PLAYERS],
    ACDeathHits[MAX_PLAYERS];

static AC_CountCallback(playerid, callback)
{
    new tick = GetTickCount();

    if (tick - ACCallbackTick[playerid][callback] >= AC_CB_LIMIT[callback][0])
    {
        ACCallbackTick[playerid][callback] = tick;
        ACCallbackHits[playerid][callback] = 0;
        return 1;
    }

    if (++ACCallbackHits[playerid][callback] < AC_CB_LIMIT[callback][1]) return 1;

    new hits = ACCallbackHits[playerid][callback], elapsed = tick - ACCallbackTick[playerid][callback];

    ACCallbackHits[playerid][callback] = 0;
    ACCallbackTick[playerid][callback] = tick;

    AC_Debug(playerid, AC_CALLBACK_FLOOD, "callback %d flood (%d in %dms, limit %d in %dms)",
        callback, hits, elapsed, AC_CB_LIMIT[callback][1], AC_CB_LIMIT[callback][0]);

    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_CALLBACK_FLOOD);
    return 1;
}

static bool:AC_IsSaneCoord(Float:value)
{
    if (value != value) return false;
    if (value < -30000.0 || value > 30000.0) return false;

    return true;
}

hook OnPlayerConnect(playerid)
{
    for (new i; i < MAX_AC_CALLBACKS; i++)
    {
        ACCallbackTick[playerid][i] = 0;
        ACCallbackHits[playerid][i] = 0;
    }
    ACDeathTick[playerid] = 0;
    ACDeathHits[playerid] = 0;
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, string:inputtext[])
{
    AC_CountCallback(playerid, AC_CB_DIALOG);

    if (response < 0 || response > 1 || listitem < -1 || listitem > AC_MAX_LISTITEM)
    {
        AC_Debug(playerid, AC_DIALOG_CRASHER, "dialog %d: response %d, listitem %d", dialogid, response, listitem);
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_DIALOG_CRASHER);
    }
    return 1;
}

hook OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    AC_CountCallback(playerid, AC_CB_STATE_CHANGE);

    if (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)
    {
        new seat = GetPlayerVehicleSeat(playerid);

        if (seat < 0 || seat > AC_MAX_SEAT)
        {
            AC_Debug(playerid, AC_SEAT_CRASHER, "seat %d in vehicle %d", seat, GetPlayerVehicleID(playerid));
            CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_SEAT_CRASHER);
            RemovePlayerFromVehicle(playerid);
        }
    }
    return 1;
}

hook OnVehicleMod(playerid, vehicleid, componentid)
{
    if (componentid < AC_MIN_COMPONENT || componentid > AC_MAX_COMPONENT)
    {
        AC_Debug(playerid, AC_TUNING_CRASHER, "component %d in vehicle %d", componentid, vehicleid);
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_TUNING_CRASHER);
        return 0;
    }
    return 1;
}

hook OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
    if (paintjobid < 0 || paintjobid > AC_MAX_PAINTJOB)
    {
        AC_Debug(playerid, AC_TUNING_CRASHER, "paintjob %d in vehicle %d", paintjobid, vehicleid);
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_TUNING_CRASHER);
        return 0;
    }
    return 1;
}

hook OnVehicleRespray(playerid, vehicleid, color1, color2)
{
    if (color1 < 0 || color1 > 255 || color2 < 0 || color2 > 255)
    {
        AC_Debug(playerid, AC_TUNING_CRASHER, "colours %d/%d in vehicle %d", color1, color2, vehicleid);
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_TUNING_CRASHER);
        return 0;
    }
    return 1;
}

hook OnEnterExitModShop(playerid, enterexit, interiorid)
{
    AC_CountCallback(playerid, AC_CB_MOD_SHOP);

    if (enterexit < 0 || enterexit > 1 || interiorid < 0 || interiorid > 3)
    {
        AC_Debug(playerid, AC_TUNING_CRASHER, "modshop enterexit %d, interior %d", enterexit, interiorid);
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_TUNING_CRASHER);
    }
    return 1;
}

hook OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if (_:weaponid < 0 || _:weaponid > 46 || _:hittype < 0 || _:hittype > 3
     || !AC_IsSaneCoord(fX) || !AC_IsSaneCoord(fY) || !AC_IsSaneCoord(fZ))
    {
        AC_Debug(playerid, AC_WEAPON_CRASHER, "weapon %d, hitType %d, pos %.1f %.1f %.1f", _:weaponid, _:hittype, fX, fY, fZ);
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_WEAPON_CRASHER);
        return 0;
    }
    return 1;
}

hook OnPlayerClickPlayer(playerid, clickedplayerid, CLICK_SOURCE:source)
{
    AC_CountCallback(playerid, AC_CB_CLICK_PLAYER);
    return 1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    AC_CountCallback(playerid, AC_CB_CLICK_TEXTDRAW);
    return 1;
}

hook OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    AC_CountCallback(playerid, AC_CB_CLICK_MAP);
    return 1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    AC_CountCallback(playerid, AC_CB_ENTER_VEHICLE);
    return 1;
}

hook OnPlayerExitVehicle(playerid, vehicleid)
{
    AC_CountCallback(playerid, AC_CB_EXIT_VEHICLE);
    return 1;
}

hook OnPlayerPickUpPickup(playerid, pickupid)
{
    AC_CountCallback(playerid, AC_CB_PICKUP);
    return 1;
}

hook OnPlayerRequestClass(playerid, classid)
{
    AC_CountCallback(playerid, AC_CB_REQUEST_CLASS);
    return 1;
}

hook OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    new tick = GetTickCount();

    if (tick - ACDeathTick[playerid] >= 1500)
    {
        ACDeathTick[playerid] = tick;
        ACDeathHits[playerid] = 0;
    }
    else if (++ACDeathHits[playerid] >= 3)
    {
        ACDeathHits[playerid] = 0;
        AC_Debug(playerid, AC_DEATH_FLOOD, "death reported too fast (3 deaths in < 1500ms)");
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_DEATH_FLOOD);
        return 0;
    }

    return 1;
}
