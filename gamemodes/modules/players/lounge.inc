
stock SetupPlayerVehicle(vehicleid)
{
    if (vehicleid != INVALID_VEHICLE_ID && vehicleid != 999)
    {
        SetVehicleNumberPlate(vehicleid, "oldmxa");
        new engine, lights, alarm, doors, bonnet, boot, objective;
        GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
        SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_ON, alarm, doors, bonnet, boot, objective);
    }
    return 1;
}

stock bool:CanUseLoungeUtility(playerid, bool:adminBypass = false, const actionName[] = "use this command")
{
    if (!IsPlayerLogged(playerid))
    {
        SendClientMessage(playerid, -1, "%s", CANNOT_DO_THIS);
        return false;
    }

    if (!IsPlayerAFK(playerid) && (!adminBypass || User[playerid][adminLevel] < 1))
    {
        SendClientMessage(playerid, -1, "%s", NEED_BE_AFK);
        return false;
    }

    if (IsPlayerInMatch(playerid))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot %s while in a match, training, practice or safehouse.", actionName);
        return false;
    }

    if (IsPlayerFrozen(playerid))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot %s right now.", actionName);
        return false;
    }

    if (IsPlayerSpectating(playerid))
    {
        SendClientMessage(playerid, -1, "%s", IN_SPEC_MODE);
        return false;
    }

    new PLAYER_STATE:pState = GetPlayerState(playerid);
    if (pState == PLAYER_STATE_NONE || pState == PLAYER_STATE_WASTED || pState == PLAYER_STATE_SPECTATING)
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot %s right now.", actionName);
        return false;
    }

    return true;
}

forward Lounge_CanSpawnLoungeVehicle(playerid);
public Lounge_CanSpawnLoungeVehicle(playerid)
{
    return CanUseLoungeUtility(playerid, false, "spawn a vehicle") ? 1 : 0;
}

@cmd(.alts = {"b"}) bike(playerid, params[], help)
{
    if (!CanUseLoungeUtility(playerid, false, "spawn a vehicle"))
        return true;

    if (pVehicle[playerid] != 999)
        DestroyVehicle(pVehicle[playerid]);

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    pVehicle[playerid] = CreateVehicle(481, x, y, z, 0, random(100), random(100), -1);
    SetupPlayerVehicle(pVehicle[playerid]);

    SetVehicleVirtualWorld(pVehicle[playerid], GetPlayerVirtualWorld(playerid));
    LinkVehicleToInterior(pVehicle[playerid], GetPlayerInterior(playerid));
    PutPlayerInVehicle(playerid, pVehicle[playerid], 0);

    return true;
}

forward Lounge_SpawnLoungeVehicle(playerid, vehicle, color1, color2);
public Lounge_SpawnLoungeVehicle(playerid, vehicle, color1, color2)
{
    if (vehicle < VEHICLE_ID_MIN || vehicle > VEHICLE_ID_MAX)
        return SendClientMessage(playerid, -1, "%s", INVALID_VEHICLE_MSG);

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    if (pVehicle[playerid] != 999)
        DestroyVehicle(pVehicle[playerid]);

    pVehicle[playerid] = CreateVehicle(vehicle, x, y, z, 0, color1, color2, -1);
    SetupPlayerVehicle(pVehicle[playerid]);
    SetVehicleVirtualWorld(pVehicle[playerid], GetPlayerVirtualWorld(playerid));
    LinkVehicleToInterior(pVehicle[playerid], GetPlayerInterior(playerid));
    PutPlayerInVehicle(playerid, pVehicle[playerid], 0);

    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}When you exit the vehicle it will be automatically deleted!");

    va_SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}You created the vehicle {FFFFFF}'{0CB80C}%s{FFFFFF}' {13A9F6}ID {FFFFFF}'{0CB80C}%d{FFFFFF}' !", VehicleNames[vehicle - VEHICLE_ID_MIN], vehicle);

    return true;
}

@cmd() car(playerid, params[], help)
{
    if (!CanUseLoungeUtility(playerid, false, "spawn a vehicle"))
        return true;

    if (IsNull(params))
        return ShowVehicleDialog(playerid);

    new vehicle, vehiclestr[30], color1 = -1, color2 = -1;
    if (sscanf(params, "s[30]D(-1)D(-1)", vehiclestr, color1, color2))
        return SendClientMessage(playerid, -1, "%s", VEHICLE_SPAWN_HELP);

    if (IsNumeric(vehiclestr))
    {
        vehicle = strval(vehiclestr);
    }
    else
    {
        vehicle = GetVehicleModelIDFromName(vehiclestr);
    }

    return SpawnLoungeVehicle(playerid, vehicle, color1, color2);
}

@cmd() getgun(playerid, params[], help)
{
    if(!CanUseLoungeUtility(playerid, false, "use /getgun"))
        return true;

    new weaponInput[32], ammo, weapid = -1;

    if(sscanf(params, "s[32]d", weaponInput, ammo))
        return SendClientMessage(playerid, -1, "%s", GETGUN_HELP);

    if(IsNumeric(weaponInput))
    {
        weapid = strval(weaponInput);
    }
    else
    {
        weapid = GetWeaponModelIDFromName(weaponInput);
    }

    if(weapid == -1)
        return SendClientMessage(playerid, -1, "%s", GETGUN_HELP);

    if(ammo < 0 || ammo > 9999)
        return SendClientMessage(playerid, -1, "%s", INVALID_AMMO_MSG);

    if(weapid < 1 || weapid > 46 || weapid == 19 || weapid == 20 || weapid == 21)
        return SendClientMessage(playerid, -1, "%s", INVALID_ID_MSG);

    if(weapid == 44 || weapid == 45)
        return SendClientMessage(playerid, -1, "%s", RETARD);

    GivePlayerWeapon(playerid, WEAPON:weapid, ammo);
    return true;
}

@cmd(.alts = {"l", "afk"}) lounge(playerid, params[], help)
{
    new String[150];
    new Float:Armour, Float:Health;
    GetPlayerArmour(playerid, Armour);
    GetPlayerHealth(playerid, Health);

    if (floatround(Health + Armour) < 200)
        return va_SendClientMessage(playerid, -1, HEALTH_ERROR_MSG, 200);

    new PLAYER_STATE:playerState = GetPlayerState(playerid);
    if(playerState == PLAYER_STATE_SPECTATING || IsPlayerSpectating(playerid))
        return SendClientMessage(playerid, -1, "%s", IN_SPEC_MODE);

    if (IsPlayerInMatch(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use /lounge while in a match, training, practice or safehouse.");

    if (IsPlayerAFK(playerid))
    {
        if(User[playerid][locationId] == -1)
        {
            return SendClientMessage(playerid, -1, "{MAIN}** {FFFFFF}You need to enter an arena first using /changemode.");
        }

        if(User[playerid][lastLocation] < 0 || User[playerid][lastLocation] >= MAX_LOCATIONS || User[playerid][lastLocation] == ARENA_AFK)
        {
            return SendClientMessage(playerid, -1, "{MAIN}** {FFFFFF}You need to enter an arena first.");
        }
        else
        {
            new oldLocation = User[playerid][locationId];

            User[playerid][isAfk] = false;
            User[playerid][locationId] = User[playerid][lastLocation];

            ApplyArenaRpWalk(playerid, oldLocation, User[playerid][locationId]);
        }

        SpawnPlayerEx(playerid);
        SendClientMessage(playerid, -1, "%s", GetRandomLeaveLoungeMsg());
    }
    else
    {
        new coloredNick[MAX_PLAYER_NAME + 18];
        format(coloredNick, sizeof(coloredNick), "{%06x}%s{FFFFFF}", GetPlayerDisplayColour(playerid), getPlayerName(playerid));
        format(String, sizeof(String), GetRandomEnterLoungeMsg(), coloredNick);

        SendClientMessageToAll(-1, "%s", String);
        SendClientMessage(playerid, -1, "%s", AFK_MSG_2);
        SendClientMessage(playerid, -1, "%s", AFK_MSG_3);
        SendClientMessage(playerid, -1, "%s", AFK_MSG_4);

        setAfk(playerid);
        updatePlayerHud(playerid);
    }
    return true;
}

ShowLoungeCommands(playerid)
{
    new title[64], cmdsstr[1536];
    format(title, sizeof(title), "{MAIN}Lounge Commands");
    format_MainColor(playerid, title, sizeof(title));

    format(cmdsstr, sizeof(cmdsstr), "{BEBEBE}Command\t{BEBEBE}Description\n");
    strcat(cmdsstr, "{FFFFFF}/car [name/id]\t{BEBEBE}Spawn custom vehicle (or visual picker)\n");
    strcat(cmdsstr, "{FFFFFF}/bike\t{BEBEBE}Spawn a fast bicycle\n");
    strcat(cmdsstr, "{FFFFFF}/fix\t{BEBEBE}Repair and restore vehicle health\n");
    strcat(cmdsstr, "{FFFFFF}/tune\t{BEBEBE}Add Nitro and Hydraulics to vehicle\n");
    strcat(cmdsstr, "{FFFFFF}/flip\t{BEBEBE}Flip overturned vehicle back up\n");
    strcat(cmdsstr, "{FFFFFF}/jetpack\t{BEBEBE}Equip a jetpack to fly freely in lounge\n");
    strcat(cmdsstr, "{FFFFFF}/getgun [gun] [ammo]\t{BEBEBE}Spawn custom weapons with ammo\n");
    strcat(cmdsstr, "{FFFFFF}/changelounge\t{BEBEBE}Save custom lounge spawn position\n");
    strcat(cmdsstr, "{FFFFFF}/training\t{BEBEBE}Open training arena & bot challenges menu\n");
    strcat(cmdsstr, "{FFFFFF}/prac\t{BEBEBE}Private aim practice with target NPC\n");
    strcat(cmdsstr, "{FFFFFF}/spec [id]\t{BEBEBE}Spectate players in combat arenas\n");
    strcat(cmdsstr, "{FFFFFF}/specoff\t{BEBEBE}Stop spectating and return to lounge\n");
    strcat(cmdsstr, "{FFFFFF}/anims\t{BEBEBE}Open animations & gestures menu\n");
    strcat(cmdsstr, "{FFFFFF}/stopanim\t{BEBEBE}Stop active animation (or press 'F')\n");
    strcat(cmdsstr, "{FFFFFF}/changemode\t{BEBEBE}Select a DM arena to battle\n");
    strcat(cmdsstr, "{FFFFFF}/lounge\t{BEBEBE}Exit lounge and return to combat arena");

    Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, cmdsstr, "Close", "");
    return 1;
}

@cmd() lcmds(playerid, params[], help)
{
    if(!CanUseLoungeUtility(playerid, false, "view lounge commands"))
        return true;

    ShowLoungeCommands(playerid);
    return true;
}

@cmd() fix(playerid, params[], help)
{
    if(!CanUseLoungeUtility(playerid, true, "fix vehicles"))
        return true;

    if(!IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be in a vehicle.");

    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be the driver.");

    new vehicleid = GetPlayerVehicleID(playerid);
    RepairVehicle(vehicleid);
    SetVehicleHealth(vehicleid, 1000.0);
    
    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Vehicle repaired!");
    return true;
}

@cmd(.alts = {"msp"}) changelounge(playerid, params[], help)
{
    if(!CanUseLoungeUtility(playerid, false, "change lounge spawn"))
        return true;

    if(IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot change your lounge spawn while inside a vehicle. Exit first.");

    if(GetPlayerVirtualWorld(playerid) != LOUNGEVW || GetPlayerInterior(playerid) != 0)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You must be inside the official Lounge world to set your spawn.");

    if(User[playerid][spawnSafehouse])
    {
        if(GetPlayerSafehouse(playerid) == -1)
        {
            User[playerid][spawnSafehouse] = false;
            UpdatePlayerData(playerid);
        }
        else
        {
            SendClientMessage(playerid, -1, "{MAIN}[LNG] {FFFFFF}You spawn at your clan safehouse, so a custom lounge would be ignored.");
            return SendClientMessage(playerid, -1, "{MAIN}[LNG] {FFFFFF}Disable {MAIN}Safehouse Spawn{FFFFFF} in {MAIN}/clan{FFFFFF} first.");
        }
    }

    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

    User[playerid][loungeX] = x;
    User[playerid][loungeY] = y;
    User[playerid][loungeZ] = z;
    User[playerid][loungeA] = a;
    User[playerid][loungeVW] = LOUNGEVW;
    User[playerid][loungeINT] = 0;

    UpdatePlayerData(playerid);
    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Your lounge spawn location has been updated!");
    return true;
}

@cmd() jetpack(playerid, params[], help)
{
    if(!CanUseLoungeUtility(playerid, true, "get a jetpack"))
        return true;

    if(IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot get a jetpack while in a vehicle.");

    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK);
    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}You received a jetpack.");
    return true;
}

@cmd() flip(playerid, params[], help)
{
    if(!CanUseLoungeUtility(playerid, true, "flip vehicles"))
        return true;

    if(!IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You are not in a vehicle.");

    new vehicleid = GetPlayerVehicleID(playerid);
    new Float:x, Float:y, Float:z, Float:a;
    GetVehiclePos(vehicleid, x, y, z);
    GetVehicleZAngle(vehicleid, a);
    SetVehiclePos(vehicleid, x, y, z + 2.0);
    SetVehicleZAngle(vehicleid, a);

    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Vehicle flipped.");
    return true;
}

@cmd() tune(playerid, params[], help)
{
    if(!CanUseLoungeUtility(playerid, true, "tune vehicles"))
        return true;

    if(!IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You are not in a vehicle.");

    new vehicleid = GetPlayerVehicleID(playerid);
    AddVehicleComponent(vehicleid, 1010);
    AddVehicleComponent(vehicleid, 1087);
    
    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Vehicle tuned (Nitro + Hydraulics).");
    return true;
}

// Lounge Server Info & Tips System
static const g_LoungeTips[][] =
{
    "{FFFFFF}Press {MAIN}'N' {FFFFFF}key to open the quick navigation and server help menu.",
    "{FFFFFF}Use {MAIN}/help {FFFFFF}or {MAIN}/cmds {FFFFFF}for tutorials, full commands and server rules.",
    "{FFFFFF}Type {MAIN}/changemode {FFFFFF}to choose or switch your DM combat arena anytime.",
    "{FFFFFF}Type {MAIN}/lounge {FFFFFF}or {MAIN}/afk {FFFFFF}to enter or leave the AFK lounge lobby.",
    "{FFFFFF}Use {MAIN}/gunmenu {FFFFFF}to customize and save your arena spawn weapon loadout.",
    "{FFFFFF}Got low health in the lounge? Step near snacks or wait to auto-regenerate.",
    "{FFFFFF}Challenge another player to an isolated 1v1 battle using {MAIN}/duel [id]{FFFFFF}.",
    "{FFFFFF}Accept incoming 1v1 duel challenges from players using {MAIN}/acceptduel{FFFFFF}.",
    "{FFFFFF}Exit or cancel an active duel match at any moment with {MAIN}/backduel{FFFFFF}.",
    "{FFFFFF}Team up with a partner and challenge pairs using {MAIN}/duel2v2{FFFFFF}.",
    "{FFFFFF}Accept a 2v2 duel request with your teammate using {MAIN}/accept2v2{FFFFFF}.",
    "{FFFFFF}Cancel or decline an ongoing 2v2 duel invitation with {MAIN}/cancel2v2{FFFFFF}.",
    "{FFFFFF}Start individual aim practice with custom targets using {MAIN}/prac{FFFFFF}.",
    "{FFFFFF}Practice mode allows you to switch targets between {MAIN}NPC {FFFFFF}and {MAIN}Ball{FFFFFF}.",
    "{FFFFFF}Customize the practice target skin ID directly inside the {MAIN}/prac {FFFFFF}menu.",
    "{FFFFFF}Simulate real combat lag by adjusting target latency in {MAIN}/prac{FFFFFF}.",
    "{FFFFFF}Adjust target movement speed in practice mode to train high-speed tracking.",
    "{FFFFFF}Set custom movement paths and routes for your practice target in {MAIN}/prac{FFFFFF}.",
    "{FFFFFF}Check your practice DPS, accuracy rate and shots landed via {MAIN}/prac stats{FFFFFF}.",
    "{FFFFFF}Restart or reposition your practice session quickly with {MAIN}/prac restart{FFFFFF}.",
    "{FFFFFF}Leave the practice arena and return to the lounge with {MAIN}/prac exit{FFFFFF}.",
    "{FFFFFF}Enter dedicated training obstacle courses and bot arenas with {MAIN}/training{FFFFFF}.",
    "{FFFFFF}Compete for the fastest record on server training challenges via {MAIN}/training{FFFFFF}.",
    "{FFFFFF}Restart your current training arena challenge using {MAIN}/restarttraining{FFFFFF}.",
    "{FFFFFF}Exit your active training arena anytime using {MAIN}/exittraining{FFFFFF}.",
    "{FFFFFF}Create or manage your clan with {MAIN}/clan {FFFFFF}and dominate territories!",
    "{FFFFFF}Found a new clan with friends for $500k using {MAIN}/createclan [tag] [name]{FFFFFF}.",
    "{FFFFFF}Clan leaders can invite new members to the clan using {MAIN}/invite [id]{FFFFFF}.",
    "{FFFFFF}Received a clan invitation? Type {MAIN}/accept {FFFFFF}to accept and join.",
    "{FFFFFF}Leave your current clan at any time by typing {MAIN}/leaveclan{FFFFFF}.",
    "{FFFFFF}Check all active clans, leaders and statistics using {MAIN}/clans{FFFFFF}.",
    "{FFFFFF}View full clan commands list and permissions using {MAIN}/clancmds{FFFFFF}.",
    "{FFFFFF}Approach an Outpost torch and press {MAIN}'Y' {FFFFFF}to capture it for your clan.",
    "{FFFFFF}Captured Outposts grant passive money and points for your clan every 5 mins.",
    "{FFFFFF}Check the ownership status of all Outpost zones with {MAIN}/territories{FFFFFF}.",
    "{FFFFFF}Outposts stay locked for 10 minutes after capture before enemies can re-take.",
    "{FFFFFF}Visit, upgrade and manage your clan hideout using {MAIN}/safemenu{FFFFFF}.",
    "{FFFFFF}Leave your clan safehouse and return to the lounge with {MAIN}/exitsafehouse{FFFFFF}.",
    "{FFFFFF}Clan leaders can customize and edit base furniture with {MAIN}/editsafehouse{FFFFFF}.",
    "{FFFFFF}Access {MAIN}/settings {FFFFFF}to customize HUD, sounds, PMs and fighting styles.",
    "{FFFFFF}Enable or disable the combat hit sound (bell) inside {MAIN}/settings{FFFFFF}.",
    "{FFFFFF}Toggle spawn protection on or off inside {MAIN}/settings{FFFFFF}.",
    "{FFFFFF}Change your melee fighting style using {MAIN}/fight {FFFFFF}or via {MAIN}/settings{FFFFFF}.",
    "{FFFFFF}Toggle private messages on or off anytime inside the {MAIN}/settings {FFFFFF}menu.",
    "{FFFFFF}Toggle connect and disconnect chat notifications inside {MAIN}/settings{FFFFFF}.",
    "{FFFFFF}Toggle headshot announcement text on or off inside {MAIN}/settings{FFFFFF}.",
    "{FFFFFF}Customize your health, armor and HUD layout using {MAIN}/customhud{FFFFFF}.",
    "{FFFFFF}Personalize your main chat text color using {MAIN}/chatcolor{FFFFFF}.",
    "{FFFFFF}Configure damage informer floating numbers with {MAIN}/dmginformer{FFFFFF}.",
    "{FFFFFF}Toggle and customize your combat damage feed with {MAIN}/dmgfeed{FFFFFF}.",
    "{FFFFFF}Set custom colors for your damage feed using {MAIN}/dmgfeedcolor{FFFFFF}.",
    "{FFFFFF}Change your crosshair hitmarker color using {MAIN}/hitmarkcol{FFFFFF}.",
    "{FFFFFF}Choose between classic, bar or compact HP HUDs via {MAIN}/customhud{FFFFFF}.",
    "{FFFFFF}Check your KD, kills, damage, streaks and history with {MAIN}/stats{FFFFFF}.",
    "{FFFFFF}View rank requirements and progression table using {MAIN}/ranks{FFFFFF}.",
    "{FFFFFF}Complete in-game challenges to earn coins and perks via {MAIN}/achievements{FFFFFF}.",
    "{FFFFFF}Check your server coins balance and rewards using {MAIN}/coins{FFFFFF}.",
    "{FFFFFF}Track your coin transactions, purchases and history using {MAIN}/logcoins{FFFFFF}.",
    "{FFFFFF}Change your account nickname anytime using {MAIN}/changenick{FFFFFF}.",
    "{FFFFFF}Update and secure your account password with {MAIN}/changepass{FFFFFF}.",
    "{FFFFFF}Link your account to Discord for sync & rewards: {MAIN}/linkdiscord{FFFFFF}.",
    "{FFFFFF}Enter your Discord verification code in-game using {MAIN}/dcode [code]{FFFFFF}.",
    "{FFFFFF}Unlink your Discord account from the server using {MAIN}/udiscord{FFFFFF}.",
    "{FFFFFF}Watch other players in combat using {MAIN}/spec [id] {FFFFFF}and {MAIN}/specoff{FFFFFF}.",
    "{FFFFFF}Switch between spectated players easily with {MAIN}/specnext {FFFFFF}and {MAIN}/specprev{FFFFFF}.",
    "{FFFFFF}Respawn instantly in your arena using {MAIN}/respawn {FFFFFF}or {MAIN}/re{FFFFFF}.",
    "{FFFFFF}Found a rule breaker or suspect? Report them via {MAIN}/report [id] [reason]{FFFFFF}.",
    "{FFFFFF}Have a question or need help? Submit it to staff with {MAIN}/ask [question]{FFFFFF}.",
    "{FFFFFF}Send a private whisper to another player using {MAIN}/pm [id] [message]{FFFFFF}.",
    "{FFFFFF}Look up a player's ID and connection details using {MAIN}/id [name/id]{FFFFFF}.",
    "{FFFFFF}Check active administrators online on the server using {MAIN}/admins{FFFFFF}.",
    "{FFFFFF}Resynchronize your player position and stream using {MAIN}/sync{FFFFFF}.",
    "{FFFFFF}Change client weather with {MAIN}/weather [id] {FFFFFF}and time with {MAIN}/time [hour]{FFFFFF}.",
    "{FFFFFF}Vote to kick an arena disrupter using {MAIN}/votekick [id]{FFFFFF}.",
    "{FFFFFF}Discover exclusive perks, custom tags and VIP benefits using {MAIN}/vip{FFFFFF}.",
    "{FFFFFF}Check all VIP commands and privileges available with {MAIN}/vipcmds{FFFFFF}.",
    "{FFFFFF}See all active VIP members currently playing with {MAIN}/vips{FFFFFF}.",
    "{FFFFFF}VIP members can toggle their VIP chat visibility using {MAIN}/von {FFFFFF}and {MAIN}/voff{FFFFFF}.",
    "{FFFFFF}VIP players can select custom visual name tags using {MAIN}/vtag{FFFFFF}.",
    "{FFFFFF}Activate your purchased VIP key directly in-game using {MAIN}/buyvip [key]{FFFFFF}.",
    "{FFFFFF}Spawn a bike in the lounge with {MAIN}/bike {FFFFFF}and tune it with {MAIN}/tune{FFFFFF}.",
    "{FFFFFF}Spawn a car in the lounge using {MAIN}/car [model/name]{FFFFFF}.",
    "{FFFFFF}Repair your lounge vehicle instantly by typing {MAIN}/fix{FFFFFF}.",
    "{FFFFFF}Flipped your vehicle in the lounge? Unflip it instantly using {MAIN}/flip{FFFFFF}.",
    "{FFFFFF}Fly and explore the lounge freely by equipping a jetpack with {MAIN}/jetpack{FFFFFF}.",
    "{FFFFFF}Customize your lounge spawn position coordinates using {MAIN}/changelounge{FFFFFF}.",
    "{FFFFFF}See all available lounge utilities and shortcuts with {MAIN}/lcmds{FFFFFF}.",
    "{FFFFFF}Get custom weapon loadouts while in the lounge using {MAIN}/getgun [name/id] [ammo]{FFFFFF}.",
    "{FFFFFF}Perform dance animations anytime for fun using {MAIN}/dance [1-4]{FFFFFF}.",
    "{FFFFFF}Stop any active character animation immediately using {MAIN}/stopanim{FFFFFF}.",
    "{FFFFFF}VIPs & Staff can toggle highlight flashing colors using {MAIN}/highlight [on/off]{FFFFFF}.",
    "{FFFFFF}Check your weekly kills and damage on the lounge ranking board.",
    "{FFFFFF}Reach the weekly TOP 5 to have your character immortalized as a lounge statue!",
    "{FFFFFF}Join our official Discord community for events, tournaments and support: {MAIN}/discord{FFFFFF}.",
    "{FFFFFF}Check server development team and contributors using {MAIN}/credits{FFFFFF}.",
    "{FFFFFF}Keep your kill streak going in DM arenas to trigger exclusive Spree announcements!",
    "{FFFFFF}Headshots with Sniper or Desert Eagle deal lethal bonus critical damage.",
    "{FFFFFF}Change your skin appearance instantly using {MAIN}/changeskin [skin_id]{FFFFFF}.",
    "{FFFFFF}Fast-switch weapons using Q/E or scroll wheel to master C-Bug and slide mechanics.",
    "{FFFFFF}Have fun, respect other players and welcome to Rejecting Deathmatch!"
};

static Text3D:s_LoungeTipLabel = Text3D:INVALID_3DTEXT_ID;
static Text3D:s_LoungeTipNameLabel = Text3D:INVALID_3DTEXT_ID;
static s_LoungeTipActor = INVALID_ACTOR_ID;
static s_CurrentTipIdx = 0;

stock GetServerMainHex(dest[], len = sizeof(dest))
{
    if(1 <= ServerDefaultChatMainColor <= sizeof(hudShades))
    {
        format(dest, len, "%06x", hudShades[ServerDefaultChatMainColor - 1][shadeHex]);
        return 1;
    }
    if(0 <= ServerDefaultHudShadeOne < sizeof(hudShades))
    {
        format(dest, len, "%06x", hudShades[ServerDefaultHudShadeOne][shadeHex]);
        return 1;
    }
    new idx = ServerDefaultHudColor;
    if(idx < 0 || idx >= sizeof(CoresDoBglh)) idx = 0;
    format(dest, len, "%s", CoresDoBglh[idx]);
    return 1;
}

stock FormatLoungeTipStr(idx, dest[], len = sizeof(dest))
{
    new mainHex[7], tag[16], formattedTip[256];
    GetServerMainHex(mainHex);
    format(tag, sizeof(tag), "{%s}", mainHex);

    format(formattedTip, sizeof(formattedTip), "%s", g_LoungeTips[idx]);

    new pos, from, taglen = strlen(tag);
    while((pos = strfind(formattedTip, "{MAIN}", true, from)) != -1)
    {
        strdel(formattedTip, pos, pos + 6);
        strins(formattedTip, tag, pos, sizeof(formattedTip));
        from = pos + taglen;
    }

    format(dest, len, "%s", formattedTip);
    return 1;
}

forward RotateLoungeTip();
public RotateLoungeTip()
{
    if (s_LoungeTipLabel == Text3D:INVALID_3DTEXT_ID) return 1;

    s_CurrentTipIdx = (s_CurrentTipIdx + 1) % sizeof(g_LoungeTips);

    new tipBuf[256];
    FormatLoungeTipStr(s_CurrentTipIdx, tipBuf);
    UpdateDynamic3DTextLabelText(s_LoungeTipLabel, -1, tipBuf);
    return 1;
}

stock InitLoungeTipSystem()
{
    if (IsValidDynamicActor(s_LoungeTipActor))
        DestroyDynamicActor(s_LoungeTipActor);

    s_LoungeTipActor = CreateDynamicActor(154, g_LoungeTipPos[0], g_LoungeTipPos[1], g_LoungeTipPos[2], g_LoungeTipPos[3]);
    SetDynamicActorVirtualWorld(s_LoungeTipActor, LOUNGEVW);
    ApplyDynamicActorAnimation(s_LoungeTipActor, "PAULNMAC", "wank_loop", 4.1, 1, 0, 0, 0, 0);

    if (IsValidDynamic3DTextLabel(s_LoungeTipNameLabel))
        DestroyDynamic3DTextLabel(s_LoungeTipNameLabel);

    new nameBuf[64], mainHex[7];
    GetServerMainHex(mainHex);
    format(nameBuf, sizeof(nameBuf), "{%s}man the fvck up", mainHex);
    s_LoungeTipNameLabel = CreateDynamic3DTextLabel(nameBuf, -1, g_LoungeTipPos[0], g_LoungeTipPos[1], g_LoungeTipPos[2] + 1.0, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, LOUNGEVW, 0, -1, 15.0);

    if (IsValidDynamic3DTextLabel(s_LoungeTipLabel))
        DestroyDynamic3DTextLabel(s_LoungeTipLabel);

    s_CurrentTipIdx = random(sizeof(g_LoungeTips));

    new tipBuf[256];
    FormatLoungeTipStr(s_CurrentTipIdx, tipBuf);
    s_LoungeTipLabel = CreateDynamic3DTextLabel(tipBuf, -1, g_LoungeTipPos[0], g_LoungeTipPos[1], g_LoungeTipPos[2], 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, LOUNGEVW, 0, -1, 15.0);

    SetTimer("RotateLoungeTip", 15000, true);
    return 1;
}

#include <YSI_Coding/y_hooks>
hook OnGameModeInit()
{
    InitLoungeTipSystem();
    return 1;
}
