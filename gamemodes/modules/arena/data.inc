new const arenaData[MAX_ZONES][arenaa] =
{
	{"Vinewood", {792.0, -1327.0, 1075.0, -1142.0}, ARENA_CATEGORY_BIG},
	{"Valle Ocultado", {-892.5, 2722.5, -725.5, 2780.5}, ARENA_CATEGORY_MEDIUM},
	{"Las Paysadas", {-358.5, 2580.5, -135.5, 2751.5}, ARENA_CATEGORY_BIG},
	{"Las Brujas", {-371.5, 1726.5, -280.5, 1904.5}, ARENA_CATEGORY_MEDIUM},
	{"Area 51", {117.5, 1821.5, 254.5, 1922.5}, ARENA_CATEGORY_MEDIUM},
	{"Fort Carson", {-354.5, 1010.5, 40.5, 1229.5}, ARENA_CATEGORY_BIG},
	{"Spiny Bed", {2234.5, 2697.5, 2458.5, 2842.5}, ARENA_CATEGORY_SMALL},
	{"Prickle Pine", {1701.5, 2720.5, 1913.5, 2880.5}, ARENA_CATEGORY_MEDIUM},
	{"Roca Escalante", {2294.5, 2243.5, 2415.5, 2408.5}, ARENA_CATEGORY_SMALL},
	{"Linden Side", {2781.5, 837.5, 2893.5, 1018.5}, ARENA_CATEGORY_SMALL},
	{"Last Dime Motel", {1855.5, 621.5, 1995.5, 781.5}, ARENA_CATEGORY_SMALL},
	{"Esplanade East", {-1605.0, 824.5, -1419.0, 1236.5}, ARENA_CATEGORY_BIG},
	{"Avispa Country Club", {-2819.0, -343.5, -2627.0, -208.5}, ARENA_CATEGORY_SMALL},
	{"City Hall", {-2840.0, 279.5, -2598.0, 471.5}, ARENA_CATEGORY_MEDIUM},
	{"Bayside", {-2562.0, 2245.5, -2229.0, 2441.5}, ARENA_CATEGORY_BIG},
	{"Garcia", {-2404.0, -76.5, -2251.0, 246.5}, ARENA_CATEGORY_MEDIUM},
	{"Downtown San Fierro", {-2010.0, 400.5, -1811.0, 599.5}, ARENA_CATEGORY_MEDIUM},
	{"Foster Valley", {-2163.0, -1007.5, -1926.0, -711.5}, ARENA_CATEGORY_BIG},
	{"Junkyard", {-1943.0, -1740.5, -1753.0, -1596.5}, ARENA_CATEGORY_SMALL},
	{"Angel Pine", {-2335.0, -2585.5, -1922.0, -2216.5}, ARENA_CATEGORY_BIG},
	{"Conference Center", {1036.5, -1857.5, 1307.5, -1678.5}, ARENA_CATEGORY_MEDIUM},
	{"Santa Maria Beach", {360.5, -1849.5, 599.5, -1721.5}, ARENA_CATEGORY_BIG},
	{"Rodeo", {275.0, -1648.5, 463.0, -1475.5}, ARENA_CATEGORY_MEDIUM},
	{"Sunrise", {1077.0, -1042.5, 1349.0, -934.5}, ARENA_CATEGORY_MEDIUM},
	{"Mulholland Intersection", {1528.0, -1162.5, 1820.0, -1005.5}, ARENA_CATEGORY_MEDIUM},
	{"Glen Park", {1855.0, -1258.5, 2063.0, -1139.5}, ARENA_CATEGORY_MEDIUM},
	{"Skate Park", {1850.0, -1476.5, 2099.0, -1321.5}, ARENA_CATEGORY_MEDIUM},	
	{"Ganton", {2222.5, -1755.5, 2553.5, -1603.5}, ARENA_CATEGORY_BIG},
	{"Los Santos Parking Lot", {1312.0, -2378.5, 1574.0, -2203.5}, ARENA_CATEGORY_BIG},
	{"Ocean Docks", {2589.0, -2517.5, 2778.0, -2373.5}, ARENA_CATEGORY_SMALL},
	{"Unity Station", {1685.0, -1951.5, 1831.0, -1842.5}, ARENA_CATEGORY_SMALL},
	{"East Beach", {2756.0, -2030.5, 2906.0, -1835.5}, ARENA_CATEGORY_MEDIUM},
	{"Idlewood", {1953.5, -1814.5, 2131.5, -1589.5}, ARENA_CATEGORY_MEDIUM},
	{"Jefferson Suburban", {2132.0, -1480.0, 2301.5, -1310.5}, ARENA_CATEGORY_MEDIUM},
	{"Jefferson", {2056.5, -1302.5, 2315.5, -1118.5}, ARENA_CATEGORY_MEDIUM},
	{"Montgomery", {1100.0, 127.5, 1513.0, 496.5}, ARENA_CATEGORY_BIG},
	{"Dillimore", {566.0, -665.5, 870.0, -431.5}, ARENA_CATEGORY_MEDIUM},
	{"Fallen Tree", {-625.0, -565.5, -466.0, -467.5}, ARENA_CATEGORY_SMALL},
	{"Panopticon", {-592.0, -209.5, -431.0, -39.5}, ARENA_CATEGORY_SMALL},
	{"Julius Thruway East", {2647.0, 1978.5, 2792.0, 2209.5}, ARENA_CATEGORY_MEDIUM},
	{"Hunter Quarry", {495.0, 799.0, 691.0, 939.0}, ARENA_CATEGORY_MEDIUM},
	{"Leafy Hollow", {-1472.0, -1601.0, -1401.0, -1443.0}, ARENA_CATEGORY_MEDIUM},
	{"Green Palms", {110.0, 1330.0, 349.0, 1484.0}, ARENA_CATEGORY_MEDIUM},
	{"The Big Ear", {-385.0, 1451.0, -263.0, 1574.0}, ARENA_CATEGORY_MEDIUM},
	{"El Corona", {1829.0, -2143.0, 1953.0, -1973.0}, ARENA_CATEGORY_MEDIUM},
	{"Doherty", {-2219.5, -286.5, -1994.5, -69.5}, ARENA_CATEGORY_MEDIUM},
	{"Flint Gas", {-180.5, -1215.5, 3.5, -1043.5}, ARENA_CATEGORY_MEDIUM},
	{"Ghost Town", {-476.5, 2161.5, -318.5, 2319.5}, ARENA_CATEGORY_MEDIUM},
	{"Las Barrancas", {-925.5, 1427.5, -713.5, 1634.5}, ARENA_CATEGORY_MEDIUM},
	{"El Quebrados", {-1621.5, 2525.5, -1374.5, 2715.5}, ARENA_CATEGORY_MEDIUM},
	{"East Los Santos", {2417.5, -1543.5, 2591.5, -1239.5}, ARENA_CATEGORY_MEDIUM},
	{"LV Flight School", {1251.5, 1202.5, 1397.5, 1424.5}, ARENA_CATEGORY_MEDIUM},
	{"Blueberry", {129.5, -236.5, 359.5, 31.5}, ARENA_CATEGORY_MEDIUM},
	{"Redsands West", {1572.5, 2016.5, 1728.5, 2258.5}, ARENA_CATEGORY_BIG},
	{"Wedding Chapel", {2419.0, 1220.5, 2625.0, 1608.5}, ARENA_CATEGORY_BIG},
	{"Palomino Creek", {2219.5, -69.5, 2543.5, 154.5}, ARENA_CATEGORY_BIG},
	{"Willowfield", {2314.5, -2044.5, 2577.5, -1901.5}, ARENA_CATEGORY_MEDIUM},
	{"Ocean Flats", {-2807.5, -71.5, -2600.5, 42.5}, ARENA_CATEGORY_MEDIUM},
	{"Chinese Garden", {3452.0, -19.5, 3532.0, 20.0},      ARENA_CATEGORY_CUSTOM, 1001.0, 0},
	{"Desert",         {3437.0, 957.5, 3530.0, 1039.0},    ARENA_CATEGORY_CUSTOM, 1000.0, 0},
	{"Egypt",          {3420.5, 1912.5, 3562.0, 2057.0},   ARENA_CATEGORY_CUSTOM, 1003.0, 0},
	{"Urbex",          {-61.0, 2472.5, 14.0, 2518.0},      ARENA_CATEGORY_CUSTOM, 992.0,  0},
	{"Zoo",            {3466.0, -1019.0, 3537.0, -972.0},  ARENA_CATEGORY_CUSTOM, 1000.0, 0}
};

new Iterator:ArenaCategory<MAX_ARENA_CATEGORIES, MAX_ZONES>;

createZones()
{
	for (new i = 0; i < MAX_ZONES; i++)
	{
		CreateZone(
			arenaData[i][arenaPos][0],
			arenaData[i][arenaPos][1],
			arenaData[i][arenaPos][2],
			arenaData[i][arenaPos][3]
		);

		Iter_Add(ArenaCategory<arenaData[i][category]>, i);
		Iter_Add(ArenaQueue<arenaData[i][category]>, i);
	}

	for (new i = 0; i < MAX_ZONES; i++)
		CreateZoneNumber(i,i,1);

	mysql_tquery(dbHandle, "SELECT zoneid, clanid, clancolor FROM `zones`", "ZoneColours");

	return true;
}
