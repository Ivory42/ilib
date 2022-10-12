#include <sdktools>
#include "ilib/vector.inc"
#include "ilib/colors.inc"

#if defined _ilib_entity_included
 #endinput
#endif
#define _ilib_entity_included

/*
* Struct for storing entity references
* Can be used to perform most actions you would normally perform on entities, just without needing to index anything or make unecessary global variables
*/
enum struct FObject
{
	int ref;

    // Sets the given entity index as the reference for this object
	void Set(int ent)
	{
		if (ent > 0)
			this.ref = EntIndexToEntRef(ent);
	}

    // Converts the reference this object points to into its entity index. Use FEntity::Valid() to ensure it's a valid entity before doing this
	int Get()
	{
		return EntRefToEntIndex(this.ref);
	}

    // Returns true if the reference this object points to is still valid
	bool Valid()
	{
		int ent = this.Get();
		return (ent > 0 && IsValidEntity(ent));
	}

	/*
    * Returns true if the entity is of the given class name
    *
    * @param1 class - classname to check for
    */
	bool Cast(const char[] class)
	{
		if (this.Valid())
		{
			char classname[256];
			this.GetClassname(classname, sizeof classname);

			return (StrContains(classname, class) != -1);
		}

		return false;
	}

    /*
    * Sets the render color of the entity this object points to
    *
    * @param1 color - color struct to apply to this entity
    */
	void SetColor(FLinearColor color)
	{
		if (this.Valid())
			SetEntityRenderColor(this.Get(), color.R, color.G, color.B, color.A);
	}

    /*
    * Wrapper for CreateEntityByName()
    *
    * @param1 classname - Entity type to create
    *
    * If successful, will automatically set the created entity as this object's reference
    */
	void Create(const char[] classname)
	{
		int entity = CreateEntityByName(classname);

		this.Set(entity);
	}

    /*
    * Wrapper for GetEntityClassname()
    *
    * @param1 classname - buffer to store the name into
    * @param2 size - size of the buffer
    */
	void GetClassname(char[] classname, int size)
	{
		if (this.Valid())
			GetEntityClassname(this.Get(), classname, size);
	}

    /*
    * Wrapper for AcceptEntityInput()
    */
	void Input(const char[] input, int activator = -1, int caller = -1, int id = 0)
	{
		if (this.Valid())
			AcceptEntityInput(this.Get(), input, activator, caller, id);
	}

    // Wrapper for HookSingleEntityOutput(). 
	void HookOutput(const char[] output, EntityOutput callback, bool once = false)
	{
		if (this.Valid())
			HookSingleEntityOutput(this.Get(), output, callback, once);
	}

    // Wrapper for UnhookSingleEntityOutput()
	void UnhookOutput(const char[] output, EntityOutput callback)
	{
		if (this.Valid())
			UnhookSingleEntityOutput(this.Get(), output, callback);
	}

    /*
    * Wrapper for DispatchKeyValue()
    *
    * @param1 key - Name of the key to set the value of
    * @param2 value - Value to set on the key, as a string
    */
	void SetKeyValue(const char[] key, const char[] value)
	{
		if (this.Valid())
			DispatchKeyValue(this.Get(), key, value);
	}

    /*
    * Wrapper for SetEntityModel()
    *
    * @param1 model - path of the model to set on this entity
    */
	void SetModel(const char[] model)
	{
		if (this.Valid())
			SetEntityModel(this.Get(), model);
	}

    /*
    * Spawns the entity this object is referencing
    */
	void Spawn()
	{
		if (this.Valid())
			DispatchSpawn(this.Get());
	}

    // Activates the entity this object is referencing
	void Activate()
	{
		if (this.Valid())
			ActivateEntity(this.Get());
	}

    // Marks this object to be removed
	void Kill()
	{
		if (this.Valid())
			RemoveEntity(this.Get());
	}

    // Marks this object to be removed on a delay
	void KillOnDelay(float duration)
	{
		char output[64];
		FormatEx(output, sizeof output, "OnUser1 !self:kill::%.1f:1", duration);
		SetVariantString(output);
		this.Input("AddOutput");
		this.Input("FireUser1");
	}

    /*
    * Teleports the entity referenced by this object
    *
    * @param1 pos - Location to teleport to
    * @param2 rot - Rotation to set
    * @param2 vel - Velocity to set
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

    /*
    * Sets the rotation and velocity of the entity referenced by this object
    * More lightweight than FObject::Teleport()
    *
    * @param1 rot - Rotation to set
    * @param2 vel - Velocity to set
    */
	void SetVelocityRotation(FRotator rot, FVector vel)
	{
		if (this.Valid())
		{
			TeleportEntity(this.Get(), NULL_VECTOR, rot.ToFloat(), vel.ToFloat());
		}
	}

	/*
    * Wrapper for SetEntPropVector()
    *
    * @param1 type - PropType of the given property
    * @param2 property - Name of the property to set
    * @param3 vector - FVector value to set on this property
    * @param4 element - Element in the array to set, if applicable
    */
	void SetPropVector(PropType type, const char[] property, FVector vector, int element = 0)
	{
		if (this.Valid())
			Vector_SetProperty(this.Get(), type, property, vector, element);
	}

    /*
    * Wrapper for SetEntPropVector() used for setting rotations
    *
    * @param1 type - PropType of the given property
    * @param2 property - Name of the property to set
    * @param3 rotator - FRotator value to set on this property
    * @param4 element - Element in the array to set, if applicable
    */
	void SetPropRotator(PropType type, const char[] property, FRotator rotator, int element = 0)
	{
		if (this.Valid())
			Rotator_SetProperty(this.Get(), type, property, rotator, element);
	}

    /*
    * Wrapper for SetEntProp()
    *
    * @param1 type - PropType of the given property
    * @param2 property - Name of the property to set
    * @param3 value - Value to set on this property
    * @param4 element - Element in the array to set, if applicable
    */
	void SetProp(PropType type, const char[] property, any value, int element = 0)
	{
		if (this.Valid())
			SetEntProp(this.Get(), type, property, value, _, element);
	}

    /*
    * Wrapper for SetEntPropEnt()
    *
    * @param1 type - PropType of the given property
    * @param2 property - Name of the property to set
    * @param3 reference - FObject to set for this property
    * @param4 element - Element in the array to set, if applicable
    */
	void SetPropEnt(PropType type, const char[] property, FObject reference, int element = 0)
	{
		if (this.Valid())
			SetEntPropEnt(this.Get(), type, property, value, element);
	}

    /*
    * Wrapper for SetEntPropFloat()
    *
    * @param1 type - PropType of the given property
    * @param2 property - Name of the property to set
    * @param3 value - float value to set on this property
    * @param4 element - Element in the array to set, if applicable
    */
	void SetPropFloat(PropType type, const char[] property, float value, int element = 0)
	{
		if (this.Valid())
			SetEntPropFloat(this.Get(), type, property, value, element);
	}

    /*
    * Wrapper for GetEntProp()
    *
    * @param1 type - PropType of the given property
    * @param2 property - Name of the property to get
    * @param3 element - Element in the array to set, if applicable
    *
    * @return - Integer value of the property, 0 if not an integer value
    */
	int GetProp(PropType type, const char[] property, int element = 0)
	{
		if (this.Valid())
			return GetEntProp(this.Get(), type, property, _, element);

		return 0;
	}

    /*
    * Wrapper for GetEntPropEnt()
    *
    * @param1 type - PropType of the given property
    * @param2 property - Name of the property to get
    * @param3 reference - FObject to use as a buffer for the entity pointer
    * @param4 element - Element in the array to set, if applicable 
    *
    */
	void GetPropEnt(PropType type, const char[] property, FObject reference, int element = 0)
	{
		if (this.Valid())
            reference.Set(GetEntPropEnt(this.Get(), type, property, element));

		return -1;
	}

    /*
    * Wrapper for GetEntPropFloat()
    *
    * @param1 type - PropType of the given property
    * @param2 property - Name of the property to get
    * @param3 element - Element in the array to set, if applicable
    *
    * @return - float value of the property, 0 if not an integer value
    */
	float GetPropFloat(PropType type, const char[] property, int element = 0)
	{
		if (this.Valid())
			return GetEntPropFloat(this.Get(), type, property, element);

		return 0.0;
	}

    /*
    * Wrapper for GetEntPropVector()
    *
    * @param1 type - PropType of the given property
    * @param2 property - Name of the property to get
    * @param3 vector - FVector to use as a buffer for the value
    * @param4 element - Element in the array to set, if applicable 
    *
    */
	void GetPropVector(PropType type, const char[] property, FVector vector, int element = 0)
	{
		if (this.Valid())
			Vector_GetProperty(this.Get(), type, property, vector, element);
	}

    /*
    * Wrapper for GetEntPropVector(). Used for getting rotation values
    *
    * @param1 type - PropType of the given property
    * @param2 property - Name of the property to get
    * @param3 rotator - FRotator to use as a buffer for the value
    * @param4 element - Element in the array to set, if applicable 
    *
    */
	void GetPropRotator(PropType type, const char[] property, FRotator rotator, int element = 0)
	{
		if (this.Valid())
			Rotator_GetProperty(this.Get(), type, property, rotator, element);
	}

    /*
    * Wrapper for GetEntPropString()
    *
    * @param1 type - PropType of the given property
    * @param2 property - Name of the property to get
    * @param3 buffer - Buffer to store the string in
    * @param4 size - Size of the buffer
    * @param5 element - Element in the array to set, if applicable 
    *
    */
	void GetPropString(PropType type, const char[] property, char[] buffer, int size, int element = 0)
	{
		if (this.Valid())
			GetEntPropString(this.Get(), type, property, buffer, size, element);
	}

    /*
    * Wrapper for HasEntProp()
    *
    * @param1 type - PropType of the given property
    * @param2 property - Name of the property to get
    *
    * @return - True if the property is found; false if the entity is invalid or the property does not exist
    */
	bool HasProp(PropType type, const char[] property)
	{
		if (this.Valid())
			return HasEntProp(this.Get(), type, property);

        return false;
	}

    /*
    * Stores the position of this object in an FVector
    *
    * @param1 buffer - Buffer to store the vector in
    */
	void GetPosition(FVector buffer)
	{
		if (this.Valid())
			this.GetPropVector(Prop_Data, "m_vecOrigin", buffer);
	}

    /*
    * Stores the angles of this object in an FRotator
    *
    * @param1 buffer - Buffer to store the vector in
    */
	void GetAngles(FRotator buffer)
	{
		if (this.Valid())
			this.GetPropRotator(Prop_Send, "m_angRotation", buffer);
	}

    /*
    * Gets the owner of this object
    *
    * @return - The entity index of the owner or itself if no owner is found
    */
	int GetOwner()
	{
		if (this.Valid())
		{
			int owner;

			if (this.HasProp(Prop_Send, "m_hOwnerEntity"))
			{
				owner = this.GetPropEnt(Prop_Send, "m_hOwnerEntity");
				if (IsValidEntity(owner) && owner > 0)
					return owner;
			}

			if (this.HasProp(Prop_Send, "m_hThrower")) // Some projectiles use m_hThrower instead
			{
				owner = this.GetPropEnt(Prop_Send, "m_hThrower");
				if (IsValidEntity(owner) && owner > 0)
					return owner;
			}

			PrintToChatAll("Entity can not accept an owner");

			// If no owner found, return itself
			return this.Get();
		}
		return -1;
	}

    // Sets the owner of this object
	void SetOwner(int owner)
	{
		if (this.Valid())
		{
			if (this.HasProp(Prop_Send, "m_hOwnerEntity"))
				this.SetPropEnt(Prop_Send, "m_hOwnerEntity", owner);

			if (this.HasProp(Prop_Send, "m_hThrower"))
				this.SetPropEnt(Prop_Send, "m_hThrower", owner);
		}
	}

    /*
    * Attaches this object to the given entity
    *
    * @param1 parent - Parent entity to attach to
    */
	void Attach(int parent)
	{
		if (this.Valid())
		{
			int ent = this.Get();

			SetVariantString("!activator");
			AcceptEntityInput(ent, "SetParent", parent, ent);
		}
	}

    /*
    * Creates and attaches a particle to this object
    *
    * @param1 name - Name of the particle to spawn
    * @param2 offset - Optional offset to apply to the particle
    *
    * @return - Entity index of the particle on success
    */
	int AttachParticle(const char[] name, FVector offset)
	{
		if (this.Valid())
		{
			int particle = CreateEntityByName("info_particle_system");

			FVector pos;
			this.GetPropVector(Prop_Data, "m_vecOrigin", pos);

			//if (!Vector_IsNull(offset))
			pos.Add(Vector_MakeFloat(offset));

			TeleportEntity(particle, Vector_MakeFloat(pos), NULL_VECTOR, NULL_VECTOR);
			DispatchKeyValue(particle, "effect_name", name);

			DispatchSpawn(particle);
			ActivateEntity(particle);
			AcceptEntityInput(particle, "Start");

			SetVariantString("!activator");
			AcceptEntityInput(particle, "SetParent", this.Get(), particle);

			return particle;
		}
		return -1;
	}

    /*
    * Finds the first instance of the given entity classname and sets it as this object's reference
    *
    * @param1 classname - Entity classname to search for
    *
    * @return - True if the entity was found, false if not
    */
	bool FindEntity(const char[] classname)
	{
		int entity;
		bool result;
		while ((entity = FindEntityByClassname(MaxClients + 1, classname)) != -1)
		{
			this.Set(entity);
			result = true;
			break;
		}
		return result;
	}
}