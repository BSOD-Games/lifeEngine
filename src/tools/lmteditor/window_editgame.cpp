//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <qinputdialog.h>
#include <qfiledialog.h>
#include <QListWidgetItem>

#include "window_editgame.h"
#include "ui_window_editgame.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Window_EditGame::Window_EditGame( std::vector<GameDescriptor>& ArrayGames, QWidget* Parent ) :
	QDialog( Parent ),
	arrayGames( &ArrayGames ),
	currentGame( nullptr ),
	ui( new Ui::Window_EditGame() )
{
	ui->setupUi( this );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Window_EditGame::~Window_EditGame()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Edit games
// ------------------------------------------------------------------------------------ //
std::vector<GameDescriptor> Window_EditGame::EditGames( RESULT_TYPE& Result, QWidget* Parent )
{
	std::vector<GameDescriptor>			arrayGames;
	Window_EditGame			window_EditGame( arrayGames, Parent );
	Result = ( RESULT_TYPE ) window_EditGame.exec();

	return arrayGames;
}

// ------------------------------------------------------------------------------------ //
// Clicked on "Add"
// ------------------------------------------------------------------------------------ //
void Window_EditGame::on_pushButton_addGame_clicked()
{
	bool			isOk = false;
	QString			name = QInputDialog::getText( this, "Input name game", "Name", QLineEdit::Normal, "", &isOk );
	if ( !isOk || name.isEmpty() ) return;

	GameDescriptor			gameDescriptor;
	gameDescriptor.name = name;

	arrayGames->push_back( gameDescriptor );
	ui->listWidget_games->addItem( name );
}

// ------------------------------------------------------------------------------------ //
// Clicked on "Remove"
// ------------------------------------------------------------------------------------ //
void Window_EditGame::on_pushButton_removeGame_clicked()
{
	QModelIndex			selectIndex = ui->listWidget_games->currentIndex();
	if ( !selectIndex.isValid() ) return;

	ui->listWidget_games->model()->removeRow( selectIndex.row() );
}

// ------------------------------------------------------------------------------------ //
// Set edit game
// ------------------------------------------------------------------------------------ //
void Window_EditGame::SetEditGame( int IndexEditItem )
{
	if ( IndexEditItem == -1 )
	{
		currentGame = nullptr;
		ui->lineEdit_gamePath->setText( "" );

		ui->pushButton_removeGame->setEnabled( false );
		ui->toolButton_gamePath->setEnabled( false );
		return;
	}

	currentGame = &arrayGames->at( IndexEditItem );
	ui->lineEdit_gamePath->setText( currentGame->path );
	ui->pushButton_removeGame->setEnabled( true );
	ui->toolButton_gamePath->setEnabled( true );
}

// ------------------------------------------------------------------------------------ //
// Clicked on "Select path game"
// ------------------------------------------------------------------------------------ //
void Window_EditGame::on_toolButton_gamePath_clicked()
{
	QString			path = QFileDialog::getOpenFileName( this, "Select path to gameinfo.txt", "", "gameinfo.txt" );
	if ( path.isEmpty() ) return;

	ui->lineEdit_gamePath->setText( path );
	currentGame->path = path;
}

// ------------------------------------------------------------------------------------ //
// Changed row in list
// ------------------------------------------------------------------------------------ //
void Window_EditGame::on_listWidget_games_currentRowChanged( int currentRow )
{
	SetEditGame( currentRow );
}

// ------------------------------------------------------------------------------------ //
// Clicked on "Cancel"
// ------------------------------------------------------------------------------------ //
void Window_EditGame::on_pushButton_cancel_clicked()
{
	done( RT_CANCEL );
}

// ------------------------------------------------------------------------------------ //
// Clicked on "OK"
// ------------------------------------------------------------------------------------ //
void Window_EditGame::on_pushButton_ok_clicked()
{
	done( RT_OK );
}
