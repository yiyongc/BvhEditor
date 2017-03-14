#pragma once
#ifndef BVH_PLAYER_SHEET_CANVAS_H
#define BVH_PLAYER_SHEET_CANVAS_H
#include <QtOpenGL/QGLWidget>
//#include <../data/Sheet.h>
#include <../data/Layer.h>
#include <../data/LayerGroup.h>
#include <../data/FrameId.h>
#include <../controllers/LayerController.h>
#include <windows.h>
#include "../QsLog2/qslog.h"
#include <qtimer.h>
#include "../data/Segment.h"
#include "../data/Arm.h"
#include "../OpenToonz/meshbuilder.h"
#include "../data/ImageFile.h"
#include "../ui/ImageListWidget.h"
#include "../OpenToonz/meshutils.h"
#include "Skeleton.h"
#include "../MeshDeformer/RigidMeshDeformer2D.h"

//using namespace cacani::controller;

namespace cacani {

	namespace data{
		class Sheet;
		class Layer;
		class LayerGroup;
	}

	namespace controller{
		class LayerController;
	}

	namespace ui {

		class SheetCanvas : public QGLWidget
		{

			Q_OBJECT

			enum State {
				STATE_CAMERA = 1,
				STATE_EDITOR = 2,
				STATE_BUILDER = 3,
				STATE_MAPPING = 4
			};

		public:

			SheetCanvas(cacani::controller::LayerController* layerController, QWidget *parent = 0);
			~SheetCanvas();

			void setImageList(ImageListWidget* imageList);
			//	void  RenderFigure(int frame_no, float scale = 1.0f);
			void  RenderFigure(float scale = 1.0f);
			void  RenderFigure(cacani::data::Joint * root, const double * data, float scale = 1.0f, bool descendent = false);
			static void  RenderBone(float x0, float y0, float z0, float x1, float y1, float z1);
			void updateBase(cacani::data::LayerGroup* new_base){ m_base = new_base; update(); }
			void updateFrame(cacani::data::FrameId new_frame){ currentFrame = new_frame;  m_frame = new_frame; update(); }
			cacani::data::FrameId getFrameId(){ return m_frame; }
			void zoomInCamera(){ m_camera_distance--; update(); }
			void zoomOutCamera(){ m_camera_distance++; update(); }

			/*void playCanvas();*/ //xiaocai_playAnimation, commented by xiaocai
			void capFrameRate(double fps);
			void RenderIKBone();
			void SetupIKArm(Joint* base, QString end_joint_name);
			void updateIKGoal(int x, int y, int z);
			void toggleIKStatus(){ m_IKEnabled = !m_IKEnabled; };
			Arm* getIKArm(){ return m_IKArm; }


			void setFirstFrame(int first) { selectedFirstFrame = first;  selectedFrameToPlay = selectedFirstFrame; }
			void setLastFrame(int last) { selectedLastFrame = last; }
			
			ImageGroup* m_imageGroup;
			ImageFile* m_image;
		protected:
			void initializeGL();
			void paintGL();
			void resizeGL(int width, int height);

			QSize minimumSizeHint() const;
			QSize sizeHint() const;
			void mousePressEvent(QMouseEvent *event);
			void mouseMoveEvent(QMouseEvent *event);
			void mouseReleaseEvent(QMouseEvent *event);
			virtual void wheelEvent(QWheelEvent *event);
			void mouseDoubleClickEvent(QMouseEvent *event);
			void keyPressEvent(QKeyEvent *event);

			void createTextureForGL(ImageFile &img);

			public slots:
				// slots for xyz-rotation slider
				void setXRotation(int angle);
				void setYRotation(int angle);
				void setZRotation(int angle);
				void drawMessage(int line_no, const char * message);

		signals:
				// signaling rotation from mouse movement
				void xRotationChanged(int angle);
				void yRotationChanged(int angle);
				void zRotationChanged(int angle);

		private:
			void drawStage();

			int xRot;
			int yRot;
			int zRot;
			int xRotStored = 0, yRotStored = 0, zRotStored = 0;
			float xTrans = 0;
			float yTrans = 0;
			float zTrans = 0;
			float xTransStored = 0, yTransStored = 0, zTransStored = 0;
			float viewRatio = 1;
			
			float imageScale = 1;
			

			QPoint lastPos;
			int selectedFirstFrame;
			int selectedLastFrame;
			int selectedFrameToPlay;

			cacani::data::Sheet* m_sheet;
			cacani::data::LayerGroup* m_base;
			cacani::controller::LayerController* m_layerController;
			cacani::data::FrameId m_frame;
			ImageListWidget* m_imageList;
			float m_camera_distance;

			int canvasState;
			char modeMessage[30];
			
			void setupProjection();

			Arm* m_IKArm;
			Point3f m_goal;
			Point3f m_IKBase;
			bool m_IKEnabled;
			Joint* m_IKBaseJoint;

			bool playOnce = false;
			bool playStatus = false;
			bool viewImages = false;
			bool viewMesh = false;

			//Build skeleton related
			bool drawSkel = false;
			Skeleton* m_skeleton;
			float vertexRadius = 0.020;
			int idToAdd = 0;
			bool vertexMoveSelected = false;
			int selectedVertex = -1;
			bool clickCollided = false;
			bool drawKey = false;
			GLdouble objX = 0.0, objY = 0.0, objZ = 0.0;

			void drawVertex(SkeletonVertex* v);
			void drawBone(SkeletonVertex* v1);
			void highlightSelectedVertex();
			void mouseToWorld(double x, double y);
			bool clickCollision(QVector2D pointCenter);
			int collisionMesh();
			void updateSelectedVertex(int id);
			void drawSkeleton();

			// Mesh deformer related
			bool m_bConstraintsValid;
			std::set<unsigned int> m_vSelected;
			unsigned int m_nSelected;
			TTextureMesh* m_deformedMesh;
			RigidMeshDeformer2D m_deformer;
			bool meshInitialized = false;

			void InvalidateConstraints();
			void ValidateConstraints();
			void InitializeDeformedMesh();
			void UpdateDeformedMesh();
			void drawHandles();

		public:
			void animate();
			void animateSelected();
			void stopAnimation();
			bool getPlayStatus();
			void setPlayStatus(bool statusToSet);
			void revertView();
			void restartAnimation();
			void togglePlayOnce(bool playOnce);
			int getFramesFromCurrentLayer();
			QTimer* m_timer;
			QTimer* m_timer_selected;
			int	currentFrame = 0;
			
			void toggleViewImage();
			void toggleViewMesh();
			void renderImage(ImageFile img);
			void renderMesh(ImageFile img);
			
			void updateState(int state);

			private slots:
				void playCanvas();
				void playCanvasSelected(); 
		};


	}
}
#endif
