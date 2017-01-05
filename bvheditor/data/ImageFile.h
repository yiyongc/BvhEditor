#pragma once
#include "OpenToonz\tmeshimage.h"
#include <QString>
#include <qimage.h>
#include <glut.h>

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

private:
	QString m_imgName;
	QImage m_img;
	TMeshImageP m_meshImg;
	GLfloat m_transformMatrix[16];
	int rotAngleImg = 0;
	float xTransImg = 0;
	float yTransImg = 0;
	float scaleImg = 1.0;
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