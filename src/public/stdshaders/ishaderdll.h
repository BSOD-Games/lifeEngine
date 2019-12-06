//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISHADERDLL_H
#define ISHADERDLL_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;
	class IShader;

	//---------------------------------------------------------------------//

	class IShaderDLL
	{
	public:
		virtual bool				Initialize( IEngine* Engine ) = 0;

		virtual UInt32_t			GetShaderCount() const = 0;
		virtual IShader*			GetShader( UInt32_t Index ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISHADERDLL_H

