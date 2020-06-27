//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LMT_MATERIAL_PROXY_PARAMETER_H
#define LMT_MATERIAL_PROXY_PARAMETER_H

#include <qstring.h>
#include <memory>

#include "engine/ishaderparameter.h"
#include "engine/imaterialproxyvar.h"

//---------------------------------------------------------------------//

class MaterialProxyParameter
{
public:
	MaterialProxyParameter();
	~MaterialProxyParameter();

	void									ResetValue();

	void									SetName( const QString& Name, le::MATERIAL_PROXY_VAR_TYPE Type );
	void									SetValueBool( bool Value );
	void									SetValueInt( int Value );
	void									SetValueFloat( float Value );	
	void									SetValueVector2D( const le::Vector2D_t& Value );
	void									SetValueVector3D( const le::Vector3D_t& Value );
	void									SetValueVector4D( const le::Vector4D_t& Value );
	void									SetValueShaderParameter( le::IShaderParameter* Value );

	bool									GetValueBool() const;
	int										GetValueInt() const;
	float									GetValueFloat() const;	
	le::Vector2D_t							GetValueVector2D() const;
	le::Vector3D_t							GetValueVector3D() const;
	le::Vector4D_t							GetValueVector4D() const;
	le::IShaderParameter*					GetValueShaderParameter() const;
	inline le::IMaterialProxyVar*			GetHandle()			{ return materialProxyVar; }
	inline QString							GetName()			{ return materialProxyVar->GetName(); }
	inline le::MATERIAL_PROXY_VAR_TYPE		GetType() const
	{
		return type;
	}

private:
	le::IMaterialProxyVar*			materialProxyVar;
	le::MATERIAL_PROXY_VAR_TYPE		type;
};

//---------------------------------------------------------------------//

typedef std::shared_ptr< MaterialProxyParameter >			MaterialProxyParameterPtr;

//---------------------------------------------------------------------//

#endif // !LMT_MATERIAL_PROXY_PARAMETER_H