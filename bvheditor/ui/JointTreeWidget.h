#pragma once
#include <qtreeview.h>
#include "../QsLog2/qslog.h"
#include <QDirModel>
#include <QTreeWidgetItem>
#include <vector>
#include "../controllers/LayerController.h"
#include "../data/JointTreeListModel.h"
//#include "SheetCanvas.h"

using namespace cacani::controller;

namespace cacani {

	namespace controller {
		class LayerController;
	}

	namespace data{
		class JointTreeListModel;
	}

	namespace ui {

		class JointTreeWidget :public QTreeView
		{
		public:
			JointTreeWidget(cacani::controller::LayerController* layerController, SheetCanvas* sheetCanvas, JointTreeListModel* jointTreeListModel);
			~JointTreeWidget();
			
			void mousePressEvent(QMouseEvent * event);
			void switchLayer(Layer* layer);

		private:
			cacani::controller::LayerController* m_layerController;
			SheetCanvas* m_sheetCanvas;
			Layer* m_layer;
		};

	}
}
