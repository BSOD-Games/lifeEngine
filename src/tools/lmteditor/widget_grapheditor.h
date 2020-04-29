//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_GRAPHEDITOR_H
#define WIDGET_GRAPHEDITOR_H

#include <QWidget>
#include <qlayout.h>
#include <nodes/FlowScene>
#include <nodes/FlowView>

//---------------------------------------------------------------------//

class Widget_GraphEditor : public QWidget
{
	Q_OBJECT

public:
	// Widget_GraphEditor
	Widget_GraphEditor( QWidget* Parent = nullptr );
	~Widget_GraphEditor();

private:
	static std::shared_ptr<QtNodes::DataModelRegistry>			RegisterDataModels();

	bool					isNodeMaterialExist;

	QVBoxLayout*			vboxLayout;
	QtNodes::FlowScene*		flowScene;
	QtNodes::FlowView*		flowView;
};

//---------------------------------------------------------------------//

#endif // WIDGET_GRAPHEDITOR_H