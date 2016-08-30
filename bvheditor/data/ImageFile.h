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
	TAffine getTransformMatrix();
	void setTransformMatrix(TAffine mat);
	GLuint m_imgTexture;

private:
	QString m_imgName;
	QImage m_img;
	TMeshImageP m_meshImg;
	TAffine m_transformMatrix;
	
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