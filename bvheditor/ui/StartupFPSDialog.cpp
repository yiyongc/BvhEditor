#include "StartupFPSDialog.h"

namespace cacani{

	namespace ui{

		StartupFPSDialog::StartupFPSDialog(LayerController* layerController, QString filename, StartupDialog * parent) :
			m_parent(parent),
			m_layerController(layerController),
			m_radio1(new QRadioButton),
			m_radio2(new QRadioButton(tr("Use customized frame rate:"))),
			m_input_fps(new QDoubleSpinBox),
			m_Button_Confirm(new QPushButton("Confirm")),
			m_Button_Cancel(new QPushButton("Cancel"))
		{

			connect(m_Button_Confirm, SIGNAL(clicked(bool)), this, SLOT(confirmClicked()));
			connect(m_Button_Cancel, SIGNAL(clicked(bool)), this, SLOT(cancelClicked()));
			connect(m_radio2, SIGNAL(toggled(bool)), this, SLOT(radio2Clicked(bool)));

			m_input_fps->setRange(0, MAXFRAMERATE);
			m_input_fps->setDecimals(1);
			m_input_fps->setDisabled(1);

		    //get the frame rate of the imported bvh file and populate the number to both radio 1 and initial value of radio 2
			initializeFirstLayer(layerController, filename);
			m_radio1->setText("Use imported file frame rate: " + QString::number(m_default_fps));
			m_input_fps->setValue(m_default_fps);

			//ui arrangements
			QGroupBox *groupBox = new QGroupBox(tr("Set frame rate (fps) for the new project"));

			m_radio1->setChecked(true);



			QVBoxLayout *vbox = new QVBoxLayout;
			vbox->addWidget(m_radio1);
			vbox->addWidget(m_radio2);
			vbox->addWidget(m_input_fps);
			vbox->addStretch(1);
			groupBox->setLayout(vbox);

			QVBoxLayout* dialog_layout = new QVBoxLayout;
			dialog_layout->addWidget(groupBox);

			QHBoxLayout* button_layout = new QHBoxLayout;
			QWidget* button_widget = new QWidget;
			button_layout->addWidget(m_Button_Confirm);
			button_layout->addWidget(m_Button_Cancel);
			button_widget->setLayout(button_layout);

			dialog_layout->addWidget(button_widget);

			setLayout(dialog_layout);
			setWindowTitle("Frame Rate Setting");
			resize(600, 300);

		}


		StartupFPSDialog::~StartupFPSDialog()
		{
		}

		void StartupFPSDialog::confirmClicked(){
			double valid_fps;
			if (m_radio1->isChecked()){
				valid_fps = m_default_fps;
			}
			else if (m_radio2->isChecked()){
				valid_fps = m_input_fps->value();
			}
			//essentially only setting the fps for the new file
			m_layerController->updateFrameRateForProject(valid_fps);
			m_parent->m_parent->m_success = 1;
			close();
		}

		void StartupFPSDialog::cancelClicked(){
			m_parent->m_parent->m_success = 0;
			close();
		}

		void StartupFPSDialog::radio2Clicked(bool checked){
			m_input_fps->setEnabled(checked);
		}

		void StartupFPSDialog::initializeFirstLayer(LayerController* layerController, QString filename){
			if (m_layerController->getCurrentRoot()->memberCount() == 0){
				m_layerController->addLayerGroupFromFile(filename.toStdString().c_str());			
			}
			m_default_fps = dynamic_cast<LayerGroup*>(m_layerController->getCurrentRoot()->childAtIndex(0))->childAtIndex(0)->getLayerOriginalFPS();
		}

	}
}

