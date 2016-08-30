#include "ImageFile.h"

ImageFile::ImageFile(QString name, QImage img) :
	m_imgName(name),
	m_img(img),
	m_imgTexture(0),
	m_meshImg(0) 
{
	m_transformMatrix = TAffine();
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

TAffine ImageFile::getTransformMatrix() {
	return m_transformMatrix;
}

void ImageFile::setTransformMatrix(TAffine mat) {
	m_transformMatrix = mat;
}