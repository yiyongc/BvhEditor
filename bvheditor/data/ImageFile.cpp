#include "ImageFile.h"

ImageFile::ImageFile(QString name, QImage img) :
	m_imgName(name),
	m_img(img),
	m_imgTexture(0),
	m_meshImg(0), 
	m_deformedMesh(new TTextureMesh),
	m_deformer(RigidMeshDeformer2D())
{
	//column by column initialize transform matrix to identity
	m_transformMatrix[0] = 1;
	m_transformMatrix[1] = 0; 
	m_transformMatrix[2] = 0;
	m_transformMatrix[3] = 0;

	m_transformMatrix[4] = 0;
	m_transformMatrix[5] = 1;
	m_transformMatrix[6] = 0;
	m_transformMatrix[7] = 0;

	m_transformMatrix[8] = 0;
	m_transformMatrix[9] = 0;
	m_transformMatrix[10] = 1;
	m_transformMatrix[11] = 0;

	m_transformMatrix[12] = 0;
	m_transformMatrix[13] = 0;
	m_transformMatrix[14] = 0;
	m_transformMatrix[15] = 1;
}

ImageFile::~ImageFile() {}

QString ImageFile::getImageName() {
	return m_imgName;
}

void ImageFile::setImageName(QString name) {
	m_imgName = name;
}

QImage ImageFile::getQImage() {
	return m_img;
}

void ImageFile::setQImage(QImage img) {
	m_img = img;
}

TMeshImageP ImageFile::getMeshImage() {
	return m_meshImg;
}

void ImageFile::setMeshImage(TMeshImageP mesh) {
	m_meshImg = mesh;
}

GLfloat* ImageFile::getTransformMatrix() {
	return m_transformMatrix;
}

void ImageFile::setTransformMatrix(float rotAngle, float xTrans, float yTrans, float scaleFactor) {
	GLfloat scale[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	GLfloat rotate[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	GLfloat translate[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	float pi = 3.14159265;

	//Set up rotation about z axis
	rotate[0] = cosf(rotAngle /180 * pi);
	rotate[4] = -sinf(rotAngle / 180 * pi);
	rotate[1] = sinf(rotAngle / 180 * pi);
	rotate[5] = cosf(rotAngle / 180 * pi);

	//Set up translation
	translate[12] = xTrans;
	translate[13] = yTrans;

	//Set up scale
	scale[0] = scaleFactor;
	scale[5] = scaleFactor;
	scale[10] = scaleFactor;

	//final transform matrix
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf(translate);
	glMultMatrixf(rotate);
	glMultMatrixf(scale);
	glGetFloatv(GL_MODELVIEW_MATRIX, m_transformMatrix);
	glPopMatrix();
}

int ImageFile::getImageRotation() {
	return rotAngleImg;
}

float ImageFile::getImageXTrans() {
	return xTransImg;
}

float ImageFile::getImageYTrans() {
	return yTransImg;
}

void ImageFile::setImageRotation(int rot) {
	rotAngleImg = rot;
}

void ImageFile::setImageXTrans(float xTrans) {
	xTransImg = xTrans;
}

void ImageFile::setImageYTrans(float yTrans) {
	yTransImg = yTrans;
}


float ImageFile::getImageScale() {
	return scaleImg;
}
 
void ImageFile::setImageScale(float scaleValue) {
	scaleImg = scaleValue;
}

void ImageFile::validateConstraints(int imageIndex, std::unordered_multimap<int, std::pair<int, int>>* map, cacani::data::Layer* m_layer, int curFrame)
{
	if (m_bConstraintsValid)
		return;

	size_t nConstraints = m_vSelected.size();
	std::set<unsigned int>::iterator cur(m_vSelected.begin()), end(m_vSelected.end());
	while (cur != end) {
		unsigned int nVertex = *cur++;
		Wml::Vector3f vVertex;

		int jointIndex = findJointIndex(map, imageIndex, nVertex);
		Wml::GMatrixd coordinates = getGlobalCoord(m_layer, jointIndex, curFrame);
		
		vVertex.X() = coordinates(0, 0) * figure_scale;
		vVertex.Y() = coordinates(1, 0) * figure_scale;
		vVertex.Z() = 0;
		//Original working
		//vVertex.X() = m_deformedMesh->vertex(nVertex).P().x;
		//vVertex.Y() = m_deformedMesh->vertex(nVertex).P().y;
		//vVertex.Z() = 0;

		m_deformer.SetDeformedHandle(nVertex, Wml::Vector2f(vVertex.X(), vVertex.Y()));
	}

	m_deformer.ForceValidation();

	m_bConstraintsValid = true;
}


Wml::GMatrixd ImageFile::calculateGlobalCoordMatrix(cacani::data::Layer* m_layer, int jointIndex, int frameNum) {
	double iden[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	Wml::GMatrixd translation(4, 4, iden);
	Wml::GMatrixd rotateX(4, 4, iden);
	Wml::GMatrixd rotateY(4, 4, iden);
	Wml::GMatrixd rotateZ(4, 4, iden);

	cacani::data::Joint* currJoint = m_layer->GetJoint(jointIndex);
	double* motionData;

	if (m_layer->m_sheets.size() != 0)
		motionData = m_layer->sheetAtIndex(frameNum)->getMotion();
	else
		motionData = dynamic_cast<cacani::data::LayerGroup*> (m_layer)->childAtIndex(0)->sheetAtIndex(frameNum)->getMotion();

	double transValues[] = { 0, 0, 0, 1 };

	//Obtain translation matrix value
	if (currJoint->parent == NULL) {
		transValues[0] = motionData[0];
		transValues[1] = motionData[1];
		transValues[2] = motionData[2];
	}
	else {
		double* offsetValues = currJoint->offset;
		transValues[0] = offsetValues[0];
		transValues[1] = offsetValues[1];
		transValues[2] = offsetValues[2];
	}
	Wml::GVectord transVector(4, transValues);
	translation.SetColumn(3, transVector);

	int numChannels = currJoint->channels.size();

	//Obtain rotation matrices based on offset in motion data
	for (int i = 0; i < numChannels; i++) {
		cacani::data::Channel* channel = currJoint->channels[i];
		double rotValue = motionData[channel->index];
		
		double sValue, cValue;
		if (rotValue < 0) 
			rotValue += 360;
		
		sValue = sin(rotValue * M_PI / 180);
		cValue = cos(rotValue * M_PI / 180);

		if (channel->type == cacani::data::Xrotation) {
			double col1Values[] = { 0, cValue, sValue, 0 };
			double col2Values[] = { 0, -sValue, cValue, 0 };

			Wml::GVectord column1(4, col1Values);
			Wml::GVectord column2(4, col2Values);

			rotateX.SetColumn(1, column1);
			rotateX.SetColumn(2, column2);
		}
		else if (channel->type == cacani::data::Yrotation) {
			double col0Values[] = { cValue, 0, -sValue, 0 };
			double col2Values[] = { sValue, 0, cValue, 0 };

			Wml::GVectord column0(4, col0Values);
			Wml::GVectord column2(4, col2Values);

			rotateY.SetColumn(0, column0);
			rotateY.SetColumn(2, column2);
		}
		else if (channel->type == cacani::data::Zrotation) {
			double col0Values[] = { cValue, sValue, 0, 0 };
			double col1Values[] = { -sValue, cValue, 0, 0 };

			Wml::GVectord column0(4, col0Values);
			Wml::GVectord column1(4, col1Values);

			rotateY.SetColumn(0, column0);
			rotateY.SetColumn(1, column1);;
		}
	}

	cacani::data::ChannelOrder order = currJoint->channel_order;
	Wml::GMatrixd rotMatrix(4, 4);
	switch (order) {
	case (cacani::data::XYZ) :
		rotMatrix = rotateX * rotateY * rotateZ;
		break;
	case (cacani::data::XZY) :
		rotMatrix = rotateX * rotateZ * rotateY;
		break;
	case (cacani::data::YXZ) :
		rotMatrix = rotateY * rotateX * rotateZ;
		break;
	case (cacani::data::YZX) :
		rotMatrix = rotateY * rotateZ * rotateX;
		break;
	case (cacani::data::ZXY) :
		rotMatrix = rotateZ * rotateX * rotateY;
		break;
	case (cacani::data::ZYX) :
		rotMatrix = rotateZ * rotateY * rotateX;
		break;
	}

	Wml::GMatrixd myMatrix = translation * rotMatrix;

	if (currJoint->parent == NULL)
		return myMatrix;
	else
		return calculateGlobalCoordMatrix(m_layer, currJoint->parent->index, frameNum) * myMatrix;
}

Wml::GMatrixd ImageFile::getGlobalCoord(cacani::data::Layer* m_layer, int jointIndex, int frameNum) {
	Wml::GMatrixd localCoord(4, 1);
	double local[] = { 0, 0, 0, 1 };
	Wml::GVectord vectorToSet(4, local);
	localCoord.SetColumn(0, vectorToSet);

	return calculateGlobalCoordMatrix(m_layer, jointIndex, frameNum) * localCoord;
}

int ImageFile::findJointIndex(std::unordered_multimap<int, std::pair<int, int>>* map, int imageIndex, int vertID) {
	auto it = map->begin();
	for (; it != map->end(); it++) {
		pair<int, int> cur = it->second;
		if (cur.first != imageIndex)
			continue;
		else {
			if (cur.second == vertID)
				return it->first;
		}
	}

	return -1;

}