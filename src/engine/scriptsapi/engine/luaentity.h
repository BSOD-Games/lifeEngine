//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUAENTITY_H
#define LUAENTITY_H

#include "common/types.h"
#include "engine/ientity.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAVector3D;
	class LUAStudioRender;
	class LUAModel;
	class LUALevel;
	class LUACamera;

	//---------------------------------------------------------------------//

	class LUAEntity : public IEntity
	{
	public:
		// IReferenceObject
		virtual void					IncrementReference();
		virtual void					DecrementReference();
		virtual void					Release();
		virtual UInt32_t				GetCountReferences() const;

		// IEntity
		virtual void					KeyValue( const char* Key, const char* Value );
		virtual void					Update();
		virtual void					Render( IStudioRender* StudioRender );

		virtual void					SetModel( IModel* Model, IBody* Body );
		virtual void					SetPosition( const Vector3D_t& Position );
		virtual void					SetLevel( ILevel* Level );

		virtual bool					IsVisible( ICamera* Camera ) const;
		virtual Vector3D_t				GetCenter() const;
		virtual const Vector3D_t&		GetPosition() const;

		// LUAEntity
		LUAEntity( luabridge::LuaRef& Object );
		LUAEntity( const LUAEntity& Copy );
		~LUAEntity();

	private:
		Vector3D_t q;
		UInt32_t						countReferences;
		luabridge::LuaRef*				object;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAENTITY_H
