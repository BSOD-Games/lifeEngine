//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIALSYSTEM_H
#define IMATERIALSYSTEM_H

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IFactory;

	//---------------------------------------------------------------------//

	class IMaterialSystem
	{
	public:
		virtual bool			LoadShaderDLL( const char* FullPath ) = 0;
		virtual void			UnloadShaderDLL( const char* FullPath ) = 0;

		virtual IFactory*		GetFactory() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIALSYSTEM_H

