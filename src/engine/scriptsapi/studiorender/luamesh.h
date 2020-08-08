//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUAMESH_H
#define LUAMESH_H

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IMesh;

	//---------------------------------------------------------------------//

	class LUAMesh
	{
	public:
		static void				Register( lua_State* LuaVM );

		LUAMesh();
		LUAMesh( IMesh* Object );
		LUAMesh( const LUAMesh& Copy );
		~LUAMesh();
		
		inline IMesh*			GetHandle() const { return object; }

	private:
		IMesh*				object;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAMESH_H
