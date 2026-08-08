
#include <YSI_Visual/y_dialog>

forward ShowInteriorsMenu(playerid);
forward OnEnterInterior(playerid, IntID);

forward DIALOG_INTERIORES(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_INTERIORES(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        ClearChat(playerid, 100);
        SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You cancelled the interior viewing mode and returned to the original world.");
        SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}If you want to start again, type {MAIN}/interiors{FFFFFF}.");
        SetPlayerVirtualWorld(playerid, User[playerid][int_VW]);
        
        new Float:pPos[3];
        GetPlayerPos(playerid, pPos[0], pPos[1], pPos[2]);
        if(User[playerid][int_PosX] != pPos[0] || User[playerid][int_PosY] != pPos[1] || User[playerid][int_PosZ] != pPos[2])
        {
            SetPlayerPos(playerid, User[playerid][int_PosX], User[playerid][int_PosY], User[playerid][int_PosZ]);
            SetPlayerInterior(playerid, User[playerid][int_PInterior]);
            User[playerid][int_InteriorSelecionado] = 0;
            return 1;
        }
        User[playerid][int_InteriorSelecionado] = 0;
        return 1;
    }

    new stringcat[2000];
    switch(listitem)
    {
        case 0: // Gyms
        {
            strcat(stringcat, "{00EEFE}[01] {FFFFFF}Ganton Gym \n");
            strcat(stringcat, "{00EEFE}[29] {FFFFFF}Below The Belt Gym (Las Venturas) \n");
            strcat(stringcat, "{00EEFE}[110] {FFFFFF}Cobra Gym \n");
            Dialog_ShowCallback(playerid, using public DIALOG_GYMS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 1: // Airports
        {
            strcat(stringcat, "{00EEFE}[112] {FFFFFF}Los Santos Airport \n");
            strcat(stringcat, "{00EEFE}[117] {FFFFFF}Francis International Airport \n");
            Dialog_ShowCallback(playerid, using public DIALOG_AIRPORTS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 2: // Apartments
        {
            strcat(stringcat, "{00EEFE}[09] {FFFFFF}B Dup Apartment 1 \n");
            strcat(stringcat, "{00EEFE}[10] {FFFFFF}B Dup Apartment 2 \n");
            Dialog_ShowCallback(playerid, using public DIALOG_APARTMENTS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 3: // Warehouses
        {
            strcat(stringcat, "{00EEFE}[07] {FFFFFF}Warehouse 1 \n");
            strcat(stringcat, "{00EEFE}[08] {FFFFFF}Warehouse 2 \n");
            strcat(stringcat, "{00EEFE}[99] {FFFFFF}Warehouse 3 \n");
            strcat(stringcat, "{00EEFE}[103] {FFFFFF}Port \n");
            Dialog_ShowCallback(playerid, using public DIALOG_WAREHOUSES<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 4: // Aircrafts
        {
            strcat(stringcat, "{00EEFE}[74] {FFFFFF}Shamal \n");
            strcat(stringcat, "{00EEFE}[137] {FFFFFF}Andromada \n");
            Dialog_ShowCallback(playerid, using public DIALOG_AIRCRAFTS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 5: // Bars/Restaurants
        {
            strcat(stringcat, "{00EEFE}[21] {FFFFFF}Restaurant 1 \n");
            strcat(stringcat, "{00EEFE}[22] {FFFFFF}Pizza Stack \n");
            strcat(stringcat, "{00EEFE}[23] {FFFFFF}Rusty Brown's Donuts \n");
            strcat(stringcat, "{00EEFE}[31] {FFFFFF}World of Coq \n");
            strcat(stringcat, "{00EEFE}[47] {FFFFFF}Burger Shot \n");
            strcat(stringcat, "{00EEFE}[75] {FFFFFF}The Welcome Pump \n");
            strcat(stringcat, "{00EEFE}[85] {FFFFFF}Palamino Restaurant \n");
            strcat(stringcat, "{00EEFE}[87] {FFFFFF}Lil' Probe Inn \n");
            strcat(stringcat, "{00EEFE}[97] {FFFFFF}Jay's Restaurant \n");
            strcat(stringcat, "{00EEFE}[126] {FFFFFF}Restaurant \n");
            strcat(stringcat, "{00EEFE}[130] {FFFFFF}Cluckin' Bell \n");
            strcat(stringcat, "{00EEFE}[139] {FFFFFF}Bar \n");
            Dialog_ShowCallback(playerid, using public DIALOG_BARS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 6: // Houses
        {
            strcat(stringcat, "{00EEFE}[12] {FFFFFF}OG Loc's House \n");
            strcat(stringcat, "{00EEFE}[43] {FFFFFF}Ryder's House \n");
            strcat(stringcat, "{00EEFE}[44] {FFFFFF}Sweet's House \n");
            strcat(stringcat, "{00EEFE}[46] {FFFFFF}Johnson's House \n");
            strcat(stringcat, "{00EEFE}[51] {FFFFFF}Angel Pine Trailer \n");
            strcat(stringcat, "{00EEFE}[55] {FFFFFF}Mad Dog's Mansion \n");
            strcat(stringcat, "{00EEFE}[59] {FFFFFF}Abandoned Airport Tower \n");
            strcat(stringcat, "{00EEFE}[67] {FFFFFF}Safehouse 1 \n");
            strcat(stringcat, "{00EEFE}[68] {FFFFFF}Safehouse 2 \n");
            strcat(stringcat, "{00EEFE}[69] {FFFFFF}Safehouse 3 \n");
            strcat(stringcat, "{00EEFE}[70] {FFFFFF}Safehouse 4 \n");
            strcat(stringcat, "{00EEFE}[76] {FFFFFF}House 1 \n");
            strcat(stringcat, "{00EEFE}[77] {FFFFFF}House 2 \n");
            strcat(stringcat, "{00EEFE}[78] {FFFFFF}House 3 \n");
            strcat(stringcat, "{00EEFE}[79] {FFFFFF}House 4 \n");
            strcat(stringcat, "{00EEFE}[81] {FFFFFF}House 5 \n");
            strcat(stringcat, "{00EEFE}[92] {FFFFFF}House 6 \n");
            strcat(stringcat, "{00EEFE}[93] {FFFFFF}House 7 \n");
            strcat(stringcat, "{00EEFE}[94] {FFFFFF}House 8 \n");
            strcat(stringcat, "{00EEFE}[95] {FFFFFF}House 9 \n");
            strcat(stringcat, "{00EEFE}[101] {FFFFFF}House 10 \n");
            strcat(stringcat, "{00EEFE}[105] {FFFFFF}House 11 \n");
            strcat(stringcat, "{00EEFE}[106] {FFFFFF}House 12 \n");
            strcat(stringcat, "{00EEFE}[113] {FFFFFF}House 13 \n");
            strcat(stringcat, "{00EEFE}[114] {FFFFFF}House 14 \n");
            strcat(stringcat, "{00EEFE}[115] {FFFFFF}House 15 \n");
            strcat(stringcat, "{00EEFE}[119] {FFFFFF}House 16 \n");
            strcat(stringcat, "{00EEFE}[120] {FFFFFF}House 17 \n");
            strcat(stringcat, "{00EEFE}[124] {FFFFFF}House 18 \n");
            strcat(stringcat, "{00EEFE}[129] {FFFFFF}House 19 \n");
            strcat(stringcat, "{00EEFE}[131] {FFFFFF}House 20 \n");
            strcat(stringcat, "{00EEFE}[135] {FFFFFF}House 21 \n");
            strcat(stringcat, "{00EEFE}[140] {FFFFFF}House 22 \n");
            strcat(stringcat, "{00EEFE}[141] {FFFFFF}House 23 \n");
            strcat(stringcat, "{00EEFE}[142] {FFFFFF}House 24 \n");
            strcat(stringcat, "{00EEFE}[143] {FFFFFF}House 25 \n");
            Dialog_ShowCallback(playerid, using public DIALOG_HOUSES<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 7: // Nightclubs
        {
            strcat(stringcat, "{00EEFE}[02] {FFFFFF}Brothel 1 \n");
            strcat(stringcat, "{00EEFE}[03] {FFFFFF}Brothel 2 \n");
            strcat(stringcat, "{00EEFE}[06] {FFFFFF}The Big Spread Ranch \n");
            strcat(stringcat, "{00EEFE}[40] {FFFFFF}The Pig Pen (Strip Club) \n");
            strcat(stringcat, "{00EEFE}[54] {FFFFFF}The Pleasure Domes \n");
            strcat(stringcat, "{00EEFE}[64] {FFFFFF}Club \n");
            strcat(stringcat, "{00EEFE}[128] {FFFFFF}Brothel 3 \n");
            Dialog_ShowCallback(playerid, using public DIALOG_NIGHTCLUBS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 8: // Casinos
        {
            strcat(stringcat, "{00EEFE}[41] {FFFFFF}Four Dragons \n");
            strcat(stringcat, "{00EEFE}[48] {FFFFFF}Caligulas \n");
            strcat(stringcat, "{00EEFE}[62] {FFFFFF}Redsands West \n");
            strcat(stringcat, "{00EEFE}[132] {FFFFFF}Caligulas Roof \n");
            strcat(stringcat, "{00EEFE}[133] {FFFFFF}Old Venturas Strip \n");
            Dialog_ShowCallback(playerid, using public DIALOG_CASINOS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 9: // Car Dealerships
        {
            strcat(stringcat, "{00EEFE}[144] {FFFFFF}Otto's Autos \n");
            strcat(stringcat, "{00EEFE}[145] {FFFFFF}Wang Cars \n");
            Dialog_ShowCallback(playerid, using public DIALOG_DEALERSHIPS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 10: // Police Stations
        {
            strcat(stringcat, "{00EEFE}[15] {FFFFFF}Las Venturas Police Station \n");
            strcat(stringcat, "{00EEFE}[27] {FFFFFF}San Fierro Police Station \n");
            strcat(stringcat, "{00EEFE}[111] {FFFFFF}Los Santos Police Station \n");
            Dialog_ShowCallback(playerid, using public DIALOG_POLICE<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 11: // Schools
        {
            strcat(stringcat, "{00EEFE}[116] {FFFFFF}School 1 \n");
            strcat(stringcat, "{00EEFE}[134] {FFFFFF}School 2 \n");
            strcat(stringcat, "{00EEFE}[136] {FFFFFF}School 3 \n");
            Dialog_ShowCallback(playerid, using public DIALOG_SCHOOLS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 12: // Offices
        {
            strcat(stringcat, "{00EEFE}[127] {FFFFFF}Rosenberg's Office \n");
            strcat(stringcat, "{00EEFE}[138] {FFFFFF}Janitor's Office \n");
            Dialog_ShowCallback(playerid, using public DIALOG_OFFICES<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 13: // Stadiums/Studios
        {
            strcat(stringcat, "{00EEFE}[38] {FFFFFF}Studio 1 \n");
            strcat(stringcat, "{00EEFE}[39] {FFFFFF}Studio 2 \n");
            strcat(stringcat, "{00EEFE}[63] {FFFFFF}Studio 3 \n");
            strcat(stringcat, "{00EEFE}[82] {FFFFFF}Studio 4 \n");
            strcat(stringcat, "{00EEFE}[109] {FFFFFF}Studio 5 \n");
            strcat(stringcat, "{00EEFE}[118] {FFFFFF}Studio 6 \n");
            Dialog_ShowCallback(playerid, using public DIALOG_STADIUMS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 14: // Factories
        {
            strcat(stringcat, "{00EEFE}[56] {FFFFFF}Big Smoke's Crack Factory \n");
            strcat(stringcat, "{00EEFE}[71] {FFFFFF}Power Plant \n");
            strcat(stringcat, "{00EEFE}[102] {FFFFFF}Sindacco Abatoir \n");
            Dialog_ShowCallback(playerid, using public DIALOG_FACTORIES<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 15: // Hotels
        {
            strcat(stringcat, "{00EEFE}[73] {FFFFFF}Jefferson Motel \n");
            Dialog_ShowCallback(playerid, using public DIALOG_HOTELS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 16: // Stores
        {
            strcat(stringcat, "{00EEFE}[13] {FFFFFF}Barber \n");
            strcat(stringcat, "{00EEFE}[16] {FFFFFF}Pro-Laps \n");
            strcat(stringcat, "{00EEFE}[17] {FFFFFF}Sex Shop \n");
            strcat(stringcat, "{00EEFE}[18] {FFFFFF}Las Venturas Tattoo Studio \n");
            strcat(stringcat, "{00EEFE}[19] {FFFFFF}San Fierro Tattoo Studio \n");
            strcat(stringcat, "{00EEFE}[20] {FFFFFF}24/7 \n");
            strcat(stringcat, "{00EEFE}[24] {FFFFFF}Ammu-nation \n");
            strcat(stringcat, "{00EEFE}[25] {FFFFFF}Victim \n");
            strcat(stringcat, "{00EEFE}[28] {FFFFFF}24/7 - 2 \n");
            strcat(stringcat, "{00EEFE}[32] {FFFFFF}Ammu-nation - 2 \n");
            strcat(stringcat, "{00EEFE}[33] {FFFFFF}SubUrban \n");
            strcat(stringcat, "{00EEFE}[50] {FFFFFF}Barber - 2 \n");
            strcat(stringcat, "{00EEFE}[52] {FFFFFF}24/7 - 3 \n");
            strcat(stringcat, "{00EEFE}[53] {FFFFFF}ZIP \n");
            strcat(stringcat, "{00EEFE}[61] {FFFFFF}Didier Sachs \n");
            strcat(stringcat, "{00EEFE}[66] {FFFFFF}Los Santos Tattoo Studio \n");
            strcat(stringcat, "{00EEFE}[72] {FFFFFF}24/7 - 4 \n");
            strcat(stringcat, "{00EEFE}[80] {FFFFFF}Binco \n");
            strcat(stringcat, "{00EEFE}[86] {FFFFFF}Dillimore Gas Station \n");
            strcat(stringcat, "{00EEFE}[88] {FFFFFF}Torreno's Ranch \n");
            strcat(stringcat, "{00EEFE}[91] {FFFFFF}Blueberry Liquor Store \n");
            strcat(stringcat, "{00EEFE}[96] {FFFFFF}Ammu-nation - 3 \n");
            strcat(stringcat, "{00EEFE}[98] {FFFFFF}24/7 - 5 \n");
            strcat(stringcat, "{00EEFE}[108] {FFFFFF}Barber - 3 \n");
            strcat(stringcat, "{00EEFE}[121] {FFFFFF}Toy Store \n");
            strcat(stringcat, "{00EEFE}[122] {FFFFFF}Ammu-nation - 4 \n");
            strcat(stringcat, "{00EEFE}[123] {FFFFFF}Ammu-nation 5 \n");
            strcat(stringcat, "{00EEFE}[125] {FFFFFF}24/7 - 6 \n");
            Dialog_ShowCallback(playerid, using public DIALOG_STORES<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 17: // Workshops
        {
            strcat(stringcat, "{00EEFE}[11] {FFFFFF}Wheel Arch Angels \n");
            strcat(stringcat, "{00EEFE}[26] {FFFFFF}Loco Low Co \n");
            strcat(stringcat, "{00EEFE}[30] {FFFFFF}Transfender \n");
            strcat(stringcat, "{00EEFE}[37] {FFFFFF}San Fierro Garage \n");
            Dialog_ShowCallback(playerid, using public DIALOG_WORKSHOPS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 18: // Bedrooms
        {
            strcat(stringcat, "{00EEFE}[34] {FFFFFF}Denise's Bedroom \n");
            strcat(stringcat, "{00EEFE}[49] {FFFFFF}Katie's Love Nest \n");
            strcat(stringcat, "{00EEFE}[83] {FFFFFF}Motel Room \n");
            strcat(stringcat, "{00EEFE}[100] {FFFFFF}Michelle's Bedroom \n");
            strcat(stringcat, "{00EEFE}[107] {FFFFFF}Millie's Bedroom \n");
            Dialog_ShowCallback(playerid, using public DIALOG_BEDROOMS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
        case 19: // Others
        {
            strcat(stringcat, "{00EEFE}[04] {FFFFFF}Inside Track Betting \n");
            strcat(stringcat, "{00EEFE}[05] {FFFFFF}Blastin Fools Recording \n");
            strcat(stringcat, "{00EEFE}[14] {FFFFFF}Planning Department \n");
            strcat(stringcat, "{00EEFE}[35] {FFFFFF}Helena's Barn \n");
            strcat(stringcat, "{00EEFE}[36] {FFFFFF}Barbara's Love Nest (Cells) \n");
            strcat(stringcat, "{00EEFE}[42] {FFFFFF}Liberty City \n");
            strcat(stringcat, "{00EEFE}[45] {FFFFFF}RC Battlefield \n");
            strcat(stringcat, "{00EEFE}[57] {FFFFFF}Burning Desire Building \n");
            strcat(stringcat, "{00EEFE}[58] {FFFFFF}Wu-Zi Mu's \n");
            strcat(stringcat, "{00EEFE}[60] {FFFFFF}Wardrobe Room \n");
            strcat(stringcat, "{00EEFE}[65] {FFFFFF}Atrium \n");
            strcat(stringcat, "{00EEFE}[84] {FFFFFF}Bank \n");
            strcat(stringcat, "{00EEFE}[89] {FFFFFF}Zombotech \n");
            strcat(stringcat, "{00EEFE}[90] {FFFFFF}Cemetery \n");
            strcat(stringcat, "{00EEFE}[104] {FFFFFF}Area 69 \n");
            Dialog_ShowCallback(playerid, using public DIALOG_OTHERS<iiiis>, DIALOG_STYLE_LIST, "Select an interior...", stringcat, "Select", "Back");
        }
    }
    return 1;
}

forward DIALOG_GYMS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_GYMS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 1);
        case 1: OnEnterInterior(playerid, 29);
        case 2: OnEnterInterior(playerid, 110);
    }
    return 1;
}

forward DIALOG_AIRPORTS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_AIRPORTS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 112);
        case 1: OnEnterInterior(playerid, 117);
    }
    return 1;
}

forward DIALOG_APARTMENTS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_APARTMENTS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 9);
        case 1: OnEnterInterior(playerid, 10);
    }
    return 1;
}

forward DIALOG_WAREHOUSES(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_WAREHOUSES(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 7);
        case 1: OnEnterInterior(playerid, 8);
        case 2: OnEnterInterior(playerid, 99);
        case 3: OnEnterInterior(playerid, 103);
    }
    return 1;
}

forward DIALOG_AIRCRAFTS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_AIRCRAFTS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 74);
        case 1: OnEnterInterior(playerid, 137);
    }
    return 1;
}

forward DIALOG_BARS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_BARS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 21);
        case 1: OnEnterInterior(playerid, 22);
        case 2: OnEnterInterior(playerid, 23);
        case 3: OnEnterInterior(playerid, 31);
        case 4: OnEnterInterior(playerid, 47);
        case 5: OnEnterInterior(playerid, 75);
        case 6: OnEnterInterior(playerid, 85);
        case 7: OnEnterInterior(playerid, 87);
        case 8: OnEnterInterior(playerid, 97);
        case 9: OnEnterInterior(playerid, 126);
        case 10: OnEnterInterior(playerid, 130);
        case 11: OnEnterInterior(playerid, 139);
    }
    return 1;
}

forward DIALOG_HOUSES(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HOUSES(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 12);
        case 1: OnEnterInterior(playerid, 43);
        case 2: OnEnterInterior(playerid, 44);
        case 3: OnEnterInterior(playerid, 46);
        case 4: OnEnterInterior(playerid, 51);
        case 5: OnEnterInterior(playerid, 55);
        case 6: OnEnterInterior(playerid, 59);
        case 7: OnEnterInterior(playerid, 67);
        case 8: OnEnterInterior(playerid, 68);
        case 9: OnEnterInterior(playerid, 69);
        case 10: OnEnterInterior(playerid, 70);
        case 11: OnEnterInterior(playerid, 76);
        case 12: OnEnterInterior(playerid, 77);
        case 13: OnEnterInterior(playerid, 78);
        case 14: OnEnterInterior(playerid, 79);
        case 15: OnEnterInterior(playerid, 81);
        case 16: OnEnterInterior(playerid, 92);
        case 17: OnEnterInterior(playerid, 93);
        case 18: OnEnterInterior(playerid, 94);
        case 19: OnEnterInterior(playerid, 95);
        case 20: OnEnterInterior(playerid, 101);
        case 21: OnEnterInterior(playerid, 105);
        case 22: OnEnterInterior(playerid, 106);
        case 23: OnEnterInterior(playerid, 113);
        case 24: OnEnterInterior(playerid, 114);
        case 25: OnEnterInterior(playerid, 115);
        case 26: OnEnterInterior(playerid, 119);
        case 27: OnEnterInterior(playerid, 120);
        case 28: OnEnterInterior(playerid, 121);
        case 29: OnEnterInterior(playerid, 124);
        case 30: OnEnterInterior(playerid, 131);
        case 31: OnEnterInterior(playerid, 135);
        case 32: OnEnterInterior(playerid, 140);
        case 33: OnEnterInterior(playerid, 141);
        case 34: OnEnterInterior(playerid, 142);
        case 35: OnEnterInterior(playerid, 143);
    }
    return 1;
}

forward DIALOG_NIGHTCLUBS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_NIGHTCLUBS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 2);
        case 1: OnEnterInterior(playerid, 3);
        case 2: OnEnterInterior(playerid, 6);
        case 3: OnEnterInterior(playerid, 40);
        case 4: OnEnterInterior(playerid, 54);
        case 5: OnEnterInterior(playerid, 64);
        case 6: OnEnterInterior(playerid, 128);
    }
    return 1;
}

forward DIALOG_CASINOS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_CASINOS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 41);
        case 1: OnEnterInterior(playerid, 48);
        case 2: OnEnterInterior(playerid, 62);
        case 3: OnEnterInterior(playerid, 132);
        case 4: OnEnterInterior(playerid, 133);
    }
    return 1;
}

forward DIALOG_DEALERSHIPS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_DEALERSHIPS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 144);
        case 1: OnEnterInterior(playerid, 145);
    }
    return 1;
}

forward DIALOG_POLICE(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_POLICE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 15);
        case 1: OnEnterInterior(playerid, 27);
        case 2: OnEnterInterior(playerid, 111);
    }
    return 1;
}

forward DIALOG_SCHOOLS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_SCHOOLS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 116);
        case 1: OnEnterInterior(playerid, 134);
        case 2: OnEnterInterior(playerid, 136);
    }
    return 1;
}

forward DIALOG_OFFICES(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_OFFICES(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 127);
        case 1: OnEnterInterior(playerid, 138);
    }
    return 1;
}

forward DIALOG_STADIUMS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_STADIUMS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 38);
        case 1: OnEnterInterior(playerid, 39);
        case 2: OnEnterInterior(playerid, 63);
        case 3: OnEnterInterior(playerid, 82);
        case 4: OnEnterInterior(playerid, 109);
        case 5: OnEnterInterior(playerid, 118);
    }
    return 1;
}

forward DIALOG_FACTORIES(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_FACTORIES(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 56);
        case 1: OnEnterInterior(playerid, 71);
        case 2: OnEnterInterior(playerid, 102);
    }
    return 1;
}

forward DIALOG_HOTELS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HOTELS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 73);
    }
    return 1;
}

forward DIALOG_STORES(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_STORES(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 13);
        case 1: OnEnterInterior(playerid, 16);
        case 2: OnEnterInterior(playerid, 17);
        case 3: OnEnterInterior(playerid, 18);
        case 4: OnEnterInterior(playerid, 19);
        case 5: OnEnterInterior(playerid, 20);
        case 6: OnEnterInterior(playerid, 24);
        case 7: OnEnterInterior(playerid, 25);
        case 8: OnEnterInterior(playerid, 28);
        case 9: OnEnterInterior(playerid, 32);
        case 10: OnEnterInterior(playerid, 33);
        case 11: OnEnterInterior(playerid, 50);
        case 12: OnEnterInterior(playerid, 52);
        case 13: OnEnterInterior(playerid, 53);
        case 14: OnEnterInterior(playerid, 61);
        case 15: OnEnterInterior(playerid, 66);
        case 16: OnEnterInterior(playerid, 72);
        case 17: OnEnterInterior(playerid, 80);
        case 18: OnEnterInterior(playerid, 86);
        case 19: OnEnterInterior(playerid, 88);
        case 20: OnEnterInterior(playerid, 91);
        case 21: OnEnterInterior(playerid, 96);
        case 22: OnEnterInterior(playerid, 98);
        case 23: OnEnterInterior(playerid, 108);
        case 24: OnEnterInterior(playerid, 121);
        case 25: OnEnterInterior(playerid, 122);
        case 26: OnEnterInterior(playerid, 123);
        case 27: OnEnterInterior(playerid, 125);
    }
    return 1;
}

forward DIALOG_WORKSHOPS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_WORKSHOPS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 11);
        case 1: OnEnterInterior(playerid, 26);
        case 2: OnEnterInterior(playerid, 30);
        case 3: OnEnterInterior(playerid, 37);
    }
    return 1;
}

forward DIALOG_BEDROOMS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_BEDROOMS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 34);
        case 1: OnEnterInterior(playerid, 49);
        case 2: OnEnterInterior(playerid, 83);
        case 3: OnEnterInterior(playerid, 100);
        case 4: OnEnterInterior(playerid, 107);
    }
    return 1;
}

forward DIALOG_OTHERS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_OTHERS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowInteriorsMenu(playerid);
    switch(listitem)
    {
        case 0: OnEnterInterior(playerid, 4);
        case 1: OnEnterInterior(playerid, 5);
        case 2: OnEnterInterior(playerid, 14);
        case 3: OnEnterInterior(playerid, 35);
        case 4: OnEnterInterior(playerid, 36);
        case 5: OnEnterInterior(playerid, 42);
        case 6: OnEnterInterior(playerid, 45);
        case 7: OnEnterInterior(playerid, 57);
        case 8: OnEnterInterior(playerid, 58);
        case 9: OnEnterInterior(playerid, 60);
        case 10: OnEnterInterior(playerid, 65);
        case 11: OnEnterInterior(playerid, 84);
        case 12: OnEnterInterior(playerid, 89);
        case 13: OnEnterInterior(playerid, 90);
        case 14: OnEnterInterior(playerid, 104);
    }
    return 1;
}

stock ShowInteriorsMenu(playerid)
{
    new stringcat[1000];
    strcat(stringcat, "Gyms \n");
    strcat(stringcat, "Airports \n");
    strcat(stringcat, "Apartments \n");
    strcat(stringcat, "Warehouses \n");
    strcat(stringcat, "Aircrafts \n");
    strcat(stringcat, "Bars and Restaurants \n");
    strcat(stringcat, "Houses \n");
    strcat(stringcat, "Nightclubs \n");
    strcat(stringcat, "Casinos \n");
    strcat(stringcat, "Car Dealerships \n");
    strcat(stringcat, "Police Stations \n");
    strcat(stringcat, "Schools \n");
    strcat(stringcat, "Offices \n");
    strcat(stringcat, "Stadiums/Studios \n");
    strcat(stringcat, "Factories \n");
    strcat(stringcat, "Hotels/Motels \n");
    strcat(stringcat, "Stores \n");
    strcat(stringcat, "Workshops \n");
    strcat(stringcat, "Bedrooms \n");
    strcat(stringcat, "Others... \n");
    Dialog_ShowCallback(playerid, using public DIALOG_INTERIORES<iiiis>, DIALOG_STYLE_LIST, "Select a category...", stringcat, "Select", "Cancel");
    return 1;
}

stock OnEnterInterior(playerid, IntID)
{
    User[playerid][int_InteriorSelecionado] = IntID;
    switch(IntID)
    {
        case 1: { SetPlayerPos(playerid, 770.8033, -0.7033, 1000.7267); SetPlayerFacingAngle(playerid, 22.8599); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[01] {FFFFFF}Ganton Gym"); }
        case 2: { SetPlayerPos(playerid, 974.0177, -9.5937, 1001.1484); SetPlayerFacingAngle(playerid, 22.6045); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[02] {FFFFFF}Brothel 1"); }
        case 3: { SetPlayerPos(playerid, 961.9308, -51.9071, 1001.1172); SetPlayerFacingAngle(playerid, 95.5381); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[03] {FFFFFF}Brothel 2"); }
        case 4: { SetPlayerPos(playerid, 830.6016, 5.9404, 1004.1797); SetPlayerFacingAngle(playerid, 125.8149); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[04] {FFFFFF}Inside Track Betting"); }
        case 5: { SetPlayerPos(playerid, 1037.8276, 0.397, 1001.2845); SetPlayerFacingAngle(playerid, 353.9335); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[05] {FFFFFF}Blastin Fools Recording Studio"); }
        case 6: { SetPlayerPos(playerid, 1212.1489, -28.5388, 1000.9531); SetPlayerFacingAngle(playerid, 170.5692); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[06] {FFFFFF}The Big Spread Ranch"); }
        case 7: { SetPlayerPos(playerid, 1290.4106, 1.9512, 1001.0201); SetPlayerFacingAngle(playerid, 179.9419); SetPlayerInterior(playerid, 18); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[07] {FFFFFF}Warehouse 1"); }
        case 8: { SetPlayerPos(playerid, 1412.1472, -2.2836, 1000.9241); SetPlayerFacingAngle(playerid, 114.661); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[08] {FFFFFF}Warehouse 2"); }
        case 9: { SetPlayerPos(playerid, 1527.0468, -12.0236, 1002.0971); SetPlayerFacingAngle(playerid, 350.0013); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[09] {FFFFFF}B Dup Apartment 1"); }
        case 10: { SetPlayerPos(playerid, 1523.5098, -47.8211, 1002.2699); SetPlayerFacingAngle(playerid, 262.7038); SetPlayerInterior(playerid, 2); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[10] {FFFFFF}B Dup Apartment 2"); }
        case 11: { SetPlayerPos(playerid, 612.2191, -123.9028, 997.9922); SetPlayerFacingAngle(playerid, 266.5704); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[11] {FFFFFF}Wheel Arch Angels"); }
        case 12: { SetPlayerPos(playerid, 512.9291, -11.6929, 1001.5653); SetPlayerFacingAngle(playerid, 198.7669); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[12] {FFFFFF}OG Loc's House"); }
        case 13: { SetPlayerPos(playerid, 418.4666, -80.4595, 1001.8047); SetPlayerFacingAngle(playerid, 343.2358); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[13] {FFFFFF}Barber"); }
        case 14: { SetPlayerPos(playerid, 386.5259, 173.6381, 1008.3828); SetPlayerFacingAngle(playerid, 63.7399); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[14] {FFFFFF}Planning Department"); }
        case 15: { SetPlayerPos(playerid, 288.4723, 170.0647, 1007.1794); SetPlayerFacingAngle(playerid, 22.0477); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[15] {FFFFFF}Las Venturas Police Station"); }
        case 16: { SetPlayerPos(playerid, 206.4627, -137.7076, 1003.0938); SetPlayerFacingAngle(playerid, 10.9347); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[16] {FFFFFF}Pro-Laps"); }
        case 17: { SetPlayerPos(playerid, -100.2674, -22.9376, 1000.7188); SetPlayerFacingAngle(playerid, 17.285); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[17] {FFFFFF}Sex Shop"); }
        case 18: { SetPlayerPos(playerid, -201.2236, -43.2465, 1002.2734); SetPlayerFacingAngle(playerid, 45.8613); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[18] {FFFFFF}Las Venturas Tattoo Studio"); }
        case 19: { SetPlayerPos(playerid, -202.9381, -6.7006, 1002.2734); SetPlayerFacingAngle(playerid, 204.2693); SetPlayerInterior(playerid, 17); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[19] {FFFFFF}San Fierro Tattoo Studio"); }
        case 20: { SetPlayerPos(playerid, -25.7220, -187.8216, 1003.5469); SetPlayerFacingAngle(playerid, 5.0760); SetPlayerInterior(playerid, 17); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[20] {FFFFFF}24/7"); }
        case 21: { SetPlayerPos(playerid, 454.9853, -107.2548, 999.4376); SetPlayerFacingAngle(playerid, 309.0195); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[21] {FFFFFF}Restaurant 1"); }
        case 22: { SetPlayerPos(playerid, 372.5565, -131.3607, 1001.4922); SetPlayerFacingAngle(playerid, 354.2285); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[22] {FFFFFF}Pizza Stack"); }
        case 23: { SetPlayerPos(playerid, 378.026, -190.5155, 1000.6328); SetPlayerFacingAngle(playerid, 141.0245); SetPlayerInterior(playerid, 17); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[23] {FFFFFF}Rusty Brown's Donuts"); }
        case 24: { SetPlayerPos(playerid, 315.244, -140.8858, 999.6016); SetPlayerFacingAngle(playerid, 7.4226); SetPlayerInterior(playerid, 7); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[24] {FFFFFF}Ammu-nation"); }
        case 25: { SetPlayerPos(playerid, 225.0306, -9.1838, 1002.218); SetPlayerFacingAngle(playerid, 85.5322); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[25] {FFFFFF}Victim"); }
        case 26: { SetPlayerPos(playerid, 611.3536, -77.5574, 997.9995); SetPlayerFacingAngle(playerid, 320.9263); SetPlayerInterior(playerid, 2); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[26] {FFFFFF}Loco Low Co"); }
        case 27: { SetPlayerPos(playerid, 246.0688, 108.9703, 1003.2188); SetPlayerFacingAngle(playerid, 0.2922); SetPlayerInterior(playerid, 10); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[27] {FFFFFF}San Fierro Police Station"); }
        case 28: { SetPlayerPos(playerid, 6.0856, -28.8966, 1003.5494); SetPlayerFacingAngle(playerid, 5.0365); SetPlayerInterior(playerid, 10); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[28] {FFFFFF}24/7 - 2"); }
        case 29: { SetPlayerPos(playerid, 773.7318, -74.6957, 1000.6542); SetPlayerFacingAngle(playerid, 5.2304); SetPlayerInterior(playerid, 7); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[29] {FFFFFF}Below The Belt Gym (Las Venturas)"); }
        case 30: { SetPlayerPos(playerid, 621.4528, -23.7289, 1000.9219); SetPlayerFacingAngle(playerid, 15.6789); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[30] {FFFFFF}Transfender"); }
        case 31: { SetPlayerPos(playerid, 445.6003, -6.9823, 1000.7344); SetPlayerFacingAngle(playerid, 172.2105); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[31] {FFFFFF}World of Coq"); }
        case 32: { SetPlayerPos(playerid, 285.8361, -39.0166, 1001.5156); SetPlayerFacingAngle(playerid, 0.7529); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[32] {FFFFFF}Ammu-nation - 2"); }
        case 33: { SetPlayerPos(playerid, 204.1174, -46.8047, 1001.8047); SetPlayerFacingAngle(playerid, 357.5777); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[33] {FFFFFF}SubUrban"); }
        case 34: { SetPlayerPos(playerid, 245.2307, 304.7632, 999.1484); SetPlayerFacingAngle(playerid, 273.4364); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[34] {FFFFFF}Denise's Bedroom"); }
        case 35: { SetPlayerPos(playerid, 290.623, 309.0622, 999.1484); SetPlayerFacingAngle(playerid, 89.9164); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[35] {FFFFFF}Helena's Barn"); }
        case 36: { SetPlayerPos(playerid, 322.5014, 303.6906, 999.1484); SetPlayerFacingAngle(playerid, 8.1747); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[36] {FFFFFF}Barbara's Love Nest (Cells)"); }
        case 37: { SetPlayerPos(playerid, -2041.2334, 178.3969, 28.8465); SetPlayerFacingAngle(playerid, 156.2153); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[37] {FFFFFF}San Fierro Garage"); }
        case 38: { SetPlayerPos(playerid, -1402.6613, 106.3897, 1032.2734); SetPlayerFacingAngle(playerid, 105.1356); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[38] {FFFFFF}Studio 1"); }
        case 39: { SetPlayerPos(playerid, -1403.0116, -250.4526, 1043.5341); SetPlayerFacingAngle(playerid, 355.8576); SetPlayerInterior(playerid, 7); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[39] {FFFFFF}Studio 2"); }
        case 40: { SetPlayerPos(playerid, 1204.6689, -13.5429, 1000.9219); SetPlayerFacingAngle(playerid, 350.0204); SetPlayerInterior(playerid, 2); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[40] {FFFFFF}The Pig Pen (Strip Club)"); }
        case 41: { SetPlayerPos(playerid, 2016.1156, 1017.1541, 996.875); SetPlayerFacingAngle(playerid, 88.0055); SetPlayerInterior(playerid, 10); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[41] {FFFFFF}Four Dragons Casino"); }
        case 42: { SetPlayerPos(playerid, -741.8495, 493.0036, 1371.9766); SetPlayerFacingAngle(playerid, 71.7782); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[42] {FFFFFF}Liberty City"); }
        case 43: { SetPlayerPos(playerid, 2447.8704, -1704.4509, 1013.5078); SetPlayerFacingAngle(playerid, 314.5253); SetPlayerInterior(playerid, 2); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[43] {FFFFFF}Ryder's House"); }
        case 44: { SetPlayerPos(playerid, 2527.0176, -1679.2076, 1015.4986); SetPlayerFacingAngle(playerid, 260.9709); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[44] {FFFFFF}Sweet's House"); }
        case 45: { SetPlayerPos(playerid, -1129.8909, 1057.5424, 1346.4141); SetPlayerFacingAngle(playerid, 274.5268); SetPlayerInterior(playerid, 10); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[45] {FFFFFF}RC Battlefield"); }
        case 46: { SetPlayerPos(playerid, 2496.0549, -1695.1749, 1014.7422); SetPlayerFacingAngle(playerid, 179.2174); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[46] {FFFFFF}Johnson's House"); }
        case 47: { SetPlayerPos(playerid, 366.0248, -73.3478, 1001.5078); SetPlayerFacingAngle(playerid, 292.0084); SetPlayerInterior(playerid, 10); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[47] {FFFFFF}Burger Shot"); }
        case 48: { SetPlayerPos(playerid, 2233.9363, 1711.8038, 1011.6312); SetPlayerFacingAngle(playerid, 184.3891); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[48] {FFFFFF}Caligulas Casino"); }
        case 49: { SetPlayerPos(playerid, 269.6405, 305.9512, 999.1484); SetPlayerFacingAngle(playerid, 215.6625); SetPlayerInterior(playerid, 2); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[49] {FFFFFF}Katie's Love Nest"); }
        case 50: { SetPlayerPos(playerid, 414.2987, -18.8044, 1001.8047); SetPlayerFacingAngle(playerid, 41.4265); SetPlayerInterior(playerid, 2); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[50] {FFFFFF}Barber 2"); }
        case 51: { SetPlayerPos(playerid, 1.1853, -3.2387, 999.4284); SetPlayerFacingAngle(playerid, 87.5718); SetPlayerInterior(playerid, 2); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[51] {FFFFFF}Angel Pine Trailer"); }
        case 52: { SetPlayerPos(playerid, -30.9875, -89.6806, 1003.5469); SetPlayerFacingAngle(playerid, 359.8401); SetPlayerInterior(playerid, 18); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[52] {FFFFFF}24/7 - 3"); }
        case 53: { SetPlayerPos(playerid, 161.4048, -94.2416, 1001.8047); SetPlayerFacingAngle(playerid, 0.7938); SetPlayerInterior(playerid, 18); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[53] {FFFFFF}ZIP Store"); }
        case 54: { SetPlayerPos(playerid, -2638.8232, 1407.3395, 906.4609); SetPlayerFacingAngle(playerid, 94.6794); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[54] {FFFFFF}The Pleasure Domes"); }
        case 55: { SetPlayerPos(playerid, 1267.8407, -776.9587, 1091.9063); SetPlayerFacingAngle(playerid, 231.3418); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[55] {FFFFFF}Mad Dog's Mansion"); }
        case 56: { SetPlayerPos(playerid, 2536.5322, -1294.8425, 1044.125); SetPlayerFacingAngle(playerid, 254.9548); SetPlayerInterior(playerid, 2); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[56] {FFFFFF}Big Smoke's Crack Factory"); }
        case 57: { SetPlayerPos(playerid, 2350.1597, -1181.0658, 1027.9766); SetPlayerFacingAngle(playerid, 99.1864); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[57] {FFFFFF}Burning Desire Building"); }
        case 58: { SetPlayerPos(playerid, -2158.6731, 642.09, 1052.375); SetPlayerFacingAngle(playerid, 86.5402); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[58] {FFFFFF}Wu-Zi Mu's House"); }
        case 59: { SetPlayerPos(playerid, 419.8936, 2537.1155, 10.00); SetPlayerFacingAngle(playerid, 67.6537); SetPlayerInterior(playerid, 10); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[59] {FFFFFF}Abandoned Airport Tower"); }
        case 60: { SetPlayerPos(playerid, 256.9047, -41.6537, 1002.0234); SetPlayerFacingAngle(playerid, 85.8774); SetPlayerInterior(playerid, 14); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[60] {FFFFFF}Wardrobe Room"); }
        case 61: { SetPlayerPos(playerid, 204.1658, -165.7678, 1000.5234); SetPlayerFacingAngle(playerid, 181.7583); SetPlayerInterior(playerid, 14); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[61] {FFFFFF}Didier Sachs Store"); }
        case 62: { SetPlayerPos(playerid, 1133.35, -7.8462, 1000.6797); SetPlayerFacingAngle(playerid, 165.8482); SetPlayerInterior(playerid, 12); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[62] {FFFFFF}Redsands West Casino"); }
        case 63: { SetPlayerPos(playerid, -1420.4277, 1616.9221, 1052.5313); SetPlayerFacingAngle(playerid, 159.1255); SetPlayerInterior(playerid, 14); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[63] {FFFFFF}Studio 3"); }
        case 64: { SetPlayerPos(playerid, 493.1443, -24.2607, 1000.6797); SetPlayerFacingAngle(playerid, 356.9864); SetPlayerInterior(playerid, 17); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[64] {FFFFFF}Club"); }
        case 65: { SetPlayerPos(playerid, 1727.2853, -1642.9451, 20.2254); SetPlayerFacingAngle(playerid, 172.4193); SetPlayerInterior(playerid, 18); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[65] {FFFFFF}Atrium"); }
        case 66: { SetPlayerPos(playerid, -202.842, -24.0325, 1002.2734); SetPlayerFacingAngle(playerid, 252.8154); SetPlayerInterior(playerid, 16); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[66] {FFFFFF}Los Santos Tattoo Studio"); }
        case 67: { SetPlayerPos(playerid, 2233.6919, -1112.8107, 1050.8828); SetPlayerFacingAngle(playerid, 8.6483); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[67] {FFFFFF}Safehouse 1"); }
        case 68: { SetPlayerPos(playerid, 1211.2484, 1049.0234, 359.941); SetPlayerFacingAngle(playerid, 170.9341); SetPlayerInterior(playerid, 6); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[68] {FFFFFF}Safehouse 2"); }
        case 69: { SetPlayerPos(playerid, 2319.1272, -1023.9562, 1050.2109); SetPlayerFacingAngle(playerid, 167.3959); SetPlayerInterior(playerid, 9); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[69] {FFFFFF}Safehouse 3"); }
        case 70: { SetPlayerPos(playerid, 2261.0977, -1137.8833, 1050.6328); SetPlayerFacingAngle(playerid, 266.88); SetPlayerInterior(playerid, 10); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[70] {FFFFFF}Safehouse 4"); }
        case 71: { SetPlayerPos(playerid, -944.2402, 1886.1536, 5.0051); SetPlayerFacingAngle(playerid, 179.8548); SetPlayerInterior(playerid, 17); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[71] {FFFFFF}Power Plant"); }
        case 72: { SetPlayerPos(playerid, -26.1856, -140.9164, 1003.5469); SetPlayerFacingAngle(playerid, 2.9087); SetPlayerInterior(playerid, 16); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[72] {FFFFFF}24/7 - 4"); }
        case 73: { SetPlayerPos(playerid, 2217.281, -1150.5349, 1025.7969); SetPlayerFacingAngle(playerid, 273.7328); SetPlayerInterior(playerid, 15); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[73] {FFFFFF}Jefferson Motel"); }
        case 74: { SetPlayerPos(playerid, 1.5491, 23.3183, 1199.5938); SetPlayerFacingAngle(playerid, 359.9054); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[74] {FFFFFF}Shamal Aircraft"); }
        case 75: { SetPlayerPos(playerid, 681.6216, -451.8933, -25.6172); SetPlayerFacingAngle(playerid, 166.166); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[75] {FFFFFF}The Welcome Pump"); }
        case 76: { SetPlayerPos(playerid, 234.6087, 1187.8195, 1080.2578); SetPlayerFacingAngle(playerid, 349.4844); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[76] {FFFFFF}House 1"); }
        case 77: { SetPlayerPos(playerid, 225.5707, 1240.0643, 1082.1406); SetPlayerFacingAngle(playerid, 96.2852); SetPlayerInterior(playerid, 2); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[77] {FFFFFF}House 2"); }
        case 78: { SetPlayerPos(playerid, 224.288, 1289.1907, 1082.1406); SetPlayerFacingAngle(playerid, 359.868); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[78] {FFFFFF}House 3"); }
        case 79: { SetPlayerPos(playerid, 239.2819, 1114.1991, 1080.9922); SetPlayerFacingAngle(playerid, 270.2654); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[79] {FFFFFF}House 4"); }
        case 80: { SetPlayerPos(playerid, 207.5219, -109.7448, 1005.1328); SetPlayerFacingAngle(playerid, 358.62); SetPlayerInterior(playerid, 15); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[80] {FFFFFF}Binco Store"); }
        case 81: { SetPlayerPos(playerid, 295.1391, 1473.3719, 1080.2578); SetPlayerFacingAngle(playerid, 352.9526); SetPlayerInterior(playerid, 15); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[81] {FFFFFF}House 5"); }
        case 82: { SetPlayerPos(playerid, -1417.8927, 932.4482, 1041.5313); SetPlayerFacingAngle(playerid, 0.7013); SetPlayerInterior(playerid, 15); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[82] {FFFFFF}Studio 4"); }
        case 83: { SetPlayerPos(playerid, 446.3247, 509.9662, 1001.4195); SetPlayerFacingAngle(playerid, 330.5671); SetPlayerInterior(playerid, 12); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[83] {FFFFFF}Motel Room"); }
        case 84: { SetPlayerPos(playerid, 2306.3826, -15.2365, 26.7496); SetPlayerFacingAngle(playerid, 274.49); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[84] {FFFFFF}Bank"); }
        case 85: { SetPlayerPos(playerid, 2331.8984, 6.7816, 26.5032); SetPlayerFacingAngle(playerid, 100.2357); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[85] {FFFFFF}Palamino Restaurant"); }
        case 86: { SetPlayerPos(playerid, 663.0588, -573.6274, 16.3359); SetPlayerFacingAngle(playerid, 264.9829); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[86] {FFFFFF}Dillimore Gas Station"); }
        case 87: { SetPlayerPos(playerid, -227.5703, 1401.5544, 27.7656); SetPlayerFacingAngle(playerid, 269.2978); SetPlayerInterior(playerid, 18); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[87] {FFFFFF}Lil' Probe Inn"); }
        case 88: { SetPlayerPos(playerid, -688.1496, 942.0826, 13.6328); SetPlayerFacingAngle(playerid, 177.6574); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[88] {FFFFFF}Torreno's Ranch"); }
        case 89: { SetPlayerPos(playerid, -1916.1268, 714.8617, 46.5625); SetPlayerFacingAngle(playerid, 152.2839); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[89] {FFFFFF}Zombotech"); }
        case 90: { SetPlayerPos(playerid, 818.7714, -1102.8689, 25.794); SetPlayerFacingAngle(playerid, 91.1439); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[90] {FFFFFF}Cemetery"); }
        case 91: { SetPlayerPos(playerid, 255.2083, -59.6753, 1.5703); SetPlayerFacingAngle(playerid, 1.4645); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[91] {FFFFFF}Blueberry Liquor Store"); }
        case 92: { SetPlayerPos(playerid, 446.626, 1397.738, 1084.3047); SetPlayerFacingAngle(playerid, 343.9647); SetPlayerInterior(playerid, 2); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[92] {FFFFFF}House 6"); }
        case 93: { SetPlayerPos(playerid, 227.3922, 1114.6572, 1080.9985); SetPlayerFacingAngle(playerid, 267.459); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[93] {FFFFFF}House 7"); }
        case 94: { SetPlayerPos(playerid, 227.7559, 1114.3844, 1080.9922); SetPlayerFacingAngle(playerid, 266.2624); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[94] {FFFFFF}House 8"); }
        case 95: { SetPlayerPos(playerid, 261.1165, 1287.2197, 1080.2578); SetPlayerFacingAngle(playerid, 178.9149); SetPlayerInterior(playerid, 4); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[95] {FFFFFF}House 9"); }
        case 96: { SetPlayerPos(playerid, 291.7626, -80.1306, 1001.5156); SetPlayerFacingAngle(playerid, 290.2195); SetPlayerInterior(playerid, 4); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[96] {FFFFFF}Ammu-nation - 3"); }
        case 97: { SetPlayerPos(playerid, 449.0172, -88.9894, 999.5547); SetPlayerFacingAngle(playerid, 89.6608); SetPlayerInterior(playerid, 4); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[97] {FFFFFF}Jay's Restaurant"); }
        case 98: { SetPlayerPos(playerid, -27.844, -26.6737, 1003.5573); SetPlayerFacingAngle(playerid, 184.3118); SetPlayerInterior(playerid, 4); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[98] {FFFFFF}24/7 - 5"); }
        case 99: { SetPlayerPos(playerid, 2135.2004, -2276.2815, 20.6719); SetPlayerFacingAngle(playerid, 318.59); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[99] {FFFFFF}Warehouse 3"); }
        case 100: { SetPlayerPos(playerid, 306.1966, 307.819, 1003.3047); SetPlayerFacingAngle(playerid, 203.1354); SetPlayerInterior(playerid, 4); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[100] {FFFFFF}Michelle's Love Nest"); }
        case 101: { SetPlayerPos(playerid, 24.3769, 1341.1829, 1084.375); SetPlayerFacingAngle(playerid, 8.3305); SetPlayerInterior(playerid, 10); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[101] {FFFFFF}House 10"); }
        case 102: { SetPlayerPos(playerid, 963.0586, 2159.7563, 1011.0303); SetPlayerFacingAngle(playerid, 175.313); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[102] {FFFFFF}Sindacco Abatoir"); }
        case 103: { SetPlayerPos(playerid, 2548.4807, 2823.7429, 10.8203); SetPlayerFacingAngle(playerid, 270.6003); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[103] {FFFFFF}Port"); }
        case 104: { SetPlayerPos(playerid, 215.1515, 1874.0579, 13.1406); SetPlayerFacingAngle(playerid, 177.5538); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[104] {FFFFFF}Area 69"); }
        case 105: { SetPlayerPos(playerid, 221.6766, 1142.4962, 1082.6094); SetPlayerFacingAngle(playerid, 184.9618); SetPlayerInterior(playerid, 4); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[105] {FFFFFF}House 11"); }
        case 106: { SetPlayerPos(playerid, 2323.7063, -1147.6509, 1050.7101); SetPlayerFacingAngle(playerid, 206.5352); SetPlayerInterior(playerid, 12); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[106] {FFFFFF}House 12"); }
        case 107: { SetPlayerPos(playerid, 344.9984, 307.1824, 999.1557); SetPlayerFacingAngle(playerid, 193.643); SetPlayerInterior(playerid, 6); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[107] {FFFFFF}Millie's Bedroom"); }
        case 108: { SetPlayerPos(playerid, 411.9707, -51.9217, 1001.8984); SetPlayerFacingAngle(playerid, 173.3449); SetPlayerInterior(playerid, 12); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[108] {FFFFFF}Barber 3"); }
        case 109: { SetPlayerPos(playerid, -1421.5618, -663.8262, 1059.5569); SetPlayerFacingAngle(playerid, 170.9341); SetPlayerInterior(playerid, 4); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[109] {FFFFFF}Studio 5"); }
        case 110: { SetPlayerPos(playerid, 773.8887, -47.7698, 1000.5859); SetPlayerFacingAngle(playerid, 10.7161); SetPlayerInterior(playerid, 6); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[110] {FFFFFF}Cobra Gym"); }
        case 111: { SetPlayerPos(playerid, 246.6695, 65.8039, 1003.6406); SetPlayerFacingAngle(playerid, 7.9562); SetPlayerInterior(playerid, 6); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[111] {FFFFFF}Los Santos Police Station"); }
        case 112: { SetPlayerPos(playerid, -1864.9434, 55.7325, 1055.5276); SetPlayerFacingAngle(playerid, 85.8541); SetPlayerInterior(playerid, 14); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[112] {FFFFFF}Los Santos Airport"); }
        case 113: { SetPlayerPos(playerid, -262.1759, 1456.6158, 1084.3672); SetPlayerFacingAngle(playerid, 82.459); SetPlayerInterior(playerid, 4); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[113] {FFFFFF}House 13"); }
        case 114: { SetPlayerPos(playerid, 22.861, 1404.9165, 1084.4297); SetPlayerFacingAngle(playerid, 349.6158); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[114] {FFFFFF}House 14"); }
        case 115: { SetPlayerPos(playerid, 140.3679, 1367.8837, 1083.8621); SetPlayerFacingAngle(playerid, 349.2372); SetPlayerInterior(playerid, 5); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[115] {FFFFFF}House 15"); }
        case 116: { SetPlayerPos(playerid, 1494.8589, 1306.48, 1093.2953); SetPlayerFacingAngle(playerid, 196.065); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[116] {FFFFFF}School 1"); }
        case 117: { SetPlayerPos(playerid, -1813.213, -58.012, 1058.9641); SetPlayerFacingAngle(playerid, 335.3199); SetPlayerInterior(playerid, 14); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[117] {FFFFFF}Francis International Airport"); }
        case 118: { SetPlayerPos(playerid, -1401.067, 1265.3706, 1039.8672); SetPlayerFacingAngle(playerid, 178.6483); SetPlayerInterior(playerid, 16); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[118] {FFFFFF}Studio 6"); }
        case 119: { SetPlayerPos(playerid, 234.2826, 1065.229, 1084.2101); SetPlayerFacingAngle(playerid, 4.3864); SetPlayerInterior(playerid, 6); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[119] {FFFFFF}House 16"); }
        case 120: { SetPlayerPos(playerid, -68.5145, 1353.8485, 1080.2109); SetPlayerFacingAngle(playerid, 3.5742); SetPlayerInterior(playerid, 6); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[120] {FFFFFF}House 17"); }
        case 121: { SetPlayerPos(playerid, -2240.1028, 136.973, 1035.4141); SetPlayerFacingAngle(playerid, 269.0954); SetPlayerInterior(playerid, 6); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[121] {FFFFFF}Toy Store"); }
        case 122: { SetPlayerPos(playerid, -2240.1028, 136.973, 1035.4141); SetPlayerFacingAngle(playerid, 20.2254); SetPlayerInterior(playerid, 6); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[122] {FFFFFF}Ammu-nation - 4"); }
        case 123: { SetPlayerPos(playerid, 316.5025, -167.6272, 999.5938); SetPlayerFacingAngle(playerid, 10.3031); SetPlayerInterior(playerid, 6); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[123] {FFFFFF}Ammu-nation 5"); }
        case 124: { SetPlayerPos(playerid, -285.2511, 1471.197, 1084.375); SetPlayerFacingAngle(playerid, 85.6547); SetPlayerInterior(playerid, 15); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[124] {FFFFFF}House 18"); }
        case 125: { SetPlayerPos(playerid, -26.8339, -55.5846, 1003.5469); SetPlayerFacingAngle(playerid, 3.9528); SetPlayerInterior(playerid, 6); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[125] {FFFFFF}24/7 - 6"); }
        case 126: { SetPlayerPos(playerid, 442.1295, -52.4782, 999.7167); SetPlayerFacingAngle(playerid, 177.9394); SetPlayerInterior(playerid, 6); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[126] {FFFFFF}Restaurant"); }
        case 127: { SetPlayerPos(playerid, 2182.2017, 1628.5848, 1043.8723); SetPlayerFacingAngle(playerid, 224.8601); SetPlayerInterior(playerid, 2); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[127] {FFFFFF}Rosenberg's Office"); }
        case 128: { SetPlayerPos(playerid, 748.4623, 1438.2378, 1102.9531); SetPlayerFacingAngle(playerid, 0.6069); SetPlayerInterior(playerid, 6); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[128] {FFFFFF}Brothel 3"); }
        case 129: { SetPlayerPos(playerid, 2807.3604, -1171.7048, 1025.5703); SetPlayerFacingAngle(playerid, 193.7117); SetPlayerInterior(playerid, 8); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[129] {FFFFFF}House 18"); }
        case 130: { SetPlayerPos(playerid, 366.0002, -9.4338, 1001.8516); SetPlayerFacingAngle(playerid, 160.528); SetPlayerInterior(playerid, 9); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[130] {FFFFFF}Cluckin' Bell"); }
        case 131: { SetPlayerPos(playerid, 2216.1282, -1076.3052, 1050.4844); SetPlayerFacingAngle(playerid, 86.428); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[131] {FFFFFF}House 19"); }
        case 132: { SetPlayerPos(playerid, 2268.5156, 1647.7682, 1084.2344); SetPlayerFacingAngle(playerid, 99.7331); SetPlayerInterior(playerid, 1); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[132] {FFFFFF}Caligulas Roof"); }
        case 133: { SetPlayerPos(playerid, 2236.6997, -1078.9478, 1049.0234); SetPlayerFacingAngle(playerid, 2.5706); SetPlayerInterior(playerid, 2); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[133] {FFFFFF}Old Venturas Strip"); }
        case 134: { SetPlayerPos(playerid, -2031.1196, -115.8287, 1035.1719); SetPlayerFacingAngle(playerid, 190.1877); SetPlayerInterior(playerid, 3); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[134] {FFFFFF}School 2"); }
        case 135: { SetPlayerPos(playerid, 2365.1089, -1133.0795, 1050.875); SetPlayerFacingAngle(playerid, 177.3947); SetPlayerInterior(playerid, 8); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[135] {FFFFFF}House 20"); }
        case 136: { SetPlayerPos(playerid, 1168.512, 1360.1145, 10.9293); SetPlayerFacingAngle(playerid, 196.5933); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[136] {FFFFFF}School 3"); }
        case 137: { SetPlayerPos(playerid, 315.4544, 976.5972, 1960.8511); SetPlayerFacingAngle(playerid, 359.6368); SetPlayerInterior(playerid, 9); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[137] {FFFFFF}Andromada Aircraft"); }
        case 138: { SetPlayerPos(playerid, 1893.0731, 1017.8958, 31.8828); SetPlayerFacingAngle(playerid, 86.1044); SetPlayerInterior(playerid, 10); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[138] {FFFFFF}Janitor's Office"); }
        case 139: { SetPlayerPos(playerid, 501.9578, -70.5648, 998.7578); SetPlayerFacingAngle(playerid, 171.5706); SetPlayerInterior(playerid, 11); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[139] {FFFFFF}Bar"); }
        case 140: { SetPlayerPos(playerid, -42.5267, 1408.23, 1084.4297); SetPlayerFacingAngle(playerid, 172.068); SetPlayerInterior(playerid, 8); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[140] {FFFFFF}House 21"); }
        case 141: { SetPlayerPos(playerid, 2283.3118, 1139.307, 1050.8984); SetPlayerFacingAngle(playerid, 19.7032); SetPlayerInterior(playerid, 11); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[141] {FFFFFF}House 22"); }
        case 142: { SetPlayerPos(playerid, 84.9244, 1324.2983, 1083.8594); SetPlayerFacingAngle(playerid, 159.5582); SetPlayerInterior(playerid, 9); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[142] {FFFFFF}House 23"); }
        case 143: { SetPlayerPos(playerid, 260.7421, 1238.2261, 1084.2578); SetPlayerFacingAngle(playerid, 84.3084); SetPlayerInterior(playerid, 9); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[143] {FFFFFF}House 24"); }
        case 144: { SetPlayerPos(playerid, -1658.1656, 1215.0002, 7.25); SetPlayerFacingAngle(playerid, 103.9074); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[144] {FFFFFF}Otto's Autos"); }
        case 145: { SetPlayerPos(playerid, -1961.6281, 295.2378, 35.4688); SetPlayerFacingAngle(playerid, 264.4891); SetPlayerInterior(playerid, 0); ClearChat(playerid, 100); SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You entered {00EEFE}[145] {FFFFFF}Wang Cars"); }
    }
    SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}To save it to a file, type {MAIN}/saveint{FFFFFF}.");
    SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}To exit the interior, type {MAIN}/exitint{FFFFFF}.");
    SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}To see more interiors, type {MAIN}/interiors{FFFFFF}.");
    return 1;
}
