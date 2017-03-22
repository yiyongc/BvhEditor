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

void ImageFile::validateConstraints()
{
	if (m_bConstraintsValid)
		return;

	size_t nConstraints = m_vSelected.size();
	std::set<unsigned int>::iterator cur(m_vSelected.begin()), end(m_vSelected.end());
	while (cur != end) {
		unsigned int nVertex = *cur++;
		Wml::Vector3f vVertex;
		vVertex.X() = m_deformedMesh->vertex(nVertex).P().x;
		vVertex.Y() = m_deformedMesh->vertex(nVertex).P().y;
		vVertex.Z() = 0;
		//m_deformedMesh.GetVertex(nVertex, vVertex);
		m_deformer.SetDeformedHandle(nVertex, Wml::Vector2f(vVertex.X(), vVertex.Y()));
	}

	m_deformer.ForceValidation();

	m_bConstraintsValid = true;
}