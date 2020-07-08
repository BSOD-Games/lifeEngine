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

#include "lmtparameter.h"
#include "lmtproxy.h"

//---------------------------------------------------------------------//

class LMTDoc
{
public:

	//---------------------------------------------------------------------//

	enum CULLFACE_TYPE
	{
		CT_BACK,
		CT_FRONT
	};

	//---------------------------------------------------------------------//

	LMTDoc();
	~LMTDoc();

	bool									Load( const std::string& Path );
	bool									Save( const std::string& Path );
	void									Clear();
	void									AddParameter( const LMTParameter& Parameter );
	void									RemoveParameter( std::uint32_t Index );
	void									AddProxy( const LMTProxy& Proxy );
	void									RemoveProxy( std::uint32_t Index );
	void									EnableDepthTest( bool Enable = true );
	void									EnableDepthWrite( bool Enable = true );
	void									EnableBlend( bool Enable = true );
	void									EnableCullFace( bool Enable = true );
	LMTParameter*							FindParameter( const std::string& Name );
	LMTProxy*								FindProxy( const std::string& Name );

	void									SetSurface( const std::string& Surface );
	void									SetShader( const std::string& Shader );
	void									SetCullfaceType( CULLFACE_TYPE Type );

	bool									IsEnabledDepthTest() const;
	bool									IsEnabledDepthWrite() const;
	bool									IsEnabledBlend() const;
	bool									IsEnabledCullFace() const;
	CULLFACE_TYPE							GetCullfaceType() const;
	const std::string&						GetShader() const;
	const std::string&						GetSurface() const;
	LMTParameter*							GetParameter( std::uint32_t Index ) const;
	const std::vector< LMTParameter >&		GetParameters() const;
	LMTProxy*								GetProxy( std::uint32_t Index ) const;
	const std::vector< LMTProxy >&			GetProxes() const;

private:
	bool								isEnabledDepthTest;
	bool								isEnabledDepthWrite;
	bool								isEnabledBlend;
	bool								isEnabledCullFace;

	CULLFACE_TYPE						cullfaceType;
	std::string							shader;
	std::string							surface;
	std::vector< LMTParameter >			parameters;
	std::vector< LMTProxy >				proxes;
};

//---------------------------------------------------------------------//