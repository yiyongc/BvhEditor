#pragma once

#include "AbstractLayerGroup.h"
#include "Layer.h"
#include "FrameId.h"
#include <vector>
#include "../QsLog2/qslog.h"


namespace cacani {
	namespace data {

		class Layer;
		class FrameId;

		//virtual inheritance to solve diamond inheritance problem
		class LayerGroup : public AbstractLayerGroup, public Layer
		{
		public:
			//constructing simple layergroup with empty content
			//default is a terminal container -- meaning children will all be layers 
			//the only exception is the root layerGroup, where its chilren will all be layergroups

			LayerGroup(const bool terminal = true, LayerGroup* parent = 0);

			//the root layergroup does not hold bvh info, hence this func does not take in bool terminal parameter
			//this project is meant for depth 2 at a max : root layergroup -> layergroup -> layer
			//hence in default it will be terminal layergroup
			//currently, layergroup will take in entire bvh info
			//in future, layer will be a body part of bvh 
			//now for simplicity, layer will be propagated (in default) with the same info as layergroup 
			//TODO: discuss on the skeleton relationship betwn layergroup and layer in the future, do we propage changes? in which directions?

			LayerGroup(const char * bvh_file_name, LayerGroup* parent = 0, const bool propagate = true);

			~LayerGroup(){}


			void addChild();
			// for terminals only
			void addChild(Layer* newLayer);
			// for non terminals only
			void addChild(LayerGroup* newLayerGroup);
			// both for terminals and non terminals
			void addChild(const char * bvh_file_name);

			void setFrameOffset(FrameId new_frameOffset){ m_frameOffset = new_frameOffset; }

			Layer* childAtIndex(unsigned int index) const;

			std::vector<Layer*> getChildren() const{ return m_children; }

			bool isTerminal(){ return m_terminal; }

			int memberCount() const;

			int frameOffset() const { return m_frameOffset; }
			
			void updateProjFrameRate(double frameRate);
			double getProjFrameRate(){ return m_proj_fps; }

			//regarding issues around fps and frame num, etc
			//all refer to the first child, since all children of the same layergroup should have same # of sheets
		    int sheetCount() const { 
				int tmp_cnt = childAtIndex(0)->sheetCount();
				for (int i = 0; i < memberCount(); i++){
					tmp_cnt = min(tmp_cnt, childAtIndex(i)->sheetCount());
				}
				return tmp_cnt;
			}
			//double getLayerOriginalFPS(){ return childAtIndex(0)->getLayerOriginalFPS(); }
			//double getLayerCurrentFPS(){ return childAtIndex(0)->getLayerCurrentFPS(); }
			int     GetNumFrame() const { 
				int tmp_num_frame = childAtIndex(0)->GetNumFrame();
				for (int i = 0; i < memberCount(); i++){
					tmp_num_frame = min(tmp_num_frame, childAtIndex(i)->GetNumFrame());
				}
				return tmp_num_frame;
			}
			//double  GetInterval() const { return  childAtIndex(0)->GetInterval(); }

		private:
			std::vector<Layer*> m_children;
			const bool m_terminal;
			int m_frameOffset;
			//only applicable for root - project fps
			double m_proj_fps;
		};
	}
}