#pragma once

#include "AbstractLayerType.h"

namespace cacani {
	namespace data {

		class AbstractSheet
		{
		public:
			enum SheetType
			{
				kCelSheet = 0x4506,
				kKeySheet = 0x4501,
				kLinkSheet = 0x4504,
				kMergeSheet = 0x4505,
				kNullSheet = 0x4500,
				kRepeatSheet = 0x4503,
				kTweenSheet = 0x4502,
				kRasterSheet = 0x4507,
				kSketchSheet = 0x4508,
				kDrawingSheet = 0x4509
			};

		public:
			virtual SheetType sheetType() const = 0;
		};

	}
}