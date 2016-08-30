#pragma once

#include <qlistwidget.h>
#include "../data/ImageFile.h"

class ImageListWidget : public QListWidget
{
public:
	ImageListWidget(ImageGroup* images);
	~ImageListWidget();
	
	void update();

private:
	ImageGroup* m_imageList;
};