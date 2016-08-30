#include "ImportDialog.h"

namespace cacani{

	namespace ui{

		ImportDialog::ImportDialog(LayerController* layerController, QString filename) :
			m_layerController(layerController),
			m_filename(filename),
			m_LayerGroupList(new QComboBox()),
			m_radio1(new QRadioButton(tr("To new layer group"))),
			m_radio2(new QRadioButton(tr("To existing layer group"))),
			m_Button_Confirm(new QPushButton("Confirm")),
			m_Button_Cancel(new QPushButton("Cancel"))
		{

			connect(m_Button_Confirm, SIGNAL(clicked(bool)), this, SLOT(confirmClicked()));
			connect(m_Button_Cancel, SIGNAL(clicked(bool)), this, SLOT(cancelClicked()));
			connect(m_radio2, SIGNAL(toggled(bool)), this, SLOT(radio2Clicked(bool)));


			QGroupBox *groupBox = new QGroupBox(tr("Import New BVH File"));

			m_radio1->setChecked(true);

			for (int i = 0; i < m_layerController->getCurrentRoot()->memberCount(); i++){
				m_LayerGroupList->addItem("Layer Group " + QString::number(i + 1));
			}

			m_LayerGroupList->setDisabled(1);

			QVBoxLayout *vbox = new QVBoxLayout;
			vbox->addWidget(m_radio1);
			vbox->addWidget(m_radio2);
			vbox->addWidget(m_LayerGroupList);
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
			setWindowTitle("Import New");
			resize(600, 300);

		}


		ImportDialog::~ImportDialog()
		{
		}

		void ImportDialog::confirmClicked(){
			if (m_radio1->isChecked()){
				m_layerController->addLayerGroupFromFile(m_filename.toStdString().c_str());
			}
			else if (m_radio2->isChecked()){
				m_layerController->addLayerToGroupFromFile(m_LayerGroupList->currentIndex(), m_filename.toStdString().c_str());
			}
			int latestLayerGroupIndex;
			if (m_LayerGroupList->isEnabled())
				latestLayerGroupIndex = m_LayerGroupList->currentIndex();
			else
				latestLayerGroupIndex = m_layerController->getCurrentRoot()->memberCount() - 1;
			NewFileFPSDialog* newFileFPSDialog = new NewFileFPSDialog(m_layerController, latestLayerGroupIndex);
			newFileFPSDialog->exec();
			close();

		}

		void ImportDialog::cancelClicked(){	
			close();
		}

		void ImportDialog::radio2Clicked(bool checked){
			m_LayerGroupList->setEnabled(checked);
		}
	}
}

