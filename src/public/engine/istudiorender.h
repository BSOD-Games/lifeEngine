//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISTUDIORENDER_H
#define ISTUDIORENDER_H

#include "engine/lifeengine.h"
#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IStudioRender
	{
	public:
		virtual void			ResizeViewport( UInt32_t X, UInt32_t Y, UInt32_t Width, UInt32_t Height ) = 0;
		// TODO: �������� ������ ���������� ��������� �� ������ �����
		
		virtual void			SetVerticalSyncEnabled( bool IsEnabled = true ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDER_H
