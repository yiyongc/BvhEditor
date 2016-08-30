#pragma once

#include <QSplitter>
#include <QScrollbar>

#include "LayerCreatorWidget.h"
#include "FrameWidget.h"
#include "Timeline.h"
#include "../data/utf8string.h"

//#include "DataController/WorkspaceViewer.h"

//#include "states/CACaniStateMachine.h"
#include "../controllers/CACStateMachine.h"  
#include "../controllers/LayerSelector.h"
//#include "DataController/Observer.h"
//#include "DataController/ObserverEnum.h"
//#include "DataController/Incident.h"

using namespace cacani::state;
using namespace cacani::controller;

namespace cacani { 
	
	namespace controller {
		class LayerController;
		class ViewSettings;  //concrete class need
	}

	namespace ui {

	class FCContainer:public QSplitter  //, public Observer
	{
		Q_OBJECT

	public:
		FCContainer(const LayerGroup* data,
					ViewSettings* viewSettings,
					LayerController* layerController,
					LayerSelector* selector,
					CACStateMachine* sm,
					QWidget* parent);
		~FCContainer();

		//Observer-subject design pattern function for taking multiple inputs from controllers
		//Created to avoid flooding codes with and too much dependencies with Qt connect function
		void updateUi(int typeCode, int code);
		//Used for updating when performing undo, possible to merge with update ui
		void updateUndoReco();
		//For updating the frames portion without touching the layer portion after drawing.
		//Function used to scroll the frames widget without using the scrollbar
		//much like scrolling using a touch device.
		void panToolIn(bool in);

		FrameWidget* getFrameWidget(){ return m_frameWidget; }
		LayerCreatorWidget* getLayerCreatorWidget(){ return m_layerWidget; }
		void updateScrollView();

	public Q_SLOTS:
		void updateDrawing();

	protected:
		//Overriding resizing event to set proper values for the scrollbars
		void resizeEvent(QResizeEvent* e);
		//Overriding show event to calculate the first scrollbar values
		void showEvent(QShowEvent* e);

	

	private Q_SLOTS:

		void adjustScroll();
		//For setting the horizontal displacement for rendering and mouse events
		void setScrollForFrame(int value);
		//For setting the vertical displacement for rendering and scroll event.
		void setScrollForLayer(int value);
		//For calculation of the proper values of the vertical and horizontal scrollbars according to the layer and frame structure
		void adjustHorzScroll(bool adjustActive = false);
		void adjustVertScroll();

		//For updating the scrollbar values when the frames widget is panned via the mouse
		void scrollToChangeLeft(int value);
		void scrollToChangeRight(int value);
		void scrollToChangeTop(int value);
		void scrollToChangeBottom(int value);
		void frameWidgetPanned(int x, int y);


	private:
		QWidget* m_leftContainer;
		LayerCreatorWidget* m_layerWidget;
		QScrollBar* m_vertSB;

		QWidget* m_rightContainer;
		FrameWidget* m_frameWidget;
		Timeline* m_timeline;
		QScrollBar* m_horzSB;

		LayerSelector* m_selector;
		CACStateMachine* m_stateMachine;
		LayerController* m_layerController;

		bool m_showed;

	private:
		std::string getActiveLayerName() const;
	};

}}

