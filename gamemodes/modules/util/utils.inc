// gneral utilities: strings, colors, dates, and conversions


stock getDateStrTD()
{
	new dString[32];
	new year, month, day, hour, minute, s;
	getdate(year, month, day);
	gettime(hour, minute, s);

	format(dString, sizeof(dString), "%02d.%02d.%02d - %02d:%02d",	day, month,	year, hour, minute);
	return dString;
}

stock FormatNumber(amount, separator = '.')
{
    new str[32];
    format(str, sizeof(str), "%d", amount);

    new len = strlen(str);
    new is_neg = (amount < 0);
    new start_idx = is_neg ? 1 : 0;
    new digits = len - start_idx;

    if (digits <= 3) return str;

    new result[32];
    new res_idx = 0;

    for (new i = 0; i < len; i++)
    {
        result[res_idx++] = str[i];
        if (i >= start_idx && i < len - 1)
        {
            new remaining = len - 1 - i;
            if (remaining % 3 == 0)
            {
                result[res_idx++] = separator;
            }
        }
    }
    result[res_idx] = EOS;
    return result;
}

stock FormatMoney(amount)
{
    new result[36];
    if (amount < 0)
    {
        format(result, sizeof(result), "-$%s", FormatNumber(-amount, '.'));
    }
    else
    {
        format(result, sizeof(result), "$%s", FormatNumber(amount, '.'));
    }
    return result;
}

stock FormatCoins(amount)
{
    new result[36];
    if (amount < 0)
    {
        format(result, sizeof(result), "-%s Coins", FormatNumber(-amount, '.'));
    }
    else
    {
        format(result, sizeof(result), "%s Coins", FormatNumber(amount, '.'));
    }
    return result;
}

stock FormatCoinsDelta(amount)
{
    new result[40];

    if (amount < 0) format(result, sizeof(result), "{F84F31}-R$%s", FormatNumber(-amount, '.'));
    else format(result, sizeof(result), "{23C552}+R$%s", FormatNumber(amount, '.'));

    return result;
}

stock GetColorHexTag(const colorTag[])
{
    new tag[7] = "FFFFFF";
    if(strfind(colorTag, "~r", true) != -1) tag = "FF4B4B";
    else if(strfind(colorTag, "~g", true) != -1) tag = "54FB62";
    else if(strfind(colorTag, "~b", true) != -1) tag = "5C5CFF";
    else if(strfind(colorTag, "~y", true) != -1) tag = "FFFF5A";
    else if(strfind(colorTag, "~p", true) != -1) tag = "FF5CFF";
    else if(strfind(colorTag, "~o", true) != -1) tag = "FFB84D";
    else if(strfind(colorTag, "~w", true) != -1) tag = "FFFFFF";
    return tag;
}

stock GetPlayerHudColorIndex(playerid)
{
    if(playerid < 0 || playerid > MAX_PLAYERS) return 0;

    new idx = User[playerid][hudColor];
    if(idx < 0 || idx >= sizeof(hudPersonalization)) return 0;
    return idx;
}

stock GetPlayerShadeIndex(playerid, bool:secondary)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return -1;

    new idx = secondary ? User[playerid][hudShadeTwo] : User[playerid][hudShadeOne];
    if(idx < 0 || idx >= sizeof(hudShades)) return -1;
    return idx;
}

stock GetPlayerHudTag(playerid, bool:secondary)
{
    new tag[20], shade = GetPlayerShadeIndex(playerid, secondary);

    if(shade != -1)
    {
        format(tag, sizeof(tag), "%s", hudShades[shade][shadeTag]);
        return tag;
    }

    new preset = GetPlayerHudColorIndex(playerid);

    if(secondary) format(tag, sizeof(tag), "%s", hudPersonalization[preset][colorTwo]);
    else format(tag, sizeof(tag), "%s", hudPersonalization[preset][colorOne]);

    return tag;
}

stock GetPlayerChatIDColorHex(playerid, dest[], len = sizeof(dest))
{
    if(playerid < 0 || playerid >= MAX_PLAYERS)
    {
        format(dest, len, "FFFFFF");
        return 1;
    }
    new val = User[playerid][chatIDColor];
    if(1 <= val <= sizeof(hudShades))
    {
        format(dest, len, "%06x", hudShades[val - 1][shadeHex]);
    }
    else if(val & 0x10000000)
    {
        format(dest, len, "%06x", val & 0xFFFFFF);
    }
    else
    {
        format(dest, len, "FFFFFF");
    }
    return 1;
}

stock GetPlayerChatTextColorHex(playerid, dest[], len = sizeof(dest))
{
    if(playerid < 0 || playerid >= MAX_PLAYERS)
    {
        format(dest, len, "FFFFFF");
        return 1;
    }
    new val = User[playerid][chatColor];
    if(1 <= val <= sizeof(hudShades))
    {
        format(dest, len, "%06x", hudShades[val - 1][shadeHex]);
    }
    else if(val & 0x10000000)
    {
        format(dest, len, "%06x", val & 0xFFFFFF);
    }
    else
    {
        format(dest, len, "FFFFFF");
    }
    return 1;
}

stock GetPlayerChatMainColorHex(playerid, dest[], len = sizeof(dest))
{
    if(playerid < 0 || playerid >= MAX_PLAYERS)
    {
        format(dest, len, "FFFFFF");
        return 1;
    }
    new val = User[playerid][chatMainColor];
    if(1 <= val <= sizeof(hudShades))
    {
        format(dest, len, "%06x", hudShades[val - 1][shadeHex]);
    }
    else if(val & 0x10000000)
    {
        format(dest, len, "%06x", val & 0xFFFFFF);
    }
    else
    {
        GetPlayerMainHex(playerid, dest, len);
    }
    return 1;
}

stock bool:IsServerOwner(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return false;
    if(!IsPlayerLogged(playerid)) return false;

    return (strcmp(User[playerid][userName], SERVER_OWNER, true) == 0);
}

stock format_MainColor(playerid, string[], len = sizeof(string))
{
    if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
    new hex[7], tag[16];
    GetPlayerMainHex(playerid, hex);
    format(tag, sizeof(tag), "{%s}", hex);

    new pos, from, taglen = strlen(tag);
    while((pos = strfind(string, "{MAIN}", false, from)) != -1)
    {
        strdel(string, pos, pos + 6);
        strins(string, tag, pos, len);
        from = pos + taglen;
    }
    return 1;
}

stock strtodown(const string[])
{
	new lowerString[512];
	new len = strlen(string);

	if(len >= sizeof(lowerString)) len = sizeof(lowerString) - 1;

	for (new character = 0; character < len; character++)
	{
		lowerString[character] = tolower(string[character]);
	}
	lowerString[len] = EOS;
	return lowerString;
}

Float:ratio(value1, value2)
{
    if(value2 <= 0) value2 = 1;
    return float(value1) / float(value2);
}

FormatRatio(value1, value2, destination[], len = sizeof(destination))
{
    if(value1 <= 0 && value2 <= 0)
        format(destination, len, "NaN");
    else
        format(destination, len, "%.2f", ratio(value1, value2));

    return 1;
}

GetVehicleModelIDFromName(const vname[])
{
	if(isnull(vname)) return -1;

	for (new i = 0; i < sizeof(VehicleNames); i++)
	{
			if (strfind(VehicleNames[i], vname, true) != -1)
					return i + 400;
	}
	return -1;
}

GetWeaponModelIDFromName(const wname[])
{
	if(isnull(wname)) return -1;

	for (new i = 1; i < sizeof(wName); i++)
	{
			if (strfind(wName[i], wname, true) != -1)
					return i;
	}
	return -1;
}

static stock bool:IsLeapYear(year)
{
	return (((year % 4) == 0 && (year % 100) != 0) || (year % 400) == 0);
}

returnDate( timestamp, _form=0 )
{
	new year=1970, day=0, month=0, hourt=0, mins=0, sec=0;

	new days_of_month[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	new names_of_month[12][10] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
	new returnstring[64];

	if(timestamp < 0) timestamp = 0;

	new days = timestamp / 86400;
	new rest = timestamp % 86400;

	hourt = rest / 3600;
	mins  = (rest % 3600) / 60;
	sec   = rest % 60;

	for (;;)
	{
		new yearDays = IsLeapYear(year) ? 366 : 365;
		if(days < yearDays) break;

		days -= yearDays;
		year++;
	}

	if(IsLeapYear(year)) days_of_month[1] = 29;

	while(month < 11 && days >= days_of_month[month])
	{
		days -= days_of_month[month];
		month++;
	}
	day = days;

	switch( _form )
	{
		case 1: format(returnstring, sizeof(returnstring), "%02d/%02d/%d %02d:%02d:%02d", day+1, month+1, year, hourt, mins, sec);
		case 2: format(returnstring, sizeof(returnstring), "%s %02d, %d, %02d:%02d:%02d", names_of_month[month],day+1,year, hourt, mins, sec);
		case 3: format(returnstring, sizeof(returnstring), "%d %c%c%c %d, %02d:%02d", day+1,names_of_month[month][0],names_of_month[month][1],names_of_month[month][2], year,hourt,mins);
		case 4: format(returnstring, sizeof(returnstring), "%s %02d, %d", names_of_month[month],day+1,year);
		case 5: format(returnstring, sizeof(returnstring), "%02d.%02d.%d %02d:%02d:%02d", day+1, month+1, year, hourt, mins, sec);
		default: format(returnstring, sizeof(returnstring), "%02d/%02d/%d %02d:%02d:%02d", day+1, month+1, year, hourt, mins, sec);
	}

	return returnstring;
}

getPlayerIp(playerid)
{
	static ip[18];
	GetPlayerIp(playerid, ip, sizeof(ip));
	return ip;
}

convertTime(timee)
{
	new strret[32];

	if(timee < 0) timee = 0;

	if(timee < 60)
		format(strret, sizeof(strret), "%ds", timee);
	else if(timee < 3600)
		format(strret, sizeof(strret), "%dm %02ds", timee / 60, timee % 60);
	else
		format(strret, sizeof(strret), "%dh %02dm %02ds", timee / 3600, (timee % 3600) / 60, timee % 60);

	return strret;
}

fmat_return(const fmat[], va_args<>)
{
	new str[300];
	va_format(str, sizeof(str), fmat, va_start<1>);
	return str;
}

HexToInt(const str[])
{
	new ret;
	unformat(str, "x", ret);
	return ret;
}

getVehicleName(vehicleid)
{
	new vehmodel = GetVehicleModel(vehicleid);
	new nameVeh[75];

	if (vehmodel < 400 || vehmodel > 611)
	{
					strcat(nameVeh, "None");
					return nameVeh;
	}
	strcat(nameVeh, VehicleNames[vehmodel - 400]);
	return nameVeh;
}

GetVehicleSpeed(vehicleid)
{
	new Float:xPos[3];
	GetVehicleVelocity(vehicleid, xPos[0], xPos[1], xPos[2]);
	return floatround(floatsqroot(xPos[0] * xPos[0] + xPos[1] * xPos[1] + xPos[2] * xPos[2]) * 170.00);
}

GetWeekDay(day=0, month=0, year=0)
{
  if (!day)
    getdate(year, month, day);

  new
    weekday_str[30],
    j,
    e
  ;

  if (month <= 2)
  {
    month += 12;
    --year;
  }

  j = year % 100;
  e = year / 100;

  switch ((day + (month+1)*26/10 + j + j/4 + e/4 - 2*e) % 7)
  {
    case 0: weekday_str = "Saturday";
    case 1: weekday_str = "Sunday";
    case 2: weekday_str = "Monday";
    case 3: weekday_str = "Tuesday";
    case 4: weekday_str = "Wednesday";
    case 5: weekday_str = "Thursday";
    case 6: weekday_str = "Friday";
  }

  return weekday_str;
}

GetDamageStyleName(style)
{
	new name[24];
	switch(style)
	{
		case 1: name = "Numbers";
		case 2: name = "Dot (*)";
		case 3: name = "X (X)";
		default: name = "Dynamic";
	}
	return name;
}

GetOnlinePlayerIDByName(const name[])
{
	if(isnull(name)) return INVALID_PLAYER_ID;

	foreach(new i : Player)
	{
		if(User[i][isLogged] && strequal(User[i][userName], name, true))
			return i;
	}
	return INVALID_PLAYER_ID;
}

ResolvePlayerTarget(const target[])
{
	if(isnull(target)) return INVALID_PLAYER_ID;

	if(IsNumeric(target))
	{
		new id = strval(target);
		if(0 <= id < MAX_PLAYERS && IsPlayerLogged(id))
			return id;

		return INVALID_PLAYER_ID;
	}

	return GetOnlinePlayerIDByName(target);
}

RedactCommandText(dest[], const cmdtext[], size)
{
	strcopy(dest, cmdtext, size);

	if(!strcmp(dest, "/changepass", true, 11) || !strcmp(dest, "/lockserver", true, 11))
	{
		new space = strfind(dest, " ");
		if(space != -1)
		{
			dest[space + 1] = EOS;
			strcat(dest, "******", size);
		}
	}
	return 1;
}

SendBanNotice(playerid, const extraLine[] = "")
{
	SendClientMessage(playerid, -1, ban_message_color"[BANNED] You have been banned from the server!");

	if(!isnull(extraLine))
		SendClientMessage(playerid, -1, "%s", extraLine);

	SendClientMessage(playerid, -1, ban_message_color"[BANNED] If you think this ban is unfair, take a screenshot (F8) of this ban.");
	va_SendClientMessage(playerid, -1, ban_message_color"[BANNED] Visit our server community: %s and create a ticket in 'Support'.", ServerHostname);
	SendClientMessage(playerid, -1, ban_message_color"[BANNED] And wait for the Administration to review your case!");
	return 1;
}

IsAccountOnline(accountid)
{
	foreach(new i : Player)
	{
		if(User[i][user_sqlid] == accountid)
		{
			if(User[i][isLogged])
			{
				return true;
			}
		}
	}
	return false;
}

SetAlpha(colour, alpha)
{
	return colour & ~0xFF | clamp(alpha, 0, 255);
}

#define CAPS_MIN_LENGTH (6)
#define CAPS_MAX_RATIO  (70)

StripCapslock(text[])
{
	new length = strlen(text), upper = 0, letters = 0;

	if(length < CAPS_MIN_LENGTH) return 0;

	for(new i; i < length; i++)
	{
		if(!(('A' <= text[i] <= 'Z') || ('a' <= text[i] <= 'z'))) continue;

		letters++;
		if('A' <= text[i] <= 'Z') upper++;
	}

	if(letters < CAPS_MIN_LENGTH) return 0;
	if((upper * 100) / letters < CAPS_MAX_RATIO) return 0;

	for(new i = 1; i < length; i++)
		if('A' <= text[i] <= 'Z') text[i] += 32;

	return 1;
}
