#include <QtWidgets>
#include <QtOpenGL>

#include "SheetCanvas.h"
#include <math.h>
#include <glut.h>
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/euler_angles.hpp>
#include <GLFW/glfw3.h>
#include "data/Matrices.h"
#include <qdebug.h>

namespace cacani { 


	namespace ui {

		static float   camera_yaw = 0.0f;      // 
		static float   camera_pitch = -20.0f;  // 

		// 
		static int     drag_mouse_r = 0; // 
		static int     drag_mouse_l = 0; // ?
		static int     drag_mouse_m = 0; // 
		static int     last_mouse_x, last_mouse_y; // ?

		// 
		static int     win_width, win_height;

		const int     message_length = 65;
		const int     max_bvh_file_number = 30;
		const float   figure_scale = 0.02f;

		SheetCanvas::SheetCanvas(cacani::controller::LayerController* layerController, QWidget *parent) :m_layerController(layerController), m_imageList(NULL)
		{
			xRot = 0;
			yRot = 0;
			zRot = 0;
			m_sheet = NULL;
			m_imageGroup = new ImageGroup;
			m_image = NULL;
			m_base = m_layerController->getCurrentRoot();
			m_frame = 0;
			m_camera_distance = 4.0f; //
			m_IKEnabled = false;
			m_IKArm = new Arm();
			m_timer = new QTimer(this);
			m_timer_selected = new QTimer(this);
			updateState(STATE_CAMERA);
			m_skeleton = NULL;

			connect(m_timer_selected, SIGNAL(timeout()), this, SLOT(playCanvasSelected()));
			connect(m_timer, SIGNAL(timeout()), this, SLOT(playCanvas()));

			//Updates GL Widget every 10ms
			QTimer* timer = new QTimer(this);
			connect(timer, SIGNAL(timeout()), this, SLOT(update()));
			timer->start(10);
		}


		SheetCanvas::~SheetCanvas(){}

		void SheetCanvas::setImageList(ImageListWidget* imageList) {
			m_imageList = imageList;
		}

		void  SheetCanvas::RenderFigure(float scale)
		{
			
			cacani::data::Layer* curLayer;

			if (!m_base->isTerminal()){

				const cacani::data::LayerGroup* curLayerGroup;
				for (int i = 0; i < m_base->memberCount(); i++){
					curLayerGroup = dynamic_cast<const cacani::data::LayerGroup*>(m_base->childAtIndex(i));
					for (int j = 0; j < curLayerGroup->memberCount(); j++){
						curLayer = curLayerGroup->childAtIndex(j);
						if (curLayer->checkIfVisible())
						{
							RenderFigure(curLayer->GetJoint(0), curLayer->sheetAtIndex(m_frame)->getMotion(), scale);
						}
						else
						{
							QLOG_INFO() << "SheetCanvas:: rendering hidden - Layer Name: " << curLayer->GetFullFileName().c_str();
						}

					}
				}

			}
			else{
				for (int j = 0; j < m_base->memberCount(); j++){
					curLayer = m_base->childAtIndex(j);
					if (curLayer->checkIfVisible())
					{
						RenderFigure(curLayer->GetJoint(0), curLayer->sheetAtIndex(m_frame)->getMotion(), scale);
					}
					else
					{
						QLOG_INFO() << "SheetCanvas:: rendering hidden - Layer Name: " << curLayer->GetFullFileName().c_str();
					}

				}

			}
		}


		void  SheetCanvas::RenderFigure(cacani::data::Joint * joint, const double * data, float scale, bool descendent)
		{
			if (joint->is_selected)   glColor3f(1.0f, 0.0f, 0.0f);
			if (!(joint->is_selected))   glColor3f(0.0f, 0.0f, 1.0f);
			bool descendent_new = false;
			if (m_IKEnabled){
				if ((joint->name.compare(m_IKBaseJoint->name) == 0) || (descendent == true)){
					descendent_new = true;
					glColor3f(0.662745f, 0.662745f, 0.662745f);
				}
			}
			if (descendent)  glColor3f(0.662745f, 0.662745f, 0.662745f);
			glPushMatrix();

			// 
			if (joint->parent == NULL)
			{
				glTranslatef(data[0] * scale, data[1] * scale, data[2] * scale);
			}
			// 
			else
			{
				glTranslatef(joint->offset[0] * scale, joint->offset[1] * scale, joint->offset[2] * scale);
				
			}

			// 
			int  i, j;
			for (i = 0; i<joint->channels.size(); i++)
			{
				cacani::data::Channel *  channel = joint->channels[i];
				if (channel->type == cacani::data::Xrotation)
					glRotatef(data[channel->index], 1.0f, 0.0f, 0.0f);
				else if (channel->type == cacani::data::Yrotation)
					glRotatef(data[channel->index], 0.0f, 1.0f, 0.0f);
				else if (channel->type == cacani::data::Zrotation)
					glRotatef(data[channel->index], 0.0f, 0.0f, 1.0f);
			}

			// 
			// 
			if (joint->children.size() == 0)
			{
				RenderBone(0.0f, 0.0f, 0.0f, joint->site[0] * scale, joint->site[1] * scale, joint->site[2] * scale);
			}
			// 
			if (joint->children.size() == 1)
			{
				cacani::data::Joint *  child = joint->children[0];
				RenderBone(0.0f, 0.0f, 0.0f, child->offset[
					0] * scale, child->offset[1] * scale, child->offset[2] * scale);
			}
			// 
			if (joint->children.size() > 1)
			{
				//
				float  center[3] = { 0.0f, 0.0f, 0.0f };
				for (i = 0; i<joint->children.size(); i++)
				{
					cacani::data::Joint *  child = joint->children[i];
					center[0] += child->offset[0];
					center[1] += child->offset[1];
					center[2] += child->offset[2];
				}
				center[0] /= joint->children.size() + 1;
				center[1] /= joint->children.size() + 1;
				center[2] /= joint->children.size() + 1;

				RenderBone(0.0f, 0.0f, 0.0f, center[0] * scale, center[1] * scale, center[2] * scale);

				//
				for (i = 0; i<joint->children.size(); i++)
				{
					cacani::data::Joint *  child = joint->children[i];
					RenderBone(center[0] * scale, center[1] * scale, center[2] * scale,
						child->offset[0] * scale, child->offset[1] * scale, child->offset[2] * scale);
				}
			}

			// 
			for (i = 0; i<joint->children.size(); i++)
			{


				RenderFigure(joint->children[i], data, scale, descendent_new);
			}

			glPopMatrix();
		}



		void  SheetCanvas::RenderBone(float x0, float y0, float z0, float x1, float y1, float z1)
		{
			GLdouble  dir_x = x1 - x0;
			GLdouble  dir_y = y1 - y0;
			GLdouble  dir_z = z1 - z0;
			GLdouble  bone_length = sqrt(dir_x*dir_x + dir_y*dir_y + dir_z*dir_z);

			// 
			static GLUquadricObj *  quad_obj = NULL;
			if (quad_obj == NULL)
				quad_obj = gluNewQuadric();
			gluQuadricDrawStyle(quad_obj, GLU_FILL);
			gluQuadricNormals(quad_obj, GLU_SMOOTH);

			glPushMatrix();

			// 
			glTranslated(x0, y0, z0);

			//

			//
			double  length;
			length = sqrt(dir_x*dir_x + dir_y*dir_y + dir_z*dir_z);
			if (length < 0.0001) {
				dir_x = 0.0; dir_y = 0.0; dir_z = 1.0;  length = 1.0;
			}
			dir_x /= length;  dir_y /= length;  dir_z /= length;

			// 
			GLdouble  up_x, up_y, up_z;
			up_x = 0.0;
			up_y = 1.0;
			up_z = 0.0;

			//
			double  side_x, side_y, side_z;
			side_x = up_y * dir_z - up_z * dir_y;
			side_y = up_z * dir_x - up_x * dir_z;
			side_z = up_x * dir_y - up_y * dir_x;

			// 
			length = sqrt(side_x*side_x + side_y*side_y + side_z*side_z);
			if (length < 0.0001) {
				side_x = 1.0; side_y = 0.0; side_z = 0.0;  length = 1.0;
			}
			side_x /= length;  side_y /= length;  side_z /= length;

			//
			up_x = dir_y * side_z - dir_z * side_y;
			up_y = dir_z * side_x - dir_x * side_z;
			up_z = dir_x * side_y - dir_y * side_x;

			//
			GLdouble  m[16] = { side_x, side_y, side_z, 0.0,
				up_x, up_y, up_z, 0.0,
				dir_x, dir_y, dir_z, 0.0,
				0.0, 0.0, 0.0, 1.0 };
			glMultMatrixd(m);

			// 
			GLdouble radius = 0.035; //
			GLdouble slices = 8.0; // 
			GLdouble stack = 3.0;  // 

			//
			gluCylinder(quad_obj, radius, radius, bone_length, slices, stack);

			glPopMatrix();
		}

		void SheetCanvas::playCanvas()
		{
			int total_frames = 0;
			cacani::data::Layer* curLayer;

			//Obtain frames from the layers to play
			if (!m_base->isTerminal()){
				const cacani::data::LayerGroup* curLayerGroup;
				for (int i = 0; i < m_base->memberCount(); i++)
				{
					curLayerGroup = dynamic_cast<const cacani::data::LayerGroup*>(m_base->childAtIndex(i));

					for (int j = 0; j < curLayerGroup->memberCount(); j++)
					{
						curLayer = curLayerGroup->childAtIndex(j);
						if ((curLayer->checkIfVisible()) && (curLayer->GetNumFrame()>total_frames))
						{
							total_frames = curLayer->GetNumFrame();
						}
					}
				}
			}
			else{
				for (int j = 0; j < m_base->memberCount(); j++){
					curLayer = m_base->childAtIndex(j);
					if ((curLayer->checkIfVisible()) && (curLayer->GetNumFrame()>total_frames))
					{
						total_frames = curLayer->GetNumFrame();
					}

				}

			}
		
			currentFrame++;
			if (currentFrame >= total_frames) {
				currentFrame = 0;
				if (playOnce) {
					playStatus = false;
					animate();
				}
			}
			m_frame = currentFrame;
			m_layerController->layerSelector()->setActiveFrame(currentFrame);
			m_layerController->updateCananiUI();
			update();

			//Update Sliders
			m_layerController->jointEditorWidget()->initializeSliderGroup();

			//Update Scroll Window
			m_layerController->updateScroll();
		}


		void SheetCanvas::playCanvasSelected()
		{
			selectedFrameToPlay++;

			if (selectedFrameToPlay > selectedLastFrame) {
				selectedFrameToPlay = selectedFirstFrame;
				if (playOnce) {
					playStatus = false;
					//animateSelected();
					m_timer_selected->stop();	
				}
			}
			m_frame = selectedFrameToPlay;
			update();
			m_layerController->layerSelector()->setActiveFrame(selectedFrameToPlay);
			m_layerController->updateCananiUI();

			//Update Sliders
			m_layerController->jointEditorWidget()->initializeSliderGroup();

			//Update Scroll Window
			m_layerController->updateScroll();
		}


		int SheetCanvas::getFramesFromCurrentLayer() {
			return m_layerController->layerSelector()->activeLayer()->GetNumFrame();
		}

		void SheetCanvas::capFrameRate(double fps) {
			static double start = 0, diff, wait;
			wait = 1 / fps;
			diff = glfwGetTime() - start;
			if (diff < wait) {
				Sleep((wait - diff)*1000);
			}
			start = glfwGetTime();
		}

		QSize SheetCanvas::minimumSizeHint() const
		{
			return QSize(50, 50);
		}

		QSize SheetCanvas::sizeHint() const
		{
			return QSize(400, 400);
		}

		static void qNormalizeAngle(int &angle)
		{
			while (angle < 0)
				angle += 360;// *16;
			while (angle > 360)
				angle -= 360;// *16;
		}

		void SheetCanvas::setXRotation(int angle)
		{
		
			qNormalizeAngle(angle);
			if (angle != xRot) {
				xRot = angle;
				emit xRotationChanged(angle);
				updateGL();
			}
		}

		void SheetCanvas::setYRotation(int angle)
		{
		
			qNormalizeAngle(angle);
			if (angle != yRot) {
				yRot = angle;
				emit yRotationChanged(angle);
				updateGL();
			}
		}

		void SheetCanvas::setZRotation(int angle)
		{
			
			qNormalizeAngle(angle);
			if (angle != zRot) {
				zRot = angle;
				emit zRotationChanged(angle);
				updateGL();
			}
		}

		void SheetCanvas::initializeGL()
		{

			float  light0_position[] = { 10.0, 10.0, 10.0, 1.0 };
			float  light0_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
			float  light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
			float  light0_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
			glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
			glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
			glEnable(GL_LIGHT0);

			//
			glEnable(GL_LIGHTING);

			// 
			glEnable(GL_COLOR_MATERIAL);

			// 
			glEnable(GL_DEPTH_TEST);

			// 
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);



			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//BackGround color
			glClearColor(0.98, 0.98, 0.98, 0.0);
		}


		void SheetCanvas::paintGL()
		{

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			
			setupProjection();

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			if (canvasState == STATE_EDITOR) {
				glTranslatef(0, 0 - 2, -4 - m_camera_distance);
				glRotatef(0 / 16.0, 1.0, 0.0, 0.0);
				glRotatef(0 / 16.0, 0.0, 1.0, 0.0);
				glRotatef(0 / 16.0, 0.0, 0.0, 1.0);
			}
			else {
				glTranslatef(xTrans, yTrans - 2, -4 - m_camera_distance);
				glRotatef(xRot, 1.0, 0.0, 0.0);
				glRotatef(yRot, 0.0, 1.0, 0.0);
				glRotatef(zRot, 0.0, 0.0, 1.0);
			}
			
			//gluLookAt(0 + xTrans, 2 + yTrans, -4-m_camera_distance + zTrans, xTrans, yTrans, zTrans, 0.0f, 1.0f, 0.0f);

			float  light0_position[] = { 10.0, 10.0, 10.0, 1.0 };
			glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

			drawMessage(0, modeMessage);

			if (canvasState != STATE_BUILDER) {
				drawStage();

				//Draw BVH Skeleton
				RenderFigure(figure_scale);

				//Draw the Mesh first before Images so that it overlays
				//Resize generally
				glScaled(1.0 / 100, 1.0 / 100, 1);
				if (viewMesh) {
					for (int i = 0; i < m_imageGroup->size(); i++) {
						if (m_imageGroup->m_images[i].getMeshImage().getPointer() != NULL)
							renderMesh(m_imageGroup->m_images[i]);
					}
				}
				if (viewImages) {
					for (int i = 0; i < m_imageGroup->size(); i++) {
						ImageFile currentImg = m_imageGroup->m_images[i];
						if (m_imageGroup->m_images[i].m_imgTexture == 0)
							createTextureForGL(m_imageGroup->m_images[i]);
						else {
							glPushMatrix();
							m_imageGroup->m_images[i].setTransformMatrix(m_imageGroup->m_images[i].getImageRotation(), m_imageGroup->m_images[i].getImageXTrans(), m_imageGroup->m_images[i].getImageYTrans(), currentImg.getImageScale());
							GLfloat* matrix = m_imageGroup->m_images[i].getTransformMatrix();
							glMultMatrixf(matrix);
							renderImage(m_imageGroup->m_images[i]);
							glPopMatrix();
						}
					}
				}
				glScaled(100, 100, 1);				

				//glutSwapBuffers();
				if (m_IKEnabled){
					RenderIKBone();
				}


				if (canvasState == STATE_MAPPING) {
					char message1[50], message2[50];
					sprintf(message1, "X pos clicked: %f  , Y pos clicked: %f ", objX, objY);
					drawMessage(1, message1);

					if (m_imageGroup->size() != 0) {
						if (m_imageGroup->m_images[0].getMeshImage().getPointer() != NULL)
							TMeshImage* m_mesh = m_imageGroup->m_images[0].getMeshImage().getPointer();
				
					}
				}

			}
			else {
				char message1[50];
				sprintf(message1, "Currently Selected Vertex: %d", selectedVertex);
				drawMessage(1, message1);


				char debug[50];
				if (drawKey) {
					sprintf(debug, "Delete Key is Pressed");
				}
				else {
					sprintf(debug, "No key/random key pressed");
				}
				drawMessage(2, debug);
			}
				if (drawSkel != false)
					drawSkeleton();
			
		}


		void SheetCanvas::setupProjection() {
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			if (canvasState == STATE_BUILDER || canvasState == STATE_MAPPING) {
				glOrtho(-2, 2, -2, 2, 1.0, 15.0);
			}
			else if (canvasState == STATE_EDITOR) {
				glOrtho(-3, 3, -3, 3, -4.0, 15.0);
			}
			else {
				gluPerspective(45, viewRatio, 1, 500);
			}
		}

		void SheetCanvas::drawStage() {
			float  size = 3.0f;
			int  num_x = 5, num_z = 5;
			double  ox, oz;
			glBegin(GL_QUADS);
			glNormal3d(0.0, 1.0, 0.0);
			ox = -(num_x * size) / 2;
			for (int x = 0; x<num_x; x++, ox += size)
			{
				oz = -(num_z * size) / 2;
				for (int z = 0; z<num_z; z++, oz += size)
				{
					if (((x + z) % 2) == 0)
						glColor4f(1.0, 1.0, 1.0, 0.9);
					else
						glColor4f(0.8, 0.8, 0.8, 0.9);

					glVertex3d(ox, 0.0, oz);
					glVertex3d(ox, 0.0, oz + size);
					glVertex3d(ox + size, 0.0, oz + size);
					glVertex3d(ox + size, 0.0, oz);
				}
			}
			glFinish();
			glEnd();
		}

		void SheetCanvas::drawMessage(int line_no, const char * message)
		{
			int   i;
			if (message == NULL)
				return;

			// 
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(0.0, win_width, win_height, 0.0);

			// 
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			// 
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);

			//
			glColor3f(1.0, 0.0, 0.0);
			glRasterPos2i(8, 24 + 18 * line_no);
			for (i = 0; message[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);

			// ?
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}

		void SheetCanvas::resizeGL(int width, int height)
		{
			glViewport(0, 0, width, height);


			viewRatio = (float)width / (float)height;

			/*glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45, (double)width / height, 1, 500);*/
			//glOrtho(-2, +2, -2, +2, 1.0, 15.0);
			win_width = width;
			win_height = height;
		}

		void SheetCanvas::mousePressEvent(QMouseEvent *event)
		{
			lastPos = event->pos();
			this->setFocus();

			if (canvasState == STATE_BUILDER) {
				//Get position of mouse click
				mouseToWorld(lastPos.x(), lastPos.y());

				if (event->button() == Qt::LeftButton) {
					if (m_skeleton != NULL) {
						for (int i = 0; i < m_skeleton->getVertexCount(); i++) {
							SkeletonVertex* v1 = m_skeleton->getVertices()[i];

							QVector2D vertexCenter(v1->getXPos(), v1->getYPos());

							//Check for collision
							clickCollided = clickCollision(vertexCenter);

							//Update selected vertex if click collides
							if (clickCollided) {
								updateSelectedVertex(v1->getID());

								//Allow vertex to be moved
								vertexMoveSelected = true;
								break;
							}
						}
					}
				}

				update();
			}
			else if (canvasState == STATE_EDITOR) {
				int imageSelection = m_imageList->currentIndex().row();

				if (imageSelection != -1) {
					m_image = &m_imageGroup->m_images.at(imageSelection);
					zRot = m_image->getImageRotation();
					xTrans = m_image->getImageXTrans();
					yTrans = m_image->getImageYTrans();

				}
			}
			else if (canvasState == STATE_MAPPING) {
				//Get position of mouse click
				mouseToWorld(lastPos.x(), lastPos.y());
			}

			if (event->buttons() & Qt::MidButton && canvasState == STATE_CAMERA) {
				if (playStatus == false) {
					playStatus = true;
				}
				else {
					playStatus = false;
				}
				animate();
			}

		}

		void SheetCanvas::mouseMoveEvent(QMouseEvent *event)
		{

			update();

			float dx = event->x() - lastPos.x();
			float dy = event->y() - lastPos.y();

			lastPos = event->pos();
			if (event->buttons() == Qt::LeftButton && m_IKEnabled && canvasState != STATE_BUILDER){
				updateIKGoal(dx, dy, 0);
			}
			else if (event->buttons() == Qt::RightButton && m_IKEnabled && canvasState != STATE_BUILDER){
				updateIKGoal(dx, 0, dy);
			}
			else if (event->buttons() & Qt::LeftButton && canvasState == STATE_CAMERA) {
				xTrans += dx/16;
				yTrans += dy/16;
			}
			else if (event->buttons() & Qt::RightButton && canvasState == STATE_CAMERA) {
				setXRotation(xRot + 8 * dy);
				setZRotation(zRot + 8 * dx);
			}
			else if (event->buttons() & Qt::LeftButton && canvasState == STATE_EDITOR && m_image != NULL) {
				xTrans += dx;
				yTrans -= dy;
				m_image->setImageXTrans(xTrans);
				m_image->setImageYTrans(yTrans);
			}
			else if (event->buttons() & Qt::RightButton && canvasState == STATE_EDITOR && m_image != NULL) {
				setZRotation(zRot - dx/4);
				m_image->setImageRotation(zRot);
			}
			else if (canvasState == STATE_BUILDER) {
				if (vertexMoveSelected) {
					if (m_skeleton != NULL) {
						//Get position of mouse in world coordinates
						mouseToWorld(lastPos.x(), lastPos.y());
						SkeletonVertex* v0 = m_skeleton->getVertex(selectedVertex);
						v0->setXPos(objX);
						v0->setYPos(objY);
					}
				}
			}

		}

		void SheetCanvas::mouseReleaseEvent(QMouseEvent* event) {
			lastPos = event->pos();

			if (canvasState == STATE_BUILDER) {
				//Update no movement state on mouse release
				vertexMoveSelected = false;

				if (event->button() == Qt::LeftButton) {


					if (!clickCollided) {
						//Check if skeleton exists
						if (m_skeleton == NULL) {
							m_skeleton = new Skeleton();
							drawSkel = true;
						}

						//Get position of mouse release point
						mouseToWorld(lastPos.x(), lastPos.y());

						//Add vertex into skeleton structure
						SkeletonVertex* vertexToAdd = new SkeletonVertex(objX, objY);
						vertexToAdd->setID(idToAdd++);
						m_skeleton->addVertex(vertexToAdd);

						//Update selection vertex
						updateSelectedVertex(vertexToAdd->getID());
						//m_skeleton->setSelectedVertex(vertexToAdd->getID());
						//selectedVertex = vertexToAdd->getID();
					}
				}
			}
		}


		void SheetCanvas::wheelEvent(QWheelEvent *event) {
			if (canvasState == STATE_CAMERA) {
				//Mouse scrolls forward
				if (event->delta() > 0) {
					zoomInCamera();
				}
				else
					zoomOutCamera();
			}
			else if (canvasState == STATE_EDITOR) {

				if (m_imageList->currentIndex().row() != -1) {
					m_image = &m_imageGroup->m_images.at(m_imageList->currentIndex().row());
					zRot = m_image->getImageRotation();
					xTrans = m_image->getImageXTrans();
					yTrans = m_image->getImageYTrans();
				}

				float currentImageScale = m_image->getImageScale();

				if (event->delta() > 0) 
					currentImageScale += 0.02;
				else
					currentImageScale -= 0.02;
				m_image->setImageScale(currentImageScale);
				update();
			}
		}

		void SheetCanvas::mouseDoubleClickEvent(QMouseEvent *e) {

			if (canvasState == STATE_CAMERA) {
				if (e->button() == Qt::RightButton) {
					revertView();
				}
				else if (e->button() == Qt::LeftButton) {
					restartAnimation();
				}
			}
		}


		void SheetCanvas::SetupIKArm(Joint* endJoint, QString base_name){

			if (m_IKArm->get_size()){
				m_IKArm->clear();
			}
			Layer* activeLayer = m_layerController->layerSelector()->activeLayer();
			Joint* baseJoint = activeLayer->GetJoint(base_name.toStdString().c_str());
			m_IKBaseJoint = baseJoint;
			Joint* tmpForIKArm = endJoint;
			vector<Segment*> segs;
			Segment *new_seg;


			while (baseJoint->name.compare(tmpForIKArm->name) != 0){
				float boneLength = figure_scale * sqrtf(tmpForIKArm->offset[0] * tmpForIKArm->offset[0] + tmpForIKArm->offset[1] * tmpForIKArm->offset[1] + tmpForIKArm->offset[2] * tmpForIKArm->offset[2]);
				new_seg = new Segment(boneLength, BALLJOINT);
				segs.push_back(new_seg);
				tmpForIKArm = tmpForIKArm->parent;
			}

			std::reverse(segs.begin(), segs.end());

			//get the child indexes from the first joint to the last - for retrace purpose later

			vector<int> childChain;
			Joint* tmpForChildChain = baseJoint;
			while (tmpForChildChain->name.compare(activeLayer->GetJoint(0)->name) != 0){
				for (int i = 0; i < tmpForChildChain->parent->children.size(); i++){
					if (tmpForChildChain->parent->children.at(i)->name.compare(tmpForChildChain->name) == 0){
						childChain.push_back(i);
						tmpForChildChain = tmpForChildChain->parent;
						if (tmpForChildChain->parent == NULL) break;
					}	
				}
			}

			//get the joint indexes from the first joint to the last - to locate channel data

			vector<int> indexChain;
			Joint* tmpForIndexChain = baseJoint;

			for (int i = activeLayer->joints.size() - 1; i >= 0; i--){
				if (activeLayer->joints.at(i)->name.compare(tmpForIndexChain->name) == 0){
					indexChain.push_back(i);
					if (tmpForIndexChain->parent == NULL) break;
					tmpForIndexChain = tmpForIndexChain->parent;
				}
			}

			//initialize starting point of the IK arm base

			glm::vec4 originalPoint(0.0f, 0.0f, 0.0f, 1.0f);
			glm::vec4 transformedVector;

			glm::mat4 m = glm::mat4(1.0f);
			glm::mat4 modelt;
			glm::mat4 modelr;

			Joint* tmpJoint = activeLayer->GetJoint(0);
			FrameId activeFrame = m_layerController->layerSelector()->activeFrame();
			int channelOffset;
			float trans_x, trans_y, trans_z;
			while (true){
				if (tmpJoint->name.compare(activeLayer->GetJoint(0)->name) == 0){
					channelOffset = 3;
					trans_x = tmpJoint->offset[0] + activeLayer->sheetAtIndex(activeFrame)->getMotion()[0];
					trans_y = tmpJoint->offset[1] + activeLayer->sheetAtIndex(activeFrame)->getMotion()[1];
					trans_z = tmpJoint->offset[2] + activeLayer->sheetAtIndex(activeFrame)->getMotion()[2];
				}
				else{
					channelOffset = 0;
					trans_x = tmpJoint->offset[0];
					trans_y = tmpJoint->offset[1];
					trans_z = tmpJoint->offset[2];

				}
				modelt = glm::translate(glm::vec3(trans_x* figure_scale, trans_y * figure_scale, trans_z * figure_scale));
				
				modelr = glm::mat4(1.0f);
				for (int i = 0; i < 3; i++){
					switch (tmpJoint->channels.at(channelOffset + i)->type){
					case data::ChannelEnum::Xrotation: { modelr = modelr * glm::eulerAngleX(float(activeLayer->sheetAtIndex(activeFrame)->getMotion()[indexChain.back() * 3 + 3 + i] * M_PI / 180.0f)); break; }
					case data::ChannelEnum::Yrotation: { modelr = modelr * glm::eulerAngleY(float(activeLayer->sheetAtIndex(activeFrame)->getMotion()[indexChain.back() * 3 + 3 + i] * M_PI / 180.0f)); break; }
					case data::ChannelEnum::Zrotation: { modelr = modelr * glm::eulerAngleZ(float(activeLayer->sheetAtIndex(activeFrame)->getMotion()[indexChain.back() * 3 + 3 + i] * M_PI / 180.0f)); break; }
					}
				}

				m = m * (modelt * modelr);
				if (baseJoint->name.compare(tmpJoint->name) == 0){ break; }
				tmpJoint = tmpJoint->children.at(childChain.back());
				childChain.pop_back();
				indexChain.pop_back();
			}

			transformedVector = m * originalPoint;
			m_IKBase = Vector3f(transformedVector.x, transformedVector.y, transformedVector.z);

			m_goal = Vector3f(0, 0, 0);
			//set IKArm base to be 0,0,0 temporarily, because the IK arm library has bug when processing arms with a non-zero base 
			//stored m_IKBase separately and use it when drawing later 
			m_IKArm = new Arm(segs, Vector3f(0, 0, 0));
		}

		void SheetCanvas::RenderIKBone(){


			// drawing is done here
			m_IKArm->draw(m_IKBase.x(), m_IKBase.y(), m_IKBase.z());

			//the target small white triangle
			float c = 0.2;
			Point3f a0 = m_goal + Vector3f(-c, 0, c);
			Point3f a1 = m_goal + Vector3f(0, 0, -c);
			Point3f a2 = m_goal + Vector3f(c, 0, c);
			Vector3f n2(0, -1, 0);
			glBegin(GL_TRIANGLES);
			glNormal3f(n2[0], n2[1], n2[2]);
			glVertex3f(a0[0], a0[1], a0[2]);
			glVertex3f(a1[0], a1[1], a1[2]);
			glVertex3f(a2[0], a2[1], a2[2]);
			glEnd();

			glFlush();
			//glutSwapBuffers();					// swap buffers (we earlier set double buffer)

		}

		//inverse kinematics and openGL uses different coordinate systems
		//inverse kinematics: z+ up, x+ right, y+ from user to screen
		//openGL: y+ up, x+ right, z+ from screen to user

		void SheetCanvas::updateIKGoal(int x, int y, int z){
			float new_x = float(x)*0.01f + m_goal.x();
			float new_y = -float(y)*0.01f + m_goal.y();
			float new_z = -float(z)*0.01f + float(x)*0.01f + m_goal.z();
			m_goal = Vector3f(new_x, new_y, new_z);
			m_IKArm->solve(m_goal, 100);
		}


		///xiaocai_playAnimation added
		void SheetCanvas::animate()
		{
			if(playStatus)
			{
				double fps = m_layerController->getFrameRateForProject();
				int interval = 1.0/fps*1000;
				m_timer->start(interval);
				//playCanvas();
			}
			else
			{
				//m_timer->stop();
				stopAnimation();
			}
		}

		//yy selected frames to play
		void SheetCanvas::animateSelected()
		{
			//notify playing
			playStatus = true;
			//calculate interval and start timer
			double fps = m_layerController->getFrameRateForProject();
			int interval = 1.0 / fps * 1000;
			m_timer_selected->start(interval);
		}

		void SheetCanvas::stopAnimation() {
			m_timer_selected->stop();
			m_timer->stop();
			playStatus = false;
		}

		//Sent to LayerSelector in order to update FrameWidget
		void SheetCanvas::restartAnimation() {
			m_frame = 0;
			currentFrame = 0;
			
			m_layerController->layerSelector()->setActiveFrame(currentFrame);
			m_layerController->updateCananiUI();
		}


		bool SheetCanvas::getPlayStatus() {
			return playStatus;
		}

		void SheetCanvas::setPlayStatus(bool status) {
			playStatus = status;
		}

		void SheetCanvas::revertView() {
			SheetCanvas::setXRotation(0);
			SheetCanvas::setYRotation(0);
			SheetCanvas::setZRotation(0);
			xTrans = 0;
			yTrans = 0;
			m_camera_distance = 4.0f;
			update();
		}

		void SheetCanvas::togglePlayOnce(bool value) {
			playOnce = value;
		}

		void SheetCanvas::toggleViewImage() {
			viewImages = !viewImages;
		}

		void SheetCanvas::toggleViewMesh() {
			viewMesh = !viewMesh;
		}

		void SheetCanvas::keyPressEvent(QKeyEvent *event) {
			
			if (canvasState == STATE_CAMERA) {
				switch (event->key()) {
				case (Qt::Key_Left) :
					qDebug() << "Left Arrow Button Pressed.";
					setYRotation(yRot + 100);
					break;
				case (Qt::Key_Right) :
					setYRotation(yRot - 100);
					break;
				case (Qt::Key_Up) :
					setXRotation(xRot + 100);
					break;
				case (Qt::Key_Down) :
					setXRotation(xRot - 100);
					break;
				case (Qt::Key_W) :
					zoomInCamera();
					break;
				case (Qt::Key_A) :
					xTrans += 0.4f;
					updateGL();
					break;
				case (Qt::Key_S) :
					zoomOutCamera();
					break;
				case (Qt::Key_D) :
					xTrans -= 0.4f;
					updateGL();
					break;
				}
			}
			else if (canvasState == STATE_BUILDER) {
				if (event->key() == Qt::Key_Delete) {
					drawKey = true;

					if (m_skeleton != NULL) {
						if (m_skeleton->getVertexCount() != 0) {
							//Get vertex to delete and parentId
							int idToDelete = selectedVertex;
							int vertexParent = m_skeleton->getVertex(selectedVertex)->getParent();

							//Update new selected vertex to parent and reconnect children to parent
							if (vertexParent != -1) {
								updateSelectedVertex(vertexParent);
								m_skeleton->removeVertex(idToDelete);
							}
						}
					}

				}
				else
					drawKey = false;
			}

			if (event->key() == Qt::Key_Space) {
				int newState = canvasState + 1;
				if (newState > 2)
					newState = 1;
				updateState(newState);
				updateGL();
			}

		}


		void SheetCanvas::renderImage(ImageFile img) {
			glDisable(GL_LIGHTING);
			glEnable(GL_BLEND);
			glDisable(GL_CULL_FACE);
			glEnable(GL_TEXTURE_2D);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1, 1, 1, 1);

			
			glBindTexture(GL_TEXTURE_2D, img.m_imgTexture);
			glFrontFace(GL_FRONT_AND_BACK);

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0, 0);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(img.getQImage().width(), 0);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(img.getQImage().width(), img.getQImage().height());
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0, img.getQImage().height());
			glEnd();

			glDisable(GL_TEXTURE_2D);	
			glDisable(GL_BLEND);
		}

		void SheetCanvas::renderMesh(ImageFile img) {
			glEnable(GL_BLEND);
			glEnable(GL_LINE_SMOOTH);
			glDisable(GL_LIGHTING);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Retrieve mesh dpi
			TPointD meshDpi;
			img.getMeshImage()->getDpi(meshDpi.x, meshDpi.y);

			//Resize mesh image
			//glScaled(/*Stage::inch */ 0.148 /*/ meshDpi.x*/, /*Stage::inch /*/ 0.148/*/ meshDpi.y*/, 1.0);

			glColor4f(0.0, 1.0, 0.0, 0.7); // Translucent green

			tglDrawEdges(*img.getMeshImage());


			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_BLEND);
		}


		void SheetCanvas::createTextureForGL(ImageFile &image) {
			QImage tex;
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);
			glAlphaFunc(GL_GREATER, 0.1f);
			glEnable(GL_ALPHA_TEST);

			QImage fixedImage(image.getQImage().width(), image.getQImage().height(), QImage::Format_ARGB32);
			QPainter painter(&fixedImage);
			painter.setCompositionMode(QPainter::CompositionMode_Source);
			painter.fillRect(fixedImage.rect(), Qt::transparent);
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
			painter.drawImage(0, 0, image.getQImage());

			painter.end();

			tex = QGLWidget::convertToGLFormat(fixedImage);
			glGenTextures(1, &image.m_imgTexture);
			glBindTexture(GL_TEXTURE_2D, image.m_imgTexture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_TEXTURE_2D);
		}



		void SheetCanvas::updateState(int state) {
			int oldState = canvasState;
			
			if (oldState == STATE_CAMERA) {
				//Store the old translatation & rotation values
				xTransStored = xTrans;
				yTransStored = yTrans;
				zTransStored = zTrans;
				xRotStored = xRot;
				yRotStored = yRot;
				zRotStored = zRot;

				//resetValues
				xRot = 0, yRot = 0, zRot = 0;
				xTrans = 0, yTrans = 0, zTrans = 0;
			}
			canvasState = state;

			switch (canvasState) {
			case STATE_CAMERA:
				sprintf(modeMessage, "Current Mode: Camera");
				//Restore camera settings
				xTrans = xTransStored;
				yTrans = yTransStored;
				zTrans = zTransStored;
				xRot = xRotStored;
				yRot = yRotStored;
				zRot = zRotStored;
				break;
			case STATE_EDITOR:
				sprintf(modeMessage, "Current Mode: Editor");
				
				//Update current selected image if any
				if (m_imageList->currentIndex().row() != -1) {
					m_image = &m_imageGroup->m_images.at(m_imageList->currentIndex().row());
					zRot = m_image->getImageRotation();
					xTrans = m_image->getImageXTrans();
					yTrans = m_image->getImageYTrans();
				}
				break;
			case STATE_BUILDER:
				sprintf(modeMessage, "Current Mode: Build Skeleton");
				revertView();
				break;
			case STATE_MAPPING:
				sprintf(modeMessage, "Current Mode: Skeleton Mapping");
				revertView();
				break;
			default:
				break;
			}
		}


		void SheetCanvas::drawVertex(SkeletonVertex* v) {
			double x = v->getXPos();
			double y = v->getYPos();
			qglColor(Qt::blue);
			glBegin(GL_POLYGON);
			glVertex2d(x - vertexRadius, y - vertexRadius);
			glVertex2d(x + vertexRadius, y - vertexRadius);
			glVertex2d(x + vertexRadius, y + vertexRadius);
			glVertex2d(x - vertexRadius, y + vertexRadius);
			glEnd();
			update();
		}

		void SheetCanvas::drawBone(SkeletonVertex* v1) {
			int parentID = v1->getParent();

			//Check if parent from vertex exists
			if (parentID != -1) {
				SkeletonVertex* v2 = NULL;
				v2 = m_skeleton->getVertex(parentID);

				if (v2 == NULL)
					return;

				glDisable(GL_LIGHTING);
				qglColor(QColor(250, 184, 70));
				glLineWidth(2.0f);  // Yellow/Orange-ish line center

				glBegin(GL_LINES);
				glVertex2d(v1->getXPos(), v1->getYPos());
				glVertex2d(v2->getXPos(), v2->getYPos());
				glEnd();


				qglColor(Qt::black);
				glLineWidth(4.0f);  // Black border

				glBegin(GL_LINES);
				glVertex2d(v1->getXPos(), v1->getYPos());
				glVertex2d(v2->getXPos(), v2->getYPos());
				glEnd();
			}
		}

		void SheetCanvas::drawSkeleton() {
			for (int i = 0; i < m_skeleton->getVertexCount(); i++) {

				drawVertex(m_skeleton->getVertices()[i]);
				drawBone(m_skeleton->getVertices()[i]);
			}

			highlightSelectedVertex();
		}


		void SheetCanvas::highlightSelectedVertex() {
			if (selectedVertex < 0)
				return;

			SkeletonVertex* vertexSelected;
			for (int i = 0; i < m_skeleton->getVertexCount(); i++) {
				if (m_skeleton->getVertices().at(i)->getID() == selectedVertex)
					vertexSelected = m_skeleton->getVertices().at(i);
			}


			float highlightBorder = vertexRadius + 0.005;


			qglColor(Qt::red);
			glLineWidth(2.0f);
			glBegin(GL_LINE_LOOP);
			glVertex2d(vertexSelected->getXPos() - highlightBorder, vertexSelected->getYPos() - highlightBorder);
			glVertex2d(vertexSelected->getXPos() + highlightBorder, vertexSelected->getYPos() - highlightBorder);
			glVertex2d(vertexSelected->getXPos() + highlightBorder, vertexSelected->getYPos() + highlightBorder);
			glVertex2d(vertexSelected->getXPos() - highlightBorder, vertexSelected->getYPos() + highlightBorder);
			glEnd();
			update();
		}


		void SheetCanvas::mouseToWorld(double x, double y) {
			//Use of QT to determine x, y position on the widget
			x = this->mapFromGlobal(QCursor::pos()).x();
			y = this->mapFromGlobal(QCursor::pos()).y();

			//Setting up of view, model, projection matrices
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);

			GLdouble modelview[16];
			glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

			GLdouble projection[16];
			glGetDoublev(GL_PROJECTION_MATRIX, projection);

			GLfloat winX, winY, winZ;


			winX = (float)x;
			//Convert normal coordinate to OpenGL-format coordinate
			winY = (float)viewport[3] - y;

			//Obtain depth
			glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

			//Obtain values for world coordinates
			gluUnProject(winX, winY, winZ, modelview, projection, viewport, &objX, &objY, &objZ);
		}


		bool SheetCanvas::clickCollision(QVector2D pointCenter) {
			QVector2D mouseClickPos(objX, objY);

			float distance = pointCenter.distanceToPoint(mouseClickPos);

			if (distance <= vertexRadius * 2)
				return true;
			else
				return false;
		}

		void SheetCanvas::updateSelectedVertex(int id) {
			selectedVertex = id;
			m_skeleton->setSelectedVertex(id);
		}

	}
}