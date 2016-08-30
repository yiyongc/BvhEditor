#pragma once

#include "LayerGroup.h"

namespace cacani {
	namespace data {

		LayerGroup::LayerGroup(const bool terminal, LayerGroup* parent) : Layer(parent), m_terminal(terminal){
			vector<Layer*> children;
			m_children = children;
			m_frameOffset = 0;
		};

		LayerGroup::LayerGroup(const char * bvh_file_name, LayerGroup* parent, const bool propagate) : Layer(bvh_file_name, parent), m_terminal(1){

			vector<Layer*> children;
			m_children = children;
			m_frameOffset = 0;
			if (propagate){
				addChild(bvh_file_name);
			}
		};

		void LayerGroup::addChild(){
			if (m_terminal){
				Layer* childLayer = new Layer();
				m_children.push_back(childLayer);
			}
			else{
				LayerGroup* childLayerGroup = new LayerGroup();
				m_children.push_back((Layer*)childLayerGroup);
			}
		}

		void LayerGroup::addChild(Layer* newLayer){

			if (m_terminal)
				m_children.push_back(newLayer);
			else
				QLOG_INFO() << "LayerGroup: addChild failed, the layergroup is not terminal layergroup";
		}

		void LayerGroup::addChild(LayerGroup* newLayerGroup){
			if (!m_terminal)
				m_children.push_back((Layer*)newLayerGroup);
			else
				QLOG_INFO() << "LayerGroup: addChild failed, the layergroup is a terminal layergroup";
		}

		void LayerGroup::addChild(const char * bvh_file_name){
			if (m_terminal){
				Layer* newLayer = new Layer(bvh_file_name);
				addChild(newLayer);
			}
			else{
				LayerGroup* newLayerGroup = new LayerGroup(bvh_file_name);
				addChild(newLayerGroup);
			}
		}

		int LayerGroup::memberCount() const{ return m_children.size(); }


		Layer* LayerGroup::childAtIndex(unsigned int index) const{
		//	QLOG_INFO() << "LayerGroup:" << "childAtIndex() = " << m_BVHGroup.at(index);
			return m_children.at(index);
		}


		void LayerGroup::updateProjFrameRate(double frameRate){
			//only applicable to root layergroup
			if (m_terminal)
				return;
			for (int i = 0; i < memberCount(); i++){
				m_proj_fps = frameRate;
				for (int j = 0; j < dynamic_cast<LayerGroup*>(childAtIndex(i))->memberCount(); j++){
					dynamic_cast<LayerGroup*>(this->childAtIndex(i))->childAtIndex(j)->updateLayerFPS(frameRate);
				}
			}


		}

	}
}