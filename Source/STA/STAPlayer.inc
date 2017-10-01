#if defined STAPLAYER_INCLUDED
#endinput
#endif
#define STAPLAYER_INCLUDED

#include "Formats.inc"
#include "Vector.inc"
#include "ReplayFrame.inc"

/*
	==============================================================
*/

/*
	When a player gets moved to spectator, this saves their previous team
*/
int PreferredTeam[MAXPLAYERS + 1];

public int Player_GetPreferredTeam(int client)
{
	return PreferredTeam[client];
}

public void Player_SetPreferredTeam(int client, int value)
{
	PreferredTeam[client] = value;
}

/*
	==============================================================
*/

bool IsSegmenting[MAXPLAYERS + 1];

public bool Player_GetIsSegmenting(int client)
{
	return IsSegmenting[client];
}

public void Player_SetIsSegmenting(int client, bool value)
{
	IsSegmenting[client] = value;
}

/*
	==============================================================
*/

int LastPausedTick[MAXPLAYERS + 1];

public int Player_GetLastPausedTick(int client)
{
	return LastPausedTick[client];
}

public void Player_SetLastPausedTick(int client, int value)
{
	LastPausedTick[client] = value;
}

/*
	==============================================================
*/

bool IsRewinding[MAXPLAYERS + 1];

public bool Player_GetIsRewinding(int client)
{
	return IsRewinding[client];
}

public void Player_SetIsRewinding(int client, bool value)
{
	IsRewinding[client] = value;
}

/*
	==============================================================
*/

bool HasRun[MAXPLAYERS + 1];

public bool Player_GetHasRun(int client)
{
	return HasRun[client];
}

public void Player_SetHasRun(int client, bool value)
{
	HasRun[client] = value;
}

/*
	==============================================================
*/

bool IsPlayingReplay[MAXPLAYERS + 1];

public bool Player_GetIsPlayingReplay(int client)
{
	return IsPlayingReplay[client];
}

public void Player_SetPlayingReplay(int client, bool value)
{
	IsPlayingReplay[client] = value;
}

/*
	==============================================================
*/

int RewindSpeed[MAXPLAYERS + 1] = {1, ...};

public void Player_SetRewindSpeed(int client, int value)
{
	RewindSpeed[client] = value;
}

public int Player_GetRewindSpeed(int client)
{
	return RewindSpeed[client];
}

/*
	==============================================================
*/

ArrayList RecordFramesList[MAXPLAYERS + 1];

public void Player_CreateFrameArray(int client)
{
	RecordFramesList[client] = CreateArray(FRAME_Length);
}

public Handle Player_GetRecordFramesList(int client)
{
	return RecordFramesList[client];
}

public void Player_PushFrame(int client, any[] frameinfo)
{
	PushArrayArray(RecordFramesList[client], frameinfo);
}

public void Player_GetFrame(int client, int index, any[] frameinfo)
{
	GetArrayArray(RecordFramesList[client], index, frameinfo);
}

public int Player_GetRecordedFramesCount(int client)
{
	return GetArraySize(RecordFramesList[client]);
}

public void Player_ResizeRecordFrameList(int client, int newsize)
{
	ResizeArray(RecordFramesList[client], newsize);
}

public void Player_DeleteRecordFrames(int client)
{	
	if (RecordFramesList[client] != null)
	{
		ClearArray(RecordFramesList[client]);
		delete RecordFramesList[client];
	}
}

/*
	==============================================================
*/

int CurrentRewindFrame[MAXPLAYERS + 1];

public int Player_GetRewindFrame(int client)
{
	return CurrentRewindFrame[client];
}

public void Player_SetRewindFrame(int client, int value)
{
	CurrentRewindFrame[client] = value;
}

public void Player_IncrementRewindFrame(int client)
{
	++CurrentRewindFrame[client];
}

public void Player_DecrementRewindFrame(int client)
{
	--CurrentRewindFrame[client];
}

/*
	==============================================================
*/

int PlayerLinkedBotIndex[MAXPLAYERS + 1];

public int Player_GetLinkedBotIndex(int client)
{
	return PlayerLinkedBotIndex[client];
}

public void Player_SetLinkedBotIndex(int client, int value)
{
	PlayerLinkedBotIndex[client] = value;
}

/*
	==============================================================
*/

int BotLinkedPlayerIndex[MAXPLAYERS + 1];

public int Bot_GetLinkedPlayerIndex(int client)
{
	return BotLinkedPlayerIndex[client];
}

public void Bot_SetLinkedPlayerIndex(int client, int value)
{
	BotLinkedPlayerIndex[client] = value;
}

/*
	==============================================================
*/

/*
	Threshold value to which zone editing snaps to
*/
int ZoneGridSize[MAXPLAYERS + 1] = {8, ...};

public int Player_GetZoneGridSize(int client)
{
	return ZoneGridSize[client];
}

public void Player_SetZoneGridSize(int client, int value)
{
	ZoneGridSize[client] = value;
}

/*
	==============================================================
*/

bool IsCreatingCheckpoint[MAXPLAYERS + 1];

public bool Player_GetIsCreatingCheckpoint(int client)
{
	return IsCreatingCheckpoint[client];
}

public void Player_SetIsCreatingCheckpoint(int client, bool value)
{
	IsCreatingCheckpoint[client] = value;
}

/*
	==============================================================
*/

/*
	See CP_BuildState in Checkpoints.inc for possible values
	
	CreatingCheckpoint gets set to false after the height stage is complete
*/
int ZoneEditingStage[MAXPLAYERS + 1];

public int Player_GetZoneEditingStage(int client)
{
	return ZoneEditingStage[client];
}

public void Player_SetZoneEditingStage(int client, int value)
{
	ZoneEditingStage[client] = value;
}

/*
	==============================================================
*/

float ZoneEditStartPos[MAXPLAYERS + 1][3];

public void Player_GetZoneEditStartPos(int client, float[3] pos)
{
	pos = ZoneEditStartPos[client];
}

public void Player_SetZoneEditStartPos(int client, float[3] value)
{
	ZoneEditStartPos[client] = value;
}

/*
	==============================================================
*/

float ZoneEditEndPos[MAXPLAYERS + 1][3];

public void Player_GetZoneEditEndPos(int client, float[3] pos)
{
	pos = ZoneEditEndPos[client];
}

public void Player_SetZoneEditEndPos(int client, float[3] value)
{
	ZoneEditEndPos[client] = value;
}

/*
	==============================================================
*/

public void Player_GetZoneEditBoxMinMax(int client, float[3] min, float[3] max)
{
	float startpos[3];
	float endpos[3];
	Player_GetZoneEditStartPos(client, startpos);
	Player_GetZoneEditEndPos(client, endpos);
	
	MinVector(startpos, endpos, min);
	MaxVector(startpos, endpos, max);
}

/*
	==============================================================
*/

/*
	What type of zone is being created if IsCreatingCheckpoint is true
*/
int ZoneEditCreationType[MAXPLAYERS + 1];

public int Player_GetZoneEditCreationType(int client)
{
	return ZoneEditCreationType[client];
}

public void Player_SetZoneEditCreationType(int client, int value)
{
	ZoneEditCreationType[client] = value;
}

/*
	==============================================================
*/

bool IsTimingRun[MAXPLAYERS + 1];

public bool Player_GetIsTimingRun(int client)
{
	return IsTimingRun[client];
}

public void Player_SetIsTimingRun(int client, bool value)
{
	IsTimingRun[client] = value;
}

/*
	==============================================================
*/

int StartTimeReplayTick[MAXPLAYERS + 1];

public int Player_GetStartTimeReplayTick(int client)
{
	return StartTimeReplayTick[client];
}

public void Player_SetStartTimeReplayTick(int client, int value)
{
	StartTimeReplayTick[client] = value;
}

/*
	==============================================================
*/

bool HasFastForwardKeyDown[MAXPLAYERS + 1];

public bool Player_GetHasFastForwardKeyDown(int client)
{
	return HasFastForwardKeyDown[client];
}

public void Player_SetHasFastForwardKeyDown(int client, bool value)
{
	HasFastForwardKeyDown[client] = value;
}

/*
	==============================================================
*/

bool HasRewindKeyDown[MAXPLAYERS + 1];

public bool Player_GetHasRewindKeyDown(int client)
{
	return HasRewindKeyDown[client];
}

public void Player_SetHasRewindKeyDown(int client, bool value)
{
	HasRewindKeyDown[client] = value;
}

/*
	==============================================================
*/

public int Player_ClampRecordFrame(int client, int frame)
{
	int count = Player_GetRecordedFramesCount(client);
	
	if (frame < 0)
	{
		frame = 0;
	}
	
	else if (frame >= count)
	{
		frame = count - 1;
	}
	
	return frame;
}

public void Player_PrintInfo(int client)
{
	PrintToChat(client, "============= Player_PrintInfo");
	PrintToChat(client, "IsSegmenting = %d", IsSegmenting[client]);
	PrintToChat(client, "IsRewinding = %d", IsRewinding[client]);
	PrintToChat(client, "HasRun = %d", HasRun[client]);
	PrintToChat(client, "PlayingReplay = %d", IsPlayingReplay[client]);
	PrintToChat(client, "CurrentRewindFrame = %d", CurrentRewindFrame[client]);
	PrintToChat(client, "PlayerLinkedBotIndex = %d", PlayerLinkedBotIndex[client]);
}

public void Player_PrintEditInfo(int client)
{
	PrintToChat(client, "============= Player_PrintEditInfo");
	PrintToChat(client, "CreatingCheckpoint = %d", IsCreatingCheckpoint[client]);
	PrintToChat(client, "ZoneGridSize = %d", ZoneGridSize[client]);
	PrintToChat(client, "ZoneEditingStage = %d", ZoneEditingStage[client]);
}

/*
	==============================================================
*/

public const int SPECMODE_NONE = 0;
public const int SPECMODE_FIRSTPERSON = 4;
public const int SPECMODE_THIRDPERSON = 5;
public const int SPECMODE_FREELOOK = 6;

public bool Player_IsSpectating(int client)
{
	return IsClientInGame(client) && GetClientTeam(client) == CS_TEAM_SPECTATOR;
}

public int Player_GetSpectateTarget(int client)
{
	if (!Player_IsSpectating(client))
	{
		return -1;
	}
	
	return GetEntDataEnt2(client, Offset_ObserverTarget);
}

public int Player_GetSpectateMode(int client)
{
	if (!Player_IsSpectating(client))
	{
		return -1;
	}
	
	return GetEntData(client, Offset_ObserverMode);
}

public int Player_GetSpectateList(int client, int[] outlist)
{
	int ret = 0;
	
	for (int i = 1; i < MaxClients + 1; ++i)
	{
		if (Player_IsSpectating(i))
		{
			int mode = Player_GetSpectateMode(i);
			
			/*
				There is an intermediate state between thirdperson and freemode
				but no need to support that I guess
			*/
			if (mode == SPECMODE_THIRDPERSON || mode == SPECMODE_FIRSTPERSON)
			{
				int target = Player_GetSpectateTarget(i);
				
				if (target == client)
				{
					outlist[ret] = i;
					++ret;
				}
			}
		}
	}
	
	return ret;
}

/*
	Adds the prefix and then adds the desired format
*/
public void STA_PrintMessageToAllClients(const char[] format, any ...)
{
	char message[255];
	message[0] = 0;
	
	char outmessage[255];
	
	if (IsSource2009())
	{
		StrCat(message, sizeof(message), STA_MessagePrefix);
	}
	
	else
	{
		StrCat(message, sizeof(message), STA_CSGOMessagePrefix);
	}

	StrCat(message, sizeof(message), format);	
	VFormat(outmessage, sizeof(outmessage), message, 2);
	
	CPrintToChatAll(outmessage);
}

/*
	Adds the prefix and then adds the desired format
*/
public void STA_PrintMessageToClient(int client, const char[] format, any ...)
{
	char message[255];
	message[0] = 0;
	
	char outmessage[255];
	
	if (IsSource2009())
	{
		StrCat(message, sizeof(message), STA_MessagePrefix);
	}

	else
	{
		StrCat(message, sizeof(message), STA_CSGOMessagePrefix);
	}

	StrCat(message, sizeof(message), format);	
	VFormat(outmessage, sizeof(outmessage), message, 3);
	
	CPrintToChat(client, outmessage);
}

/*
	Adds the prefix and then adds the desired format
*/
public void STA_PrintMessageToClientSpectators(int client, const char[] format, any ...)
{
	char message[255];
	message[0] = 0;
	
	char outmessage[255];
	
	char clientname[MAX_NAME_LENGTH];
	GetClientName(client, clientname, sizeof(clientname));
	
	if (IsSource2009())
	{
		StrCat(message, sizeof(message), STA_MessagePrefix);
		StrCat(message, sizeof(message), " {lightblue}* ");
	}

	else
	{
		StrCat(message, sizeof(message), STA_CSGOMessagePrefix);
		StrCat(message, sizeof(message), " {bluegrey}* ");
	}
	
	StrCat(message, sizeof(message), clientname);

	if (IsSource2009())
	{
		StrCat(message, sizeof(message), " * {ghostwhite}- ");
	}

	else
	{
		StrCat(message, sizeof(message), " * {default}- ");
	}

	StrCat(message, sizeof(message), format);
	VFormat(outmessage, sizeof(outmessage), message, 3);
	
	int specs[MAXPLAYERS + 1];
	int count = Player_GetSpectateList(client, specs);
	
	for (int i = 0; i < count; ++i)
	{
		CPrintToChat(specs[i], message);
	}
}
