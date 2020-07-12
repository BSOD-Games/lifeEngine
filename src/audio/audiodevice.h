//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <string>

//---------------------------------------------------------------------//

struct ALCdevice;
struct ALCcontext;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class AudioDevice
	{
	public:
		AudioDevice();
		~AudioDevice();

		bool			Create();
		void			Destroy();

	private:
		bool			IsExtensionSupported( const std::string& Extension );

		ALCdevice*			audioDevice;
		ALCcontext*			audioContext;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !AUDIODEVICE_H