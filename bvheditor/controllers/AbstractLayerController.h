#pragma once

#include "../data/utf8string.h" 
#include "../data/LayerId.h"
#include "../data/Layer.h"
#include "qobject.h"


using namespace cacani::data; 

namespace cacani {


	namespace controller {

		class AbstractLayerController : public QObject
		{
		public:
			virtual void setLayerOpacity(LayerId& lid, int value){};

			virtual void setLayerExpanded(const LayerId& lid){};
			virtual void setLayerVisible(const LayerId& lid){};
			virtual void setLayerOnion(const LayerId& lid) {};
			virtual void setLayerLocked(const LayerId& lid){};

			virtual void setSelectedLocked(const Layer* selectedLayer){};   //SECOND ATTEMPT// deleted namespaces
			virtual void setSelectedVisible(const Layer* selectedLayer){};  //SECOND ATTEMPT// deleted namespaces
			virtual void setSelectedOnion(const Layer* selectedLayer){};    //SECOND ATTEMPT// deleted namespaces

			//Moving only cels or groups
			virtual void moveSelectedCels(const LayerId& destinationId){};
			virtual void moveSelectedCel(const LayerId& selectedCelId, const LayerId& destinationId){};
			//Moving layers are selected to a destination i.e. under or above a certain layer/group
			virtual void moveSelectedLayers(const LayerId& destinationId){};
			virtual void moveSelectedLayer(const LayerId& selectedId, const LayerId& destinationId){};
			//Moving layers, with the mouse releasing on a group
			virtual void moveSelectedLayersIntoCel(const LayerId& destinationId){};
			virtual void moveSelectedLayerIntoCel(const LayerId& selectedId, const LayerId& destinationId){};

			virtual void setLayerName(const LayerId& lid, const ::cacani::utf8string& name){};

			virtual void addCel() = 0;
			virtual void addDrawingLayer() = 0;
			virtual void duplicateLayer() = 0;
			virtual void duplicateCel() = 0;
			virtual void removeLayers() = 0;
			virtual void removeCels() = 0;
			
			//1st run//virtual void selectSimilarColorType(const Layer* selectedLayer, const LayerId& selectedLid) = 0;
			virtual void selectSimilarColorType(const Layer* selectedLayer, const LayerId& selectedLid) = 0;

			virtual void changeLayerColor(const LayerId& lid, int r, int g, int b, int a) = 0;
			virtual void changeSelectedLayerColor(int r, int g, int b) = 0;

			virtual bool isRepeatSeqSync(const LayerId& lid, const LayerId& destId){ return true; }
			virtual bool isLayerSeqSync(const LayerId& lid, const LayerId& destId) { return true; }

			virtual void moveFrameSeq(const LayerId& celLid, int layerIndex, int movingFrame, int destFrame) = 0;

			virtual void setCelOffset(const LayerId& lid, int offset)= 0;   //SECOND ATTEMPT// deleted namespaces
			virtual bool anyAvailableSheet() = 0;
			
			virtual void appendKeyFrameToActiveCel(int count) =0;
			virtual void appendRepeatFrameToActiveCel(int count) = 0;
			virtual void prependKeyFrameToActiveCel(int count) = 0;
			virtual void insertKeyFrames(const LayerId& celLid, int layerIndex, int frameNo, int count) = 0;
			virtual void insertRepeatFrames(const LayerId& celLid, int layerIndex, int frameNo, int count) = 0;
			virtual void dupFrameIntoSeq(const LayerId& celLid, int layerIndex, int frameNo, int count) = 0;
			virtual void dupSeqToLoop(const LayerId& celLid, int layerIndex, int frameNo, int count, bool convert = false, bool key = false)= 0;
			virtual void dupSeqToPingRev(const LayerId& celLid, int layerIndex, int frameNo, int count, bool convert = false, bool key = false)= 0 ;
			virtual void dupSeqToPingPong(const LayerId& celLid, int layerIndex, int frameNo, int count, bool convert = false, bool key = false) = 0;

			virtual void removeFrameSeq(const LayerId& celLid, int layerIndex, int frame, int count) =0;
			virtual void removeSelectedFrames() = 0;
		};


	}
}