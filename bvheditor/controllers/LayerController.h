#pragma once

#include "AbstractLayerController.h"
#include "../QsLog2/qslog.h"
#include "../data/LayerGroup.h"
#include "../ui/JointEditorWidget.h"
#include "../ui/SheetCanvas.h"
#include "../data/LayerId.h"
#include "../data/Arm.h"
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/euler_angles.hpp>
#include "../ui/FCContainer.h"


#define LAYERGROUPDEPTH 1
#define LAYERDEPTH 2


using namespace cacani::ui;

namespace cacani {

	namespace ui{
		class JointEditorWidget;
		class FCContainer;
		enum SliderLinkOffset;
		enum SliderLinkPos;
		enum SliderLinkRot;
	}

	namespace controller {

		class LayerSelector;

		class LayerController : public AbstractLayerController
		{

			Q_OBJECT

		public:
			LayerController();
			~LayerController();

			void setLayerOpacity(LayerId& lid, int value);

			void setLayerExpanded(const LayerId& lid);
			void setLayerVisible(const LayerId& lid);
			void setLayerInvisible(const LayerId& lid);
			void setLayerOnion(const LayerId& lid);
			void setLayerLocked(const LayerId& lid);

			void setSelectedLocked(const Layer* selectedLayer);		
			void setSelectedVisible(const Layer* selectedLayer);	
			void setSelectedOnion(const Layer* selectedLayer);		

			//Moving only cels or groups
			void moveSelectedCels(const LayerId& destinationId);
			void moveSelectedCel(const LayerId& selectedCelId, const LayerId& destinationId);
			//Moving layers are selected to a destination i.e. under or above a certain layer/group
			void moveSelectedLayers(const LayerId& destinationId);
			void moveSelectedLayer(const LayerId& selectedId, const LayerId& destinationId);
			//Moving layers, with the mouse releasing on a group
			void moveSelectedLayersIntoCel(const LayerId& destinationId);
			void moveSelectedLayerIntoCel(const LayerId& selectedId, const LayerId& destinationId);

			void setLayerName(const LayerId& lid, const ::cacani::utf8string& name);

			void addCel();
			void addDrawingLayer();
			void duplicateLayer();
			//duplicate current frame, focus on the latter frame 
			void duplicateCurrentFrame();
			void duplicateCel();
			void removeLayers();
			void removeCels();
			void removeCurrentFrame();

			void selectSimilarColorType(const Layer* selectedLayer, const LayerId& selectedLid);

			void changeLayerColor(const LayerId& lid, int r, int g, int b, int a);
			void changeSelectedLayerColor(int r, int g, int b);

			bool isRepeatSeqSync(const LayerId& lid, const LayerId& destId){ return true; }
			bool isLayerSeqSync(const LayerId& lid, const LayerId& destId) { return true; }

			void moveFrameSeq(const LayerId& celLid, int layerIndex, int movingFrame, int destFrame);

			void setCelOffset(const LayerId& lid, int offset);     
			bool anyAvailableSheet();

			void appendKeyFrameToActiveCel(int count);
			void appendRepeatFrameToActiveCel(int count);
			void prependKeyFrameToActiveCel(int count);
			void insertKeyFrames(const LayerId& celLid, int layerIndex, int frameNo, int count);
			void insertRepeatFrames(const LayerId& celLid, int layerIndex, int frameNo, int count);
			void dupFrameIntoSeq(const LayerId& celLid, int layerIndex, int frameNo, int count);
			void dupSeqToLoop(const LayerId& celLid, int layerIndex, int frameNo, int count, bool convert = false, bool key = false);
			void dupSeqToPingRev(const LayerId& celLid, int layerIndex, int frameNo, int count, bool convert = false, bool key = false);
			void dupSeqToPingPong(const LayerId& celLid, int layerIndex, int frameNo, int count, bool convert = false, bool key = false);

			void removeFrameSeq(const LayerId& celLid, int layerIndex, int frame, int count);
			void removeSelectedFrames();

			void selectJoint(Layer* layer, vector<int>* row_group);

			void updateJointOffset(Joint* joint, cacani::ui::SliderLinkOffset offsetType, double offsetValue); 
			//xiaocai added
			void updateJointOffset(Layer* layer, Joint* joint, cacani::ui::SliderLinkOffset offsetType, double offsetValue);

			void updateRootPosition(FrameId frame, Layer* layer, cacani::ui::SliderLinkPos positionType, double positionValue);
			void updateJointRotation(FrameId frame, Layer* layer, Joint* joint, cacani::ui::SliderLinkRot rotationType, double rotationValue);
			void updateJointRotOrder(Layer* layer, Joint* joint, cacani::data::ChannelOrder order);

			double getJointOffset(Joint* joint, cacani::ui::SliderLinkOffset offsetType);
			double getRootPosition(FrameId frame, Layer* layer, cacani::ui::SliderLinkPos positionType);
			double getJointRotation(FrameId frame, Layer* layer, Joint* joint, cacani::ui::SliderLinkRot rotationType);
			cacani::data::ChannelOrder getJointRotOrder(Joint* joint);

			void attachJointEditorWidget(JointEditorWidget* myJointEditorWidget){ m_jointEditorWidget = myJointEditorWidget; }
			void attachLayerSelector(LayerSelector* myLayerSelector){ m_layerSelector = myLayerSelector; }

			void addLayerGroup();
			void addLayerGroup(LayerGroup* newLayerGroup);
			void addLayerGroupFromFile(const char * bvh_file_name);

			void addLayerToGroup(int layerGroupIndex);
			void addLayerToGroup(int layerGroupIndex, Layer* newLayer);
			void addLayerToGroupFromFile(int layerGroupIndex, const char * bvh_file_name);
			void exportLayer(int layerGroupIndex, int layerIndex, QString targetLoc);

			LayerGroup* getCurrentRoot(){ return m_root; }
			void setCurrentRoot(LayerGroup* root){ m_root = root;}

			void updateFrameRateForProject(double frameRate){

				m_root->updateProjFrameRate(frameRate); //updateCananiUI();
				emit updateCananiUI();
				//emit FPS_updated(); 
			}
			double getFrameRateForProject(){ return m_root->getProjFrameRate(); }

			void updateLayerFPS(Layer* new_layer, double new_fps){
				new_layer->updateLayerFPS(new_fps); //updateCananiUI();
				emit updateCananiUI();
				//emit FPS_updated(); 
			}

			void inbetween(int num_inbetween);
			
			
			void attachFCContainer(FCContainer* fCContainer);
			void updateScroll();

			FCContainer* getFCContainer() { return m_FCContainer; }


			LayerSelector* layerSelector(){ return m_layerSelector; }
			void applyIK(Arm* model_arm, Joint* endJoint, Joint* baseJoint);

			void addFrames(FrameId fid, int num);

			void makeTPose(FrameId fid);

			JointEditorWidget* jointEditorWidget() { return m_jointEditorWidget; }

Q_SIGNALS:
			void updateCananiUI();

		private:
			LayerGroup* m_root;
			JointEditorWidget* m_jointEditorWidget;
			LayerSelector* m_layerSelector;
			FCContainer *m_FCContainer;

			//begin: xiaocai added
			void ChangeHipsHeightAccordingToLegLengths(Layer* layer);
			//end: xiaocai added

		};
	}
}






/////////////TEMP METHODS WITHOUT LAYERID///////////////

/*
void setCurrentLayerGroup(LayerGroup* layerGroup){
QLOG_INFO() << "LayerController:" << "setCurrentLayerGroup() called";
m_LayerGroup = layerGroup;
}
*/
/*
void addLayerToCurrentLayerGroup(Layer* layer){
QLOG_INFO() << "LayerController:" << "addLayerToCurrentLayerGroup() called";
m_LayerGroup->addLayer(layer);
}
*/