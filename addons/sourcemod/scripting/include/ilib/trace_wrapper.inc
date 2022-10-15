#include "ilib/vector.inc"

#if defined _ilib_trace_included
 #endinput
#endif
#define _ilib_trace_included

/*
* Handles creating line traces between two points
* Must be deleted after it is done being used
*/
methodmap RayTrace < Handle
{
    /*
    * Unlike normal sourcepawn traces, angles cannot be used to create an infinite trace
    * Infinite traces use up slightly more overhead and really aren't necessary
    */
	public RayTrace(FVector pos, FVector end, int mask, TraceEntityFilter filter, any data = -1)
	{
		return view_as<RayTrace>(TR_TraceRayFilterEx(Vector_MakeFloat(pos), Vector_MakeFloat(end), mask, RayType_EndPoint, filter, data));
	}

    // Returns true if this trace hit an entity
	public bool DidHit()
	{
		return TR_DidHit(this);
	}

    /*
    * Stores the ending position of this trace
    * 
    * @param1 pos - Buffer to store the position
    */
	public void GetEndPosition(FVector pos)
	{
		float end[3];
		TR_GetEndPosition(end, this);

		pos.Set(end);
	}

    /*
    * Stores the starting position of this trace
    * 
    * @param1 pos - Buffer to store the position
    */
	public void GetStartPosition(FVector pos)
	{
		float start[3];
		TR_GetStartPosition(this, start);

		pos.Set(start);
	}

    /*
    * Stores the normal vector of this trace at its impact point
    * 
    * @param1 normal - Buffer to store the vector
    */
	public void GetNormalVector(FVector normal)
	{
		float pos[3];
		TR_GetPlaneNormal(this, pos);

		normal.Set(pos);
	}

    /*
    * Gets the entity index of the hit entity
    * 
    * @return - Entity index of the entity that was hit
    */
	public int GetHitEntity()
	{
		return TR_GetEntityIndex(this);
	}

    /*
    * Displays the trace and shows whether a hit was made or not
    * 
    * @param1 duration - Duration to display the trace
    */
	public void DebugTrace(float duration = 5.0)
	{
		FVector start, end;
		int color[4];

		if (this.DidHit())
			color = {0, 255, 0, 255};
		else
			color = {255, 0, 0, 255};

		this.GetStartPosition(start);
		this.GetEndPosition(end);

		TE_SetupBeamPoints(Vector_MakeFloat(start), Vector_MakeFloat(end), PrecacheModel("materials/sprites/laser.vmt"), PrecacheModel("materials/sprites/laser.vmt"), 1, 10, duration, 10.0, 10.0, 1, 0.0, color, 1);
		TE_SendToAll();
	}
}

/*
* Trace wrapper for hull traces
*/
methodmap HullTrace < RayTrace
{
	public HullTrace(FVector pos, FVector end, FVector mins, FVector maxs, int flags, TraceEntityFilter filter, any data = -1)
	{
		return view_as<HullTrace>(TR_TraceHullFilterEx(Vector_MakeFloat(pos), Vector_MakeFloat(end), Vector_MakeFloat(mins), Vector_MakeFloat(maxs), flags, filter, data));
	}
}
