#include "LayerController.h"
#include "../QsLog2/qslog.h"
#include "../data/Layer.h"
#include "gtc/type_ptr.hpp"

#include <fstream>
#include <string.h>



namespace cacani {

	namespace controller {

		const float   figure_scale = 0.02f;

		LayerController::LayerController() {
			QLOG_INFO() << "LayerController:" << "Started";
			m_root = NULL;
			m_jointEditorWidget = NULL;
		}

		LayerController::~LayerController(){}

		void LayerController::addLayerGroup(){
			m_root->addChild();
			emit updateCananiUI(); 
			//updateCananiUI();
		}

		void LayerController::addLayerGroup(LayerGroup* newLayerGroup){
			m_root->addChild(newLayerGroup); 
			emit updateCananiUI(); 
			//updateCananiUI();
		}

		void LayerController::addLayerGroupFromFile(const char * bvh_file_name){
			m_root->addChild(bvh_file_name);
			emit updateCananiUI(); //updateCananiUI();
		}

		void LayerController::addLayerToGroup(int layerGroupIndex) {
			dynamic_cast<LayerGroup*>(m_root->childAtIndex(layerGroupIndex))->addChild();
			emit updateCananiUI(); //updateCananiUI();
		}

		void LayerController::addLayerToGroup(int layerGroupIndex, Layer* newLayer){
			dynamic_cast<LayerGroup*>(m_root->childAtIndex(layerGroupIndex))->addChild(newLayer);
			emit updateCananiUI(); //updateCananiUI();
		}

		void LayerController::addLayerToGroupFromFile(int layerGroupIndex, const char * bvh_file_name){
			QLOG_INFO() << "LayerController.cpp:" << "loadBVHLayer:" << "called";
			dynamic_cast<LayerGroup*>(m_root->childAtIndex(layerGroupIndex))->addChild(bvh_file_name);
			emit updateCananiUI(); //updateCananiUI();
		}

		void LayerController::exportLayer(int layerGroupIndex, int layerIndex, QString targetLoc){
			QString motionName = QString::fromStdString(dynamic_cast<LayerGroup*>(m_root->childAtIndex(layerGroupIndex))->childAtIndex(layerIndex)->GetMotionName());
			dynamic_cast<LayerGroup*>(m_root->childAtIndex(layerGroupIndex))->childAtIndex(layerIndex)->exportBVHtoDir(targetLoc+"/"+motionName);
		}


		void LayerController::setLayerOpacity(LayerId& lid, int value){

		}


		void LayerController::setLayerExpanded(const LayerId& lid){

		}

		void LayerController::setLayerVisible(const LayerId& lid){
			LayerId* operateId = new LayerId(lid);
			cacani::data::LayerId::IndexType operateIndex = operateId->index();
			if (operateId->depth() == LAYERDEPTH){
				operateId->decrementIndent();
				cacani::data::LayerId::IndexType operateGrpIndex = operateId->index();
				const LayerGroup* cel = dynamic_cast<const LayerGroup*>(m_root->childAtIndex(operateGrpIndex));
				cel->data::LayerGroup::childAtIndex(operateIndex)->setVisible();
			}
			else if (operateId->depth() == LAYERGROUPDEPTH)
				m_root->childAtIndex(operateIndex)->setVisible();
		}

		void LayerController::setLayerInvisible(const LayerId& lid){
			LayerId* operateId = new LayerId(lid);
			cacani::data::LayerId::IndexType operateIndex = operateId->index();
			if (operateId->depth() == LAYERDEPTH){
				operateId->decrementIndent();
				cacani::data::LayerId::IndexType operateGrpIndex = operateId->index();

				dynamic_cast<const LayerGroup*>(m_root->childAtIndex(operateGrpIndex))->childAtIndex(operateIndex)->disableVisible();
			}
			else if (operateId->depth() == LAYERGROUPDEPTH)
				m_root->childAtIndex(operateIndex)->disableVisible();
		}

		void LayerController::setLayerOnion(const LayerId& lid){

		}

		void LayerController::setLayerLocked(const LayerId& lid){

		}


		void LayerController::setSelectedLocked(const Layer* selectedLayer){

		}

		void LayerController::setSelectedVisible(const Layer* selectedLayer){

		}

		void LayerController::setSelectedOnion(const Layer* selectedLayer){

		}

		//Moving only cels or groups
		void LayerController::moveSelectedCels(const LayerId& destinationId){

		}

		void LayerController::moveSelectedCel(const LayerId& selectedCelId, const LayerId& destinationId){

		}

		//Moving layers are selected to a destination i.e. under or above a certain layer/group
		void LayerController::moveSelectedLayers(const LayerId& destinationId){

		}

		void LayerController::moveSelectedLayer(const LayerId& selectedId, const LayerId& destinationId){

		}

		//Moving layers, with the mouse releasing on a group
		void LayerController::moveSelectedLayersIntoCel(const LayerId& destinationId){

		}

		void LayerController::moveSelectedLayerIntoCel(const LayerId& selectedId, const LayerId& destinationId){

		}


		void LayerController::setLayerName(const LayerId& lid, const ::cacani::utf8string& name){

		}


		void LayerController::addCel(){

		}

		void LayerController::addDrawingLayer(){


		}

		void LayerController::duplicateLayer(){

		}

		void LayerController::duplicateCel(){

		}

		void LayerController::duplicateCurrentFrame(){
			LayerGroup* rm_target = dynamic_cast<LayerGroup*>(m_layerSelector->activeCel());
			for (int i = 0; i < rm_target->memberCount(); i++){
				Layer* rm_layer = rm_target->childAtIndex(i);
				rm_layer->duplicateSheet(m_layerSelector->activeFrame());
			}
		}

		void LayerController::removeLayers(){

		}

		void LayerController::removeCels(){

		}

		void LayerController::removeCurrentFrame(){
			LayerGroup* rm_target = dynamic_cast<LayerGroup*>(m_layerSelector->activeCel());
			for (int i = 0; i < rm_target->memberCount(); i++){
				Layer* rm_layer = rm_target->childAtIndex(i);
				rm_layer->deleteSheet(m_layerSelector->activeFrame());
			}
		}


		void LayerController::selectSimilarColorType(const Layer* selectedLayer, const LayerId& selectedLid){

		}


		void LayerController::changeLayerColor(const LayerId& lid, int r, int g, int b, int a){

		}

		void LayerController::changeSelectedLayerColor(int r, int g, int b){

		}


		void LayerController::moveFrameSeq(const LayerId& celLid, int layerIndex, int movingFrame, int destFrame){

		}


		void LayerController::setCelOffset(const LayerId& lid, int offset){

		}

		//deleted the namespaces
		bool LayerController::anyAvailableSheet(){
			return 0;
		}


		void LayerController::appendKeyFrameToActiveCel(int count){

		}

		void LayerController::appendRepeatFrameToActiveCel(int count){

		}

		void LayerController::prependKeyFrameToActiveCel(int count){

		}

		void LayerController::insertKeyFrames(const LayerId& celLid, int layerIndex, int frameNo, int count){

		}

		void LayerController::insertRepeatFrames(const LayerId& celLid, int layerIndex, int frameNo, int count){

		}

		void LayerController::dupFrameIntoSeq(const LayerId& celLid, int layerIndex, int frameNo, int count){

		}

		void LayerController::dupSeqToLoop(const LayerId& celLid, int layerIndex, int frameNo, int count, bool convert, bool key){

		}

		void LayerController::dupSeqToPingRev(const LayerId& celLid, int layerIndex, int frameNo, int count, bool convert, bool key){

		}

		void LayerController::dupSeqToPingPong(const LayerId& celLid, int layerIndex, int frameNo, int count, bool convert, bool key){

		}


		void LayerController::removeFrameSeq(const LayerId& celLid, int layerIndex, int frame, int count){
			//layerIndex seems confusing since the paint method and other interactive events do not distinguish individual layers
			//hence make a new version instead of this commented one -> ignore layerIndex
			/*
			LayerGroup* rm_target = dynamic_cast<LayerGroup*>(m_root->childAtIndex(celLid.index()));
			Layer* rm_layer = rm_target->childAtIndex(layerIndex);
			rm_layer->removeSheets(frame, count);
			*/

			//remove frame seq for the layergroup and all its children
			LayerGroup* rm_target = dynamic_cast<LayerGroup*>(m_root->childAtIndex(celLid.index()));
			rm_target->removeSheets(frame, count);
			for (int i = 0; i < rm_target->memberCount(); i++){
				Layer* rm_layer = rm_target->childAtIndex(i);
				rm_layer->removeSheets(frame, count);
			}
		}

		void LayerController::removeSelectedFrames(){

		}

		void LayerController::selectJoint(Layer* layer, vector<int>* row_group){
			Joint* rootJoint = layer->GetJoint(0);
			Joint* targetJoint = rootJoint;
			for (int i = row_group->size()-1; i >= 0; i--){
				targetJoint = targetJoint->children.at(row_group->at(i));
			}
			QLOG_INFO() << "LayerController:" << "selectJoint target joint name" << QString::fromStdString(targetJoint->name);
			layer->selectJoint(targetJoint);
			if (layer->GetMotionName() == m_jointEditorWidget->getCurrentLayer()->GetMotionName()) m_jointEditorWidget->updateJoint(targetJoint);
			else m_jointEditorWidget->initializeJoint(layer, targetJoint);
		}

		void LayerController::updateJointOffset(Joint* joint, cacani::ui::SliderLinkOffset offsetType, double offsetValue){
			QLOG_INFO() << "LayerController updateJointOffset value is " << offsetValue;

			joint->offset[offsetType] = offsetValue;
		}

		// xiaocai commented
		/*void LayerController::updateJointOffset(Layer *layer, Joint* joint, cacani::ui::SliderLinkOffset offsetType, double offsetValue){
		QLOG_INFO() << "LayerController updateJointOffset value is " << offsetValue;

		joint->offset[offsetType] = offsetValue;

		}*/

		// xiaocai added: if the lengths of legs change, we have to adjust the height of Hips joint.
		void LayerController::updateJointOffset(Layer *layer, Joint* joint, cacani::ui::SliderLinkOffset offsetType, double offsetValue)
		{
			QLOG_INFO() << "LayerController updateJointOffset value is " << offsetValue;

			joint->offset[offsetType] = offsetValue;

			// xiaocai added: LeftLeg, LeftFoot, RightLeg, RightFoot
			if( joint->name.compare( /*string("LeftLeg")*/"LeftLeg" ) == 0 || joint->name.compare( "LeftFoot" ) == 0)
			{			
				layer->UpdateLLegLengthChange();
				ChangeHipsHeightAccordingToLegLengths(layer);
				return;
			}
			if( joint->name.compare("RightLeg" ) == 0 || joint->name.compare( "RightFoot" ) == 0)
			{
				layer->UpdateRLegLengthChange();
				ChangeHipsHeightAccordingToLegLengths(layer);
				return;
			}

		}

		// xiaocai added: if the lengths of legs change, we have to adjust the height of Hips joint.
		void LayerController::ChangeHipsHeightAccordingToLegLengths(Layer* layer)
		{
			double LLegLengthChange = layer->GetLLegLengthChange();
			double RLegLengthChange = layer->GetRLegLengthChange();

			double HipsYChange = LLegLengthChange>RLegLengthChange? LLegLengthChange:RLegLengthChange;
			//double HipsYChange = maxChange>0.0 ? maxChange:0.0 ;


			int numFrames = layer->GetNumFrame();

			for(int iFrame=0; iFrame< numFrames; ++iFrame)
			{
				//updateRootPosition(i, layer, SliderLinkPos::Yposition, 0); 
				Sheet *motionSheet = layer->sheetAtIndex(iFrame);
				const double *motionData = motionSheet->getOriginalMotionData();
				motionSheet->setMotion(1/*channelCount*/, motionData[1]+HipsYChange); //1 means - cacani::ui::SliderLinkPos::Yposition)
			}

		}

		void LayerController::updateRootPosition(FrameId frame, Layer* layer, cacani::ui::SliderLinkPos positionType, double positionValue){

			//set channel offset based on channel type
			//order: xpos, ypos, zpos (as default in bvh)
			int channelCount = 0;

			if (positionType == cacani::ui::SliderLinkPos::Yposition)
				channelCount += 1;
			else if (positionType == cacani::ui::SliderLinkPos::Zposition)
				channelCount += 2;

			layer->sheetAtIndex(frame)->setMotion(channelCount, positionValue);

		}


		void LayerController::updateJointRotation(FrameId frame, Layer* layer, Joint* joint, cacani::ui::SliderLinkRot rotationType, double rotationValue){

			//count num of channels before the joint specified
			int channelCount = 0;
			for (int i = 0; i < joint->index; i++){
				channelCount += layer->GetJoint(i)->channels.size();
			}
			//count offset of channel in this joint based on channel order
			if (rotationType == cacani::ui::SliderLinkRot::Xrotation){
				if ((joint->channel_order == YXZ) || (joint->channel_order == ZXY))
					channelCount += 1;
				else if ((joint->channel_order == YZX) || (joint->channel_order == ZYX))
					channelCount += 2;
			}
			else if (rotationType == cacani::ui::SliderLinkRot::Yrotation){
				if ((joint->channel_order == XYZ) || (joint->channel_order == ZYX))
					channelCount += 1;
				else if ((joint->channel_order == XZY) || (joint->channel_order == ZXY))
					channelCount += 2;
			}
			else if (rotationType == cacani::ui::SliderLinkRot::Zrotation){
				if ((joint->channel_order == XZY) || (joint->channel_order == YZX))
					channelCount += 1;
				else if ((joint->channel_order == XYZ) || (joint->channel_order == YXZ))
					channelCount += 2;
			}

			// if it is root joint, need to offset 3
			int	indexOffset = 0;
			if (joint->parent == NULL)
				indexOffset = 3;

			// need to set rotation to the current and all subsequent frames
			for (int i = frame; i < layer->GetNumFrame(); i++){
				layer->sheetAtIndex(i)->setMotion(channelCount + indexOffset, rotationValue);
			}
		}

		void LayerController::updateJointRotOrder(Layer* layer, Joint* joint, cacani::data::ChannelOrder order){
			layer->changeChannelOrder(joint, order);

		}


		double LayerController::getJointOffset(Joint* joint, cacani::ui::SliderLinkOffset offsetType){

			return joint->offset[offsetType];

		}

		double LayerController::getRootPosition(FrameId frame, Layer* layer, cacani::ui::SliderLinkPos positionType){

			//set channel offset based on channel type
			//order: xpos, ypos, zpos (as default in bvh)
			int channelCount = 0;

			if (positionType == cacani::ui::SliderLinkPos::Yposition)
				channelCount += 1;
			else if (positionType == cacani::ui::SliderLinkPos::Zposition)
				channelCount += 2;

			return layer->sheetAtIndex(frame)->getMotion(channelCount);

		}

		double LayerController::getJointRotation(FrameId frame, Layer* layer, Joint* joint, cacani::ui::SliderLinkRot rotationType){


			//count num of channels before the joint specified
			int channelCount = 0;
			for (int i = 0; i < joint->index; i++){
				channelCount += layer->GetJoint(i)->channels.size();
			}
			//count offset of channel in this joint based on channel order
			if (rotationType == cacani::ui::SliderLinkRot::Xrotation){
				if ((joint->channel_order == YXZ) || (joint->channel_order == ZXY))
					channelCount += 1;
				else if ((joint->channel_order == YZX) || (joint->channel_order == ZYX))
					channelCount += 2;
			}
			else if (rotationType == cacani::ui::SliderLinkRot::Yrotation){
				if ((joint->channel_order == XYZ) || (joint->channel_order == ZYX))
					channelCount += 1;
				else if ((joint->channel_order == XZY) || (joint->channel_order == ZXY))
					channelCount += 2;
			}
			else if (rotationType == cacani::ui::SliderLinkRot::Zrotation){
				if ((joint->channel_order == XZY) || (joint->channel_order == YZX))
					channelCount += 1;
				else if ((joint->channel_order == XYZ) || (joint->channel_order == YXZ))
					channelCount += 2;
			}

			// if it is root joint, need to offset 3
			int	indexOffset = 0;
			if (joint->parent == NULL)
				indexOffset = 3;


			return layer->sheetAtIndex(frame)->getMotion(channelCount + indexOffset);
		}

		cacani::data::ChannelOrder LayerController::getJointRotOrder(Joint* joint){
			return joint->channel_order; 
		}

		//this is for generating inbetween frames for ik function
		//num frames changes always apply to all layers within the same layergroup
		void LayerController::inbetween(int num_inbetween){
			LayerGroup* rm_target = dynamic_cast<LayerGroup*>(m_layerSelector->activeCel());
			for (int i = 0; i < rm_target->memberCount(); i++){
				Layer* rm_layer = rm_target->childAtIndex(i);
				rm_layer->inbetween(m_layerSelector->activeFrame(), num_inbetween);
			}
		}

		//m_layerSelector->activeFrame()
		void LayerController::addFrames(FrameId fid, int numFrames){
			LayerGroup* rm_target = dynamic_cast<LayerGroup*>(m_layerSelector->activeCel());
			for (int i = 0; i < rm_target->memberCount(); i++){
				Layer* rm_layer = rm_target->childAtIndex(i);
				rm_layer->addFrames(fid, numFrames);
			}
		}


		void LayerController::applyIK(Arm* model_arm, Joint* endJoint, Joint* baseJoint){


			vector<Segment*> segments = model_arm->get_segments();
			FrameId activeFrame = m_layerSelector->activeFrame();
			Layer* activeLayer = m_layerSelector->activeLayer();
			vector<int> indexChain;
			Joint* tmpForIndexChain = endJoint;
			int breakJointIndex;

			//for index chain, first element will be base, last one will be end
			//(after the later reversal)
			for (int i = activeLayer->joints.size() - 1; i >= 0 ; i--){
				if (activeLayer->joints.at(i)->index == tmpForIndexChain->index){
					indexChain.push_back(i);
					//if (tmpForIndexChain->parent == NULL) { breakJointIndex = i; break; }
					if (tmpForIndexChain->name.compare(baseJoint->name) == 0){ breakJointIndex = i; break; }
					tmpForIndexChain = tmpForIndexChain->parent;
				}
			}

			//this is to store a the sequence of joint indices needed
			//in order to prepare the rotation matrix prior to applying IK changes
			//these joints are the parts before the base joint

			//for prepIndexChain, first element will be root
			//last element will be the parent of base joint (after later reversal)
			vector<int> prepIndexChain;
			//there won't be anything in prepIndexChain if the base joint is the root joint
			//..because there will be no prior transformations to prepare
			if (tmpForIndexChain->index != 0){
				tmpForIndexChain = tmpForIndexChain->parent;
				while (true){
					prepIndexChain.push_back(tmpForIndexChain->index);
					if (tmpForIndexChain->index == 0) break;
					tmpForIndexChain = tmpForIndexChain->parent;
				}
			}


			//parent joint first child next
			std::reverse(indexChain.begin(), indexChain.end());
			std::reverse(prepIndexChain.begin(), prepIndexChain.end());


			//pop the last item because the rotation always affect the joint's parent
			//change: actually no need to pop - just leaving last item untouched will fun
			//keep the last item in case there are only 2 joints involved in IK
			//then will have to use the end joint to calculate offset
			//indexChain.pop_back();
			bool initial_traverse = true;
				while (true){

					//get the combinations of all rotations prior to base joint
					glm::mat4 modelr = glm::mat4(1.0f);
					int channelOffset;
					Joint* tmpJoint;
					for (int i = 0; i < prepIndexChain.size(); i++){
						tmpJoint = activeLayer->GetJoint(prepIndexChain.at(i));
						int test = tmpJoint->index;
						if (tmpJoint->index == 0){
							channelOffset = 3;
						}
						else{
							channelOffset = 0;
						}
						for (int j = 0; j < 3; j++){
							switch (tmpJoint->channels.at(channelOffset + j)->type){
							case data::ChannelEnum::Xrotation: { modelr = modelr * glm::eulerAngleX(float(activeLayer->sheetAtIndex(activeFrame)->getMotion()[prepIndexChain.at(i) * 3 + 3 + j] * M_PI / 180.0f)); break; }
							case data::ChannelEnum::Yrotation: { modelr = modelr * glm::eulerAngleY(float(activeLayer->sheetAtIndex(activeFrame)->getMotion()[prepIndexChain.at(i) * 3 + 3 + j] * M_PI / 180.0f)); break; }
							case data::ChannelEnum::Zrotation: { modelr = modelr * glm::eulerAngleZ(float(activeLayer->sheetAtIndex(activeFrame)->getMotion()[prepIndexChain.at(i) * 3 + 3 + j] * M_PI / 180.0f)); break; }
							}
						}

					}


					//get the translation of base joint, to combine with the previous rotation data
					//this will be the natural shape of the base bone of IK
					glm::mat4 modelt;
					float trans_x, trans_y, trans_z;

					//to get the offset values of the base's child joint
					//this is also going to affect the IK rotation as it has not been accounted for in IK angle storage
					//get the normalized transformation

					trans_x = activeLayer->GetJoint(indexChain.at(1))->offset[0];
					trans_y = activeLayer->GetJoint(indexChain.at(1))->offset[1];
					trans_z = activeLayer->GetJoint(indexChain.at(1))->offset[2];

					glm::vec3 bone = glm::vec3(trans_x, trans_y, trans_z);
					bone = glm::normalize(bone);
					modelt = glm::translate(bone);

					//get the translation defined by modelt and modelr
					//this is the transformation portion that is ignored by IK arm
					glm::mat4 m = modelt * modelr;

					//need to know what rotation is needed from bvh nature bone direction (extend from prior rotations and the next offset) to 0,0,1 direction of IK
					//rotating one 3d vector (a) to another (b)
					//implemented the following link, tested working.
					//http://math.stackexchange.com/questions/293116/rotating-one-3d-vector-to-another

					glm::vec4 a_initial(0.0f, 0.0f, 0.0f, 1.0f);
					glm::vec3 a((m * a_initial).x, (m * a_initial).y, (m * a_initial).z);
					
					a = glm::normalize(a);
					glm::vec3 b(0.0f, 0.0f, 1.0f);
					glm::vec3 x = glm::normalize(glm::cross(a, b));
					
					float theta = glm::acos(glm::dot(a, b));
					//if pi - theta <= threshold,
					//let x be any vector orthogonal to a
					if (3.1415926 - theta <= 0.05)
						x = glm::cross(a, glm::vec3(0.0, 0.0, 1.0));
					// 0    -x3   x2
					// x3   0     -x1
					// -x2  x1    0
					//store column-wise
					float helper_matrix_items[9] = {0, x.z, -x.y, -x.z, 0, x.x, x.y, -x.x, 0};
					glm::mat3 helper_matrix = glm::make_mat3(helper_matrix_items);
					glm::mat3 transit_rot_matrix = glm::mat3(1.0f) + glm::sin(theta) * helper_matrix + (1 - glm::cos(theta))*(helper_matrix*helper_matrix);
					//if theta <= threshold,
					//let rotation matrix be identity matrix
					if (theta <= 0.05)
						transit_rot_matrix = glm::mat3(1.0f);
					
					glm::vec3 result = glm::normalize(transit_rot_matrix * a);
					bool test_result = result == b;

					float transit_matrix_items[16] = { transit_rot_matrix[0].x, transit_rot_matrix[0].y, transit_rot_matrix[0].z, 0, 
													   transit_rot_matrix[1].x, transit_rot_matrix[1].y, transit_rot_matrix[1].z, 0, 
													   transit_rot_matrix[2].x, transit_rot_matrix[2].y, transit_rot_matrix[2].z, 0, 
													   0, 0, 0, 1 };
					glm::mat4 final_transit_matrix = glm::make_mat4(transit_matrix_items);

					//make translational matrix Tz to let offset equal to (0,0,1)
					glm::mat4 modelTz = glm::translate(glm::vec3(0, 0, 1));
					//modelTz = glm::translate(glm::vec3(trans_x, trans_y, trans_z));

					//get the transitional matrix from the bone natural pose 
					//to IK natural direction (0,0,1)
					//R1-to-R2 = R2 * (R1^-1)

					glm::mat4 model_propagate = modelTz * glm::inverse(m);

					/*
					//short converter from mat4 to array for displaying in log
					double dArray_propagate[16] = { 0.0 };
					const float *pSource_propagate = (const float*)glm::value_ptr(model_propagate);
					for (int i = 0; i < 16; ++i){
						dArray_propagate[i] = pSource_propagate[i];					
					}
					QLOG_INFO() << "LayerController:: model_propagate " << dArray_propagate[0] << dArray_propagate[1] << dArray_propagate[2] << dArray_propagate[3];
					QLOG_INFO() << "LayerController:: model_propagate " << dArray_propagate[4] << dArray_propagate[5] << dArray_propagate[6] << dArray_propagate[7];
					QLOG_INFO() << "LayerController:: model_propagate " << dArray_propagate[8] << dArray_propagate[9] << dArray_propagate[10] << dArray_propagate[11];
					QLOG_INFO() << "LayerController:: model_propagate " << dArray_propagate[12] << dArray_propagate[13] << dArray_propagate[14] << dArray_propagate[15];
					//converter end
					*/

					//glm::mat4 modelTrans = modelTz * glm::inverse(m);
					glm::mat4 modelTrans = final_transit_matrix;
					

					/*
					//short converter from mat4 to array for displaying in log
					double dArray_trans[16] = { 0.0 };
					const float *pSource_trans = (const float*)glm::value_ptr(modelTrans);
					for (int i = 0; i < 16; ++i){
						dArray_trans[i] = pSource_trans[i];
					}
					QLOG_INFO() << "LayerController:: modelTrans " << dArray_trans[0] << dArray_trans[1] << dArray_trans[2] << dArray_trans[3];
					QLOG_INFO() << "LayerController:: modelTrans " << dArray_trans[4] << dArray_trans[5] << dArray_trans[6] << dArray_trans[7];
					QLOG_INFO() << "LayerController:: modelTrans " << dArray_trans[8] << dArray_trans[9] << dArray_trans[10] << dArray_trans[11];
					QLOG_INFO() << "LayerController:: modelTrans " << dArray_trans[12] << dArray_trans[13] << dArray_trans[14] << dArray_trans[15];
					//converter end
					*/

					bool aa = glm::mat4(1.0f) == modelTrans;
					//if (!aa){ model_propagate = glm::mat4(1.0f); }
					//combine Mtrans with IK saved Transformation
					//this will be the final value to be saved
					Eigen::AngleAxisf tmpAngleAxis = segments.at(0)->T;
					glm::mat4 modelSaved = glm::rotate(tmpAngleAxis.angle(), glm::vec3(tmpAngleAxis.axis().x(), tmpAngleAxis.axis().y(), tmpAngleAxis.axis().z()));

					/*
					//short converter from mat4 to array for displaying in log
					double dArray_saved[16] = { 0.0 };
					const float *pSource_saved = (const float*)glm::value_ptr(modelSaved);
					for (int i = 0; i < 16; ++i){
						dArray_saved[i] = pSource_saved[i];
					}
					QLOG_INFO() << "LayerController:: modelSaved " << dArray_saved[0] << dArray_saved[1] << dArray_saved[2] << dArray_saved[3];
					QLOG_INFO() << "LayerController:: modelSaved " << dArray_saved[4] << dArray_saved[5] << dArray_saved[6] << dArray_saved[7];
					QLOG_INFO() << "LayerController:: modelSaved " << dArray_saved[8] << dArray_saved[9] << dArray_saved[10] << dArray_saved[11];
					QLOG_INFO() << "LayerController:: modelSaved " << dArray_saved[12] << dArray_saved[13] << dArray_saved[14] << dArray_saved[15];
					//converter end
					*/

					//glm::mat4 modelCombined = modelSaved*modelTrans;
					glm::mat4 modelCombined = model_propagate * modelSaved * modelTrans;

					/*
					//short converter from mat4 to array for displaying in log
					double dArray_combined[16] = { 0.0 };
					const float *pSource_combined = (const float*)glm::value_ptr(modelCombined);
					for (int i = 0; i < 16; ++i){
						dArray_combined[i] = pSource_combined[i];
					}
					QLOG_INFO() << "LayerController:: modelCombined " << dArray_combined[0] << dArray_combined[1] << dArray_combined[2] << dArray_combined[3];
					QLOG_INFO() << "LayerController:: modelCombined " << dArray_combined[4] << dArray_combined[5] << dArray_combined[6] << dArray_combined[7];
					QLOG_INFO() << "LayerController:: modelCombined " << dArray_combined[8] << dArray_combined[9] << dArray_combined[10] << dArray_combined[11];
					QLOG_INFO() << "LayerController:: modelCombined " << dArray_combined[12] << dArray_combined[13] << dArray_combined[14] << dArray_combined[15];
					//converter end
					*/

					bool bb = glm::mat4(1.0f) == modelSaved;
					bool cc = (modelCombined == modelSaved);
					glm::quat combined = glm::quat_cast(modelCombined);

					//convert glm quat to my own quat for ease of conversion btwn quat and euler angles
					Quat tmpQuaternion = { combined.x, combined.y, combined.z, combined.w };

					//about correct handling of rotation order 
					//refer to Layer.cpp line 468

					EulerAngles tmpEuler;
					tmpJoint = activeLayer->GetJoint(indexChain.at(0));
					//convert it from quaternion to Euler for BVH to use
					switch (tmpJoint->channel_order){
					case XYZ: { tmpEuler = Eul_FromQuat_Degree(tmpQuaternion, EulOrdXYZr); break; }
					case XZY: { tmpEuler = Eul_FromQuat_Degree(tmpQuaternion, EulOrdXZYr); break; }
					case YXZ: { tmpEuler = Eul_FromQuat_Degree(tmpQuaternion, EulOrdYXZr); break; }
					case YZX: { tmpEuler = Eul_FromQuat_Degree(tmpQuaternion, EulOrdYZXr); break; }
					case ZXY: { tmpEuler = Eul_FromQuat_Degree(tmpQuaternion, EulOrdZXYr); break; }
					case ZYX: { tmpEuler = Eul_FromQuat_Degree(tmpQuaternion, EulOrdZYXr); break; }
					}

					activeLayer->sheetAtIndex(activeFrame)->setMotion(indexChain.at(0) * 3 + 3 + 0, tmpEuler.x);
					activeLayer->sheetAtIndex(activeFrame)->setMotion(indexChain.at(0) * 3 + 3 + 1, tmpEuler.y);
					activeLayer->sheetAtIndex(activeFrame)->setMotion(indexChain.at(0) * 3 + 3 + 2, tmpEuler.z);

					segments.erase(segments.begin());
					if (segments.size() == 0){ break; }
					//propagate the whole procedure to the next child,
					//therefore needs to shift the first index in indexChain to prepIndexChain
					int newJointIndex = indexChain.at(0);
					prepIndexChain.push_back(newJointIndex);
					indexChain.erase(indexChain.begin());
					tmpJoint = activeLayer->GetJoint(prepIndexChain.at(0));
					initial_traverse = false;
			}

		}
		
		void LayerController::makeTPose(FrameId fid) {
			m_layerSelector->activeLayer()->makeTPose(fid);
		}

		void LayerController::attachFCContainer(FCContainer* container) {
			m_FCContainer = container;
		}

		void LayerController::updateScroll() {
			m_FCContainer->updateScrollView();
		}

	};
}
