#if defined VECTOR_INCLUDED
#endinput
#endif
#define VECTOR_INCLUDED

public void GetArrayVector3(any[] array, int index, float[3] outvec)
{
	outvec[0] = array[index];
	outvec[1] = array[index + 1];
	outvec[2] = array[index + 2];
}

public void GetArrayVector2(any[] array, int index, float[2] outvec)
{
	outvec[0] = array[index];
	outvec[1] = array[index + 1];
}

public void CopyVector3ToArray(float[3] vector, any[] array, int index)
{
	array[index] = vector[0];
	array[index + 1] = vector[1];
	array[index + 2] = vector[2];
}

public void CopyVector2ToArray(float[2] vector, any[] array, int index)
{
	array[index] = vector[0];
	array[index + 1] = vector[1];
}

public void CopyVector2ToVector3(float[2] vec2, float[3] vec3)
{
	vec3[0] = vec2[0];
	vec3[1] = vec2[1];
	vec3[2] = 0.0;
}

public void CopyVector3ToVector2(float[3] vec3, float[2] vec2)
{
	vec2[0] = vec3[0];
	vec2[1] = vec3[1];
}

public void CopyVector3ToVector3(float[3] source, float[3] dest)
{
	dest[0] = source[0];
	dest[1] = source[1];
	dest[2] = source[2];
}

public void ZeroVector3(float[3] vec3)
{
	vec3[0] = 0.0;
	vec3[1] = 0.0;
	vec3[2] = 0.0;
}

public float SnapValueTo(float pos, int gridsize)
{
	int roundpos = RoundToNearest(pos);
	
	int closest = roundpos - roundpos % gridsize;	
	int offset = roundpos - closest;
	
	if (offset > (gridsize / 2))
	{
		closest += gridsize;
	}
	
	else if (offset < (-gridsize / 2))
	{
		closest -= gridsize;
	}
	
	return float(closest);
}

public void SnapVectorTo(float[3] pos, int gridsize)
{	
	pos[0] = (SnapValueTo(pos[0], gridsize));
	pos[1] = (SnapValueTo(pos[1], gridsize));
	pos[2] = (SnapValueTo(pos[2], gridsize));
}

public void MinVector(float[3] vec1, float[3] vec2, float[3] output)
{
	for (int i = 0; i < 3; ++i)
	{
		output[i] = vec1[i] < vec2[i] ? vec1[i] : vec2[i];
	}
}

public void MaxVector(float[3] vec1, float[3] vec2, float[3] output)
{
	for (int i = 0; i < 3; ++i)
	{
		output[i] = vec1[i] > vec2[i] ? vec1[i] : vec2[i];
	}
}

public void VectorLength(float[3] vec, float& xyvalue, float& zvalue)
{	
	float veccopy[3];
	CopyVector3ToVector3(vec, veccopy);
	
	zvalue = veccopy[2];
	veccopy[2] = 0.0;
	
	xyvalue = GetVectorLength(veccopy);
}

public void ClientVelocityLength(int client, float& xyvalue, float& zvalue)
{
	float velocity[3];			
	GetEntPropVector(client, Prop_Data, "m_vecAbsVelocity", velocity);
	
	VectorLength(velocity, xyvalue, zvalue);
}
