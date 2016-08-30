#pragma once
#include "qdialog.h"
#include "qlabel.h"
#include "qradiobutton.h"
#include "qcombobox.h"
#include "qgroupbox.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../controllers/LayerController.h"
#include <QPushButton>
#include "NewFileFPSDialog.h"



namespace cacani {


	namespace ui {

		class ImportDialog :
			public QDialog
		{

			Q_OBJECT

		public:
			ImportDialog(LayerController* layerController, QString filename);
			~ImportDialog();

		private:
			
			LayerController* m_layerController;
			QString m_filename;
			QRadioButton* m_radio1;
			QRadioButton* m_radio2;
			QComboBox* m_LayerGroupList;
			QPushButton* m_Button_Confirm;
			QPushButton* m_Button_Cancel;

		private Q_SLOTS:
			void confirmClicked();
			void cancelClicked();
			void radio2Clicked(bool checked);
		};

	}
}


