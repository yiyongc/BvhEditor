#pragma once
#include <qwidget.h>
#include <QHBoxLayout>
#include <qslider.h>
#include "../QsLog2/qslog.h"
#include <qobject.h>
#include <qcombobox.h>
#include "../data/Layer.h"
#include "../controllers/LayerController.h"
#include "SheetCanvas.h"
#include <QDoubleSpinBox>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <QMessageBox>
#include "FrameWidget.h"
#include "LayerCreatorWidget.h"


#define MAX_OFFSET 60
#define MIN_OFFSET -60
#define MAX_ROTATION 180
#define MIN_ROTATION -180
#define MAX_POSITION 300
#define MIN_POSITION -300
#define MAXFRAMERATE 1000

using namespace cacani::data;
using namespace cacani::controller;

namespace cacani {

	namespace controller{

		class LayerController;
		class LayerSelector;
	}



	namespace ui {

		class SheetCanvas;
		class FrameWidget;
		class LayerCreatorWidget;

		enum SliderLinkPos
		{
			
			Xposition, Yposition, Zposition
		
		};

		enum SliderLinkOffset
		{
			
			Xoffset, Yoffset, Zoffset

		};

		enum SliderLinkRot
		{
			Xrotation, Yrotation, Zrotation
		};


		class JointEditorWidget:public QWidget
		{
			Q_OBJECT

		public:
			JointEditorWidget(LayerController* layerController, SheetCanvas* sheetCanvas);
			~JointEditorWidget();

			///In the futute, there will be updateFrame() and updateFromCanvasChange()
			void updateJoint(Joint* joint);
			void initializeJoint(Layer* layer, Joint* joint);
			void initializeJoint(Layer* layer);
			//void initializeWidget(LayerController* myLayerController, Layer* myLayer);
			void initializeLayerSelector(LayerSelector* myLayerSelector){ m_layerSelector = myLayerSelector; }
			void initializeFrameWidget(FrameWidget* myFrameWidget){ m_frameWidget = myFrameWidget; }
			void initializeLayerCreatorWidget(LayerCreatorWidget* myLayerCreatorWidget){ m_layerCreatorWidget = myLayerCreatorWidget; }
			void updateFrame(FrameId new_frame){ m_frame = new_frame; }
			FrameId getFrameId(){ return m_frame; }

			void initializeSliderGroup();

			Layer* getCurrentLayer(){ return m_layer; }

		private:
			void initializeSlider(QSlider* qslider, SliderLinkOffset linkType);
			void initializeSlider(QSlider* qslider, SliderLinkPos linkType);
			void initializeSlider(QSlider* qslider, SliderLinkRot linkType);
			void initializeCombobox(QComboBox* m_comboBox_rot_order);



		private slots:
			void OffsetXHasChanged(int);
			void OffsetYHasChanged(int);
			void OffsetZHasChanged(int);
			void PosXHasChanged(int);
			void PosYHasChanged(int);
			void PosZHasChanged(int);
			void RotXHasChanged(int);
			void RotYHasChanged(int);
			void RotZHasChanged(int);
			void ComboBoxSelectionHasChanged(int);
			void ProjFPSHasChanged();
			void EnableIK();
			void ApplyIK();
			void ExitIK();
			void UpdateIKBaseJoint(int);

		private:
			QSlider* m_slider_offset_x;
			QSlider* m_slider_offset_y;
			QSlider* m_slider_offset_z;
			QSlider* m_slider_position_x;
			QSlider* m_slider_position_y;
			QSlider* m_slider_position_z;
			QSlider* m_slider_rot_x;
			QSlider* m_slider_rot_y;
			QSlider* m_slider_rot_z;
			QComboBox* m_comboBox_rot_order;
			QDoubleSpinBox* m_proj_fps;
			QPushButton* m_button_fps;
			QPushButton* m_button_ik_enable;
			QPushButton* m_button_ik_apply;
			QPushButton* m_button_ik_exit;
			QComboBox* m_comboBox_ik_base_joint;
			QSpinBox* m_spinBox_ik_num_frames; //number of in-betwn frames to generate

			//This is because channel data depends on different frames. joint itself does not have rotation data
			//joint is just for tranlation data here, and will be universal
			//layer is just for rotation data here
			//need to discuss on joint translation data --- should we keep at sheet or layer level
			//currently here is rundandent and dangerous
			Layer* m_layer;
			Joint* m_joint;
			FrameId m_frame;
			LayerController* m_layerController;
			SheetCanvas* m_sheetCanvas;
			LayerSelector* m_layerSelector;
			FrameWidget* m_frameWidget;
			LayerCreatorWidget* m_layerCreatorWidget;
		};

	}
}