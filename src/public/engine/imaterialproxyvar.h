//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIAL_PROXY_VAR_H
#define IMATERIAL_PROXY_VAR_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IShaderParameter;

    //---------------------------------------------------------------------//

    enum MATERIAL_PROXY_VAR_TYPE
    {
        MPVT_NONE,
        MPVT_FLOAT,
        MPVT_INT,
        MPVT_BOOL,
        MPVT_VECTOR_2D,
        MPVT_VECTOR_3D,
        MPVT_VECTOR_4D,
        MPVT_ARRAY_FLOAT,
        MPVT_ARRAY_INT,
        MPVT_ARRAY_VECTOR_2D,
        MPVT_ARRAY_VECTOR_3D,
        MPVT_ARRAY_VECTOR_4D,
        MPVT_SHADER_PARAMETER
    };

    //---------------------------------------------------------------------//

    class IMaterialProxyVar
    {
    public:
        virtual ~IMaterialProxyVar() {}
        virtual void					Clear() = 0;

        virtual void					SetName( const char* Name ) = 0;
        virtual void					SetValueInt( int Value ) = 0;
        virtual void					SetValueFloat( float Value ) = 0;
        virtual void					SetValueBool( bool Value ) = 0;
        virtual void					SetValueVector2D( const Vector2D_t& Value ) = 0;
        virtual void					SetValueVector3D( const Vector3D_t& Value ) = 0;
        virtual void					SetValueVector4D( const Vector4D_t& Value ) = 0;
        virtual void					SetValueShaderParameter( IShaderParameter* Value ) = 0;
        virtual void                                    SetValueArrayFloat( float* Array, UInt32_t Count ) = 0;
        virtual void                                    SetValueArrayInt( int* Array, UInt32_t Count ) = 0;
        virtual void                                    SetValueArrayVector2D( Vector2D_t* Array, UInt32_t Count ) = 0;
        virtual void                                    SetValueArrayVector3D( Vector3D_t* Array, UInt32_t Count ) = 0;
        virtual void                                    SetValueArrayVector4D( Vector4D_t* Array, UInt32_t Count ) = 0;

        virtual bool					IsDefined() const = 0;
        virtual const char*				GetName() const = 0;
        virtual MATERIAL_PROXY_VAR_TYPE                 GetType() const = 0;
        virtual int					GetValueInt() const = 0;
        virtual float					GetValueFloat() const = 0;
        virtual bool					GetValueBool() const = 0;
        virtual const Vector2D_t&                       GetValueVector2D() const = 0;
        virtual const Vector3D_t&                       GetValueVector3D() const = 0;
        virtual const Vector4D_t&                       GetValueVector4D() const = 0;
        virtual IShaderParameter*                       GetValueShaderParameter() const = 0;
        virtual float*                                  GetValueArrayFloat( UInt32_t& Count ) = 0;
        virtual int*                                    GetValueArrayInt( UInt32_t& Count ) = 0;
        virtual Vector2D_t*                             GetValueArrayVector2D( UInt32_t& Count ) = 0;
        virtual Vector3D_t*                             GetValueArrayVector3D( UInt32_t& Count ) = 0;
        virtual Vector4D_t*                             GetValueArrayVector4D( UInt32_t& Count ) = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define MATERIALPROXYVAR_INTERFACE_VERSION "LE_MaterialProxyVar001"

//---------------------------------------------------------------------//

#endif // !IMATERIAL_PROXY_VAR_H
