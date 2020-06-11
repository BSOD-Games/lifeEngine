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
#include <vector>

#include "lmttypes.h"

//---------------------------------------------------------------------//

class LMTProxyParameter
{
public:

	//---------------------------------------------------------------------//

	enum PARAMETER_TYPE
	{
		PT_NONE,
		PT_FLOAT,
		PT_INT,
		PT_BOOL,
		PT_VECTOR_2D,
		PT_VECTOR_3D,
		PT_VECTOR_4D,
		PT_ARRAY_FLOAT,
		PT_ARRAY_INT,
		PT_ARRAY_VECTOR_2D,
		PT_ARRAY_VECTOR_3D,
		PT_ARRAY_VECTOR_4D,
		PT_SHADER_PARAMETER
	};

	//---------------------------------------------------------------------//

	LMTProxyParameter();
	LMTProxyParameter( const LMTProxyParameter& Copy );
	~LMTProxyParameter();

	void								Clear();

	void								SetName( const std::string& Name );
	void								SetValueInt( int Value );
	void								SetValueFloat( float Value );
	void								SetValueBool( bool Value );
	void								SetValueVector2D( const LMTVector2D& Value );
	void								SetValueVector3D( const LMTVector3D& Value );
	void								SetValueVector4D( const LMTVector4D& Value );
	void								SetValueShaderParameter( const std::string& Value );
	void								SetValueArrayFloat( const std::vector< float >& Array );
	void								SetValueArrayInt( const std::vector< int >& Array );
	void								SetValueArrayVector2D( const std::vector< LMTVector2D >& Array );
	void								SetValueArrayVector3D( const std::vector< LMTVector3D >& Array );
	void								SetValueArrayVector4D( const std::vector< LMTVector4D >& Array );

	bool								IsDefined() const;
	const std::string&					GetName() const;
	PARAMETER_TYPE						GetType() const;
	int									GetValueInt() const;
	float								GetValueFloat() const;
	bool								GetValueBool() const;
	LMTVector2D							GetValueVector2D() const;
	LMTVector3D							GetValueVector3D() const;
	LMTVector4D							GetValueVector4D() const;
	std::string							GetValueShaderParameter() const;
	std::vector< float >				GetValueArrayFloat() const;
	std::vector< int >					GetValueArrayInt() const;
	std::vector< LMTVector2D >			GetValueArrayVector2D() const;
	std::vector< LMTVector3D >			GetValueArrayVector3D() const;
	std::vector< LMTVector4D >			GetValueArrayVector4D() const;

private:
	void								DeleteValue();
	void								CopyValue( void* Value, PARAMETER_TYPE Type );

	bool						isDefined;
	void*						value;

	PARAMETER_TYPE				type;
	std::string					name;
};

//---------------------------------------------------------------------//