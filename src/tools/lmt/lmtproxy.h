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

#include "lmtproxyparameter.h"

//---------------------------------------------------------------------//

class LMTProxy
{
public:

	//---------------------------------------------------------------------//

	LMTProxy();
	LMTProxy( const LMTProxy& Copy );
	~LMTProxy();

	void											Clear();

	void											SetName( const std::string& Name );
	void											AddParameter( LMTProxyParameter& Parameter );
	void											RemoveParameter( std::uint32_t Index );
	LMTProxyParameter*								FindParameter( const std::string& Name );

	const std::string&								GetName() const;
	LMTProxyParameter*								GetParameter( std::uint32_t Index ) const;
	const std::vector< LMTProxyParameter >			GetParameters() const;

private:
	std::string									name;
	std::vector< LMTProxyParameter >			parameters;
};

//---------------------------------------------------------------------//