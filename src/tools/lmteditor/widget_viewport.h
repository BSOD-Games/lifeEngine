//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_VIEWPORT_H
#define WIDGET_VIEWPORT_H

#include <QWidget>
#include <QTimer>

#include "engine/icamera.h"
#include "studiorender/istudiorenderinternal.h"

//---------------------------------------------------------------------//

class Widget_Viewport : public QWidget
{
	Q_OBJECT

public:	
	// QPaintDevice
	virtual QPaintEngine*			paintEngine() const;

	// QWidget
	virtual void					paintEvent( QPaintEvent* Event );
	virtual void					resizeEvent( QResizeEvent* Event );
	virtual void					showEvent( QShowEvent* Event );

	// Widget_Viewport
	Widget_Viewport( QWidget* Parent = nullptr );
	~Widget_Viewport();

	bool					Initialize();

private slots:
	void					Render();

private:
	bool							isInitialize;
	QTimer							timerUpdate;
	le::IStudioRenderInternal*		studioRender;
};

//---------------------------------------------------------------------//

#endif // WIDGET_VIEWPORT_H
