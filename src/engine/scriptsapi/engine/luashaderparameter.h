//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUASHADERPARAMETER_H
#define LUASHADERPARAMETER_H

#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAVector2D;
	class LUAVector3D;
	class LUAVector4D;
	class LUAColor;
	class IShaderParameter;

	//---------------------------------------------------------------------//

	class LUAShaderParameter
	{
	public:
		static void				Register( lua_State* LuaVM );

		LUAShaderParameter();
		~LUAShaderParameter();
		void					Clear();

		void					SetName( const char* Name );
		void					SetValueInt( int Value );
		void					SetValueFloat( float Value );
		void					SetValueShaderFlag( bool Value );
		void					SetValueVector2D( const LUAVector2D& Value );
		void					SetValueVector3D( const LUAVector3D& Value );
		void					SetValueVector4D( const LUAVector4D& Value );
		void					SetValueColor( const LUAColor& Value );
		//void					SetValueTexture( ITexture* Texture );

		bool					IsDefined() const;
		const char*				GetName() const;
		UInt32_t				GetType() const;
		int						GetValueInt() const;
		float					GetValueFloat() const;
		bool					GetValueShaderFlag() const;
		LUAVector2D				GetValueVector2D() const;
		LUAVector3D				GetValueVector3D() const;
		LUAVector4D				GetValueVector4D() const;
		LUAColor				GetValueColor() const;
		//ITexture*				GetValueTexture() const;
	private:
		IShaderParameter*		object;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUASHADERPARAMETER_H
