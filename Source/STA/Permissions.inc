#if defined PERMISSIONS_INCLUDED
#endinput
#endif
#define PERMISSIONS_INCLUDED

#include "Formats.inc"

public bool PlayerHasPermissionToUse(int client)
{
	AdminId adminid = GetUserAdmin(client);	
	return GetAdminFlag(adminid, Admin_Generic);
}

public bool PlayerHasPermisionToManage(int client)
{
	AdminId adminid = GetUserAdmin(client);	
	return GetAdminFlag(adminid, Admin_Root);
}

public bool HandlePlayerPermission(int client)
{
	if (!PlayerHasPermissionToUse(client))
	{
		STA_PrintMessageToClient(client, "You do not have permission to use STA");
		return false;
	}
	
	return true;
}

public bool HandleManagePermission(int client)
{
	if (!PlayerHasPermisionToManage(client))
	{
		STA_PrintMessageToClient(client, "You do not have permission to manage STA");
		return false;
	}
	
	return true;
}
