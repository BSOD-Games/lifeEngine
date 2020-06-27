//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_PROXYPARAMETER_SHADERPARAMETER_H
#define WIDGET_PROXYPARAMETER_SHADERPARAMETER_H

#include <QWidget>

#include "shaderparameter.h"
#include "materialproxyparameter.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_ProxyParameter_ShaderParameter;
}

//---------------------------------------------------------------------//

class Widget_ProxyParameter_ShaderParameter : public QWidget
{
	Q_OBJECT

public:	
	// Widget_ProxyParameter_ShaderParameter
	Widget_ProxyParameter_ShaderParameter( MaterialProxyParameterPtr MaterialProxyParameter, const std::vector< ShaderParameterPtr >& ShaderParameters, QWidget* Parent = nullptr );
	~Widget_ProxyParameter_ShaderParameter();

	virtual void			showEvent( QShowEvent* Event );

private slots:
	void			on_comboBox_shaderParameter_currentIndexChanged( int Value );

private:
	void			UpdateComboBoxShaderParameters();
	void			SetShaderParameter( int Index );

	MaterialProxyParameterPtr						proxyParameter;	
	Ui::Widget_ProxyParameter_ShaderParameter*		ui;
	const std::vector< ShaderParameterPtr >&		shaderParameters;
};

//---------------------------------------------------------------------//

#endif // WIDGET_PROXYPARAMETER_SHADERPARAMETER_H
