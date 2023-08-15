#include "ilib/entity.inc"

#if defined _ilib_shared_included
 #endinput
#endif
#define _ilib_shared_included

/*
 * Used as a base entity for different entity types. If you need to apply custom properties or refer back to the entity, use APersistentObject
 */
methodmap AObject
{
	public AObject(int entity)
	{
        return view_as<AObject>(ConstructObject(entity).ref);
    }

	// Returns the object reference
	public FObject GetObject()
	{
		FObject entity;
		entity.ref = view_as<int>(this);

		return entity;
	}

	// Returns the entity index of this object
	public int Get()
	{
		return this.GetObject().Get();
	}

	// Checks if the object reference is valid
	public bool Valid()
	{
		return this.GetObject().Valid();
	}

	// Checks if this object is of a the given classname. NOT a true cast
    public bool Cast(char[] classname)
    {
        return this.GetObject().Cast(classname);
    }

	// Deletes this object reference
	public void Kill()
	{
		this.GetObject().Kill();
	}

	// Sets the given keyvalue on this object
	public void SetKeyValue(const char[] key, char[] value)
	{
		this.GetObject().SetKeyValue(key, value);
	}

	// Sets the given keyvalue on this object as an integer
	public void SetKeyValueInt(const char[] key, int value)
	{
		this.GetObject().SetKeyValueInt(key, value);
	}

	// Sets the given keyvalue on this object as a float
	public void SetKeyValueFloat(const char[] key, float value)
	{
		this.GetObject().SetKeyValueFloat(key, value);
	}

	// Sets the given keyvalue on this object as a vector
	public void SetKeyValueVector(const char[] key, FVector value)
	{
		this.GetObject().SetKeyValueVector(key, value);
	}

	// Sets the owner of this object reference
	public void SetOwner(FObject value)
	{
		this.GetObject().SetOwner(value);
	}

	// Gets the owner of this object reference
	public FObject GetOwner()
	{
		return this.GetObject().GetOwner();
	}

	/*
	*	Gets the classname of this object
	*
	*	@param type		The type of classname to get, see EClassType
	*
	*	@return		String containing the desired classtype
	*/
	public char[] GetClassname(EClassType type = ClassType_Name)
	{
		char classname[64];
		if (this.Valid())
		{
			switch (type)
			{
				case ClassType_Name: this.GetObject().GetClassname(classname, sizeof classname);
				case ClassType_Networked: this.GetObject().GetNetClass(classname, sizeof classname);
			}
		}

		return classname;
	}

	// Wrapper for FinishSpawn() for AObject types
	public static void SpawnObject(AObject entity, FTransform spawn)
	{
		FGameplayStatics.FinishSpawn(entity.GetObject(), spawn);
	}
}

/*
 * Similar to AObject, but wraps a StringMap and can easily be given custom properties. Unlike AObject, this opens a handle and MUST be closed.
 * When closing this handle, use `FObjectStatics.RemoveObject` instead of delete or CloseHandle to prevent lingering entities.
 * While this can be used to wrap clients as well as any other entities, it is better to use AClient if you need to wrap a client;
 * do NOT call FObjectStatics.RemoveObject` on a client, you can safely use delete for clients.
 */
methodmap APersistentObject < StringMap
{
	public APersistentObject(FObject entity)
	{
		StringMap properties
		if (entity.Valid())
		{
			properties = new StringMap();
			properties.SetArray("ObjectReference", entity, sizeof FObject);
		}
		return view_as<APersistentObject>(properties);
	}

	// Returns the object reference
	public FObject GetObject()
	{
		FObject entity;
		
		this.GetArray("ObjectReference", entity, sizeof FObject);

		return entity;
	}
	
	public void SetObject(FObject entity) { this.SetArray("ObjectReference", entity, sizeof FObject); }

	// Returns the entity index of this object
	public int Get()
	{
		return this.GetObject().Get();
	}

	// Checks if the object reference is valid
	public bool Valid()
	{
		return this.GetObject().Valid();
	}

	// Checks if this object is of the given classname. NOT a true cast
    public bool Cast(char[] classname)
    {
        return this.GetObject().Cast(classname);
    }

	// Sets the given keyvalue on this object
	public void SetKeyValue(const char[] key, char[] value)
	{
		this.GetObject().SetKeyValue(key, value);
	}

	// Sets the given keyvalue on this object as an integer
	public void SetKeyValueInt(const char[] key, int value)
	{
		this.GetObject().SetKeyValueInt(key, value);
	}

	// Sets the given keyvalue on this object as a float
	public void SetKeyValueFloat(const char[] key, float value)
	{
		this.GetObject().SetKeyValueFloat(key, value);
	}

	// Sets the given keyvalue on this object as a vector
	public void SetKeyValueVector(const char[] key, FVector value)
	{
		this.GetObject().SetKeyValueVector(key, value);
	}

	// Sets the owner of this object reference
	public void SetOwner(FObject value)
	{
		this.GetObject().SetOwner(value);
	}

	// Gets the owner of this object reference
	public FObject GetOwner()
	{
		return this.GetObject().GetOwner();
	}

	/*
	*	Gets the classname of this object
	*
	*	@param type		The type of classname to get, see EClassType
	*
	*	@return		String containing the desired classtype
	*/
	public char[] GetClassname(EClassType type = ClassType_Name)
	{
		char classname[64];
		if (this.Valid())
		{
			switch (type)
			{
				case ClassType_Name: this.GetObject().GetClassname(classname, sizeof classname);
				case ClassType_Networked: this.GetObject().GetNetClass(classname, sizeof classname);
			}
		}

		return classname;
	}

	/*
	 * Custom Properties
	 */

	// Sets a general property on this object
	public void SetObjectProp(const char[] key, any value) { this.SetValue(key, value); }

	// Gets a property on this object
	public any GetObjectProp(const char[] key)
	{
		any value;

		this.GetValue(key, value);

		return value;
	}

	// Sets a float property on this object
	public void SetObjectPropFloat(const char[] key, float value) { this.SetValue(key, value); }

	// Gets a float property on this object
	public float GetObjectPropFloat(const char[] key)
	{
		float value = 0.0;

		this.GetValue(key, value);

		return value;
	}

	// Sets a vector property on this object
	public void SetObjectPropVector(const char[] key, FVector value) { this.SetArray(key, value, sizeof FVector); }

	// Gets a vector property on this object
	public FVector GetObjectPropVector(const char[] key)
	{
		FVector vector;

		this.GetArray(key, vector, sizeof vector);

		return vector;
	}

	// Sets a rotator property on this object
	public void SetObjectPropRotator(const char[] key, FRotator value) { this.SetArray(key, value, sizeof FRotator); }

	// Gets a rotator property on this object
	public FRotator GetObjectPropRotator(const char[] key)
	{
		FRotator rotator;

		this.GetArray(key, rotator, sizeof FRotator);

		return rotator;
	}

	// Sets an entity property on this object
	public void SetObjectPropEnt(const char[] key, FObject entity) { this.SetArray(key, entity, sizeof FObject); }

	// Gets an entity property on this object
	public FObject GetObjectPropEnt(const char[] key)
	{
		FObject entity;

		this.GetArray(key, entity, sizeof FObject);

		return entity;
	}

	// Sets a string property on this object
	public void SetObjectPropString(const char[] key, char[] value) { this.SetString(key, value); }

	// Gets a string property from this object
	public void GetObjectPropString(const char[] key, char[] buffer, int size) { this.GetString(key, buffer, size); }
}

/*
 * Class derived from `APersistenObject` used to easily add properties to clients. Keep in mind that this is a `StringMap` and opens a handle.
 * If you do not need to have a persistent wrapper on a client, use the FClient struct instead.
 * This should mostly be used with a global `SObjectMap` to keep track of the open handles.
 */
methodmap AClient < APersistentObject
{
	public AClient(FClient client)
	{
		APersistentObject properties;
		if (client.Valid())
		{
			properties = new APersistentObject(client.GetObject());
		}

		return view_as<AClient>(properties);
	}

	// Returns an `FClient` struct of the wrapped client
	public FClient GetClient() { return CastToClient(this.GetObject()); }

	// Health of this client
	property int Health
	{
		public get()
		{
			FClient client;
			client = CastToClient(this.GetObject());

			return client.GetHealth();
		}

		public set(int health) { SetEntityHealth(this.GetClient().Get(), health); }
	}

	// Max health of this client. (Read only)
	property int MaxHealth
	{
		public get()
		{
			FClient client;
			client = CastToClient(this.GetObject());

			return client.GetMaxHealth();
		}
	}
}

public bool operator!=(AClient one, AClient two)
{
	return (one.GetClient().Get() != two.GetClient().Get());
}

public bool operator==(AClient one, AClient two)
{
	return (one.GetClient().Get() == two.GetClient().Get());
}

// Acts as a StringMap but takes FObjects as keys
methodmap SObjectMap < StringMap
{
	public SObjectMap()
	{
		StringMap map = new StringMap();
		return view_as<SObjectMap>(map);
	}

	// Sets a StringMap value with an `FObject` as a key
	public void SetObjectValue(FObject key, any value)
	{
		char index[8];
		FormatEx(index, sizeof index, "%d", key.GetReference());
		
		this.SetValue(index, value);
	}

	/**
	 * Stores a value in the buffer parameter from this object map
	 * 
	 * @param key		`FObject` to use as a key
	 * @param value		Buffer to store the value in
	 */
	public void GetObjectValue(FObject key, any& value)
	{
		char index[8];
		FormatEx(index, sizeof index, "%d", key.GetReference());

		this.GetValue(index, value);
	}
	
	// Returns a base persistent object. Make sure to cast to whichever type you're getting.
	public APersistentObject GetObject(FObject key)
	{
		char index[8];
		FormatEx(index, sizeof index, "%d", key.GetReference());
		
		APersistentObject entity;
		this.GetValue(index, entity);
		
		return entity;
	}

	// Checks if the given key exists in this map
	public bool HasKey(FObject key)
	{
		char index[8];
		FormatEx(index, sizeof index, "%d", key.GetReference());

		return this.ContainsKey(index);
	}
	
	/**
	 * Removes the value in the map at the given object key
	 */
	public void RemoveObjectValue(FObject key)
	{
		char index[8];
		FormatEx(index, sizeof index, "%d", key.GetReference());
		
		this.Remove(index);
	}
}

methodmap FObjectStatics
{
	// Properly removes the entity and closes the respective handle tied to it
	public static void RemoveObject(APersistentObject entity)
	{
		FObject reference;
		reference = entity.GetObject();

		reference.Kill();

		delete entity;
	}
}
