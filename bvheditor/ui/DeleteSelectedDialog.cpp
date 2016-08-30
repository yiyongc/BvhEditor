#include "DeleteSelectedDialog.h"

namespace cacani {

	namespace ui{

		DeleteSelectedDialog::DeleteSelectedDialog(SheetCanvas* sheetCanvas, LayerController* layerController, LayerSelector* layerSelector) :
			m_sheetCanvas(sheetCanvas),
			m_layerController(layerController),
			m_layerSelector(layerSelector),
			m_firstFrame(new QComboBox),
			m_lastFrame(new QComboBox),
			m_Button_Confirm(new QPushButton("Confirm")),
			m_Button_Cancel(new QPushButton("Cancel"))
		{
			//Setup link for the 2 buttons
			connect(m_Button_Confirm, SIGNAL(clicked(bool)), this, SLOT(confirmClicked()));
			connect(m_Button_Cancel, SIGNAL(clicked(bool)), this, SLOT(cancelClicked()));

			//Setup link for first combo box to affect second combo box
			connect(m_firstFrame, SIGNAL(currentIndexChanged(int)), this, SLOT(populateLastFrameList(int)));

			//Populate first frame combo box
			for (int i = 0; i < m_sheetCanvas->getFramesFromCurrentLayer(); i++) {
				m_firstFrame->addItem("Frame " + QString::number(i + 1));
			}

			//Frame Selection
			QLabel* first = new QLabel("Select First Frame To Delete:");
			QLabel* last = new QLabel("Select Last Frame To Delete (Inclusive):");
			QFormLayout* inputLayout = new QFormLayout;
			inputLayout->addRow(first, m_firstFrame);
			inputLayout->addRow(last, m_lastFrame);
			QWidget* inputWidget = new QWidget;
			inputWidget->setLayout(inputLayout);

			//Buttons
			QHBoxLayout* buttonLayout = new QHBoxLayout;
			buttonLayout->addWidget(m_Button_Confirm);
			buttonLayout->addWidget(m_Button_Cancel);
			QWidget* buttonWidget = new QWidget;
			buttonWidget->setLayout(buttonLayout);

			//Dialog
			QVBoxLayout* dialogLayout = new QVBoxLayout;
			dialogLayout->addWidget(inputWidget);
			dialogLayout->addWidget(buttonWidget);
			setLayout(dialogLayout);
			setWindowTitle("Delete Selected Frames");
			resize(500, 300);
		}

		DeleteSelectedDialog::~DeleteSelectedDialog() {}

		void DeleteSelectedDialog::confirmClicked() {
			int firstFrame = m_firstFrame->currentIndex() + 1;
			int lastFrame = m_lastFrame->currentIndex() + firstFrame;

			QMessageBox msgBox(QMessageBox::Question, tr("Delete Selected Frames"), "Delete Frame " + QString::number(firstFrame) +
				" to " + QString::number(lastFrame) + "?", QMessageBox::Ok | QMessageBox::Cancel, this);
			QPalette mainPalette = this->palette();
			mainPalette.setColor(QPalette::WindowText, QColor(0, 0, 0));
			msgBox.setPalette(mainPalette);
			int ret = msgBox.exec();

			if (ret == QMessageBox::Ok)	{
				int totalFrames = m_sheetCanvas->getFramesFromCurrentLayer();
				int currentFrame = m_layerSelector->activeFrame();
				int numToDelete = lastFrame - firstFrame + 1;

				if (numToDelete == totalFrames) {
					//add one basic frame before deleting the rest of the frames
					m_layerController->addFrames(0, 1);
					
					//Make added frame show as activeframe
					m_layerSelector->setActiveFrame(0);

					firstFrame++;
				}
				else {
					//Check active frame
					//if current frame will be outside of frame number, reduce it by the number of frames removed
					if (currentFrame > (totalFrames - 1 - numToDelete))
						m_layerSelector->setActiveFrame(currentFrame - numToDelete);
					//if within deletion window, set active frame to first frame
					else if (currentFrame >= firstFrame - 1 && currentFrame <= lastFrame - 1)
						m_layerSelector->setActiveFrame(0);
				}
				//Frame(s) deletion
				m_layerController->removeFrameSeq(m_layerSelector->activeLayerId(), m_layerSelector->activeLayerId().index(), (firstFrame - 1), numToDelete);

				//update view
				m_layerController->updateCananiUI();

				//Update Sliders
				m_layerController->jointEditorWidget()->initializeSliderGroup();

				close();
			}
			
		}

		void DeleteSelectedDialog::cancelClicked() {
			close();
		}

		void DeleteSelectedDialog::populateLastFrameList(int firstFrameIndex) {
			m_lastFrame->clear();

			int totalFrames = m_sheetCanvas->getFramesFromCurrentLayer();
			int numberOfFramesLeft = totalFrames - firstFrameIndex;

			for (int i = 0; i < numberOfFramesLeft; i++) {
				m_lastFrame->addItem("Frame " + QString::number(firstFrameIndex + 1 + i));
			}
		}
	}
}