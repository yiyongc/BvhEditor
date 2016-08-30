#include "CACStateMachine.h"

namespace cacani {
	namespace state {

		CACStateMachine::CACStateMachine(){}

		CACStateMachine::~CACStateMachine(){}

		bool CACStateMachine::isIdleState() 
		{
			return true;
		}

		bool CACStateMachine::isPlaybackState()
		{
			return false;
		}

		bool CACStateMachine::isPlaying()
		{
			return false;
		}

		bool CACStateMachine::setDrawingState(int enterFlag)
		{
			return false;
		}

		bool CACStateMachine::setLayerState(int enterFlag)
		{
			return false;
		}

		bool CACStateMachine::setIdleState(int enterFlag)
		{
			return false;
		}

		/*
		void CACStateMachine::attach(QWidget* observer) {
			bool dupe;
			for (int i = 0; i < m_observers.size(); i++){
				dupe = (observer == m_observers.at(i));
				if (dupe) return;
			}
			m_observers.push_back(observer);
		}

		void CACStateMachine::detach(QWidget* observer) {
			bool match;

			if (m_observers.size != 0){
				for (int i = 0; i < m_observers.size(); i++){
					match = (observer == m_observers.at(i));
					if (match) {
						m_observers.erase(m_observers.begin() + i);
						break;
					}
				}

			}

			QLOG_INFO() << "CACStateMachine: detach() failed because observer is not yet in";

		}
		*/
	}
}
