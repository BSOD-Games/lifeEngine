//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
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

