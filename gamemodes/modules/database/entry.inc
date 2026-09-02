#define MYSQL_HOST "localhost"
#define MYSQL_USER "rejecting.files"
#define MYSQL_PASS "dbeb1766e619452f7f36d2ca84e59f7b"
#define MYSQL_DB "rejecting.files"

IsValidPassword(const password[])
{
	new len = strlen(password);
	if(len < MIN_PASSWORD_LENGTH || len > MAX_PASSWORD_LENGTH) return 0;

	for(new i = 0; password[i] != EOS; ++i)
	{
		switch(password[i])
		{
			case '0'..'9', 'A'..'Z', 'a'..'z': continue;
			default: return 0;
		}
	}
	return 1;
}

initDatabase()
{
	dbHandle = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DB);

	if(dbHandle == MYSQL_INVALID_HANDLE || mysql_errno() != 0)
	{
		print("MySQL connection failed.");
		SendRconCommand("exit");
		return true;
	}

	print("MySQL connection success.");

	mysql_tquery(dbHandle, "UPDATE `accounts` SET `is_online` = 0");
	deleteDuplicateRows();
	initializeServer(SERVER_MODE);
	return true;
}

forward deleteDuplicateRows();
public deleteDuplicateRows()
{
	mysql_tquery(dbHandle, "DELETE FROM `banimentos` WHERE `unban_timestamp` > 0 AND `unban_timestamp` <= UNIX_TIMESTAMP()");
	mysql_tquery(dbHandle, "DELETE B1 FROM `banimentos` AS B1 INNER JOIN `banimentos` AS B2 WHERE B1.id < B2.id AND B1.username = B2.username");
	mysql_tquery(dbHandle, "DELETE S1 FROM `ispsbanidas` AS S1 INNER JOIN `ispsbanidas` AS S2 WHERE S1.id > S2.id AND S1.isp = S2.isp");
	return true;
}

forward MySQL:GetDatabaseHandle();
public MySQL:GetDatabaseHandle()
{
	return dbHandle;
}

#include "modules/database/admin.inc"
#include "modules/database/discord.inc"
#include "modules/database/user.inc"
#include "modules/database/zones.inc"
