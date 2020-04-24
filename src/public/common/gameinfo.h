//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef GAMEINFO_H
#define GAMEINFO_H

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct GameInfo
	{
		GameInfo() :
			gameDir( nullptr ),
			title( nullptr ),
			gameDLL( nullptr ),
			icon( nullptr )
		{}

		~GameInfo() { Clear(); }

		inline void			Clear()
		{
			if ( gameDir )		delete[] gameDir;
			if ( title )		delete[] title;
			if ( gameDLL )		delete[] gameDLL;
                        if ( icon )		delete[] icon;
		}

		char*			gameDir;
		char*			title;
		char*			gameDLL;
		char*			icon;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !GAMEINFO_H
