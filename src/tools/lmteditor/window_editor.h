//////////////////////////////////////////////////////////////////////////
//
//			***  ***
//			Copyright (C)
//
// Репозиторий:
// Авторы:		Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////
#ifndef WINDOW_EDITOR_H
#define WINDOW_EDITOR_H

#include <QMainWindow>

namespace Ui {
	class Window_Editor;
}

class Window_Editor : public QMainWindow
{
	Q_OBJECT

public:
	explicit Window_Editor(QWidget *parent = nullptr);
	~Window_Editor();

private:
	Ui::Window_Editor *ui;
};

#endif // WINDOW_EDITOR_H
