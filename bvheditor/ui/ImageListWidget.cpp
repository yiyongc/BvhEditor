#include "ImageListWidget.h"


ImageListWidget::ImageListWidget(ImageGroup* img) : m_imageList(img) {
}

ImageListWidget::~ImageListWidget() {}

void ImageListWidget::update() {
	this->clear();
	for (int i = 0; i < m_imageList->size(); i++) {
		this->addItem(m_imageList->m_images[i].getImageName());
	}
	QListWidget::update();
}