#include "LayerSelector.h"
#include "../ui/SheetCanvas.h"
#include "../ui/JointTreeWidget.h"
#include "../controllers/LayerController.h"


namespace cacani {

	namespace controller  {

		LayerSelector::LayerSelector(LayerController* layerController, SheetCanvas* sheetCanvas, JointEditorWidget* jointEditorWidget, JointTreeWidget* jointTreeWidget) 
			:m_layerController(layerController), m_sheetCanvas(sheetCanvas),
			m_jointEditorWidget(jointEditorWidget), m_jointTreeWidget(jointTreeWidget)
		{
			vector<LayerId> selectedLayerIds;
			m_selectedLayerIds = selectedLayerIds;

			set<FrameId> selectedFrameIds;
			m_selectedFrameIds = selectedFrameIds;

			m_activeFrameId = 0;
			LayerId * init_layerId = new LayerId(0, 0);
			init_layerId->setIndent(1);
			m_activeLayerId = *init_layerId;

			QLOG_INFO() << "LayerSelector:" << "Constructor called";
		}

		
		LayerSelector::~LayerSelector(){}

		LayerId LayerSelector::findLayerId(const Layer* al){

			//if (!al->check_load_success()){
				//QLOG_INFO() << "LayerSelector:" << "m_layerController->getCurrentRoot() has not been loaded, cannot findLayerId";
				//return LayerId();
			//}

			int it_m;
			int it_n;

			LayerGroup* root = m_layerController->getCurrentRoot();

			for (it_m = 0; it_m < root->memberCount(); it_m++){
				const LayerGroup* tmp_terminal = dynamic_cast<const LayerGroup*>(root->childAtIndex(it_m));
				vector<Layer*> tmp_copy = tmp_terminal->getChildren();
				vector<Layer*>::iterator it;
				for (it = tmp_copy.begin(); it != tmp_copy.end(); ++it)
				{
					if ((*it) == al){
						it_n = std::distance(tmp_copy.begin(), it);
						LayerId* result = new LayerId(LAYERIDDEPTH);
						result->setIndent(1);
						result->setIndex(it_m);
						result->setIndent(2);
						result->setIndex(it_n);
						return *result;
					}
				}
			}
			return LayerId();
		}


		LayerId LayerSelector::findLayerId(const Layer* selectedLayer, const LayerGroup* selectedParent){
			
			LayerGroup* root = m_layerController->getCurrentRoot();
			int it_m;
			int it_n;

			LayerId* result = new LayerId(LAYERIDDEPTH);
			result->setIndent(1);

			for (it_m = 0; it_m < root->memberCount(); it_m++){
				if (root->childAtIndex(it_m) == selectedParent){
					result->setIndex(it_m);
					break;
				}
				//the last loop -- still haven't met the previous childAtIndex condition 
				//-- failed
				else if (it_m == root->memberCount() - 1){
					return LayerId();
				}
			}
			result->setIndent(2);

			const LayerGroup* tmp_copy = dynamic_cast<const LayerGroup*>(root->childAtIndex(it_m));
			for (it_n = 0; it_n < tmp_copy->memberCount(); it_n++){
				if (tmp_copy->childAtIndex(it_n) == selectedLayer){
					result->setIndex(it_n);
					return *result;
				}
				//failed condition
				else if (it_n == tmp_copy->memberCount() - 1){
					return LayerId();
				}
			}
		}

		bool LayerSelector::layerIsSelected(const LayerId& lid){

			vector<LayerId>::iterator it;

			it = find(m_selectedLayerIds.begin(), m_selectedLayerIds.end(), lid);
			if (it != m_selectedLayerIds.end()){
				QLOG_INFO() << "LayerSelector:" << "Element found in m_selectedLayerIds";
				return true;
			}
			else{
				QLOG_INFO() << "Element not found in m_selectedLayerIds";
				return false;
			}
		}

		bool LayerSelector::frameIsSelected(const FrameId fid){ 
			
			set<FrameId>::iterator it;
			it = m_selectedFrameIds.find(fid);
			bool is_in = ( it != m_selectedFrameIds.end() );

			QLOG_INFO() << "LayerSelector:" << "Element found/not found in m_selectedFrameIds: " << is_in;
			return is_in;
		}


		void LayerSelector::setActiveSheet(const SheetId& sid, bool broadcast){
			m_activeSheetId = sid;
		}

		void LayerSelector::setActiveLayer(const LayerId& lid, bool broadcast){
			m_activeLayerId = lid;		
			m_sheetCanvas->updateBase(dynamic_cast<LayerGroup*>(activeCel()));
			//m_JointTreeListModel->changeJointModelData(activeLayer());
			m_jointTreeWidget->switchLayer(activeLayer());
			m_jointEditorWidget->initializeJoint(activeLayer(), activeLayer()->GetJoint(0));
		}

		void LayerSelector::setActiveFrame(const FrameId fid){ 
			m_activeFrameId = fid;
			m_sheetCanvas->updateFrame(fid);
			m_jointEditorWidget->updateFrame(fid);
		}

		void LayerSelector::setJointTreeListModel(JointTreeListModel* model){
			m_JointTreeListModel = model;
		}


		void LayerSelector::addSelectedLayer(const LayerId& lid){
			m_selectedLayerIds.push_back(lid);
		}

		void LayerSelector::addSelectedFrame(const FrameId fid){  
			m_selectedFrameIds.insert(fid);
		}

		void LayerSelector::addFrameSelection(const LayerId& lid, int level){}

		void LayerSelector::popSelectedFrame(FrameId pos){
			m_selectedFrameIds.erase(pos);
		}

		void LayerSelector::selectLayerRange(const LayerId& selectedLid){}

		void LayerSelector::selectFrameRange(int level){}
		 
		const vector<LayerId>& LayerSelector::selectedLayersId(){
			return m_selectedLayerIds;
		}

		set<FrameId>& LayerSelector::selectedFrames(){ 
			return m_selectedFrameIds;
		}

		Layer* LayerSelector::activeLayer() const{
			// make activeLayerId for tmp use as non-const
			LayerId activeLayerId = m_activeLayerId;
			activeLayerId.setIndent(1);
			int m = activeLayerId.index();
			activeLayerId.setIndent(2);
			int n = activeLayerId.index();
			return static_cast<const LayerGroup*>(m_layerController->getCurrentRoot()->getChildren().at(m))->getChildren().at(n);
		}

		Layer* LayerSelector::activeCel() const{
			LayerId activeLayerId = m_activeLayerId;
			activeLayerId.setIndent(1);
			int m = activeLayerId.index();
			return m_layerController->getCurrentRoot()->getChildren().at(m);
		}

		int LayerSelector::activeCelIndex() const{
			LayerId activeLayerId = m_activeLayerId;
			activeLayerId.setIndent(1);
			int m = activeLayerId.index();
			return m;
		}

		int LayerSelector::activeCelChildIndex() const{
			LayerId activeLayerId = m_activeLayerId;
			activeLayerId.setIndent(1);
			int m = activeLayerId.index();
			activeLayerId.setIndent(2);
			int n = activeLayerId.index();
			return n;
		}


		const LayerId& LayerSelector::activeLayerId() const{
			return m_activeLayerId;
		}


		FrameId LayerSelector::activeFrame() const{	
			return m_activeFrameId;
		}

		SheetId LayerSelector::activeSheetId() const{
			return m_activeSheetId;
		}


		Layer* LayerSelector::activeLayerRaw() const{
			return 0;
		}

		Cel* LayerSelector::activeCelRaw() const{
			return 0;
		}

		const SheetId& LayerSelector::absoluteActiveSheetId() const{
			//SheetId* tmp = new SheetId(activeLayerId(), activeFrame());
			//return *tmp;
			return SheetId();
		}

		const FrameId& LayerSelector::absoluteActiveFrame() const{  
			return 0;
		}

		void LayerSelector::applyToSelectedFrameRanges(const function<void(int, int)>& f){}

		void LayerSelector::clearSelectedLayers(){
			m_selectedLayerIds.clear();
		}

		void LayerSelector::clearSelectedFrames(){
			m_selectedFrameIds.clear();
		}




	}
}
