#pragma once

#include "Layer.h"


namespace cacani {
	namespace data {

		//virtual inheritance to solve diamond inheritance problem
		class AbstractLayerGroup:public virtual AbstractLayer
		{
		public:
			virtual int memberCount() const = 0;
			virtual Layer* childAtIndex(unsigned int index) const = 0;
			virtual int frameOffset() const { return 0; }
		};

	}
}