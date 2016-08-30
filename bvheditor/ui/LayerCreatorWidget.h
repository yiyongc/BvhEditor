#pragma once

//#include "DataController/WorkspaceViewer.h"
//#include "DataController/PlayController.h"

#include "../controllers/CACStateMachine.h"		//#include "states/AbstractCACStateMachine.h"
#include "../controllers/LayerSelector.h"		//#include "DataController/AbstractLayerSelector.h"
#include "../data/Layer.h"
#include "../controllers/LayerController.h"
#include "../data/LayerGroup.h"
#include <QMouseEvent>
#include <qbasictimer.h>
#include <qmenu.h>
#include "InsertFrameDialog.h"

using namespace cacani::state;
using namespace cacani::controller;
using namespace cacani::ui;

namespace cacani { 
	
	namespace controller {
		class LayerController;
	}

	namespace ui {
		class InsertFrameDialog;
	class LayerCreatorWidget:public QWidget
	{
		Q_OBJECT

	public:
		LayerCreatorWidget(const LayerGroup* root,
							LayerController* m_layerController,
							LayerSelector* selector,
							CACStateMachine* sm,
							QWidget* parent);
		~LayerCreatorWidget();

		//For setting the preferred size when the widget first appear
		//or for Qt to set in a layout
		QSize sizeHint() const;
		QSize minimumSizeHint() const;
		QSize maximumSizeHint() const;
		
		//Calculate the pixel height of the entire layer structure
		int calculateTotalHeight();
		//For setting the vertical displacement for rendering and mouse events
		void setScrollPosY(int pos);

		//For enabling and disabling options in the context menu
		void updateMenus();

	Q_SIGNALS:
		//Signal for FCContainer to update the scrollbars. 
		//Triggered when moving layers via dragging and dropping using the mouse
		void sliderBarBottom(int value);
		void sliderBarTop(int value);

	protected:
		void paintEvent(QPaintEvent* e);

		void mousePressEvent(QMouseEvent* e);
		void mouseMoveEvent(QMouseEvent* e);
		void mouseReleaseEvent(QMouseEvent* e);
		void mouseDoubleClickEvent(QMouseEvent* e);

		void contextMenuEvent(QContextMenuEvent* e);

		//For handling the vertical scrolling to aid moving of layers to destination not yet in view.
		void timerEvent(QTimerEvent* e);

		virtual void createContextMenu();

	protected Q_SLOTS:
		//For adding a new cel (child of the root)
		void addNewCel();
		//For adding a new drawing layer (child of an existing cel)
		void addDrawingLayer();
		
		//Create a new layer and duplicate the contents of the currently selected layer
		//into the new layer
		void duplicateActiveLayer();
		//Create a new Cel and any child layers corresponding to the child layers of the current cel
		//then duplicating the contents and inserting them to the appropriate layer/cel.
		void duplicateActiveCel();

		//Deleting the selected cels/ layers
		void removeSelectedCels();
		void removeSelectedLayers();

		//Each layer is taggd with a color identifier. Below function allow us to select other layers with the same
		//color tag
		void selectSimilarColors();

		//For setting or removing the layer's color tag.
		void setNoTag();
		void setOrangeTag();
		void setYellowTag();
		void setPaleGreenTag();
		void setGreenTag();
		void setSkyBlueTag();
		void setUltraMarineTag();
		void setPurpleTag();
		void setRedTag();

		void deleteFrame();
		void deleteFrames();

		void addFrames();
		void addOne();
		void addFive();
		void addTen();

	private:
		//Used for chagning the rendering size of each frames
		enum DisplayType
		{
			kMinimized,
			kNormalized,
		}
		m_displayType;

		//Used for differentiating the different clicking region on a single layer
		enum MouseClickRegion
		{
			kExpandIcon,
			kColorBox,
			kSliderRegion,
			kVisibleStatusRegion,
			kLockStatusRegion,
			kOnionStatusRegion,
			kOtherRegion,
			kNone,
		}
		m_mouseClickRegion;

		//Used for differentiating the different type of mouse moivng
		enum MouseMovingType
		{
			kCelMoving,
			kLayerMoving,
			kMoveIntoCel,
			kNoMoving
		}
		m_mouseMovingType;

	private:
		void paintMinimized(QPaintEvent* e);
		void paintNormal(QPaintEvent* e);
		//For painting a layer
		void drawLayer(QPainter& p, QRect& layerRect, int top, const Layer* l, bool isCel);

		//For locating a layer according to its pixel location
		void findLayer(int y, const Layer*& selectedLayer, const LayerGroup*& selectedParent);
		//Cel mouse hit testing
		void moveCelHitTest(int y);
		//Layer mouse hit testing
		void moveLayerHitTest(int y);

		//Moving a layer to its destination  recognized by its layerId via drag and drop
		void moveSelectedLayer(const LayerId& lid, const LayerId& destId);
		//Moving the currently selected layers to a destination recognized by its layerId
		void moveSelectedLayers(const LayerId& destId);
		//Moving the currently selected layers into a cel, called when the drag and drop ends
		//on a cel
		void moveSelectedLayerIntoCel(const LayerId& lid, const LayerId& destId);
		//Moving the currently selected layers into a cel, called when the drag and drop ends
		//on a layer that is a child of that cel
		void moveSelectedLayersToCel(const LayerId& destId);

		

	private:
		LayerSelector* m_selector;
		CACStateMachine* m_stateMachine;
		LayerController* m_layerController;
		const LayerGroup* m_root;
		//const Cut* m_root;

		QMouseEvent m_cacheEvent;

		QBasicTimer* m_scrollTimer;

		int m_scrollPosY;
		int m_dragLayerLine;
		QRect m_dragBox;

		bool m_manhattan;

		QIcon m_lockIcon;
		QIcon m_visibleIcon;
		QIcon m_onionIcon;
		QIcon m_muteIcon;
		QIcon m_unmuteIcon;

		QPolygon m_expandedTriangle;
		QPolygon m_closedTriangle;

		const Layer* m_clickedLayer;
		int m_clickedALIndex;
		LayerId m_lidCache;
		LayerId m_movingDesId;
		const LayerGroup* m_parentCel;
		int m_selectedAudioCache;

		QPoint m_initialPoint;
		QRect m_interactingRect;

		QAction* m_addCelActn;
		QAction* m_addDrawingLayerActn;
		QAction* m_duplicateLayerActn;
		QAction* m_duplicateCelActn;
		QAction* m_removeSelectedCelActn;
		QAction* m_removeSelectedLayersActn;

		QAction* m_addOne;
		QAction* m_addFive;
		QAction* m_addTen;
		QAction* m_addFrames;

		QAction* m_deleteFrame;
		QAction* m_deleteFrames;

		QMenu* m_contextMenu;

		QAction* m_selectSimilarColor;
		QAction* m_noColorTag;
		QAction* m_orangeTag;
		QAction* m_yellowTag;
		QAction* m_paleGreenTag;
		QAction* m_greenTag;
		QAction* m_skyBlueTag;
		QAction* m_ultraMarineTag;
		QAction* m_purpleTag;
		QAction* m_redTag;

		QAction* m_histActn;

		QMenu* m_colorContext;

		QString m_audioDirectoryPath;
	};

}}