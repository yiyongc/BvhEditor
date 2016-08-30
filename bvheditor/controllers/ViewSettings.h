#pragma once
#include "AbstractViewSettings.h"

namespace cacani {
	namespace controller {

		class ViewSettings : public AbstractViewSettings
		{
		public:

			ViewSettings();
			~ViewSettings();

			void setActiveCelIndex(int index);
			void setTempCelOffset(int offset);
		};

	}
}