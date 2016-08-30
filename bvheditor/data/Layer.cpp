#pragma once

#include "Layer.h"
#include <fstream>
#include <string.h>



namespace cacani {
	namespace data{

		//for loading
		void Layer::getBVHDataFromFile(const char * bvh_file_name){

			QLOG_TRACE() << "Layer:" << "addBVHLayerFromFile() = " << "entered";

#define  BUFFER_LENGTH  1024*4

			ifstream  file;
			char      line[BUFFER_LENGTH];
			char *    token;
			char      separater[] = " :,\t";
			vector< Joint * >   joint_stack;
			Joint *   joint = NULL;
			Joint *   new_joint = NULL;
			bool      is_site = false;
			double    x, y, z;
			int       i, j;

			Clear();

			file_name = bvh_file_name;
			const char *  mn_first = bvh_file_name;
			const char *  mn_last = bvh_file_name + strlen(bvh_file_name);
			if (strrchr(bvh_file_name, '\\') != NULL)
				mn_first = strrchr(bvh_file_name, '\\') + 1;
			else if (strrchr(bvh_file_name, '/') != NULL)
				mn_first = strrchr(bvh_file_name, '/') + 1;
			if (strrchr(bvh_file_name, '.') != NULL)
				mn_last = strrchr(bvh_file_name, '.');
			if (mn_last < mn_first)
				mn_last = bvh_file_name + strlen(bvh_file_name);
			motion_name.assign(mn_first, mn_last);

			file.open(bvh_file_name, ios::in);
			if (file.is_open() == 0)  return; 
			
			while (!file.eof())
			{
				 
				if (file.eof())  goto bvh_error;
				
				file.getline(line, BUFFER_LENGTH);
				token = strtok(line, separater);
 
				if (token == NULL)  continue;

				if (strcmp(token, "{") == 0)
				{
					joint_stack.push_back(joint);
					joint = new_joint;
					continue;
				}
				 
				if (strcmp(token, "}") == 0)
				{
					 
					joint = joint_stack.back();
					joint_stack.pop_back();
					is_site = false;
					continue;
				}

				 
				if ((strcmp(token, "ROOT") == 0) ||
					(strcmp(token, "JOINT") == 0))
				{
					
					new_joint = new Joint();
					new_joint->index = joints.size();
					new_joint->parent = joint;
					new_joint->has_site = false;
					new_joint->is_selected = false;
					new_joint->offset[0] = 0.0;  new_joint->offset[1] = 0.0;  new_joint->offset[2] = 0.0;
					new_joint->site[0] = 0.0;  new_joint->site[1] = 0.0;  new_joint->site[2] = 0.0;
					joints.push_back(new_joint);
					if (joint)
						joint->children.push_back(new_joint);

					
					token = strtok(NULL, "");
					while (*token == ' ')  token++;
					new_joint->name = token;
					
					joint_index[new_joint->name] = new_joint;
					continue;
				}

				
				if ((strcmp(token, "End") == 0))
				{
					new_joint = joint;
					is_site = true;
					continue;
				}

				
				if (strcmp(token, "OFFSET") == 0)
				{
					 
					token = strtok(NULL, separater);
					x = token ? atof(token) : 0.0;
					token = strtok(NULL, separater);
					y = token ? atof(token) : 0.0;
					token = strtok(NULL, separater);
					z = token ? atof(token) : 0.0;

					 
					if (is_site)
					{
						joint->has_site = true;
						joint->site[0] = x;
						joint->site[1] = y;
						joint->site[2] = z;

						//xiaocai added
						joint->siteOriginal[0] = x;
						joint->siteOriginal[1] = y;
						joint->siteOriginal[2] = z;
					}
					else
						
					{
						joint->offset[0] = x;
						joint->offset[1] = y;
						joint->offset[2] = z;

						joint->offsetOriginal[0] = x;
						joint->offsetOriginal[1] = y;
						joint->offsetOriginal[2] = z;
					}
					continue;
				}

				
				if (strcmp(token, "CHANNELS") == 0)
				{
					
					token = strtok(NULL, separater);
					joint->channels.resize(token ? atoi(token) : 0);

					// 
					string tmp;
					for (i = 0; i<joint->channels.size(); i++)
					{
						//
						Channel *  channel = new Channel();
						channel->joint = joint;
						channel->index = channels.size();
						channels.push_back(channel);
						joint->channels[i] = channel;

						//
						token = strtok(NULL, separater);
						if (strcmp(token, "Xrotation") == 0){
							channel->type = Xrotation;
							tmp.append("X");
						}
						else if (strcmp(token, "Yrotation") == 0){
							channel->type = Yrotation;
							tmp.append("Y");
						}
						else if (strcmp(token, "Zrotation") == 0){
							channel->type = Zrotation;
							tmp.append("Z");
						}
						else if (strcmp(token, "Xposition") == 0)
							channel->type = Xposition;
						else if (strcmp(token, "Yposition") == 0)
							channel->type = Yposition;
						else if (strcmp(token, "Zposition") == 0)
							channel->type = Zposition;
					}
					if (tmp == "XYZ") joint->channel_order = XYZ;
					else if (tmp == "XZY") joint->channel_order = XZY;
					else if (tmp == "YXZ") joint->channel_order = YXZ;
					else if (tmp == "YZX") joint->channel_order = YZX;
					else if (tmp == "ZXY") joint->channel_order = ZXY;
					else if (tmp == "ZYX") joint->channel_order = ZYX;

				}

				// Motion
				if (strcmp(token, "MOTION") == 0)
					break;
			}


			// 
			file.getline(line, BUFFER_LENGTH);
			token = strtok(line, separater);
			if (strcmp(token, "Frames") != 0)  goto bvh_error;
			token = strtok(NULL, separater);
			if (token == NULL)  goto bvh_error;
			original_num_frame = atoi(token);
			current_num_frame = original_num_frame;

			file.getline(line, BUFFER_LENGTH);
			token = strtok(line, ":");
			if (strcmp(token, "Frame Time") != 0)  goto bvh_error;
			token = strtok(NULL, separater);
			if (token == NULL)  goto bvh_error;
			original_interval = atof(token);
			current_interval = original_interval;

			//get inverse number of interval to get frame rate
			//then round it to 1 decimal point
			original_frame_rate = ((double)((int)((1 / original_interval) * 10 + 0.5))) / 10;
			current_frame_rate = original_frame_rate;

			num_channel = channels.size();

			//CREATING MOTION --- 2D ARRAY


			original_motion = new double*[original_num_frame];
			for (int i = 0; i<original_num_frame; i++){
				original_motion[i] = new double[num_channel];

				file.getline(line, BUFFER_LENGTH);
				token = strtok(line, separater);
				for (j = 0; j<num_channel; j++)
				{
					if (token == NULL)
						goto bvh_error;
					original_motion[i][j] = atof(token);
					token = strtok(NULL, separater);
				}
			}

			file.close();
			is_load_success = true;
			
bvh_error:
			file.close();

		}

		void Layer::updateLayerFPS(double newFrameRate){

			if (!m_sheets.size()){
				//this is a newly created layer -- no sheets yet
				//create sheets based on current or origina  l rate make no difference
				//need to further judge if resampled motion or original motion is needed
				createSheets(newFrameRate);
				return;

			}else if (newFrameRate == current_frame_rate){
				//current is same as the updated rate --- need no action
				return;

			}else{

				m_sheets.clear();
				//create sheets based on current rate and resampled motion
				createSheets(newFrameRate);
			}

		}

		void Layer::createSheets(double newFrameRate){

			//if clause: conventional creation of sheets -- no change of fps 
			if (original_frame_rate == newFrameRate){
				//createsheets based on original motion (rate is original rate)
				current_frame_rate = original_frame_rate;
				current_num_frame = original_num_frame;
				current_interval = original_interval;
				createSheets(original_motion);

			}
			else{

				//else clause: smart resampling of bvh
				//and creating sheets according to resampled motion

				int factor_original = 1;
				int factor_new = 1;
				int original_fps_int ;
				int new_fps_int;
				double tmp_original_fps = original_frame_rate;
				double tmp_new_fps = newFrameRate;

				//if not already an int, convert to int 
				//and save the factor
				while (floor(tmp_original_fps) != tmp_original_fps){
					factor_original *= 10;
					tmp_original_fps *= 10;
				}

				while (floor(tmp_new_fps) != tmp_new_fps){
					factor_new *= 10;
					tmp_new_fps *= 10;
				}

				original_fps_int = (int)tmp_original_fps;
				new_fps_int = (int)tmp_new_fps;

				int factor_gcd = gcd(factor_new, factor_original);
				int fps_lcm = lcm(original_fps_int, new_fps_int);

				int interpolate_interval = (fps_lcm / original_fps_int) *(factor_original / factor_gcd);
				int drop_interval = (fps_lcm / new_fps_int) *(factor_new / factor_gcd);

				current_frame_rate = newFrameRate;
				current_num_frame = original_num_frame*interpolate_interval / drop_interval;
				current_interval = 1 / newFrameRate;

				double** motion_interpolate_result = interpolate(interpolate_interval);
				int res_interpolate = 0;
				for (int i = 0; i < original_num_frame*interpolate_interval; i++){
					res_interpolate = motion_interpolate_result[i][0];
				}
				double** motion_drop_result = drop_frames(motion_interpolate_result, drop_interval);
				int res_drop = 0;
				for (int i = 0; i < current_num_frame; i++){
					res_drop = motion_drop_result[i][0];
				}
				createSheets(motion_drop_result);
			}

		}

		//fid is current fid, which is the newly duplicated frame for ik purpose
		void Layer::inbetween(FrameId fid, int num_inbetween){

			int total_frames = num_inbetween + 2;
			int interpolate = num_inbetween + 1;

			double ** motion_to_interpolate = new double*[total_frames];
			for (int i = 0; i < total_frames; i++){
				motion_to_interpolate[i] = new double[num_channel];
			}

			//except the last frame, coz last frame does not have next one to interpolate with
			//for position only
			//copy interpolation result col by col (channel by channel)
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < interpolate; j++){
					motion_to_interpolate[j][i] = (linspace(m_sheets.at(fid - 1)->getMotion()[i], m_sheets.at(fid)->getMotion()[i], interpolate))[j];
				}
			}

			//except last frame
			//copy interpolation result col by col (every complete rotation as a group. i.e. 3 channels)
			//convert to quaternions -> interpolate -> convert back
			//for (int i = 0; i < original_num_frame - 1; i++){
			for (int j = 0; j < joints.size(); j++){
				EulerAngles cur_eul, next_eul;
				Quat cur_quat, next_quat;
				//QQuaternion * quat_to_interpolate = new QQuaternion[interpolate_interval];
				//std::vector<Quat> quat_interpolate_res;
				switch (joints.at(j)->channel_order){
				case XYZ:{
					//additional 3 to offset the first 3 position channels
					cur_eul = Eul_(m_sheets.at(fid - 1)->getMotion()[3 + j * 3 + 2], m_sheets.at(fid - 1)->getMotion()[3 + j * 3 + 1], m_sheets.at(fid - 1)->getMotion()[3 + j * 3], EulOrdXYZr);
					next_eul = Eul_(m_sheets.at(fid)->getMotion()[3 + j * 3 + 2], m_sheets.at(fid)->getMotion()[3 + j * 3 + 1], m_sheets.at(fid)->getMotion()[3 + j * 3], EulOrdXYZr);
					cur_quat = Eul_ToQuat_Degree(cur_eul);
					next_quat = Eul_ToQuat_Degree(next_eul);
					QQuaternion cur_q_quat(cur_quat.w, cur_quat.x, cur_quat.y, cur_quat.z);
					QQuaternion next_q_quat(next_quat.w, next_quat.x, next_quat.y, next_quat.z);
					for (int k = 0; k < interpolate; k++){

						//double dot_product = cur_q_quat.scalar()*next_q_quat.scalar() + cur_q_quat.x()*next_q_quat.x() + cur_q_quat.y()*next_q_quat.y() + cur_q_quat.z()*next_q_quat.z();
						//if (dot_product < 0)
						//	cur_q_quat = -cur_q_quat;

						QQuaternion interpolated = QQuaternion::slerp(cur_q_quat, next_q_quat, (float)k / (float)interpolate);
						Quat tmp;
						tmp.w = interpolated.scalar();
						tmp.x = interpolated.x();
						tmp.y = interpolated.y();
						tmp.z = interpolated.z();

						//quat_interpolate_res.push_back(tmp);
						EulerAngles res = Eul_FromQuat_Degree(tmp, EulOrdXYZr);
						motion_to_interpolate[k][3 + j * 3] = res.z;
						motion_to_interpolate[k][3 + j * 3 + 1] = res.y;
						motion_to_interpolate[k][3 + j * 3 + 2] = res.x;
					}

					break; //optional
						 }

				case XZY:{
					//additional 3 to offset the first 3 position channels
					cur_eul = Eul_(m_sheets.at(fid - 1)->getMotion()[3 + j * 3 + 2], m_sheets.at(fid - 1)->getMotion()[3 + j * 3 + 1], m_sheets.at(fid - 1)->getMotion()[3 + j * 3], EulOrdXZYr);
					next_eul = Eul_(m_sheets.at(fid)->getMotion()[3 + j * 3 + 2], m_sheets.at(fid)->getMotion()[3 + j * 3 + 1], m_sheets.at(fid)->getMotion()[3 + j * 3], EulOrdXZYr);
					cur_quat = Eul_ToQuat_Degree(cur_eul);
					next_quat = Eul_ToQuat_Degree(next_eul);
					QQuaternion cur_q_quat(cur_quat.w, cur_quat.x, cur_quat.y, cur_quat.z);
					QQuaternion next_q_quat(next_quat.w, next_quat.x, next_quat.y, next_quat.z);
					for (int k = 0; k < interpolate; k++){

						//double dot_product = cur_q_quat.scalar()*next_q_quat.scalar() + cur_q_quat.x()*next_q_quat.x() + cur_q_quat.y()*next_q_quat.y() + cur_q_quat.z()*next_q_quat.z();
						//if (dot_product < 0)
						//	cur_q_quat = -cur_q_quat;

						QQuaternion interpolated = QQuaternion::slerp(cur_q_quat, next_q_quat, (float)k / (float)interpolate);
						Quat tmp;
						tmp.w = interpolated.scalar();
						tmp.x = interpolated.x();
						tmp.y = interpolated.y();
						tmp.z = interpolated.z();

						//quat_interpolate_res.push_back(tmp);
						EulerAngles res = Eul_FromQuat_Degree(tmp, EulOrdXZYr);
						motion_to_interpolate[k][3 + j * 3] = res.z;
						motion_to_interpolate[k][3 + j * 3 + 1] = res.y;
						motion_to_interpolate[k][3 + j * 3 + 2] = res.x;
					}

					break; //optional
						 }
				case YXZ:{
					//additional 3 to offset the first 3 position channels
					cur_eul = Eul_(m_sheets.at(fid - 1)->getMotion()[3 + j * 3 + 2], m_sheets.at(fid - 1)->getMotion()[3 + j * 3 + 1], m_sheets.at(fid - 1)->getMotion()[3 + j * 3], EulOrdYXZr);
					next_eul = Eul_(m_sheets.at(fid)->getMotion()[3 + j * 3 + 2], m_sheets.at(fid)->getMotion()[3 + j * 3 + 1], m_sheets.at(fid)->getMotion()[3 + j * 3], EulOrdYXZr);
					cur_quat = Eul_ToQuat_Degree(cur_eul);
					next_quat = Eul_ToQuat_Degree(next_eul);
					QQuaternion cur_q_quat(cur_quat.w, cur_quat.x, cur_quat.y, cur_quat.z);
					QQuaternion next_q_quat(next_quat.w, next_quat.x, next_quat.y, next_quat.z);
					for (int k = 0; k < interpolate; k++){

						//double dot_product = cur_q_quat.scalar()*next_q_quat.scalar() + cur_q_quat.x()*next_q_quat.x() + cur_q_quat.y()*next_q_quat.y() + cur_q_quat.z()*next_q_quat.z();
						//if (dot_product < 0)
						//	cur_q_quat = -cur_q_quat;

						QQuaternion interpolated = QQuaternion::slerp(cur_q_quat, next_q_quat, (float)k / (float)interpolate);
						Quat tmp;
						tmp.w = interpolated.scalar();
						tmp.x = interpolated.x();
						tmp.y = interpolated.y();
						tmp.z = interpolated.z();

						//quat_interpolate_res.push_back(tmp);
						EulerAngles res = Eul_FromQuat_Degree(tmp, EulOrdYXZr);
						motion_to_interpolate[k][3 + j * 3] = res.z;
						motion_to_interpolate[k][3 + j * 3 + 1] = res.y;
						motion_to_interpolate[k][3 + j * 3 + 2] = res.x;
					}

					break; //optional
						 }
				case YZX:{
					//additional 3 to offset the first 3 position channels
					cur_eul = Eul_(m_sheets.at(fid - 1)->getMotion()[3 + j * 3 + 2], m_sheets.at(fid - 1)->getMotion()[3 + j * 3 + 1], m_sheets.at(fid - 1)->getMotion()[3 + j * 3], EulOrdYZXr);
					next_eul = Eul_(m_sheets.at(fid)->getMotion()[3 + j * 3 + 2], m_sheets.at(fid)->getMotion()[3 + j * 3 + 1], m_sheets.at(fid)->getMotion()[3 + j * 3], EulOrdYZXr);
					cur_quat = Eul_ToQuat_Degree(cur_eul);
					next_quat = Eul_ToQuat_Degree(next_eul);
					QQuaternion cur_q_quat(cur_quat.w, cur_quat.x, cur_quat.y, cur_quat.z);
					QQuaternion next_q_quat(next_quat.w, next_quat.x, next_quat.y, next_quat.z);
					for (int k = 0; k < interpolate; k++){

						//double dot_product = cur_q_quat.scalar()*next_q_quat.scalar() + cur_q_quat.x()*next_q_quat.x() + cur_q_quat.y()*next_q_quat.y() + cur_q_quat.z()*next_q_quat.z();
						//if (dot_product < 0)
						//	cur_q_quat = -cur_q_quat;

						QQuaternion interpolated = QQuaternion::slerp(cur_q_quat, next_q_quat, (float)k / (float)interpolate);
						Quat tmp;
						tmp.w = interpolated.scalar();
						tmp.x = interpolated.x();
						tmp.y = interpolated.y();
						tmp.z = interpolated.z();

						//quat_interpolate_res.push_back(tmp);
						EulerAngles res = Eul_FromQuat_Degree(tmp, EulOrdYZXr);
						motion_to_interpolate[k][3 + j * 3] = res.z;
						motion_to_interpolate[k][3 + j * 3 + 1] = res.y;
						motion_to_interpolate[k][3 + j * 3 + 2] = res.x;
					}

					break; //optional
						 }
				case ZXY:{
					//additional 3 to offset the first 3 position channels
					cur_eul = Eul_(m_sheets.at(fid - 1)->getMotion()[3 + j * 3 + 2], m_sheets.at(fid - 1)->getMotion()[3 + j * 3 + 1], m_sheets.at(fid - 1)->getMotion()[3 + j * 3], EulOrdZXYr);
					next_eul = Eul_(m_sheets.at(fid)->getMotion()[3 + j * 3 + 2], m_sheets.at(fid)->getMotion()[3 + j * 3 + 1], m_sheets.at(fid)->getMotion()[3 + j * 3], EulOrdZXYr);
					cur_quat = Eul_ToQuat_Degree(cur_eul);
					next_quat = Eul_ToQuat_Degree(next_eul);
					QQuaternion cur_q_quat(cur_quat.w, cur_quat.x, cur_quat.y, cur_quat.z);
					QQuaternion next_q_quat(next_quat.w, next_quat.x, next_quat.y, next_quat.z);
					for (int k = 0; k < interpolate; k++){

						//double dot_product = cur_q_quat.scalar()*next_q_quat.scalar() + cur_q_quat.x()*next_q_quat.x() + cur_q_quat.y()*next_q_quat.y() + cur_q_quat.z()*next_q_quat.z();
						//if (dot_product < 0)
						//	cur_q_quat = -cur_q_quat;

						QQuaternion interpolated = QQuaternion::slerp(cur_q_quat, next_q_quat, (float)k / (float)interpolate);
						Quat tmp;
						tmp.w = interpolated.scalar();
						tmp.x = interpolated.x();
						tmp.y = interpolated.y();
						tmp.z = interpolated.z();

						//quat_interpolate_res.push_back(tmp);
						EulerAngles res = Eul_FromQuat_Degree(tmp, EulOrdZXYr);
						motion_to_interpolate[k][3 + j * 3] = res.z;
						motion_to_interpolate[k][3 + j * 3 + 1] = res.y;
						motion_to_interpolate[k][3 + j * 3 + 2] = res.x;
					}

					break; //optional
						 }
				case ZYX:{
					//additional 3 to offset the first 3 position channels
					cur_eul = Eul_(m_sheets.at(fid - 1)->getMotion()[3 + j * 3 + 2], m_sheets.at(fid - 1)->getMotion()[3 + j * 3 + 1], m_sheets.at(fid - 1)->getMotion()[3 + j * 3], EulOrdZYXr);
					next_eul = Eul_(m_sheets.at(fid)->getMotion()[3 + j * 3 + 2], m_sheets.at(fid)->getMotion()[3 + j * 3 + 1], m_sheets.at(fid)->getMotion()[3 + j * 3], EulOrdZYXr);
					cur_quat = Eul_ToQuat_Degree(cur_eul);
					next_quat = Eul_ToQuat_Degree(next_eul);
					QQuaternion cur_q_quat(cur_quat.w, cur_quat.x, cur_quat.y, cur_quat.z);
					QQuaternion next_q_quat(next_quat.w, next_quat.x, next_quat.y, next_quat.z);
					for (int k = 0; k < interpolate; k++){

						//double dot_product = cur_q_quat.scalar()*next_q_quat.scalar() + cur_q_quat.x()*next_q_quat.x() + cur_q_quat.y()*next_q_quat.y() + cur_q_quat.z()*next_q_quat.z();
						//if (dot_product < 0)
						//	cur_q_quat = -cur_q_quat;

						QQuaternion interpolated = QQuaternion::slerp(cur_q_quat, next_q_quat, (float)k / (float)interpolate);
						Quat tmp;
						tmp.w = interpolated.scalar();
						tmp.x = interpolated.x();
						tmp.y = interpolated.y();
						tmp.z = interpolated.z();

						//quat_interpolate_res.push_back(tmp);
						EulerAngles res = Eul_FromQuat_Degree(tmp, EulOrdZYXr);
						motion_to_interpolate[k][3 + j * 3] = res.z;
						motion_to_interpolate[k][3 + j * 3 + 1] = res.y;
						motion_to_interpolate[k][3 + j * 3 + 2] = res.x;
					}

					break; //optional
						 }
				default: //Optional
					break;
				}
			}
			//}

			//for last frame's interpolation -> just copy for the last frame
			for (int j = 0; j < channels.size(); j++){
				motion_to_interpolate[total_frames-1][j] = m_sheets.at(fid)->getMotion()[j];
			}

			//next is to really create those sheets and insert into the layer
			for (int i = 1; i < total_frames - 1; i++){
				Sheet* interpolate_sheet = new Sheet(channels.size(), motion_to_interpolate[i]);
				m_sheets.insert(m_sheets.begin() + fid - 1 + i, interpolate_sheet);
			}

			current_num_frame += total_frames - 2;
			//return motion_to_interpolate;
		}

		void Layer::addFrames(FrameId fid, int numFrames) {
			double* motionToAdd = new double[num_channel];
			
			//Create empty motion array
			for (int i = 0; i < num_channel; i++) {
				motionToAdd[i] = 0;
			}

			//Update minimum height
			double height;
			height = HierarchyLength(joints.at(0));
			motionToAdd[1] = height;

			//Add sheets
			for (int j = 0; j < numFrames; j++) {
				Sheet* newSheet = new Sheet(num_channel, motionToAdd);
				m_sheets.insert(m_sheets.begin() + fid + j, newSheet);
			}

			current_num_frame += numFrames;
		}

		void Layer::makeTPose(FrameId fid) {

			double* motionEdit = m_sheets.at(fid)->getMotion();
			for (int i = 0; i < num_channel; i++)
				motionEdit[i] = 0;
			groundModel(fid);
		}

		void Layer::groundModel(FrameId fid) {
			double* motionEdit = m_sheets.at(fid)->getMotion();
			motionEdit[1] = motionEdit[1] = HierarchyLength(joints.at(0));
		}

		double** Layer::interpolate(int interpolate_interval){

			int tmp_num_frames = original_num_frame*interpolate_interval;
			double ** motion_to_interpolate = new double*[tmp_num_frames];
			for (int i = 0; i < tmp_num_frames; i++){
				motion_to_interpolate[i] = new double[num_channel];
			}

			//except the last frame, coz last frame does not have next one to interpolate with
			//for position only
			//copy interpolation result col by col (channel by channel)
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < original_num_frame - 1; j++)
					for (int k = 0; k < interpolate_interval; k++){
						motion_to_interpolate[j*interpolate_interval + k][i] = (linspace(original_motion[j][i], original_motion[j + 1][i], interpolate_interval))[k];
					}
			}

			//except last frame
			//copy interpolation result col by col (every complete rotation as a group. i.e. 3 channels)
			//convert to quaternions -> interpolate -> convert back
			for (int i = 0; i < original_num_frame - 1; i++){
				for (int j = 0; j < joints.size(); j++){
					EulerAngles cur_eul, next_eul;
					Quat cur_quat, next_quat;
					//QQuaternion * quat_to_interpolate = new QQuaternion[interpolate_interval];
					//std::vector<Quat> quat_interpolate_res;
					switch (joints.at(j)->channel_order){
					case XYZ:{
						//additional 3 to offset the first 3 position channels
						cur_eul = Eul_(original_motion[i][3 + j * 3 + 2], original_motion[i][3 + j * 3 + 1], original_motion[i][3 + j * 3], EulOrdXYZr);
						next_eul = Eul_(original_motion[i + 1][3 + j * 3 + 2], original_motion[i + 1][3 + j * 3 + 1], original_motion[i + 1][3 + j * 3], EulOrdXYZr);
						cur_quat = Eul_ToQuat_Degree(cur_eul);
						next_quat = Eul_ToQuat_Degree(next_eul);
						QQuaternion cur_q_quat(cur_quat.w, cur_quat.x, cur_quat.y, cur_quat.z);
						QQuaternion next_q_quat(next_quat.w, next_quat.x, next_quat.y, next_quat.z);
						for (int k = 0; k < interpolate_interval; k++){

							//double dot_product = cur_q_quat.scalar()*next_q_quat.scalar() + cur_q_quat.x()*next_q_quat.x() + cur_q_quat.y()*next_q_quat.y() + cur_q_quat.z()*next_q_quat.z();
							//if (dot_product < 0)
							//	cur_q_quat = -cur_q_quat;

							QQuaternion interpolated = QQuaternion::slerp(cur_q_quat, next_q_quat, (float)k / (float)interpolate_interval);
							Quat tmp;
							tmp.w = interpolated.scalar();
							tmp.x = interpolated.x();
							tmp.y = interpolated.y();
							tmp.z = interpolated.z();

							//quat_interpolate_res.push_back(tmp);
							EulerAngles res = Eul_FromQuat_Degree(tmp, EulOrdXYZr);
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3] = res.z;
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3 + 1] = res.y;
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3 + 2] = res.x;
						}

						break; //optional
							 }

					case XZY:{
						//additional 3 to offset the first 3 position channels
						cur_eul = Eul_(original_motion[i][3 + j * 3 + 2], original_motion[i][3 + j * 3 + 1], original_motion[i][3 + j * 3], EulOrdXZYr);
						next_eul = Eul_(original_motion[i + 1][3 + j * 3 + 2], original_motion[i + 1][3 + j * 3 + 1], original_motion[i + 1][3 + j * 3], EulOrdXZYr);
						cur_quat = Eul_ToQuat_Degree(cur_eul);
						next_quat = Eul_ToQuat_Degree(next_eul);
						QQuaternion cur_q_quat(cur_quat.w, cur_quat.x, cur_quat.y, cur_quat.z);
						QQuaternion next_q_quat(next_quat.w, next_quat.x, next_quat.y, next_quat.z);
						for (int k = 0; k < interpolate_interval; k++){

							//double dot_product = cur_q_quat.scalar()*next_q_quat.scalar() + cur_q_quat.x()*next_q_quat.x() + cur_q_quat.y()*next_q_quat.y() + cur_q_quat.z()*next_q_quat.z();
							//if (dot_product < 0)
							//	cur_q_quat = -cur_q_quat;

							QQuaternion interpolated = QQuaternion::slerp(cur_q_quat, next_q_quat, (float)k / (float)interpolate_interval);
							Quat tmp;
							tmp.w = interpolated.scalar();
							tmp.x = interpolated.x();
							tmp.y = interpolated.y();
							tmp.z = interpolated.z();

							//quat_interpolate_res.push_back(tmp);
							EulerAngles res = Eul_FromQuat_Degree(tmp, EulOrdXZYr);
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3] = res.z;
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3 + 1] = res.y;
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3 + 2] = res.x;
						}

						break; //optional
							 }
					case YXZ:{
						//additional 3 to offset the first 3 position channels
						cur_eul = Eul_(original_motion[i][3 + j * 3 + 2], original_motion[i][3 + j * 3 + 1], original_motion[i][3 + j * 3], EulOrdYXZr);
						next_eul = Eul_(original_motion[i + 1][3 + j * 3 + 2], original_motion[i + 1][3 + j * 3 + 1], original_motion[i + 1][3 + j * 3], EulOrdYXZr);
						cur_quat = Eul_ToQuat_Degree(cur_eul);
						next_quat = Eul_ToQuat_Degree(next_eul);
						QQuaternion cur_q_quat(cur_quat.w, cur_quat.x, cur_quat.y, cur_quat.z);
						QQuaternion next_q_quat(next_quat.w, next_quat.x, next_quat.y, next_quat.z);
						for (int k = 0; k < interpolate_interval; k++){

							//double dot_product = cur_q_quat.scalar()*next_q_quat.scalar() + cur_q_quat.x()*next_q_quat.x() + cur_q_quat.y()*next_q_quat.y() + cur_q_quat.z()*next_q_quat.z();
							//if (dot_product < 0)
							//	cur_q_quat = -cur_q_quat;

							QQuaternion interpolated = QQuaternion::slerp(cur_q_quat, next_q_quat, (float)k / (float)interpolate_interval);
							Quat tmp;
							tmp.w = interpolated.scalar();
							tmp.x = interpolated.x();
							tmp.y = interpolated.y();
							tmp.z = interpolated.z();

							//quat_interpolate_res.push_back(tmp);
							EulerAngles res = Eul_FromQuat_Degree(tmp, EulOrdYXZr);
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3] = res.z;
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3 + 1] = res.y;
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3 + 2] = res.x;
						}

						break; //optional
							 }
					case YZX:{
						//additional 3 to offset the first 3 position channels
						cur_eul = Eul_(original_motion[i][3 + j * 3 + 2], original_motion[i][3 + j * 3 + 1], original_motion[i][3 + j * 3], EulOrdYZXr);
						next_eul = Eul_(original_motion[i + 1][3 + j * 3 + 2], original_motion[i + 1][3 + j * 3 + 1], original_motion[i + 1][3 + j * 3], EulOrdYZXr);
						cur_quat = Eul_ToQuat_Degree(cur_eul);
						next_quat = Eul_ToQuat_Degree(next_eul);
						QQuaternion cur_q_quat(cur_quat.w, cur_quat.x, cur_quat.y, cur_quat.z);
						QQuaternion next_q_quat(next_quat.w, next_quat.x, next_quat.y, next_quat.z);
						for (int k = 0; k < interpolate_interval; k++){

							//double dot_product = cur_q_quat.scalar()*next_q_quat.scalar() + cur_q_quat.x()*next_q_quat.x() + cur_q_quat.y()*next_q_quat.y() + cur_q_quat.z()*next_q_quat.z();
							//if (dot_product < 0)
							//	cur_q_quat = -cur_q_quat;

							QQuaternion interpolated = QQuaternion::slerp(cur_q_quat, next_q_quat, (float)k / (float)interpolate_interval);
							Quat tmp;
							tmp.w = interpolated.scalar();
							tmp.x = interpolated.x();
							tmp.y = interpolated.y();
							tmp.z = interpolated.z();

							//quat_interpolate_res.push_back(tmp);
							EulerAngles res = Eul_FromQuat_Degree(tmp, EulOrdYZXr);
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3] = res.z;
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3 + 1] = res.y;
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3 + 2] = res.x;
						}

						break; //optional
							 }
					case ZXY:{
						//additional 3 to offset the first 3 position channels
						cur_eul = Eul_(original_motion[i][3 + j * 3 + 2], original_motion[i][3 + j * 3 + 1], original_motion[i][3 + j * 3], EulOrdZXYr);
						next_eul = Eul_(original_motion[i + 1][3 + j * 3 + 2], original_motion[i + 1][3 + j * 3 + 1], original_motion[i + 1][3 + j * 3], EulOrdZXYr);
						cur_quat = Eul_ToQuat_Degree(cur_eul);
						next_quat = Eul_ToQuat_Degree(next_eul);
						QQuaternion cur_q_quat(cur_quat.w, cur_quat.x, cur_quat.y, cur_quat.z);
						QQuaternion next_q_quat(next_quat.w, next_quat.x, next_quat.y, next_quat.z);
						for (int k = 0; k < interpolate_interval; k++){

							//double dot_product = cur_q_quat.scalar()*next_q_quat.scalar() + cur_q_quat.x()*next_q_quat.x() + cur_q_quat.y()*next_q_quat.y() + cur_q_quat.z()*next_q_quat.z();
							//if (dot_product < 0)
							//	cur_q_quat = -cur_q_quat;

							QQuaternion interpolated = QQuaternion::slerp(cur_q_quat, next_q_quat, (float)k / (float)interpolate_interval);
							Quat tmp;
							tmp.w = interpolated.scalar();
							tmp.x = interpolated.x();
							tmp.y = interpolated.y();
							tmp.z = interpolated.z();

							//quat_interpolate_res.push_back(tmp);
							EulerAngles res = Eul_FromQuat_Degree(tmp, EulOrdZXYr);
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3] = res.z;
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3 + 1] = res.y;
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3 + 2] = res.x;
						}

						break; //optional
							 }
					case ZYX:{
						//additional 3 to offset the first 3 position channels
						cur_eul = Eul_(original_motion[i][3 + j * 3 + 2], original_motion[i][3 + j * 3 + 1], original_motion[i][3 + j * 3], EulOrdZYXr);
						next_eul = Eul_(original_motion[i + 1][3 + j * 3 + 2], original_motion[i + 1][3 + j * 3 + 1], original_motion[i + 1][3 + j * 3], EulOrdZYXr);
						cur_quat = Eul_ToQuat_Degree(cur_eul);
						next_quat = Eul_ToQuat_Degree(next_eul);
						QQuaternion cur_q_quat(cur_quat.w, cur_quat.x, cur_quat.y, cur_quat.z);
						QQuaternion next_q_quat(next_quat.w, next_quat.x, next_quat.y, next_quat.z);
						for (int k = 0; k < interpolate_interval; k++){

							//double dot_product = cur_q_quat.scalar()*next_q_quat.scalar() + cur_q_quat.x()*next_q_quat.x() + cur_q_quat.y()*next_q_quat.y() + cur_q_quat.z()*next_q_quat.z();
							//if (dot_product < 0)
							//	cur_q_quat = -cur_q_quat;

							QQuaternion interpolated = QQuaternion::slerp(cur_q_quat, next_q_quat, (float)k / (float)interpolate_interval);
							Quat tmp;
							tmp.w = interpolated.scalar();
							tmp.x = interpolated.x();
							tmp.y = interpolated.y();
							tmp.z = interpolated.z();

							//quat_interpolate_res.push_back(tmp);
							EulerAngles res = Eul_FromQuat_Degree(tmp, EulOrdZYXr);
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3] = res.z;
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3 + 1] = res.y;
							motion_to_interpolate[i*interpolate_interval + k][3 + j * 3 + 2] = res.x;
						}

						break; //optional
							 }
					default: //Optional
						break;
					}
				}
			}

			//for last frame's interpolation -> just copy for the last k frames
			for (int j = 0; j < channels.size(); j++){
				for (int k = 0; k < interpolate_interval; k++){
					motion_to_interpolate[(original_num_frame - 1)*interpolate_interval + k][j] = original_motion[(original_num_frame - 1)][j];
				}
			}
			return motion_to_interpolate;
		}

		double* Layer::linspace(double cur, double next, int N) {
			double h = (next - cur) / static_cast<double> (N);
			double* xs = new double[N];
			double val = cur;
			for (int i = 0; i < N; i++){
				xs[i] = val;
				val += h;
			}
			return xs;
		}

		double** Layer::drop_frames(double** motion, int drop_interval){
			double ** res_motion = new double*[current_num_frame];
			for (int i = 0; i < current_num_frame; i++){
				res_motion[i] = new double[num_channel];
			}
			for (int i = 0; i < current_num_frame; i++){
				for (int j = 0; j < num_channel; j++){
					res_motion[i][j] = motion[i*drop_interval][j];
				}
			}
			return res_motion;
		}

		void Layer::createSheets(double** motion){

			Sheet* sheetFromLoadedLayer;

			for (int i = 0; i < current_num_frame; i++){

				sheetFromLoadedLayer = new Sheet(num_channel, motion[i]);

				m_sheets.push_back(sheetFromLoadedLayer);

			}

			if (current_num_frame == 0){
				sheetFromLoadedLayer = new Sheet(num_channel, original_motion[0]);
				m_sheets.push_back(sheetFromLoadedLayer);
				current_num_frame = 1;
			}
			QLOG_INFO() << "Layer: Sheets creation completed -- m_Sheets.size() = " << m_sheets.size();

		}


		void Layer::exportBVHtoDir(QString targetDir){

			string export_file_name = targetDir.toStdString().substr(0, targetDir.size()) + "_export.bvh";  //-4 is because the original file name ends with .bvh
			std::ofstream outfile(export_file_name);

			outfile << "HIERARCHY" << endl;

			Joint* curJoint = joints.at(0);

			//Skeleton export - recursive function to export all joint info in hierarchy
			printJoint(curJoint, outfile, 0);

			//Motion export
			outfile << "MOTION" << endl;
			outfile << "Frames: " << to_string(current_num_frame) << endl;
			outfile << "Frame Time:	" << current_interval << endl;
			double* motion;
			for (int i = 0; i < current_num_frame; i++){
				motion = m_sheets.at(i)->getMotion();
				for (int j = 0; j < num_channel; j++)
					outfile << motion[j] << " ";
				outfile << " " << endl;
			}

			outfile.close();
		}

		void Layer::printJoint(Joint* curJoint, ofstream& outfile, int level)
		{
			//ASSUMPTIONS
			//assume joint name is unique
			//assume first joint must be root
			//assume there is only 1 root

			string indent = string(level * 2, ' ');
			if (curJoint->name == joints.at(0)->name)
				outfile << indent << "ROOT " << curJoint->name << endl;
			else
				outfile << indent << "JOINT " << curJoint->name << endl;

			outfile << indent << "{" << endl;

			outfile << indent << "  OFFSET " << curJoint->offset[0] << " " << curJoint->offset[1] << " " << curJoint->offset[2] << endl;

			outfile << indent << "  CHANNELS " << curJoint->channels.size();

			for (int i = 0; i < curJoint->channels.size(); i++){
				outfile << " " << getChannelName(curJoint->channels.at(i)->type);
			}

			outfile << endl;

			if (curJoint->has_site)
			{
				outfile << indent << "  End Site" << endl;
				outfile << indent << "  {" << endl;
				outfile << indent << "    OFFSET " << curJoint->site[0] << " " << curJoint->site[1] << " " << curJoint->site[2] << endl;
				outfile << indent << "  }" << endl;
			}


			for (int i = 0; i < curJoint->children.size(); i++){
				printJoint(curJoint->children.at(i), outfile, level + 1);
			}
			outfile << indent << "}" << endl;
		}

		//get greatest common divisor between 2 int
		int Layer::gcd(int a, int b)
		{
			for (;;)
			{
				if (a == 0) return b;
				b %= a;
				if (b == 0) return a;
				a %= b;
			}
		}

		//get least common multiple between 2 int
		int Layer::lcm(int a, int b)
		{
			int temp = gcd(a, b);

			return temp ? (a / temp * b) : 0;
		}

		string Layer::getChannelName(ChannelEnum channel){
			switch (channel){
			case 0: {return "Xrotation"; break; }
			case 1: {return "Yrotation"; break;}
			case 2: {return "Zrotation"; break; }
			case 3: {return "Xposition"; break; }
			case 4: {return "Yposition"; break; }
			case 5: {return "Zposition"; break; }
			default: {return NULL; break; }
			}
		}

		Layer::Layer(LayerGroup* parent) :m_parent(parent)
		{
			QLOG_INFO() << "Layer:" << "Constructor called";
			//motion = NULL;
			Clear();
			file_name = "default";
		}

		Layer::Layer(const char * bvh_file_name, LayerGroup* parent) :m_parent(parent)
		{
			QLOG_INFO() << "Layer:" << "Constructor called";
			//motion = NULL;
			Clear();
			getBVHDataFromFile(bvh_file_name);

		}

		Layer::~Layer()
		{
			Clear();
		}

		void  Layer::Clear()
		{
			int  i;
			for (i = 0; i<channels.size(); i++)
				delete  channels[i];
			for (i = 0; i<joints.size(); i++)
				delete  joints[i];
			//if (motion != NULL)
			//delete  motion;

			is_load_success = false;

			file_name = "";
			motion_name = "";

			num_channel = 0;
			channels.clear();
			joints.clear();
			joint_index.clear();

			current_num_frame = 0;
			original_frame_rate = 0;
			current_interval = 0.0;
			original_interval = 0.0; 
			//motion = NULL;
		}


		void Layer::changeChannelOrder(Joint* joint, ChannelOrder newOrder){

			//if it is root, change order for last 3 channels
			//		since the first 3 are position values


			int	indexOffset = 0;
			if (joint->parent == NULL)
				indexOffset = 3;

			//set all channel indices to the min value, going to set it correctly after changing order
			channels[1 + indexOffset]->index = joint->channels[0 + indexOffset]->index;
			channels[2 + indexOffset]->index = joint->channels[0 + indexOffset]->index;


			if (newOrder != joint->channel_order){
				ChannelOrder oldOrder = joint->channel_order;
				Channel* tmp;

				//firstly change the old order to standardized xzy order

				if (oldOrder % 2){
					tmp = joint->channels[2 + indexOffset];
					joint->channels[2 + indexOffset] = joint->channels[1 + indexOffset];
					joint->channels[1 + indexOffset] = tmp;
				}
				if (oldOrder / 2 == 1){
					tmp = joint->channels[0 + indexOffset];
					joint->channels[0 + indexOffset] = joint->channels[1 + indexOffset];
					joint->channels[1 + indexOffset] = tmp;
				}
				else if (oldOrder / 2 == 2) {
					tmp = joint->channels[0 + indexOffset];
					joint->channels[0 + indexOffset] = joint->channels[1 + indexOffset];
					joint->channels[1 + indexOffset] = joint->channels[2 + indexOffset];
					joint->channels[2 + indexOffset] = tmp;
				}


				//then change the standardized xyz order to new order


				if (newOrder / 2 == 1){
					tmp = joint->channels[0 + indexOffset];
					joint->channels[0 + indexOffset] = joint->channels[1 + indexOffset];
					joint->channels[1 + indexOffset] = tmp;
				}
				else if (newOrder / 2 == 2) {
					tmp = joint->channels[0 + indexOffset];
					joint->channels[0 + indexOffset] = joint->channels[2 + indexOffset];
					joint->channels[1 + indexOffset] = tmp;
					joint->channels[2 + indexOffset] = joint->channels[1 + indexOffset];
				}
				if (newOrder % 2){
					tmp = joint->channels[2 + indexOffset];
					joint->channels[2 + indexOffset] = joint->channels[1 + indexOffset];
					joint->channels[1 + indexOffset] = tmp;
				}

				//set all channel indices to the correct value
				channels[1 + indexOffset]->index += 1;
				channels[2 + indexOffset]->index += 2;


				joint->channel_order = newOrder;
			}
		}

		bool Layer::isOnionSkinOn() const{
			return 0;
		}

		bool Layer::isLocked() const{
			return 0;
		}

		bool Layer::isVisible() const{
			return 1;
		}

		//To be moved to LayerGroup if this attribute is determined not necessary in Layer
		bool Layer::isExpanded() const{
			return 1;
		}


		unsigned int Layer::getOpacity() const{
			return 0;
		}


	
		//const utf8string& Layer::getName() const{
		//cannot return uft8string - no constructor
		//return utf8string();
		//}

		//const Argb& getColor();

		//const Argb& getColorSep();


		void Layer::setLocked(bool locked){

		}

		void Layer::setVisible(bool visible){

		}

		void Layer::setOnionSkinOn(bool on){

		}

		void Layer::setExpanded(bool expanded){

		}

		void Layer::setOpacity(unsigned int opacity){

		}

		void Layer::setName(const utf8string& name){

		}

		//void setColor(const Argb& color);


		/*unsigned int Layer::sheetCount() const{
		return 0;
		}*/

		//Looks like, the incoming index is the displayed index (max = size)
		//At least it's like this in framewidget paint normalized 
		Sheet* Layer::sheetAtIndex(int index) const{
			//if (index >= m_sheets.size())
			//	return m_sheets.at(m_sheets.size() - 1);
			//else
			//	return m_sheets.at(index);
			if (index < m_sheets.size())
				return m_sheets.at(index);
		}

		unsigned int Layer::layerType() const{
			return 0;
		}

		const string Layer::GetShortFileName() const {
			string pureFileName = file_name.substr(file_name.find_last_of("/\\") + 1);
			string shortFileName = pureFileName.substr(0, pureFileName.find_last_of("."));
			return shortFileName;
		}

		Joint *   Layer::GetJoint(const string & j) const  {
			map< string, Joint * >::const_iterator  i = joint_index.find(j);
			return  (i != joint_index.end()) ? (*i).second : NULL;
		}

		Joint *   Layer::GetJoint(const char * j) const  {
			map< string, Joint * >::const_iterator  i = joint_index.find(j);
			return  (i != joint_index.end()) ? (*i).second : NULL;
		}

		void Layer::selectJoint(int index){ 
			joints.at(index)->is_selected = 1; 
		}

		void Layer::selectJoint(Joint* targetJoint){

			for (int i = 0; i < joints.size(); i++){
				joints.at(i)->is_selected = 0;
			}
			targetJoint->is_selected = 1;

		}

		void Layer::removeSheets(int index, int count){
			if( (m_sheets.size() < index + count) || (m_sheets.size() <= index)){
				QLOG_INFO() << "Layer::removeSheets failed: remove region out of boundary";
			}
			else{
				for (int i = index; i < index + count; i++){
					delete m_sheets[i];
				}
				m_sheets.erase(m_sheets.begin() + index, m_sheets.begin() + index + count);
				int pause = 1;
				current_num_frame -= count;
			}
		}

		void Layer::duplicateSheet(FrameId fid){

			m_sheets.insert(m_sheets.begin() + fid, m_sheets.at(fid)->clone());
			current_num_frame++;
		}

		void Layer::deleteSheet(FrameId fid){

			m_sheets.erase(m_sheets.begin() + fid);
			current_num_frame--;
		}

		//begin: xiaocai added
		double Layer::ComputeCurrentLLegLength()
		{		
			double LLegLength = 0.0;
			Joint *LLeg = GetJoint("LeftLeg");
			Joint *LFoot = GetJoint("LeftFoot");

			const double *LLegOffSet =   &LLeg->offset[0];
			const double *LFootOffSet = &LFoot->offset[0];

			double LLegTotal[3];
			LLegTotal[0] = LLegOffSet[0] + LFootOffSet[0];
			LLegTotal[1] = LLegOffSet[1] + LFootOffSet[1];
			LLegTotal[2] = LLegOffSet[2] + LFootOffSet[2];

			LLegLength = sqrt(LLegTotal[0]*LLegTotal[0] + LLegTotal[1]*LLegTotal[1] + LLegTotal[2]*LLegTotal[2]);

			return LLegLength;		
		}
		
		//yy: This function is to do traversal to get max length of the joint
		double Layer::HierarchyLength(Joint* joint) {
			double length = 0;
			double currentLength;

			if (joint->children.size() != 0) {
				for (int i = 0; i < joint->children.size(); i++) {
					currentLength = HierarchyLength(joint->children.at(i));
					if (length < currentLength)
						length = currentLength;
				}
			}
			if (joint->offset[1] < 0)
				return -1 * joint->offset[1] + length;
			else
				return length;
		}


		double Layer::ComputeCurrentRLegLength()
		{
			double RLegLength = 0.0;
			Joint *RLeg = GetJoint("RightLeg");
			Joint *RFoot = GetJoint("RightFoot");
			double *RLegOffset =   RLeg->offset;
			double *RFootOffSet = RFoot->offset;

			double RLegTotal[3];
			RLegTotal[0] = RLegOffset[0] + RFootOffSet[0];
			RLegTotal[1] = RLegOffset[1] + RFootOffSet[1];
			RLegTotal[2] = RLegOffset[2] + RFootOffSet[2];

			RLegLength = sqrt(RLegTotal[0]*RLegTotal[0] + RLegTotal[1]*RLegTotal[1] + RLegTotal[2]*RLegTotal[2]);

			return RLegLength;
		}

		void Layer::UpdateLLegLengthChange() { 
			LLegLengthChange = ComputeCurrentLLegLength() - LLegLengthOriginal; 
		}
		void Layer::UpdateRLegLengthChange() { 
			RLegLengthChange = ComputeCurrentRLegLength() - RLegLengthOriginal; 
		}

		//void Layer::ChangeHipsHeightAccordingToLegLengths(double lengthChange)
		//{
		//	//
		//	int numFrames = current_num_frame;
		//	for (int i = 0; i < numFrames; i++)
		//	{
		//		
		//	}


		//}
		//end: xiaocai added

	}
}
