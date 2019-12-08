//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IRESOURCESYSTEM_H
#define IRESOURCESYSTEM_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct Image;	
	typedef		void			( *LoadImageFn_t )( const char* Path, Image& Image, bool& IsError, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels );

	//---------------------------------------------------------------------//

	class IResourceSystem
	{
	public:
		// TODO: �� �������� ��������� �� ������ � ������������, ����������

		virtual void					RegisterParser( const char* Format, LoadImageFn_t LoadImage ) = 0;
		virtual Image*					LoadImage( const char* Name, const char* Group, const char* Path, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false ) = 0;
		virtual void					DeleteImage( const char* Name, const char* Group ) = 0;
		virtual void					DeleteImages( const char* Group ) = 0;
		virtual void					DeleteImages() = 0;
		virtual void					DeleteAll() = 0;

		virtual Image*					GetImage( const char* Name, const char* Group ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IRESOURCESYSTEM_H

