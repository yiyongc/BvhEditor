#pragma once
#include "OpenToonz\tmeshimage.h"
#include <QString>
#include <qimage.h>
#include <glut.h>
#include "../MeshDeformer/RigidMeshDeformer2D.h"
#include <unordered_map>
#include "../data/Layer.h"
#include "../data/LayerGroup.h"

#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

const float   figure_scale = 0.04f;

// This class is to store images with their respective meshes and their transformation matrix to ensure coherence between the images
class ImageFile {

public:
	ImageFile(QString name, QImage img);
	~ImageFile();
	
	QString getImageName();
	void setImageName(QString name);
	QImage getQImage();
	void setQImage(QImage img);
	TMeshImageP getMeshImage();
	void setMeshImage(TMeshImageP mesh);
	GLfloat* getTransformMatrix();
	void setTransformMatrix(float rotAngle, float xTrans, float yTrans, float scale);
	GLuint m_imgTexture;

	int getImageRotation();
	float getImageXTrans();
	float getImageYTrans();
	float getImageScale();
	void setImageRotation(int rot);
	void setImageXTrans(float xTrans);
	void setImageYTrans(float yTrans);
	void setImageScale(float scaleValue); 
	TTextureMesh* getDeformedMesh() {
		return m_deformedMesh;
	}
	
	RigidMeshDeformer2D m_deformer;

	void invalidateConstraints() { m_bConstraintsValid = false;  }

	void validateConstraints(int imageIndex, std::unordered_multimap<int, std::pair<int, int>>* map, cacani::data::Layer* m_layer, int curFrame);

	std::set<unsigned int> m_vSelected;

private:
	QString m_imgName;
	QImage m_img;
	TMeshImageP m_meshImg;
	TTextureMesh* m_deformedMesh;
	GLfloat m_transformMatrix[16];
	int rotAngleImg = 0;
	float xTransImg = 0;
	float yTransImg = 0;
	float scaleImg = 1.0;

	bool m_bConstraintsValid = false;
	

	Wml::GMatrixd calculateGlobalCoordMatrix(cacani::data::Layer* m_layer, int jointIndex, int frameNum);
	Wml::GMatrixd getGlobalCoord(cacani::data::Layer* m_layer, int jointIndex, int frameNum);
	int findJointIndex(std::unordered_multimap<int, std::pair<int, int>>* map, int imageIndex, int vertID);

};

struct ImageGroup {
	std::vector<ImageFile> m_images;

	int size() {
		return m_images.size();
	}

	void deleteImages() {
		m_images.clear();
	}
};
