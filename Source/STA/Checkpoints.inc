#if defined CHECKPOINTS_INCLUDED
#endinput
#endif
#define CHECKPOINTS_INCLUDED

#include "Boundingbox.inc"
#include "Permissions.inc"
#include "STAPlayer.inc"

#include <entity_prop_stocks>

/*
	A start checkpoint will reset the time to 0
*/
enum
{
	CP_ZoneTypeStart,
	CP_ZoneTypeNormal,
	CP_ZoneTypeEnd,
};

/*
	Possible states when CreatingCheckpoint is true
*/
enum
{
	CP_BuildState_Start,
	CP_BuildState_End,
};

enum
{
	/*
		This index is only used by us to later remove this trigger entity
	*/
	CPZone_EntID,
	
	CPZone_ZoneType,
	
	CPZone_MinX,
	CPZone_MinY,
	CPZone_MinZ,
	
	CPZone_MaxX,
	CPZone_MaxY,
	CPZone_MaxZ,
	
	CPZone_ArrayLength,
};

enum
{
	CPMenu_CreateStart,
	CPMenu_CreateNormal,
	CPMenu_CreateEnd,
	
	CPMenu_Proceed,
	
	CPMenu_LoadFromFile,
	CPMenu_SaveToFile,
	CPMenu_RemoveZones,
};

#define CP_ZoneTypeStartColor {255, 0, 255, 255}
#define CP_ZoneTypeNormalColor {155, 55, 155, 255}
#define CP_ZoneTypeEndColor {255, 0, 255, 255}

public void CP_GetZoneColor(int zonetype, int outcolor[4])
{		
	switch (zonetype)
	{
		case CP_ZoneTypeStart:
		{
			outcolor = CP_ZoneTypeStartColor;
		}
		
		case CP_ZoneTypeNormal:
		{
			outcolor = CP_ZoneTypeNormalColor;
		}
		
		case CP_ZoneTypeEnd:
		{
			outcolor = CP_ZoneTypeEndColor;
		}
	}
}

ArrayList STA_Checkpoints = null;
int STA_LaserSprite;

public bool CP_ZoneEditTraceFilter(int entity, int contentsmask, any data)
{
	if (entity == data || entity < MAXPLAYERS)
	{
		return false;
	}
	
	return true;
}

public void CP_GetPlayerForwardVector(int client, float[3] pos, float[3] forwarddir, float[3] endpos)
{
	float angle[3];
	
	GetClientEyePosition(client, pos);
	GetClientEyeAngles(client, angle);
	
	GetAngleVectors(angle, forwarddir, NULL_VECTOR, NULL_VECTOR);				
	NormalizeVector(forwarddir, forwarddir);
	ScaleVector(forwarddir, 1024.0);
	AddVectors(pos, forwarddir, endpos);
}

public void CP_ResetPlayer(int client)
{
	Player_SetZoneEditingStage(client, CP_BuildState_Start);
	Player_SetIsCreatingCheckpoint(client, false);
}

public int MenuHandler_Checkpoints(Menu menu, MenuAction action, int param1, int param2)
{
	int client = param1;
	
	if (action == MenuAction_Select)
	{
		char info[3];
		bool found = GetMenuItem(menu, param2, info, sizeof(info));
		
		if (!found)
		{
			return;
		}
		
		int itemid = StringToInt(info);
		
		switch (itemid)
		{
			case CPMenu_LoadFromFile:
			{
				CP_LoadZonesFromFile(client);
			}
			
			case CPMenu_SaveToFile:
			{
				CP_SaveZonesToFile(client);
			}
			
			case CPMenu_RemoveZones:
			{
				CP_RemoveAllCheckpoints();
				STA_OpenZoneMenu(client);
			}
			
			/*
				Just entered editing mode, no previous state
			*/
			case CPMenu_CreateStart, CPMenu_CreateNormal, CPMenu_CreateEnd:
			{
				Player_SetIsCreatingCheckpoint(client, true);
				
				int zonetype = itemid;
				
				switch (zonetype)
				{
					case CPMenu_CreateStart:
					{
						STA_PrintMessageToClient(client, "Creating zone: Start");
					}
					
					case CPMenu_CreateNormal:
					{
						STA_PrintMessageToClient(client, "Creating zone: Checkpoint");
					}
					
					case CPMenu_CreateEnd:
					{
						STA_PrintMessageToClient(client, "Creating zone: End");
					}
				}
				
				Player_SetZoneEditCreationType(client, zonetype);				
				Player_SetZoneEditingStage(client, CP_BuildState_Start);
				
				STA_OpenZoneMenu(client);
			}
			
			case CPMenu_Proceed:
			{
				int stage = Player_GetZoneEditingStage(client);
		
				switch (stage)
				{
					/*
						Start position has been set
					*/
					case CP_BuildState_Start:
					{
						Player_SetZoneEditingStage(client, CP_BuildState_End);
					}
					
					/*
						End position has been set
					*/
					case CP_BuildState_End:
					{
						float min[3];
						float max[3];
						
						Player_GetZoneEditBoxMinMax(client, min, max);
						
						any cpinfo[CPZone_ArrayLength];
						
						cpinfo[CPZone_ZoneType] = Player_GetZoneEditCreationType(client);
						
						CopyVector3ToArray(min, cpinfo, CPZone_MinX);
						CopyVector3ToArray(max, cpinfo, CPZone_MaxX);
						
						int zoneid = GetArraySize(STA_Checkpoints);
						
						PushArrayArray(STA_Checkpoints, cpinfo);
						
						CP_CreateTriggerForCheckpoint(zoneid);
						
						CP_ResetPlayer(client);
					}
				}
				
				STA_OpenZoneMenu(client);
			}
		}
	}

	else if (action == MenuAction_Cancel)
	{
		CP_ResetPlayer(client);
	}
	
	else if (action == MenuAction_End)
	{		
		delete menu;
	}
}

public void STA_OpenZoneMenu(int client)
{
	//Player_PrintEditInfo(client);
	
	bool onteam = IsPlayingOnTeam(client);
	
	if (!onteam)
	{
		STA_PrintMessageToClient(client, "You must be in a team to manage checkpoints");
		return;
	}
	
	Menu menu = CreateMenu(MenuHandler_Checkpoints);
	SetMenuTitle(menu, "STA Checkpoints");
	
	if (!Player_GetIsCreatingCheckpoint(client))
	{
		Menu_AddEnumEntry(menu, CPMenu_CreateStart, "Create start");
		Menu_AddEnumEntry(menu, CPMenu_CreateNormal, "Create checkpoint");
		Menu_AddEnumEntry(menu, CPMenu_CreateEnd, "Create end");
		
		Menu_AddEnumEntry(menu, CPMenu_LoadFromFile, "Load checkpoints (override)");
		
		if (GetArraySize(STA_Checkpoints) > 0)
		{
			Menu_AddEnumEntry(menu, CPMenu_SaveToFile, "Save checkpoints");
			Menu_AddEnumEntry(menu, CPMenu_RemoveZones, "Remove checkpoints");
		}
	}
	
	else
	{
		int stage = Player_GetZoneEditingStage(client);
		
		switch (stage)
		{
			case CP_BuildState_Start:
			{
				Menu_AddEnumEntry(menu, CPMenu_Proceed, "Set point 1");
			}
			
			case CP_BuildState_End:
			{
				Menu_AddEnumEntry(menu, CPMenu_Proceed, "Set point 2");
			}
		}
	}
	
	DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public Action STA_ZoneEdit(int client, int args)
{
	if (!HandlePlayerPermission(client))
	{
		return Plugin_Handled;
	}
	
	STA_OpenZoneMenu(client);
	return Plugin_Handled;
}

public Action STA_ZoneGridSize(int client, int args)
{
	if (args == 2)
	{
		char newgridbuf[3];
		GetCmdArg(1, newgridbuf, sizeof(newgridbuf));
		
		int newgridsize;
		
		newgridsize = StringToInt(newgridbuf);
		
		Player_SetZoneGridSize(client, newgridsize);
	}
	
	return Plugin_Handled;
}

public void CP_RemoveAllCheckpoints()
{
	if (STA_Checkpoints == null)
	{
		return;
	}
	
	int arraysize = GetArraySize(STA_Checkpoints);
	
	for (int i = 0; i < arraysize; ++i)
	{
		int entid = GetArrayCell(STA_Checkpoints, i, CPZone_EntID);
		
		if (entid == -1)
		{
			continue;
		}
		
		RemoveEdict(entid);
	}
	
	ClearArray(STA_Checkpoints);
}

/*
	Triggers are required to have a model
*/
#define CP_Model "models/error.mdl"

public void CP_MapStartInit()
{
	PrecacheModel(CP_Model);

	Handle gameconfig = LoadGameConfigFile("funcommands.games");
	if (!gameconfig)
	{
		SetFailState("funcommands.games.txt not found.");
		return;
	}

	char spritebeam[PLATFORM_MAX_PATH];
	if (!GameConfGetKeyValue(gameconfig, "SpriteBeam", spritebeam, sizeof(spritebeam)) || !spritebeam[0])
	{
		SetFailState("SpriteBeam key value not found.");
		return;
	}

	STA_LaserSprite = PrecacheModel(spritebeam, true);
	
	CreateTimer(1.0, RenewZoneBoxes, _, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
	
	CP_RemoveAllCheckpoints();
	
	STA_Checkpoints = CreateArray(CPZone_ArrayLength);
}

public void CP_OnMapEnd()
{
	CP_RemoveAllCheckpoints();
	delete STA_Checkpoints;
}

public void CP_Init()
{	
	RegConsoleCmd("sm_zones", STA_ZoneEdit);
	RegConsoleCmd("sm_zone_gridsize", STA_ZoneGridSize);
}

public void CP_UpdateZoneEditBox(int client)
{
	float min[3];
	float max[3];
	float points[8][3];
	
	Player_GetZoneEditBoxMinMax(client, min, max);
	STA_PointsFromBox(min, max, points);
	
	int color[4];
	int zonetype = Player_GetZoneEditCreationType(client);
	CP_GetZoneColor(zonetype, color);
	
	STA_DisplayBox(client, STA_LaserSprite, points, 0.1, color);
}

public Action RenewZoneBoxes(Handle timer, any client)
{
	int arraysize = GetArraySize(STA_Checkpoints);
	
	for (int i = 0; i < arraysize; ++i)
	{
		int zonetype;
		float min[3];
		float max[3];
		
		float points[8][3];
		
		any cpinfo[CPZone_ArrayLength];
		GetArrayArray(STA_Checkpoints, i, cpinfo);
		
		zonetype = cpinfo[CPZone_ZoneType];

		GetArrayVector3(cpinfo, CPZone_MinX, min);
		GetArrayVector3(cpinfo, CPZone_MaxX, max);
		
		STA_PointsFromBox(min, max, points);
		
		int color[4];
		CP_GetZoneColor(zonetype, color);
		
		STA_DisplayBoxAll(STA_LaserSprite, points, 1.0, color);
	}
	
	return Plugin_Continue;
}

public bool CP_IsNormalFlat(float[3] tracenormal)
{
	for (int i = 0; i < 3; ++i)
	{
		if (tracenormal[i] != -1.0 || tracenormal[i] != 0.0 || tracenormal[i] != 1.0)
		{
			return false;
		}
	}
	
	return true;
}

public void CP_Update(int client)
{
	int stage = Player_GetZoneEditingStage(client);
	
	switch (stage)
	{
		case CP_BuildState_Start, CP_BuildState_End:
		{			
			float pos[3];
			GetClientAbsOrigin(client, pos);
			
			SnapVectorTo(pos, Player_GetZoneGridSize(client));
			
			if (stage == CP_BuildState_Start)
			{
				Player_SetZoneEditStartPos(client, pos);
			}
			
			else if (stage == CP_BuildState_End)
			{					
				Player_SetZoneEditEndPos(client, pos);
				CP_UpdateZoneEditBox(client);
			}
		}
	}
}

public int MenuHandler_ZoneSelect(Menu menu, MenuAction action, int param1, int param2)
{
	int client = param1;

	if (action == MenuAction_Select)
	{
		/*
			"info" is the filename including extension
		*/
		char info[512];
		bool found = GetMenuItem(menu, param2, info, sizeof(info));
		
		if (!found)
		{
			return;
		}
		
		char mapbuf[MAX_NAME_LENGTH];
		GetCurrentMap(mapbuf, sizeof(mapbuf));
		
		char filepath[PLATFORM_MAX_PATH];
		BuildPath(Path_SM, filepath, sizeof(filepath), "%s/%s/%s/%s", STA_RootPath, STA_ZoneFolder, mapbuf, info);
		
		File file = OpenFile(filepath, "rb");
		
		if (file == null)
		{
			STA_PrintMessageToClient(client, "\"%s\" could not be opened", info);
			return;
		}
		
		CP_ResetPlayer(client);
		
		CP_RemoveAllCheckpoints();
		
		int arraysize;
		ReadFileCell(file, arraysize, 4);
		
		any cpinfo[CPZone_ArrayLength];
		cpinfo[CPZone_EntID] = -1;
			
		for (int i = 0; i < arraysize; ++i)
		{			
			for (int j = CPZone_ZoneType; j < CPZone_ArrayLength; ++j)
			{
				ReadFileCell(file, cpinfo[j], 4);
			}
			
			PushArrayArray(STA_Checkpoints, cpinfo);
		}
		
		delete file;
		
		for (int i = 0; i < arraysize; ++i)
		{
			CP_CreateTriggerForCheckpoint(i);
		}
		
		STA_PrintMessageToClient(client, "Loaded zones \"%s\"", info);
		
		STA_OpenZoneMenu(client);
	}
	
	else if (action == MenuAction_Cancel)
	{
		
	}
	
	else if (action == MenuAction_End)
	{		
		delete menu;
	}
}

public void CP_LoadZonesFromFile(int client)
{	
	char mapbuf[MAX_NAME_LENGTH];
	GetCurrentMap(mapbuf, sizeof(mapbuf));
	
	char mapzonebuf[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, mapzonebuf, sizeof(mapzonebuf), "%s/%s/%s", STA_RootPath, STA_ZoneFolder, mapbuf);
	
	if (!DirExists(mapzonebuf))
	{
		STA_PrintMessageToClient(client, "No zones available for \"%s\"", mapbuf);
		return;
	}
	
	DirectoryListing dirlist = OpenDirectory(mapzonebuf);
	
	if (dirlist == null)
	{
		STA_PrintMessageToClient(client, "Could not open STA directory path");
		return;
	}
	
	FileType curtype;
	char curname[512];
	int index = 0;
	
	Menu selectmenu = CreateMenu(MenuHandler_ZoneSelect);
	SetMenuTitle(selectmenu, "Zone File Select");
	
	while (dirlist.GetNext(curname, sizeof(curname), curtype))
	{
		if (curtype != FileType_File)
			continue;

		AddMenuItem(selectmenu, curname, curname);
		index++;
	}

	delete dirlist;

	if (index == 0)
	{
		STA_PrintMessageToClient(client, "No zones available");
		return;
	}
	
	DisplayMenu(selectmenu, client, MENU_TIME_FOREVER);
}

public void CP_SaveZonesToFile(int client)
{
	char mapbuf[MAX_NAME_LENGTH];
	GetCurrentMap(mapbuf, sizeof(mapbuf));
	
	char playernamebuf[MAX_NAME_LENGTH];
	GetClientName(client, playernamebuf, sizeof(playernamebuf));
	
	char newdirbuf[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, newdirbuf, sizeof(newdirbuf), "%s/%s/%s", STA_RootPath, STA_ZoneFolder, mapbuf);
				
	if (!DirExists(newdirbuf))
		CreateDirectory(newdirbuf, 511);
	
	int steamid = GetSteamAccountID(client);
	
	char timebuf[128];
	FormatTime(timebuf, sizeof(timebuf), "%Y %m %d, %H %M %S");
	
	char namebuf[256];
	FormatEx(namebuf, sizeof(namebuf), "[%d] %s (%s)", steamid, playernamebuf, timebuf);
	
	char filename[PLATFORM_MAX_PATH];
	FormatEx(filename, sizeof(filename), "%s/%s.STA", newdirbuf, namebuf);
	
	File file = OpenFile(filename, "wb");
	
	if (file == null)
	{
		STA_PrintMessageToClient(client, "Could not save zones");
		return;
	}
	
	int arraysize = GetArraySize(STA_Checkpoints);
	WriteFileCell(file, arraysize, 4);
	
	any cpinfo[CPZone_ArrayLength];
	
	for (int i = 0; i < arraysize; ++i)
	{
		GetArrayArray(STA_Checkpoints, i, cpinfo);
		
		for (int j = CPZone_ZoneType; j < CPZone_ArrayLength; ++j)
		{
			WriteFileCell(file, cpinfo[j], 4);
		}
	}
	
	delete file;
	
	STA_PrintMessageToClient(client, "Saved as \"%s\"", namebuf);
}

public bool CP_CanPassHookTest(int client)
{
	if (!IsPlayerAlive(client))
	{
		//PrintToServer("alive");
		return false;
	}
	
	if (!IsFakeClient(client))
	{
		//PrintToServer("not a bot");
		return false;
	}
	
	//PrintToServer("ok");
	return true;
}

public bool CP_CanPassHookSayTest(int client)
{	
	return true;
}

public void CP_StartTouch(int entity, int client)
{
	if (!CP_CanPassHookTest(client))
	{
		return;
	}
	
	bool isfake = false;
	int fakeid = 0;
	
	if (IsFakeClient(client))
	{
		isfake = true;
		fakeid = client;
		
		client = Bot_GetLinkedPlayerIndex(client);
	}
	
	int zonetype;
	int zoneid;
	int unused2;
	int unused3;
	Offsets_GetEntityRenderColor(entity, zonetype, zoneid, unused2, unused3);
	
	bool timing = Player_GetIsTimingRun(client);
	
	if (timing)
	{
		if (zonetype == CP_ZoneTypeNormal || zonetype == CP_ZoneTypeEnd)
		{
			char message[256];
			
			/*
			float xyvel;
			float zvel;
			
			if (isfake)
			{
				ClientVelocityLength(fakeid, xyvel, zvel);
			}
			
			else
			{
				ClientVelocityLength(client, xyvel, zvel);
			}
			
			FormatEx(message, sizeof(message), STA_ZoneSpeedEnterFormat, xyvel, zvel);
			
			if (isfake)
			{
				char fakenamebuf[MAX_NAME_LENGTH];
				GetClientName(fakeid, fakenamebuf, sizeof(fakenamebuf));
				
				CPrintToChat(client, "%s{lightblue}* %s * %s", STA_MessagePrefix, fakenamebuf, message);
			}
			
			else
			{
				CPrintToChat(client, "%s%s", STA_MessagePrefix, message);
			}
			*/
			
			float tickinterval = GetTickInterval();
			int curframe = Player_GetRewindFrame(client) - Player_GetStartTimeReplayTick(client);
			
			float curtime = curframe * tickinterval;
			
			char curtimebuf[64];
			FormatTimeSpan(curtimebuf, sizeof(curtimebuf), curtime);
			
			if (zonetype == CP_ZoneTypeNormal)
			{
				if (IsSource2009())
				{
					FormatEx(message, sizeof(message), STA_CheckpointFormat, zoneid, curtimebuf);
				}

				else
				{
					FormatEx(message, sizeof(message), STA_CSGOCheckpointFormat, zoneid, curtimebuf);
				}
				
				if (isfake)
				{
					STA_PrintMessageToClientSpectators(fakeid, message);
				}
				
				else
				{
					STA_PrintMessageToClient(client, message);
				}
			}
			
			else
			{
				char namebuf[MAX_NAME_LENGTH];				
				GetClientName(fakeid, namebuf, sizeof(namebuf));
				
				if (IsSource2009())
				{
					FormatEx(message, sizeof(message), STA_EndFinishFormat, namebuf, curtimebuf);
				}

				else
				{
					FormatEx(message, sizeof(message), STA_CSGOEndFinishFormat, namebuf, curtimebuf);
				}
				
				STA_PrintMessageToAllClients(message);
				
				Player_SetIsTimingRun(client, false);
			}
		}
		
		else if (zonetype == CP_ZoneTypeStart)
		{
			Player_SetIsTimingRun(client, false);
		}
	}
	
	//PrintToServer("enter to type %d", zonetype);
}

public void CP_EndTouch(int entity, int client)
{
	if (!CP_CanPassHookTest(client))
	{
		return;
	}
	
	/*bool isfake = false;
	int fakeid = 0;*/
	
	if (IsFakeClient(client))
	{
		/*isfake = true;
		fakeid = client;*/
		
		client = Bot_GetLinkedPlayerIndex(client);
	}
	
	int zonetype;
	int zoneid;
	int unused2;
	int unused3;
	Offsets_GetEntityRenderColor(entity, zonetype, zoneid, unused2, unused3);
	
	bool timing = Player_GetIsTimingRun(client);
	
	if (zonetype == CP_ZoneTypeStart)
	{
		if (!timing)
		{
			Player_SetStartTimeReplayTick(client, Player_GetRewindFrame(client));
			Player_SetIsTimingRun(client, true);
		}
	}
	
	/*
	float xyvel;
	float zvel;
	
	if (isfake)
	{
		ClientVelocityLength(fakeid, xyvel, zvel);
	}
	
	else
	{
		ClientVelocityLength(client, xyvel, zvel);
	}
	
	char message[256];
	FormatEx(message, sizeof(message), STA_ZoneSpeedLeaveFormat, xyvel, zvel);
	
	if (isfake)
	{
		char fakenamebuf[MAX_NAME_LENGTH];
		GetClientName(fakeid, fakenamebuf, sizeof(fakenamebuf));
		
		CPrintToChat(client, "%s{lightblue}* %s * %s", STA_MessagePrefix, fakenamebuf, message);
	}
	
	else
	{
		CPrintToChat(client, "%s%s", STA_MessagePrefix, message);
	}
	*/
	//PrintToServer("leave from type %d", zonetype);
}

public void CP_CreateTriggerForCheckpoint(int checkpoint)
{
	int newent = CreateEntityByName("trigger_multiple");
	
	if (!IsValidEntity(newent))
	{
		return;
	}
	
	any cpinfo[CPZone_ArrayLength];
	GetArrayArray(STA_Checkpoints, checkpoint, cpinfo);
	
	SetArrayCell(STA_Checkpoints, checkpoint, newent, CPZone_EntID);
	
	float min[3];
	float max[3];	
	//Player_GetZoneEditBoxMinMax(client, min, max);
	GetArrayVector3(cpinfo, CPZone_MinX, min);
	GetArrayVector3(cpinfo, CPZone_MaxX, max);
	
	float origin[3];
	STA_GetBoxOrigin(min, max, origin);
	
	float localmin[3];
	float localmax[3];
	STA_GetBoxSize(min, max, origin, localmin, localmax);
	
	char zonename[MAX_TARGET_LENGTH];
	FormatEx(zonename, sizeof(zonename), "sta_zone_%d", checkpoint);
	
	/*
		Flag 1 = Only clients
	*/
	DispatchKeyValue(newent, "spawnflags", "1");
	DispatchKeyValue(newent, "StartDisabled", "0");
	DispatchKeyValue(newent, "targetname", zonename);
	DispatchKeyValue(newent, "wait", "0");
	DispatchKeyValue(newent, "model", CP_Model);
	
	int zonetype = cpinfo[CPZone_ZoneType];
	
	/*
		We store our trigger data in render color to easily get its userdata in touch/leave hooks 
	*/
	SetEntityRenderColor(newent, zonetype, checkpoint);
	
	DispatchSpawn(newent);
	ActivateEntity(newent);
	
	/*
		All these settings can only be changed after the entity is spawned
	*/
	DispatchKeyValueVector(newent, "origin", origin);
	DispatchKeyValueVector(newent, "mins", localmin);
	DispatchKeyValueVector(newent, "maxs", localmax);
	
	/*
		Flag 2 = SOLID_BBOX 
	*/
	DispatchKeyValue(newent, "solid", "2");
	
	/*
		Flag 32 = EF_NODRAW
	*/
	SetEntData(newent, Offsets_Effects, GetEntData(newent, Offsets_Effects) | 32);
	
	SDKHookEx(newent, SDKHook_StartTouch, CP_StartTouch);
	SDKHookEx(newent, SDKHook_EndTouch, CP_EndTouch);
}

public void STA_GetCheckpointBoundingBox(int checkpoint, float[3] min, float[3] max)
{
	min[0] = GetArrayCell(STA_Checkpoints, checkpoint, CPZone_MinX);
	min[1] = GetArrayCell(STA_Checkpoints, checkpoint, CPZone_MinY);
	min[2] = GetArrayCell(STA_Checkpoints, checkpoint, CPZone_MinZ);
	
	max[0] = GetArrayCell(STA_Checkpoints, checkpoint, CPZone_MaxX);
	max[1] = GetArrayCell(STA_Checkpoints, checkpoint, CPZone_MaxY);
	max[2] = GetArrayCell(STA_Checkpoints, checkpoint, CPZone_MaxZ);
}

public void STA_DrawCheckpoint(int checkpoint)
{
	float points[8][3];
	STA_PointsFromCheckpoint(checkpoint, points);	
}

public void STA_PointsFromCheckpoint(int checkpoint, float[8][3] points)
{
	float mins[3];
	float maxs[3];
	STA_GetCheckpointBoundingBox(checkpoint, mins, maxs);
	
	STA_PointsFromBox(mins, maxs, points);
}
