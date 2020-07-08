//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "lmttypes.h"

//---------------------------------------------------------------------//

class LMTParameter
{
public:

	//---------------------------------------------------------------------//

	enum PARAMETER_TYPE
	{
		PT_TEXTURE,
		PT_FLOAT,
		PT_INT,
		PT_BOOL,
		PT_VECTOR_2D,
		PT_VECTOR_3D,
		PT_VECTOR_4D,
		PT_COLOR
	};

	//---------------------------------------------------------------------//

	LMTParameter();
	LMTParameter( const LMTParameter& Copy );
	~LMTParameter();

	void						Clear();

	void						SetName( const std::string& Name );
	void						SetValueInt( int Value );
	void						SetValueFloat( float Value );
	void						SetValueBool( bool Value );
	void						SetValueVector2D( const LMTVector2D& Value );
	void						SetValueVector3D( const LMTVector3D& Value );
	void						SetValueVector4D( const LMTVector4D& Value );
	void						SetValueColor( const LMTColor& Value );
	void						SetValueTexture( const std::string& Path );

	bool						IsDefined() const;
	const std::string&			GetName() const;
	PARAMETER_TYPE				GetType() const;
	int							GetValueInt() const;
	float						GetValueFloat() const;
	bool						GetValueBool() const;
	const LMTVector2D			GetValueVector2D() const;
	const LMTVector3D			GetValueVector3D() const;
	const LMTVector4D			GetValueVector4D() const;
	const LMTColor				GetValueColor() const;
	const std::string			GetValueTexture() const;

private:
	void						DeleteValue();
	void						CopyValue( void* Value, PARAMETER_TYPE Type );

	bool					isDefined;
	void*					value;

	PARAMETER_TYPE			type;
	std::string				name;
};

//---------------------------------------------------------------------//