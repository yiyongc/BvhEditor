#include "InsertFrameDialog.h"


namespace cacani {

	namespace ui {
		
		InsertFrameDialog::InsertFrameDialog(SheetCanvas* sheetCanvas, cacani::controller::LayerController* layerController, cacani::controller::LayerSelector* layerSelector) :
			m_sheetCanvas(sheetCanvas),
			m_layerController(layerController),
			m_layerSelector(layerSelector),
			m_Button_Confirm(new QPushButton("Confirm")),
			m_Button_Cancel(new QPushButton("Cancel")),
			m_insertLocation(new QComboBox())
		{
			//Setup link for the 2 buttons
			connect(m_Button_Confirm, SIGNAL(clicked(bool)), this, SLOT(confirmClicked()));
			connect(m_Button_Cancel, SIGNAL(clicked(bool)), this, SLOT(cancelClicked()));

			//Setup combo box for selecting where to insert frames
			for (int i = 0; i <= m_sheetCanvas->getFramesFromCurrentLayer(); i++) {
				m_insertLocation->addItem("Frame " + QString::number(i + 1));
			}

			//adding the buttons into a widget			
			QHBoxLayout* buttonLayout = new QHBoxLayout;
			buttonLayout->addWidget(m_Button_Confirm);
			buttonLayout->addWidget(m_Button_Cancel);
			QWidget* buttonWidget = new QWidget;
			buttonWidget->setLayout(buttonLayout);

			//Setting up selection widget
			QFormLayout* selectionLayout = new QFormLayout;
			QLabel* selectionLabel = new QLabel("Choose Where To Insert:");
			QLabel* numberLabel = new QLabel("Choose How Many Frames To Insert:");
			m_numberToInsert = new QSpinBox;
			m_numberToInsert->setRange(1, 999);
			selectionLayout->addRow(selectionLabel, m_insertLocation);
			selectionLayout->addRow(numberLabel, m_numberToInsert);
			QWidget* inputWidget = new QWidget;
			inputWidget->setLayout(selectionLayout);

			//Combining both Widgets
			QVBoxLayout* dialogLayout = new QVBoxLayout;
			dialogLayout->addWidget(inputWidget);
			dialogLayout->addWidget(buttonWidget);

			setLayout(dialogLayout);
			setWindowTitle("Insert Frames To Layer");
			resize(500, 300);
		}

		InsertFrameDialog:: ~InsertFrameDialog() {}

		void InsertFrameDialog::confirmClicked() {
			//Stop any ongoing animation first
			m_sheetCanvas->stopAnimation();

			m_layerController->addFrames(m_insertLocation->currentIndex(), m_numberToInsert->value());

			//Make added frame show as activeframe in the framewidget and update view
			m_layerSelector->setActiveFrame(m_insertLocation->currentIndex());
			m_layerController->updateCananiUI();
			m_sheetCanvas->update();

			//Update Sliders
			m_layerController->jointEditorWidget()->initializeSliderGroup();

			close();
		}

		void InsertFrameDialog::cancelClicked() {
			close();
		}

	};
}
