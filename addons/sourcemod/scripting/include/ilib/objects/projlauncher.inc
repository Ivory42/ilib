#include <tf2_stocks>
#include "ilib/entity.inc"
#include "ilib/vector.inc"

#if defined _ilib_projlauncher_included
 #endinput
#endif
#define _ilib_projlauncher_included

// Enum for setting the type of a weapon mimic
enum EWeaponType
{
	WeaponType_Rocket = 0,
	WeaponType_Grenade,
	WeaponType_Arrow,
	WeaponType_Sticky
};

/*
* Map used to wrap around tf_point_weapon_mimic
* 
*/

methodmap FProjectileLauncher
{
	/*
	* Creates a projectile launcher with the given type and properties
	*
	* @param cast			Use this constructor as a cast
	* @param entity			If casting, test this entity
	* @param type       	Weapon type to use, see EWeaponType
	* @param minSpeed		Minimum speed to use for the projectiles
	* @param maxSpeed		Maxmimum speed to use for the projectiles
	* @param damage			Damage the projectiles should deal
	* @param blastradius	Blast radius, if applicable
	* @param inaccuracy		Spread angle of this launcher
	* 
	* @return Wrapper for a deferred FObject. Use FinishSpawn() to activate
	*/
	public FProjectileLauncher(bool cast = false, FObject entity = {}, EWeaponType type = WeaponType_Rocket, float minSpeed = 1100.0, float maxSpeed = 1100.0, int damage = 90, float blastradius = 146.0, float inaccuracy = 0.0)
	{
		// Can't overload constructors so just doing this instead
		if (!cast)
		{
			FObject launcher;
			launcher = CreateObjectDeferred("tf_point_weapon_mimic");

			launcher.SetProp(Prop_Data, "m_nWeaponType", view_as<int>(type));

			// Speeds
			launcher.SetPropFloat(Prop_Data, "m_flSpeedMin", minSpeed);
			launcher.SetPropFloat(Prop_Data, "m_flSpeedMax", maxSpeed);

			// Damage and radius
			launcher.SetKeyValueInt("Damage", damage);
			launcher.SetPropFloat(Prop_Data, "m_flSplashRadius", blastradius);

			// Spread
			launcher.SetPropFloat(Prop_Data, "m_flSpreadAngle", inaccuracy);

			// Set the scale
			launcher.SetPropFloat(Prop_Data, "m_flModelScale", 1.0);

			return view_as<FProjectileLauncher>(launcher.ref);
		}
		else if (entity.Cast("tf_point_weapon_mimic")) // Do a cast instead...
			return view_as<FProjectileLauncher>(entity.ref);
		else
			return view_as<FProjectileLauncher>(0); // Invalid object if cast fails
	}

	// Type of weapon this launcher will mimic
	property EWeaponType Type
	{
		public set(EWeaponType value)
		{
			this.GetObject().SetProp(Prop_Data, "m_nWeaponType", view_as<int>(value));
		}
		public get()
		{
			return view_as<EWeaponType>(this.GetObject().GetProp(Prop_Data, "m_nWeaponType"));
		}
	}

	// Maximum speed for projectiles
	property float MaxSpeed
	{
		public set(float value)
		{
			this.GetObject().SetPropFloat(Prop_Data, "m_flSpeedMax", value);
		}
		public get()
		{
			return this.GetObject().GetPropFloat(Prop_Data, "m_flSpeedMax");
		}
	}

	// Minimum speed for projectiles
	property float MinSpeed
	{
		public set(float value)
		{
			this.GetObject().SetPropFloat(Prop_Data, "m_flSpeedMin", value);
		}
		public get()
		{
			return this.GetObject().GetPropFloat(Prop_Data, "m_flSpeedMin");
		}
	}

	// Damage for projectiles
	property float Damage
	{
		public set(float value)
		{
			this.GetObject().SetPropFloat(Prop_Data, "m_flDamage", value);
		}
		public get()
		{
			return this.GetObject().GetPropFloat(Prop_Data, "m_flDamage");
		}
	}

	// Splash radius
	property float BlastRadius
	{
		public set(float value)
		{
			this.GetObject().SetPropFloat(Prop_Data, "m_flSplashRadius", value);
		}
		public get()
		{
			return this.GetObject().GetPropFloat(Prop_Data, "m_flSplashRadius");
		}
	}

	// Spread angle for this launcher
	property float Inaccuracy
	{
		public set(float value)
		{
			this.GetObject().SetPropFloat(Prop_Data, "m_flSpreadAngle", value);
		}
		public get()
		{
			return this.GetObject().GetPropFloat(Prop_Data, "m_flSpreadAngle");
		}
	}
	
	public FObject GetObject()
	{
		FObject entity;
		entity.ref = view_as<int>(this);

		return entity;
	}

	public bool Valid()
	{
		return this.GetObject().Valid();
	}

	public void Fire()
	{
		if (this.Valid())
			this.GetObject().Input("FireOnce");
	}

	// Fires several projectiles from this launcher
	public void FireMultiple(int count)
	{
		if (this.Valid())
		{
			SetVariantInt(count);
			this.GetObject().Input("FireMultiple");
		}
	}

	// Detonates all stickies owned by this launcher
	public void DetonateStickies()
	{
		if (this.Valid())
			this.GetObject().Input("DetonateStickies");
	}
}

/**
 * Checks if a projectile was launched by a projectile launcher
 * 
 * @param entity     Projectile to test
 * @return           True if the projectile was launched by a projectile launcher. False if no launcher or invalid entity type
 */
stock bool ProjectileLaunched(FObject entity)
{
	if (entity.Cast("tf_projectile_"))
	{
		FProjectileLauncher launcher = FProjectileLauncher(true, entity.GetPropEnt(Prop_Send, "m_hLauncher"));

		return launcher.Valid();
	}

	return false;
}

/**
 * Sets team and owner properties on this projectile. Mainly used for projectiles launched from tf_point_weapon_mimic
 * 
 * @param projectile     Spawned projectile
 * @param owner          Owning client
 */
stock void SetProjectileProperties(FObject projectile, FClient owner)
{
	projectile.SetOwner(owner.GetObject());
	projectile.SetProp(Prop_Send, "m_iTeamNum", owner.GetTeam());
}