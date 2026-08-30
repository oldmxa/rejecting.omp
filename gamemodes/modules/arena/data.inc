new const arenaData[MAX_ZONES][arenaa] =
{
	{"Vinewood", {792.0, -1327.0, 1075.0, -1142.0}, ARENA_CATEGORY_BIG},
	{"Valle Ocultado", {-892.5, 2722.5, -725.5, 2780.5}, ARENA_CATEGORY_SMALL},
	{"Las Paysadas", {-358.5, 2580.5, -135.5, 2751.5}, ARENA_CATEGORY_BIG},
	{"Las Brujas", {-371.5, 1726.5, -280.5, 1904.5}, ARENA_CATEGORY_MEDIUM},
	{"Area 51", {117.5, 1821.5, 254.5, 1922.5}, ARENA_CATEGORY_MEDIUM},
	{"Fort Carson", {-354.5, 1010.5, 40.5, 1229.5}, ARENA_CATEGORY_BIG},
	{"Spiny Bed", {2234.5, 2697.5, 2458.5, 2842.5}, ARENA_CATEGORY_MEDIUM},
	{"Prickle Pine", {1701.5, 2720.5, 1913.5, 2880.5}, ARENA_CATEGORY_MEDIUM},
	{"Roca Escalante", {2279.0210, 2230.6528, 2443.0210, 2445.6528}, ARENA_CATEGORY_MEDIUM},
	{"Linden Side", {2781.5, 837.5, 2893.5, 1018.5}, ARENA_CATEGORY_MEDIUM},
	{"Last Dime Motel", {1813.0383, 610.0694, 2080.0383, 792.0694}, ARENA_CATEGORY_MEDIUM},
	{"Esplanade East", {-1605.0, 824.5, -1419.0, 1236.5}, ARENA_CATEGORY_BIG},
	{"Avispa Country Club", {-2819.0, -343.5, -2627.0, -208.5}, ARENA_CATEGORY_SMALL},
	{"City Hall", {-2840.0, 279.5, -2598.0, 471.5}, ARENA_CATEGORY_MEDIUM},
	{"Bayside", {-2562.0, 2245.5, -2229.0, 2441.5}, ARENA_CATEGORY_BIG},
	{"Garcia", {-2404.0, -76.5, -2251.0, 246.5}, ARENA_CATEGORY_MEDIUM},
	{"Downtown San Fierro", {-2010.0, 400.5, -1811.0, 599.5}, ARENA_CATEGORY_MEDIUM},
	{"Foster Valley", {-2163.0, -1007.5, -1926.0, -711.5}, ARENA_CATEGORY_BIG},
	{"Junkyard", {-1943.0, -1740.5, -1753.0, -1596.5}, ARENA_CATEGORY_SMALL},
	{"Angel Pine", {-2335.0, -2585.5, -1922.0, -2216.5}, ARENA_CATEGORY_BIG},
	{"Conference Center", {1021.4, -1860.9, 1304.4, -1700.9}, ARENA_CATEGORY_MEDIUM},
	{"Santa Maria Beach", {360.5, -1849.5, 599.5, -1721.5}, ARENA_CATEGORY_BIG},
	{"Rodeo", {275.0, -1648.5, 463.0, -1475.5}, ARENA_CATEGORY_MEDIUM},
	{"Sunrise", {1077.0, -1042.5, 1349.0, -934.5}, ARENA_CATEGORY_MEDIUM},
	{"Mulholland Intersection", {1528.0, -1162.5, 1820.0, -1005.5}, ARENA_CATEGORY_MEDIUM},
	{"Glen Park", {1855.0, -1258.5, 2063.0, -1139.5}, ARENA_CATEGORY_MEDIUM},
	{"Skate Park", {1850.0, -1476.5, 2099.0, -1321.5}, ARENA_CATEGORY_MEDIUM},	
	{"Ganton", {2222.5, -1755.5, 2553.5, -1603.5}, ARENA_CATEGORY_MEDIUM},
	{"Los Santos Parking Lot", {1312.0, -2378.5, 1574.0, -2203.5}, ARENA_CATEGORY_BIG},
	{"Ocean Docks", {2532.0383, -2569.9272, 2809.0383, -2327.9272}, ARENA_CATEGORY_MEDIUM},
	{"Unity Station", {1674.0383, -1971.6007, 1912.0383, -1807.6007}, ARENA_CATEGORY_MEDIUM},
	{"East Beach", {2607.0, -2058.9, 2910.0, -1890.9}, ARENA_CATEGORY_MEDIUM},
	{"Idlewood", {1953.5, -1814.5, 2131.5, -1589.5}, ARENA_CATEGORY_MEDIUM},
	{"Jefferson Suburban", {2132.0, -1480.0, 2301.5, -1310.5}, ARENA_CATEGORY_MEDIUM},
	{"Jefferson", {2056.5, -1302.5, 2315.5, -1118.5}, ARENA_CATEGORY_MEDIUM},
	{"Montgomery", {1100.0, 127.5, 1513.0, 496.5}, ARENA_CATEGORY_BIG},
	{"Dillimore", {566.0, -665.5, 870.0, -431.5}, ARENA_CATEGORY_MEDIUM},
	{"Fallen Tree", {-625.0, -565.5, -466.0, -467.5}, ARENA_CATEGORY_SMALL},
	{"Panopticon", {-592.0, -209.5, -431.0, -39.5}, ARENA_CATEGORY_SMALL},
	{"Julius Thruway East", {2647.0, 1978.5, 2792.0, 2209.5}, ARENA_CATEGORY_MEDIUM},
	{"Hunter Quarry", {495.0, 799.0, 691.0, 939.0}, ARENA_CATEGORY_MEDIUM},
	{"Leafy Hollow", {-1472.0, -1601.0, -1401.0, -1443.0}, ARENA_CATEGORY_SMALL},
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
	{"Vinewood North", {791.0, -1132.0, 1058.0, -976.0}, ARENA_CATEGORY_MEDIUM},
	{"Octane Springs", {348.0, 1083.0, 730.0, 1289.5}, ARENA_CATEGORY_BIG},
	{"The BIG Spread Ranch", {651.4, 1825.0, 816.4, 2098.0}, ARENA_CATEGORY_MEDIUM},
	{"Easter Basin GAS", {-1789.5, 289.3, -1533.5, 551.3}, ARENA_CATEGORY_MEDIUM},
	{"Juniper Hollow", {-2526.0, 904.0, -2302.0, 1111.0}, ARENA_CATEGORY_MEDIUM},
	{"Richman", {145.0, -1313.2, 396.0, -1121.2}, ARENA_CATEGORY_MEDIUM},
	{"Willowfield", {1992.020996, -2107.920349, 2280.020996, -1918.920349}, ARENA_CATEGORY_MEDIUM},
	{"LS International", {1504.0, -2380.9, 1829.0, -2187.9}, ARENA_CATEGORY_BIG},
	{"Fishing Dropoff", {2155.041870, -2709.382019, 2288.041870, -2477.382019}, ARENA_CATEGORY_MEDIUM},
	{"LSPD", {1538.9863, -1718.3581, 1688.9863, -1578.3581}, ARENA_CATEGORY_SMALL},
	{"Hilltop Farm", {920.920288, -429.593994, 1195.920288, -245.593994}, ARENA_CATEGORY_MEDIUM},
	{"Randolph Industrial", {1338.159851, 619.513854, 1778.159851, 823.513854}, ARENA_CATEGORY_BIG},
	{"Old Venturas Strip", {1891.125122, 2101.864532, 2162.125122, 2324.864532}, ARENA_CATEGORY_MEDIUM},
	{"Abandoned Airstrip", {53.701538, 2409.513885, 444.701538, 2603.513885}, ARENA_CATEGORY_BIG},
	{"Battery Point", {-2748.520828, 1337.069396, -2583.520828, 1485.069396}, ARENA_CATEGORY_SMALL},
	{"SFPD HQ", {-1731.520767, 622.729064, -1515.520767, 756.729064}, ARENA_CATEGORY_SMALL},
	{"East Beach Highway", {2702.982788, -1670.472534, 2907.982788, -1449.472534}, ARENA_CATEGORY_MEDIUM},
	{"Market Star Street", {1058.351013, -1402.125122, 1367.351013, -1148.125122}, ARENA_CATEGORY_BIG},
	{"Chinese Garden", {3452.0, -19.5, 3532.0, 20.0},      ARENA_CATEGORY_CUSTOM, 1001.0, 0},
	{"Desert",         {3437.0, 957.5, 3530.0, 1039.0},    ARENA_CATEGORY_CUSTOM, 1000.0, 0},
	{"Egypt",          {3420.5, 1912.5, 3562.0, 2057.0},   ARENA_CATEGORY_CUSTOM, 1003.0, 0},
	{"Urbex",          {-61.0, 2472.5, 14.0, 2518.0},      ARENA_CATEGORY_CUSTOM, 992.0,  0},
	{"Zoo",            {3466.0, -1019.0, 3537.0, -972.0},  ARENA_CATEGORY_CUSTOM, 1000.0, 0}
};

#assert MAX_ZONES == sizeof(arenaData)

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
