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
	void									SetValueTexture( const Texture& Value );
	void									SetValueColor( const QColor& Value );

	Texture									GetValueTexture() const;
	QColor									GetValueColor() const;
	
	inline bool								IsDefined() const
	{
		if ( !shaderParameter ) return false;
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
