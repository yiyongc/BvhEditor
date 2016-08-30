#pragma once 

namespace cacani {

	namespace state {

		class AbstractCACStateMachine
		{
		public:
			virtual bool isIdleState() = 0;
			virtual bool isPlaybackState()=0;
			virtual bool isPlaying()=0;

			virtual bool setDrawingState(int enterFlag = 0) = 0;
			virtual bool setLayerState(int enterFlag = 0) = 0;
			virtual bool setIdleState(int enterFlag = 0) = 0;
		};

	}
}