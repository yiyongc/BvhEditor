#include <QWidget>
#include <QEvent>
#include <QStandardPaths>
#include <QIcon>
#include <QBasicTimer>
#include <QAction>
#include <QTextCodec>
#include <QMenu>
#include <QPainter>
#include <QColor>
#include <QMouseEvent>
#include <QApplication>
#include <QByteArray>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>


#include "LayerCreatorWidget.h"
#include "FrameCreatorDefs.h"
#include "../data/utf8string.h"
//#include "DataController/LayerColorDef.h"
//#include "DualOptionsDialog.h"

#include "../controllers/LayerSelector.h"     //#include "DataController/LayerSelector.h"
#include "../QsLog2/qslog.h"

#ifndef DISABLE_AUDIO
#define DISABLE_AUDIO
#endif


using namespace cacani::data;			//
using namespace cacani::controller;		//
using namespace cacani::state;			//



namespace cacani { 
	
	namespace state{
		class CACStateMachine; //
	}
	
	namespace ui {
		
		LayerCreatorWidget::LayerCreatorWidget(const LayerGroup* root, LayerController* layerController, LayerSelector* selector, CACStateMachine* sm, QWidget* parent)
		:QWidget(parent), m_stateMachine(sm), m_root(root), m_selector(selector), m_layerController(layerController)
		, m_cacheEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), Qt::NoButton, 0, 0)
	{
		QLOG_INFO() << "LayerCreatorWidget:" << "Started";

		QPalette pal(palette());
		pal.setColor(QPalette::Background, LAYER_BKGND_COLOR);
		setAutoFillBackground(true);
		setPalette(pal);

		m_mouseMovingType = LayerCreatorWidget::kNoMoving;
		m_displayType = LayerCreatorWidget::kNormalized;

		m_expandedTriangle<<QPoint(-LAYER_EXPANDTRI_WIDTH/2, -LAYER_EXPANDTRI_HEIGHT/2)
			<<QPoint(LAYER_EXPANDTRI_WIDTH/2, -LAYER_EXPANDTRI_HEIGHT/2)
			<<QPoint(0, LAYER_EXPANDTRI_HEIGHT/2);

		m_closedTriangle<<QPoint(-LAYER_EXPANDTRI_HEIGHT/2, -LAYER_EXPANDTRI_WIDTH/2)
			<<QPoint(-LAYER_EXPANDTRI_HEIGHT/2, LAYER_EXPANDTRI_WIDTH/2)
			<<QPoint(LAYER_EXPANDTRI_HEIGHT/2, 0);

		m_manhattan = false;

		m_mouseClickRegion = LayerCreatorWidget::kNone;
		m_displayType = LayerCreatorWidget::kNormalized;

		m_scrollPosY = 0;

		m_audioDirectoryPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();

		m_lockIcon=QIcon(QPixmap(QString::fromUtf8(":/app_drawing/res/Layer/ls_lock_light.png")));
		m_visibleIcon=QIcon(QPixmap(QString::fromUtf8(":/app_drawing/res/Layer/ls_eye_light.png")));
		m_onionIcon=QIcon(QPixmap(QString::fromUtf8(":/app_drawing/res/Layer/ls_lightbox_light.png")));
		m_muteIcon=QIcon(QPixmap(QString::fromUtf8(":/app_drawing/res/Layer/cb_mute.png")));
		m_unmuteIcon=QIcon(QPixmap(QString::fromUtf8(":/app_drawing/res/Layer/cb_audio.png")));

		createContextMenu();

		m_scrollTimer = new QBasicTimer();
		QLOG_INFO() << "LayerCreatorWidget:" << "construction completed";

	}

	void LayerCreatorWidget::createContextMenu()
	{
		m_addCelActn = new QAction(tr("New Cel"), this);
		connect(m_addCelActn, SIGNAL(triggered()), this, SLOT(addNewCel()));

		m_addDrawingLayerActn = new QAction(tr("New Drawing Layer"), this);
		connect(m_addDrawingLayerActn, SIGNAL(triggered()), this, SLOT(addDrawingLayer()));
		m_duplicateLayerActn = new QAction(tr("Duplicate Active Layer"), this);
		connect(m_duplicateLayerActn, SIGNAL(triggered()), this, SLOT(duplicateActiveLayer()));

		m_duplicateCelActn = new QAction(tr("Duplicate Active Cel"), this);
		connect(m_duplicateCelActn, SIGNAL(triggered()), this, SLOT(duplicateActiveCel()));

		m_removeSelectedCelActn = new QAction(tr("Remove Cel(s)"), this);
		connect(m_removeSelectedCelActn, SIGNAL(triggered()), this, SLOT(removeSelectedCels()));

		m_removeSelectedLayersActn = new QAction(tr("Remove Layer(s)"), this);
		connect(m_removeSelectedLayersActn, SIGNAL(triggered()), this, SLOT(removeSelectedLayers()));

		m_deleteFrame = new QAction(tr("Delete Current Frame"), this);
		connect(m_deleteFrame, SIGNAL(triggered()), this, SLOT(deleteFrame()));

		m_deleteFrames = new QAction(tr("Delete All Frames"), this);
		connect(m_deleteFrames, SIGNAL(triggered()), this, SLOT(deleteFrames()));

		m_addOne = new QAction(tr("Quick Add One Frame"), this);
		connect(m_addOne, SIGNAL(triggered()), this, SLOT(addOne()));

		m_addFive = new QAction(tr("Quick Add Five Frames"), this);
		connect(m_addFive, SIGNAL(triggered()), this, SLOT(addFive()));

		m_addTen = new QAction(tr("Quick Add Ten Frames"), this);
		connect(m_addTen, SIGNAL(triggered()), this, SLOT(addTen()));

		m_addFrames = new QAction(tr("Add Frames To Current Layer"), this);
		connect(m_addFrames, SIGNAL(triggered()), this, SLOT(addFrames()));

#ifndef DISABLE_AUDIO	
		m_removeAudioLayer = new QAction(tr("Remove Audio Layer"), this);
		connect(m_removeAudioLayer, SIGNAL(triggered()), this, SLOT(removeAudioLayer()));
#endif

		m_contextMenu = new QMenu(this);
		//m_contextMenu->addAction(m_addCelActn);
		//m_contextMenu->addAction(m_addDrawingLayerActn);
		//m_contextMenu->addSeparator();
		//m_contextMenu->addAction(m_duplicateCelActn);
		//m_contextMenu->addAction(m_duplicateLayerActn);
		//m_contextMenu->addSeparator();
		//m_contextMenu->addAction(m_removeSelectedCelActn);
		//m_contextMenu->addAction(m_removeSelectedLayersActn);
		m_contextMenu->addAction(m_addFrames);
		m_contextMenu->addSeparator();
		m_contextMenu->addAction(m_addOne);
		m_contextMenu->addAction(m_addFive);
		m_contextMenu->addAction(m_addTen);
		m_contextMenu->addSeparator();
		m_contextMenu->addAction(m_deleteFrame);
		m_contextMenu->addAction(m_deleteFrames);

#ifndef DISABLE_AUDIO
		m_contextMenu->addAction(m_removeAudioLayer);
		m_contextMenu->addSeparator();
#endif

		m_selectSimilarColor = new QAction(tr("Select Similar Color Tag"), this);
		connect(m_selectSimilarColor, SIGNAL(triggered()), this, SLOT(selectSimilarColors()));

		m_noColorTag = new QAction(tr("No Color Tag"), this);
		m_noColorTag->setCheckable(true);
		connect(m_noColorTag, SIGNAL(triggered()), this, SLOT(setNoTag()));

		m_orangeTag = new QAction(tr("Orange Tag"), this);
		m_orangeTag->setCheckable(true);
		connect(m_orangeTag, SIGNAL(triggered()), this, SLOT(setOrangeTag()));

		m_yellowTag = new QAction(tr("Yellow Tag"), this);
		m_yellowTag->setCheckable(true);
		connect(m_yellowTag, SIGNAL(triggered()), this, SLOT(setYellowTag()));

		m_paleGreenTag = new QAction(tr("Pale Green Tag"), this);
		m_paleGreenTag->setCheckable(true);
		connect(m_paleGreenTag, SIGNAL(triggered()), this, SLOT(setPaleGreenTag()));

		m_greenTag = new QAction(tr("Green Tag"), this);
		m_greenTag->setCheckable(true);
		connect(m_greenTag, SIGNAL(triggered()), this, SLOT(setGreenTag()));

		m_skyBlueTag = new QAction(tr("Sky Blue Tag"), this);
		m_skyBlueTag->setCheckable(true);
		connect(m_skyBlueTag, SIGNAL(triggered()), this, SLOT(setSkyBlueTag()));

		m_ultraMarineTag = new QAction(tr("Ultra Marine Tag"), this);
		m_ultraMarineTag->setCheckable(true);
		connect(m_ultraMarineTag, SIGNAL(triggered()), this, SLOT(setUltraMarineTag()));

		m_purpleTag = new QAction(tr("Purple Tag"), this);
		m_purpleTag->setCheckable(true);
		connect(m_purpleTag, SIGNAL(triggered()), this, SLOT(setPurpleTag()));

		m_redTag = new QAction(tr("Red Tag"), this);
		m_redTag->setCheckable(true);
		connect(m_redTag, SIGNAL(triggered()), this, SLOT(setRedTag()));

		m_histActn = 0;

		m_colorContext = new QMenu(this);
		m_colorContext->addAction(m_selectSimilarColor);
		m_colorContext->addSeparator();
		m_colorContext->addAction(m_orangeTag);
		m_colorContext->addAction(m_yellowTag);
		m_colorContext->addAction(m_paleGreenTag);
		m_colorContext->addAction(m_greenTag);
		m_colorContext->addAction(m_skyBlueTag);
		m_colorContext->addAction(m_ultraMarineTag);
		m_colorContext->addAction(m_purpleTag);
		m_colorContext->addAction(m_redTag);
	}

	LayerCreatorWidget::~LayerCreatorWidget()
	{
		delete m_scrollTimer;
	}

	QSize LayerCreatorWidget::sizeHint() const
	{
		return QSize(LAYER_WIDGET_WIDTH, FRAMEWID_HEIGHT);
	}

	QSize LayerCreatorWidget::minimumSizeHint() const
	{
		return QSize(LAYER_WIDGET_WIDTH, FRAMEWID_HEIGHT);
	}

	QSize LayerCreatorWidget::maximumSizeHint() const
	{
		return QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
	}

	int LayerCreatorWidget::calculateTotalHeight()
	{
		QLOG_INFO() << "LayerCreatorWidget:" << " calculateTotalHeight() " << "called";

		int totalHeight = 0;
		int accumHeight = 0;

		for(size_t i=0; i<m_root->memberCount(); ++i)
		{
			const LayerGroup* cel = dynamic_cast<const LayerGroup*>(m_root->childAtIndex(i));

			int rectHeight = (SHEET_BORDER * 2) + (cel->isExpanded() ? (static_cast<int>(cel->memberCount()) + 1) : 1)*SHEET_HEIGHT;
			if (cel->isExpanded())
				rectHeight += static_cast<int>(cel->memberCount())*TIMELINE_NORM_TOP_BUFFER;

			totalHeight+=rectHeight + TIMELINE_INTER_CEL;
		}
		QLOG_INFO() << "LayerCreatorWidget:" << " calculateTotalHeight() " << "completed, totalHeight = " << totalHeight;

		return totalHeight;

	}

	void LayerCreatorWidget::setScrollPosY(int pos)
	{
		m_scrollPosY = pos;

		if(m_cacheEvent.buttons() != Qt::NoButton && m_cacheEvent.type() == QEvent::MouseMove)
			mouseMoveEvent(&m_cacheEvent);

		update();
	}

	void LayerCreatorWidget::paintEvent(QPaintEvent* e)
	{
		if(m_displayType == LayerCreatorWidget::kNormalized)
			paintNormal(e);
		else if(m_displayType == LayerCreatorWidget::kMinimized)
			paintMinimized(e);
	}
	
	//Function to paint the layers in a minimized view. Not used at the moment
	void LayerCreatorWidget::paintMinimized(QPaintEvent* e)
	{
	}

	//Function to paint the layers in a normal view.
	void LayerCreatorWidget::paintNormal(QPaintEvent* e)
	{
		QLOG_INFO() << "LayerCreatorWidget:" << " paintNormal() " << "called";

		QPainter p(this);

		//m_scrollPosY is a variable that describles the current vertical displacement of the layer display according to 
		//the scroll bar in FCContainer
		p.translate(0, -m_scrollPosY);
		//accumHeight is an integer that keep tracks of the y coord of the next rectangle that a cel and its children is going to be rendered into
		int accumHeight = 0;
		//layerTop is an integer that keep tracks of the y coord of the next rectable that a layer is going to be rendered into
		int layerTop = 0;
		
		//Render the drawing layers
		//Looping through the Cels.

		for(size_t i=0; i<m_root->memberCount(); ++i)
		{
			
			const LayerGroup* cel = dynamic_cast<const LayerGroup*>(m_root->childAtIndex(i));
			LayerId celId = m_selector->findLayerId(cel);
			//Width of the rectangle for the cel is the same as the width of the widget
			int rectWidth = width();
			//Height of the rectangle is the height of each layer rect + the margin between each layer * the total number of children + the cel
			//Each cel and its children layers are enveloped in a dark-colored rectangle.
			int rectHeight = (SHEET_BORDER * 2) + (cel->isExpanded() ? (static_cast<int>(cel->memberCount()) + 1) : 1)*SHEET_HEIGHT;
			

			if (cel->isExpanded())
				rectHeight += static_cast<int>(cel->memberCount())*TIMELINE_NORM_TOP_BUFFER;

			QRect bkRect(0, accumHeight, rectWidth, rectHeight);
			//y coordinate of the cel to be rendered
			int layerTop = SHEET_BORDER + accumHeight;

			//checking if the cel block intersects with the current widget rendering window after taking into account scrolling.
			//If intersects, render the cel and its contents.
			if(bkRect.adjusted(0, -m_scrollPosY, 0, -m_scrollPosY).intersects(rect()))
			{
				p.setPen(Qt::NoPen);
				p.setBrush(LAYER_CEL_BKGND_COLOR);
				p.drawRect(bkRect);

				QRect celRect(0, layerTop, width(), SHEET_HEIGHT);
				//Check if the cel is selected.
				//if selected, render in different color;
				if (m_selector->layerIsSelected(celId))
				{
					QRect selLayerRect(0, layerTop, width(), SHEET_HEIGHT);
					selLayerRect.adjust(0, -1, 0, 1);
					p.setPen(Qt::NoPen);
					p.setBrush(LAYER_SELECTED_LAYER_COLOR);
					p.drawRect(selLayerRect);
				}


				//TODO
				drawLayer(p, celRect, layerTop, cel, true);



				//Calculate next layer y coordinate.
				layerTop += SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER;
				//Check if the cel is expanded. 
				//If expanded, render its children

				//currently hardcoded to be 1
				if (cel->isExpanded())
				{
					for(size_t j=0; j<cel->memberCount(); ++j)
					{
						const Layer* l = cel->childAtIndex(j);
						QRect layerRect(0, layerTop, width(), SHEET_HEIGHT);
						LayerId lid = m_selector->findLayerId(l);

						//checking if the layer rect intersects with the current widget rendering window after taking into account scrolling.
						//If intersects, render the layer.
						if(layerRect.adjusted(0, -m_scrollPosY, 0, -m_scrollPosY).intersects(rect()))
						{
							//Check if the layer is selected.
							//if selected, render in different color;
							if (m_selector->layerIsSelected(lid))
							{
								QRect selLayerRect(0, layerTop, width(), SHEET_HEIGHT);
								selLayerRect.adjust(0, -1, 0, 1);
								p.setPen(Qt::NoPen);
								p.setBrush(LAYER_SELECTED_LAYER_COLOR);
								p.drawRect(selLayerRect.adjusted(LAYER_TYPE_TAB, 0, 0, 0));
							}

							drawLayer(p, layerRect, layerTop, l,false);
						}

						layerTop += SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER;
					}
				}
			}
			accumHeight += rectHeight + TIMELINE_INTER_CEL;

		}

		p.resetTransform();
		p.setPen(QPen(LAYER_BOTTOM_HL, 1));
		p.drawLine(0, rect().top(), width(), rect().top());

		//Drag line;
		//For rendering of indicators when dragging and dropping to alter the layer arrangments
		if(m_movingDesId.indent() > 0)
		{
			p.translate(0, -m_scrollPosY);
			//if destination is to a cel or layer, draw a line to indicate destination
			if(m_mouseMovingType == LayerCreatorWidget::kCelMoving || m_mouseMovingType == LayerCreatorWidget::kLayerMoving)
			{
				p.setPen(QPen(DRAG_LINE_COLOR, 1));
				p.drawLine(0, m_dragLayerLine, width(), m_dragLayerLine);
			}
			//if destination is INTO a cel, draw a box around the cel.
			else if(m_mouseMovingType == LayerCreatorWidget::kMoveIntoCel)
			{
				p.setPen(QPen(DRAG_LINE_COLOR, 1));
				p.setBrush(Qt::NoBrush);
				p.drawRect(m_dragBox);
			}
		}

		QLOG_INFO() << "LayerCreatorWidget:" << " paintNormal() " << "completed";

	}
	
	//Layer drawing function, with a boolean flag to determine if it is a cel.
	void LayerCreatorWidget::drawLayer(QPainter& p, QRect& layerRect, int top, const Layer* l, bool isCel)
	{
		QLOG_INFO() << "LayerCreatorWidget:" << " drawLayer() " << "called";

		//Defining the rectangle that will be rendered for the Layer's color tag
		QRect colorRect(LAYER_COLOR_LEFT, layerRect.top() + (layerRect.height() - LAYER_COLOR_WIDTH)/2, LAYER_COLOR_WIDTH, LAYER_COLOR_WIDTH);
		//If not a cel, get its layer's tag's color and render
		if(!isCel)
		{
			p.setPen(Qt::NoPen);
			////QColor layerColor(l->getColor().r(), l->getColor().g(), l->getColor().b());
			////p.setBrush(layerColor);
			//TODO: The red color (255,0,0) for temp testing only
			p.setBrush(QColor(255,0,0)); //
			p.drawRect(colorRect);
		}
		//If cel, render a triangle as indicator of whether it is expanded; downward pointing triangle if expanded
		//Right side pointing triangle if not.
		else
		{
			p.translate(LAYER_EXPANDTRI_WIDTH/2 + 2, colorRect.center().y());
			p.setBrush(LAYER_STRING_COLOR);

			if(l->isExpanded())
				p.drawPolygon(m_expandedTriangle);
			else
				p.drawPolygon(m_closedTriangle);

			p.resetTransform();
			p.translate(0, -m_scrollPosY);
		}

		//Drawing the opacity slider. Which is just a rect with a dark color background, with a lighter color indicating
		colorRect.adjust(LAYER_COLOR_WIDTH + LAYER_SLIDER_BUFFER, 0, LAYER_COLOR_WIDTH + LAYER_SLIDER_BUFFER, 0);
		p.setBrush(LAYER_SLIDER_COLOR_F);
		p.setPen(Qt::NoPen);
		p.drawRect(colorRect);
		if(l->getOpacity() != 100)
		{
			//Convert opacity value to a percentage/floating value, then use that to determine the height of the "slider" rect
			//to render
			float percentage = float(100 - l->getOpacity())/100.0f;
			int bottomHeight = static_cast<int>(percentage*colorRect.height());
			colorRect.setBottom(colorRect.top() + bottomHeight - 1);
			p.setBrush(LAYER_SLIDER_BKGND);
			p.drawRect(colorRect);
		}

		//Drawing the name
		//Defining the rectangle which the name of the layer will render into.
		QRect nameRect(colorRect.right() + LAYER_COLOR_LEFT, LAYER_TOP_BUFFER+top,
			rect().right() - LAYER_STATUS_WIDTH*3 - LAYER_RIGHT_BUFFER - colorRect.right() - LAYER_COLOR_LEFT,
			SHEET_HEIGHT);

		//Converting the stored name from the layer to a UTF-8 QString before rendering.
		//TODO  
		//QByteArray nameByte = l->getName().rawData();
		QByteArray nameByte = l->GetShortFileName().c_str(); //

		QTextCodec *codec=QTextCodec::codecForName("UTF-8");
		QString name = codec->toUnicode(nameByte);
		p.setPen(QPen(LAYER_STRING_COLOR, 1));
		p.drawText(nameRect, Qt::AlignVCenter | Qt::AlignLeft, name);

		//Drawing the 3 boxes for the 3 statuses, Visibility, Locking and Onion Skinning
		//Boxes are defined first, then the status are rendered using icons previously loaded.
		QRect statusBox(nameRect.right()+1, layerRect.center().y()-LAYER_STATUS_WIDTH/2-1, LAYER_STATUS_WIDTH, LAYER_STATUS_WIDTH);
		if(l->isVisible())
			m_visibleIcon.paint(&p, statusBox);
		else
		{
			p.drawPoint(statusBox.center());
		}

		statusBox.adjust(LAYER_STATUS_WIDTH, 0, LAYER_STATUS_WIDTH, 0);
		if(l->isLocked())
			m_lockIcon.paint(&p, statusBox);
		else
		{
			p.drawPoint(statusBox.center());
		}

		statusBox.adjust(LAYER_STATUS_WIDTH, 0, LAYER_STATUS_WIDTH, 0);
		if(l->isOnionSkinOn())
			m_onionIcon.paint(&p, statusBox);
		else
		{
			p.drawPoint(statusBox.center());
		}
	}

	void LayerCreatorWidget::mousePressEvent(QMouseEvent* e)
	{
		QLOG_INFO() << "LayerCreatorWidget:" << " mousePressEvent() " << "called";

		m_cacheEvent = QMouseEvent(QEvent::MouseButtonPress, e->pos(), e->button(), e->buttons(),
			e->modifiers());

		if(e->button() == Qt::LeftButton || e->button() == Qt::RightButton)
		{
			m_initialPoint = e->pos();
			int accumHeight = 0;
		
			//Hit testing for layers
			const Layer* selectedLayer = 0;
			const LayerGroup* selectedParent = 0;

			int layerTop, layerBottom;
			QRect layerRect;
			int i = 0;
			
			for(size_t i=0; i<m_root->memberCount(); ++i)
			{
				const LayerGroup* cel = dynamic_cast<const LayerGroup*>(m_root->childAtIndex(i));

				int rectHeight = (SHEET_BORDER * 2) + (cel->isExpanded() ? (static_cast<int>(cel->memberCount()) + 1) : 1)*SHEET_HEIGHT;
				if (cel->isExpanded())
					rectHeight += static_cast<int>(cel->memberCount())*TIMELINE_NORM_TOP_BUFFER;

				//Hit test check for cel
				int finalY = e->y() + m_scrollPosY;
				if(finalY >= accumHeight && finalY <= rectHeight+accumHeight)
				{
					int levely = finalY - accumHeight;
					layerTop = SHEET_BORDER + SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER/2;
					layerBottom = rectHeight - SHEET_BORDER - SHEET_HEIGHT- TIMELINE_NORM_TOP_BUFFER/2;
					if (cel->isExpanded())
					{
						if(levely >= 0 && levely <= layerTop)
						{
							selectedLayer = (Layer*)cel;
							selectedParent = cel;
							layerRect = QRect(0, SHEET_BORDER + accumHeight, width(), SHEET_HEIGHT);
							break;
						}
						else if(levely <= rectHeight && levely >= layerBottom)
						{
							if(cel->memberCount() == 0)
							{
								selectedLayer = (Layer*)cel;
								selectedParent = cel;
								layerRect = QRect(0, SHEET_BORDER + accumHeight, width(), SHEET_HEIGHT);
								break;
							}
							else
							{
								selectedLayer = cel->childAtIndex(cel->memberCount() - 1);
								selectedParent = cel;
								layerRect = QRect(0, SHEET_BORDER + SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER + accumHeight + (static_cast<int>(cel->memberCount())-1)*(SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER),
									width(), SHEET_HEIGHT);
								break;
							}
						}
						else if(cel->memberCount() >= 2)
						{
							int layerY = (levely - layerTop)/(SHEET_HEIGHT+TIMELINE_NORM_TOP_BUFFER);
							selectedLayer = cel->childAtIndex(layerY);
							layerRect = QRect(0, SHEET_BORDER+ SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER + accumHeight + layerY*(SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER),
								width(), SHEET_HEIGHT);
							selectedParent = cel;
							break;
						}
					}
					else
					{
						selectedLayer = (Layer*)cel;
						selectedParent = cel;
						layerRect = QRect(0, SHEET_BORDER + accumHeight, width(), SHEET_HEIGHT);
					}
				}

				if(selectedLayer && selectedParent)
				{
					break;
				}

				accumHeight += rectHeight + TIMELINE_INTER_CEL;
			}
				
			if(selectedLayer && selectedParent)
			{
				m_clickedLayer = selectedLayer;
				m_parentCel = selectedParent;
				m_lidCache = m_selector->findLayerId(selectedLayer, selectedParent);

				//Check Region of click
				if(e->button() == Qt::LeftButton)
				{
					QRect colorRect(LAYER_COLOR_LEFT, layerRect.top() + (layerRect.height() -LAYER_COLOR_WIDTH)/2, LAYER_COLOR_WIDTH, LAYER_COLOR_WIDTH);
					if((const Layer*)m_parentCel == m_clickedLayer)		//Cel
					{
						colorRect = QRect(2, colorRect.center().y()-LAYER_EXPANDTRI_WIDTH/2, LAYER_EXPANDTRI_WIDTH, LAYER_EXPANDTRI_WIDTH);
					}

					QRect sliderRect = colorRect.adjusted(LAYER_COLOR_WIDTH + LAYER_SLIDER_BUFFER, 0, LAYER_COLOR_WIDTH + LAYER_SLIDER_BUFFER, 0);
					if ((const Layer*)m_parentCel == m_clickedLayer)
					{
						QRect colorRect2(LAYER_COLOR_LEFT, layerRect.top() + (layerRect.height() -LAYER_COLOR_WIDTH)/2, LAYER_COLOR_WIDTH, LAYER_COLOR_WIDTH);
							
						sliderRect = colorRect2.adjusted(LAYER_COLOR_WIDTH + LAYER_SLIDER_BUFFER, 0, LAYER_COLOR_WIDTH + LAYER_SLIDER_BUFFER, 0);
					}

					QRect nameRect(colorRect.right() + LAYER_COLOR_LEFT, colorRect.top(),
						rect().right() - LAYER_STATUS_WIDTH*3 - LAYER_RIGHT_BUFFER - colorRect.right() - LAYER_COLOR_LEFT,
						SHEET_HEIGHT);
					QRect visibleBox(nameRect.right()+1, sliderRect.top(), LAYER_STATUS_WIDTH, sliderRect.height());
					QRect lockedBox = visibleBox.adjusted(LAYER_STATUS_WIDTH, 0, LAYER_STATUS_WIDTH, 0);
					QRect onionbox = lockedBox.adjusted(LAYER_STATUS_WIDTH, 0, LAYER_STATUS_WIDTH, 0);

					QPoint adjustedPos = e->pos();
					adjustedPos.setY(e->pos().y() + m_scrollPosY);
					if (colorRect.contains(adjustedPos) && (const Layer*)m_parentCel == m_clickedLayer)
						m_mouseClickRegion = LayerCreatorWidget::kExpandIcon;
					else if(colorRect.contains(adjustedPos))
						m_mouseClickRegion = LayerCreatorWidget::kColorBox;
					else if(sliderRect.contains(adjustedPos))
					{
						m_mouseClickRegion = LayerCreatorWidget::kSliderRegion;
						m_interactingRect = sliderRect;
					}
					else if(nameRect.contains(adjustedPos))
						m_mouseClickRegion = LayerCreatorWidget::kOtherRegion;
					else if(visibleBox.contains(adjustedPos))
						m_mouseClickRegion = LayerCreatorWidget::kVisibleStatusRegion;
					else if(lockedBox.contains(adjustedPos))
						m_mouseClickRegion = LayerCreatorWidget::kLockStatusRegion;
					else if(onionbox.contains(adjustedPos))
						m_mouseClickRegion = LayerCreatorWidget::kOnionStatusRegion;
					else
						m_mouseClickRegion = LayerCreatorWidget::kOtherRegion;
				}
				else if(e->button() == Qt::RightButton)
				{
					if (m_clickedLayer != (const Layer*)m_parentCel)
					{
						QPoint adjustedPos = e->pos();
						adjustedPos.setY(e->pos().y() + m_scrollPosY);
						QRect colorRect(LAYER_COLOR_LEFT, layerRect.top() + (layerRect.height() -LAYER_COLOR_WIDTH)/2, LAYER_COLOR_WIDTH, LAYER_COLOR_WIDTH);
						if(colorRect.contains(adjustedPos))
						{
							m_mouseClickRegion = LayerCreatorWidget::kColorBox;
						}
						else
							m_mouseClickRegion = LayerCreatorWidget::kOtherRegion;
					}
					else
						m_mouseClickRegion = LayerCreatorWidget::kOtherRegion;
				}
			}
			else
				m_mouseClickRegion = LayerCreatorWidget::kNone;
		}
		QLOG_INFO() << "LayerCreatorWidget:" << " mousePressEvent() " << "completed";

	}

	void LayerCreatorWidget::mouseMoveEvent(QMouseEvent* e)
	{
		m_cacheEvent = QMouseEvent(QEvent::MouseMove, e->pos(), e->button(), e->buttons(),
			0);

		if(!m_manhattan && m_mouseClickRegion != LayerCreatorWidget::kNone)
		{
			if((m_initialPoint - e->pos()).manhattanLength() >= QApplication::startDragDistance())
				m_manhattan = true;
		}

		if(m_manhattan)
		{
			if((e->y() < AUTO_SCROLL_DEF || e->y() > height() - AUTO_SCROLL_DEF) && !m_scrollTimer->isActive())
				m_scrollTimer->start(250, this);
		}

		int finalY = e->y() + m_scrollPosY;
		if (m_manhattan && m_mouseClickRegion != LayerCreatorWidget::kNone && m_mouseClickRegion != LayerCreatorWidget::kSliderRegion)
		{
			if(m_stateMachine->isIdleState())
			{
				//Cel Moving
				if (m_clickedLayer == (const Layer*)m_parentCel)
				{
					m_mouseMovingType = LayerCreatorWidget::kCelMoving;
					moveCelHitTest(finalY);
					update();
				}
				else
				{
					m_mouseMovingType = LayerCreatorWidget::kLayerMoving;
					moveLayerHitTest(finalY);
					update();
				}
			}
		}
		else if (m_mouseClickRegion == LayerCreatorWidget::kSliderRegion && (m_stateMachine->isIdleState() || m_stateMachine->isPlaybackState()))
		{
			if(finalY >= m_interactingRect.bottom())
			{
				m_layerController->setLayerOpacity(m_lidCache, 0);
			}
			else if(finalY <= m_interactingRect.top())
			{
				m_layerController->setLayerOpacity(m_lidCache, 100);
			}
			else
			{
				float opacity = float(m_interactingRect.bottom()-finalY)/float(m_interactingRect.height())*100;					
				m_layerController->setLayerOpacity(m_lidCache, static_cast<int>(opacity));
			}
		}
	}

	void LayerCreatorWidget::mouseReleaseEvent(QMouseEvent* e)
	{
		
		m_cacheEvent = QMouseEvent(QEvent::MouseButtonRelease, e->pos(), e->button(), e->buttons(),
			0);

		if(!m_manhattan)
		{
			if(m_stateMachine->isIdleState())
			{
				//m_stateMachine->setLayerState(cacani::state::kClearSelectedElements | cacani::state::kClearSelectedRegion);
				
				if(m_mouseClickRegion == LayerCreatorWidget::kExpandIcon)
				{
					m_selector->setActiveLayer(m_lidCache);
					m_layerController->setLayerExpanded(m_lidCache);
				}
				else if(m_mouseClickRegion == LayerCreatorWidget::kVisibleStatusRegion)
				{
					if (!m_selector->layerIsSelected(m_lidCache))
						m_layerController->setLayerVisible(m_lidCache);
					else
						m_layerController->setSelectedVisible(m_clickedLayer);
				}
				else if(m_mouseClickRegion == LayerCreatorWidget::kLockStatusRegion)
				{
					if (!m_selector->layerIsSelected(m_lidCache))
						m_layerController->setLayerLocked(m_lidCache);
					else
						m_layerController->setSelectedLocked(m_clickedLayer);
				}
				else if(m_mouseClickRegion == LayerCreatorWidget::kOnionStatusRegion)
				{
					if (!m_selector->layerIsSelected(m_lidCache))
						m_layerController->setLayerOnion(m_lidCache);
					else
						m_layerController->setSelectedOnion(m_clickedLayer);
				}
				else if(m_mouseClickRegion != LayerCreatorWidget::kNone)
				{				
					if(e->modifiers() & Qt::ControlModifier && !(e->modifiers() & Qt::ShiftModifier))
					{
						if (m_clickedLayer != m_selector->activeLayer())
							m_selector->addSelectedLayer(m_lidCache);
					}
					else if(e->modifiers() & Qt::ShiftModifier && !(e->modifiers() & Qt::ControlModifier))
					{
						m_selector->selectLayerRange(m_lidCache);
					}
					else
					{
						if(e->button() != Qt::RightButton)
							m_selector->setActiveLayer(m_lidCache);
						else
						{
							if (!m_selector->layerIsSelected(m_lidCache))
								m_selector->setActiveLayer(m_lidCache);
						}
					}
				}
				////m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
			}
			else if(m_stateMachine->isPlaybackState())
			{
				if(m_mouseClickRegion == LayerCreatorWidget::kExpandIcon)
				{
					m_layerController->setLayerExpanded(m_lidCache);
				}
				else if(m_mouseClickRegion == LayerCreatorWidget::kVisibleStatusRegion)
				{
					if (!m_selector->layerIsSelected(m_lidCache))
						m_layerController->setLayerVisible(m_lidCache);
					else
						m_layerController->setSelectedVisible(m_clickedLayer);
				}
				else if(m_mouseClickRegion == LayerCreatorWidget::kLockStatusRegion)
				{
					if (!m_selector->layerIsSelected(m_lidCache))
						m_layerController->setLayerLocked(m_lidCache);
					else
						m_layerController->setSelectedLocked(m_clickedLayer);
				}
				else if(m_mouseClickRegion == LayerCreatorWidget::kOnionStatusRegion)
				{
					if (!m_selector->layerIsSelected(m_lidCache))
						m_layerController->setLayerOnion(m_lidCache);
					else
						m_layerController->setSelectedOnion(m_clickedLayer);
				}
			}
		}
		else
		{
			if(m_stateMachine->isIdleState())
			{
				//Cel moving
				if(m_movingDesId.indent() == 0)
				{
					m_manhattan = false;
					if(m_scrollTimer->isActive())
						m_scrollTimer->stop();
					if(e->button() == Qt::LeftButton)
						m_mouseClickRegion = LayerCreatorWidget::kNone;
					update();
					return ;
				}

				////	m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);
				if(m_mouseMovingType == LayerCreatorWidget::kCelMoving)
				{
					if (m_selector->layerIsSelected(m_lidCache))
						m_layerController->moveSelectedCels(m_movingDesId);
					else
						m_layerController->moveSelectedCel(m_lidCache, m_movingDesId);
				}
				//Handle layer to different cel movements
				else if(m_mouseMovingType == LayerCreatorWidget::kLayerMoving)
				{					
					if (m_selector->layerIsSelected(m_lidCache))
						moveSelectedLayers(m_movingDesId);
					else
						moveSelectedLayer(m_lidCache, m_movingDesId);
				}
				//Handle layer to different cel movements
				else if(m_mouseMovingType == LayerCreatorWidget::kMoveIntoCel)
				{
					if (m_selector->layerIsSelected(m_lidCache))
						moveSelectedLayersToCel(m_movingDesId);
					else
						moveSelectedLayerIntoCel(m_lidCache, m_movingDesId);
				}

				////m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
				m_mouseMovingType = LayerCreatorWidget::kNoMoving;
			}
		}


		m_manhattan = false;
		if(m_scrollTimer->isActive())
			m_scrollTimer->stop();
		if(e->button() == Qt::LeftButton)
			m_mouseClickRegion = LayerCreatorWidget::kNone;
		update();
		
	}

	void LayerCreatorWidget::mouseDoubleClickEvent(QMouseEvent* e)
	{
		QLOG_INFO() << "LayerCreatorWidget:" << " mouseDoubleClickEvent() " << "called";

		
		m_cacheEvent = QMouseEvent(QEvent::MouseButtonDblClick, e->pos(), e->button(), e->buttons(),
			e->modifiers());

		if(e->button() == Qt::LeftButton && m_stateMachine->isIdleState())
		{
			int accumHeight = 0;

			//Hit testing for layers
			const Layer* selectedLayer = 0;
			const LayerGroup* selectedParent = 0;

			int layerTop, layerBottom;
			QRect layerRect;
			for(size_t i=0; i<m_root->memberCount(); ++i)
			{
				const LayerGroup* cel = dynamic_cast<const LayerGroup*>(m_root->childAtIndex(i));

				int rectHeight = (SHEET_BORDER * 2) + (cel->isExpanded() ? (static_cast<int>(cel->memberCount()) + 1) : 1)*SHEET_HEIGHT;
				if (cel->isExpanded())
					rectHeight += static_cast<int>(cel->memberCount())*TIMELINE_NORM_TOP_BUFFER;

				//Hit test check for cel
				int finalY = e->y() + m_scrollPosY;
				if(finalY >= accumHeight && finalY <= rectHeight+accumHeight)
				{
					int levely = finalY - accumHeight;
					layerTop = SHEET_BORDER + SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER/2;
					layerBottom = rectHeight - SHEET_BORDER - SHEET_HEIGHT- TIMELINE_NORM_TOP_BUFFER/2;
					if (cel->isExpanded())
					{
						if(levely >= 0 && levely <= layerTop)
						{
							selectedLayer = (Layer*)cel;
							selectedParent = cel;
							layerRect = QRect(0, SHEET_BORDER + accumHeight, width(), SHEET_HEIGHT);
							break;
						}
						else if(levely <= rectHeight && levely >= layerBottom)
						{
							if(cel->memberCount() == 0)
							{
								selectedLayer = (Layer*)cel;
								selectedParent = cel;
								layerRect = QRect(0, SHEET_BORDER + accumHeight, width(), SHEET_HEIGHT);
								break;
							}
							else
							{
								selectedLayer = cel->childAtIndex(cel->memberCount()-1);
								selectedParent = cel;
								layerRect = QRect(0, SHEET_BORDER + SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER + accumHeight + (static_cast<int>(cel->memberCount())-1)*(SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER),
									width(), SHEET_HEIGHT);
								break;
							}
						}
						else if(cel->memberCount() >= 2)
						{
							int layerY = (levely - layerTop)/(SHEET_HEIGHT+TIMELINE_NORM_TOP_BUFFER);
							selectedLayer = cel->childAtIndex(layerY);
							layerRect = QRect(0, SHEET_BORDER+ SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER + accumHeight + layerY*(SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER),
								width(), SHEET_HEIGHT);
							selectedParent = cel;
							break;
						}
					}
					else
					{
						selectedLayer = (Layer*)cel;
						selectedParent = cel;
						layerRect = QRect(0, SHEET_BORDER + accumHeight, width(), SHEET_HEIGHT);
					}
				}

				if(selectedLayer && selectedParent)
				{
					break;
				}

				accumHeight += rectHeight + TIMELINE_INTER_CEL;
			}
			
			if(selectedLayer && selectedParent)
			{
				m_clickedLayer = selectedLayer;
				m_parentCel = selectedParent;
				m_lidCache = m_selector->findLayerId(selectedLayer, selectedParent);

				QRect colorRect(LAYER_COLOR_LEFT, layerRect.top() + (layerRect.height() -LAYER_COLOR_WIDTH)/2, LAYER_COLOR_WIDTH, LAYER_COLOR_WIDTH);
				if ((Layer*)m_parentCel == m_clickedLayer)		//Cel
				{
					colorRect = QRect(2, colorRect.center().y()-LAYER_EXPANDTRI_WIDTH/2, LAYER_EXPANDTRI_WIDTH, LAYER_EXPANDTRI_WIDTH);
				}

				QRect sliderRect = colorRect.adjusted(LAYER_COLOR_WIDTH + LAYER_SLIDER_BUFFER, 0, LAYER_COLOR_WIDTH + LAYER_SLIDER_BUFFER, 0);
				QRect nameRect(colorRect.right() + LAYER_COLOR_LEFT, colorRect.top(),
					rect().right() - LAYER_STATUS_WIDTH*3 - LAYER_RIGHT_BUFFER - colorRect.right() - LAYER_COLOR_LEFT,
					SHEET_HEIGHT);
				QRect visibleBox(nameRect.right()+1, nameRect.center().y()-LAYER_STATUS_WIDTH/2-1, LAYER_STATUS_WIDTH, LAYER_STATUS_WIDTH);
				QRect lockedBox = visibleBox.adjusted(LAYER_STATUS_WIDTH, 0, LAYER_STATUS_WIDTH, 0);
				QRect onionbox = lockedBox.adjusted(LAYER_STATUS_WIDTH, 0, LAYER_STATUS_WIDTH, 0);

				QPoint adjustedPos = e->pos();
				adjustedPos.setY(e->pos().y() + m_scrollPosY);
				if(nameRect.contains(adjustedPos))
				{
					m_stateMachine->setLayerState();
					//QByteArray nameByte = m_clickedLayer->getName().rawData();
					//QTextCodec *codec=QTextCodec::codecForName("UTF-8");
					//QString name = codec->toUnicode(nameByte);
					QString name = "Layer";//
					QString  newName=QInputDialog::getText(this, tr("Layer's Name"), tr("Enter New name"), QLineEdit::Normal, name);
				
					if(!newName.isEmpty())
					{
						m_layerController->setLayerName(m_lidCache, utf8string(newName.toUtf8().data()));
					}

					m_stateMachine->setIdleState();
				}
				else if(visibleBox.contains(adjustedPos))
				{
					//	m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);
					if(!m_selector->layerIsSelected(m_lidCache))
						m_layerController->setLayerVisible(m_lidCache);
					else
						m_layerController->setSelectedVisible(m_clickedLayer);
					m_stateMachine->setIdleState();
				}
				else if(lockedBox.contains(adjustedPos))
				{
					//m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);
					if(!m_selector->layerIsSelected(m_lidCache))
						m_layerController->setLayerLocked(m_lidCache);
					else
						m_layerController->setSelectedLocked(m_clickedLayer);
					m_stateMachine->setIdleState();
				}
				else if(onionbox.contains(adjustedPos))
				{
					//m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);
					if(!m_selector->layerIsSelected(m_lidCache))
						m_layerController->setLayerOnion(m_lidCache);
					else
						m_layerController->setSelectedOnion(m_clickedLayer);
					m_stateMachine->setIdleState();
				}
			}
		}
		
		QLOG_INFO() << "LayerCreatorWidget:" << " mouseDoubleClickEvent() " << "completed";

	}
	
	void LayerCreatorWidget::contextMenuEvent(QContextMenuEvent* e)
	{
		if(m_mouseClickRegion == LayerCreatorWidget::kColorBox)
		{
			m_orangeTag->setChecked(false);
			m_yellowTag->setChecked(false);
			m_paleGreenTag->setChecked(false);
			m_greenTag->setChecked(false);
			m_skyBlueTag->setChecked(false);
			m_ultraMarineTag->setChecked(false);
			m_purpleTag->setChecked(false);
			m_redTag->setChecked(false);
			m_noColorTag->setChecked(false);
			m_histActn = 0;

			////Argb layerColor = m_clickedLayer->getColor();

			//
			/*if(layerColor == LAYER_ORANGE_COLOR)
			{
				m_orangeTag->setChecked(true);
				m_histActn = m_orangeTag;
			}
			else if(layerColor == LAYER_YELLOW_COLOR)
			{
				m_yellowTag->setChecked(true);
				m_histActn = m_yellowTag;
			}
			else if(layerColor == LAYER_PALE_GREEN_COLOR)
			{
				m_paleGreenTag->setChecked(true);
				m_histActn = m_paleGreenTag;
			}
			else if(layerColor == LAYER_GREEN_COLOR)
			{
				m_greenTag->setChecked(true);
				m_histActn = m_greenTag;
			}
			else if(layerColor == LAYER_SKY_BLUE_COLOR)
			{
				m_skyBlueTag->setChecked(true);
				m_histActn = m_skyBlueTag;
			}
			else if(layerColor == LAYER_ULTRA_MAR_COLOR)
			{
				m_ultraMarineTag->setChecked(true);
				m_histActn = m_ultraMarineTag;
			}
			else if(layerColor == LAYER_PURPLE_COLOR)
			{
				m_purpleTag->setChecked(true);
				m_histActn = m_purpleTag;
			}
			else if(layerColor == LAYER_RED_COLOR)
			{
				m_redTag->setChecked(true);
				m_histActn = m_redTag;
			}
			else*/
			////{
				m_noColorTag->setChecked(true);
				m_histActn = m_noColorTag;
			////}

			m_colorContext->move(e->globalPos());
			m_colorContext->exec();

		}
		else
		{
#ifndef DISABLE_AUDIO
			if(m_audioController->audioCount() == 0)
			{
				m_removeAudioLayer->setDisabled(true);
			}
			else
				m_removeAudioLayer->setEnabled(true);
#endif

			m_contextMenu->move(e->globalPos());
			m_contextMenu->exec();
		}



		m_mouseClickRegion = LayerCreatorWidget::kNone;
	}

	void LayerCreatorWidget::timerEvent(QTimerEvent* e)
	{
		if(e->timerId() == m_scrollTimer->timerId()&& m_cacheEvent.buttons() != Qt::NoButton
			&& m_cacheEvent.type() == QEvent::MouseMove)
		{
			if(m_cacheEvent.y() < AUTO_SCROLL_DEF)
			{
				Q_EMIT sliderBarTop(SHEET_HEIGHT + SHEET_BORDER);
			}
			else if (m_cacheEvent.y() > height() - AUTO_SCROLL_DEF)
			{
				Q_EMIT sliderBarBottom(SHEET_HEIGHT + SHEET_BORDER);
			}
		}
	}
			
	void LayerCreatorWidget::findLayer(int y, const Layer*& selectedLayer, const LayerGroup*& selectedParent)
	{
		QLOG_INFO() << "LayerCreatorWidget:" << " findLayer() " << "called";

		int accumHeight = 0;
		for(size_t i=0; i<m_root->memberCount(); ++i)
		{
			const LayerGroup* cel = dynamic_cast<const LayerGroup*>(m_root->childAtIndex(i));

			int rectHeight = (SHEET_BORDER * 2) + (cel->isExpanded() ? (static_cast<int>(cel->memberCount()) + 1) : 1)*SHEET_HEIGHT;
			if (cel->isExpanded())
				rectHeight += static_cast<int>(cel->memberCount())*TIMELINE_NORM_TOP_BUFFER;

			//Hit test check for cel
			if(y >= accumHeight && y <= rectHeight+accumHeight)
			{
				int levely = y-accumHeight;
				int top = SHEET_BORDER + SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER/2;
				int bottom = rectHeight - SHEET_BORDER - SHEET_HEIGHT- TIMELINE_NORM_TOP_BUFFER/2;
				if (cel->isExpanded())
				{
					if(levely >= 0 && levely <= top)
					{
						selectedLayer = (Layer*)cel;
						selectedParent = cel;
						break;
					}
					else if(levely <= rectHeight && levely >= bottom)
					{
						if(cel->memberCount() == 0)
						{
							selectedLayer = (Layer*)cel;
							selectedParent = cel;
							break;
						}
						else
						{
							selectedLayer = cel->childAtIndex(cel->memberCount() - 1);
							selectedParent = cel;
							break;
						}
					}
					else if(cel->memberCount() >= 2)
					{
						int layerY = (levely - top)/(SHEET_HEIGHT+TIMELINE_NORM_TOP_BUFFER);
						selectedLayer = cel->childAtIndex(layerY);
						selectedParent = cel;
						break;
					}
				}
				else
				{
					selectedLayer = (Layer*)cel;
					selectedParent = cel;
				}
			}

			if(selectedLayer && selectedParent)
			{
				break;
			}

			accumHeight += rectHeight + TIMELINE_INTER_CEL;
		}
		QLOG_INFO() << "LayerCreatorWidget:" << " findLayer() " << "completed";

	}

	void LayerCreatorWidget::addNewCel()
	{   //
		/*
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);

			m_layerController->addCel();

			m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
		}
		*/
	}

	void LayerCreatorWidget::addDrawingLayer()
	{   //
		/*
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);

			m_layerController->addDrawingLayer();

			m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
		}
		*/
	}

	void LayerCreatorWidget::duplicateActiveLayer()
	{   //
		/*
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);

			m_layerController->duplicateLayer();

			m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
		}
		*/
	}

	void LayerCreatorWidget::duplicateActiveCel()
	{   //
		/*
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);

			m_layerController->duplicateCel();

			m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
		}
		*/
	}

	void LayerCreatorWidget::removeSelectedCels()
	{   //
		/*
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);

			m_layerController->removeCels();

			m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
		}
		*/
	}

	void LayerCreatorWidget::removeSelectedLayers()
	{   //
		/*
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);

			m_layerController->removeLayers();

			m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
		}
		*/
	}

	void LayerCreatorWidget::deleteFrame() {
		//Stop playing animation
		m_selector->getSheetCanvas()->stopAnimation();
	
		//Show current frame within scroll window
		m_layerController->updateScroll();

		QMessageBox msgBox(QMessageBox::Question, tr("Delete Frames"), tr("Delete Current Frame?"),
			QMessageBox::Ok | QMessageBox::Cancel, this);

		QPalette mainPalette = this->palette();
		mainPalette.setColor(QPalette::WindowText, QColor(0, 0, 0));
		msgBox.setPalette(mainPalette);
		int ret = msgBox.exec();

		if (ret == QMessageBox::Ok)	{

			int maxFrames = m_selector->activeLayer()->GetNumFrame();
			int activeFrame = m_selector->activeFrame();

			//Check if there is only 1 frame
			if (maxFrames == 1) {
				m_layerController->addFrames(m_selector->activeFrame(), 1);
				m_selector->getSheetCanvas()->update();
				m_layerController->getFCContainer()->getFrameWidget()->update();
				activeFrame++;
			}
			else {
				//Check if it is last index that is being deleted
				if (activeFrame == (maxFrames - 1)) {
					m_selector->setActiveFrame(activeFrame - 1);
				}
			}
			//qDebug() << m_selector->activeLayer()->GetNumFrame();
			m_layerController->removeFrameSeq(m_selector->activeLayerId(), m_selector->activeLayerId().index(), activeFrame, 1);

			//update view
			m_layerController->updateCananiUI();

			//Update Sliders
			m_layerController->jointEditorWidget()->initializeSliderGroup();
		}
	}

	void LayerCreatorWidget::deleteFrames() {
		//Stop playing animation
		m_selector->getSheetCanvas()->stopAnimation();

		QMessageBox msgBox(QMessageBox::Question, tr("Delete Frames"), tr("Delete All Frames?"),
			QMessageBox::Ok | QMessageBox::Cancel, this);

		QPalette mainPalette = this->palette();
		mainPalette.setColor(QPalette::WindowText, QColor(0, 0, 0));
		msgBox.setPalette(mainPalette);
		int ret = msgBox.exec();

		if (ret == QMessageBox::Ok)	{
			int maxFrames = m_selector->activeLayer()->GetNumFrame();

			//Add 1 frame and remove the rest
			m_layerController->addFrames(0, 1);

			//Make added frame show as activeframe
			m_selector->setActiveFrame(0);

			//Remove remaining frames
			m_layerController->removeFrameSeq(m_selector->activeLayerId(), m_selector->activeLayerId().index(), 1, maxFrames);

			//update view
			m_layerController->updateCananiUI();

			//Update Sliders
			m_layerController->jointEditorWidget()->initializeSliderGroup();

			//Show current frame within scroll window
			m_layerController->updateScroll();
		}
	}

	void LayerCreatorWidget::addOne() {
		//Stop playing animation
		m_selector->getSheetCanvas()->stopAnimation();

		m_layerController->addFrames(m_selector->activeFrame(), 1);
		m_selector->getSheetCanvas()->update();
		
		//update view
		m_layerController->updateCananiUI();

		//Update Sliders
		m_layerController->jointEditorWidget()->initializeSliderGroup();

		//Show current frame within scroll window
		m_layerController->updateScroll();
	}

	void LayerCreatorWidget::addFive() {
		//Stop playing animation
		m_selector->getSheetCanvas()->stopAnimation();

		m_layerController->addFrames(m_selector->activeFrame(), 5);
		m_selector->getSheetCanvas()->update();
		
		//update view
		m_layerController->updateCananiUI();

		//Update Sliders
		m_layerController->jointEditorWidget()->initializeSliderGroup();

		//Show current frame within scroll window
		m_layerController->updateScroll();
	}

	void LayerCreatorWidget::addTen() {
		//Stop playing animation
		m_selector->getSheetCanvas()->stopAnimation();

		m_layerController->addFrames(m_selector->activeFrame(), 10);
		m_selector->getSheetCanvas()->update();

		//update view
		m_layerController->updateCananiUI();

		//Update Sliders
		m_layerController->jointEditorWidget()->initializeSliderGroup();

		//Show current frame within scroll window
		m_layerController->updateScroll();
	}

	void LayerCreatorWidget::addFrames() {
		InsertFrameDialog* insertFrameDialog = new InsertFrameDialog(m_selector->getSheetCanvas(), m_layerController, m_selector);
		insertFrameDialog->exec();
	}


	void LayerCreatorWidget::selectSimilarColors()
	{
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState();

			m_layerController->selectSimilarColorType(m_clickedLayer, m_lidCache);

			m_stateMachine->setIdleState();
		}		
	}

	void LayerCreatorWidget::setNoTag()
	{
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState();

			if(m_selector->layerIsSelected(m_lidCache))
				m_layerController->changeSelectedLayerColor(0, 0, 0);
			else
				m_layerController->changeLayerColor(m_lidCache,0, 0,0, 0);

			if(m_histActn != m_noColorTag)
				m_histActn->setChecked(false);

			m_stateMachine->setIdleState();
		}		
	}

	void LayerCreatorWidget::setOrangeTag()
	{
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState();
			
			////Argb color = LAYER_ORANGE_COLOR;

			////if(m_selector->layerIsSelected(m_lidCache))
			////m_layerController->changeSelectedLayerColor(color.r(),color.g(), color.b());
			////else
			////m_layerController->changeLayerColor(m_lidCache,color.r(),color.g(), color.b(), color.a());

			if(m_histActn != m_orangeTag)
				m_histActn->setChecked(false);

			m_stateMachine->setIdleState();
		}		
	}

	void LayerCreatorWidget::setYellowTag()
	{
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState();
			
			////Argb color = LAYER_YELLOW_COLOR;

			////if(m_selector->layerIsSelected(m_lidCache))
			//	//m_layerController->changeSelectedLayerColor(color.r(),color.g(), color.b());
			////else
			//	//m_layerController->changeLayerColor(m_lidCache,color.r(),color.g(), color.b(), color.a());

			if(m_histActn != m_yellowTag)
				m_histActn->setChecked(false);

			m_stateMachine->setIdleState();
		}		
	}

	void LayerCreatorWidget::setPaleGreenTag()
	{
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState();
			
			////Argb color = LAYER_PALE_GREEN_COLOR;

			////if(m_selector->layerIsSelected(m_lidCache))
			//	//m_layerController->changeSelectedLayerColor(color.r(),color.g(), color.b());
			////else
			////m_layerController->changeLayerColor(m_lidCache,color.r(),color.g(), color.b(), color.a());

			if(m_histActn != m_paleGreenTag)
				m_histActn->setChecked(false);

			m_stateMachine->setIdleState();
		}		
	}

	void LayerCreatorWidget::setGreenTag()
	{
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState();

			////Argb color = LAYER_GREEN_COLOR;

			////if(m_selector->layerIsSelected(m_lidCache))
			////m_layerController->changeSelectedLayerColor(color.r(),color.g(), color.b());
			////else
			////m_layerController->changeLayerColor(m_lidCache,color.r(),color.g(), color.b(), color.a());

			if(m_histActn != m_greenTag)
				m_histActn->setChecked(false);

			m_stateMachine->setIdleState();
		}		
	}

	void LayerCreatorWidget::setSkyBlueTag()
	{
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState();

			////Argb color = LAYER_SKY_BLUE_COLOR;

			////if(m_selector->layerIsSelected(m_lidCache))
			//	//m_layerController->changeSelectedLayerColor(color.r(),color.g(), color.b());
			////else
			//	//m_layerController->changeLayerColor(m_lidCache,color.r(),color.g(), color.b(), color.a());

			if(m_histActn != m_skyBlueTag)
				m_histActn->setChecked(false);

			m_stateMachine->setIdleState();
		}		
	}

	void LayerCreatorWidget::setUltraMarineTag()
	{
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState();

			////Argb color = LAYER_ULTRA_MAR_COLOR;

			////if(m_selector->layerIsSelected(m_lidCache))
			//	//m_layerController->changeSelectedLayerColor(color.r(),color.g(), color.b());
			////else
			//	//m_layerController->changeLayerColor(m_lidCache,color.r(),color.g(), color.b(), color.a());

			if(m_histActn != m_ultraMarineTag)
				m_histActn->setChecked(false);

			m_stateMachine->setIdleState();
		}		
	}

	void LayerCreatorWidget::setPurpleTag()
	{
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState();

			////Argb color = LAYER_PURPLE_COLOR;

			////if(m_selector->layerIsSelected(m_lidCache))
			//	//m_layerController->changeSelectedLayerColor(color.r(),color.g(), color.b());
			////else
			//	//m_layerController->changeLayerColor(m_lidCache,color.r(),color.g(), color.b(), color.a());

			if(m_histActn != m_purpleTag)
				m_histActn->setChecked(false);

			m_stateMachine->setIdleState();
		}		
	}

	void LayerCreatorWidget::setRedTag()
	{
		if(m_stateMachine->isIdleState())
		{
			m_stateMachine->setLayerState();

			////Argb color = LAYER_RED_COLOR;

			////if(m_selector->layerIsSelected(m_lidCache))
			//	//m_layerController->changeSelectedLayerColor(color.r(),color.g(), color.b());
			////else
			//	//m_layerController->changeLayerColor(m_lidCache,color.r(),color.g(), color.b(), color.a());

			if(m_histActn != m_redTag)
				m_histActn->setChecked(false);

			m_stateMachine->setIdleState();
		}		
	}

	void LayerCreatorWidget::updateMenus()
	{
		if (!m_selector->activeCel())
		{
			m_addDrawingLayerActn->setDisabled(true);;
			m_removeSelectedCelActn->setDisabled(true);
			m_removeSelectedLayersActn->setDisabled(true);
			//m_selectOtherLayerActn->setDisabled(true);
			m_duplicateLayerActn->setDisabled(true);
		}
		else
		{
			m_addDrawingLayerActn->setDisabled(false);
			m_removeSelectedCelActn->setDisabled(false);
			m_removeSelectedLayersActn->setDisabled(false);
			//m_selectOtherLayerActn->setDisabled(false);
			if(m_selector->activeLayerId().indent() == 2)
				m_duplicateLayerActn->setDisabled(false);
			else
				m_duplicateLayerActn->setDisabled(true);
		}
	}

	void LayerCreatorWidget::moveCelHitTest(int y)
	{
		QLOG_INFO() << "LayerCreatorWidget:" << " moveCelHitTest() " << "called";

		int accumHeight = 0;
		for(int i=0; i<static_cast<int>(m_root->memberCount()); ++i)
		{
			const LayerGroup* cel = dynamic_cast<const LayerGroup*>(m_root->childAtIndex(i));

			int rectHeight = (SHEET_BORDER * 2) + (cel->isExpanded() ? (static_cast<int>(cel->memberCount()) + 1) : 1)*SHEET_HEIGHT;
			if (cel->isExpanded())
				rectHeight += static_cast<int>(cel->memberCount())*TIMELINE_NORM_TOP_BUFFER;

			if(y < accumHeight)
			{
				m_movingDesId = LayerId(0);
				m_movingDesId.addIndex(i);
				m_movingDesId.setIndent(1);
				m_dragLayerLine = accumHeight;
				break;
			}

			if (y > accumHeight + rectHeight &&  i == m_root->memberCount() - 1)
			{
				m_movingDesId = LayerId(0);
				m_movingDesId.addIndex(i+1);
				m_movingDesId.setIndent(1);
				m_dragLayerLine = accumHeight + rectHeight;
				break;
			}

			accumHeight += rectHeight + TIMELINE_INTER_CEL;
		}
		QLOG_INFO() << "LayerCreatorWidget:" << " moveCelHitTest() " << "completed";

	}

	void LayerCreatorWidget::moveLayerHitTest(int y)
	{
		QLOG_INFO() << "LayerCreatorWidget:" << " moveLayerHitTest() " << "called";

		int accumHeight = 0;
		m_movingDesId = LayerId(0);
		m_movingDesId.addIndex(0);
		m_movingDesId.addIndex(0);
		for (int i = 0; i<static_cast<int>(m_root->memberCount()); ++i)
		{
			const LayerGroup* cel = dynamic_cast<const LayerGroup*>(m_root->childAtIndex(i));

			int rectHeight = (SHEET_BORDER * 2) + (cel->isExpanded() ? (static_cast<int>(cel->memberCount()) + 1) : 1)*SHEET_HEIGHT;
			if (cel->isExpanded())
				rectHeight += static_cast<int>(cel->memberCount())*TIMELINE_NORM_TOP_BUFFER;

			//Hit test check for cel
			if(y >= accumHeight && y <= rectHeight+accumHeight)
			{
				int levely = y-accumHeight;
				int top = SHEET_BORDER + SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER/2;
				int bottom = rectHeight - SHEET_BORDER - SHEET_HEIGHT- TIMELINE_NORM_TOP_BUFFER/2;
				if (cel->isExpanded())
				{
					//Cel
					if(levely >= 0 && levely <= top)
					{
						m_mouseMovingType = LayerCreatorWidget::kMoveIntoCel;
						m_dragBox = QRect(0, accumHeight, width(), top);
						m_movingDesId.setIndent(1);
						m_movingDesId.setIndex(i);
						return ;
					}
					//Layer or cel
					else if(levely <= rectHeight && levely >= bottom)
					{
						//Cel
						if(cel->memberCount() == 0)
						{
							m_mouseMovingType = LayerCreatorWidget::kMoveIntoCel;
							m_dragBox = QRect(0, bottom+accumHeight, width(), rectHeight);
							m_movingDesId.setIndent(1);
							m_movingDesId.setIndex(i);
							return ;
						}
						//Layer
						else
						{
							m_mouseMovingType = LayerCreatorWidget::kLayerMoving;;
							m_movingDesId.setIndent(1);
							m_movingDesId.setIndex(i);
							m_movingDesId.setIndent(2);
							//Move to top
							if(levely < bottom + (rectHeight-bottom)/2)
							{
								m_dragLayerLine = bottom + accumHeight;
								m_movingDesId.setIndex(static_cast<int>(cel->memberCount()) -1);
							}
							//Move to bottom
							else
							{
								m_dragLayerLine = accumHeight+ rectHeight;
								m_movingDesId.setIndex(static_cast<int>(cel->memberCount()));
							}
							return ;
						}
					}
					//Layer
					else if(cel->memberCount() >= 2)
					{
						m_mouseMovingType = LayerCreatorWidget::kLayerMoving;
						int rememant = (levely-top)%(SHEET_HEIGHT+TIMELINE_NORM_TOP_BUFFER);
						int level = (levely-top)/(SHEET_HEIGHT+TIMELINE_NORM_TOP_BUFFER);
						m_movingDesId.setIndent(1);
						m_movingDesId.setIndex(i);
						m_movingDesId.setIndent(2);
						if(rememant < SHEET_HEIGHT/2)
						{
							m_dragLayerLine = level*(SHEET_HEIGHT+TIMELINE_NORM_TOP_BUFFER)+top+accumHeight;
							m_movingDesId.setIndex(level);
						}
						else
						{
							m_dragLayerLine = (level+1)*(SHEET_HEIGHT+TIMELINE_NORM_TOP_BUFFER)+top+accumHeight;
							m_movingDesId.setIndex(level+1);
						}
						return ;
					}
				}
				else
				{
					m_mouseMovingType = LayerCreatorWidget::kMoveIntoCel;
					m_dragBox = QRect(0, accumHeight, width(), top);
					m_movingDesId.setIndent(1);
					m_movingDesId.setIndex(i);
					return ;
				}
			}
			else if(y > rectHeight+accumHeight && i == static_cast<int>(m_root->memberCount())-1)
			{
				if (cel->isExpanded())
				{
					m_mouseMovingType = LayerCreatorWidget::kLayerMoving;
					m_movingDesId.setIndent(1);
					m_movingDesId.setIndex(i);
					m_movingDesId.setIndent(2);
					m_movingDesId.setIndex(static_cast<int>(cel->memberCount()));
					m_dragLayerLine = rectHeight+accumHeight;
				}
				else
				{
					m_mouseMovingType = LayerCreatorWidget::kMoveIntoCel;
					m_dragBox = QRect(0, accumHeight, width(), rectHeight);
					m_movingDesId.setIndent(1);
					m_movingDesId.setIndex(i);
				}
			}

			accumHeight += rectHeight + TIMELINE_INTER_CEL;
		}
		QLOG_INFO() << "LayerCreatorWidget:" << " moveLayerHitTest() " << "completed";

	}

	/*void LayerCreatorWidget::createAudioWaveImage(const AudioLayer* al, int index)
	{
		if(!al->isEmpty())
		{
			int top = SHEET_HEIGHT/2;
			int waveHeight = SHEET_HEIGHT;

			if(al->channelCount() == 1)
			{
				top = SHEET_HEIGHT;
				waveHeight = SHEET_HEIGHT*2;
			}

			float audioLength = (float)al->totalTime() / 1000.0f;

			int totalFramesUsed = static_cast<int>(audioLength*24.0f)*(SHEET_WIDTH+SHEET_BORDER);

			int minHeight = 0, maxHeight = 0;
			int range = std::pow(2.0f, (float)(al->bitsPerSample()));
			minHeight = -range/2;
			maxHeight = range/2;

			float horizontalScaling = 1.0f/(float)al->samplePerSec()*24.0f*(SHEET_WIDTH+SHEET_BORDER);
			float verticalScaling = 1.0f/float(maxHeight-minHeight)*waveHeight;

			QPixmap* pixmap = new QPixmap(totalFramesUsed, SHEET_HEIGHT*2);
			pixmap->fill(QColor(0, 0, 0, 0));
			QPainter p(pixmap);

			p.setPen(QColor(204,204,204));

			al->forEach(1, )
			if(!al->channelOneData().empty())
			{
				for(unsigned int i=0; i<static_cast<unsigned int>(al->channelOneData().size()-1); ++i)
				{
					p.drawLine(QPointF(float(i)*horizontalScaling,
						(float)al->channelOneData()[i]*verticalScaling + top),
						QPointF(float(i+1)*horizontalScaling,
						(float)al->channelOneData()[i+1]*verticalScaling + top));
				}

				top += SHEET_HEIGHT;

			}

			if(!al->channelTwoData().empty())
			{
				for(unsigned int i=0; i<static_cast<unsigned int>(al->channelTwoData().size()-1); ++i)
				{
					p.drawLine(QPointF(float(i)*horizontalScaling,
						(float)al->channelTwoData()[i]*verticalScaling + top),
						QPointF(float(i+1)*horizontalScaling,
						(float)al->channelTwoData()[i+1]*verticalScaling  + top));
				}
			}

			p.end();

			QByteArray bytes;
			QBuffer buffer(&bytes);

			buffer.open(QIODevice::WriteOnly);
			pixmap->save(&buffer, "png");

			int byteSize = bytes.size();

			

			m_layerController->setAudioImageData(index,
				bytes.data(), byteSize);

			delete pixmap;
		}
	}*/

	void LayerCreatorWidget::moveSelectedLayer(const LayerId& lid, const LayerId& destId)
	{
		//Moving layer need to maintain the same sequence at the same frames it originally has.
		//If moving layer has less offset than the destination cel, append delta offset to the cel
		//If moving layer has more offset than the destination cel, append delta offset to the layer

		//If moving layer sheetCount + offset is less than the destination cel, append extra frames to the back
		//If moving layer sheetCount + offset is more than the destination cel, append extra frames to cel back

		//If sequence is different, check if the user wants to change the seq, if yes, go ahead.  If not, abort.
		if (!m_layerController->isRepeatSeqSync(lid, destId))
		{
			QMessageBox msgBox(QMessageBox::Information, tr("Different Sheet Sequence"),
				tr("Moving layer and destination has different sequence of extends!"), QMessageBox::Ok, this);

			QPalette currentPalette = this->palette();
			currentPalette.setColor(QPalette::WindowText, QColor(225, 225, 225));
			msgBox.setPalette(currentPalette);

			msgBox.exec();

			return;
		}
		if (!m_layerController->isLayerSeqSync(lid, destId))
		{
			QMessageBox msgBox(QMessageBox::Information, tr("Change Sheet Sequence"), tr("Moving layer has different frame sequence compared to destination.\n") +
				tr("Do you want to change the moving layer's sequence?"), QMessageBox::Ok | QMessageBox::Cancel, this);

			QPalette currentPalette = this->palette();
			currentPalette.setColor(QPalette::WindowText, QColor(225, 225, 225));
			msgBox.setPalette(currentPalette);

			int ret = msgBox.exec();
			if(ret == QMessageBox::Ok)
				m_layerController->moveSelectedLayer(lid, destId);
		}
		else
			m_layerController->moveSelectedLayer(lid, destId);
	}

	void LayerCreatorWidget::moveSelectedLayers(const LayerId& destId)
	{
		bool repeatSequenceMatched = true;
		for (unsigned i = 0; i<m_selector->selectedLayersId().size(); ++i)
		{
			if (!m_layerController->isRepeatSeqSync(m_selector->selectedLayersId()[i], destId))
			{
				repeatSequenceMatched = false;
				break;
			}
		}

		if (!repeatSequenceMatched)
		{
			QMessageBox msgBox(QMessageBox::Information, tr("Different Sheet Sequence"), 
				tr("Moving layer and destination has different sequence of extends!"), QMessageBox::Ok, this);

			QPalette currentPalette = this->palette();
			currentPalette.setColor(QPalette::WindowText, QColor(225, 225, 225));
			msgBox.setPalette(currentPalette);
	
			msgBox.exec();

			return;
		}

		bool synced = true;
		for(unsigned i = 0; i<m_selector->selectedLayersId().size(); ++i)
		{
			if (!m_layerController->isLayerSeqSync(m_selector->selectedLayersId()[i], destId))
			{
				synced = false;
				break;
			}
		}

		if(!synced)
		{
			QMessageBox msgBox(QMessageBox::Information, tr("Change Sheet Sequence"), tr("Moving layer(s) has different frame sequence compared to destination.\n") +
				tr("Do you want to change the moving layers' sequence?"), QMessageBox::Ok | QMessageBox::Cancel, this);

			QPalette currentPalette = this->palette();
			currentPalette.setColor(QPalette::WindowText, QColor(225, 225, 225));
			msgBox.setPalette(currentPalette);

			int ret = msgBox.exec();
			if(ret == QMessageBox::Ok)
				m_layerController->moveSelectedLayers(destId);
		}
		else
			m_layerController->moveSelectedLayers(destId);
	}

	void LayerCreatorWidget::moveSelectedLayerIntoCel(const LayerId& lid, const LayerId& destId)
	{
		if (!m_layerController->isRepeatSeqSync(lid, destId))
		{
			QMessageBox msgBox(QMessageBox::Information, tr("Different Sheet Sequence"),
				tr("Moving layer and destination has different sequence of extends!"), QMessageBox::Ok, this);

			QPalette currentPalette = this->palette();
			currentPalette.setColor(QPalette::WindowText, QColor(225, 225, 225));
			msgBox.setPalette(currentPalette);

			msgBox.exec();

			return;
		}

		if (!m_layerController->isLayerSeqSync(lid, destId))
		{
			QMessageBox msgBox(QMessageBox::Information, tr("Change Sheet Sequence"), tr("Moving layer has different frame sequence compared to destination.\n") +
				tr("Do you want to change the moving layer's sequence?"), QMessageBox::Ok | QMessageBox::Cancel, this);

			QPalette currentPalette = this->palette();
			currentPalette.setColor(QPalette::WindowText, QColor(225, 225, 225));
			msgBox.setPalette(currentPalette);

			int ret = msgBox.exec();
			if(ret == QMessageBox::Ok)
				m_layerController->moveSelectedLayerIntoCel(lid, destId);
		}
		else
			m_layerController->moveSelectedLayerIntoCel(lid, destId);
	}
	
	void LayerCreatorWidget::moveSelectedLayersToCel(const LayerId& destId)
	{
		bool repeatSequenceMatched = true;
		for (unsigned i = 0; i<m_selector->selectedLayersId().size(); ++i)
		{
			if (!m_layerController->isRepeatSeqSync(m_selector->selectedLayersId()[i], destId))
			{
				repeatSequenceMatched = false;
				break;
			}
		}

		if (!repeatSequenceMatched)
		{
			QMessageBox msgBox(QMessageBox::Information, tr("Different Sheet Sequence"),
				tr("Moving layer and destination has different sequence of extends!"), QMessageBox::Ok, this);

			QPalette currentPalette = this->palette();
			currentPalette.setColor(QPalette::WindowText, QColor(225, 225, 225));
			msgBox.setPalette(currentPalette);

			msgBox.exec();

			return;
		}

		bool synced = true;
		for (unsigned i = 0; i<m_selector->selectedLayersId().size(); ++i)
		{
			if (!m_layerController->isLayerSeqSync(m_selector->selectedLayersId()[i], destId))
			{
				synced = false;
				break;
			}
		}

		if(!synced)
		{
			QMessageBox msgBox(QMessageBox::Information, tr("Change Sheet Sequence"), tr("Moving layer(s) has different frame sequence compared to destination.\n") +
				tr("Do you want to change the moving layers' sequence?"), QMessageBox::Ok | QMessageBox::Cancel, this);

			QPalette currentPalette = this->palette();
			currentPalette.setColor(QPalette::WindowText, QColor(225, 225, 225));
			msgBox.setPalette(currentPalette);

			int ret = msgBox.exec();
			if(ret == QMessageBox::Ok)
				m_layerController->moveSelectedLayersIntoCel(destId);
		}
		else
			m_layerController->moveSelectedLayersIntoCel(destId);
	}
}}