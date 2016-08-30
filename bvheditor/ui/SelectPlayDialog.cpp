#include "SelectPlayDialog.h"

namespace cacani{
	
	namespace ui{

		SelectPlayDialog::SelectPlayDialog(SheetCanvas* sheetCanvas) :
			m_sheetCanvas(sheetCanvas),
			m_Button_Play(new QPushButton("Play")),
			m_Button_Cancel(new QPushButton("Cancel")),
			m_firstFrameList(new QComboBox()),
			m_lastFrameList(new QComboBox())
		{

			//Setup link for the 2 buttons
			connect(m_Button_Play, SIGNAL(clicked(bool)), this, SLOT(playClicked()));
			connect(m_Button_Cancel, SIGNAL(clicked(bool)), this, SLOT(cancelClicked()));
			
			//Setup link for first frame combobox to affect the last frame combobox
			connect(m_firstFrameList, SIGNAL(currentIndexChanged(int)), this, SLOT(populateLastFrameList(int)));

			//First Frame Input
			QLabel* first = new QLabel("Select first frame:");
			//QLineEdit* firstLine = new QLineEdit;
			int currentNumFrames = m_sheetCanvas->getFramesFromCurrentLayer();

			for (int i = 0; i < (currentNumFrames - 1); i++) {
				m_firstFrameList->addItem("Frame " + QString::number(i + 1));
			}


			//Last Frame Input
			QLabel* last = new QLabel("Select last frame:");
			//QLineEdit* lastLine = new QLineEdit;

			//adding the 2 rows into a widget
			QFormLayout* inputLayout = new QFormLayout;
			inputLayout->addRow(first, m_firstFrameList);
			inputLayout->addRow(last, m_lastFrameList);
			QWidget* inputWidget = new QWidget;
			inputWidget->setLayout(inputLayout);

			//adding the buttons into a widget			
			QHBoxLayout* buttonLayout = new QHBoxLayout;
			buttonLayout->addWidget(m_Button_Play);
			buttonLayout->addWidget(m_Button_Cancel);
			QWidget* buttonWidget = new QWidget;
			buttonWidget->setLayout(buttonLayout);

			//Combining both Widgets
			QVBoxLayout* dialogLayout = new QVBoxLayout;
			dialogLayout->addWidget(inputWidget);
			dialogLayout->addWidget(buttonWidget);

			
			
			setLayout(dialogLayout);
			setWindowTitle("Playing Selected Frames");
			resize(500, 300);
			
		}


		SelectPlayDialog::~SelectPlayDialog() {}

		void SelectPlayDialog::playClicked() {
			//Stop any ongoing animation first
			m_sheetCanvas->stopAnimation();

			//calculate the first and last frame window
			int firstFrame = m_firstFrameList->currentIndex();
			int lastFrame = 1 + firstFrame + m_lastFrameList->currentIndex();

			//play selected frames
			m_sheetCanvas->setFirstFrame(firstFrame);
			m_sheetCanvas->setLastFrame(lastFrame);
			m_sheetCanvas->animateSelected();
			close();
		}

		void SelectPlayDialog::cancelClicked() {
			close();
		}

		void SelectPlayDialog::populateLastFrameList(int firstFrame) {
			m_lastFrameList->clear();
			for (int i = (firstFrame + 1); i < m_sheetCanvas->getFramesFromCurrentLayer(); i++) {
				m_lastFrameList->addItem("Frame " + QString::number(i + 1));
			}
		}


	}
}