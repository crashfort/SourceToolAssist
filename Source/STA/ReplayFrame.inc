#if defined REPLAYFRAME_INCLUDED
#endinput
#endif
#define REPLAYFRAME_INCLUDED

/*
	Number of entries in a frame array

	0: Buttons
	1: Pos X
	2: Pos Y
	3: Pos Z
	4: Angle X
	5: Angle Y
	6: Vel X
	7: Vel Y
	8: Vel Z
*/

enum
{	
	FRAME_Buttons,
	FRAME_PosX,
	FRAME_PosY,
	FRAME_PosZ,
	FRAME_AngX,
	FRAME_AngY,
	FRAME_VelX,
	FRAME_VelY,
	FRAME_VelZ,
	
	FRAME_Length,
};
