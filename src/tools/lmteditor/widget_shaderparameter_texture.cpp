//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <qfiledialog.h>
#include <qmessagebox.h>

#include "common/gameinfo.h"
#include "engine/iengine.h"
#include "engine/iresourcesystem.h"

#include "engineapi.h"
#include "errors.h"
#include "shaderparameter.h"
#include "widget_shaderparameter_texture.h"
#include "ui_widget_shaderparameter_texture.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Texture::Widget_ShaderParameter_Texture( ShaderParameterPtr ShaderParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ShaderParameter_Texture() ),
	shaderParameter( ShaderParameter )
{
	ui->setupUi( this );
	if ( !shaderParameter )		return;
	if ( shaderParameter->GetType() == le::SPT_TEXTURE )
	{
		ShaderParameter::Texture		texture = shaderParameter->GetValueTexture();
		if ( !texture.texture ) return;

		ui->lineEdit_path->setText( texture.path );
	}
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Texture::~Widget_ShaderParameter_Texture()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: pushed button "Select texture"
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Texture::on_toolButton_path_clicked()
{
	if ( !shaderParameter ) return;

	QString			gameDir = EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir;
	QString			parameterName = shaderParameter->GetName();
	QString			path = QFileDialog::getOpenFileName( this, "Select texture [" + parameterName + "]", gameDir );
	if ( path.isEmpty() ) return;

	if ( !path.startsWith( gameDir ) )
	{
		Error_Info( "The selected file should be in the game directory" );
		return;
	}

	path.remove( gameDir );
	le::ITexture*			texture = EngineAPI::GetInstance()->GetResourceSystem()->LoadTexture( path.toLocal8Bit().data(), path.toLocal8Bit().data() );
	if ( !texture )
	{
		QString			messageError = "Fail load texture [" + path + "]\nDatails in logs";
		Error_Info( messageError.toLocal8Bit().data() );
		return;
	}
	
	shaderParameter->SetValueTexture( ShaderParameter::Texture( texture, path ) );
	ui->lineEdit_path->setText( path );
}