//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUAMODEL_H
#define LUAMODEL_H

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAVector3D;
	class LUAQuaternion;
	class LUAMesh;
	class IModel;

	//---------------------------------------------------------------------//

	class LUAModel
	{
	public:
		static void				Register( lua_State* LuaVM );

		LUAModel();
		LUAModel( IModel* Model );
		LUAModel( const LUAModel& Copy );
		~LUAModel();
		
		void					Move( const LUAVector3D& FactorMove );
		void					Rotate( const LUAVector3D& FactorRotate );
		void					Rotate( const LUAQuaternion& FactorRotate );
		void					Scale( const LUAVector3D& FactorScale );

		void					SetPosition( const LUAVector3D& Position );
		void					SetRotation( const LUAVector3D& Rotation );
		void					SetRotation( const LUAQuaternion& Rotation );
		void					SetScale( const LUAVector3D& Scale );
		void					SetMesh( const LUAMesh& Mesh );
		void					SetMin( const LUAVector3D& MinPosition );
		void					SetMax( const LUAVector3D& MaxPosition );
		void					SetStartFace( UInt32_t StartFace );
		void					SetCountFace( UInt32_t CountFace );

		LUAVector3D				GetPosition() const;
		LUAQuaternion			GetRotation() const;
		LUAVector3D				GetScale() const;
		//virtual const Matrix4x4_t& GetTransformation() = 0;
		LUAMesh					GetMesh();
		LUAVector3D				GetMin();
		LUAVector3D				GetMax();
		UInt32_t				GetStartFace() const;
		UInt32_t				GetCountFace() const;
		inline IModel*			GetHandle() const { return object; }

	private:
		IModel*					object;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAMODEL_H
