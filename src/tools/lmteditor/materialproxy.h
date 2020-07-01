//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LMT_MATERIAL_PROXY_H
#define LMT_MATERIAL_PROXY_H

#include <qstring.h>
#include <vector>
#include <memory>

#include "engine/imaterialproxy.h"
#include "materialproxyparameter.h"

//---------------------------------------------------------------------//

class MaterialProxy
{
public:
	MaterialProxy();
	~MaterialProxy();

	bool								Create( const QString& Name );
	void								Delete();
	void								AddParameter( const QString& Name, le::MATERIAL_PROXY_VAR_TYPE Type );
	void								AddParameter( MaterialProxyParameterPtr MaterialProxyParameter );
	void								RemoveParameter( quint32 Index );

	bool								HasParameter( const QString& Name ) const;
	inline le::IMaterialProxy*			GetHandle()						{ return materialProxy; }
	inline const QString&				GetName()						{ return name; }
	inline quint32						GetCountParameters() const		{ return parameters.size(); }
	inline MaterialProxyParameterPtr	GetParameter( quint32 Index ) const
	{
		if ( Index >= parameters.size() ) return nullptr;
		return parameters[ Index ];
	}
	inline const std::vector< MaterialProxyParameterPtr >&			GetParameters() const { return parameters; }

private:
	QString											name;

	le::IMaterialProxy*								materialProxy;
	std::vector< MaterialProxyParameterPtr >		parameters;
};

//---------------------------------------------------------------------//

typedef std::shared_ptr< MaterialProxy >			MaterialProxyPtr;

//---------------------------------------------------------------------//

#endif // !LMT_MATERIAL_PROXY_H