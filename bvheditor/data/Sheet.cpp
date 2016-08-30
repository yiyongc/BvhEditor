#pragma once
#include "Sheet.h"


namespace cacani {
	namespace data {

		Sheet::Sheet(){}

		Sheet::Sheet(int numOfChannels, double * motion)
		{

		
			//m_joints size ok
			//QLOG_TRACE() << "Sheet: result num_joints = " << m_joints.size();
			m_numOfChannels = numOfChannels;
			m_motion = new double[numOfChannels];

			//xiaocai added
			m_motionOriginal = new double[numOfChannels];

			for (int i = 0; i < numOfChannels; i++)
			{
				m_motion[i] = motion[i];

				//xiaocai added
				m_motionOriginal[i] = motion[i];
			}



			///m_motion size OK = num_channels
			//QLOG_TRACE() << "Sheet: result number of motions = " ;


		}

		Sheet::~Sheet()
		{
			//xiaocai added
			delete[] m_motion; m_motion = NULL;
			delete[] m_motionOriginal; m_motionOriginal = NULL;
		}

		cacani::data::AbstractSheet::SheetType Sheet::sheetType() const
		{
			//TODO
			//this is a random return just for testing, logic to be implemented
			return kCelSheet;
		}

	}
}