#include "Window_Editor.h"
#include "ui_Window_Editor.h"

Window_Editor::Window_Editor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Window_Editor)
{
    ui->setupUi(this);
}

Window_Editor::~Window_Editor()
{
    delete ui;
}

