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

#include "engine/imaterialproxyvar.h"

//---------------------------------------------------------------------//

class MaterialProxyParameter
{
public:
	MaterialProxyParameter();
	~MaterialProxyParameter();

	void									ResetValue();

	void									SetName( const QString& Name, le::MATERIAL_PROXY_VAR_TYPE Type );

	inline le::IMaterialProxyVar*			GetHandle()			{ return materialProxyVar; }
	inline QString							GetName()			{ return materialProxyVar->GetName(); }
	inline bool								IsDefined() const
	{
		return materialProxyVar->IsDefined();
	}
	inline le::MATERIAL_PROXY_VAR_TYPE		GetType() const
	{
		return type;
	}

private:
	le::IMaterialProxyVar*			materialProxyVar;
	le::MATERIAL_PROXY_VAR_TYPE		type;
	void*							value;
};

//---------------------------------------------------------------------//

typedef std::shared_ptr< MaterialProxyParameter >			MaterialProxyParameterPtr;

//---------------------------------------------------------------------//

#endif // !LMT_MATERIAL_PROXY_PARAMETER_H