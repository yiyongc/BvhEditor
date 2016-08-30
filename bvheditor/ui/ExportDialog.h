#pragma once
#include "qdialog.h"
#include "../controllers/LayerController.h"
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qfiledialog.h>
#include <qmessagebox.h>

namespace cacani {

	namespace ui {

		class ExportDialog :
			public QDialog
		{
			Q_OBJECT

		public:
			ExportDialog(LayerController* layerController);
			~ExportDialog();

		private:

			LayerController* m_layerController;
			QComboBox* m_LayerGroupList;
			QComboBox* m_LayerList;
			QString m_targetloc;
			QPushButton* m_Button_Browse;
			QPushButton* m_Button_Confirm;
			QPushButton* m_Button_Cancel;

		private Q_SLOTS:
			void confirmClicked();
			void cancelClicked();
			void browseClicked();
			void layerGroupChosen(int index);
		};
	}
}

