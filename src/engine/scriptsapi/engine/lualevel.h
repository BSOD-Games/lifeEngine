//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUALEVEL_H
#define LUALEVEL_H

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <string>
#include <unordered_map>
#include <LuaBridge/LuaBridge.h>

#include "engine/ifactory.h"
#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUACamera;
	class ILevel;

	//---------------------------------------------------------------------//

	class LUALevel
	{
	public:
		
		//---------------------------------------------------------------------//

		class EntityFactory : public IFactory
		{
		public:
			// IFactory
			virtual void* Create( const char* NameInterface );
			virtual void			Delete( void* Object );

			// EntityFactory
			~EntityFactory();
			void					Register( std::string Name, luabridge::LuaRef Class );

		private:
			std::unordered_map< std::string, luabridge::LuaRef* >			entites;
		};

		//---------------------------------------------------------------------//

		static void									Register( lua_State* LuaVM );
		static void									RegisterEntity( const char* Name, luabridge::LuaRef Class );
		static inline const EntityFactory&			GetEntityFactory()			{ return entityFactory; }

		LUALevel();
		LUALevel( ILevel* Level );
		LUALevel( const LUALevel& Copy );
		~LUALevel();
		void					Update();
		void					Render();
		void					Clear();
		void					AddCamera( const LUACamera& Camera );
		void					RemoveCamera( const LUACamera& Camera );
		void					RemoveCamera( UInt32_t Index );

		bool					IsLoaded() const;
		const char*				GetNameFormat() const;
		UInt32_t				GetCountCameras() const;
		LUACamera				GetCamera( UInt32_t Index ) const;
		inline ILevel*			GetLevel() const						{ return object; }

	private:
		ILevel*							object;
		static EntityFactory			entityFactory;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUALEVEL_H
