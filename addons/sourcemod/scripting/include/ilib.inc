#include <sdktools>
#include "ilib/vector.inc"
#include "ilib/trace_wrapper.inc"
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
* Creates an FClient struct from a given object reference
*
* @param entity		Entity object to check
*
* @return			FClient info containing the entity reference
*/

stock FClient CastToClient(FObject entity)
{
	FClient client;
	client.reference = entity;

	return client;
}

/*
* Interpolates between two values making a smooth transition
*
* @param current Starting value
* @param target Target value
* @param time Time interval for interpolation speed
*
* @return - Interpolated value between the current and target
*/
stock float InterpolateFloat(float current, float target, float time)
{
	return (1.0 - time) * current + time * target;
}

/*
* Gets an angle in degrees between two rotators
* 
* @param origin Origin angle
*
* @param target Target angle
*
* @return - Float value of angle in degrees 
*/
stock float GetAngle(FRotator origin, FRotator target)
{
	FVector aim, ang;

	aim = origin.GetForwardVector();
	ang = target.GetForwardVector();

	return RadToDeg(ArcCosine(Vector_DotProduct(aim, ang) / aim.Length(true)));
}

/*
* Gets the rotation between two locations in world space
*
* @param origin Origin position
* @param destination Destination position
*
* @return FRotator containing the world angles from the origin to the destination
*/
stock FRotator CalcAngle(FVector origin, FVector destination)
{
	FRotator angles;
	FVector buffer;
	Vector_Subtract(destination, origin, buffer);

	Vector_GetAngles(buffer, angles);

	return angles;
}

// Emits the given sound to all players
stock void EmitSoundToPlayers(const char[] sample)
{
	for (int i = 1; i <= MaxClients; i++)
	{
		FClient client;
		client.Set(i);
		if (client.Valid())
			EmitSoundToClient(client.Get(), sample);
	}
}

/*
* Creates a tf_hud_text object and prepares a timer to display it for the given duration
*
* @param client Client reference to display this message to
* @param message Message to display
* @param duration Duration to hold the message for
* @param icon Icon to use for the message
*/
stock void DisplayTFMessage(FClient client, const char[] message, float duration, const char[] icon = "leaderboard_streak")
{
	if (client.Valid())
	{
		float endTime = GetGameTime() + duration;

		DataPack data = CreateDataPack();
		data.WriteCell(client.Get());
		data.WriteString(message);
		data.WriteFloat(endTime);
		data.WriteString(icon);

		data.Reset();

		CreateTimer(0.1, DisplayTFMessageTimer, data, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
	}
}

// Timer function used with DiplayTFMessage
Action DisplayTFMessageTimer(Handle timer, DataPack data)
{
	data.Reset();

	FClient client;
	client.Set(data.ReadCell());

	if (client.Valid())
	{
		char message[256], icon[128];
		bool end;

		data.ReadString(message, sizeof message);

		float time = data.ReadFloat();
		end = time <= GetGameTime();

		//PrintToChat(client.Get(), "Timer on text: %.1f", time - GetGameTime());

		if (end)
		{
			delete data;
			return Plugin_Stop;
		}

		data.ReadString(icon, sizeof icon);

		WriteGameText(client, message, icon);

		return Plugin_Continue;
	}

	delete data;
	return Plugin_Stop;
}

/*
* Displays a tf_hud_text to the given client
*
* If the message needs to persist for a specific time, use DisplayTFMessage instead
* The Handle created from this message is garbage collected and does not need to be closed
*
* @param client Client reference to display this text to
*
* @param message Message to display
*
* @param icon Icon used with this message
*
*/
stock void WriteGameText(FClient client, const char[] message, const char[] icon = "leaderboard_streak")
{
	Handle GlobalText = StartMessageOne("HudNotifyCustom", client.Get()); // This handle is garbage collected, we don't need to worry about it

	SetGlobalTransTarget(client.Get());
	BfWriteString(GlobalText, message);
	BfWriteString(GlobalText, icon);
	BfWriteByte(GlobalText, client.GetTeam());
	EndMessage();
}