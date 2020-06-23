//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LMT_SHADERPARAMETER_H
#define LMT_SHADERPARAMETER_H

#include <qstring.h>
#include <memory>

#include "engine/ishaderparameter.h"

//---------------------------------------------------------------------//

class ShaderParameter
{
public:

	//---------------------------------------------------------------------//

	struct Texture
	{
		Texture( le::ITexture* Texture = nullptr, QString Path = "" );
		~Texture();

		le::ITexture*				texture;
		QString						path;
	};

	//---------------------------------------------------------------------//

	ShaderParameter();
	~ShaderParameter();

	void									ResetValue();

	void									SetName( const QString& Name, le::SHADER_PARAMETER_TYPE Type );
	void									SetValueInt( int Value );
	void									SetValueFloat( float Value );
	void									SetValueBool( bool Value );
	void									SetValueTexture( const Texture& Value );
	void									SetValueColor( const QColor& Value );
	void									SetValueVector4D( const le::Vector4D_t& Value );
	void									SetValueVector3D( const le::Vector3D_t& Value );
	void									SetValueVector2D( const le::Vector2D_t& Value );

	Texture									GetValueTexture() const;
	int										GetValueInt() const;
	float									GetValueFloat() const;
	bool									GetValueBool() const;
	QColor									GetValueColor() const;
	le::Vector4D_t							GetValueVector4D() const;
	le::Vector3D_t							GetValueVector3D() const;
	le::Vector2D_t							GetValueVector2D() const;

	inline bool								IsDefined() const
	{
		return shaderParameter->IsDefined();
	}

	inline QString							GetName() const
	{
		return shaderParameter->GetName();
	}

	inline le::SHADER_PARAMETER_TYPE		GetType() const
	{
		return type;
	}

	inline le::IShaderParameter*			GetHandle() const
	{
		return shaderParameter;
	}

private:
	le::IShaderParameter*			shaderParameter;
	le::SHADER_PARAMETER_TYPE		type;
	void*							value;
};

//---------------------------------------------------------------------//

typedef std::shared_ptr< ShaderParameter >			ShaderParameterPtr;

//---------------------------------------------------------------------//

#endif // !LMT_SHADERPARAMETER_H
