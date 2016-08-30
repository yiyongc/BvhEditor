#pragma once

#include "AbstractSheet.h"
#include <vector>
#include <map>
#include <string>
#include "Layer.h"

using namespace  std;


namespace cacani {
	namespace data {

		struct Joint;
		struct Channel;

		class Sheet : public AbstractSheet 
		{

		public:

			Sheet();

			//Sheet(vector<Channel*> channels, vector<Joint*> joints, double* motion);

			Sheet(int numOfChannels, double* motion);
			~Sheet();

			SheetType sheetType() const;
			void setMotion(int index, double value){ m_motion[index] = value; }
			double getMotion(int index){ return m_motion[index]; }
			double* getMotion(){ return m_motion; }
			Sheet* clone(){ return new Sheet(this->m_numOfChannels, this->m_motion); }

			private:
		//	vector< Channel * >      m_channels;    // 
		//	vector< Joint * >        m_joints;      // 
			
			//motion data: # of numbers --- number of channels
			double*          m_motion;      // 
			int			 m_numOfChannels;

			//xiaocai addded
		private:
			double *m_motionOriginal;
		public:
			const double* getOriginalMotionData() const { return m_motionOriginal; }
		};

	}
}