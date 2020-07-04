//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WINDOW_EDITGAME_H
#define WINDOW_EDITGAME_H

#include <vector>
#include <QDialog>
#include <QModelIndex>

#include "gamedescriptor.h"

//---------------------------------------------------------------------//

class QListWidgetItem;

//---------------------------------------------------------------------//

namespace Ui
{
	class Window_EditGame;
}

//---------------------------------------------------------------------//

class Window_EditGame : public QDialog
{
	Q_OBJECT

public:

	enum RESULT_TYPE
	{
		RT_OK,
		RT_CANCEL
	};

	// Window_EditGame
	Window_EditGame( std::vector<GameDescriptor>& ArrayGames, QWidget* Parent = nullptr );
	~Window_EditGame();

private slots:
	void			on_pushButton_addGame_clicked();
	void			on_pushButton_removeGame_clicked();
	void			on_toolButton_gamePath_clicked();
	void			on_listWidget_games_currentRowChanged( int currentRow );
	void			on_pushButton_cancel_clicked();
	void			on_pushButton_ok_clicked();

protected:
	// QWidget
	virtual void	closeEvent( QCloseEvent* Event );

private:
	void			SetEditGame( int IndexEditItem );

	Ui::Window_EditGame*					ui;
	GameDescriptor*							currentGame;
	std::vector<GameDescriptor>*			arrayGames;
};

//---------------------------------------------------------------------//

#endif // WINDOW_EDITGAME_H
