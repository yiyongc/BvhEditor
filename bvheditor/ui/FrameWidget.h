#pragma once

#include <qbasictimer.h>
#include <QMouseEvent>
#include <qmenu.h>
#include "../controllers/LayerController.h"
#include "../data/LayerGroup.h"
#include "../controllers/LayerSelector.h"
#include "../data/SheetId.h"
#include "../controllers/ViewSettings.h"		

//#include "DataController/WorkspaceViewer.h"
//#include "DataController/PlayController.h"
//#include "CreateFrameDialog.h"

//#include "states/CACaniStateMachine.h"
#include "../controllers/CACStateMachine.h"
#include "../data/FrameId.h"

using namespace cacani::state;
using namespace cacani::controller;
using namespace cacani::data;

namespace cacani { 
	
	namespace data{
		struct 	FrameId;

	}
	namespace controller {
		class LayerViewer;
		class LayerSelector;
		class LayerController;
		class ViewSettings;
	}

	namespace state{
		class CACStateMachine;
	}

	
	namespace ui {

	class FrameWidget:public QWidget
	{
		Q_OBJECT

	public:
		//FrameWidget(const LayerGroup* root, ViewSettings* viewSettings, LayerController* layerController, LayerSelector* selector, CACStateMachine* sm, QWidget* parent);
		FrameWidget(ViewSettings* viewSettings, LayerController* layerController, LayerSelector* selector, CACStateMachine* sm, QWidget* parent);
		~FrameWidget();
		
		//For setting the preferred size when the widget first appear
		//or for Qt to set in a layout
		QSize sizeHint() const;
		QSize minimumSizeHint() const;
		QSize maximumSizeHint() const;

		//Calculate the largrest width of the entire layer structure
		//while taking into account the presense of any offset in any cel
		int calculateTotalWidth();
		//Calculate the pixel distance of the active frame from the left/right border for it to be shown.
		int calculateActiveScrollRange();
		//For setting the horizontal displacement for rendering and mouse events
		void setScrollPosX(int pos);
		//For setting the vertical displacement for rendering and mouse events
		void setScrollPosY(int pos);

		//For setting whether the pan tool has enter the frame widget
		void panToolIn(bool in);

		//For restarting animation and resetting the FrameWidget to the first frame
		void restartAnimation();

		void paintNormalized();

	Q_SIGNALS:
		//Signal for FCContainer to update the scrollbars. 
		//Triggered when moving sheets via dragging and dropping using the mouse
		void sliderBarRight(int value);
		void sliderBarLeft(int value);
		void sliderBarBottom(int value);
		void sliderBarTop(int value);
		void framePanned(int x, int y);
		
		//Trigger smaller grid view
		void smallGrid(bool smallMode);

	protected:
		//For handling the vertical scrolling to aid moving of layers to destination not yet in view.
		void timerEvent(QTimerEvent* e);

		void paintEvent(QPaintEvent* e);

		void mousePressEvent(QMouseEvent* e);
		void mouseMoveEvent(QMouseEvent* e);
		void mouseReleaseEvent(QMouseEvent* e);
		void mouseDoubleClickEvent(QMouseEvent* e);

		void contextMenuEvent(QContextMenuEvent* e);

	private:
		

		void mousePressNorm(QMouseEvent* e);
		void mouseMoveNorm(QMouseEvent* e);
		void mouseReleaseNorm(QMouseEvent* e);
		void mouseDblClkNorm(QMouseEvent*e );

		//Getting the assigned colors of sheets according to their type and the layer type
		void getLayerColors(QColor& keySelColor, QColor& keyUnselColor, QColor& tweenSelColor, QColor& tweenUnselColor, const Layer* l);

		//For locating a layer according to its pixel location
		void findLayer(int y, const Layer*& selectedLayer, const LayerGroup*& selectedParent);

		//Changing the mouse cursor accordingly to position and where it is
		void setMouseCursor(QMouseEvent* e);

		//Adding a series of consecutive frames to selection
		void addBandsofFrames(int startX, int endX);
		//Adding a series of consecutive frames to selection
		void addBandsofLayers(int startX, int endX, int starty, int endy);
		//Function to check if 2 ranges intersects
		bool rangeIntersects(int range1Top, int range1Bot, int range2Top, int range2Bot);

		//Getting the frame No. from the x coordinate and the excess pixel distance.
		void getFrame(int x, int& frame, int& leftover);
		//Function to deal with selection of frames
		void selectFrames(QMouseEvent* e, int frameOffset);

	private Q_SLOTS:
		void setSmallGrid(bool smallMode);

		//Show dialog for adding types of frames etc
		void showDuplicateDialog();
		//Remove selected frames
		void removeSelectedFrames();
		void refreshFPS(){ calculateTotalWidth();  paintNormalized(); }

		
	private:
		enum MouseMoveType
		{
			kNone,
			kAudioDragging,
			kFrameCreating,
			kFrameRemoving,
			kRubberBand,
			kMove,
			kCelOffset,
			kPan,
			kNoMove,
		};

		enum DisplayType
		{
			kMinimized,
			kNormalized,
		};

		enum MouseClickRegion
		{
			kSheetNone = 0x00000000,
			kCelRegion = 0x00000001,
			kSheetLeft = 0x00000002,
			kSheetRight = 0x00000004,
			kSheetMiddle = 0x00000008,
			kSheetNoDrag = 0x00000010,
		};

		enum MouseModifiers
		{
			kNoModifiers = 0x00000000,
			kShiftModifier = 0x02000000,
			kCtrlModifier = 0x04000000,
			kAltModifier = 0x08000000,
		};

		enum MouseClickType
		{
			kNoClick,
			kSingleClick,
			kDoubleClick,
		};

		enum MouseStateMode
		{
			kIdleMode,
			kPlayMode,
			kNoneMode,
			kOthers,
		}
		m_mouseStateMode;

	private:
		//const LayerGroup* m_root;
		//const Cut* m_root;

		CACStateMachine* m_stateMachine;
		LayerSelector* m_selector;
		LayerController* m_layerController;
		ViewSettings* m_viewSettings;

		QString m_audioDirectoryPath;

		QMouseEvent m_cacheEvent;

		QFontMetrics* m_fontMetrics;

		QBasicTimer* m_timer;
		QBasicTimer* m_scrollTimer;
		
		bool m_manhattan;
		bool m_showStrokeNums;
		bool m_hideNumber;
		bool m_timerUp;
		bool m_rubberBandSelection;
		bool m_panToolIn;

		int m_gridWidth;
		int m_scrollValueX;
		int m_scrollValueY;
		int m_startFrame;
		//GAOHE
		int m_frameOffset;
		int m_numberOfFrame;
		QPoint m_initialPoint;
		QPoint m_movingPoint;

		MouseMoveType m_mouseMoveType;
		DisplayType m_displayType;
		int  m_clickRegion;
		MouseModifiers m_mouseModifiers;
		MouseClickType m_mouseClickType;
		const Layer* m_clickedLayer;
		const LayerGroup* m_parentCel;
		LayerId m_celLid;

		QMenu* m_frameContext;
		QMenu* m_audioContext;

		QIcon m_lockIcon;
		
		QAction* m_deleteSelectedFrames;
		QAction* m_showExactStrokeNoActn;
		QAction* m_hideNumbersActn;
		QAction* m_duplicateSelection;

		QAction* m_propGroups;
		QAction* m_replaceGroups;

		QAction* m_smallGridView;

		QString m_directoryPath;

		//CreateFrameDialog* m_creatorDialog;
	};

}}