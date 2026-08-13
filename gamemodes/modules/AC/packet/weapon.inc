#if defined _ac_weapons_included
    #endinput
#endif
#define _ac_weapons_included

#include <YSI_Coding\y_hooks>

#define AC_MAX_MELEE_AMMO (10)

const
    AC_RPC_RESET_WEAPONS = 21,
    AC_RPC_GIVE_WEAPON = 22,
    AC_RPC_SET_SPAWN_INFO = 68,
    AC_RPC_DAMAGE = 115,
    AC_RPC_REQUEST_CLASS = 128,
    AC_RPC_WEAP_AMMO = 145,

    AC_PACKET_WEAPONS = 204,
    AC_PACKET_BULLET = 206,
    AC_PACKET_ONFOOT = 207;

static
    WEAPON:ACSpawnWeapon[MAX_PLAYERS + 1][3],
    WEAPON:ACWeapon[MAX_PLAYERS + 1][MAX_WEAPON_SLOTS];

ORPC:AC_RPC_RESET_WEAPONS(playerid, BitStream:bs)
{
    ACWeapon[playerid] = ACWeapon[MAX_PLAYERS];
    return 1;
}

ORPC:AC_RPC_GIVE_WEAPON(playerid, BitStream:bs)
{
    new weaponid, bullets;

    BS_ReadValue(bs,
        PR_UINT32, weaponid,
        PR_UINT32, bullets
    );

    if (bullets)
    {
        ACWeapon[playerid][GetWeaponSlot(WEAPON:weaponid)] = WEAPON:weaponid;
    }

    AC_ResetAmmoSlot(playerid, GetWeaponSlot(WEAPON:weaponid));

    return 1;
}

ORPC:AC_RPC_SET_SPAWN_INFO(playerid, BitStream:bs)
{
    new spawnWeapons[3], spawnWeaponsAmmo[3];

    BS_ReadValue(bs,
        PR_IGNORE_BITS, 8 + 32 + 8 + 32 + 32 + 32 + 32,
        PR_UINT32, spawnWeapons[0],
        PR_UINT32, spawnWeapons[1],
        PR_UINT32, spawnWeapons[2],
        PR_UINT32, spawnWeaponsAmmo[0],
        PR_UINT32, spawnWeaponsAmmo[1],
        PR_UINT32, spawnWeaponsAmmo[2]
    );

    for (new i; i < sizeof(spawnWeapons); i++)
    {
        if (spawnWeaponsAmmo[i])
        {
            ACSpawnWeapon[playerid][i] = WEAPON:spawnWeapons[i];
        }
        else
        {
            ACSpawnWeapon[playerid][i] = WEAPON_FIST;
        }
    }

    return 1;
}

IRPC:AC_RPC_DAMAGE(playerid, BitStream:bs)
{
    new bool:giveOrTake, targetid, weaponid;

    BS_ReadValue(bs,
        PR_BOOL, giveOrTake,
        PR_UINT16, targetid,
        PR_IGNORE_BITS, 32,
        PR_UINT32, weaponid
    );

    if (((IsMeleeWeapon(WEAPON:weaponid) && weaponid != _:WEAPON_PISTOLWHIP) || IsBulletWeapon(WEAPON:weaponid))
       && (
            (!giveOrTake && weaponid != _:ACWeapon[playerid][GetWeaponSlot(WEAPON:weaponid)])
            || (giveOrTake && 0 <= targetid < MAX_PLAYERS && weaponid != _:ACWeapon[targetid][GetWeaponSlot(WEAPON:weaponid)])
       )
    )
    {
        return 0;
    }

    return 1;
}

ORPC:AC_RPC_REQUEST_CLASS(playerid, BitStream:bs)
{
    new requestResponse;
    BS_ReadUint8(bs, requestResponse);

    if (!requestResponse)
    {
        ACSpawnWeapon[playerid] = ACSpawnWeapon[MAX_PLAYERS];
        return 1;
    }

    new spawnWeapons[3], spawnWeaponsAmmo[3];

    BS_ReadValue(bs,
        PR_IGNORE_BITS, 8 + 32 + 8 + 32 + 32 + 32 + 32,
        PR_UINT32, spawnWeapons[0],
        PR_UINT32, spawnWeapons[1],
        PR_UINT32, spawnWeapons[2],
        PR_UINT32, spawnWeaponsAmmo[0],
        PR_UINT32, spawnWeaponsAmmo[1],
        PR_UINT32, spawnWeaponsAmmo[2]
    );

    for (new i; i < sizeof(spawnWeapons); i++)
    {
        if (spawnWeaponsAmmo[i])
        {
            ACSpawnWeapon[playerid][i] = WEAPON:spawnWeapons[i];
        }
        else
        {
            ACSpawnWeapon[playerid][i] = WEAPON_FIST;
        }
    }

    return 1;
}

ORPC:AC_RPC_WEAP_AMMO(playerid, BitStream:bs)
{
    new weaponid, ammo;

    BS_ReadValue(bs,
        PR_UINT8, weaponid,
        PR_UINT16, ammo
    );

    if (!ammo)
    {
        ACWeapon[playerid][GetWeaponSlot(WEAPON:weaponid)] = WEAPON_FIST;
    }

    AC_ResetAmmoSlot(playerid, GetWeaponSlot(WEAPON:weaponid));

    return 1;
}

IPacket:AC_PACKET_WEAPONS(playerid, BitStream:bs)
{
    if (IsPlayerNPC(playerid)) return 1;

    new weaponsUpdate[PR_WeaponsUpdate];

    BS_IgnoreBits(bs, 8);
    BS_ReadWeaponsUpdate(bs, weaponsUpdate);

    if (weaponsUpdate[PR_slotWeaponAmmo][0] > AC_MAX_MELEE_AMMO)
    {
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, E_REJECTING_AC_S0BEIT);
        return 0;
    }

    new bool:hack;

    for (new i; i < PR_MAX_WEAPON_SLOTS; i++)
    {
        if (weaponsUpdate[PR_slotWeaponId][i] != _:ACWeapon[playerid][WEAPON_SLOT:i])
        {
            weaponsUpdate[PR_slotWeaponId][i] =
            weaponsUpdate[PR_slotWeaponAmmo][i] = 0;

            hack = true;
            AC_ResetAmmoSlot(playerid, i);
            continue;
        }

        AC_CheckSlotAmmo(playerid, i, weaponsUpdate[PR_slotWeaponAmmo][i]);
    }

    if (hack)
    {
        BS_SetWriteOffset(bs, 8);
        BS_WriteWeaponsUpdate(bs, weaponsUpdate);
    }

    return 1;
}

IPacket:AC_PACKET_BULLET(playerid, BitStream:bs)
{
    if (IsPlayerNPC(playerid)) return 1;

    new bulletData[PR_BulletSync];

    BS_IgnoreBits(bs, 8);
    BS_ReadBulletSync(bs, bulletData);

    AC_DebugBullet(playerid, bulletData[PR_hitType]);

    AC_CheckSilentAim(playerid, BULLET_HIT_TYPE:bulletData[PR_hitType], bulletData[PR_hitId],
        bulletData[PR_origin][0], bulletData[PR_origin][1], bulletData[PR_origin][2],
        bulletData[PR_hitPos][0], bulletData[PR_hitPos][1], bulletData[PR_hitPos][2]);

    if (bulletData[PR_weaponId] != _:ACWeapon[playerid][GetWeaponSlot(WEAPON:bulletData[PR_weaponId])]) return 0;

    AC_CountShot(playerid, WEAPON:bulletData[PR_weaponId]);

    return 1;
}

IPacket:AC_PACKET_ONFOOT(playerid, BitStream:bs)
{
    if (IsPlayerNPC(playerid)) return 1;

    new onFootData[PR_OnFootSync];

    BS_IgnoreBits(bs, 8);
    BS_ReadOnFootSync(bs, onFootData);

    if (onFootData[PR_weaponId] != _:ACWeapon[playerid][GetWeaponSlot(WEAPON:onFootData[PR_weaponId])])
    {
        onFootData[PR_keys] &= ~_:KEY_FIRE;
        onFootData[PR_weaponId] = WEAPON_FIST;

        BS_SetWriteOffset(bs, 8);
        BS_WriteOnFootSync(bs, onFootData);
    }

    return 1;
}

hook OnPlayerConnect(playerid)
{
    ACSpawnWeapon[playerid] = ACSpawnWeapon[MAX_PLAYERS];
    ACWeapon[playerid] = ACWeapon[MAX_PLAYERS];

    return 1;
}

hook OnPlayerSpawn(playerid)
{
    ACWeapon[playerid] = ACWeapon[MAX_PLAYERS];

    for (new i; i < sizeof(ACSpawnWeapon[]); i++)
    {
        if (ACSpawnWeapon[playerid][i])
        {
            ACWeapon[playerid][GetWeaponSlot(ACSpawnWeapon[playerid][i])] = ACSpawnWeapon[playerid][i];
        }
    }

    return 1;
}

AC_GivePlayerWeapon(playerid, WEAPON:weaponid, ammo)
{
    if (weaponid == WEAPON_BRASSKNUCKLE)
    {
        new WEAPON:weapons;
        GetPlayerWeaponData(playerid, WEAPON_SLOT_UNARMED, weapons, ammo);

        if (weapons == WEAPON_BRASSKNUCKLE && ammo > 0) return 0;

        ammo = 1;
    }

    return GivePlayerWeapon(playerid, weaponid, ammo);
}
#if defined _ALS_GivePlayerWeapon
    #undef GivePlayerWeapon
#else
    #define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon AC_GivePlayerWeapon
