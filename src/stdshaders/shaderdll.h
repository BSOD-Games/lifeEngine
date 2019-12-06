//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADERDLL_H
#define SHADERDLL_H

#include "stdshaders/ishaderdll.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ShaderDLL : public IShaderDLL
	{
	public:
		// IShaderDLL
		virtual bool				Initialize( IEngine* Engine );
	
		virtual UInt32_t			GetShaderCount() const;
		virtual IShader*			GetShader( UInt32_t Index ) const;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADERDLL_H

