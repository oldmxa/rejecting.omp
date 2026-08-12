#if defined _ac_compat_included
    #endinput
#endif
#define _ac_compat_included

#if !defined MAX_WEAPON_SLOTS
    #define MAX_WEAPON_SLOTS (13)
#endif

#if !defined WEAPON_SLOT_UNARMED
    #define WEAPON_SLOT_UNARMED (0)
#endif

#if !defined VARTYPE_NONE
    #define VARTYPE_NONE PLAYER_VARTYPE_NONE
#endif

stock GetWeaponSlot(WEAPON:weaponid)
{
    switch (_:weaponid)
    {
        case 0, 1:                  return 0;
        case 2 .. 9:                return 1;
        case 22, 23, 24:            return 2;
        case 25, 26, 27:            return 3;
        case 28, 29, 32:            return 4;
        case 30, 31:                return 5;
        case 33, 34:                return 6;
        case 35, 36, 37, 38:        return 7;
        case 16, 17, 18, 39:        return 8;
        case 41, 42, 43:            return 9;
        case 10 .. 15:              return 10;
        case 44, 45, 46:            return 11;
        case 40:                    return 12;
    }
    return 0;
}
