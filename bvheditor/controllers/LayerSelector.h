#pragma once

#include "AbstractLayerSelector.h"
#include "../data/FrameId.h"
#include "../data/SheetId.h"
#include "../data/LayerId.h"
#include "../QsLog2/qslog.h"
#include <vector>
#include <set>

#define LAYERIDDEPTH 2

using namespace cacani::data;


namespace cacani {
	
	namespace data{
		class LayerId;
		class Cel;
		class Layer;
		class LayerGroup;
		class SheetId;
		struct FrameId;
		class JointTreeListModel;
	}

	namespace ui{
		class SheetCanvas;
		class JointTreeWidget;
		class JointEditorWidget;
	}
	
	namespace controller  {

		class LayerController;

		class LayerSelector : public AbstractLayerSelector
		{
		public:
			LayerSelector(LayerController* layerController, cacani::ui::SheetCanvas* sheetCanvas, cacani::ui::JointEditorWidget* jointEditorWidget, cacani::ui::JointTreeWidget* jointTreeWidget);

			~LayerSelector();

			LayerId findLayerId(const Layer* al);
			LayerId findLayerId(const Layer* selectedLayer, const LayerGroup* selectedParent);

			bool layerIsSelected(const LayerId& lid);	
			bool frameIsSelected(const FrameId fid); 

			void setActiveSheet(const SheetId& sid, bool broadcast = true);
			void setActiveLayer(const LayerId& lid, bool broadcast = true);
			void setActiveFrame(const FrameId fid); 
			void setJointTreeListModel(JointTreeListModel* model);

			void addSelectedLayer(const LayerId& lid);
			void addSelectedFrame(const FrameId fid); 
			void addFrameSelection(const LayerId& lid, int level);

			void popSelectedFrame(FrameId pos);  

			void selectLayerRange(const LayerId& selectedLid);
			void selectFrameRange(int level);

			const vector<LayerId>& selectedLayersId();
			set<FrameId>& selectedFrames();  

			Layer* activeCel() const;
			int activeCelIndex() const;
			int activeCelChildIndex() const;
			Layer* activeLayer() const;
			const LayerId& activeLayerId() const;
			SheetId activeSheetId() const;
			FrameId activeFrame() const;  

			Layer* activeLayerRaw() const;
			Cel* activeCelRaw() const;

			const SheetId& absoluteActiveSheetId() const;
			const FrameId& absoluteActiveFrame() const;  

			void applyToSelectedFrameRanges(const function<void(int, int)>& f);

			void clearSelectedLayers();
			void clearSelectedFrames();

			cacani::ui::SheetCanvas* getSheetCanvas() { return m_sheetCanvas; }

		private:
			LayerController* m_layerController;
			cacani::ui::SheetCanvas* m_sheetCanvas;
			cacani::ui::JointTreeWidget* m_jointTreeWidget;
			cacani::ui::JointEditorWidget* m_jointEditorWidget;

			vector<LayerId> m_selectedLayerIds;
			set<FrameId> m_selectedFrameIds;
			LayerId m_activeLayerId;
			FrameId m_activeFrameId;
			SheetId m_activeSheetId;

			JointTreeListModel* m_JointTreeListModel;
		};
	}
}