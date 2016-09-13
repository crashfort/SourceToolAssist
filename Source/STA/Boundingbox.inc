#if defined BOUNDINGBOX_INCLUDED
#endinput
#endif
#define BOUNDINGBOX_INCLUDED

#include "Offsets.inc"

/*
	    3---7
	   /|  /|
	  / | / |
	 2---6  |
	 |  1|--5
	 | / | /
	 |/  |/
	 0---4
	
	From Source 2007
*/
public void STA_PointsFromBox(float[3] mins, float[3] maxs, float[8][3] points)
{	
	points[0][0] = mins[0];
	points[0][1] = mins[1];
	points[0][2] = mins[2];

	points[1][0] = mins[0];
	points[1][1] = mins[1];
	points[1][2] = maxs[2];

	points[2][0] = mins[0];
	points[2][1] = maxs[1];
	points[2][2] = mins[2];

	points[3][0] = mins[0];
	points[3][1] = maxs[1];
	points[3][2] = maxs[2];

	points[4][0] = maxs[0];
	points[4][1] = mins[1];
	points[4][2] = mins[2];

	points[5][0] = maxs[0];
	points[5][1] = mins[1];
	points[5][2] = maxs[2];

	points[6][0] = maxs[0];
	points[6][1] = maxs[1];
	points[6][2] = mins[2];

	points[7][0] = maxs[0];
	points[7][1] = maxs[1];
	points[7][2] = maxs[2];
}

public bool STA_BoxIntersects(float[3] min1, float[3] max1, float[3] min2, float[3] max2)
{
	return
	(min1[0] <= max2[0] && max1[0] >= min2[0]) &&
	(min1[1] <= max2[1] && max1[1] >= min2[1]) &&
	(min1[2] <= max2[2] && max1[2] >= min2[2]);
}

public void STA_GetPlayerBoundingBox(int client, float[3] min, float[3] max)
{
	GetEntDataVector(client, Offset_VecMin, min);
	GetEntDataVector(client, Offset_VecMax, max);
}

public void STA_GetBoxOrigin(float[3] min, float[3] max, float[3] output)
{
	float center[3];
	MakeVectorFromPoints(min, max, center);
	
	center[0] /= 2.0;
	center[1] /= 2.0;
	center[2] /= 2.0;
	
	AddVectors(min, center, output);
}

public void STA_GetBoxSize(float[3] min, float[3] max, float[3] center, float[3] outputmin, float[3] outputmax)
{
	for (int i = 0; i < 3; ++i)
	{
		outputmin[i] = min[i] - center[i];
		
		if (outputmin[i] > 0.0)
		{
			outputmin[i] *= -1.0;
		}
		
		outputmax[i] = FloatAbs(max[i] - center[i]);
	}
}

public void STA_DisplayBox(int client, int sprite, float[8][3] points, float lifetime, int color[4])
{
	TE_SetupBeamPoints(points[0], points[1], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToClient(client);
	
	TE_SetupBeamPoints(points[1], points[5], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToClient(client);
	
	TE_SetupBeamPoints(points[5], points[4], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToClient(client);
	
	TE_SetupBeamPoints(points[4], points[0], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToClient(client);
	
	TE_SetupBeamPoints(points[2], points[3], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToClient(client);
	
	TE_SetupBeamPoints(points[3], points[7], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToClient(client);
	
	TE_SetupBeamPoints(points[7], points[6], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToClient(client);
	
	TE_SetupBeamPoints(points[6], points[2], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToClient(client);
	
	TE_SetupBeamPoints(points[0], points[2], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToClient(client);
	
	TE_SetupBeamPoints(points[1], points[3], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToClient(client);
	
	TE_SetupBeamPoints(points[5], points[7], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToClient(client);
	
	TE_SetupBeamPoints(points[4], points[6], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToClient(client);
}

public void STA_DisplayBoxAll(int sprite, float[8][3] points, float lifetime, int color[4])
{
	TE_SetupBeamPoints(points[0], points[1], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToAll();
	
	TE_SetupBeamPoints(points[1], points[5], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToAll();
	
	TE_SetupBeamPoints(points[5], points[4], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToAll();
	
	TE_SetupBeamPoints(points[4], points[0], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToAll();
	
	TE_SetupBeamPoints(points[2], points[3], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToAll();
	
	TE_SetupBeamPoints(points[3], points[7], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToAll();
	
	TE_SetupBeamPoints(points[7], points[6], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToAll();
	
	TE_SetupBeamPoints(points[6], points[2], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToAll();
	
	TE_SetupBeamPoints(points[0], points[2], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToAll();
	
	TE_SetupBeamPoints(points[1], points[3], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToAll();
	
	TE_SetupBeamPoints(points[5], points[7], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToAll();
	
	TE_SetupBeamPoints(points[4], points[6], sprite, 0, 0, 0, lifetime, 2.0, 2.0, 10, 0.0, color, 0);
	TE_SendToAll();
}
