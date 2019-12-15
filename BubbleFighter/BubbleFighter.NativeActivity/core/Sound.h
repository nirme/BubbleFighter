#include <stdio.h>
#include <stdlib.h>
#include <SLES/OpenSLES.h>

#define SLEEP(x) /* Client system sleep function to sleep x milliseconds would replace SLEEP macro */
#define MAX_NUMBER_INTERFACES 3
#define MAX_NUMBER_OUTPUT_DEVICES 6
#define CAR_SPEED_KPH 50 /* Speed of car is 50km/hour */
#define CAR_SPEED_MMPSEC CAR_SPEED_KPH*1000000/3600 /* Speed of car in mm/second */
/* Checks for error. If any errors exit the application! */
void CheckErr(SLresult res)
{
	if (res != SL_RESULT_SUCCESS)
	{
		/* Debug printing to be placed here */
		exit(1);
	}
}
/* Stationary 3D positioned Midi , 2 PCM sources car engine noise and siren moving fast from left to right,
Listener stationary looking forward
*/
void TestGame(SLObjectItf sl)
{
	SLEngineItf EngineItf;
	SLAudioIODeviceCapabilitiesItf AudioIODeviceCapabilitiesItf;
	SLAudioOutputDescriptor AudioOutputDescriptor;
	SLuint32 OutputDeviceIDs[MAX_NUMBER_OUTPUT_DEVICES];
	SLint32 numOutputs = 0;
	SLboolean headset_available = SL_BOOLEAN_FALSE;
	SLuint32 headset_deviceID = 0;
	SLuint32 deviceID = 0;
	SLresult res;
	SLObjectItf Midi_player;
	SLPlayItf Midi_playItf;
	SLEffectSendItf Midi_effectSendItf;
	SL3DLocationItf Midi_3DLocationItf;
	SLDataSource midSrc;

	SLDataSource bnkSrc;
	SLDataLocator_URI Midi_fileLoc = { SL_DATALOCATOR_URI, "file:///foo.mid" };
	SLDataFormat_MIME Midi_fileFmt = { SL_DATAFORMAT_MIME, "audio/x-midi", SL_CONTAINERTYPE_SMF };
	SLDataLocator_URI Midi_bankLoc = { SL_DATALOCATOR_URI, "file:///foo.dls" };
	SLDataFormat_MIME Midi_bankFmt = { SL_DATAFORMAT_MIME, "audio/dls", SL_CONTAINERTYPE_MOBILE_DLS };
	SLObjectItf Pcm1_player;
	SLObjectItf Pcm2_player;
	SLPlayItf Pcm1_playItf;
	SLPlayItf Pcm2_playItf;
	SLSeekItf Pcm1_seekItf;
	SLSeekItf Pcm2_seekItf;
	SLPrefetchStatusItf Pcm1_prefetchItf;
	SLPrefetchStatusItf Pcm2_prefetchItf;
	SL3DGroupingItf Pcm1_3DGroupingItf;
	SL3DGroupingItf Pcm2_3DGroupingItf;
	SLEffectSendItf Pcm1_effectSendItf;
	SLEffectSendItf Pcm2_effectSendItf;
	SLDataLocator_URI pcm1Loc = { SL_DATALOCATOR_URI, "file:///pcm1.wav" };
	SLDataLocator_URI pcm2Loc = { SL_DATALOCATOR_URI, "file:///pcm2.wav" };
	SLDataFormat_MIME pcmFormat = { SL_DATAFORMAT_MIME, "audio/x-wav", SL_CONTAINERTYPE_WAV };
	SLDataSource pcm1Src;
	SLDataSource pcm2Src;
	SLObjectItf Pcm_3DGroup;
	SL3DLocationItf Pcm_3DLocationItf;
	SL3DDopplerItf Pcm_3DDopplerItf;
	SLVec3D Location = { -500000,5000,0 }; /* 500 meters to the left of origin, 5 meters in front of origin */
	SLVec3D Midi_Location = { 5000,-3000,3000 }; /* 5 meters to the right of origin, 3 meters behind origin, 3 meters above origin */
	SLVec3D StartVelocity = { CAR_SPEED_MMPSEC,0,0 };
	SLObjectItf GameListener;
	SL3DLocationItf Listener_3DLocationItf;
	SLVec3D Listener_Front = { 0,0,-1000 }; /* Vector for having listener look forward */
	SLVec3D Listener_Above = { 0,1000,0 }; /* Vector for having listener look forward */
	SLObjectItf OutputMix;
	SLVolumeItf volumeItf;
	SLOutputMixItf outputMixItf;

	SLEnvironmentalReverbItf EnvReverbItf;
	SLEnvironmentalReverbSettings ReverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_CITY;
	SLDataLocator_OutputMix locator_outputmix;
	SLDataSink audioSink;
	int i;
	SLboolean required[MAX_NUMBER_INTERFACES];
	SLInterfaceID iidArray[MAX_NUMBER_INTERFACES];
	SLmillisecond MidiDurationMsec = 0;
	SLmillisecond PcmDurationMsec = 0;
	/* Get the SL Engine Interface which is implicit*/
	res = (*sl)->GetInterface(sl, SL_IID_ENGINE, (void *)&EngineItf); CheckErr(res);
	/* Initialize arrays required[] and iidArray[] */
	for (i = 0; i < MAX_NUMBER_INTERFACES; i++)
	{
		required[i] = SL_BOOLEAN_FALSE;
		iidArray[i] = SL_IID_NULL;
	}
	/* Get the Audio IO DEVICE CAPABILITIES interface */
	res = (*sl)->GetInterface(sl, SL_IID_AUDIOIODEVICECAPABILITIES, (void *)&AudioIODeviceCapabilitiesItf); CheckErr(res);
	numOutputs = MAX_NUMBER_OUTPUT_DEVICES;
	res = (*AudioIODeviceCapabilitiesItf)->GetAvailableAudioOutputs(AudioIODeviceCapabilitiesItf, &numOutputs, OutputDeviceIDs); CheckErr(res);
	/* Search for headset output device */
	for (i = 0; i < numOutputs; i++)
	{
		res = (*AudioIODeviceCapabilitiesItf)->QueryAudioOutputCapabilities(AudioIODeviceCapabilitiesItf, OutputDeviceIDs[i], &AudioOutputDescriptor); CheckErr(res);
		if ((AudioOutputDescriptor.deviceConnection == SL_DEVCONNECTION_ATTACHED_WIRED) &&
			(AudioOutputDescriptor.deviceScope == SL_DEVSCOPE_USER) &&
			(AudioOutputDescriptor.deviceLocation == SL_DEVLOCATION_HEADSET))
		{
			headset_deviceID = OutputDeviceIDs[i];
			headset_available = SL_BOOLEAN_TRUE;
			break;
		}
	}
	/* Expect headset output to be available */

	if (!headset_available)
	{
		/* Debug printing to be placed here */
		exit(1);
	}
	/* Set arrays required[] and iidArray[] for VOLUME and ENVIRONMENTAL REVERB interface (OUTPUTMIX is implicit) */
	required[0] = SL_BOOLEAN_TRUE;
	iidArray[0] = SL_IID_VOLUME;
	required[1] = SL_BOOLEAN_TRUE;
	iidArray[1] = SL_IID_ENVIRONMENTALREVERB;
	/* Create Output Mix object to be used by all players */
	res = (*EngineItf)->CreateOutputMix(EngineItf, &OutputMix, 2, iidArray, required); CheckErr(res);
	/* Realizing the Output Mix object in synchronous mode. */
	res = (*OutputMix)->Realize(OutputMix, SL_BOOLEAN_FALSE); CheckErr(res);
	res = (*OutputMix)->GetInterface(OutputMix, SL_IID_VOLUME, (void *)&volumeItf); CheckErr(res);
	/* Get the environmental reverb interface */
	res = (*OutputMix)->GetInterface(OutputMix, SL_IID_ENVIRONMENTALREVERB, (void *)&EnvReverbItf); CheckErr(res);
	/* Set reverb environment to city. */
	res = (*EnvReverbItf)->SetEnvironmentalReverbProperties(EnvReverbItf, &ReverbSettings); CheckErr(res);
	/* Get Output Mix interface */
	res = (*OutputMix)->GetInterface(OutputMix, SL_IID_OUTPUTMIX, (void *)&outputMixItf); CheckErr(res);
	/* Route output to headset */
	res = (*outputMixItf)->ReRoute(outputMixItf, 1, &headset_deviceID); CheckErr(res);
	/* Set up the MIDI data source */
	midSrc.pLocator = (void*)&Midi_fileLoc;
	midSrc.pFormat = (void*)&Midi_fileFmt;
	/* Set up the bank data source */
	bnkSrc.pLocator = (void*)&Midi_bankLoc;
	bnkSrc.pFormat = (void*)&Midi_bankFmt;
	/* Setup the data sink structure */
	locator_outputmix.locatorType = SL_DATALOCATOR_OUTPUTMIX;
	locator_outputmix.outputMix = OutputMix;
	audioSink.pLocator = (void *)&locator_outputmix;
	audioSink.pFormat = NULL;

	/* Create the Midi player */
	/* Set arrays required[] and iidArray[] for 3DLOCATION and EFFECTSEND interfaces (PlayItf is implicit) */
	required[0] = SL_BOOLEAN_TRUE;
	iidArray[0] = SL_IID_3DLOCATION;
	required[1] = SL_BOOLEAN_TRUE;
	iidArray[1] = SL_IID_EFFECTSEND;
	res = (*EngineItf)->CreateMidiPlayer(EngineItf, &Midi_player, &midSrc, &bnkSrc, &audioSink, NULL, NULL, 2, iidArray, required); CheckErr(res);
	/* Realizing the Midi player object in synchronous mode. */
	res = (*Midi_player)->Realize(Midi_player, SL_BOOLEAN_FALSE); CheckErr(res);
	/* Get playback, 3D location and effectsend interfaces
	for Midi player */
	res = (*Midi_player)->GetInterface(Midi_player, SL_IID_PLAY, (void *)&Midi_playItf); CheckErr(res);
	res = (*Midi_player)->GetInterface(Midi_player, SL_IID_3DLOCATION, (void *)&Midi_3DLocationItf); CheckErr(res);
	res = (*Midi_player)->GetInterface(Midi_player, SL_IID_EFFECTSEND, (void *)&Midi_effectSendItf); CheckErr(res);
	/* Get duration of Midi content */
	res = (*Midi_playItf)->GetDuration(Midi_playItf, &MidiDurationMsec); CheckErr(res);
	/* Set 3D location of Midi */
	res = (*Midi_3DLocationItf)->SetLocationCartesian(Midi_3DLocationItf, &Midi_Location); CheckErr(res);
	/* Enable the reverb effect and set the reverb level for Midi Player at -3dB (-300mB) */
	res = (*Midi_effectSendItf)->EnableEffectSend(Midi_effectSendItf, EnvReverbItf, SL_BOOLEAN_TRUE, -300);
	/* Setup the data source structures for pcm1 and pcm2 */
	pcm1Src.pLocator = (void *)&pcm1Loc;
	pcm2Src.pLocator = (void *)&pcm2Loc;
	pcm1Src.pFormat = (void *)&pcmFormat;
	pcm2Src.pFormat = (void *)&pcmFormat;
	/* Set arrays required[] and iidArray[] for PREFETCH, SEEK, 3DGROUPING and EFFECTSEND interfaces (PlayItf is implicit) */
	required[0] = SL_BOOLEAN_TRUE;
	iidArray[0] = SL_IID_PREFETCHSTATUS;
	required[1] = SL_BOOLEAN_TRUE;
	iidArray[1] = SL_IID_3DGROUPING;
	required[2] = SL_BOOLEAN_TRUE;
	iidArray[2] = SL_IID_SEEK;
	required[3] = SL_BOOLEAN_TRUE;
	iidArray[3] = SL_IID_EFFECTSEND;

	/* Create the pcm1 player */
	res = (*EngineItf)->CreateAudioPlayer(EngineItf, &Pcm1_player, &pcm1Src, &audioSink, 4, iidArray, required); CheckErr(res);
	/* Realizing the pcm1 player in synchronous mode. */
	res = (*Pcm1_player)->Realize(Pcm1_player, SL_BOOLEAN_FALSE); CheckErr(res);
	/* Get playback, prefetch, seek, 3D grouping and effect send interfaces for Pcm1 player */
	res = (*Pcm1_player)->GetInterface(Pcm1_player, SL_IID_PLAY, (void *)&Pcm1_playItf); CheckErr(res);
	res = (*Pcm1_player)->GetInterface(Pcm1_player, SL_IID_PREFETCHSTATUS, (void *)&Pcm1_prefetchItf); CheckErr(res);
	res = (*Pcm1_player)->GetInterface(Pcm1_player, SL_IID_SEEK, (void *)&Pcm1_seekItf); CheckErr(res);
	res = (*Pcm1_player)->GetInterface(Pcm1_player, SL_IID_3DGROUPING, (void *)&Pcm1_3DGroupingItf); CheckErr(res);
	res = (*Pcm1_player)->GetInterface(Pcm1_player, SL_IID_EFFECTSEND, (void *)&Pcm1_effectSendItf); CheckErr(res);
	/* Get duration of pcm1 content */
	res = (*Pcm1_playItf)->GetDuration(Pcm1_playItf, &PcmDurationMsec); CheckErr(res);
	if (PcmDurationMsec != SL_TIME_UNKNOWN)
	{
		/* Enable looping of entire file */
		res = (*Pcm1_seekItf)->SetLoop(Pcm1_seekItf, SL_BOOLEAN_TRUE, 0, PcmDurationMsec); CheckErr(res);
	}
	else
	{
		/* Debug printing to be placed here */
		exit(1);
	}
	/* Enable the reverb effect and set the reverb level for Pcm1 Player at -3dB (-300mB) */
	res = (*Pcm1_effectSendItf)->EnableEffectSend(Pcm1_effectSendItf, EnvReverbItf, SL_BOOLEAN_TRUE, -300);
	/* Create the pcm2 player */
	res = (*EngineItf)->CreateAudioPlayer(EngineItf, &Pcm2_player, &pcm2Src, &audioSink, 4, iidArray, required); CheckErr(res);
	/* Realizing the pcm2 player in synchronous mode. */
	res = (*Pcm2_player)->Realize(Pcm2_player, SL_BOOLEAN_FALSE); CheckErr(res);
	/* Get playback, prefetch, 3D grouping and effect send interfaces for Pcm2 player */

	res = (*Pcm2_player)->GetInterface(Pcm2_player, SL_IID_PLAY, (void *)&Pcm2_playItf); CheckErr(res);
	res = (*Pcm2_player)->GetInterface(Pcm2_player, SL_IID_PREFETCHSTATUS, (void *)&Pcm2_prefetchItf); CheckErr(res);
	res = (*Pcm2_player)->GetInterface(Pcm2_player, SL_IID_SEEK, (void *)&Pcm2_seekItf); CheckErr(res);
	res = (*Pcm2_player)->GetInterface(Pcm2_player, SL_IID_3DGROUPING, (void *)&Pcm2_3DGroupingItf); CheckErr(res);
	res = (*Pcm2_player)->GetInterface(Pcm2_player, SL_IID_EFFECTSEND, (void *)&Pcm2_effectSendItf); CheckErr(res);
	/* Get duration of pcm2 content */
	res = (*Pcm2_playItf)->GetDuration(Pcm2_playItf, &PcmDurationMsec); CheckErr(res);
	if (PcmDurationMsec != SL_TIME_UNKNOWN)
	{
		/* Enable looping of entire file */
		res = (*Pcm2_seekItf)->SetLoop(Pcm2_seekItf, SL_BOOLEAN_TRUE, 0, PcmDurationMsec); CheckErr(res);
	}
	else
	{
		/* Debug printing to be placed here */
		exit(1);
	}
	/* Enable the reverb effect and set the reverb level for Pcm2 Player at -3dB (-300mB) */
	res = (*Pcm2_effectSendItf)->EnableEffectSend(Pcm2_effectSendItf, EnvReverbItf, SL_BOOLEAN_TRUE, -300);
	/* Set arrays required[] and iidArray[] for 3DDoppler interface (3DLocation is implicit) */
	required[0] = SL_BOOLEAN_TRUE;
	iidArray[0] = SL_IID_3DDOPPLER;
	/* Create 3DGroup to be used for pcm1 and pcm2 */
	res = (*EngineItf)->Create3DGroup(EngineItf, &Pcm_3DGroup, 1, iidArray, required); CheckErr(res);
	/* Realizing the 3DGroup in synchronous mode. */
	res = (*Pcm_3DGroup)->Realize(Pcm_3DGroup, SL_BOOLEAN_FALSE); CheckErr(res);
	res = (*Pcm_3DGroup)->GetInterface(Pcm_3DGroup, SL_IID_3DLOCATION, (void *)&Pcm_3DLocationItf); CheckErr(res);
	res = (*Pcm_3DGroup)->GetInterface(Pcm_3DGroup, SL_IID_3DDOPPLER, (void *)&Pcm_3DDopplerItf); CheckErr(res);
	/* Add pcm1 and pcm2 players to 3DGroup */
	res = (*Pcm1_3DGroupingItf)->Set3DGroup(Pcm1_3DGroupingItf, Pcm_3DGroup); CheckErr(res);

	res = (*Pcm2_3DGroupingItf)->Set3DGroup(Pcm2_3DGroupingItf, Pcm_3DGroup); CheckErr(res);
	/* Set arrays required[] and iidArray[] for 3DDoppler interface (3DLocation is implicit) *
	required[0] = SL_BOOLEAN_TRUE;
	iidArray[0] = SL_IID_3DLOCATION;
	/* Create Listener */
	res = (*EngineItf)->CreateListener(EngineItf, &GameListener, 1, iidArray, required); CheckErr(res);
	/* Realizing the Listener in synchronous mode. */
	res = (*GameListener)->Realize(GameListener, SL_BOOLEAN_FALSE); CheckErr(res);
	res = (*GameListener)->GetInterface(GameListener, SL_IID_3DLOCATION, (void *)&Listener_3DLocationItf); CheckErr(res);
	/* Set 3D orientation of Listener to look forward - even though this is the default */
	res = (*Listener_3DLocationItf)->SetOrientationVectors(Listener_3DLocationItf, &Listener_Front, &Listener_Above); CheckErr(res);
	/* Set location of 3Dgroup */
	res = (*Pcm_3DLocationItf)->SetLocationCartesian(Pcm_3DLocationItf, &Location); CheckErr(res);
	/* Set velocity of 3Dgroup */
	res = (*Pcm_3DDopplerItf)->SetVelocityCartesian(Pcm_3DDopplerItf, &StartVelocity); CheckErr(res);
	/* Place pcm1 and pcm2 players into Paused state to start prefetch */
	res = (*Pcm1_playItf)->SetPlayState(Pcm1_playItf, SL_PLAYSTATE_PAUSED); CheckErr(res);
	res = (*Pcm2_playItf)->SetPlayState(Pcm2_playItf, SL_PLAYSTATE_PAUSED); CheckErr(res);
	/* Wait until prefetch buffer is full for both pcm1 and pcm2 players */
	{
		SLpermille Pcm1FillLevel = 0;
		SLpermille Pcm2FillLevel = 0;
		while ((Pcm1FillLevel != 1000) || (Pcm2FillLevel != 1000))
		{
			res = (*Pcm1_prefetchItf)->GetFillLevel(Pcm1_prefetchItf, &Pcm1FillLevel); CheckErr(res);
			res = (*Pcm2_prefetchItf)->GetFillLevel(Pcm2_prefetchItf, &Pcm2FillLevel); CheckErr(res);
		}
	}

	/* Start all 3 players */
	res = (*Midi_playItf)->SetPlayState(Midi_playItf, SL_PLAYSTATE_PLAYING); CheckErr(res);
	res = (*Pcm1_playItf)->SetPlayState(Pcm1_playItf, SL_PLAYSTATE_PLAYING); CheckErr(res);
	res = (*Pcm2_playItf)->SetPlayState(Pcm2_playItf, SL_PLAYSTATE_PLAYING); CheckErr(res);
	/* Move the location of the 3Dgroup each second to maintain a speed of 50km/hour in direction of x-axis */
	{
		SLmillisecond Midi_pos;
		SLint32 XaxisLocation = Location.x;
		do
		{
			SLEEP(1000); /* Delay 1000ms i.e. 1 second */
			XaxisLocation = Location.x + CAR_SPEED_MMPSEC;
			if (XaxisLocation <= 5000000) /* Continue moving car sounds until 5km away */
			{
				Location.x = XaxisLocation;
				res = (*Pcm_3DLocationItf)->SetLocationCartesian(Pcm_3DLocationItf, &Location); CheckErr(res);
			}
			res = (*Midi_playItf)->GetPosition(Midi_playItf, &Midi_pos); CheckErr(res);
		} while (Midi_pos < MidiDurationMsec); /* continue until Midi file finishes */
	}
	/* Stop the PCM players */
	res = (*Pcm1_playItf)->SetPlayState(Pcm1_playItf, SL_PLAYSTATE_STOPPED); CheckErr(res);
	res = (*Pcm2_playItf)->SetPlayState(Pcm2_playItf, SL_PLAYSTATE_STOPPED); CheckErr(res);
	/* Remove pcm1 and pcm2 players from 3DGroup */
	res = (*Pcm1_3DGroupingItf)->Set3DGroup(Pcm1_3DGroupingItf, NULL); CheckErr(res);
	res = (*Pcm2_3DGroupingItf)->Set3DGroup(Pcm2_3DGroupingItf, NULL); CheckErr(res);
	/* Destroy the objects */
	(*Pcm_3DGroup)->Destroy(Pcm_3DGroup);
	(*GameListener)->Destroy(GameListener);
	(*Pcm2_player)->Destroy(Pcm2_player);
	(*Pcm1_player)->Destroy(Pcm1_player);
	(*Midi_player)->Destroy(Midi_player);
	(*OutputMix)->Destroy(OutputMix);

}
int sl_main(void)
{
	SLresult res;
	SLObjectItf sl;
	char c;
	SLEngineOption EngineOption[] = {
	(SLuint32)SL_ENGINEOPTION_THREADSAFE,
	(SLuint32)SL_BOOLEAN_TRUE,
	(SLuint32)SL_ENGINEOPTION_MAJORVERSION, (SLuint32)1,
	(SLuint32)SL_ENGINEOPTION_MINORVERSION, (SLuint32)1
	};
	/* Simple test harness! */
	while ((c = getchar()) != 'q')
	{
		switch (c)
		{
		case '3':
			/* Create OpenSL ES */
			res = slCreateEngine(&sl, 3, EngineOption, 0, NULL, NULL); CheckErr(res);
			/* Realizing the SL Engine in synchronous mode. */
			res = (*sl)->Realize(sl, SL_BOOLEAN_FALSE); CheckErr(res);
			TestGame(sl);
			/* Shutdown OpenSL ES */
			(*sl)->Destroy(sl);
			break;
		default:
			break;
		}
	}
	exit(0);
}
