//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "window_selectgame.h"
#include "window_editgame.h"
#include "ui_window_selectgame.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Window_SelectGame::Window_SelectGame( QWidget* Parent ) :
	QDialog( Parent ),
	ui( new Ui::Window_SelectGame() )
{
	ui->setupUi( this );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Window_SelectGame::~Window_SelectGame()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Clicked on "Edit"
// ------------------------------------------------------------------------------------ //
void Window_SelectGame::on_pushButton_editGames_clicked()
{
	Window_EditGame::RESULT_TYPE		result;
	std::vector<GameDescriptor>			arrayGames = Window_EditGame::EditGames( result, this );
	if ( result == Window_EditGame::RT_CANCEL ) return;

	ui->comboBox_games->clear();
	for ( int index = 0, count = arrayGames.size(); index < count; ++index )
		ui->comboBox_games->addItem( arrayGames[ index ].name );
}
