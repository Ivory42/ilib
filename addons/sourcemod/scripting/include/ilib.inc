#include "ilib/vector.inc"
#include "ilib/traces.inc"
#include "ilib/colors.inc"
#include "ilib/timer.inc"
#include "ilib/tempent.inc"
#include "ilib/entity.inc"
#include "ilib/client.inc"

#if defined _ilib_included
 #endinput
#endif
#define _ilib_included



/*
* Struct used to store information about a client
* Can be used to perform most actions normally used on client indexes
*/
enum struct FClient
{
	FObject reference; // Reference used for this client

    /*
    * Sets the reference of this client as the given client index
    *
    * @param1 client - Client index to set as this reference
    */
	void Set(int client)
	{
		if (client > 0 && client <= MaxClients && IsClientInGame(client))
			reference.Set(client);
	}

    // Gets this client's referenced index
	int Get()
	{
		return this.reference.Get();
	}

	bool Valid()
	{
		int client = this.reference.Get();
		if (client > 0 && client <= MaxClients)
			return IsClientInGame(client);

		return false;
	}

    // Returns whether or not this client is alive
	bool Alive()
	{
		if (this.Valid())
			return IsPlayerAlive(this.Get());

		return false;
	}

	/*
    * Teleports the reference client
    *
    * @param1 pos - Location to teleport to
    * @param2 rot - Rotation to set
    * @param3 vel - Velocity to set
    */
	void Teleport(FVector pos, FRotator rot, FVector vel)
	{
		if (this.Valid())
		{
			float position[3], angles[3], velocity[3];
			if (Vector_IsNull(pos))
				position = NULL_VECTOR;
			else
				position = Vector_MakeFloat(pos);

			if (Rotator_IsNull(rot))
				angles = NULL_VECTOR;
			else
				angles = Rotator_MakeFloat(rot);

			if (Vector_IsNull(vel))
				velocity = NULL_VECTOR;
			else
				velocity = Vector_MakeFloat(vel);

			TeleportEntity(this.Get(), position, angles, velocity);
		}
	}

	// Retrieves this client's team as an integer
	int GetTeam()
	{
		if (this.Valid())
			return GetClientTeam(this.Get());

		return -1;
	}

	// Retrieves this client's current health value as an integer
	int GetHealth()
	{
		if (this.Alive())
			return GetClientHealth(this.Get())

		return 0;
	}

	// Retrieves this client's maximum health value as an integer
	int GetMaxHealth()
	{
		if (this.Alive())
			return GetEntProp(this.Get(), Prop_Data, "m_iMaxHealth");

		return 0;
	}

	/*
    * Adds health to this client
	* @param1 amount - amount of health to add
	* @param2 overheal - As a multiplier, how much this healing can overheal the player by
    */
	void AddHealth(int amount, float overheal = 1.0)
	{
		if (this.Valid())
		{
			int health = GetClientHealth(this.Get());

			int max = GetEntProp(this.Get(), Prop_Data, "m_iMaxHealth");
			if (health > max * overheal)
				return;

			health += amount;
			if (health > max * overheal)
				health = RoundFloat(max * overheal);

			SetEntityHealth(this.Get(), health);
		}
	}

	// Retrieves this client's class as a TFClassType
	TFClassType GetClass()
	{
		if (this.Valid())
			return TF2_GetPlayerClass(this.Get());

		return TFClass_Unknown;
	}

	/*
    * Sets a condition on this client
    *
    * @param1 cond - TFCond condition value to set
    * @param2 duration - Duration for the condition
    * @param3 inflictor - Who inflicted this condition
    */
	void AddCondition(TFCond cond, float duration, int inflictor = 0)
	{
		if (this.Valid())
			TF2_AddCondition(this.Get(), cond, duration, inflictor);
	}

	/*
    * Removes the condition given from this client
    *
    * @param1 cond - TFCond condition to remove
    */
	void RemoveCondition(TFCond cond)
	{
		if (this.Valid())
			TF2_RemoveCondition(this.Get(), cond);
	}

    /*
    * Checks whether this client is in the given condition
    *
    * @param1 cond - Condition to check for
    *
    * @return - True if the client is in the condition
    */
	bool InCondition(TFCond cond)
	{
		if (this.Valid())
			return TF2_IsPlayerInCondition(this.Get(), cond);

		return false;
	}

	// Wrapper for EmitSoundToClient()
	void EmitSound(const char[] sound, int entity = 0, int channel = SNDCHAN_AUTO, int level = 200, int flags = 0, float vol = 1.0, int pitch = 100)
	{
		if (this.Valid())
			EmitSoundToClient(this.Get(), sound, entity, channel, level, flags, vol, pitch);
	}

    /*
    * Prints a center message to this client
    *
    * @param1 message - Text to display to this client
    */
	void PrintCenterText(const char[] message)
	{
		if (this.Valid())
			PrintCenterText(this.Get(), message);
	}

    // Gets the max speed of this client
	float GetMaxSpeed()
	{
		if (this.Valid())
			return GetEntPropFloat(this.Get(), Prop_Data, "m_flMaxspeed");

		else return 0.0;
	}

	/*
    * Gets the absolute origin of this client
    *
    * @param1 pos - FVector to use as a buffer for this value
    */
	void GetPosition(FVector pos)
	{
		if (this.Valid())
			Vector_GetClientPosition(this.Get(), pos);
		else
			LogError("Invalid Client! Client.GetPosition failed!");
	}

	/*
    * Gets the eye position of this client
    *
    * @param1 pos - FVector to use as a buffer for this value
    */
	void GetEyePosition(FVector pos)
	{
		if (this.Valid())
			Vector_GetClientEyePosition(this.Get(), pos);
		else
			LogError("Invalid Client! Client.GetEyePosition failed!");
	}

	/*
    * Gets the eye angles of this client
    *
    * @param1 rot - FRotator to use as a buffer for this value
    */
	void GetEyeAngles(FRotator rot)
	{
		if (this.Valid())
			Rotator_GetClientEyeAngles(this.Get(), rot);
		else
			LogError("Invalid Client! Client.GetEyeAngles failed!");
	}

	/*
    * Gets the absolute angles of this client
    *
    * @param1 rot - FRotator to use as a buffer for this value
    */
	void GetAngles(FRotator rot)
	{
		if (this.Valid())
			Rotator_GetClientAbsAngles(this.Get(), rot);
		else
			LogError("Invalid Client! Client.GetAngles failed!");
	}

	/*
    * Gets the velocity of this client
    *
    * @param1 pos - FVector to use as a buffer for this value
    */
	void GetVelocity(FVector vel)
	{
		if (this.Valid())
			Vector_GetProperty(this.Get(), Prop_Data, "m_vecVelocity", vel);
	}

	/*
    * Ignites this client with a duration and attacker
    *
    * @param1 attacker - Entity index of the attacker
    * @param2 duration - Duration of burn
    */
	void Ignite(int attacker, float duration)
	{
		if (this.Valid())
			TF2_IgnitePlayer(this.Get(), attacker, duration);
	}

	/*
    * Executes a client command on this client
    *
    * @param1 command - Command to execute
    */
	void ExecuteCommand(const char[] command)
	{
		if (this.Valid())
			ClientCommand(this.Get(), command);
	}

    /*
    * Returns the entity index of the weapon using the given slot
    *
    * @param1 slot - Slot to check for
    *
    * @return - Entity index of the weapon equipped in this slot
    */
	int GetSlot(int slot)
	{
		if (this.Valid())
			return GetPlayerWeaponSlot(this.Get(), slot);

		return -1;
	}
}

/*
* Constructs an FObject with the given entity index as its reference
*
* @param1 index - Entity index to use as a reference
*/
stock any[sizeof FObject] ConstructObject(int index)
{
    FObject entity;
    entity.Set(index);

    return entity;
}

/*
* Constructs an FClient with the given client as its reference
*
* @param1 index - Client index to use as a reference
* @param2 userid - Is the given index a userid
*/
stock any[sizeof FObject] ConstructClient(int index, bool userid = false)
{
    FClient client;
    
    if (userid)
    {
        int clientId = GetClientOfUserId(index);
        client.Set(clientId);
    }
    else
        client.Set(index);

    return client;
}


// Returns a reference to the world, or client 0
stock any[sizeof FClient] GetWorld()
{
    FClient world;
    world.reference.ref = 0;

    return world;
}