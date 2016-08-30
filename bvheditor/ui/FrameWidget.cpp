#include <vector>

#include <qwidget.h>  
#include <qevent.h>   
#include <qapplication.h>   
#include <qbasictimer.h>   
#include <qaction.h>    
#include <qmenu.h>      
#include <qstandardpaths.h>   
#include <qpainter.h>   
#include <qpoint.h>    
#include <qdesktopwidget.h>		
#include <qrect.h>		
#include <qsize.h>		
#include <qmessagebox.h>		



#include "FrameWidget.h"
#include "FrameCreatorDefs.h"
//#include "CreateFrameDialog.h"
//#include "io/IOQtStream.h"
//#include "FreeImage.h"
//#include "DualOptionsDialog.h"
//#include "DockUI/DockColors.h"

#include "../controllers/AbstractLayerSelector.h" //#include "DataController/AbstractLayerSelector.h"
#include "../controllers/AbstractViewSettings.h"  //#include "DataController/AbstractViewSettings.h"
#include "../data/AbstractSheet.h"   
#include "../data/AbstractLayerGroup.h"
#include "../controllers/AbstractLayerSelector.h"
#include "../QsLog2/qslog.h"

//using namespace cacani::io;
using namespace std;
using namespace cacani::data;


namespace cacani {
	namespace ui {

		FrameWidget::FrameWidget(ViewSettings* viewSettings, LayerController* layerController, LayerSelector* selector, CACStateMachine* sm, QWidget* parent)
			: QWidget(parent), m_selector(selector), m_stateMachine(sm), m_layerController(layerController), m_viewSettings(viewSettings)
			, m_cacheEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), Qt::NoButton, 0, 0), m_parentCel(nullptr), m_clickedLayer(nullptr), m_panToolIn(false)
		{

			connect(m_layerController, SIGNAL(FPS_updated()), this, SLOT(refreshFPS()));

			QPalette pal(palette());
			pal.setColor(QPalette::WindowText, QColor(225, 225, 225));
			pal.setColor(QPalette::Background, TIMELINE_BKGN);
			setAutoFillBackground(true);
			setPalette(pal);

			QFont currentFont = QApplication::font();
			currentFont.setPointSize(8);
			setFont(currentFont);
			m_fontMetrics = new QFontMetrics(currentFont);

			m_manhattan = false;
			m_showStrokeNums = true;
			m_hideNumber = false;
			m_timerUp = false;
			m_rubberBandSelection = false;

			m_numberOfFrame = 0;

			m_mouseMoveType = FrameWidget::kNone;
			m_displayType = FrameWidget::kNormalized;
			m_clickRegion = (int)FrameWidget::kSheetNone;

			m_scrollValueX = 0;
			m_scrollValueY = 0;
			m_mouseModifiers = FrameWidget::kNoModifiers;
			m_mouseClickType = FrameWidget::kNoClick;
			m_mouseStateMode = FrameWidget::kIdleMode;

			setCursor(Qt::ArrowCursor);
			setMouseTracking(true);

			m_timer = new QBasicTimer();
			m_scrollTimer = new QBasicTimer();

			m_deleteSelectedFrames = new QAction(tr("Remove Selected Frames"), this);
			connect(m_deleteSelectedFrames, SIGNAL(triggered()), this, SLOT(removeSelectedFrames()));

			m_propGroups = new QAction(tr("Propagate Groups"), this);
			connect(m_propGroups, SIGNAL(triggered()), this, SIGNAL(propGroups()));

			m_replaceGroups = new QAction(tr("Propagate Groups and Override"), this);
			connect(m_replaceGroups, SIGNAL(triggered()), this, SIGNAL(replaceGroups()));

			m_smallGridView = new QAction(tr("Small Grid"), this);
			m_smallGridView->setCheckable(true);
			m_smallGridView->setChecked(false);
			connect(m_smallGridView, SIGNAL(toggled(bool)), this, SLOT(setSmallGrid(bool)));

			m_duplicateSelection = new QAction(tr("Duplicate Selection"), this);
			connect(m_duplicateSelection, SIGNAL(triggered()), this, SLOT(showDuplicateDialog()));

			m_lockIcon = QIcon(QPixmap(QString::fromUtf8(":/app_drawing/res/Layer/ls_lock_light.png")));

			m_frameContext = new QMenu(this);
			m_frameContext->addAction(m_deleteSelectedFrames);
			m_frameContext->addSeparator();
			m_frameContext->addAction(m_propGroups);
			m_frameContext->addAction(m_replaceGroups);
			m_frameContext->addSeparator();
			m_frameContext->addAction(m_smallGridView);
			m_frameContext->addSeparator();
			m_frameContext->addAction(m_duplicateSelection);


			m_directoryPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();

			//m_creatorDialog = new CreateFrameDialog(this);
			//m_creatorDialog->hide();

			m_gridWidth = SHEET_WIDTH;

			m_celLid.addIndex(0);
			m_celLid.addIndex(0);
			m_celLid.setIndent(1);
			m_parentCel = 0;


			//setFocusPolicy(Qt::NoFocus);
		}

		FrameWidget::~FrameWidget()
		{
			delete m_fontMetrics;
			delete m_timer;
			delete m_scrollTimer;
		}

		void FrameWidget::setSmallGrid(bool smallMode)
		{
			if (smallMode)
				m_gridWidth = SHEET_SMALL_WIDTH;
			else
				m_gridWidth = SHEET_WIDTH;

			update();
			Q_EMIT smallGrid(smallMode);
		}

		void FrameWidget::panToolIn(bool in)
		{
			m_panToolIn = in;
			if (m_mouseClickType == FrameWidget::kNoClick)
			{
				if (m_panToolIn)
					setCursor(Qt::OpenHandCursor);
			}
		}

		int FrameWidget::calculateTotalWidth()
		{
			int maxWidth = 0;

			for (size_t i = 0; i<m_layerController->getCurrentRoot()->memberCount(); ++i)
			{
				const LayerGroup* c = dynamic_cast<const LayerGroup*>(m_layerController->getCurrentRoot()->childAtIndex(i));
				int frameOffset = c->frameOffset();
				int currentWidth = (static_cast<int>(c->sheetCount()) + frameOffset + FRAME_EXTRA)*(m_gridWidth + SHEET_BORDER) + SHEET_BORDER;
				if (currentWidth > maxWidth)
					maxWidth = currentWidth;
			}

			int front = 0, back = 0;

			
			int currentActiveFrame = m_selector->absoluteActiveSheetId().fid();
			int currentActiveFrameWidth = (currentActiveFrame + 2)*(m_gridWidth + SHEET_BORDER) + SHEET_BORDER;

			if (maxWidth < currentActiveFrameWidth)
				maxWidth = currentActiveFrameWidth;

			return maxWidth;
		}

		int FrameWidget::calculateActiveScrollRange()
		{
			
			int activeFrameLeft = (m_selector->activeFrame() + 1)*(m_gridWidth + SHEET_BORDER) + SHEET_BORDER;
			int activeFrameRight = (m_selector->activeFrame())*(m_gridWidth + SHEET_BORDER) + SHEET_BORDER;

			if (activeFrameLeft > width() + m_scrollValueX)
				return activeFrameLeft - width() - m_scrollValueX;
			else if (activeFrameRight < m_scrollValueX)
				return activeFrameRight - m_scrollValueX;
			else
				return 0;
		}

		void FrameWidget::setScrollPosX(int pos)
		{
			m_scrollValueX = pos;

			if (m_cacheEvent.buttons() != Qt::NoButton && m_cacheEvent.type() == QEvent::MouseMove)
				mouseMoveEvent(&m_cacheEvent);

			update();
		}

		void FrameWidget::setScrollPosY(int pos)
		{
			m_scrollValueY = pos;

			if (m_cacheEvent.buttons() != Qt::NoButton && m_cacheEvent.type() == QEvent::MouseMove)
				mouseMoveEvent(&m_cacheEvent);

			update();
		}

		QSize FrameWidget::sizeHint() const
		{
			return QSize(400, FRAMEWID_HEIGHT);
		}

		QSize FrameWidget::minimumSizeHint() const
		{
			return QSize(100, FRAMEWID_HEIGHT);
		}

		QSize FrameWidget::maximumSizeHint() const
		{
			return QSize(QWIDGETSIZE_MAX, FRAMEWID_HEIGHT);
		}

		void FrameWidget::paintEvent(QPaintEvent* e)
		{
			paintNormalized();
		}

		void FrameWidget::timerEvent(QTimerEvent* e)
		{
			if (e->timerId() == m_timer->timerId())
			{
				if (!m_timerUp)
				{
					m_timerUp = true;
					setCursor(Qt::OpenHandCursor);
				}
			}
			else if (e->timerId() == m_scrollTimer->timerId() && m_cacheEvent.buttons() != Qt::NoButton
				&& m_cacheEvent.type() == QEvent::MouseMove)
			{
				if (m_cacheEvent.x() < AUTO_SCROLL_DEF)
				{
					Q_EMIT sliderBarLeft(m_gridWidth + SHEET_BORDER);
				}
				else if (m_cacheEvent.x() > width() - AUTO_SCROLL_DEF)
				{
					Q_EMIT sliderBarRight(m_gridWidth + SHEET_BORDER);
				}

				if (m_cacheEvent.y() < AUTO_SCROLL_DEF)
				{
					Q_EMIT sliderBarTop(SHEET_HEIGHT + SHEET_BORDER);
				}
				else if (m_cacheEvent.y() > height() - AUTO_SCROLL_DEF)
				{
					Q_EMIT sliderBarBottom(SHEET_HEIGHT + SHEET_BORDER);
				}
			}
		}

		void FrameWidget::paintNormalized()
		{
			QPainter p(this);
			p.translate(SHEET_BORDER - m_scrollValueX, -m_scrollValueY);

			QPoint startPt1, startPt2, endPt1, endPt2;

			int accumHeight = SHEET_BORDER;
			//Drawing the sheet grid
			//vector to store the top y coord of the selected LAYERS
			vector<int> rectPoints;
			bool layerIsSelected = false;
			bool celIsSelected = false;

			//Render the layers
			//Loopin through all the cels
			//string x = m_layerController->getCurrentRoot()->
			QLOG_INFO() << "FrameWidget:" << " m_layerController->getCurrentRoot() (originally name root cut type)->memberCount() = " << m_layerController->getCurrentRoot()->memberCount();

			for (size_t i = 0; i<m_layerController->getCurrentRoot()->memberCount(); ++i)
			{
				const LayerGroup* cel = dynamic_cast<const LayerGroup*>(m_layerController->getCurrentRoot()->childAtIndex(i));
				//Obtain the layerI d of this cel
				LayerId celId = m_selector->findLayerId(cel); 
				//Check if this cel is selected via its layerId
				//if selected store the y coordinate into the vector rectPoints
				if (m_selector->layerIsSelected(celId))
					rectPoints.push_back(accumHeight + SHEET_BORDER);

				//Frame offset of the cel
				int frameOffSet = cel->frameOffset()*(m_gridWidth + SHEET_BORDER);
				//If the user is in the middle of modifying the frame offset and the cel pointer matches the modified cel
				//Add the additional frames values, m_numberOfFrame to frameOffset.
				if (m_mouseMoveType == FrameWidget::kCelOffset && cel == m_parentCel)
					frameOffSet += m_numberOfFrame*(m_gridWidth + SHEET_BORDER);

				//Define the width of a rectangle that encompasses the entire cel's sheets
				int rectWidth = static_cast<int>(cel->sheetCount())*(m_gridWidth + SHEET_BORDER) + SHEET_BORDER;
				//If user is in the midst of creating or deleting frames, adjust the width accordingly.
				if (m_mouseMoveType == kFrameCreating && cel == m_parentCel)
					rectWidth += m_numberOfFrame*(m_gridWidth + SHEET_BORDER);
				else if (m_mouseMoveType == kFrameRemoving && cel == m_parentCel)
					rectWidth -= m_numberOfFrame*(m_gridWidth + SHEET_BORDER);

				//Define the height of a rectangle that encompasses the entire cel's sheets
				//First only the height of the cel
				int rectHeight = (SHEET_BORDER * 2) + (cel->isExpanded() ? (static_cast<int>(cel->memberCount()) + 1) : 1)*SHEET_HEIGHT;
				//If cel is expanded, add in the height of its children
				if (cel->isExpanded())
					rectHeight += static_cast<int>(cel->memberCount())*TIMELINE_NORM_TOP_BUFFER;

				//Define the rectangle that encompass the entire cel
				QRect bkRect(frameOffSet - SHEET_BORDER, accumHeight, rectWidth, rectHeight);
				//If the cel's rectangle does not intersect with the widget's space, skip, get the y coordinate of the next cel and go to the next one.
				if (!rangeIntersects(bkRect.adjusted(-m_scrollValueX, -m_scrollValueY, -m_scrollValueX, -m_scrollValueY).top(),
					bkRect.adjusted(-m_scrollValueX, -m_scrollValueY, -m_scrollValueX, -m_scrollValueY).bottom(),
					rect().top(), rect().bottom()))
				{
					accumHeight += rectHeight + TIMELINE_INTER_CEL;
					continue;
				}

				//Reduce the size of the cel's rectangle to fit the widget's space.
				if (bkRect.x() - m_scrollValueX < -SHEET_BORDER)
					bkRect.setLeft(m_scrollValueX - SHEET_BORDER);
				if (bkRect.right() - m_scrollValueX > rect().right())
					bkRect.setRight(rect().right() + m_scrollValueX);
				if (bkRect.y() - m_scrollValueY < 0)
					bkRect.setTop(m_scrollValueY);
				if (bkRect.bottom() - m_scrollValueY > rect().bottom())
					bkRect.setBottom(rect().bottom() + m_scrollValueY);

				//Draw the rectangle if the cel has sheets
				if (cel->sheetCount() > 0)
				{
					p.setPen(Qt::NoPen);
					p.setBrush(TIMELINE_CELBOX_BKGN);
					p.drawRect(bkRect);
				}

				//Define a rectangle that encompass the entire cel width but only 1 sheet high.
				QRect textRect(bkRect.left(), bkRect.top(), bkRect.width(), SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER + SHEET_BORDER);

				if (!cel->isExpanded())
				{
					//Calculate the min starting frame number to render
					int k = m_scrollValueX / (m_gridWidth + SHEET_BORDER) - cel->frameOffset();
					k = max(0, k);
					//Calculate the max frame number to render.
					int maxRender = (m_scrollValueX + width()) / (m_gridWidth + SHEET_BORDER) - cel->frameOffset() + 1;
					maxRender = min(static_cast<int>(cel->sheetCount()), maxRender);
					//Looping to render the localized frame numbers for the cel
					for (; k<maxRender; ++k)
					{
						//If the user is in the midst of removing frames.
						if (m_mouseMoveType == FrameWidget::kFrameRemoving && m_parentCel == cel)
						{
							int firstFrame;
							//m_startFrame is the frame which the removing operation first start.
							//frames between m_startFrame and m_startFrame - m_numberOfFrame is considered for 
							//removal, thus if k is between these 2 values, do not render.
							if (m_clickRegion == (int)FrameWidget::kSheetLeft)
							{
								firstFrame = m_startFrame - m_numberOfFrame;
								if (k >= firstFrame && k < m_startFrame)
									continue;
							}
							else if (m_clickRegion == (int)FrameWidget::kSheetRight)
							{
								firstFrame = m_startFrame + 1 - m_numberOfFrame;
								if (k >= firstFrame && k < m_startFrame + 1)
									continue;
							}
						}

						//Getting the sheet of the cel at frame k
						const Sheet* s = cel->sheetAtIndex(k);
						//If sheet exists
						if (s)
						{
							QRect sheetRect;
							//If user is creating new frames and the current cel matches the modifying cel
							if (m_mouseMoveType == FrameWidget::kFrameCreating && m_parentCel == cel)
							{
								//if this sheet is affected by the user interaction when creating frames, factor in the number of frames moved.
								if ((m_clickRegion == (int)FrameWidget::kSheetRight && k>m_startFrame) ||
									(m_clickRegion == (int)FrameWidget::kSheetLeft && k >= m_startFrame))
									sheetRect = QRect((k + m_numberOfFrame)*(m_gridWidth + SHEET_BORDER) + frameOffSet, accumHeight + SHEET_BORDER + 1 * (SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER), m_gridWidth, SHEET_HEIGHT);
								else
									sheetRect = QRect(k*(m_gridWidth + SHEET_BORDER) + frameOffSet, accumHeight + SHEET_BORDER + 1 * (SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER), m_gridWidth, SHEET_HEIGHT);

							}
							//If user is removing frames and the current cel matches the modifying cel
							else if (m_mouseMoveType == FrameWidget::kFrameRemoving  && m_parentCel == cel)
							{
								//if this sheet is affected by the user interaction when creating frames, factor in the number of frames moved.
								if ((m_clickRegion == (int)FrameWidget::kSheetLeft && k >= m_startFrame - m_numberOfFrame) ||
									(m_clickRegion == (int)FrameWidget::kSheetRight && k >= m_startFrame - m_numberOfFrame + 1))
									sheetRect = QRect((k - m_numberOfFrame)*(m_gridWidth + SHEET_BORDER) + frameOffSet, accumHeight + SHEET_BORDER + 1 * (SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER), m_gridWidth, SHEET_HEIGHT);
								else
									sheetRect = QRect(k*(m_gridWidth + SHEET_BORDER) + frameOffSet, accumHeight + SHEET_BORDER + 1 * (SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER), m_gridWidth, SHEET_HEIGHT);
							}
							//If no action on this cel, just define the rectangle for that sheet.
							else
							{
								sheetRect = QRect(k*(m_gridWidth + SHEET_BORDER) + frameOffSet, accumHeight + SHEET_BORDER + 1 * (SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER), m_gridWidth, SHEET_HEIGHT);
							}

							//Rendering the frame numbers
							QRect frameNoRect(sheetRect.left(), textRect.top(), sheetRect.width(), sheetRect.top() - textRect.top());
							p.setBrush(Qt::NoBrush);
							p.setPen(QPen(CEL_LEVEL_NUM_COLOR, 1));
							p.drawText(frameNoRect, Qt::AlignVCenter | Qt::AlignHCenter, QString::number(k + 1));
						}
					}
				}
				else
				{
					//Looping through the cel's children.
					for (int j = 0; j<static_cast<int>(cel->memberCount()); ++j)
					{
						const Layer* l = cel->childAtIndex(j);
						//if selected store the y coordinate into the vector rectPoints
						if (layerIsSelected = m_selector->layerIsSelected(m_selector->findLayerId(l)))
							rectPoints.push_back(accumHeight + SHEET_BORDER + (j + 1)*(SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER));

						QColor keySelectColor, keyUnselectedColor, tweenSelectColor, tweenUnselectedColor;
						//Get the different layer colors
						getLayerColors(keySelectColor, keyUnselectedColor, tweenSelectColor, tweenUnselectedColor, l);
						QColor previousColor;

						int k1 = m_scrollValueX / (m_gridWidth + SHEET_BORDER);
						int k2 = cel->frameOffset();
						
						//Calculate the min starting frame number to render
						int k = k1 - k2;
						k = max(0, k);
						//Calculate the max frame number to render.
						int maxRender = (m_scrollValueX + width()) / (m_gridWidth + SHEET_BORDER) - cel->frameOffset() + 1;
						maxRender = min(static_cast<int>(cel->sheetCount()), maxRender);
						for (; k<maxRender; ++k)
						{
							if (m_mouseMoveType == FrameWidget::kFrameRemoving && m_parentCel == cel)
							{
								int firstFrame;
								//m_startFrame is the frame which the removing operation first start.
								//frames between m_startFrame and m_startFrame - m_numberOfFrame is considered for 
								//removal, thus if k is between these 2 values, do not render.
								if (m_clickRegion == (int)FrameWidget::kSheetLeft)
								{
									firstFrame = m_startFrame - m_numberOfFrame;
									if (k >= firstFrame && k < m_startFrame)
										continue;
								}
								else if (m_clickRegion == (int)FrameWidget::kSheetRight)
								{
									firstFrame = m_startFrame + 1 - m_numberOfFrame;
									if (k >= firstFrame && k < m_startFrame + 1)
										continue;
								}
							}

							//Getting the sheet of the layer at frame k
							const Sheet* s = l->sheetAtIndex(k);
							//If sheet exists
							if (s)
							{
								unsigned int startingFrame = k;
								//Check if the sheet is a repeat sheet aka extends
								//If it is a repeat sheet
								if (s->sheetType() == Sheet::kRepeatSheet)
								{
									//Keep looping backwards until a non-repeat sheet is found.
									//Check for previous original reference
									unsigned int refFrame = k - 1;
									for (; refFrame >= 0; --refFrame)
									{
										const Sheet* refSheet = l->sheetAtIndex(refFrame);
										if (refSheet->sheetType() != Sheet::kRepeatSheet)
										{
											//Getting the previous color
											if (l->layerType() == ABS_CEL)
											{
												//Storing the color of the reference sheet for rendering the repeat later
												//const CelSheet* cs = dynamic_cast<const CelSheet*>(refSheet);
												//if (cs->getChildSheetType() == TID_KEY_SHEET)
												//{
													if (layerIsSelected)
														previousColor = keySelectColor;
													else
														previousColor = keyUnselectedColor;
												//}
												//else
												//{
													//	if (layerIsSelected)
													//		previousColor = tweenSelectColor;
													//	else
													//	previousColor = tweenUnselectedColor;
													//}
											}
											else
											{
												//Storing the color of the reference sheet for rendering the repeat later
												//const DrawingSheet* ds = dynamic_cast<const DrawingSheet*>(refSheet);
												//if (ds->sheetFlag() == DrawingSheet::kKey)
												//{
												if (layerIsSelected)
														previousColor = keySelectColor;
												else
														previousColor = keyUnselectedColor;
												//}
												//else
												//{
												//	if (layerIsSelected)
												//		previousColor = tweenSelectColor;
												//	else
												//		previousColor = tweenUnselectedColor;
													//}
											}

											break;
										}
									}

								}

								QRect sheetRect;
								//if this sheet is affected by the user interaction when creating frames, factor in the number of frames moved.
								if (m_mouseMoveType == FrameWidget::kFrameCreating && m_parentCel == cel)
								{
									if ((m_clickRegion == (int)FrameWidget::kSheetRight && k>m_startFrame) ||
										(m_clickRegion == (int)FrameWidget::kSheetLeft && k >= m_startFrame))
										sheetRect = QRect((k + m_numberOfFrame)*(m_gridWidth + SHEET_BORDER) + frameOffSet, accumHeight + SHEET_BORDER + (j + 1)*(SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER), m_gridWidth + 1, SHEET_HEIGHT);
									else
										sheetRect = QRect(k*(m_gridWidth + SHEET_BORDER) + frameOffSet, accumHeight + SHEET_BORDER + (j + 1)*(SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER), m_gridWidth + 1, SHEET_HEIGHT);

								}
								//if this sheet is affected by the user interaction when creating frames, factor in the number of frames moved.
								else if (m_mouseMoveType == FrameWidget::kFrameRemoving  && m_parentCel == cel)
								{
									if ((m_clickRegion == (int)FrameWidget::kSheetLeft && k >= m_startFrame - m_numberOfFrame) ||
										(m_clickRegion == (int)FrameWidget::kSheetRight && k >= m_startFrame - m_numberOfFrame + 1))
										sheetRect = QRect((k - m_numberOfFrame)*(m_gridWidth + SHEET_BORDER) + frameOffSet, accumHeight + SHEET_BORDER + (j + 1)*(SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER), m_gridWidth + 1, SHEET_HEIGHT);
									else
										sheetRect = QRect(k*(m_gridWidth + SHEET_BORDER) + frameOffSet, accumHeight + SHEET_BORDER + (j + 1)*(SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER), m_gridWidth + 1, SHEET_HEIGHT);
								}
								//If no action on this cel, just define the rectangle for that sheet.
								else
								{
									sheetRect = QRect(k*(m_gridWidth + SHEET_BORDER) + frameOffSet, accumHeight + SHEET_BORDER + (j + 1)*(SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER), m_gridWidth + 1, SHEET_HEIGHT);
								}

								//Check if the sheet rect is within the widget's draw area/window
								if (sheetRect.left() - m_scrollValueX < width() || sheetRect.right() - m_scrollValueX > 0)
								{
									//Rendering the frame numbers
									if (j == 0)
									{
										QRect frameNoRect(sheetRect.left(), textRect.top(), sheetRect.width(), sheetRect.top() - textRect.top());
										p.setBrush(Qt::NoBrush);
										p.setPen(QPen(CEL_LEVEL_NUM_COLOR, 1));
										p.drawText(frameNoRect, Qt::AlignVCenter | Qt::AlignHCenter, QString::number(k + 1));
									}

									//Rendering the sheets corresponding to different types of layer
									if (l->layerType() == ABS_COLOR_SEP_LAYER || l->layerType() == ABS_DRAWING_LAYER)
									{
										//Rendering repeat sheet using its reference frame colors.
										//A horizontal line is drawn across to differentiate it from others
										if (s->sheetType() == Sheet::kRepeatSheet)
										{
											p.setBrush(previousColor);
											sheetRect.adjust(-2, 0, 0, 0);
											p.setPen(Qt::NoPen);
											p.drawRect(sheetRect);
											p.setPen(QPen(Qt::black, 1));
											p.drawLine(sheetRect.left(), sheetRect.center().y(), sheetRect.right(), sheetRect.center().y());
										}
										else
										{
											//Rendering selected 
											if (layerIsSelected)
											{
												p.setBrush(keySelectColor);
												previousColor = keySelectColor;
											}
											//Rendering non-selected
											else
											{
												
												p.setBrush(keyUnselectedColor);
												previousColor = keyUnselectedColor;
											}

											p.setPen(Qt::NoPen);
											p.drawRect(sheetRect);

											//A short line is drawn near the bottom of the rect to differentiate from other sheets(not present in the codes here)
											p.setPen(QPen(Qt::black, 1));
											p.drawLine(sheetRect.left() + SHEET_KEY_BAR_LEFT_DISP, sheetRect.top() + SHEET_BAR_DISP,
												sheetRect.right() - SHEET_KEY_BAR_LEFT_DISP, sheetRect.top() + SHEET_BAR_DISP);

										}
									}
									//Rendering for other type of layers(not present in codes here)
									else
									{
										//selected - orange
										if (layerIsSelected)
											p.setBrush(keySelectColor);
										//showing active (clicked) frame - dark middle light blue
										else if ((k == m_layerController->layerSelector()->activeFrame()) || ((k == m_layerController->layerSelector()->activeLayer()->sheetCount()-1)&&(m_layerController->layerSelector()->activeFrame()>k)))
										{
											//layer focused
											if ((i == m_layerController->layerSelector()->activeCelIndex())&&(j == m_layerController->layerSelector()->activeCelChildIndex()))
											{
												//dark blue
												QColor tmp(0, 0, 255);
												p.setBrush(tmp);
											}
											//layer is not focused, but layer group is active
											else if (i == m_layerController->layerSelector()->activeCelIndex()){
												//middle blue
												QColor tmp(0, 128, 255);
												p.setBrush(tmp);
											}
											//inactive
											else
											{
												//light blue
												if (k < m_layerController->layerSelector()->activeLayer()->sheetCount()){
													QColor tmp(153, 204, 255);
													p.setBrush(tmp);
												}
												//need to exclude the context that mouse is clicked out of normal region
												//which leads to more light blue blocks
												else{
													p.setBrush(keyUnselectedColor);
												}
	
											}
											
										}
										//unselected
										else
										{ 
											
											p.setBrush(keyUnselectedColor);

										}

										if (s->sheetType() == Sheet::kRepeatSheet)
										{
											sheetRect.adjust(-2, 0, 2, 0);
											p.setPen(Qt::NoPen);
											p.drawRect(sheetRect);
										}
										else
										{
											p.setPen(Qt::NoPen);
											p.drawRect(sheetRect);
										}
									}
								}
							}
						}
					}
				}
				accumHeight += rectHeight + TIMELINE_INTER_CEL;
			}

			//Drawing the selected frames
			//Getting the set of selected frames
			set<FrameId> sFrames = m_selector->selectedFrames();
			p.setPen(QPen(TIMELINE_LIGHTBOX_ORAN, 1));
			p.setBrush(TIMELINE_LIGHTBOX_DULL_ORAN);
			//Looping thru the previously stored selected layers y coordinates
			
			
			for (size_t i = 0; i<rectPoints.size(); ++i)
			{
				int rectPoint = rectPoints[i];
				for (auto fid : sFrames)
				{
					//Render the active frame later
					if (fid == m_selector->absoluteActiveFrame())
						continue;

					QRect rect;
					//Define rendering rect and render
					//if this sheet is affected by the user interaction when creating frames, factor in the number of frames moved.
					if (m_mouseMoveType == kFrameCreating)
					{
						if ((m_clickRegion & FrameWidget::kSheetRight && m_startFrame + m_parentCel->frameOffset() < fid)
							|| (m_clickRegion & FrameWidget::kSheetLeft && m_startFrame + m_parentCel->frameOffset() <= fid))
						{
							rect = QRect((fid + m_numberOfFrame)*(m_gridWidth + SHEET_BORDER), rectPoint, m_gridWidth, SHEET_HEIGHT);
						}
						else
							rect = QRect(fid*(m_gridWidth + SHEET_BORDER), rectPoint, m_gridWidth, SHEET_HEIGHT);
					}
					else
					{
						rect = QRect(fid*(m_gridWidth + SHEET_BORDER), rectPoint, m_gridWidth, SHEET_HEIGHT);
					}

					p.drawRect(rect);
				}

				QRect activeRect;
				//Render the active sheet
				if (m_mouseMoveType == kFrameCreating)
				{
					//Factor in user creating/removing frames
					if ((m_clickRegion & FrameWidget::kSheetRight && m_startFrame + m_parentCel->frameOffset() < m_selector->absoluteActiveFrame())
						|| (m_clickRegion & FrameWidget::kSheetLeft && m_startFrame + m_parentCel->frameOffset() <= m_selector->absoluteActiveFrame()))
						activeRect = QRect((m_selector->absoluteActiveFrame() + m_numberOfFrame)*(m_gridWidth + SHEET_BORDER), rectPoint, m_gridWidth, SHEET_HEIGHT);
					else
						activeRect = QRect(m_selector->absoluteActiveFrame()*(m_gridWidth + SHEET_BORDER), rectPoint, m_gridWidth, SHEET_HEIGHT);

				}
				else
					activeRect = QRect(m_selector->absoluteActiveFrame()*(m_gridWidth + SHEET_BORDER), rectPoint, m_gridWidth, SHEET_HEIGHT);

				p.drawRect(activeRect.adjusted(-1, -1, 1, 0));

			}

			
			// draw in sections of consecutive frames
			if (sFrames.size() > 0)
			{
			std::vector<FrameId> sFramesCopy(sFrames.size());
			std::copy(sFrames.begin(), sFrames.end(), sFramesCopy.begin());
			std::sort(sFramesCopy.begin(), sFramesCopy.end());
			FrameId start, last;
			start = last = sFramesCopy.at(0);

				auto getX = [&, this](int fstart, int fend, int& x, int& selectionWidth)
				{
					// range is from start to last
					int nframes = fend - fstart + 1;// sFrames.size();
					selectionWidth = (m_gridWidth)* nframes + (nframes - 1) * SHEET_BORDER + SHEET_BORDER;
					//int offset = 0;// (m_mouseMoveType == kFrameCreating && fstart > m_startFrame) ? m_numberOfFrame : 0;

					x = (fstart /*+ offset*/)*(m_gridWidth + SHEET_BORDER) + SHEET_BORDER / 2;
				};

				auto highlightRanges = [&, this](int fstart, int fend)
				{
					int x;
					int selectionWidth;
					getX(fstart, fend, x, selectionWidth);
					p.resetTransform();
					p.translate(-m_scrollValueX, 0);
					p.drawLine(x, 0, x, height());
					p.drawLine(x + selectionWidth, 0, x + selectionWidth, height());
					p.resetTransform();
				};

				m_selector->applyToSelectedFrameRanges(highlightRanges);
		    }

			//Draw Rubberband for the rubberband selection
			if (m_mouseMoveType == FrameWidget::kRubberBand)
			{
				p.resetTransform();
				p.translate(-m_scrollValueX, -m_scrollValueY);
				p.setPen(QPen(Qt::black, 1));
				p.setBrush(Qt::NoBrush);
				p.setCompositionMode(QPainter::RasterOp_NotSourceXorDestination);
				p.drawRect(m_initialPoint.x(), m_initialPoint.y(), m_movingPoint.x() - m_initialPoint.x(),
					m_movingPoint.y() - m_initialPoint.y());
			}

			//Drawing the dragging line displayed during moving layers
			if (m_mouseMoveType == FrameWidget::kMove)
			{
				int currentMovingFrame = m_startFrame + m_numberOfFrame;
				if (currentMovingFrame != m_startFrame && currentMovingFrame != m_startFrame + 1)
				{
					if (currentMovingFrame > static_cast<int>(m_parentCel->sheetCount()) + m_parentCel->frameOffset())
						currentMovingFrame = static_cast<int>(m_parentCel->sheetCount()) + m_parentCel->frameOffset();

					if (currentMovingFrame < m_parentCel->frameOffset())
						currentMovingFrame = m_parentCel->frameOffset();

					int xCoord = currentMovingFrame*(m_gridWidth + SHEET_BORDER);
					if (xCoord - m_scrollValueX >= 0 && xCoord - m_scrollValueX <= rect().width())
					{
						p.setPen(QPen(Qt::white, 2));
						p.drawLine(xCoord, 0, xCoord, rect().bottom());
					}
				}
			}
		}

		void FrameWidget::mousePressEvent(QMouseEvent* e)
		{
			QWidget* fw = QApplication::focusWidget();
			if (m_panToolIn)
			{
				m_initialPoint = e->pos();
				m_mouseClickType = FrameWidget::kSingleClick;
				m_mouseMoveType = FrameWidget::kPan;
				setCursor(Qt::ClosedHandCursor);
			}
			else
			{
				mousePressNorm(e);
			}
		}

		void FrameWidget::mouseMoveEvent(QMouseEvent* e)
		{
			if (m_mouseMoveType == FrameWidget::kPan && m_mouseClickType == FrameWidget::kSingleClick)
			{
				QPoint diff = e->pos() - m_initialPoint;
				m_initialPoint = e->pos();
				Q_EMIT framePanned(-diff.x(), -diff.y());
			}
			else
			{
				mouseMoveNorm(e);
			}
		}

		void FrameWidget::mouseReleaseEvent(QMouseEvent* e)
		{
			if (m_mouseMoveType == FrameWidget::kPan)
			{
				m_mouseMoveType = FrameWidget::kNone;
				m_mouseClickType = FrameWidget::kNoClick;
				setMouseCursor(e);
			}
			else
			{
				mouseReleaseNorm(e);
				e->ignore();
			}
		}

		void FrameWidget::mouseDoubleClickEvent(QMouseEvent* e)
		{
			mouseDblClkNorm(e);

			mouseReleaseEvent(e);
		}

		void FrameWidget::contextMenuEvent(QContextMenuEvent* e)
		{
			m_timer->stop();
			if (m_selector->activeCelRaw())
			{
				m_mouseClickType = FrameWidget::kNoClick;
				m_clickRegion = (int)FrameWidget::kSheetNone;
				m_frameContext->move(e->globalPos());
				m_frameContext->exec();
			}

		}

		void FrameWidget::getFrame(int x, int& frame, int& leftover)
		{
			int x2 = x - SHEET_BORDER / 2 + m_scrollValueX;
			frame = x2 / (m_gridWidth + SHEET_BORDER);
			leftover = x2 % (m_gridWidth + SHEET_BORDER);
		}

		void FrameWidget::selectFrames(QMouseEvent* e, int frameOffset)
		{
			
			FrameId fid = static_cast<FrameId>(m_startFrame + frameOffset);


			LayerId lid;
			lid = m_selector->findLayerId(m_clickedLayer, m_parentCel);

			if (!(e->button() == Qt::RightButton && m_selector->frameIsSelected(fid)))
			{
				// select frames and layers
				if (e->modifiers() && Qt::ControlModifier && !(e->modifiers() && Qt::ShiftModifier))
				{	// Ctrl select
					if (m_selector->frameIsSelected(fid) && m_selector->activeFrame() != fid)
						m_selector->popSelectedFrame(fid);
					else
					{
						m_selector->addSelectedFrame(fid);
						m_selector->addSelectedLayer(lid);
					}
				}
				else if (e->modifiers() && Qt::ShiftModifier && !(e->modifiers() && Qt::ControlModifier))
				{	// Shift select
					//m_selector->clearAll();
					m_selector->selectFrameRange(m_startFrame + frameOffset);

					m_selector->selectLayerRange(lid);
				}
				else
				{
					m_selector->setActiveSheet(SheetId(lid, m_startFrame + frameOffset));
					m_selector->setActiveFrame(m_startFrame + frameOffset);
					m_selector->setActiveLayer(m_selector->findLayerId(m_clickedLayer));
				}
			}
			
		}

		void FrameWidget::mousePressNorm(QMouseEvent* e)
		{
			///m_selector->deselectAll();
			m_cacheEvent = QMouseEvent(QEvent::MouseButtonPress, e->pos(), e->button(), e->buttons(),
				e->modifiers());
			if (e->button() == Qt::LeftButton || e->button() == Qt::RightButton)
			{
				m_mouseMoveType = FrameWidget::kNoMove;
				m_mouseClickType = FrameWidget::kSingleClick;
				m_mouseModifiers = (FrameWidget::MouseModifiers)((int)e->modifiers());
				//m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);

				if (m_stateMachine->isIdleState())
				{
					int leftOver;
					//set the startframe and leftover frames
					getFrame(e->x(), m_startFrame, leftOver);

					int accumHeight = 0;
					m_clickedLayer = 0;
					const LayerGroup* selectedParent = 0;
					int y = e->y() + m_scrollValueY;
					findLayer(y, m_clickedLayer, selectedParent);

					if (selectedParent)
					{
						int frameOffset = 0;
						m_parentCel = selectedParent;
						if (m_clickedLayer && selectedParent)
						{
							frameOffset = m_parentCel->frameOffset();
						}

						//Hit testing for frames
						m_startFrame -= m_parentCel->frameOffset();
						if (m_clickedLayer)
						{
							frameOffset = m_parentCel->frameOffset();
							if (m_clickedLayer == m_parentCel)
							{
								if (m_startFrame >= static_cast<int>(m_clickedLayer->sheetCount()) || m_startFrame < 0)
									m_clickRegion = m_clickRegion | FrameWidget::kSheetNoDrag;
								else if (e->button() == Qt::LeftButton)
								{
									m_clickRegion = m_clickRegion | FrameWidget::kCelRegion;

								}

								// select cel
								LayerId lid;
								lid = m_selector->findLayerId(m_clickedLayer, m_parentCel);
								//Move selecting to mouseRelease
								//m_selector->setActiveFrame(m_startFrame + frameOffset);
								//m_selector->setActiveLayer(lid);
							}
							else
							{
								if (m_startFrame >= static_cast<int>(m_clickedLayer->sheetCount()) || m_startFrame < 0)
									m_clickRegion = m_clickRegion | FrameWidget::kSheetNoDrag;

								if (!(m_clickRegion & FrameWidget::kSheetNoDrag) && !(m_clickRegion & FrameWidget::kSheetNone))
								{
									if (m_clickedLayer->sheetCount() > 0)
									{
										if (leftOver >= 0 && leftOver <= SHEET_BORDER / 2 + SHEET_INNER)
										{
											m_clickRegion = m_clickRegion | FrameWidget::kSheetLeft;
										}
										else if (leftOver >= m_gridWidth - SHEET_INNER + SHEET_BORDER / 2 && leftOver <= m_gridWidth + SHEET_BORDER / 2)
										{
											m_clickRegion = m_clickRegion | FrameWidget::kSheetRight;
										}
										else if (leftOver > SHEET_BORDER / 2 + SHEET_INNER && leftOver < SHEET_BORDER / 2 + m_gridWidth - SHEET_INNER)
										{
											m_clickRegion = m_clickRegion | FrameWidget::kSheetMiddle;
											m_timerUp = false;
											m_timer->start(1000, this);
										}
										else
											m_clickRegion = (int)FrameWidget::kSheetNone;
									}
									else
										m_clickRegion = (int)FrameWidget::kSheetNone;
								}
								else
								{
									if (leftOver >= 0 && leftOver <= m_gridWidth + SHEET_BORDER / 2)
										m_clickRegion = m_clickRegion | FrameWidget::kSheetMiddle;
									else
										m_clickRegion = (int)FrameWidget::kSheetNone;
								}

								//selectFrames(e, frameOffset, false);
							}

							m_celLid = LayerId(0);
							m_celLid = m_selector->findLayerId(m_parentCel, m_parentCel);
						}
						else
						{
							m_clickRegion = (int)FrameWidget::kSheetNone;
						}
						
						//GAOHE added
						m_frameOffset = frameOffset;

						m_initialPoint.setX(e->x() + m_scrollValueX);
						m_initialPoint.setY(e->y() + m_scrollValueY);
						m_movingPoint = m_initialPoint;
					}

				}

				//Moved to mouseRelease
				/*if (m_parentCel)
				selectFrames(e, m_parentCel->frameOffset(), false);*/
				//m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
			}
		}

		void FrameWidget::mouseMoveNorm(QMouseEvent* e)
		{
			m_cacheEvent = QMouseEvent(QEvent::MouseMove, e->pos(), e->button(), e->buttons(),
				0);

			if (m_mouseClickType == FrameWidget::kNoClick)
			{
				setMouseCursor(e);
			}
			else
			{
				if (!m_manhattan)
				{
					QPoint current;
					current.setX(e->x() + m_scrollValueX);
					current.setY(e->y() + m_scrollValueY);
					if ((m_initialPoint - current).manhattanLength() >= QApplication::startDragDistance())
						m_manhattan = true;
				}

				if (m_manhattan && !m_timerUp)
				{
					m_timer->stop();
				}

				if (m_manhattan && m_mouseClickType == FrameWidget::kSingleClick && (e->buttons() & Qt::LeftButton))
				{
					if ((e->x() < AUTO_SCROLL_DEF || e->x() > width() - AUTO_SCROLL_DEF) && !m_scrollTimer->isActive())
						m_scrollTimer->start(250, this);

					if ((e->y() < AUTO_SCROLL_DEF || e->y() > height() - AUTO_SCROLL_DEF) && !m_scrollTimer->isActive())
						m_scrollTimer->start(250, this);

					auto rubberBandSelect = [&](bool audio){
						m_mouseMoveType = FrameWidget::kRubberBand;

						int frameSelectStart = m_initialPoint.x() - SHEET_BORDER / 2;
						int frameSelectEnd = e->x() + m_scrollValueX - SHEET_BORDER / 2;

						m_selector->clearSelectedFrames();
						m_selector->clearSelectedLayers();

						//m_selector->activeAbstractLayer() not available, changed to activeLayer()
						
						
						if (m_clickedLayer != m_selector->activeLayer() || m_startFrame + m_parentCel->frameOffset() != m_selector->activeFrame())
						{
							if (m_clickedLayer && m_parentCel)
							{
								LayerId lid;
								lid = m_selector->findLayerId(m_clickedLayer, m_parentCel);
								m_selector->setActiveSheet(SheetId(lid, m_startFrame + m_parentCel->frameOffset()), false);

								m_rubberBandSelection = true;
							}
						}

						addBandsofLayers(min(frameSelectStart, frameSelectEnd),
							max(frameSelectStart, frameSelectEnd),
							min(m_initialPoint.y(), e->y() + m_scrollValueY),
							max(m_initialPoint.y(), e->y() + m_scrollValueY));

						//m_selector->setActiveFrame(frameSelectStart);
						addBandsofFrames(min(frameSelectStart, frameSelectEnd), max(frameSelectStart, frameSelectEnd));
					};

					if (m_clickRegion & FrameWidget::kSheetMiddle && m_mouseMoveType != FrameWidget::kCelRegion && !m_timerUp)
					{
						rubberBandSelect(false);
					}
					else if (m_clickRegion != FrameWidget::kSheetNone && !(m_clickRegion & FrameWidget::kSheetNoDrag))
					{
						int x = e->x() - SHEET_BORDER / 2 + m_scrollValueX;
						int frameOffset = m_parentCel ? m_parentCel->frameOffset() : 0;
						int currentFrameNo = x / (m_gridWidth + SHEET_BORDER) - frameOffset;
						m_numberOfFrame = currentFrameNo - m_startFrame;

						if (!(m_clickRegion & FrameWidget::kSheetMiddle) && !(m_clickRegion & FrameWidget::kCelRegion))
						{
							//Adding frames
							if (m_numberOfFrame > 0)
							{
								m_mouseMoveType = FrameWidget::kFrameCreating;
							}
							//Removing frames
							else if (m_numberOfFrame < 0)
							{
								currentFrameNo = max(-1, currentFrameNo);
								m_mouseMoveType = FrameWidget::kFrameRemoving;
								m_numberOfFrame = m_startFrame - currentFrameNo;

								if (m_clickRegion & FrameWidget::kSheetRight)
									m_numberOfFrame = min((int)m_parentCel->sheetCount(), m_numberOfFrame);
								else if (m_clickRegion & FrameWidget::kSheetLeft)
									m_numberOfFrame = min(m_startFrame, m_numberOfFrame);
							}
						}
						else if ((m_clickRegion & FrameWidget::kSheetMiddle) && m_timerUp)
						{
							//Sheet Moving
							m_mouseMoveType = FrameWidget::kMove;
							int x = e->x() - SHEET_BORDER / 2 + m_scrollValueX;
							int currentFrameNo = x / (m_gridWidth + SHEET_BORDER);
							m_numberOfFrame = currentFrameNo - m_startFrame;
						}
						else if ((m_clickRegion == FrameWidget::kCelRegion))
						{
							int x = e->x() - SHEET_BORDER / 2 + m_scrollValueX;
							int currentFrameNo = x / (m_gridWidth + SHEET_BORDER) - m_parentCel->frameOffset();
							m_numberOfFrame = currentFrameNo - m_startFrame;
							m_numberOfFrame = max(-m_parentCel->frameOffset(), m_numberOfFrame);
							m_viewSettings->setTempCelOffset(m_numberOfFrame + m_parentCel->frameOffset());
							m_mouseMoveType = FrameWidget::kCelOffset;
						}
					}
					update();
				}
				//m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
			}
		}

		void FrameWidget::mouseReleaseNorm(QMouseEvent* e)
		{
			if (!m_timerUp)
			{
				m_timer->stop();
			}

			m_cacheEvent = QMouseEvent(QEvent::MouseButtonRelease, e->pos(), e->button(), e->buttons(),
				0);

			//m_selector->deselectAll();

			if (!m_manhattan)
			{
				if (m_clickRegion != FrameWidget::kSheetNone)
				{
					selectFrames(e, m_parentCel->frameOffset());
					//m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
				}
			}
			else
			{
				if (m_mouseMoveType == FrameWidget::kMove)
				{
					if (m_stateMachine->isIdleState())
					{
						//m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);

						LayerId lid;
						lid = m_selector->findLayerId(m_clickedLayer, m_parentCel);
						int index = lid.index();
						lid.setIndent(1);

						int destFrame = m_startFrame + m_numberOfFrame - m_parentCel->frameOffset();
						if (destFrame != m_startFrame && destFrame != m_startFrame + 1)
						{
							if (destFrame > static_cast<int>(m_parentCel->sheetCount()))
								destFrame = static_cast<int>(m_parentCel->sheetCount());

							if (destFrame < 0)
								destFrame = 0;

							if (m_selector->frameIsSelected(m_startFrame + m_parentCel->frameOffset()))
							{
								m_layerController->moveFrameSeq(lid, index, -1, destFrame);
							}
							else
							{
								m_layerController->moveFrameSeq(lid, index, m_startFrame, destFrame);
							}
						}

						//m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
					}
				}
				else if (m_mouseMoveType == FrameWidget::kCelOffset)
				{
					int frameOffset = m_parentCel->frameOffset();
					frameOffset += m_numberOfFrame;
					frameOffset = max(0, frameOffset);

					if (m_stateMachine->isIdleState())
					{
						//m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);

						m_viewSettings->setActiveCelIndex(-1);
						m_layerController->setCelOffset(m_celLid, frameOffset);

						//m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
					}
				}
				else if (m_mouseMoveType == FrameWidget::kFrameCreating)
				{
					if (m_stateMachine->isIdleState())
					{
						//
						/*
						m_creatorDialog->setSingleFrameOptionsVisible(true);
						m_creatorDialog->setMultiFrameOptionsVisible(true);

						if (!m_layerController->anyAvailableSheet())
						{
							m_creatorDialog->disableDuplicate(true);
							m_creatorDialog->disableLoop(true);
							m_creatorDialog->disablePingpong(true);
							m_creatorDialog->disableReverse(true);
						}
						else
						{
							m_creatorDialog->disableDuplicate(false);
							m_creatorDialog->disableLoop(false);
							m_creatorDialog->disablePingpong(false);
							m_creatorDialog->disableReverse(false);
						}

						if (m_selector->activeFrame() < 0 || m_selector->activeFrame() >= static_cast<int>(m_parentCel->sheetCount()))
							m_creatorDialog->disableDuplicate(true);

						if (m_clickRegion == (int)FrameWidget::kSheetRight)
							m_creatorDialog->disableExtend(false);
						else
						{
							if (m_startFrame <= 0)
								m_creatorDialog->disableExtend(true);
							else
								m_creatorDialog->disableExtend(false);
						}
						

						QDesktopWidget service;
						QRect screen = service.screenGeometry(service.screenNumber(this));
						QSize dialogSizeHint = m_creatorDialog->sizeHint();
						QPoint movingPt = screen.center() - QPoint(dialogSizeHint.width() / 2, dialogSizeHint.height() / 2);
						m_creatorDialog->move(movingPt);
						m_creatorDialog->activateWindow();
						int ret = m_creatorDialog->exec();

						if (ret)
						{
							LayerId lid;
							lid = m_selector->findLayerId(m_clickedLayer, m_parentCel);
							int index;
							int startFrame = (m_clickRegion == (int)FrameWidget::kSheetLeft) ? m_startFrame : m_startFrame + 1;
							m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);
							switch (m_creatorDialog->frameActionType())
							{
							case CreateFrameDialog::kDuplicate:
								index = lid.index();
								lid.setIndent(1);
								m_layerController
									->dupFrameIntoSeq(lid, index, startFrame, m_numberOfFrame);
								break;

							case CreateFrameDialog::kExtend:
								index = lid.index();
								lid.setIndent(1);
								m_layerController
									->insertRepeatFrames(lid, index, startFrame, m_numberOfFrame);
								break;

							case CreateFrameDialog::kBlankKeyFrames:
								index = lid.index();
								lid.setIndent(1);
								m_layerController
									->insertKeyFrames(lid, index, startFrame, m_numberOfFrame);
								break;

							case CreateFrameDialog::kBlankTweenFrames:
								break;

							case CreateFrameDialog::kLoopSequence:
								if (m_creatorDialog->getLoopValue() > 0)
								{
									index = lid.index();
									lid.setIndent(1);
									m_layerController
										->dupSeqToLoop(lid, index, startFrame, m_creatorDialog->getLoopValue(), false, false);
								}
								break;

							case CreateFrameDialog::kReverseSequence:
								if (m_creatorDialog->getLoopValue() > 0)
								{
									index = lid.index();
									lid.setIndent(1);
									m_layerController
										->dupSeqToPingRev(lid, index, startFrame, m_creatorDialog->getLoopValue(), false, false);
								}
								break;

							case CreateFrameDialog::kPingpongSequence:
								if (m_creatorDialog->getLoopValue() > 0)
								{
									index = lid.index();
									lid.setIndent(1);
									m_layerController
										->dupSeqToPingPong(lid, index, startFrame, m_creatorDialog->getLoopValue(), false, false);

								}
								break;

							case CreateFrameDialog::kRepeatFrames:
								break;

							case CreateFrameDialog::kNullFrames:
								break;
							}
							m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
						}
						*/
					}
				}
				else if (m_mouseMoveType == FrameWidget::kFrameRemoving)
				{
					int frameOffset = m_parentCel->frameOffset();
					if (m_numberOfFrame> 0)
					{
						QMessageBox msgBox(QMessageBox::Question, tr("Delete Frames"), tr("Frames will be deleted, are you sure?"),
							QMessageBox::Ok | QMessageBox::Cancel, this);

						QPalette mainPalette = this->palette();
						mainPalette.setColor(QPalette::WindowText, QColor(0, 0, 0));
						msgBox.setPalette(mainPalette);
						int ret = msgBox.exec();

						if (ret == QMessageBox::Ok)
						{
							
							int maxFrames = m_layerController->layerSelector()->activeLayer()->GetNumFrame();
							int activeFrame = m_selector->activeFrame();
					
							int startingFrame;
							LayerId lid;
							lid = m_selector->findLayerId(m_clickedLayer, m_parentCel);
							int index = lid.index();
							lid.setIndent(1);
							//m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);
							if (m_clickRegion == (int)FrameWidget::kSheetLeft)
							{
								startingFrame = m_startFrame - m_numberOfFrame;
								startingFrame = max(0, startingFrame);
								//yy added - fix bug of deleting away last frame while showing it
								if (activeFrame > (maxFrames - 1) - m_numberOfFrame)
									m_selector->setActiveFrame((maxFrames - 1) - m_numberOfFrame);
								m_layerController->removeFrameSeq(lid, index, startingFrame, m_numberOfFrame);
							}
							else if (m_clickRegion == (int)FrameWidget::kSheetRight)
							{
								startingFrame = m_startFrame - m_numberOfFrame + 1;
								startingFrame = max(0, startingFrame);
								//yy added - fix bug of deleting away last frame while showing it
								if (activeFrame > (maxFrames - 1) - m_numberOfFrame)
									m_selector->setActiveFrame((maxFrames - 1) - m_numberOfFrame);
								m_layerController->removeFrameSeq(lid, index, startingFrame, m_numberOfFrame);
							}
							//m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
						}
					}
				}

				
				m_manhattan = false;

				if (m_rubberBandSelection)
				{
					//m_selector->notifyActiveSheetChange();
					m_rubberBandSelection = false;
				}
			}

			//GAOHE: For updating canvas under simple single click on frame
			
			//m_selector->setActiveLayer(m_selector->findLayerId(m_clickedLayer));

			m_mouseMoveType = kNoMove;
			if (e->button() != Qt::RightButton)
			{
				m_mouseClickType = FrameWidget::kNoClick;
				m_clickRegion = (int)FrameWidget::kSheetNone;
			}

			if (m_scrollTimer->isActive())
				m_scrollTimer->stop();

			update();
		}

		void FrameWidget::mouseDblClkNorm(QMouseEvent* e)
		{
			if (e->button() == Qt::LeftButton && m_stateMachine->isIdleState())
			{
				int y = e->y() + m_scrollValueY;
				const LayerGroup* selectedParent = 0;
				const Layer* selectedLayer = 0;
				findLayer(y, selectedLayer, selectedParent);

				if (selectedParent && selectedParent->isExpanded())
				{
					int frameOffset = selectedParent->frameOffset();
					int x = e->x() - SHEET_BORDER / 2 + m_scrollValueX;
					int activeFrame = x / (m_gridWidth + SHEET_BORDER) - frameOffset;
					
					//
					/*
					m_creatorDialog->setSingleFrameOptionsVisible(true);
					m_creatorDialog->setMultiFrameOptionsVisible(false);

					if (activeFrame >= static_cast<int>(selectedParent->sheetCount()))
					{
						m_creatorDialog->disableDuplicate(true);
						m_creatorDialog->disableExtend(false);
						m_creatorDialog->disableLoop(true);
						m_creatorDialog->disablePingpong(true);
						m_creatorDialog->disableReverse(true);

						QDesktopWidget service;
						QRect screen = service.screenGeometry(service.screenNumber(this));
						QSize dialogSizeHint = m_creatorDialog->sizeHint();
						QPoint movingPt = screen.center() - QPoint(dialogSizeHint.width() / 2, dialogSizeHint.height() / 2);
						m_creatorDialog->move(movingPt);
						m_creatorDialog->activateWindow();
						int ret = m_creatorDialog->exec();
						
						if (ret)
						{
							m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);

							if (m_creatorDialog->frameActionType() == CreateFrameDialog::kBlankKeyFrames)
							{
								m_layerController
									->appendKeyFrameToActiveCel(activeFrame - static_cast<int>(selectedParent->sheetCount()) + 1);
							}
							else if (m_creatorDialog->frameActionType() == CreateFrameDialog::kBlankTweenFrames)
							{
							}
							else if (m_creatorDialog->frameActionType() == CreateFrameDialog::kExtend)
							{
								m_layerController
									->appendRepeatFrameToActiveCel(activeFrame - static_cast<int>(selectedParent->sheetCount()) + 1);
							}
							else if (m_creatorDialog->frameActionType() == CreateFrameDialog::kNullFrames)
							{
							}

							m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
						}
						
					}
					else if (activeFrame < 0 && activeFrame >= -frameOffset)
					{
						m_creatorDialog->disableDuplicate(true);
						m_creatorDialog->disableExtend(true);
						m_creatorDialog->disableLoop(true);
						m_creatorDialog->disablePingpong(true);
						m_creatorDialog->disableReverse(true);

						QDesktopWidget service;
						QRect screen = service.screenGeometry(service.screenNumber(this));
						QSize dialogSizeHint = m_creatorDialog->sizeHint();
						QPoint movingPt = screen.center() - QPoint(dialogSizeHint.width() / 2, dialogSizeHint.height() / 2);
						m_creatorDialog->move(movingPt);
						m_creatorDialog->activateWindow();
						int ret = m_creatorDialog->exec();
						if (ret)
						{
							m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);

							if (m_creatorDialog->frameActionType() == CreateFrameDialog::kBlankKeyFrames)
							{
								m_layerController
									->prependKeyFrameToActiveCel(abs(activeFrame));
							}
							else
							{

							}

							m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
						}
					}
					*/
				}
				else if(selectedParent && !selectedParent->isExpanded() && selectedLayer == selectedParent)
				{
					//m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);
				m_layerController->setLayerExpanded(m_celLid);
				//m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
				}
			}
		}

		void FrameWidget::getLayerColors(QColor& keySelColor, QColor& keyUnselColor, QColor& tweenSelColor, QColor& tweenUnselColor, const Layer* l)
		{
			if (l->layerType() == ABS_DRAWING_LAYER)
			{
				keySelColor = DRAW_LAYER_KEY_S;
				keyUnselColor = DRAW_LAYER_KEY_U;
				tweenSelColor = DRAW_LAYER_TWEEN_S;
				tweenUnselColor = DRAW_LAYER_TWEEN_U;
			}
		}

		void FrameWidget::findLayer(int y, const Layer*& selectedLayer, const LayerGroup*& selectedParent)
		{
			int accumHeight = 0;
			for (size_t i = 0; i<m_layerController->getCurrentRoot()->memberCount(); ++i)
			{
				const LayerGroup* cel = dynamic_cast<const LayerGroup*>(m_layerController->getCurrentRoot()->childAtIndex(i));

				int rectHeight = (SHEET_BORDER * 2) + (cel->isExpanded() ? (static_cast<int>(cel->memberCount()) + 1) : 1)*SHEET_HEIGHT;
				if (cel->isExpanded())
					rectHeight += static_cast<int>(cel->memberCount())*TIMELINE_NORM_TOP_BUFFER;

				//Hit test check for cel
				if (y >= accumHeight && y <= rectHeight + accumHeight)
				{
					int levely = y - accumHeight;
					int top = SHEET_BORDER + SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER / 2;
					int bottom = rectHeight - SHEET_BORDER - SHEET_HEIGHT - TIMELINE_NORM_TOP_BUFFER / 2;
					if (cel->isExpanded())
					{
						if (levely >= 0 && levely <= top)
						{
							selectedLayer = cel;
							selectedParent = cel;
							break;
						}
						else if (levely <= rectHeight && levely >= bottom)
						{
							if (cel->memberCount() == 0)
							{
								selectedLayer = cel;
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
						else if (cel->memberCount() >= 2)
						{
							int layerY = (levely - top) / (SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER);
							//GAOHE modified:
							//originally it always select the last layer
							//selectedLayer = cel->childAtIndex(cel->memberCount() - 1);
							selectedLayer = cel->childAtIndex(layerY);
							selectedParent = cel;
							break;
						}
					}
					else
					{
						selectedLayer = cel;
						selectedParent = cel;
					}
				}

				if (selectedLayer && selectedParent)
				{
					break;
				}

				accumHeight += rectHeight + TIMELINE_INTER_CEL;
			}
		}

		void FrameWidget::setMouseCursor(QMouseEvent* e)
		{
			if (m_panToolIn)
			{
				setCursor(Qt::OpenHandCursor);
			}
			else
			{
				const Layer* selectedLayer = 0;
				const LayerGroup* selectedParent = 0;
				int y = e->y() + m_scrollValueY;
				findLayer(y, selectedLayer, selectedParent);
				int x = e->x() - SHEET_BORDER / 2 + m_scrollValueX;


				int frameOffset = 0;
				if (selectedParent && selectedLayer)
				{
					frameOffset = selectedParent->frameOffset();

					int activeFrame = x / (m_gridWidth + SHEET_BORDER) - frameOffset;
					m_startFrame = activeFrame;
					int leftOver = x % (m_gridWidth + SHEET_BORDER);

					if (selectedLayer == selectedParent)
					{
						setCursor(Qt::ArrowCursor);
						return;
					}
					else if (selectedLayer->sheetCount() > 0)
					{
						if (selectedLayer && activeFrame < static_cast<int>(selectedLayer->sheetCount()) && activeFrame >= 0)
						{
							if (leftOver >= 0 && leftOver <= SHEET_BORDER / 2 + SHEET_INNER)
							{
								if (activeFrame > 0)
									setCursor(Qt::SplitHCursor);
								else
									setCursor(Qt::ArrowCursor);
								return;
							}
							else if (leftOver >= m_gridWidth - SHEET_INNER + SHEET_BORDER / 2 && leftOver <= m_gridWidth + SHEET_BORDER / 2)
							{
								setCursor(Qt::SplitHCursor);
								return;
							}
							else
							{
								setCursor(Qt::ArrowCursor);
								return;
							}
						}
						/*else if(selectedLayer && activeFrame == -1 && frameOffset>0)
						{
						if(leftOver >= m_gridWidth-SHEET_INNER + SHEET_BORDER/2 && leftOver <= m_gridWidth+SHEET_BORDER/2)
						{
						setCursor(Qt::SplitHCursor);
						return;
						}
						else
						{
						setCursor(Qt::ArrowCursor);
						return;
						}
						}*/
						else
						{
							setCursor(Qt::ArrowCursor);
							return;
						}
					}
					else
						setCursor(Qt::ArrowCursor);
				}
				else
					setCursor(Qt::ArrowCursor);
			}

		}


		void FrameWidget::addBandsofFrames(int startX, int endX)
		{
			int startFrame = startX / (m_gridWidth + SHEET_BORDER);
			int endFrame = endX / (m_gridWidth + SHEET_BORDER);
			int activeFrame = m_selector->activeSheetId().fid();

			for (int i = startFrame; i <= endFrame; ++i)
			{
				if (i >= 0)
					m_selector->addSelectedFrame(i);
			}
		}

		void FrameWidget::addBandsofLayers(int startX, int endX, int starty, int endy)
		{
			int accumHeight = 0;
			bool first = false;
			LayerId lid(0);
			lid.addIndex(0);
			lid.addIndex(0);
			lid.setIndent(2);

			int startFrame = startX / (m_gridWidth + SHEET_BORDER);
			int endFrame = endX / (m_gridWidth + SHEET_BORDER);
			int activeFrame = m_selector->activeSheetId().fid();



			//if (activeFrame < startFrame || activeFrame > endFrame);
			//	activeFrame = startFrame;

				
			//YY: Allow animation on both left and right mouse movements
			if (activeFrame != startFrame || activeFrame != endFrame) {
				if (startFrame < activeFrame) {
					if (startFrame >= 0)
						activeFrame = startFrame;
				}
				else if (endFrame > activeFrame) {
					int maxFrames = m_layerController->layerSelector()->activeLayer()->GetNumFrame();
					if (endFrame <= maxFrames-1) {
						activeFrame = endFrame;
					}
				}

				//GAOHE added: animation effect when dragging
				m_selector->setActiveFrame(activeFrame);
			}

			unsigned int i = 0;
			for (unsigned int i = 0; i<static_cast<unsigned int>(m_layerController->getCurrentRoot()->memberCount()); ++i)
			{
				const LayerGroup* cel = dynamic_cast<const LayerGroup*>(m_layerController->getCurrentRoot()->childAtIndex(i));
				lid.setIndent(1);
				lid.setIndex(i);

				int rectHeight = (SHEET_BORDER * 2) + (cel->isExpanded() ? (static_cast<int>(cel->memberCount()) + 1) : 1)*SHEET_HEIGHT;
				if (cel->isExpanded())
					rectHeight += static_cast<int>(cel->memberCount())*TIMELINE_NORM_TOP_BUFFER;

				if (rangeIntersects(starty, endy, accumHeight, rectHeight + accumHeight))
				{
					int starty2 = starty - accumHeight;
					int endy2 = endy - accumHeight;
					int top = SHEET_BORDER + SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER / 2;
					int bottom = rectHeight - SHEET_BORDER - SHEET_HEIGHT - TIMELINE_NORM_TOP_BUFFER / 2;

					if (rangeIntersects(starty2, endy2, 0, top))
					{
						if (!first)
						{
							m_selector->addFrameSelection(lid, activeFrame);
							first = true;
						}
						else
						{
							if (!m_selector->layerIsSelected(lid))
								m_selector->addSelectedLayer(lid);
						}
					}

					if (cel->isExpanded())
					{
						for (unsigned int j = 0; j<static_cast<int>(cel->memberCount()); ++j)
						{
							lid.setIndent(2);
							lid.setIndex(j);
							int sheetTop = top + j*(SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER);
							int sheetBottom = top + (j + 1)*(SHEET_HEIGHT + TIMELINE_NORM_TOP_BUFFER);
							if (rangeIntersects(starty2, endy2, sheetTop, sheetBottom))
							{
								const Layer* l = cel->childAtIndex(j);

								if (!first)
								{
										if (activeFrame >= startFrame && activeFrame <= endFrame)
											m_selector->addFrameSelection(lid, activeFrame);
										else
										m_selector->addFrameSelection(lid, startFrame);

									first = true;
								}
								else
								{
									if (!m_selector->layerIsSelected(lid))
										m_selector->addSelectedLayer(lid);
								}
							}
						}
					}
				}

				accumHeight += rectHeight + TIMELINE_INTER_CEL;
			}
		}

		bool FrameWidget::rangeIntersects(int range1Top, int range1Bot, int range2Top, int range2Bot)
		{
			if (range1Top <= range2Top && range1Bot <= range2Bot && range1Bot >= range2Top)
				return true;

			if (range1Top >= range2Top && range1Top <= range2Bot && range1Bot >= range2Bot)
				return true;

			if (range1Top >= range2Top && range1Bot <= range2Bot)
				return true;

			if (range1Top <= range2Top && range1Bot >= range2Bot)
				return true;

			return false;
		}

		void FrameWidget::showDuplicateDialog()
		{
			/*
			m_creatorDialog->disableLoop(false);
			m_creatorDialog->disablePingpong(false);
			m_creatorDialog->disableReverse(false);
			m_creatorDialog->setSingleFrameOptionsVisible(false);
			m_creatorDialog->setMultiFrameOptionsVisible(true);
			int ret = m_creatorDialog->exec();

			if (ret)
			{
				m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);

				LayerId lid;
				lid = m_selector->findLayerId(m_clickedLayer, m_parentCel);
				int index;
				int startFrame = (m_clickRegion == (int)FrameWidget::kSheetLeft) ? m_startFrame : m_startFrame + 1;
				m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);
				switch (m_creatorDialog->frameActionType())
				{
				case CreateFrameDialog::kLoopSequence:
					if (m_creatorDialog->getLoopValue() > 0)
					{
						index = lid.index();
						lid.setIndent(1);
						m_layerController
							->dupSeqToLoop(lid, index, startFrame, m_creatorDialog->getLoopValue(), false, false);
					}
					break;

				case CreateFrameDialog::kReverseSequence:
					if (m_creatorDialog->getLoopValue() > 0)
					{
						index = lid.index();
						lid.setIndent(1);
						m_layerController
							->dupSeqToPingRev(lid, index, startFrame, m_creatorDialog->getLoopValue(), false, false);
					}
					break;

				case CreateFrameDialog::kPingpongSequence:
					if (m_creatorDialog->getLoopValue() > 0)
					{
						index = lid.index();
						lid.setIndent(1);
						m_layerController
							->dupSeqToPingPong(lid, index, startFrame, m_creatorDialog->getLoopValue(), false, false);
					}
					break;
				}
				m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
			}
			*/
		}

		void FrameWidget::removeSelectedFrames()
		{
			QMessageBox msgBox(QMessageBox::Question, tr("Delete selected frames?"),
				tr("Delete selected frames?"), QMessageBox::Ok | QMessageBox::Cancel);
			QPalette currentPalette = msgBox.palette();
			//currentPalette.setColor(QPalette::WindowText, DockColors::instance().statusBarTextColor());
			msgBox.setPalette(currentPalette);

			int ret = msgBox.exec();
			if (ret != QMessageBox::Ok)
				return;

			/*
			if (m_stateMachine->setLayerState(cacani::state::kClearSelectedElements))
			{
				m_layerController->removeSelectedFrames();

				m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
			}
			*/
		}

		
	}
}