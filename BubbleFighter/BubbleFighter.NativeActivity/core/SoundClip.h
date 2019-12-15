#pragma once

namespace core
{

	class SoundClip
	{

		SL_DATALOCATOR_BUFFERQUEUE

		typedef struct SLDataLocator_BufferQueue_ {
			SLuint32 locatorType; = SL_DATALOCATOR_BUFFERQUEUE
			SLuint32 numBuffers; = 1
		} SLDataLocator_BufferQueue;


		typedef struct SLDataFormat_MIME_ {
			SLuint32 		formatType; = SL_DATAFORMAT_MIME
			SLchar * 		mimeType;
			SLuint32		containerType; = SL_CONTAINERTYPE
		} SLDataFormat_MIME;

/*
.mp3	"audio/mpeg3"
.mp3	"audio/x-mpeg-3"
!!!!.mp3	"video/mpeg"
.mp3	"video/x-mpeg"
*/

		mime = SLDataFormat_MIME{ SL_DATAFORMAT_MIME, "audio/x-wav", SL_CONTAINERTYPE_MP3 }

		typedef struct SLDataSource_ {
			void *pLocator; = (void *)&bufferQueue;
			void *pFormat; = (void *)&mime;
		} SLDataSource;

		SLDataLocator_Address
		SLDataFormat_PCM_EX


			SLDataLocator_BufferQueue


			typedef struct SLDataFormat_PCM_EX_ {
			SLuint32 formatType;
			SLuint32 numChannels;
			SLuint32 sampleRate;
			SLuint32 bitsPerSample;
			SLuint32 containerSize;
			SLuint32 channelMask;
			SLuint32 endianness;
			SLuint32 representation;
		} SLDataFormat_PCM_EX;


		CreateAudioPlayer
	};
}
