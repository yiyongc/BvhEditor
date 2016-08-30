#pragma once

#include <functional>   
#include <set>		
#include <vector>		


namespace cacani {
	
	namespace data{
		class Layer;
		class Cel;
		class LayerId;
		class LayerGroup;
		class SheetId;
		struct FrameId;
	}
	

	namespace controller  {

		class AbstractLayerSelector
		{
		public:
			virtual cacani::data::LayerId findLayerId(const cacani::data::Layer* al) = 0;
			virtual cacani::data::LayerId findLayerId(const cacani::data::Layer* selectedLayer, const cacani::data::LayerGroup* selectedParent) = 0;

			virtual bool layerIsSelected(const cacani::data::LayerId& lid) = 0;
			virtual bool frameIsSelected(const cacani::data::FrameId fid) = 0;

			virtual void setActiveSheet(const cacani::data::SheetId& sid, bool broadcast = true) = 0;
			virtual void setActiveLayer(const cacani::data::LayerId& lid, bool broadcast = true) = 0;
			//virtual void setActiveFrame(const cacani::data::FrameId fid) = 0;
			virtual void addSelectedLayer(const cacani::data::LayerId& lid) = 0;
			virtual void selectLayerRange(const cacani::data::LayerId& selectedLid) = 0;
			virtual void selectFrameRange(int level) = 0;
			//virtual void popSelectedFrame(cacani::data::FrameId pos) = 0;

			virtual const std::vector<cacani::data::LayerId>& selectedLayersId() = 0;

			virtual cacani::data::Layer* activeLayer() const = 0;
			virtual cacani::data::Layer* activeCel() const = 0;
			virtual const cacani::data::LayerId& activeLayerId() const = 0;

			virtual const cacani::data::SheetId& absoluteActiveSheetId() const = 0;
			//virtual const cacani::data::FrameId& absoluteActiveFrame() const = 0;
			//virtual cacani::data::FrameId activeFrame() const = 0;
			virtual cacani::data::SheetId activeSheetId() const = 0;

			//virtual std::set<cacani::data::FrameId>& selectedFrames() = 0;
			
			virtual void applyToSelectedFrameRanges(const std::function<void(int, int)>& f) = 0;

			virtual cacani::data::Layer* activeLayerRaw() const = 0;
			virtual cacani::data::Cel* activeCelRaw() const = 0;

			virtual void addFrameSelection(const cacani::data::LayerId& lid, int level) = 0;
			//virtual void addSelectedFrame(const cacani::data::FrameId fid) = 0;
			
			virtual void clearSelectedLayers() = 0;
			virtual void clearSelectedFrames() = 0;
		};
	}
}
