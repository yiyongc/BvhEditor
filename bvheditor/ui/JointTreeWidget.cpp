#include "JointTreeWidget.h"

namespace cacani {


	namespace ui {

		JointTreeWidget::JointTreeWidget(cacani::controller::LayerController* layerController, SheetCanvas* sheetCanvas, JointTreeListModel* jointTreeListModel)
		{
			m_layerController = layerController;
			m_sheetCanvas = sheetCanvas;
			m_layer = dynamic_cast<const LayerGroup*>(m_layerController->getCurrentRoot()->childAtIndex(0))->childAtIndex(0);
			setModel(jointTreeListModel);
			expandAll();
		}


		JointTreeWidget::~JointTreeWidget()
		{
		}

		void JointTreeWidget::mousePressEvent(QMouseEvent * event)
		{
			QLOG_INFO() << "JointTreeWidget:" << "mouse event";
			QTreeView::mousePressEvent(event);


			QModelIndexList list = this->selectionModel()->selectedIndexes();
			QDirModel* model = (QDirModel*)this->model();
			int row = -1;
			int column = -1;

			std::vector<int>* row_group = new std::vector<int>;

			foreach(QModelIndex index, list)
			{
				int cnt = 0;
				while (index.parent().isValid()){
					row_group->push_back(index.row());
					cnt++;
					index = index.parent();
				}
				for (int i = 0; i < row_group->size(); i++){
					QLOG_INFO() << "JointTreeWidget:" << "row_group->at("<<i<<") =" << row_group->at(i);
				}
				QLOG_INFO() << "JointTreeWidget:" << "cnt =" << cnt;
	
				
			}
			m_layerController->selectJoint(m_layer, row_group);
			m_sheetCanvas->update();

		}


		void JointTreeWidget::switchLayer(Layer* layer){
			QItemSelectionModel *m = this->selectionModel();
			JointTreeListModel* newModel = new JointTreeListModel(layer);
			this->setModel(newModel);
			delete m;
			m_layer = layer;
			expandAll();
		}

	}
}