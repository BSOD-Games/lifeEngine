//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <QCloseEvent>

#include "configuration.h"
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

	Configuration		configuration;
	if ( !configuration.Load() ) return;

	games = configuration.GetGames();
	UpdateListGames();
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Window_SelectGame::~Window_SelectGame()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Get game
// ------------------------------------------------------------------------------------ //
GameDescriptor Window_SelectGame::GetGame() const
{
	if ( ui->comboBox_games->count() == 0 )
		return GameDescriptor();

	return games[ ui->comboBox_games->currentIndex() ];
}

// ------------------------------------------------------------------------------------ //
// Clicked on "Edit"
// ------------------------------------------------------------------------------------ //
void Window_SelectGame::on_pushButton_editGames_clicked()
{
	std::vector<GameDescriptor>		editGames = games;
	Window_EditGame					window_EditGame( editGames, this );
	Window_EditGame::RESULT_TYPE	Result = ( Window_EditGame::RESULT_TYPE ) window_EditGame.exec();

	if ( Result == Window_EditGame::RT_OK )
	{
		Configuration		configurations;
		configurations.SetGames( editGames );
		configurations.Save();

		games = editGames;
		UpdateListGames();
	}
}

// ------------------------------------------------------------------------------------ //
// Update list games in combobox
// ------------------------------------------------------------------------------------ //
void Window_SelectGame::UpdateListGames()
{
	ui->comboBox_games->clear();
	for ( int index = 0, count = games.size(); index < count; ++index )
		ui->comboBox_games->addItem( games[ index ].name );
}

// ------------------------------------------------------------------------------------ //
// Clicked on "OK"
// ------------------------------------------------------------------------------------ //
void Window_SelectGame::on_pushButton_ok_clicked()
{
	done( RT_OK );
}

// ------------------------------------------------------------------------------------ //
// Clicked on "Cancel"
// ------------------------------------------------------------------------------------ //
void Window_SelectGame::on_pushButton_cancel_clicked()
{
	done( RT_CANCEL );
}

// ------------------------------------------------------------------------------------ //
// Event close window
// ------------------------------------------------------------------------------------ //
void Window_SelectGame::closeEvent( QCloseEvent* Event )
{
	if ( Event->type() == QCloseEvent::Close )
		done( RT_CANCEL );

	Event->accept();
}