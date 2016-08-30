#include "ExportDialog.h"

namespace cacani {


	namespace ui {
		ExportDialog::ExportDialog(LayerController* layerController):
			m_layerController(layerController),
			m_LayerGroupList(new QComboBox()),
			m_LayerList(new QComboBox()),
			m_Button_Confirm(new QPushButton("Confirm")),
			m_Button_Cancel(new QPushButton("Cancel")),
			m_Button_Browse(new QPushButton("..."))
		{

			connect(m_Button_Confirm, SIGNAL(clicked(bool)), this, SLOT(confirmClicked()));
			connect(m_Button_Cancel, SIGNAL(clicked(bool)), this, SLOT(cancelClicked()));
			connect(m_LayerGroupList, SIGNAL(currentIndexChanged(int)), this, SLOT(layerGroupChosen(int)));
			connect(m_Button_Browse, SIGNAL(clicked(bool)), this, SLOT(browseClicked()));


			QLabel* l1 = new QLabel("Please select a layer group");
			QHBoxLayout* combo1_layout = new QHBoxLayout;
			combo1_layout->addWidget(l1);
			combo1_layout->addWidget(m_LayerGroupList);
			QWidget* combo1_widget = new QWidget;
			combo1_widget->setLayout(combo1_layout);

			for (int i = 0; i < m_layerController->getCurrentRoot()->memberCount(); i++){
				m_LayerGroupList->addItem("Layer Group " + QString::number(i + 1));
			}

			QLabel* l2 = new QLabel("Please select a layer to export");
			QHBoxLayout* combo2_layout = new QHBoxLayout;
			combo2_layout->addWidget(l2);
			combo2_layout->addWidget(m_LayerList);
			QWidget* combo2_widget = new QWidget;
			combo2_widget->setLayout(combo2_layout);

			QLabel* l3 = new QLabel("Browse target location:");
			QHBoxLayout* targetloc_layout = new QHBoxLayout;
			targetloc_layout->addWidget(l3);
			targetloc_layout->addWidget(m_Button_Browse);
			targetloc_layout->setStretch(0, 4);
			targetloc_layout->setStretch(1, 1);
			QWidget* targetloc_widget = new QWidget;
			targetloc_widget->setLayout(targetloc_layout);

			QHBoxLayout* button_layout = new QHBoxLayout;
			button_layout->addWidget(m_Button_Confirm);
			button_layout->addWidget(m_Button_Cancel);
			QWidget* button_widget = new QWidget;
			button_widget->setLayout(button_layout);

			QVBoxLayout* dialog_layout = new QVBoxLayout;
			dialog_layout->addWidget(combo1_widget);
			dialog_layout->addWidget(combo2_widget);
			dialog_layout->addWidget(targetloc_widget);
			dialog_layout->addWidget(button_widget);

			setLayout(dialog_layout);
			setWindowTitle("Export BVH");
			resize(800, 300);
		}


		ExportDialog::~ExportDialog()
		{
		}

		void ExportDialog::confirmClicked(){
			if (m_targetloc == NULL){
				QMessageBox msgBox;
				msgBox.setText("Please choose a target directory to export.");
				msgBox.exec();
			}
			else{
				m_layerController->exportLayer(m_LayerGroupList->currentIndex(), m_LayerList->currentIndex(), m_targetloc);
				close();
			}
		}

		void ExportDialog::cancelClicked(){
			close();
		}

		void ExportDialog::layerGroupChosen(int index){
			m_LayerList->clear();
			for (int i = 0; i < dynamic_cast<LayerGroup*>(m_layerController->getCurrentRoot()->childAtIndex(index))->memberCount(); i++){
				m_LayerList->addItem(QString::fromStdString(dynamic_cast<LayerGroup*>(m_layerController->getCurrentRoot()->childAtIndex(index))->childAtIndex(i)->GetMotionName()));
			}
		}

		void ExportDialog::browseClicked(){
			m_targetloc = QFileDialog::getExistingDirectory(
				this, 
				tr("Open Directory"),
				"",
				QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		}

	}
}
