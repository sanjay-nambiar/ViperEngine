#ifndef AUDIO_MANAGER_HEADER
#define AUDIO_MANAGER_HEADER

#include <string>
#include "Types.h"
#include "Vector3.h"


namespace Viper
{
	/** This is an interface for AudioManager implementations. 
	 *  The API is designed to encourage event based audio systems with a single listener. 
	 *  A simple implementation can treat a SoundBank as a single sound file 
	 *  and have a default event for the sound.
	*/
	class AudioManager
	{
	public:
		/** Loads a sound bank into the system
		 * @param bankName The sound bank file name to load into the system
		 * @param isAsync If set to true, loads the sound bank asynchronously. False by default.
		 * @param shouldDecompress If set to true, the sound data is decompressed and stored in memory. False by default.
		*/
		virtual void LoadSoundBank(const std::string& bankName, bool_t isAsync = false, bool_t shouldDecompress = false) = 0;

		/** Loads all the events from a sound bank. Some systems require additional metadata to be loaded before loading events. 
		 *  Hence load events is given as a separate API. The events will be unloaded with the bank.
		*   @param bankName The sound bank file name from which the events are to be loaded.
		*/
		virtual void LoadSoundBankEvents(const std::string& bankName) = 0;

		/** Unloads a sound bank into the system
		 *  @param bankName The sound bank file name to unload from the system
		*/
		virtual void UnLoadSoundBank(const std::string& bankName) = 0;

		/** Sets the 3D position, forward and up directions and velocity of the listener
		 *  @param position The position of the audio listener in the world
		 *  @param forward A vector denoting the forward direction from the listener's point of view
		 *  @param up A vector denoting the up direction from the listener's point of view
		 *  @param velocity The velocity of the listener. Defaults to zero.
		*/
		virtual void SetListener3dAttributes(const Vector3& position, const Vector3& forward, const Vector3& up,
			const Vector3& velocity = Vector3{0,0,0}) = 0;

		/** Sets the 3D position, velocity of the event
		 *  @param eventName The name of the event of which the properties are to be set
		 *  @param position The position of the event in the world
		 *  @param velocity The velocity of the event. Defaults to zero.
		*/
		virtual void SetEvent3dAttributes(const std::string& eventName, const Vector3& position, const Vector3& velocity = Vector3{0,0,0}) = 0;

		/** Adds the given event to a group so that a group of events can be controlled together
		 *  @param eventName The name of the event to be added to the group
		 *  @param groupName The name of the group to which the event is to be added
		*/
		virtual void SetEventGroup(const std::string& eventName, const std::string& groupName) = 0;
		
		/** Sets the playback volume of an event
		 *  @param eventName The name of the event whose volume needs to be set
		 *  @param volumedB The new volume value in decibels
		*/
		virtual void SetEventVolume(const std::string& eventName, float volumedB) = 0;

		/** Sets the playback volume of all the events belonging to the specified group
		 *	@param groupName The name of the group whose volume needs to be set
		 *	@param volumedB The new volume value in decibels
		*/
		virtual void SetGroupVolume(const std::string& groupName, float volumedB) = 0;

		/** Plays an audio event
		 *  @param eventName The name of the event to be played
		*/
		virtual void PlayEvent(const std::string& eventName) = 0;

		/** Stops an already playing audio event
		 *  @param eventName The name of the event to be stopped
		 *  @param isImmediate If set to true, stops the event immediately. Defaults to false which causes the event audio to fade out.
		*/
		virtual void StopEvent(const std::string& eventName, bool_t isImmediate = false) = 0;

		/** Stops all the playing events from a group
		 *  @param groupName The name of the group to be stopped
		 *  @param isImmediate If set to true, stops all the events event immediately. Defaults to false which causes event audio to fade out.
		*/
		virtual void StopGroup(const std::string& groupName, bool_t isImmediate = false) = 0;

		/** Stops all the playing sounds
		*/
		virtual void StopAll(bool_t isImmediate = false) = 0;

		/** Updates any required state for the audio manager.
		 *  This method is ideally called in the game loop's update section
		 */
		virtual void Update() = 0;

		/** Checks if a given event is playing or not
		 *  @param eventName The name of the event which should be checked
		*/
		virtual bool_t IsPlaying(const std::string& eventName) const = 0;

		virtual ~AudioManager()
		{};
	};
}

#endif // AUDIO_MANAGER_HEADER
