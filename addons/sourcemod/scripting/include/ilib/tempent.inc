#include <sdktools>
#include "ilib/client.h"

#if defined _ilib_tempents_included
 #endinput
#endif
#define _ilib_tempents_included

/*
* Struct used to store information for most tempents
*/
enum struct FTempentProperties
{
	float radius;
	float end_radius;

	float scale;

	int model;
	int halo;

	int start_frame;
	int frame_rate;

	float lifetime;
	float width;
	float amplitude;

	int magnitude;

	int color[4];

	int speed;

	int flags;
}

// General tempent constructor - most common tempents will use these properties
stock any[sizeof FTempentProperties] ConstructTempent(float scale = 1.0, float radius = 0.0, float end_radius = 0.0, int model = -1, int halo = -1, float life = 0.0, float width = 0.0, int rate = 1)
{
	FTempentProperties info;

	info.scale = scale;
	info.radius = radius;
	info.end_radius = end_radius;
	info.model = model;
	info.halo = halo;
	info.lifetime = life;
	info.width = width;

	info.frame_rate = rate;

	return info;
}

int tempent_index;

/*
* Methodmap for creating tempents
*/
methodmap TempEnt
{
	public TempEnt()
	{
		tempent_index++;
		return view_as<TempEnt>(tempent_index);
	}

    /*
    * Wrapper for TE_SetupBeamRingPoint()
    *
    * @param1 client - Client to send this tempent to, the world can be used to send to all
    * @param2 origin - Position to spawn this tempent at
    * @param3 info - Struct containing the information for this tempent
    */
	public void CreateRing(FClient client, FVector origin, FTempentProperties info)
	{
		float pos[3];
		pos = Vector_MakeFloat(origin);

		TE_SetupBeamRingPoint(pos, info.radius, info.end_radius, info.model, info.halo, info.start_frame, info.frame_rate, info.lifetime, info.width, info.amplitude, info.color, info.speed, info.flags);

        if (IsWorld(client))
            TE_SendToAll();
		else if (client.Valid())
			TE_SendToClient(client.Get());
	}

    /*
    * Wrapper for TE_SetupSmoke()
    *
    * @param1 client - Client to send this tempent to, the world can be used to send to all
    * @param2 origin - Position to spawn this tempent at
    * @param3 info - Struct containing the information for this tempent
    */
	public void CreateSmoke(FClient client, FVector origin, FTempentProperties info)
	{
		float pos[3];
		pos = Vector_MakeFloat(origin);

		TE_SetupSmoke(pos, info.model, info.scale, info.frame_rate);

		if (IsWorld(client))
            TE_SendToAll();
		else if (client.Valid())
			TE_SendToClient(client.Get());
	}

    /*
    * Wrapper for TE_SetupExplosion()
    *
    * @param1 client - Client to send this tempent to, the world can be used to send to all
    * @param2 origin - Position to spawn this tempent at
    * @param3 info - Struct containing the information for this tempent
    */
	public void CreateExplosion(FVector origin, FTempentProperties info)
	{
		float pos[3];
		pos = Vector_MakeFloat(origin);

		TE_SetupExplosion(pos, info.model, info.scale, info.frame_rate, info.flags, RoundFloat(info.radius), info.magnitude);
		
        if (IsWorld(client))
            TE_SendToAll();
		else if (client.Valid())
			TE_SendToClient(client.Get());
	}

    /*
    * Wrapper for TE_CreateBeamPoints()
    *
    * @param1 client - Client to send this tempent to, the world can be used to send to all
    * @param2 origin - Position to spawn this tempent at
    * @param3 info - Struct containing the information for this tempent
    */
	public void CreateBeam(FVector origin, FVector destination, FTempentProperties info)
	{
		TE_SetupBeamPoints(Vector_MakeFloat(origin), Vector_MakeFloat(destination), info.model, info.halo, info.start_frame, info.frame_rate, info.lifetime, info.width, info.width, info.magnitude, info.amplitude, info.color, info.speed);
		
        if (IsWorld(client))
            TE_SendToAll();
		else if (client.Valid())
			TE_SendToClient(client.Get());
	}
}

/*
* Creates a box from beams and displays it to all
*
* @param1 pos - World position to spawn the box
* @param2 size - Size of mins and maxs of the box
*/
stock void Tempent_DrawBox(FVector pos, float size)
{
	FVector min, max;
	max = ConstructVector(size, size, size);
	min = ConstructVector(size * -1.0, size * -1.0, size * -1.0);

	Vector_Add(max, pos, max);
	Vector_Add(min, pos, min);

	FVector pos1, pos2, pos3, pos4, pos5, pos6;
	pos1 = max;
	pos1.x = min.x;

	pos2 = max;
	pos2.y = min.y;

	pos3 = max;
	pos3.z = min.z;

	pos4 = min;
	pos4.x = max.x;

	pos5 = min;
	pos5.y = max.y;

	pos6 = min;
	pos6.z = max.z;

	FTempentProperties info;
	info.model = PrecacheModel("materials/sprites/laser.vmt");
	info.halo = info.model;
	info.lifetime = 0.1;
	info.width = 1.0;
	info.color = {255, 255, 255, 255};

    FClient world = GetWorld();

	TempEnt laser = TempEnt();
	laser.CreateBeam(world, max, pos1, info);
	laser.CreateBeam(world, max, pos2, info);
	laser.CreateBeam(world, max, pos3, info);

	laser.CreateBeam(world, pos6, pos1, info);
	laser.CreateBeam(world, pos6, pos2, info);
	laser.CreateBeam(world, pos6, min, info);

	laser.CreateBeam(world, pos4, min, info);
	laser.CreateBeam(world, pos4, pos3, info);
	laser.CreateBeam(world, pos4, pos2, info);

	laser.CreateBeam(world, pos5, min, info);
	laser.CreateBeam(world, pos5, pos1, info);
	laser.CreateBeam(world, pos5, pos3, info);
}