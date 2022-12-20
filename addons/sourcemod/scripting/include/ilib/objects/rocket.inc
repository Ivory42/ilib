#include <tf2_stocks>
#include "ilib/entity.inc"
#include "ilib/vector.inc"

#if defined _ilib_rocket_included
 #endinput
#endif
#define _ilib_rocket_included

/*
* Optional struct for quickly creating rockets
* These rockets are no different than just spawning a tf_projectile_rocket or equivalent; this just acts as a quicker way to create them
*/

enum ERocketType
{
	Rocket_Default, // Default rocket from most rocket launchers
	Rocket_Plasma,  // Cow mangler rocket
	Rocket_Sentry   // Sentry salvo
}

enum struct FRocket
{
	FObject Reference; // Entity object reference of this rocket

	float Speed; // Speed of this rocket when fired

	// Returns the object reference of this rocket
	FObject GetObject()
	{
		return this.Reference;
	}

	// Returns the entity index this rocket references
	int Get()
	{
		return this.GetObject().Get();
	}

	// Checks if the object reference is valid
	bool Valid()
	{
		return this.GetObject().Valid();
	}

	// Checks if this is a valid rocket
	bool IsRocket()
	{
		if (this.Valid())
		{
			if (this.GetObject().Cast("tf_projectile_rocket"))
				return true;
			
			if (this.GetObect().Cast("tf_projectile_energy_ball"))
				return true;

			if (this.GetObject().Cast("tf_projectile_sentryrocket"))
				return true;
		}

		return false;
	}

	/*
	* Fires this projectile based on its set speed in the given direction
	*
	* @param direction		Direction this projectile should fire at, as an angle
	*/
	void FireProjectile(FRotator direction)
	{
		if (this.IsRocket())
		{
			FVector uDir = direction.GetForwardVector(); // Get the unit direction
			uDir.Scale(this.Speed);

			this.GetObject().SetVelocityRotation(direction, uDir); // Fire the rocket
		}
	}

	// Returns this rocket's networked class
	char[] GetRocketType()
	{
		char classname[64];
		if (this.Valid())
			this.GetObject().GetNetClass(classname, sizeof classname);

		return classname;
	}

	/*
	* Sets the damage of this rocket
	*
	* @param damage		Damage this rocket should deal
	*/
	void SetDamage(float damage)
	{
		if (this.IsRocket())
			SetEntDataFloat(this.Get(), FindSendPropInfo(this.GetRocketType(), "m_iDeflected") + 4, damage);
	}
}

/*
* Quickly spawns a rocket with the given properties. If you need to set keyvalues or any other properties, use CreateRocketDeferred() instead.
* Spawns a stock rocket by default
*
* @param angle		Rotation this rocket should fire at
* @param type		The type of rocket this should spawn as
* @param speed		The speed this rocket should travel at
* @param damage		The initial damage for this rocket
*/
stock FRocket CreateRocket(FRotator angle, ERocketType type = Rocket_Default, float speed = 1100.0, float damage = 90.0)
{
	FRocket rocket;
	char classname[64];

	switch (type)
	{
		case Rocket_Default: FormatEx(classname, "tf_projectile_rocket");
		case Rocket_Plasma:	FormatEx(classname, "tf_projectile_energy_ball");
		case Rocket_Sentry:	FormatEx(classname, "tf_projectile_sentryrocket");
	}
	rocket.Reference = CreateObject(classname);
	rocket.Speed = speed;
	rocket.SetDamage(damage);

	rocket.FireProjectile(angle); // Fire the rocket
}

/*
* Creates a rocket object but does not spawn it. Call FinishSpawn() on the object's reference when ready to spawn
*
* @param type		The type of rocket this should spawn as
* @param speed		The speed this rocket should travel at
* @param damage		The damage this rocket should deal
*/
stock FRocket CreateRocketDeferred(ERocketType type = Rocket_Default, float speed = 1100.0, float damage = 90.0)
{
	FRocket rocket;
	char classname[64];

	switch (type)
	{
		case Rocket_Default: FormatEx(classname, "tf_projectile_rocket");
		case Rocket_Plasma:	FormatEx(classname, "tf_projectile_energy_ball");
		case Rocket_Sentry:	FormatEx(classname, "tf_projectile_sentryrocket");
	}

	rocket.Reference = CreateObjectDeferred(classname);
	rocket.Speed = speed;
	rocket.SetDamage(damage);
}

/*
* Returns an FRocket if the given object is a rocket
*
* @param entity		Entity to test
*/
stock FRocket CastToRocket(FObject entity)
{
	FRocket rocket;

	// Basically the same as FRocket::Valid(), we do this so we don't have to set the reference of the rocket until after we validate that it is a rocket.
	// Otherwise this entity will still be set as the rocket's reference, regardless, making this cast pointless
	if (entity.Cast("tf_projectile_rocket"))
		rocket.Reference = entity;
	else if (entity.Cast("tf_projectile_energy_ball"))
		rocket.Reference = entity;
	else if (entity.Cast("tf_projectile_sentryrocket"))
		rocket.Reference = entity;

	GetRocketSpeed(rocket);

	return rocket;
}

/*
* Gets the speed of the rocket based on the velocity and sets FRocket::Speed appropriately
*
* @param rocket		Rocket struct to populate
*/
stock void GetRocketSpeed(FRocket rocket)
{
	FVector vel;
	rocket.GetObject().GetPropVector(Prop_Data, "m_vecVelocity", vel);

	float speed = vel.Length();

	rocket.Speed = speed;
}