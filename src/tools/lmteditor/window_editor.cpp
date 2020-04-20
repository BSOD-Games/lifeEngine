//////////////////////////////////////////////////////////////////////////
//
//			***  ***
//			Copyright (C)
//
// Репозиторий:
// Авторы:		Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////
#include "window_editor.h"
#include "ui_window_editor.h"

Window_Editor::Window_Editor(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Window_Editor)
{
	ui->setupUi(this);
}

Window_Editor::~Window_Editor()
{
	delete ui;
}
