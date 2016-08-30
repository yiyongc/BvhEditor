#include "NewFileFPSDialog.h"

namespace cacani{

	namespace ui{

		NewFileFPSDialog::NewFileFPSDialog(LayerController* layerController, int layerGroupIndex) :
			m_layerController(layerController),
			m_Button_Proj(new QPushButton("Keep Project fps")),
			m_Button_File(new QPushButton("Change to New File fps"))
		{

			connect(m_Button_Proj, SIGNAL(clicked(bool)), this, SLOT(buttonProjClicked()));
			connect(m_Button_File, SIGNAL(clicked(bool)), this, SLOT(buttonFileClicked()));

			m_proj_fps = m_layerController->getFrameRateForProject();

			int latest_layergroup_index = layerGroupIndex;
			int latest_layer_index = dynamic_cast<LayerGroup*>(m_layerController->getCurrentRoot()->childAtIndex(latest_layergroup_index))->memberCount()-1;
			m_new_layer = dynamic_cast<LayerGroup*>(m_layerController->getCurrentRoot()->childAtIndex(latest_layergroup_index))->childAtIndex(latest_layer_index);
			m_newfile_fps = m_new_layer->getLayerOriginalFPS();

			QLabel* l1 = new QLabel("Project frame rate: " + QString::number(m_proj_fps));
			QLabel* l2 = new QLabel("New file frame rate: " + QString::number(m_newfile_fps));

			QWidget* widget_button_group = new QWidget;
			QHBoxLayout *hbox = new QHBoxLayout;

			if (m_proj_fps != m_newfile_fps){

				hbox->addWidget(m_Button_Proj);
				hbox->addWidget(m_Button_File);

				setWindowTitle("Converting frame rate");

			}
			else{

				hbox->addWidget(m_Button_Proj);
				m_Button_Proj->setText("Continue");

				setWindowTitle("Confirming frame rate");

			}
			hbox->addStretch(1);
			widget_button_group->setLayout(hbox);

			QVBoxLayout* dialog_layout = new QVBoxLayout;
			dialog_layout->addWidget(l1);
			dialog_layout->addWidget(l2);
			dialog_layout->addWidget(widget_button_group);

			setLayout(dialog_layout);


			resize(600, 300);
		}


		NewFileFPSDialog::~NewFileFPSDialog()
		{
		}

		void NewFileFPSDialog::buttonProjClicked(){
			//use proj fps to create the new layer
			m_layerController->updateLayerFPS(m_new_layer, m_proj_fps);
			//m_new_layer->updateLayerFPS(m_proj_fps);
			close();
		}


		void NewFileFPSDialog::buttonFileClicked(){
			//use file fps to update the entire proj
			m_layerController->updateFrameRateForProject(m_newfile_fps);
			close();
		}

		
	}
}

