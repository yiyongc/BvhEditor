#ifndef FRAMEID_H
#define FRAMEID_H

namespace cacani {
	namespace data{

		class FrameId{
			private:
				int number;

			public:
				// default frame id is 1
				FrameId(){ number = 1; } 
				FrameId(int number){ this->number = number; }
				operator int() const { return number; }

		};
	}
}

#endif