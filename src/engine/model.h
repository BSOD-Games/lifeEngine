//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MODEL_H
#define MODEL_H

#include "engine/imodel.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Model : public IModel
	{
	public:
		// IModel
		virtual void					SetMesh( IMesh* Mesh );
		virtual void					SetMin( const Vector3D_t& MinPosition );
		virtual void					SetMax( const Vector3D_t& MaxPosition );

		virtual IMesh*					GetMesh() const;
		virtual const Vector3D_t&		GetMin() const;
		virtual const Vector3D_t&		GetMax() const;

		// ITransformable
		virtual void					Move( const Vector3D_t& FactorMove );
		virtual void					Rotate( const Vector3D_t& FactorRotate );
		virtual void					Rotate( const Quaternion_t& FactorRotate );
		virtual void					Scale( const Vector3D_t& FactorScale );

		virtual void					SetPosition( const Vector3D_t& Position );
		virtual void					SetRotation( const Vector3D_t& Rotation );
		virtual void					SetRotation( const Quaternion_t& Rotation );
		virtual void					SetScale( const Vector3D_t& Scale );

		virtual const Vector3D_t&		GetPosition() const;
		virtual const Quaternion_t&		GetRotation() const;
		virtual const Vector3D_t&		GetScale() const;
		virtual const Matrix4x4_t&		GetTransformation();

		// Model
		Model();
		~Model();

	private:
		bool				isNeedUpdateTransformation;

		IMesh*				mesh;
		Vector3D_t			localMin;
		Vector3D_t			localMax;
		Vector3D_t			min;
		Vector3D_t			max;
		Vector3D_t			position;
		Quaternion_t		rotation;
		Vector3D_t			scale;
		Matrix4x4_t			transformation;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MODEL_H

