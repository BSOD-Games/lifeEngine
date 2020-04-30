//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_NODETECHNIQUE_H
#define WIDGET_NODETECHNIQUE_H

#include <qwidget.h>

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_NodeTechnique;
}

//---------------------------------------------------------------------//

class Widget_NodeTechnique : public QWidget
{
	Q_OBJECT

public:
	// Widget_NodeTechnique
	Widget_NodeTechnique( QWidget* Parent = nullptr );
	~Widget_NodeTechnique();

	// TODO: Add method for getting type technique
	quint32				GetCountPasses() const;

signals:
	void				CountPassesChanged( quint32 Value );

private slots:
	void				on_spinBox_countPass_valueChanged( int Value );

private:
	Ui::Widget_NodeTechnique*			ui;
};

//---------------------------------------------------------------------//

#endif // !WIDGET_NODETECHNIQUE_H

