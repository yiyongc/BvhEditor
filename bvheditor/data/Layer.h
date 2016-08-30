#pragma once

#include "AbstractLayer.h"
#include "utf8string.h"
#include "../QsLog2/qslog.h"

#include <vector>
#include <map>
#include <string>
#include "Sheet.h"
#include <fstream>
#include "../data/EulerAngles.h"
#include <QQuaternion>
#include "FrameId.h"


using namespace  std;


namespace cacani {
	namespace data{

		class LayerGroup;
		class Sheet;
		class FrameId;

		enum  ChannelEnum
		{
			Xrotation, Yrotation, Zrotation,
			Xposition, Yposition, Zposition
		};

		enum ChannelOrder
		{
			XYZ, XZY, YXZ, YZX, ZXY, ZYX
		};

		struct  Joint;

		struct  Channel
		{
			Joint *              joint;
			ChannelEnum          type; 
			int                  index;
		};

		struct  Joint
		{
			string               name;
			int                  index;
			Joint *              parent;
			vector< Joint * >    children;
			double               offset[3];
			bool                 has_site;
			double               site[3];
			vector< Channel * >  channels;
			bool				 is_selected;
			ChannelOrder		 channel_order;

			//xiaocai added
			double               offsetOriginal[3];
			double               siteOriginal[3];
		};


		//to solve diamond inheritance problem
		class Layer : virtual public AbstractLayer
		{
		public:

			Layer(LayerGroup* parent = 0);
			Layer(const char * bvh_file_name, LayerGroup* parent = 0);
			~Layer();

			//GetFullFileName means the full file directory (including file name and extension)
			//GetShortFileName means only file name (without dir or file extension)
			//USE motion name as the unique identifier -- determine criteria for == operator

			const string GetFullFileName() const { return file_name; }
			const string GetShortFileName() const;
			const string &  GetMotionName() const { return motion_name; }
			const int       GetNumJoint() const { return  joints.size(); }
			Joint *   GetJoint(int no) const { return  joints[no]; }
			const int       GetNumChannel() const { return  channels.size(); }
			const Channel * GetChannel(int no) const { return  channels[no]; }
			Joint *   GetJoint(const string & j) const;
			Joint *   GetJoint(const char * j) const;

			//double  GetMotion(int frame, int channel) const { return  motion[frame*num_channel + channel]; }

			//void  SetMotion(int frame, int channel, double value) { motion[frame*num_channel + channel] = value; }
			///newly added for testing
			const void  SetFileName(string filename_in) { file_name = filename_in; }



			void getBVHDataFromFile(const char * bvh_file_name);
			void exportBVHtoDir(QString targetDir);
			void printJoint(Joint* curJoint, ofstream& outfile, int level);
			string getChannelName(ChannelEnum channel);


			void changeChannelOrder(Joint* joint, ChannelOrder order);

			bool isOnionSkinOn() const;
			bool isLocked() const;
			bool isVisible() const;
			//To be moved to LayerGroup if this attribute is determined not necessary in Layer
			bool isExpanded() const;

			unsigned int getOpacity() const;
			const utf8string& getName() const;
			//const Argb& getColor();
			//const Argb& getColorSep();

			void setLocked(bool locked);
			void setVisible(bool visible);
			void setOnionSkinOn(bool on);
			void setExpanded(bool expanded);
			void setOpacity(unsigned int opacity);
			void setName(const utf8string& name);
			//void setColor(const Argb& color);

			int sheetCount() const { return m_sheets.size(); }
			Sheet* sheetAtIndex(int index) const;

			unsigned int layerType() const;

			bool checkIfVisible(){ return is_visible; }
			void setVisible(){ is_visible = 1; }
			void disableVisible(){ is_visible = 0; }
			void toggleVisible(){ is_visible = !is_visible; }

			void selectJoint(int index);
			void selectJoint(Joint* targetJoint);

			bool check_load_success() const { return is_load_success; }
			void set_load_success(bool success) { is_load_success = success; }

			//USE motion name as the unique identifier
			bool operator == (const Layer& rhs) const { return motion_name  == rhs.GetMotionName(); }

			void removeSheets(int index, int count);

			double getLayerOriginalFPS(){ return original_frame_rate; }
			double getLayerCurrentFPS(){ return current_frame_rate; }
			int     GetNumFrame() const { return  current_num_frame; }

			void updateLayerFPS(double newFrameRate);
			void createSheets(double newFrameRate);
			//this will be only called after checking fps suitability
			void createSheets(double** motion);

			//duplicate 1 frame and place it next to the duplicated frame
			void duplicateSheet(FrameId fid);
			void deleteSheet(FrameId fid);
			int gcd(int a, int b);
			int lcm(int a, int b);
			double** interpolate(int interpolate_interval);
			double** drop_frames(double** motion, int drop_interval);
			double* Layer::linspace(double a, double b, int N);

			//specifically for ik in-between generation: only for generating interpolated frames between 2 consecutive frames
			void inbetween(FrameId fid, int interpolate_interval);
			void addFrames(FrameId fid, int numFrames);
			void makeTPose(FrameId fid);
			void groundModel(FrameId fid);

			//private:
			friend ostream& operator<<(std::ostream& out, const ChannelEnum value)
			{
				const char* s = 0;

#define PROCESS_VAL(p) case(p): s = #p; break;

				switch (value){
					PROCESS_VAL(Xrotation);
					PROCESS_VAL(Yrotation);
					PROCESS_VAL(Zrotation);
					PROCESS_VAL(Xposition);
					PROCESS_VAL(Yposition);
					PROCESS_VAL(Zposition);
				}

#undef PROCESS_VAL

				return out << s;

			}

			void  Clear();

			//protected:

			bool                     is_load_success;
			string                   file_name;
			string                   motion_name;
			int                      num_channel;
			vector< Channel * >      channels;
			vector< Joint * >        joints;
			map< string, Joint * >   joint_index;
			int                      original_num_frame;
			int						 current_num_frame;
			double                   original_interval;
			double					 current_interval;
			double				     original_frame_rate;
			double					 current_frame_rate;
			//only original motion is stored as part of layer
			//fps converted motion is directly stored in sheets
			double**	             original_motion;

			//For current statuses
			bool is_visible;

			vector<Sheet*> m_sheets;
			LayerGroup* m_parent;

			//begin: xiaocai added
		protected:
			double LLegLengthOriginal;
			double RLegLengthOriginal;

			double LLegLengthChange;
			double RLegLengthChange;

		public:
			double GetOriginalLLegLength() const { return LLegLengthOriginal; }
			double GetOriginalRLegLength() const { return RLegLengthOriginal; }

			double ComputeCurrentLLegLength();
			double ComputeCurrentRLegLength();

			double GetLLegLengthChange() const { return LLegLengthChange; }
			double GetRLegLengthChange() const { return RLegLengthChange; }

			void UpdateLLegLengthChange();
			void UpdateRLegLengthChange();

			double HierarchyLength(Joint* joint);

		};
	}
}