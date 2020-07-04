//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <stdexcept>
#include <qinputdialog.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <QListWidgetItem>
#include <QCloseEvent>

#include "common/types.h"

#include "window_editgame.h"
#include "ui_window_editgame.h"
#include "errors.h"
#include "configuration.h"

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

	// Fill list games
	for ( le::UInt32_t index = 0, count = ArrayGames.size(); index < count; ++index )
		ui->listWidget_games->addItem( ArrayGames[ index ].name );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Window_EditGame::~Window_EditGame()
{
	delete ui;
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
	arrayGames->erase( selectIndex.row() + arrayGames->begin() );
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
		ui->lineEdit_gamePath->setEnabled( false );
		return;
	}

	currentGame = &arrayGames->at( IndexEditItem );
	ui->lineEdit_gamePath->setText( currentGame->path );
	ui->pushButton_removeGame->setEnabled( true );
	ui->toolButton_gamePath->setEnabled( true );
	ui->lineEdit_gamePath->setEnabled( true );
}

// ------------------------------------------------------------------------------------ //
// Clicked on "Select path game"
// ------------------------------------------------------------------------------------ //
void Window_EditGame::on_toolButton_gamePath_clicked()
{
	QString			path = QFileDialog::getExistingDirectory( this, "Select path to game" );
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
	try
	{
		for ( le::UInt32_t index = 0, count = arrayGames->size(); index < count; ++index )
		{
			GameDescriptor&			game = arrayGames->at( index );
			if ( game.name.isEmpty() )		throw std::runtime_error( "In game #" + std::to_string( index+1 ) + " name is empty" );
			if ( game.path.isEmpty() )		throw std::runtime_error( "In game #" + std::to_string( index+1 ) + " path is empty" );
		}
	}
	catch ( const std::exception& Exception )
	{
		Error_Info( Exception.what() );
		return;
	}

	done( RT_OK );
}

// ------------------------------------------------------------------------------------ //
// Event close window
// ------------------------------------------------------------------------------------ //
void Window_EditGame::closeEvent( QCloseEvent* Event )
{
	if ( Event->type() == QCloseEvent::Close )
		done( RT_CANCEL );

	Event->accept();
}
