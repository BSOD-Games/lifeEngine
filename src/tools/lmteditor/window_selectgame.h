//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WINDOW_SELECTGAME_H
#define WINDOW_SELECTGAME_H

#include <vector>
#include <QDialog>

#include "gamedescriptor.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Window_SelectGame;
}

//---------------------------------------------------------------------//

class Window_SelectGame : public QDialog
{
	Q_OBJECT

public:
	enum RESULT_TYPE
	{
		RT_OK,
		RT_CANCEL
	};

	// Window_SelectGame
	Window_SelectGame( QWidget* Parent = nullptr );
	~Window_SelectGame();

	GameDescriptor			GetGame() const;

private slots:
	void				on_pushButton_editGames_clicked();
	void				on_pushButton_ok_clicked();
	void				on_pushButton_cancel_clicked();

protected:
	// QWidget
	virtual void		closeEvent( QCloseEvent* event );

private:
	void				UpdateListGames();

	Ui::Window_SelectGame*			ui;
	std::vector<GameDescriptor>		games;
};

//---------------------------------------------------------------------//

#endif // WINDOW_SELECTGAME_H
