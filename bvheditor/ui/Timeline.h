#pragma once

#include <qwidget.h>	//

//#include "DataController/WorkspaceViewer.h"

#include "../controllers/CACStateMachine.h"		//#include "states/AbstractCACStateMachine.h"
#include "../controllers/LayerSelector.h"

using namespace cacani::state;
using namespace cacani::controller;

namespace cacani { 
	
	namespace controller {
		class LayerSelector;
	}

	namespace ui {

	class Timeline:public QWidget
	{
		Q_OBJECT

	public:
		Timeline(LayerSelector* selector, CACStateMachine* sm, QWidget* parent);
		~Timeline();

		//For setting the preferred size when the widget first appear
		//or for Qt to set in a layout
		QSize sizeHint() const;
		QSize minimumSizeHint() const;
		QSize maximumSizeHint() const;

		//For updating the horizontal frame displacement due to changes to the horizontal scroll bar
		void setScrollPosX(int pos);

	public Q_SLOTS:
		//For triggering different size display
		void setSmallGrid(bool smallMode);

	protected:
		void paintEvent(QPaintEvent* e);

		void mousePressEvent(QMouseEvent* e);
		void mouseMoveEvent(QMouseEvent* e);
		void mouseReleaseEvent(QMouseEvent* e);

	private:
		//Enumeration to differentiate the different type of mouse movement as determined by
		//the area clicked
		enum MoveMoveType
		{
			kFrameMoving,
			kLightBoxLeft,
			kLightBoxRight,
			kPlayRangeLeft,
			kPlayRangeRight,
			kSingleClick,
			kNone,
		}
		m_moveType;

		//For differentiating the different kinds of mouse interaction
		//according to the state of the system.
		enum MouseStateMode
		{
			kIdleMode,
			kPlayMode,
			kNoneMode,
			kOthers,
		}
		m_mouseStateMode;

	private:
		LayerSelector* m_selector;
		CACStateMachine* m_stateMachine;

		QPolygon m_leftTriangle;
		QPolygon m_rightTriangle;

		QPolygon m_leftBrace;
		QPolygon m_rightBrace;

		QPoint m_initialPoint;

		bool m_manhattan;

		int m_scrollPosX;
		int m_gridWidth;
	};

}}