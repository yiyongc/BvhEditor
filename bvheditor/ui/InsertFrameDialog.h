#pragma once
#include "qdialog.h"
#include "qlabel.h"
#include "qcombobox.h"
#include "qgroupbox.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <qspinbox.h>
#include "../controllers/LayerSelector.h"
#include "SheetCanvas.h"

namespace cacani {

	namespace controller {
		class LayerController;
		class LaterSelector;
	}

	namespace ui {
		class SheetCanvas;

		class InsertFrameDialog :
			public QDialog
		{
			Q_OBJECT

		public:
			InsertFrameDialog(SheetCanvas* sheetCanvas, cacani::controller::LayerController* layerController, cacani::controller::LayerSelector* layerSelector);
			~InsertFrameDialog();

		private:
			SheetCanvas* m_sheetCanvas;
			cacani::controller::LayerController* m_layerController;
			cacani::controller::LayerSelector* m_layerSelector;
			QComboBox* m_insertLocation;
			QSpinBox* m_numberToInsert;
			QPushButton* m_Button_Confirm;
			QPushButton* m_Button_Cancel;

			private Q_SLOTS:
			void confirmClicked();
			void cancelClicked();

		};
	}
}