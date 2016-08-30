#pragma once
#include "qdialog.h"
#include "qlabel.h"
#include "qcombobox.h"
#include "qgroupbox.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "../ui/sheetCanvas.h"
#include <QPushButton>

namespace cacani {

	namespace ui {

		class DeleteSelectedDialog :
			public QDialog
		{

			Q_OBJECT

		public:
			DeleteSelectedDialog(SheetCanvas* sheetCanvas, LayerController* layerController, LayerSelector* layerSelector);
			~DeleteSelectedDialog();

		private:
			SheetCanvas* m_sheetCanvas;
			LayerController* m_layerController;
			LayerSelector* m_layerSelector;
			QComboBox* m_firstFrame;
			QComboBox* m_lastFrame;
			QPushButton* m_Button_Confirm;
			QPushButton* m_Button_Cancel;

			private Q_SLOTS:
			void confirmClicked();
			void cancelClicked();
			void populateLastFrameList(int firstFrameNum);

		};

	}
}