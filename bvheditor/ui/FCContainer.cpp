#include <qwidget.h> 
#include <qsplitter.h> 
#include <qscrollbar.h> 
#include <QHBoxLayout>  
#include <QMouseEvent>

#include "FCContainer.h"
#include "FrameCreatorDefs.h"
#include "../controllers/LayerSelector.h" 
#include "../data/utf8string.h"
#include "../QsLog2/qslog.h"


namespace cacani {
	namespace ui {

		FCContainer::FCContainer(const LayerGroup* data, ViewSettings* viewSettings, LayerController* layerController, LayerSelector* selector, CACStateMachine* sm, QWidget* parent)
			:QSplitter(parent), m_selector(selector), m_stateMachine(sm), m_layerController(layerController)
		{
			using namespace QsLogging;

			QLOG_INFO() << "FCContainer:" << "Started";
			connect(m_layerController, SIGNAL(updateCananiUI()), this, SLOT(adjustScroll()));
			connect(m_layerController, SIGNAL(updateCananiUI()), this, SLOT(updateDrawing()));
			

			//initialize left hand side layer widget
			m_leftContainer = new QWidget(this);

			//m_topWidget = new LayerTopWidget(m_leftContainer);
			//m_topWidget->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
			
			
			m_layerWidget = new LayerCreatorWidget(data, layerController, selector, sm, m_leftContainer);
			connect(m_layerWidget, SIGNAL(sliderBarTop(int)), this, SLOT(scrollToChangeTop(int)));
			connect(m_layerWidget, SIGNAL(sliderBarBottom(int)), this, SLOT(scrollToChangeBottom(int)));
			connect(m_layerWidget, SIGNAL(audioLayerVolumeChanged(int, double)), this, SIGNAL(audioLayerVolumeChanged(int, double)));
			connect(m_layerWidget, SIGNAL(audioLayerMuted(int, bool)), this, SIGNAL(audioLayerMuted(int, bool)));

			//initialize the scroll bar for layer widget 
			m_vertSB = new QScrollBar(Qt::Vertical, m_leftContainer);
			m_vertSB->setFixedWidth(LAYER_VSCROLL_WIDTH);
			connect(m_vertSB, SIGNAL(valueChanged(int)), this, SLOT(setScrollForLayer(int)));

			//put layer widget and the scroll bar into timeline ui
			QHBoxLayout* m_leftHorzLayout = new QHBoxLayout;
			m_leftHorzLayout->setSpacing(0);
			m_leftHorzLayout->setContentsMargins(0, 0, 0, 0);
			m_leftHorzLayout->addWidget(m_layerWidget);
			m_leftHorzLayout->addWidget(m_vertSB);

			QVBoxLayout* m_leftLayout = new QVBoxLayout(m_leftContainer);
			m_leftLayout->setSpacing(0);
			m_leftLayout->setContentsMargins(0, 0, 0, 0);
			//m_leftLayout->addWidget(m_topWidget);
			m_leftLayout->addLayout(m_leftHorzLayout);

			m_rightContainer = new QWidget(this);

			//set up timeline
			m_timeline = new Timeline(selector, sm, m_rightContainer);
			m_timeline->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));

			//set up framewidget
			m_frameWidget = new FrameWidget((ViewSettings*)viewSettings, layerController, selector, sm, m_rightContainer);

			//m_frameWidget = new FrameWidget(data, (ViewSettings*)viewSettings, layerController, selector, sm, m_rightContainer);
			connect(m_frameWidget, SIGNAL(sliderBarLeft(int)), this, SLOT(scrollToChangeLeft(int)));
			connect(m_frameWidget, SIGNAL(sliderBarRight(int)), this, SLOT(scrollToChangeRight(int)));
			connect(m_frameWidget, SIGNAL(sliderBarTop(int)), this, SLOT(scrollToChangeTop(int)));
			connect(m_frameWidget, SIGNAL(sliderBarBottom(int)), this, SLOT(scrollToChangeBottom(int)));
			connect(m_frameWidget, SIGNAL(generateInbetweens()), this, SIGNAL(generateInbetweens()));
			connect(m_frameWidget, SIGNAL(propGroups()), this, SIGNAL(propGroups()));
			connect(m_frameWidget, SIGNAL(replaceGroups()), this, SIGNAL(replaceGroups()));
			connect(m_frameWidget, SIGNAL(smallGrid(bool)), m_timeline, SLOT(setSmallGrid(bool)));
			connect(m_frameWidget, SIGNAL(framePanned(int, int)), this, SLOT(frameWidgetPanned(int, int)));

			m_horzSB = new QScrollBar(Qt::Horizontal, m_rightContainer);
			m_horzSB->setFixedHeight(LAYER_VSCROLL_WIDTH);
			connect(m_horzSB, SIGNAL(valueChanged(int)), this, SLOT(setScrollForFrame(int)));

			//add horizontal scroll bar, timeline and framewidget to ui
			QVBoxLayout* m_rightLayout = new QVBoxLayout(m_rightContainer);
			m_rightLayout->setSpacing(0);
			m_rightLayout->setContentsMargins(0, 0, 0, 0);
			m_rightLayout->addWidget(m_horzSB);
			m_rightLayout->addWidget(m_timeline);
			m_rightLayout->addWidget(m_frameWidget);

			this->insertWidget(0, m_leftContainer);
			this->setCollapsible(0, false);
			this->insertWidget(1, m_rightContainer);
			this->setCollapsible(1, false);

			QList<int> sizes;
			sizes.push_back(m_layerWidget->minimumSizeHint().width());
			sizes.push_back(1000);
			this->setSizes(sizes);

			connect(this, SIGNAL(splitterMoved(int, int)), this, SLOT(adjustScroll()));

			m_showed = false;

			//m_topWidget->updateTitle(getActiveLayerName(), 0);
		}

		FCContainer::~FCContainer()
		{
		}

		void FCContainer::panToolIn(bool in)
		{
			m_frameWidget->panToolIn(in);
		}

	
		void FCContainer::updateUi(int typeCode, int code)
		{
			/*switch (code)
			{
			case OFF_PEG_COLOR_REMOVED:
				m_timeline->update();
				break;

			case LAYERSTRUCTCHANGE:
			case SHEETSTRUCTCHANGE:
			case LAYERSHEETCHANGE:
			case AUDIO_POOL_CHANGED:
			case LAYEREXPANDED:
			case NEWFILE:
			case OPENFILE:
				m_layerWidget->updateMenus();
				m_topWidget->updateTitle(getActiveLayerName(), 0);
				adjustScroll();
				break;

			case AUDIO_PATH_CHANGED:
			case ACTIVELAYERCHANGE:
			case ACTIVESHEETCHANGE:
			case ACTIVELAYERSHEETCHANGE:
			case LAYERVIEWCHANGE:
			case PLAYBACK_FRAMERATE:*/
			//adjustHorzScroll(true);
			/*
				m_frameWidget->update();
				m_topWidget->updateTitle(getActiveLayerName(), 0);
				m_timeline->update();
				m_layerWidget->updateMenus();

			case LAYERLOCKCHANGE:
			case LAYERONIONCHANGE:
			case LAYERCOLORCHANGE:
			case LAYERNAMECHANGE:
			case LAYEROPACITYCHANGED:
			case LAYER_TAB_OPACITY:
				m_layerWidget->update();
				break;

				//case PLAYBACK_PLAY:
			case PLAYBACK_STOP:
				m_timeline->update();
				m_topWidget->updateTitle(getActiveLayerName(), 0);
				m_frameWidget->update();
				break;

			case PLAYBACK_RANGE_CHANGE:
			case PLAYBACK_FRAME_CHANGE:
				adjustHorzScroll();
				m_timeline->update();
				m_topWidget->updateTitle(getActiveLayerName(), 0);
				break;

			case PLAYBACK_FRAME_CHANGE_TB:
			case PLAYBACK_RANGE_CHANGE_TB:
				adjustHorzScroll();
				m_timeline->update();
				m_topWidget->updateTitle(getActiveLayerName(), 0);
				break;

			case LIGHTBOXCHANGE_TB:
			case LIGHTBOXCHANGED:
				m_topWidget->checkShowKeyFrames();
				m_timeline->update();
				break;

			case MAINELEMENTCHANGED:
				m_frameWidget->update();
				break;
			}*/
		}

		void FCContainer::updateScrollView() {
			adjustHorzScroll(true);
		}

		void FCContainer::updateUndoReco()
		{
			m_timeline->update();
			m_frameWidget->update();
			m_layerWidget->updateMenus();
			m_layerWidget->update();
			//m_topWidget->update();
		}

		void FCContainer::updateDrawing()
		{
			m_frameWidget->update();
		}

		void FCContainer::resizeEvent(QResizeEvent* e)
		{
			QSplitter::resizeEvent(e);
			adjustScroll();
		}

		void FCContainer::adjustScroll()
		{
			adjustHorzScroll();

			adjustVertScroll();
		}

		void FCContainer::adjustHorzScroll(bool adjustActive)
		{
			int rangeX = m_frameWidget->calculateTotalWidth();
			m_horzSB->setRange(0, std::max(0, rangeX - m_frameWidget->size().width()));
			m_horzSB->setPageStep(m_frameWidget->size().width());
			m_horzSB->setSingleStep(SHEET_WIDTH + SHEET_BORDER);

			int horzPos = 0;
			if (adjustActive)
			{
				horzPos = m_horzSB->value() + m_frameWidget->calculateActiveScrollRange();
			}
			else
				horzPos = m_horzSB->value();

			m_horzSB->setValue(horzPos);
			m_frameWidget->setScrollPosX(m_horzSB->value());
			m_timeline->setScrollPosX(m_horzSB->value());
		}

		void FCContainer::adjustVertScroll()
		{
			int rangeY = m_layerWidget->calculateTotalHeight();
			m_vertSB->setRange(0, std::max(0, rangeY - m_layerWidget->size().height()));
			m_vertSB->setPageStep(m_layerWidget->size().height());
			m_vertSB->setSingleStep(SHEET_HEIGHT);
			m_layerWidget->setScrollPosY(m_vertSB->value());
			m_frameWidget->setScrollPosY(m_vertSB->value());
		}

		void FCContainer::scrollToChangeLeft(int value)
		{
			m_horzSB->setValue(m_horzSB->value() - value);
			setScrollForFrame(m_horzSB->value());
		}

		void FCContainer::scrollToChangeRight(int value)
		{
			m_horzSB->setValue(m_horzSB->value() + value);
			setScrollForFrame(m_horzSB->value());
		}

		void FCContainer::scrollToChangeTop(int value)
		{
			m_vertSB->setValue(m_vertSB->value() - value);
			setScrollForLayer(m_vertSB->value());
		}

		void FCContainer::scrollToChangeBottom(int value)
		{
			m_vertSB->setValue(m_vertSB->value() + value);
			setScrollForLayer(m_vertSB->value());
		}

		void FCContainer::frameWidgetPanned(int x, int y)
		{
			m_horzSB->setValue(m_horzSB->value() + x);
			m_vertSB->setValue(m_vertSB->value() + y);
			setScrollForLayer(m_vertSB->value());
			setScrollForFrame(m_horzSB->value());
		}

		void FCContainer::setScrollForFrame(int value)
		{
			m_frameWidget->setScrollPosX(value);
			m_timeline->setScrollPosX(m_horzSB->value());
		}

		void FCContainer::setScrollForLayer(int value)
		{
			m_frameWidget->setScrollPosY(value);
			m_layerWidget->setScrollPosY(value);
		}

		void FCContainer::showEvent(QShowEvent* e)
		{
			if (!m_showed)
			{
				adjustScroll();
				m_showed = true;
			}
		}

		std::string FCContainer::getActiveLayerName() const
		{
			return  "Hello World!";//m_selector->activeLayer()->getName().rawData();
		}

		/*void FCContainer::mouseReleaseEvent(QMouseEvent* e)
		{
			m_mouseReleaseEvent();
		}*/

	}
}