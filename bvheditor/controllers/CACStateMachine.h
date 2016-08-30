#pragma once

#include "AbstractCACStateMachine.h"
#include <qwidget.h>
#include "../QsLog2/qslog.h"


namespace cacani {
	namespace state {
		class CACStateMachine : public AbstractCACStateMachine
		{
		public:
			CACStateMachine();
			~CACStateMachine();

			bool isIdleState();
			bool isPlaybackState();
			bool isPlaying();
			bool isEditingState();

			bool setDrawingState(int enterFlag = 0);
			bool setLayerState(int enterFlag = 0);
			bool setIdleState(int enterFlag = 0);
			bool setEditingState(int enterFlag = 0);

			
		private:
			//vector<QWidget*> m_observers;
		};
	}
}
