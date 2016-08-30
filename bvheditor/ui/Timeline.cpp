#include <QPen>  //
#include <QPainter>  //
#include <QMouseEvent>  //
#include <QApplication>  //

#include "Timeline.h"
#include "FrameCreatorDefs.h"
#include "../controllers/LayerSelector.h"	//#include "DataController/AbstractLayerSelector.h"
#include "../controllers/CACStateMachine.h"
#include "../QsLog2/qslog.h"

using namespace cacani::controller;


namespace cacani {


	namespace ui {

		Timeline::Timeline(LayerSelector* selector, CACStateMachine* sm, QWidget* parent)
			:m_stateMachine(sm), m_selector(selector), QWidget(parent)
		{
			QLOG_INFO() << "Timeline:" << "Started";

			QPalette pal(palette());
			pal.setColor(QPalette::Background, TIMELINE_BKGN);
			setAutoFillBackground(true);
			setPalette(pal);

			m_leftTriangle << QPoint(0, 0) << QPoint(-2, SHEET_HEIGHT / 2) << QPoint(0, SHEET_HEIGHT);
			m_rightTriangle << QPoint(0, 0) << QPoint(2, SHEET_HEIGHT / 2) << QPoint(0, SHEET_HEIGHT);

			m_leftBrace << QPoint(0, 0) << QPoint(0, 11) << QPoint(2, 11) << QPoint(2, 4) << QPoint(6, 0);
			m_rightBrace << QPoint(0, 0) << QPoint(0, 11) << QPoint(-2, 11) << QPoint(-2, 4) << QPoint(-6, 0);

			setMouseTracking(true);

			m_moveType = Timeline::kNone;
			m_mouseStateMode = Timeline::kIdleMode;
			m_manhattan = false;
			m_scrollPosX = 0;

			m_gridWidth = SHEET_WIDTH;
			QLOG_INFO() << "Timeline:" << "construction completed";

		}

		Timeline::~Timeline()
		{
		}

		void Timeline::setSmallGrid(bool smallMode)
		{
			if (smallMode)
				m_gridWidth = SHEET_SMALL_WIDTH;
			else
				m_gridWidth = SHEET_WIDTH;

			update();
		}

		QSize Timeline::sizeHint() const
		{
			return QSize(400, TIME_BAR_HEIGHT);
		}

		QSize Timeline::minimumSizeHint() const
		{
			return QSize(100, TIME_BAR_HEIGHT);
		}

		QSize Timeline::maximumSizeHint() const
		{
			return QSize(QWIDGETSIZE_MAX, TIME_BAR_HEIGHT);
		}

		void Timeline::setScrollPosX(int pos)
		{
			m_scrollPosX = pos;
			update();
		}

		void Timeline::paintEvent(QPaintEvent* e)
		{
			QPainter p(this);

			//Painting the off pegs
			int startFrame = m_scrollPosX / (SHEET_BORDER + m_gridWidth);
			p.translate(-m_scrollPosX, 0);
			p.translate(startFrame*(SHEET_BORDER + m_gridWidth), 0);
			for (int i = startFrame; i <= startFrame + width() / (SHEET_BORDER + m_gridWidth) + 1; ++i)
			{
				QRect txtRect(SHEET_BORDER, rect().height() - SHEET_HEIGHT, m_gridWidth, SHEET_HEIGHT);

				p.setPen(QPen(TIMELINE_TEXTCOLOR, 1));
				p.drawText(txtRect, Qt::AlignVCenter | Qt::AlignHCenter, QString::number(i + 1));

				QRect orangeEllipse;
				orangeEllipse = QRect(txtRect.center().x() - LIGHTBOX_SMALL_GRAY / 2, LIGHTBOX_LARGE_GRAY / 2 - 1 + TIMELINE_TOP_BUFFER, LIGHTBOX_SMALL_GRAY, LIGHTBOX_SMALL_GRAY);
				p.setPen(QPen(TIMELINE_LIGHTBOX_SMALL_GRAY, 1));
				p.setBrush(Qt::NoBrush);
				p.drawEllipse(orangeEllipse);
				p.translate(SHEET_BORDER + m_gridWidth, 0);
			}

			p.resetTransform();

			int activeFrame = m_selector->absoluteActiveFrame();
			QPoint activePos(activeFrame*(SHEET_BORDER + m_gridWidth) + SHEET_BORDER, rect().height() - SHEET_HEIGHT);
			if (rect().adjusted(m_scrollPosX, 0, m_scrollPosX, 0).contains(activePos))
			{
				p.setPen(QPen(TIMELINE_LIGHTBOX_ORAN, 2));
				p.setBrush(TIMELINE_LIGHTBOX_DULL_ORAN);
				QRect activeRect(activePos.x(), activePos.y(), m_gridWidth, SHEET_HEIGHT);
				activeRect.adjust(0, 1, 1, -1);
				p.drawRect(activeRect);
			}
			p.resetTransform();
		}

		void Timeline::mousePressEvent(QMouseEvent* e)
		{
			if (e->button() == Qt::LeftButton)
			{
				int activeFrame = m_selector->absoluteActiveFrame();
				QRect activeRect(activeFrame*(SHEET_BORDER + m_gridWidth) + SHEET_BORDER, rect().height() - SHEET_HEIGHT,
					m_gridWidth, SHEET_HEIGHT);
				QRect leftDragRect(activeRect.left() - SHEET_BORDER / 2 - 1, activeRect.top(), SHEET_BORDER + SHEET_INNER, activeRect.height());
				QRect rightDragRect(activeRect.right() - SHEET_INNER, activeRect.top(), SHEET_BORDER + SHEET_INNER, activeRect.height());
				int leftRange, rightRange;


				m_initialPoint = QPoint(e->x() + m_scrollPosX, e->y());
				if (m_stateMachine->isIdleState())
				{
					if (m_stateMachine->isIdleState())
						m_mouseStateMode = Timeline::kIdleMode;

					if (leftDragRect.contains(m_initialPoint) && m_stateMachine->isIdleState())
					{
						m_moveType = Timeline::kLightBoxLeft;
					}
					else if (rightDragRect.contains(m_initialPoint) && m_stateMachine->isIdleState())
					{
						m_moveType = Timeline::kLightBoxRight;
					}
					else if (activeRect.contains(m_initialPoint) && m_stateMachine->isIdleState())
					{
						m_moveType = Timeline::kFrameMoving;
					}
					else
					{
						if (m_initialPoint.y() >= rect().height() - SHEET_HEIGHT && m_initialPoint.y() <= rect().bottom())
						{
							m_moveType = Timeline::kFrameMoving;
							int frameNumber = (e->x() - SHEET_BORDER + m_scrollPosX) / (SHEET_BORDER + m_gridWidth);
							m_selector->setActiveFrame(frameNumber);

							////m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
						}
						else
							m_moveType = Timeline::kSingleClick;
					}
				}
			}
		}

		void Timeline::mouseMoveEvent(QMouseEvent* e)
		{
			QPoint currentPoint(e->x() + m_scrollPosX, e->y());

			if (e->buttons() == Qt::NoButton)
			{
				int activeFrame = m_selector->absoluteActiveFrame();
				QRect activeRect(activeFrame*(SHEET_BORDER + m_gridWidth) + SHEET_BORDER, rect().height() - SHEET_HEIGHT,
					m_gridWidth, SHEET_HEIGHT);
				QRect leftDragRect(activeRect.left() - SHEET_BORDER / 2 - 1, activeRect.top(), SHEET_BORDER + SHEET_INNER, activeRect.height());
				QRect rightDragRect(activeRect.right() - SHEET_INNER, activeRect.top(), SHEET_BORDER + SHEET_INNER, activeRect.height());

				if (leftDragRect.contains(currentPoint))
				{
					setCursor(Qt::SizeHorCursor);
				}
				else if (rightDragRect.contains(currentPoint))
				{
					setCursor(Qt::SizeHorCursor);
				}
				else
				{
					setCursor(Qt::ArrowCursor);
				}
			}

			if (m_moveType != Timeline::kNone && !m_manhattan)
			{
				if ((currentPoint - m_initialPoint).manhattanLength() >= QApplication::startDragDistance())
					m_manhattan = true;
			}

			if (m_moveType != Timeline::kNone && m_manhattan)
			{
				int rawframeNumber = (currentPoint.x() - SHEET_BORDER) / (SHEET_BORDER + m_gridWidth);
				int frameNumber = std::max(0, rawframeNumber);  //int frameNumber = max(0, rawframeNumber); 

				if (m_mouseStateMode == Timeline::kIdleMode && m_stateMachine->isIdleState())
				{
					if (m_moveType == Timeline::kFrameMoving)
					{
						if (m_selector->absoluteActiveFrame() != frameNumber)
						{
							////m_stateMachine->setLayerState(cacani::state::kClearSelectedElements);
							m_selector->setActiveFrame(frameNumber);
							////m_stateMachine->setIdleState(cacani::state::kCheckSheettype);
						}
					}
				}
			}
		}

		void Timeline::mouseReleaseEvent(QMouseEvent* e)
		{
			if (m_moveType != Timeline::kNone && m_manhattan)
			{
				m_moveType = Timeline::kNone;
				m_mouseStateMode = Timeline::kNoneMode;
				m_manhattan = false;
			}
			else
			{
				m_moveType = Timeline::kNone;
				m_mouseStateMode = Timeline::kNoneMode;
			}
		}

	}
}