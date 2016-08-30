#pragma once

#include "utf8string.h"  
//1st run//#include "Argb.h"
#include "Sheet.h"

namespace cacani {
	namespace data{

		class Sheet;

		class AbstractLayer
		{
		public:
			virtual bool isOnionSkinOn() const = 0;
			virtual bool isLocked() const = 0;
			virtual bool isVisible() const = 0;
			//To be moved to LayerGroup if this attribute is determined not necessary in Layer
			virtual bool isExpanded() const = 0;

			virtual unsigned int getOpacity() const = 0;
			//virtual const utf8string& getName() const = 0;
			//virtual const Argb& getColor() const = 0;
			//virtual const Argb& getColorSep() const = 0;

			virtual void setLocked(bool locked) = 0;
			virtual void setVisible(bool visible) = 0;
			virtual void setOnionSkinOn(bool on) = 0;
			virtual void setExpanded(bool expanded) = 0;
			virtual void setOpacity(unsigned int opacity) = 0;
			virtual void setName(const utf8string& name) = 0;
			//virtual void setColor(const Argb& color) = 0;

			virtual int sheetCount() const { return 0; }

			virtual Sheet* sheetAtIndex(int index) const = 0;

			virtual unsigned int layerType() const = 0;

		};

	}
}