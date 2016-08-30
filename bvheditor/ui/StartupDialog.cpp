#include "StartupDialog.h"

namespace cacani{

	namespace ui{

		StartupDialog::StartupDialog(LayerController* layerController, MainWindow * parent) :
			m_parent(parent),
			m_layerController(layerController),
			m_filename(""),
			m_Label_LayerName(new QLabel("")),
			m_Button_Browse(new QPushButton("Browse")),
			m_Button_Confirm(new QPushButton("Confirm")),
			m_Button_Cancel(new QPushButton("Cancel"))
		{

			connect(m_Button_Browse, SIGNAL(clicked(bool)), this, SLOT(browseClicked()));
			connect(m_Button_Confirm, SIGNAL(clicked(bool)), this, SLOT(confirmClicked()));
			connect(m_Button_Cancel, SIGNAL(clicked(bool)), this, SLOT(cancelClicked()));
			connect(this, SIGNAL(file_updated(QString)), this, SLOT(fileUpdated(QString)));


			QLabel* l1 = new QLabel("Please select a BVH file to start the project");

			QWidget* widget_button_group = new QWidget;
			QHBoxLayout *hbox = new QHBoxLayout;
			hbox->addWidget(m_Button_Browse);
			hbox->addWidget(m_Button_Confirm);
			hbox->addWidget(m_Button_Cancel);
			hbox->addStretch(1);
			widget_button_group->setLayout(hbox);

			QVBoxLayout* dialog_layout = new QVBoxLayout;
			dialog_layout->addWidget(l1);
			dialog_layout->addWidget(m_Label_LayerName);
			dialog_layout->addWidget(widget_button_group);

			setLayout(dialog_layout);
			setWindowTitle("New Project");
			resize(600, 300);
		}


		StartupDialog::~StartupDialog()
		{
		}

		void StartupDialog::confirmClicked(){

			if (((m_filename == NULL) || (m_filename == "")) && ((m_Label_LayerName->text() == NULL) || (m_Label_LayerName->text() == ""))){
				QMessageBox msgBox;
				msgBox.setText("Please select an BVH file to start a new project.");
				msgBox.exec();
			} else{
				m_parent->m_success = 0;
				m_filename = m_Label_LayerName->text();
				StartupFPSDialog* startupFPSDialog = new StartupFPSDialog(m_layerController, m_filename, this);
				startupFPSDialog->exec();
				if (m_parent->m_success) close();
			}
		}

		void StartupDialog::cancelClicked(){
			m_parent->m_success = 0;
			close();
		}

		void StartupDialog::browseClicked(){
			 m_filename = QFileDialog::getOpenFileName(
				this,
				tr("Open BVH File"),
				"",
				"BVH file (*.bvh);;"
				);
			 if ((m_filename != NULL) && (m_filename != ""))
				 emit file_updated(m_filename); 
		}

		void StartupDialog::fileUpdated(QString fileName){
			m_Label_LayerName->setText(fileName);
		}
		
	}
}

