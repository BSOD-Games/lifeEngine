#ifndef WINDOW_EDITOR_H
#define WINDOW_EDITOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Window_Editor; }
QT_END_NAMESPACE

class Window_Editor : public QMainWindow
{
    Q_OBJECT

public:
    Window_Editor(QWidget *parent = nullptr);
    ~Window_Editor();

private:
    Ui::Window_Editor *ui;
};
#endif // WINDOW_EDITOR_H
