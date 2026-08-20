#if defined _safehouse_enums_included
    #endinput
#endif
#define _safehouse_enums_included

forward InitSafehouseSystem();
forward OnSafehousesLoad();
forward OnSafehouseConfigLoad();
forward OnSafehouseInserted(index);
forward SafehouseTick();
forward RestoreSafehouseSpawn(playerid);

forward bool:IsSafehouseMember(playerid, index);
forward bool:IsSafehouseLeader(playerid, index);
forward bool:CanUseSafehouseVault(playerid, index);

enum E_SAFEHOUSE
{
    sh_SQLID,
    sh_ClanID,
    sh_Name[SAFEHOUSE_NAME_LEN],
    sh_Password[SAFEHOUSE_PASS_LEN],
    sh_Level,
    sh_Interior,
    sh_VirtualWorld,

    Float:sh_ExtX, Float:sh_ExtY, Float:sh_ExtZ,
    Float:sh_IntX, Float:sh_IntY, Float:sh_IntZ, Float:sh_IntA,
    Float:sh_ExitX, Float:sh_ExitY, Float:sh_ExitZ,

    sh_Vault,
    sh_BoughtAt,
    sh_ExpiresAt,
    sh_NextCharge,
    sh_LastCharge,
    sh_Status,
    bool:sh_ExpiringShown,

    STREAMER_TAG_PICKUP:sh_Pickup,
    STREAMER_TAG_MAP_ICON:sh_MapIcon,
    Text3D:sh_Label
}

new SafehouseData[MAX_SAFEHOUSES][E_SAFEHOUSE];

enum E_SAFEHOUSE_LEVEL
{
    lvl_Name[24],
    lvl_Price,
    lvl_Coins,
    lvl_Slots,
    lvl_Interior,
    Float:lvl_IntX, Float:lvl_IntY, Float:lvl_IntZ, Float:lvl_IntA
}

new g_PlayerInSafehouse[MAX_PLAYERS];

new g_PlayerSafehousePrompt[MAX_PLAYERS];

new g_PlayerSafehouseEdit[MAX_PLAYERS];

new g_PlayerSafehouseCfg[MAX_PLAYERS];
new g_PlayerSafehouseEditField[MAX_PLAYERS];

new g_SafehouseEditList[MAX_PLAYERS][MAX_SAFEHOUSES];
new g_SafehouseEditCount[MAX_PLAYERS];
