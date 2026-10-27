#if defined _animations_included
    #endinput
#endif
#define _animations_included

#include <YSI_Coding/y_hooks>

stock StopPlayerAnimation(playerid)
{
    if(!IsPlayerLogged(playerid)) return 0;

    if(GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_NONE)
    {
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    }

    ClearAnimations(playerid, FORCE_SYNC:SYNC_ALL);
    ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.0, false, false, false, false, 0, FORCE_SYNC:SYNC_ALL);
    DeletePVar(playerid, "PlayingCustomAnim");
    return 1;
}

stock PlayAnim(playerid, const animlib[], const animname[], Float:fDelta, loop, lockx, locky, freeze, animTime)
{
    if(!IsPlayerLogged(playerid)) return 0;

    if(IsPlayerInCombat(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot perform animations while in combat.");
        return 0;
    }

    if(IsPlayerInAnyVehicle(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot perform animations while inside a vehicle.");
        return 0;
    }

    new PLAYER_STATE:pState = GetPlayerState(playerid);
    if(pState == PLAYER_STATE_NONE || pState == PLAYER_STATE_WASTED || pState == PLAYER_STATE_SPECTATING)
        return 0;

    if(GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_NONE)
    {
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    }

    ApplyAnimation(playerid, animlib, animname, fDelta, bool:loop, bool:lockx, bool:locky, bool:freeze, animTime, FORCE_SYNC:SYNC_ALL);
    SetPVarInt(playerid, "PlayingCustomAnim", 1);
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if((newkeys & KEY_SECONDARY_ATTACK) && !IsPlayerInAnyVehicle(playerid))
    {
        if(GetPVarInt(playerid, "PlayingCustomAnim") == 1 || GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_NONE)
        {
            StopPlayerAnimation(playerid);
        }
    }
    return 1;
}

// Core Animation

@cmd(.alts = {"stopdance", "animstop", "cancelanim"}) stopanim(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    StopPlayerAnimation(playerid);
    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Animation stopped.");
    return true;
}

// Special Actions
@cmd() handsup(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(IsPlayerInCombat(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot perform animations while in combat.");
    if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this in a vehicle.");
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_HANDSUP);
    SetPVarInt(playerid, "PlayingCustomAnim", 1);
    return true;
}

@cmd() piss(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(IsPlayerInCombat(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot perform animations while in combat.");
    if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this in a vehicle.");
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION:68);
    SetPVarInt(playerid, "PlayingCustomAnim", 1);
    return true;
}

@cmd() cellin(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(IsPlayerInCombat(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot perform animations while in combat.");
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
    SetPVarInt(playerid, "PlayingCustomAnim", 1);
    return true;
}

@cmd() cellout(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
    DeletePVar(playerid, "PlayingCustomAnim");
    return true;
}

// Dances
@cmd(.alts = {"dance1"}) dance(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(IsPlayerInCombat(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot perform animations while in combat.");
    new danceid = 1;
    if(!IsNull(params) && IsNumeric(params))
    {
        danceid = strval(params);
        if(danceid < 1 || danceid > 4) danceid = 1;
    }
    switch(danceid)
    {
        case 1: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE1);
        case 2: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE2);
        case 3: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE3);
        case 4: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE4);
    }
    SetPVarInt(playerid, "PlayingCustomAnim", 1);
    return true;
}

@cmd() dance2(playerid, params[], help) { if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED); if(IsPlayerInCombat(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot perform animations while in combat."); SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE2); SetPVarInt(playerid, "PlayingCustomAnim", 1); return true; }
@cmd() dance3(playerid, params[], help) { if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED); if(IsPlayerInCombat(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot perform animations while in combat."); SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE3); SetPVarInt(playerid, "PlayingCustomAnim", 1); return true; }
@cmd() dance4(playerid, params[], help) { if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED); if(IsPlayerInCombat(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot perform animations while in combat."); SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE4); SetPVarInt(playerid, "PlayingCustomAnim", 1); return true; }

// Rap
@cmd() rap(playerid, params[], help) { PlayAnim(playerid, "RAPPING", "RAP_A_Loop", 4.0, 1, 1, 1, 1, 0); return true; }
@cmd() rap2(playerid, params[], help) { PlayAnim(playerid, "RAPPING", "RAP_B_Loop", 4.0, 1, 1, 1, 1, 0); return true; }
@cmd() rap3(playerid, params[], help) { PlayAnim(playerid, "RAPPING", "RAP_C_Loop", 4.0, 1, 1, 1, 1, 0); return true; }

// Wank
@cmd() wankoff(playerid, params[], help) { PlayAnim(playerid, "PAULNMAC", "wank_in", 4.0, 1, 1, 1, 1, 0); return true; }
@cmd() wank(playerid, params[], help) { PlayAnim(playerid, "PAULNMAC", "wank_loop", 4.0, 1, 1, 1, 1, 0); return true; }

// Strip
@cmd(.alts = {"strip1"}) strip(playerid, params[], help) { PlayAnim(playerid, "STRIP", "strip_A", 4.0, 1, 1, 1, 1, 0); return true; }
@cmd() strip2(playerid, params[], help) { PlayAnim(playerid, "STRIP", "strip_B", 4.0, 1, 1, 1, 1, 0); return true; }
@cmd() strip3(playerid, params[], help) { PlayAnim(playerid, "STRIP", "strip_C", 4.0, 1, 1, 1, 1, 0); return true; }
@cmd() strip4(playerid, params[], help) { PlayAnim(playerid, "STRIP", "strip_D", 4.0, 1, 1, 1, 1, 0); return true; }
@cmd() strip5(playerid, params[], help) { PlayAnim(playerid, "STRIP", "strip_E", 4.0, 1, 1, 1, 1, 0); return true; }
@cmd() strip6(playerid, params[], help) { PlayAnim(playerid, "STRIP", "strip_F", 4.0, 1, 1, 1, 1, 0); return true; }
@cmd() strip7(playerid, params[], help) { PlayAnim(playerid, "STRIP", "strip_G", 4.0, 1, 1, 1, 1, 0); return true; }

// Sexy
@cmd(.alts = {"sexy1"}) sexy(playerid, params[], help) { PlayAnim(playerid, "SNM", "SPANKING_IDLEW", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() sexy2(playerid, params[], help) { PlayAnim(playerid, "SNM", "SPANKING_IDLEP", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() sexy3(playerid, params[], help) { PlayAnim(playerid, "SNM", "SPANKINGW", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() sexy4(playerid, params[], help) { PlayAnim(playerid, "SNM", "SPANKINGP", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() sexy5(playerid, params[], help) { PlayAnim(playerid, "SNM", "SPANKEDW", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() sexy6(playerid, params[], help) { PlayAnim(playerid, "SNM", "SPANKEDP", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() sexy7(playerid, params[], help) { PlayAnim(playerid, "SNM", "SPANKING_ENDW", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() sexy8(playerid, params[], help) { PlayAnim(playerid, "SNM", "SPANKING_ENDP", 4.1, 0, 1, 1, 1, 1); return true; }

// BJ
@cmd(.alts = {"bj1"}) bj(playerid, params[], help) { PlayAnim(playerid, "BLOWJOBZ", "BJ_COUCH_START_P", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() bj2(playerid, params[], help) { PlayAnim(playerid, "BLOWJOBZ", "BJ_COUCH_START_W", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() bj3(playerid, params[], help) { PlayAnim(playerid, "BLOWJOBZ", "BJ_COUCH_LOOP_P", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() bj4(playerid, params[], help) { PlayAnim(playerid, "BLOWJOBZ", "BJ_COUCH_LOOP_W", 4.1, 0, 1, 1, 1, 1); return true; }

// Gangs & Signs
@cmd() lean(playerid, params[], help) { PlayAnim(playerid, "GANGS", "leanIDLE", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() follow(playerid, params[], help) { PlayAnim(playerid, "WUZI", "Wuzi_follow", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() greet(playerid, params[], help) { PlayAnim(playerid, "WUZI", "Wuzi_Greet_Wuzi", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() stand(playerid, params[], help) { PlayAnim(playerid, "WUZI", "Wuzi_stand_loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() injured(playerid, params[], help) { PlayAnim(playerid, "SWEET", "Sweet_injuredloop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() injured2(playerid, params[], help) { PlayAnim(playerid, "SWAT", "gnstwall_injurd", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() hitch(playerid, params[], help) { PlayAnim(playerid, "MISC", "Hiker_Pose", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() bitchslap(playerid, params[], help) { PlayAnim(playerid, "MISC", "bitchslap", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() cpr(playerid, params[], help) { PlayAnim(playerid, "MEDIC", "CPR", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() gift(playerid, params[], help) { PlayAnim(playerid, "KISSING", "gift_give", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() chairsit(playerid, params[], help) { PlayAnim(playerid, "PED", "SEAT_idle", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() slapped(playerid, params[], help) { PlayAnim(playerid, "SWEET", "ho_ass_slapped", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() slapass(playerid, params[], help) { PlayAnim(playerid, "SWEET", "sweet_ass_slap", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() drunk(playerid, params[], help) { PlayAnim(playerid, "PED", "WALK_DRUNK", 4.1, 1, 1, 1, 1, 1); return true; }
@cmd() skate(playerid, params[], help) { PlayAnim(playerid, "SKATE", "skate_run", 4.1, 1, 1, 1, 1, 1); return true; }
@cmd() gwalk(playerid, params[], help) { PlayAnim(playerid, "PED", "WALK_gang1", 4.1, 1, 1, 1, 1, 1); return true; }
@cmd() gwalk2(playerid, params[], help) { PlayAnim(playerid, "PED", "WALK_gang2", 4.1, 1, 1, 1, 1, 1); return true; }
@cmd() limp(playerid, params[], help) { PlayAnim(playerid, "PED", "WALK_old", 4.1, 1, 1, 1, 1, 1); return true; }
@cmd() eatsit(playerid, params[], help) { PlayAnim(playerid, "FOOD", "FF_Sit_Loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() celebrate(playerid, params[], help) { PlayAnim(playerid, "benchpress", "gym_bp_celebrate", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() celebrate2(playerid, params[], help) { PlayAnim(playerid, "GYMNASIUM", "gym_tread_celebrate", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() win(playerid, params[], help) { PlayAnim(playerid, "CASINO", "cards_win", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() win2(playerid, params[], help) { PlayAnim(playerid, "CASINO", "Roulette_win", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd(.alts = {"animyes", "nodyes"}) agree(playerid, params[], help) { PlayAnim(playerid, "CLOTHES", "CLO_Buy", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() deal(playerid, params[], help) { PlayAnim(playerid, "DEALER", "DEALER_DEAL", 3.0, 0, 0, 0, 0, 0); return true; }
@cmd() deal2(playerid, params[], help) { PlayAnim(playerid, "DEALER", "DRUGS_BUY", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() thankyou(playerid, params[], help) { PlayAnim(playerid, "FOOD", "SHP_Thank", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() invite1(playerid, params[], help) { PlayAnim(playerid, "GANGS", "Invite_Yes", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() invite2(playerid, params[], help) { PlayAnim(playerid, "GANGS", "Invite_No", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() sit(playerid, params[], help) { PlayAnim(playerid, "INT_OFFICE", "OFF_Sit_Type_Loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() scratch(playerid, params[], help) { PlayAnim(playerid, "MISC", "Scratchballs_01", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() bomb(playerid, params[], help) { PlayAnim(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() getarrested(playerid, params[], help) { PlayAnim(playerid, "ped", "ARRESTgun", 4.0, 0, 1, 1, 1, -1); return true; }
@cmd() laugh(playerid, params[], help) { PlayAnim(playerid, "RAPPING", "Laugh_01", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() lookout(playerid, params[], help) { PlayAnim(playerid, "SHOP", "ROB_Shifty", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() robman(playerid, params[], help) { PlayAnim(playerid, "SHOP", "ROB_Loop_Threat", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() crossarms(playerid, params[], help) { PlayAnim(playerid, "COP_AMBIENT", "Coplook_loop", 4.0, 0, 1, 1, 1, -1); return true; }
@cmd() crossarms2(playerid, params[], help) { PlayAnim(playerid, "DEALER", "DEALER_IDLE", 4.0, 0, 1, 1, 1, -1); return true; }
@cmd() crossarms3(playerid, params[], help) { PlayAnim(playerid, "DEALER", "DEALER_IDLE_01", 4.0, 0, 1, 1, 1, -1); return true; }
@cmd() lay(playerid, params[], help) { PlayAnim(playerid, "BEACH", "bather", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() lay2(playerid, params[], help) { PlayAnim(playerid, "BEACH", "SitnWait_loop_W", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() vomit(playerid, params[], help) { PlayAnim(playerid, "FOOD", "EAT_Vomit_P", 3.0, 0, 0, 0, 0, 0); return true; }
@cmd() eat(playerid, params[], help) { PlayAnim(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0); return true; }
@cmd() wave(playerid, params[], help) { PlayAnim(playerid, "ON_LOOKERS", "wave_loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() crack(playerid, params[], help) { PlayAnim(playerid, "CRACK", "crckdeth2", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() crack2(playerid, params[], help) { PlayAnim(playerid, "CRACK", "crckidle2", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() smokem(playerid, params[], help) { PlayAnim(playerid, "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() smokef(playerid, params[], help) { PlayAnim(playerid, "SMOKING", "F_smklean_loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() gro(playerid, params[], help) { PlayAnim(playerid, "BEACH", "ParkSit_M_loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() fsit(playerid, params[], help) { PlayAnim(playerid, "BEACH", "ParkSit_W_loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() chat(playerid, params[], help) { PlayAnim(playerid, "PED", "IDLE_CHAT", 4.1, 1, 1, 1, 1, 1); return true; }
@cmd() fucku(playerid, params[], help) { PlayAnim(playerid, "PED", "fucku", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() taichi(playerid, params[], help) { PlayAnim(playerid, "PARK", "Tai_Chi_Loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() relax(playerid, params[], help) { PlayAnim(playerid, "BEACH", "Lay_Bac_Loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() nod(playerid, params[], help) { PlayAnim(playerid, "COP_AMBIENT", "Coplook_nod", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() cry1(playerid, params[], help) { PlayAnim(playerid, "GRAVEYARD", "mrnF_loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() cry2(playerid, params[], help) { PlayAnim(playerid, "GRAVEYARD", "mrnM_loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() carsit(playerid, params[], help) { PlayAnim(playerid, "CAR", "Tap_hand", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() carsit2(playerid, params[], help) { PlayAnim(playerid, "LOWRIDER", "Sit_relaxed", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() fwalk(playerid, params[], help) { PlayAnim(playerid, "ped", "WOMAN_walksexy", 4.1, 1, 1, 1, 1, 1); return true; }
@cmd() mwalk(playerid, params[], help) { PlayAnim(playerid, "ped", "WALK_player", 4.1, 1, 1, 1, 1, 1); return true; }
@cmd() stretch(playerid, params[], help) { PlayAnim(playerid, "PLAYIDLES", "stretch", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() chant(playerid, params[], help) { PlayAnim(playerid, "RIOT", "RIOT_CHANT", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() angry(playerid, params[], help) { PlayAnim(playerid, "RIOT", "RIOT_ANGRY", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() exhausted(playerid, params[], help) { PlayAnim(playerid, "FAT", "IDLE_tired", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() carsmoke(playerid, params[], help) { PlayAnim(playerid, "PED", "Smoke_in_car", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() aim(playerid, params[], help) { PlayAnim(playerid, "PED", "gang_gunstand", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() aim2(playerid, params[], help) { PlayAnim(playerid, "SHOP", "SHP_Gun_Aim", 4.0, 0, 1, 1, 1, -1); return true; }
@cmd() getup(playerid, params[], help) { PlayAnim(playerid, "PED", "getup", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() akick(playerid, params[], help) { PlayAnim(playerid, "FIGHT_E", "FightKick", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() box(playerid, params[], help) { PlayAnim(playerid, "GYMNASIUM", "gym_shadowbox", 4.1, 1, 1, 1, 1, 1); return true; }
@cmd() cockgun(playerid, params[], help) { PlayAnim(playerid, "SILENCED", "Silence_reload", 3.0, 0, 0, 0, 0, 0); return true; }
@cmd() liftup(playerid, params[], help) { PlayAnim(playerid, "CARRY", "liftup", 3.0, 0, 0, 0, 0, 0); return true; }
@cmd() putdown(playerid, params[], help) { PlayAnim(playerid, "CARRY", "putdwn", 3.0, 0, 0, 0, 0, 0); return true; }
@cmd() joint(playerid, params[], help) { PlayAnim(playerid, "GANGS", "smkcig_prtl", 4.0, 0, 1, 1, 1, 1); return true; }
@cmd() die(playerid, params[], help) { PlayAnim(playerid, "KNIFE", "KILL_Knife_Ped_Die", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() die2(playerid, params[], help) { PlayAnim(playerid, "PARACHUTE", "FALL_skyDive_DIE", 4.0, 0, 1, 1, 1, -1); return true; }
@cmd() shakehead(playerid, params[], help) { PlayAnim(playerid, "MISC", "plyr_shkhead", 3.0, 0, 0, 0, 0, 0); return true; }
@cmd() benddown(playerid, params[], help) { PlayAnim(playerid, "BAR", "Barserve_bottle", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() checkout(playerid, params[], help) { PlayAnim(playerid, "GRAFFITI", "graffiti_Chkout", 4.0, 0, 0, 0, 0, 0); return true; }

// Gsigns (1 to 5)
@cmd(.alts = {"gsign1"}) gsign(playerid, params[], help) { PlayAnim(playerid, "GHANDS", "gsign1", 4.0, 0, 1, 1, 1, 1); return true; }
@cmd() gsign2(playerid, params[], help) { PlayAnim(playerid, "GHANDS", "gsign2", 4.0, 0, 1, 1, 1, 1); return true; }
@cmd() gsign3(playerid, params[], help) { PlayAnim(playerid, "GHANDS", "gsign3", 4.0, 0, 1, 1, 1, 1); return true; }
@cmd() gsign4(playerid, params[], help) { PlayAnim(playerid, "GHANDS", "gsign4", 4.0, 0, 1, 1, 1, 1); return true; }
@cmd() gsign5(playerid, params[], help) { PlayAnim(playerid, "GHANDS", "gsign5", 4.0, 0, 1, 1, 1, 1); return true; }

// Ghand (1 to 5)
@cmd(.alts = {"ghand1"}) ghand(playerid, params[], help) { PlayAnim(playerid, "GHANDS", "gsign1LH", 4.0, 0, 1, 1, 1, 1); return true; }
@cmd() ghand2(playerid, params[], help) { PlayAnim(playerid, "GHANDS", "gsign2LH", 4.0, 0, 1, 1, 1, 1); return true; }
@cmd() ghand3(playerid, params[], help) { PlayAnim(playerid, "GHANDS", "gsign3LH", 4.0, 0, 1, 1, 1, 1); return true; }
@cmd() ghand4(playerid, params[], help) { PlayAnim(playerid, "GHANDS", "gsign4LH", 4.0, 0, 1, 1, 1, 1); return true; }
@cmd() ghand5(playerid, params[], help) { PlayAnim(playerid, "GHANDS", "gsign5LH", 4.0, 0, 1, 1, 1, 1); return true; }

// Gang (1 to 7)
@cmd(.alts = {"gang1"}) gang(playerid, params[], help) { PlayAnim(playerid, "GANGS", "hndshkaa", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() gang2(playerid, params[], help) { PlayAnim(playerid, "GANGS", "hndshkba", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() gang3(playerid, params[], help) { PlayAnim(playerid, "GANGS", "hndshkca", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() gang4(playerid, params[], help) { PlayAnim(playerid, "GANGS", "hndshkcb", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() gang5(playerid, params[], help) { PlayAnim(playerid, "GANGS", "hndshkda", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() gang6(playerid, params[], help) { PlayAnim(playerid, "GANGS", "hndshkea", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() gang7(playerid, params[], help) { PlayAnim(playerid, "GANGS", "hndshkfa", 4.0, 0, 0, 0, 0, 0); return true; }

// Bat (1 to 5)
@cmd(.alts = {"bat1"}) bat(playerid, params[], help) { PlayAnim(playerid, "BASEBALL", "Bat_IDLE", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() bat2(playerid, params[], help) { PlayAnim(playerid, "BASEBALL", "Bat_M", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() bat3(playerid, params[], help) { PlayAnim(playerid, "BASEBALL", "BAT_PART", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() bat4(playerid, params[], help) { PlayAnim(playerid, "CRACK", "Bbalbat_Idle_01", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() bat5(playerid, params[], help) { PlayAnim(playerid, "CRACK", "Bbalbat_Idle_02", 4.0, 1, 0, 0, 0, 0); return true; }

// Basket (1 to 6)
@cmd(.alts = {"basket1"}) basket(playerid, params[], help) { PlayAnim(playerid, "BSKTBALL", "BBALL_def_loop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() basket2(playerid, params[], help) { PlayAnim(playerid, "BSKTBALL", "BBALL_idleloop", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() basket3(playerid, params[], help) { PlayAnim(playerid, "BSKTBALL", "BBALL_pickup", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() basket4(playerid, params[], help) { PlayAnim(playerid, "BSKTBALL", "BBALL_Jump_Shot", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() basket5(playerid, params[], help) { PlayAnim(playerid, "BSKTBALL", "BBALL_Dnk", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() basket6(playerid, params[], help) { PlayAnim(playerid, "BSKTBALL", "BBALL_run", 4.1, 1, 1, 1, 1, 1); return true; }

// Bar (1 to 4)
@cmd(.alts = {"bar1"}) bar(playerid, params[], help) { PlayAnim(playerid, "BAR", "Barcustom_get", 3.0, 0, 0, 0, 0, 0); return true; }
@cmd() bar2(playerid, params[], help) { PlayAnim(playerid, "BAR", "Barcustom_order", 3.0, 0, 0, 0, 0, 0); return true; }
@cmd() bar3(playerid, params[], help) { PlayAnim(playerid, "BAR", "Barserve_give", 3.0, 0, 0, 0, 0, 0); return true; }
@cmd() bar4(playerid, params[], help) { PlayAnim(playerid, "BAR", "Barserve_glass", 3.0, 0, 0, 0, 0, 0); return true; }

// Bed (1 to 4)
@cmd(.alts = {"bed1"}) bed(playerid, params[], help) { PlayAnim(playerid, "INT_HOUSE", "BED_In_L", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() bed2(playerid, params[], help) { PlayAnim(playerid, "INT_HOUSE", "BED_In_R", 4.1, 0, 1, 1, 1, 1); return true; }
@cmd() bed3(playerid, params[], help) { PlayAnim(playerid, "INT_HOUSE", "BED_Loop_L", 4.0, 1, 0, 0, 0, 0); return true; }
@cmd() bed4(playerid, params[], help) { PlayAnim(playerid, "INT_HOUSE", "BED_Loop_R", 4.0, 1, 0, 0, 0, 0); return true; }

// Kiss (1 to 7)
@cmd(.alts = {"kiss1"}) kiss(playerid, params[], help) { PlayAnim(playerid, "BD_FIRE", "Grlfrd_Kiss_03", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() kiss2(playerid, params[], help) { PlayAnim(playerid, "KISSING", "Grlfrd_Kiss_01", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() kiss3(playerid, params[], help) { PlayAnim(playerid, "KISSING", "Grlfrd_Kiss_02", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() kiss4(playerid, params[], help) { PlayAnim(playerid, "KISSING", "Grlfrd_Kiss_03", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() kiss5(playerid, params[], help) { PlayAnim(playerid, "KISSING", "Playa_Kiss_01", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() kiss6(playerid, params[], help) { PlayAnim(playerid, "KISSING", "Playa_Kiss_02", 4.0, 0, 0, 0, 0, 0); return true; }
@cmd() kiss7(playerid, params[], help) { PlayAnim(playerid, "KISSING", "Playa_Kiss_03", 4.0, 0, 0, 0, 0, 0); return true; }

// anims Dialog

forward DIALOG_ANIM_CATEGORIES(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_ANIM_LIST(playerid, dialogid, response, listitem, string:inputtext[]);

stock ShowAnimsCategoryMenu(playerid)
{
    if(!IsPlayerLogged(playerid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(IsPlayerInCombat(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot perform animations while in combat.");

    new title[64], body[512];
    format(title, sizeof(title), "{MAIN}Animations Menu");
    format_MainColor(playerid, title);

    format(body, sizeof(body),
        "{MAIN}Category\t{MAIN}Description\n"\
        "{FFFFFF}1. Dances & Party\t{B4B5B7}Dance moves, rapping, celebrations\n"\
        "{FFFFFF}2. Street & Gangs\t{B4B5B7}Gang handshakes, signs, lean, arms\n"\
        "{FFFFFF}3. Social & Interactions\t{B4B5B7}Wave, chat, kiss, nods, slap\n"\
        "{FFFFFF}4. Sports & Fight\t{B4B5B7}Boxing, kicks, basketball, baseball\n"\
        "{FFFFFF}5. Poses & Relax\t{B4B5B7}Sitting, laying, relaxation, beds\n"\
        "{FFFFFF}6. Walk Styles\t{B4B5B7}Gang walks, drunk, old, sexy walk\n"\
        "{FFFFFF}7. Expressions & Fun\t{B4B5B7}Laughter, crying, strip, sexy, wank\n"\
        "{FFFFFF}8. Actions & Emergency\t{B4B5B7}Handsup, injured, CPR, bomb, aim"
    );

    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_ANIM_CATEGORIES<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Close");
    return 1;
}

public DIALOG_ANIM_CATEGORIES(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return 1;
    if(IsPlayerInCombat(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot perform animations while in combat.");

    SetPVarInt(playerid, "AnimCategory", listitem);
    new title[64], body[1024];

    switch(listitem)
    {
        case 0: // Dances & Party
        {
            format(title, sizeof(title), "{MAIN}Anims - Dances & Party");
            format(body, sizeof(body),
                "{MAIN}Command\t{MAIN}Animation Action\n"\
                "{FFFFFF}/dance1 - /dance4\t{BEBEBE}Dance routines (1 to 4)\n"\
                "{FFFFFF}/rap - /rap3\t{BEBEBE}Rapping stage moves (1 to 3)\n"\
                "{FFFFFF}/celebrate\t{BEBEBE}Benchpress gym victory\n"\
                "{FFFFFF}/celebrate2\t{BEBEBE}Treadmill victory\n"\
                "{FFFFFF}/win\t{BEBEBE}Casino cards victory\n"\
                "{FFFFFF}/win2\t{BEBEBE}Roulette victory"
            );
        }
        case 1: // Street & Gangs
        {
            format(title, sizeof(title), "{MAIN}Anims - Street & Gangs");
            format(body, sizeof(body),
                "{MAIN}Command\t{MAIN}Animation Action\n"\
                "{FFFFFF}/gang1 - /gang7\t{BEBEBE}Gang handshakes (1 to 7)\n"\
                "{FFFFFF}/gsign1 - /gsign5\t{BEBEBE}Gang hand signs (1 to 5)\n"\
                "{FFFFFF}/ghand1 - /ghand5\t{BEBEBE}Left-hand signs (1 to 5)\n"\
                "{FFFFFF}/lean\t{BEBEBE}Lean against wall idle\n"\
                "{FFFFFF}/crossarms - /crossarms3\t{BEBEBE}Cross arms stance (1 to 3)\n"\
                "{FFFFFF}/joint\t{BEBEBE}Smoke joint on foot\n"\
                "{FFFFFF}/checkout\t{BEBEBE}Inspect graffiti tag"
            );
        }
        case 2: // Social & Interactions
        {
            format(title, sizeof(title), "{MAIN}Anims - Social & Interactions");
            format(body, sizeof(body),
                "{MAIN}Command\t{MAIN}Animation Action\n"\
                "{FFFFFF}/wave\t{BEBEBE}Wave hand to someone\n"\
                "{FFFFFF}/greet\t{BEBEBE}Respectful greeting\n"\
                "{FFFFFF}/chat\t{BEBEBE}Talking idle gesture\n"\
                "{FFFFFF}/nod\t{BEBEBE}Nod head in agreement\n"\
                "{FFFFFF}/shakehead\t{BEBEBE}Shake head in disagreement\n"\
                "{FFFFFF}/kiss1 - /kiss7\t{BEBEBE}Kissing partner (1 to 7)\n"\
                "{FFFFFF}/bitchslap\t{BEBEBE}Slap someone across face\n"\
                "{FFFFFF}/slapped / /slapass\t{BEBEBE}Ass slap reaction\n"\
                "{FFFFFF}/fucku\t{BEBEBE}Middle finger gesture\n"\
                "{FFFFFF}/gift\t{BEBEBE}Give a gift box\n"\
                "{FFFFFF}/thankyou\t{BEBEBE}Thank you bow"
            );
        }
        case 3: // Sports & Fight
        {
            format(title, sizeof(title), "{MAIN}Anims - Sports & Fight");
            format(body, sizeof(body),
                "{MAIN}Command\t{MAIN}Animation Action\n"\
                "{FFFFFF}/box\t{BEBEBE}Shadow boxing workout\n"\
                "{FFFFFF}/akick\t{BEBEBE}Roundhouse martial kick\n"\
                "{FFFFFF}/basket1 - /basket6\t{BEBEBE}Basketball drills (1 to 6)\n"\
                "{FFFFFF}/bat1 - /bat5\t{BEBEBE}Baseball bat swings (1 to 5)\n"\
                "{FFFFFF}/taichi\t{BEBEBE}Tai Chi martial focus\n"\
                "{FFFFFF}/skate\t{BEBEBE}Skateboard run pose"
            );
        }
        case 4: // Poses & Relax
        {
            format(title, sizeof(title), "{MAIN}Anims - Poses & Relax");
            format(body, sizeof(body),
                "{MAIN}Command\t{MAIN}Animation Action\n"\
                "{FFFFFF}/sit\t{BEBEBE}Office chair sit\n"\
                "{FFFFFF}/chairsit\t{BEBEBE}Bar stool sit\n"\
                "{FFFFFF}/fsit / /gro\t{BEBEBE}Ground sitting (Female/Male)\n"\
                "{FFFFFF}/eatsit\t{BEBEBE}Fast food booth sit\n"\
                "{FFFFFF}/lay / /lay2\t{BEBEBE}Sunbathe lying down\n"\
                "{FFFFFF}/relax\t{BEBEBE}Beach chill pose\n"\
                "{FFFFFF}/bed1 - /bed4\t{BEBEBE}Lying in bed (1 to 4)\n"\
                "{FFFFFF}/carsit / /carsit2\t{BEBEBE}Car passenger chilling\n"\
                "{FFFFFF}/stretch\t{BEBEBE}Stretch arms"
            );
        }
        case 5: // Walk Styles
        {
            format(title, sizeof(title), "{MAIN}Anims - Walk Styles");
            format(body, sizeof(body),
                "{MAIN}Command\t{MAIN}Animation Action\n"\
                "{FFFFFF}/gwalk / /gwalk2\t{BEBEBE}Gangster walks (1 and 2)\n"\
                "{FFFFFF}/mwalk\t{BEBEBE}Standard male stride\n"\
                "{FFFFFF}/fwalk\t{BEBEBE}Feminine sexy walk\n"\
                "{FFFFFF}/drunk\t{BEBEBE}Stumbling drunk walk\n"\
                "{FFFFFF}/limp\t{BEBEBE}Elderly limp walk"
            );
        }
        case 6: // Expressions & Fun
        {
            format(title, sizeof(title), "{MAIN}Anims - Expressions & Fun");
            format(body, sizeof(body),
                "{MAIN}Command\t{MAIN}Animation Action\n"\
                "{FFFFFF}/laugh\t{BEBEBE}Hearty laughing\n"\
                "{FFFFFF}/cry1 / /cry2\t{BEBEBE}Crying in grief (1 and 2)\n"\
                "{FFFFFF}/angry\t{BEBEBE}Angry riot shout\n"\
                "{FFFFFF}/chant\t{BEBEBE}Riot crowd chant\n"\
                "{FFFFFF}/piss\t{BEBEBE}Peeing animation\n"\
                "{FFFFFF}/vomit\t{BEBEBE}Puking sickness\n"\
                "{FFFFFF}/scratch\t{BEBEBE}Scratch pose\n"\
                "{FFFFFF}/wank / /wankoff\t{BEBEBE}Masturbation moves\n"\
                "{FFFFFF}/strip1 - /strip7\t{BEBEBE}Striptease dance (1 to 7)\n"\
                "{FFFFFF}/sexy1 - /sexy8\t{BEBEBE}Sexy spanking moves (1 to 8)\n"\
                "{FFFFFF}/bj1 - /bj4\t{BEBEBE}Couch intimates (1 to 4)"
            );
        }
        case 7: // Actions & Emergency
        {
            format(title, sizeof(title), "{MAIN}Anims - Actions & Emergency");
            format(body, sizeof(body),
                "{MAIN}Command\t{MAIN}Animation Action\n"\
                "{FFFFFF}/handsup\t{BEBEBE}Surrender hands up\n"\
                "{FFFFFF}/getarrested\t{BEBEBE}Cuffed arrest stance\n"\
                "{FFFFFF}/injured / /injured2\t{BEBEBE}Wounded bleeding loops\n"\
                "{FFFFFF}/cpr\t{BEBEBE}Perform CPR chest compressions\n"\
                "{FFFFFF}/die / /die2\t{BEBEBE}Death collapses (1 and 2)\n"\
                "{FFFFFF}/crack / /crack2\t{BEBEBE}Crack overdosing\n"\
                "{FFFFFF}/aim / /aim2\t{BEBEBE}One-handed weapon aim poses\n"\
                "{FFFFFF}/cockgun\t{BEBEBE}Pistol slide cocking\n"\
                "{FFFFFF}/bomb\t{BEBEBE}Plant explosive device\n"\
                "{FFFFFF}/cellin / /cellout\t{BEBEBE}Cell phone to ear / holster"
            );
        }
    }

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_ANIM_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Execute", "Back");
    return 1;
}

public DIALOG_ANIM_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        return ShowAnimsCategoryMenu(playerid);
    }

    if(IsPlayerInCombat(playerid))
    {
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot perform animations while in combat.");
    }

    new animCategory = GetPVarInt(playerid, "AnimCategory");

    switch(animCategory)
    {
        case 0: // Dances & Party
        {
            switch(listitem)
            {
                case 0: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE1);
                case 1: PlayAnim(playerid, "RAPPING", "RAP_A_Loop", 4.0, 1, 1, 1, 1, 0);
                case 2: PlayAnim(playerid, "benchpress", "gym_bp_celebrate", 4.0, 1, 0, 0, 0, 0);
                case 3: PlayAnim(playerid, "GYMNASIUM", "gym_tread_celebrate", 4.0, 1, 0, 0, 0, 0);
                case 4: PlayAnim(playerid, "CASINO", "cards_win", 4.0, 1, 0, 0, 0, 0);
                case 5: PlayAnim(playerid, "CASINO", "Roulette_win", 4.0, 1, 0, 0, 0, 0);
            }
        }
        case 1: // Street & Gangs
        {
            switch(listitem)
            {
                case 0: PlayAnim(playerid, "GANGS", "hndshkaa", 4.0, 0, 0, 0, 0, 0);
                case 1: PlayAnim(playerid, "GHANDS", "gsign1", 4.0, 0, 1, 1, 1, 1);
                case 2: PlayAnim(playerid, "GHANDS", "gsign1LH", 4.0, 0, 1, 1, 1, 1);
                case 3: PlayAnim(playerid, "GANGS", "leanIDLE", 4.0, 1, 0, 0, 0, 0);
                case 4: PlayAnim(playerid, "COP_AMBIENT", "Coplook_loop", 4.0, 0, 1, 1, 1, -1);
                case 5: PlayAnim(playerid, "GANGS", "smkcig_prtl", 4.0, 0, 1, 1, 1, 1);
                case 6: PlayAnim(playerid, "GRAFFITI", "graffiti_Chkout", 4.0, 0, 0, 0, 0, 0);
            }
        }
        case 2: // Social
        {
            switch(listitem)
            {
                case 0: PlayAnim(playerid, "ON_LOOKERS", "wave_loop", 4.0, 1, 0, 0, 0, 0);
                case 1: PlayAnim(playerid, "WUZI", "Wuzi_Greet_Wuzi", 4.0, 0, 0, 0, 0, 0);
                case 2: PlayAnim(playerid, "PED", "IDLE_CHAT", 4.1, 1, 1, 1, 1, 1);
                case 3: PlayAnim(playerid, "COP_AMBIENT", "Coplook_nod", 4.0, 0, 0, 0, 0, 0);
                case 4: PlayAnim(playerid, "MISC", "plyr_shkhead", 3.0, 0, 0, 0, 0, 0);
                case 5: PlayAnim(playerid, "KISSING", "Grlfrd_Kiss_01", 4.0, 0, 0, 0, 0, 0);
                case 6: PlayAnim(playerid, "MISC", "bitchslap", 4.0, 0, 0, 0, 0, 0);
                case 7: PlayAnim(playerid, "SWEET", "sweet_ass_slap", 4.0, 0, 0, 0, 0, 0);
                case 8: PlayAnim(playerid, "PED", "fucku", 4.0, 0, 0, 0, 0, 0);
                case 9: PlayAnim(playerid, "KISSING", "gift_give", 4.0, 0, 0, 0, 0, 0);
                case 10: PlayAnim(playerid, "FOOD", "SHP_Thank", 4.0, 1, 0, 0, 0, 0);
            }
        }
        case 3: // Sports & Fight
        {
            switch(listitem)
            {
                case 0: PlayAnim(playerid, "GYMNASIUM", "gym_shadowbox", 4.1, 1, 1, 1, 1, 1);
                case 1: PlayAnim(playerid, "FIGHT_E", "FightKick", 4.0, 0, 0, 0, 0, 0);
                case 2: PlayAnim(playerid, "BSKTBALL", "BBALL_def_loop", 4.0, 1, 0, 0, 0, 0);
                case 3: PlayAnim(playerid, "BASEBALL", "Bat_IDLE", 4.0, 1, 0, 0, 0, 0);
                case 4: PlayAnim(playerid, "PARK", "Tai_Chi_Loop", 4.0, 1, 0, 0, 0, 0);
                case 5: PlayAnim(playerid, "SKATE", "skate_run", 4.1, 1, 1, 1, 1, 1);
            }
        }
        case 4: // Poses & Relax
        {
            switch(listitem)
            {
                case 0: PlayAnim(playerid, "INT_OFFICE", "OFF_Sit_Type_Loop", 4.0, 1, 0, 0, 0, 0);
                case 1: PlayAnim(playerid, "PED", "SEAT_idle", 4.0, 1, 0, 0, 0, 0);
                case 2: PlayAnim(playerid, "BEACH", "ParkSit_M_loop", 4.0, 1, 0, 0, 0, 0);
                case 3: PlayAnim(playerid, "FOOD", "FF_Sit_Loop", 4.0, 1, 0, 0, 0, 0);
                case 4: PlayAnim(playerid, "BEACH", "bather", 4.0, 1, 0, 0, 0, 0);
                case 5: PlayAnim(playerid, "BEACH", "Lay_Bac_Loop", 4.0, 1, 0, 0, 0, 0);
                case 6: PlayAnim(playerid, "INT_HOUSE", "BED_Loop_L", 4.0, 1, 0, 0, 0, 0);
                case 7: PlayAnim(playerid, "CAR", "Tap_hand", 4.0, 1, 0, 0, 0, 0);
                case 8: PlayAnim(playerid, "PLAYIDLES", "stretch", 4.0, 0, 0, 0, 0, 0);
            }
        }
        case 5: // Walk Styles
        {
            switch(listitem)
            {
                case 0: PlayAnim(playerid, "PED", "WALK_gang1", 4.1, 1, 1, 1, 1, 1);
                case 1: PlayAnim(playerid, "ped", "WALK_player", 4.1, 1, 1, 1, 1, 1);
                case 2: PlayAnim(playerid, "ped", "WOMAN_walksexy", 4.1, 1, 1, 1, 1, 1);
                case 3: PlayAnim(playerid, "PED", "WALK_DRUNK", 4.1, 1, 1, 1, 1, 1);
                case 4: PlayAnim(playerid, "PED", "WALK_old", 4.1, 1, 1, 1, 1, 1);
            }
        }
        case 6: // Expressions & Fun
        {
            switch(listitem)
            {
                case 0: PlayAnim(playerid, "RAPPING", "Laugh_01", 4.0, 0, 0, 0, 0, 0);
                case 1: PlayAnim(playerid, "GRAVEYARD", "mrnF_loop", 4.0, 1, 0, 0, 0, 0);
                case 2: PlayAnim(playerid, "RIOT", "RIOT_ANGRY", 4.0, 0, 0, 0, 0, 0);
                case 3: PlayAnim(playerid, "RIOT", "RIOT_CHANT", 4.0, 1, 0, 0, 0, 0);
                case 4: SetPlayerSpecialAction(playerid, SPECIAL_ACTION:68);
                case 5: PlayAnim(playerid, "FOOD", "EAT_Vomit_P", 3.0, 0, 0, 0, 0, 0);
                case 6: PlayAnim(playerid, "MISC", "Scratchballs_01", 4.0, 1, 0, 0, 0, 0);
                case 7: PlayAnim(playerid, "PAULNMAC", "wank_loop", 4.0, 1, 1, 1, 1, 0);
                case 8: PlayAnim(playerid, "STRIP", "strip_A", 4.0, 1, 1, 1, 1, 0);
                case 9: PlayAnim(playerid, "SNM", "SPANKING_IDLEW", 4.1, 0, 1, 1, 1, 1);
                case 10: PlayAnim(playerid, "BLOWJOBZ", "BJ_COUCH_START_P", 4.1, 0, 1, 1, 1, 1);
            }
        }
        case 7: // Actions & Emergency
        {
            switch(listitem)
            {
                case 0: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_HANDSUP);
                case 1: PlayAnim(playerid, "ped", "ARRESTgun", 4.0, 0, 1, 1, 1, -1);
                case 2: PlayAnim(playerid, "SWEET", "Sweet_injuredloop", 4.0, 1, 0, 0, 0, 0);
                case 3: PlayAnim(playerid, "MEDIC", "CPR", 4.0, 1, 0, 0, 0, 0);
                case 4: PlayAnim(playerid, "KNIFE", "KILL_Knife_Ped_Die", 4.1, 0, 1, 1, 1, 1);
                case 5: PlayAnim(playerid, "CRACK", "crckdeth2", 4.0, 1, 0, 0, 0, 0);
                case 6: PlayAnim(playerid, "PED", "gang_gunstand", 4.0, 1, 0, 0, 0, 0);
                case 7: PlayAnim(playerid, "SILENCED", "Silence_reload", 3.0, 0, 0, 0, 0, 0);
                case 8: PlayAnim(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0);
                case 9: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
            }
        }
    }

    SetPVarInt(playerid, "PlayingCustomAnim", 1);
    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Press {MAIN}'F' {FFFFFF}or use {MAIN}/stopanim {FFFFFF}to cancel animation.");
    return 1;
}

// /anims command
@cmd(.alts = {"animlist", "animations", "animmenu"}) anims(playerid, params[], help)
{
    return ShowAnimsCategoryMenu(playerid);
}
