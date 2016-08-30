#pragma once

namespace cacani {
	namespace controller {

		class AbstractViewSettings
		{
		public:
			virtual void setActiveCelIndex(int index) = 0;
			virtual void setTempCelOffset(int offset) = 0;
		};

	}
}