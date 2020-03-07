//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_PROXY_VAR_H
#define MATERIAL_PROXY_VAR_H

#include <string>
#include <vector>

#include "engine/imaterialproxyvar.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class MaterialProxyVar : public IMaterialProxyVar
    {
    public:
        // IMaterialProxyVar
        virtual void                            Clear();

        virtual void                            SetName( const char* Name );
        virtual void                            SetValueInt( int Value );
        virtual void                            SetValueFloat( float Value );
        virtual void                            SetValueBool( bool Value );
        virtual void                            SetValueVector2D( const Vector2D_t& Value );
        virtual void                            SetValueVector3D( const Vector3D_t& Value );
        virtual void                            SetValueVector4D( const Vector4D_t& Value );
        virtual void                            SetValueShaderParameter( IShaderParameter* Value ) ;
        virtual void                            SetValueArrayFloat( float* Array, UInt32_t Count );
        virtual void                            SetValueArrayInt( int* Array, UInt32_t Count );
        virtual void                            SetValueArrayVector2D( Vector2D_t* Array, UInt32_t Count );
        virtual void                            SetValueArrayVector3D( Vector3D_t* Array, UInt32_t Count );
        virtual void                            SetValueArrayVector4D( Vector4D_t* Array, UInt32_t Count );

        virtual bool                            IsDefined() const;
        virtual const char*                     GetName() const;
        virtual MATERIAL_PROXY_VAR_TYPE         GetType() const;
        virtual int                             GetValueInt() const;
        virtual float                           GetValueFloat() const;
        virtual bool                            GetValueBool() const;
        virtual const Vector2D_t&               GetValueVector2D() const;
        virtual const Vector3D_t&               GetValueVector3D() const;
        virtual const Vector4D_t&               GetValueVector4D() const;
        virtual IShaderParameter*               GetValueShaderParameter() const;
        virtual float*                          GetValueArrayFloat( UInt32_t& Count );
        virtual int*                            GetValueArrayInt( UInt32_t& Count );
        virtual Vector2D_t*                     GetValueArrayVector2D( UInt32_t& Count );
        virtual Vector3D_t*                     GetValueArrayVector3D( UInt32_t& Count );
        virtual Vector4D_t*                     GetValueArrayVector4D( UInt32_t& Count );

        // MaterialProxyVar
        MaterialProxyVar();
        ~MaterialProxyVar();

    private:
        union Values
        {
            Values();
            ~Values();

            int                                 value_int;
            float                               value_float;
            bool                                value_bool;
            Vector2D_t                          value_vector2D;
            Vector3D_t                          value_vector3D;
            Vector4D_t                          value_vector4D;
            IShaderParameter*                   value_shaderParameter;
            std::vector< float >                value_arrayFloat;
            std::vector< int >                  value_arrayInt;
            std::vector< Vector2D_t >           value_arrayVector2D;
            std::vector< Vector3D_t >           value_arrayVector3D;
            std::vector< Vector4D_t >           value_arrayVector4D;
        };

        bool                            isDefined;

        std::string                     name;
        MATERIAL_PROXY_VAR_TYPE         type;
        Values                          values;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // IMATERIAL_PROXY_VAR_H
