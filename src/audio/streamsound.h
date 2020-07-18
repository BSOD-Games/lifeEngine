//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef STREAMSOUND_H
#define STREAMSOUND_H

#include <thread>
#include <mutex>
#include <vector>

#include "audio/istreamsound.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class StreamSound : public IStreamSound
	{
	public:
		// IReferenceObject
		virtual void			IncrementReference();
		virtual void			DecrementReference();
		virtual void			Release();
		virtual UInt32_t		GetCountReferences() const;

		// IStreamSound
		virtual void			Create();
		virtual void			Delete();
		virtual void			Play();
		virtual void			Pause();
		virtual void			Stop();
		virtual void			Open( IParserSoundBuffer* ParserSoundBuffer );
		virtual void			Close();

		virtual void			SetLoop( bool IsLoop );
		virtual void			SetRelativeToListener( bool IsRelative );
		virtual void			SetVolume( float Volume );
		virtual void			SetPitch( float Pitch );
		virtual void			SetMinDistance( float Distance );
		virtual void			SetAttenuation( float Attenuation );
		virtual void			SetPosition( const Vector3D_t& Position );

		virtual bool			IsOpened() const;
		virtual bool			IsLooped() const;
		virtual bool			IsRelativeToListener() const;
		virtual float			GetVolume() const;
		virtual float			GetPitch() const;
		virtual float			GetMinDistance() const;
		virtual float			GetAttenuation() const;
		virtual SOUND_STATUS	GetStatus() const;
		virtual Vector3D_t		GetPosition() const;

		// StreamSound
		StreamSound();
		~StreamSound();

	private:

		//---------------------------------------------------------------------//

		enum
		{
			NoLoop = -1,
			BufferCount = 3,
			BufferRetries = 2
		};

		struct Chunk
		{
			const Byte_t*		samples;
			UInt32_t			sampleCount;
		};

		//---------------------------------------------------------------------//

		void					StreamData();
		bool					FillQueue();
		void					ClearQueue();
		bool					FillAndPushBuffer( UInt32_t BufferIndex, bool ImmediateLoop = false );

		bool					GetData( Chunk& Data );

		bool					isStreaming;
		bool					isLoop;
		UInt32_t				countReferences;
		UInt32_t				handle;
		UInt32_t				sampleFormat;
		UInt32_t				sampleRate;
		UInt32_t				buffers[ BufferCount ];
		SOUND_STATUS			threadState;

		IParserSoundBuffer*		parserSoundBuffer;
		mutable std::mutex		mutexStreamData;
		std::thread*			threadStreamData;
		std::vector< Byte_t >	samples;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !STREAMSOUND_H