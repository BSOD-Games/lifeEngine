//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_SHADERPARAMETER_TEXTURE_H
#define WIDGET_SHADERPARAMETER_TEXTURE_H

#include <QWidget>
#include <QTimer>

#include "engine/icamera.h"
#include "engine/ishaderparameter.h"
#include "studiorender/istudiorenderinternal.h"

#include "shaderparameter.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_ShaderParameter_Texture;
}

//---------------------------------------------------------------------//

class Widget_ShaderParameter_Texture : public QWidget
{
	Q_OBJECT

public:	
	// Widget_ShaderParameter_Texture
	Widget_ShaderParameter_Texture( ShaderParameterPtr ShaderParameter, QWidget* Parent = nullptr );
	~Widget_ShaderParameter_Texture();

private slots:
	void			on_toolButton_path_clicked();

private:
	ShaderParameterPtr									shaderParameter;
	Ui::Widget_ShaderParameter_Texture*					ui;
};

//---------------------------------------------------------------------//

#endif // WIDGET_SHADERPARAMETER_TEXTURE_H
