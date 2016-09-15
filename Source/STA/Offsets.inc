#if defined OFFSETS_INCLUDED
#endinput
#endif
#define OFFSETS_INCLUDED

/*
	Cached offsets
*/
public int Offset_CollisionGroup;
public int Offset_VecMin;
public int Offset_VecMax;
public int Offset_ObserverTarget;
public int Offset_ObserverMode;
public int Offset_SolidType;
public int Offsets_Effects;
public int Offsets_RenderColor;

void Offsets_ThrowIfNotFound(int offset, char[] name)
{
	if (offset == -1)
	{
		PrintToServer("################# %s not found", name);
	}
}

public void Offsets_Init()
{
	Offset_CollisionGroup = FindSendPropInfo("CBaseEntity", "m_CollisionGroup");
	Offsets_ThrowIfNotFound(Offset_CollisionGroup, "m_CollisionGroup");
	
	Offset_VecMin = FindSendPropInfo("CBaseEntity", "m_vecMins");
	Offsets_ThrowIfNotFound(Offset_VecMin, "m_vecMins");
	
	Offset_VecMax = FindSendPropInfo("CBaseEntity", "m_vecMaxs");
	Offsets_ThrowIfNotFound(Offset_VecMax, "m_vecMaxs");
	
	Offset_ObserverTarget = FindSendPropInfo("CBasePlayer", "m_hObserverTarget");
	Offsets_ThrowIfNotFound(Offset_ObserverTarget, "m_hObserverTarget");
	
	Offset_ObserverMode = FindSendPropInfo("CBasePlayer", "m_iObserverMode");
	Offsets_ThrowIfNotFound(Offset_ObserverMode, "m_iObserverMode");
	
	Offset_SolidType = FindSendPropInfo("CBaseEntity", "m_nSolidType");
	Offsets_ThrowIfNotFound(Offset_SolidType, "m_nSolidType");
	
	Offsets_Effects = FindSendPropInfo("CBaseEntity", "m_fEffects");
	Offsets_ThrowIfNotFound(Offsets_Effects, "m_fEffects");
	
	Offsets_RenderColor = FindSendPropInfo("CBaseEntity", "m_clrRender");
	Offsets_ThrowIfNotFound(Offsets_RenderColor, "m_clrRender");
}

public void Offsets_GetEntityRenderColor(int entity, int& red, int& green, int& blue, int& alpha)
{
	red = GetEntData(entity, Offsets_RenderColor, 1);
	green = GetEntData(entity, Offsets_RenderColor + 1, 1);
	blue = GetEntData(entity, Offsets_RenderColor + 2, 1);
	alpha = GetEntData(entity, Offsets_RenderColor + 3, 1);
}
