#include "JointEditorWidget.h"
#include "qlabel.h"

namespace cacani {

	namespace data{
		enum ChannelOrder;
	}

	namespace ui {


		JointEditorWidget::JointEditorWidget(LayerController* layerController, SheetCanvas* sheetCanvas):
			m_slider_offset_x (new QSlider()),
			m_slider_offset_y (new QSlider()),
			m_slider_offset_z (new QSlider()),
			m_slider_position_x (new QSlider()),
			m_slider_position_y (new QSlider()),
			m_slider_position_z (new QSlider()),
			m_slider_rot_x (new QSlider()),
			m_slider_rot_y (new QSlider()),
			m_slider_rot_z (new QSlider()),
			m_comboBox_rot_order (new QComboBox()),
			m_proj_fps (new QDoubleSpinBox()),
			m_button_fps (new QPushButton("Apply FPS")),
			m_button_ik_enable(new QPushButton("Enable IK")),
			m_button_ik_apply(new QPushButton("Apply IK")),
			m_button_ik_exit(new QPushButton("Exit IK")),
			m_comboBox_ik_base_joint(new QComboBox()),
			m_spinBox_ik_num_frames(new QSpinBox()),
			m_layer (NULL),
			m_joint (NULL),
			m_layerController (layerController),
			m_sheetCanvas (sheetCanvas)
		{
			m_proj_fps->setRange(0.1, MAXFRAMERATE);
			m_proj_fps->setDecimals(1);
			m_proj_fps->setValue(m_layerController->getFrameRateForProject());

			//initialize # of in-btwn frames to gen = 1
			m_spinBox_ik_num_frames->setValue(1);

			//disable IK-related UI items until enable is hit
			m_button_ik_apply->setDisabled(1);
			m_button_ik_exit->setDisabled(1);
			m_comboBox_ik_base_joint->setDisabled(1);
			m_spinBox_ik_num_frames->setDisabled(1);
			
			//Working with 2D for now
			m_slider_rot_z->setDisabled(1);
			m_slider_position_z->setDisabled(1);
			m_slider_offset_z->setDisabled(1);


			m_comboBox_rot_order->addItem("XYZ");
			m_comboBox_rot_order->addItem("XZY");
			m_comboBox_rot_order->addItem("YXZ");
			m_comboBox_rot_order->addItem("YZX");
			m_comboBox_rot_order->addItem("ZXY");
			m_comboBox_rot_order->addItem("ZYX");


			connect(m_slider_offset_x, SIGNAL(valueChanged(int)), this, SLOT(OffsetXHasChanged(int)));
			connect(m_slider_offset_y, SIGNAL(valueChanged(int)), this, SLOT(OffsetYHasChanged(int)));
			connect(m_slider_offset_z, SIGNAL(valueChanged(int)), this, SLOT(OffsetZHasChanged(int)));
			connect(m_slider_position_x, SIGNAL(valueChanged(int)), this, SLOT(PosXHasChanged(int)));
			connect(m_slider_position_y, SIGNAL(valueChanged(int)), this, SLOT(PosYHasChanged(int)));
			connect(m_slider_position_z, SIGNAL(valueChanged(int)), this, SLOT(PosZHasChanged(int)));
			connect(m_slider_rot_x, SIGNAL(valueChanged(int)), this, SLOT(RotXHasChanged(int)));
			connect(m_slider_rot_y, SIGNAL(valueChanged(int)), this, SLOT(RotYHasChanged(int)));
			connect(m_slider_rot_z, SIGNAL(valueChanged(int)), this, SLOT(RotZHasChanged(int)));
			connect(m_comboBox_rot_order, SIGNAL(currentIndexChanged(int)), this, SLOT(ComboBoxSelectionHasChanged(int)));
			connect(m_button_fps, SIGNAL(clicked(bool)), this, SLOT(ProjFPSHasChanged()));
			connect(m_button_ik_enable, SIGNAL(clicked(bool)), this, SLOT(EnableIK()));
			connect(m_button_ik_apply, SIGNAL(clicked(bool)), this, SLOT(ApplyIK()));
			connect(m_button_ik_exit, SIGNAL(clicked(bool)), this, SLOT(ExitIK()));
			connect(m_comboBox_ik_base_joint, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateIKBaseJoint(int)));


			QLabel* lfps = new QLabel("Set Project FPS");
			QLabel* l1 = new QLabel();
			l1->setText("X Offset");
			QLabel* l2 = new QLabel();
			l2->setText("Y Offset");
			QLabel* l3 = new QLabel();
			l3->setText("Z Offset");

			QLabel* l4 = new QLabel();
			l4->setText("X Position");
			QLabel* l5 = new QLabel();
			l5->setText("Y Position");
			QLabel* l6 = new QLabel();
			l6->setText("Z Position");

			QLabel* l7 = new QLabel();
			l7->setText("X Rotation");
			QLabel* l8 = new QLabel();
			l8->setText("Y Rotation");
			QLabel* l9 = new QLabel();
			l9->setText("Z Rotation");

			QLabel* l10 = new QLabel();
			l10->setText("Rotation Order");

			QLabel* l11 = new QLabel("Select End Node: ");
			QLabel* l12 = new QLabel("Specify Number of In-between Frames: ");

			QHBoxLayout* fpsLayout = new QHBoxLayout;
			fpsLayout->addWidget(lfps);
			fpsLayout->addWidget(m_proj_fps);
			fpsLayout->addWidget(m_button_fps);


			QHBoxLayout* labelTransLayout = new QHBoxLayout;
			labelTransLayout->addWidget(l1);
			labelTransLayout->addWidget(l2);
			labelTransLayout->addWidget(l3);


			QHBoxLayout* sliderTransLayout = new QHBoxLayout;
			sliderTransLayout->addWidget(m_slider_offset_x);
			sliderTransLayout->addWidget(m_slider_offset_y);
			sliderTransLayout->addWidget(m_slider_offset_z);

			QHBoxLayout* labelPosLayout = new QHBoxLayout;
			labelPosLayout->addWidget(l4);
			labelPosLayout->addWidget(l5);
			labelPosLayout->addWidget(l6);


			QHBoxLayout* sliderPosLayout = new QHBoxLayout;
			sliderPosLayout->addWidget(m_slider_position_x);
			sliderPosLayout->addWidget(m_slider_position_y);
			sliderPosLayout->addWidget(m_slider_position_z);

			QHBoxLayout* labelRotLayout = new QHBoxLayout;
			labelRotLayout->addWidget(l7);
			labelRotLayout->addWidget(l8);
			labelRotLayout->addWidget(l9);

			QHBoxLayout* sliderRotLayout = new QHBoxLayout;
			sliderRotLayout->addWidget(m_slider_rot_x);
			sliderRotLayout->addWidget(m_slider_rot_y);
			sliderRotLayout->addWidget(m_slider_rot_z);

			QHBoxLayout* rotOrderLayout = new QHBoxLayout;
			rotOrderLayout->addWidget(l10);
			rotOrderLayout->addWidget(m_comboBox_rot_order);

			QHBoxLayout* ikLayoutSec0 = new QHBoxLayout;
			ikLayoutSec0->addWidget(m_button_ik_enable);
			QWidget* ikWidget0 = new QWidget;
			ikWidget0->setLayout(ikLayoutSec0);

			QHBoxLayout* ikLayoutSec1 = new QHBoxLayout;
			ikLayoutSec1->addWidget(l11);
			ikLayoutSec1->addWidget(m_comboBox_ik_base_joint);
			QWidget* ikWidget1 = new QWidget;
			ikWidget1->setLayout(ikLayoutSec1);

			QHBoxLayout* ikLayoutSec2 = new QHBoxLayout;
			ikLayoutSec2->addWidget(l12);
			ikLayoutSec2->addWidget(m_spinBox_ik_num_frames);
			QWidget* ikWidget2 = new QWidget;
			ikWidget2->setLayout(ikLayoutSec2);

			QHBoxLayout* ikLayoutSec3 = new QHBoxLayout;
			ikLayoutSec3->addWidget(m_button_ik_apply);
			ikLayoutSec3->addWidget(m_button_ik_exit);
			QWidget* ikWidget3 = new QWidget;
			ikWidget3->setLayout(ikLayoutSec3);

			QVBoxLayout* ikLayout = new QVBoxLayout;
			ikLayout->addWidget(ikWidget0);
			ikLayout->addWidget(ikWidget1);
			ikLayout->addWidget(ikWidget2);
			ikLayout->addWidget(ikWidget3);

			QWidget* inputFps = new QWidget;
			inputFps->setLayout(fpsLayout);
			QWidget* labelTrans = new QWidget;
			labelTrans->setLayout(labelTransLayout);
			QWidget* sliderTrans = new QWidget;
			sliderTrans->setLayout(sliderTransLayout);
			QWidget* labelPos = new QWidget;
			labelPos->setLayout(labelPosLayout);
			QWidget* sliderPos = new QWidget;
			sliderPos->setLayout(sliderPosLayout);
			QWidget* labelRot = new QWidget;
			labelRot->setLayout(labelRotLayout);
			QWidget* sliderRot = new QWidget;
			sliderRot->setLayout(sliderRotLayout);
			QWidget* rotOrder = new QWidget;
			rotOrder->setLayout(rotOrderLayout);
			QWidget* ikWidget = new QWidget;
			ikWidget->setLayout(ikLayout);

			QVBoxLayout* sliderGroupLayout = new QVBoxLayout;

			sliderGroupLayout->addWidget(inputFps);
			sliderGroupLayout->addWidget(labelTrans);
			sliderGroupLayout->addWidget(sliderTrans);
			sliderGroupLayout->addWidget(labelPos);
			sliderGroupLayout->addWidget(sliderPos);
			sliderGroupLayout->addWidget(rotOrder);
			sliderGroupLayout->addWidget(labelRot);
			sliderGroupLayout->addWidget(sliderRot);
			sliderGroupLayout->addWidget(ikWidget);

			setLayout(sliderGroupLayout);
		}


		JointEditorWidget::~JointEditorWidget()
		{
			delete m_layer;
		}

		//xiaocai added
		void JointEditorWidget::OffsetXHasChanged(int value){ m_layerController->updateJointOffset(m_layer, m_joint, Xoffset, value); m_sheetCanvas->update(); }
		void JointEditorWidget::OffsetYHasChanged(int value){ m_layerController->updateJointOffset(m_layer, m_joint, Yoffset, value); m_sheetCanvas->update(); }
		void JointEditorWidget::OffsetZHasChanged(int value){ m_layerController->updateJointOffset(m_layer, m_joint, Zoffset, value); m_sheetCanvas->update(); }

		// commented by xiaocai 
		/*void JointEditorWidget::OffsetXHasChanged(int value){ m_layerController->updateJointOffset(m_joint, Xoffset, value); m_sheetCanvas->update(); }
		void JointEditorWidget::OffsetYHasChanged(int value){ m_layerController->updateJointOffset(m_joint, Yoffset, value); m_sheetCanvas->update(); }
		void JointEditorWidget::OffsetZHasChanged(int value){ m_layerController->updateJointOffset(m_joint, Zoffset, value); m_sheetCanvas->update(); }*/
		void JointEditorWidget::PosXHasChanged(int value){ m_layerController->updateRootPosition(m_sheetCanvas->getFrameId(), m_layer, Xposition, value); m_sheetCanvas->update(); }
		void JointEditorWidget::PosYHasChanged(int value){ m_layerController->updateRootPosition(m_sheetCanvas->getFrameId(), m_layer, Yposition, value); m_sheetCanvas->update(); }
		void JointEditorWidget::PosZHasChanged(int value){ m_layerController->updateRootPosition(m_sheetCanvas->getFrameId(), m_layer, Zposition, value); m_sheetCanvas->update(); }
		void JointEditorWidget::RotXHasChanged(int value){ m_layerController->updateJointRotation(m_sheetCanvas->getFrameId(), m_layer, m_joint, Xrotation, value); m_sheetCanvas->update(); }
		void JointEditorWidget::RotYHasChanged(int value){ m_layerController->updateJointRotation(m_sheetCanvas->getFrameId(), m_layer, m_joint, Yrotation, value); m_sheetCanvas->update(); }
		void JointEditorWidget::RotZHasChanged(int value){ m_layerController->updateJointRotation(m_sheetCanvas->getFrameId(), m_layer, m_joint, Zrotation, value); m_sheetCanvas->update(); }
		void JointEditorWidget::ComboBoxSelectionHasChanged(int index) { m_layerController->updateJointRotOrder(m_layer, m_joint, static_cast<ChannelOrder>(index)); m_sheetCanvas->update(); }

		void JointEditorWidget::ProjFPSHasChanged(){ 
			if (m_layerController->getFrameRateForProject() != m_proj_fps->value()){	
				m_layerController->updateFrameRateForProject(m_proj_fps->value());
				m_sheetCanvas->update();
				QMessageBox msgBox;
				msgBox.setText("The change in frame rate was applied to the entire project.");
				msgBox.exec();
			}
		}

		void JointEditorWidget::EnableIK(){


			if (m_joint->parent == NULL){
				QMessageBox msgBox;
				msgBox.setText("Please select a non-root joint from joint tree to begin.");
				msgBox.exec();
				return;
			}


			//enable ik related components
			m_button_ik_enable->setDisabled(1);
			m_button_ik_apply->setDisabled(0);
			m_button_ik_exit->setDisabled(0);
			m_comboBox_ik_base_joint->setDisabled(0);
			m_spinBox_ik_num_frames->setDisabled(0);

			//freeze layercreator and framewidget, because in ik mode changing of frames or layers is illegal
			m_frameWidget->setDisabled(1);
			m_layerCreatorWidget->setDisabled(1);

			Joint* curJoint = m_joint;
			while (curJoint->parent != NULL){
				curJoint = curJoint->parent;
				m_comboBox_ik_base_joint->addItem(QString::fromStdString(curJoint->name));
			} 

			//duplicate frame and reset focus to the new frame
			m_layerController->duplicateCurrentFrame();
			m_layerSelector->setActiveFrame(m_layerSelector->activeFrame() + 1);

			//m_joint is the IK arm base, and m_comboBox_ik_base_joint is the end of the arm
			//m_sheetCanvas->SetupIKArm(m_joint, m_comboBox_ik_base_joint->currentText());
			m_sheetCanvas->toggleIKStatus();
			m_sheetCanvas->update();
			m_frameWidget->update();
		}
		
		void JointEditorWidget::ApplyIK(){

			m_layerController->applyIK(m_sheetCanvas->getIKArm(), m_joint, m_layerSelector->activeLayer()->GetJoint(m_comboBox_ik_base_joint->currentText().toStdString().c_str()));

			//generate inbetween frames
			m_layerController->inbetween(m_spinBox_ik_num_frames->value());
			m_layerSelector->setActiveFrame(m_layerSelector->activeFrame() + m_spinBox_ik_num_frames->value());
			m_sheetCanvas->update();
			m_frameWidget->update();

			//disable IK
			m_button_ik_enable->setDisabled(0);
			m_button_ik_apply->setDisabled(1);
			m_button_ik_exit->setDisabled(1);
			m_comboBox_ik_base_joint->setDisabled(1);
			m_comboBox_ik_base_joint->clear();
			m_spinBox_ik_num_frames->setDisabled(1);
			m_frameWidget->setDisabled(0);
			m_layerCreatorWidget->setDisabled(0);
			m_sheetCanvas->toggleIKStatus();

		}


		void JointEditorWidget::ExitIK(){

			//delete duplicate frame and reset focus to original frame
			m_layerController->removeCurrentFrame();
			m_layerSelector->setActiveFrame(m_layerSelector->activeFrame() - 1);
			m_sheetCanvas->update();
			m_frameWidget->update();

			//disable IK
			m_button_ik_enable->setDisabled(0);
			m_button_ik_apply->setDisabled(1);
			m_button_ik_exit->setDisabled(1);
			m_comboBox_ik_base_joint->setDisabled(1);
			m_comboBox_ik_base_joint->clear();
			m_spinBox_ik_num_frames->setDisabled(1);
			m_frameWidget->setDisabled(0);
			m_layerCreatorWidget->setDisabled(0);
			m_sheetCanvas->toggleIKStatus();

		}

		void JointEditorWidget::UpdateIKBaseJoint(int){
			if(m_comboBox_ik_base_joint->currentText()!=NULL)
				m_sheetCanvas->SetupIKArm(m_joint, m_comboBox_ik_base_joint->currentText());
		}


		void JointEditorWidget::initializeSlider(QSlider* qslider, SliderLinkOffset linkType)
		{
			qslider->setMinimum(MIN_OFFSET);
			qslider->setMaximum(MAX_OFFSET);
			QLOG_INFO() << "JointEditorWidget initializeSlider value is " << m_joint->offset[linkType];

			qslider->blockSignals(true);
			qslider->setValue(m_layerController->getJointOffset(m_joint, linkType));
			qslider->blockSignals(false);
		}

		void JointEditorWidget::initializeSlider(QSlider* qslider, SliderLinkRot linkType){

			qslider->setMinimum(MIN_ROTATION);
			qslider->setMaximum(MAX_ROTATION);

			qslider->blockSignals(true);
			qslider->setValue(m_layerController->getJointRotation(m_sheetCanvas->getFrameId(), m_layer, m_joint, linkType));
			qslider->blockSignals(false);

		}

		void JointEditorWidget::initializeSlider(QSlider* qslider, SliderLinkPos linkType){

			qslider->setMinimum(MIN_POSITION);
			qslider->setMaximum(MAX_POSITION);

			qslider->blockSignals(true);
			qslider->setValue(m_layerController->getRootPosition(m_sheetCanvas->getFrameId(), m_layer, linkType));
			qslider->blockSignals(false);

		}

		void JointEditorWidget::initializeCombobox(QComboBox* m_comboBox_rot_order){

			m_comboBox_rot_order->blockSignals(true);
			m_comboBox_rot_order->setCurrentIndex(m_layerController->getJointRotOrder(m_joint));
			m_comboBox_rot_order->blockSignals(false);
		}

		// xiaocai cms: selected joint
		void JointEditorWidget::updateJoint(Joint* joint){

			m_joint = joint;
			initializeSliderGroup();

		}

		// 
		void JointEditorWidget::initializeSliderGroup(){

			initializeSlider(m_slider_offset_x, Xoffset);
			initializeSlider(m_slider_offset_y, Yoffset);
			initializeSlider(m_slider_offset_z, Zoffset);
			initializeSlider(m_slider_position_x, Xposition);
			initializeSlider(m_slider_position_y, Yposition);
			initializeSlider(m_slider_position_z, Zposition);
			initializeSlider(m_slider_rot_x, Xrotation);
			initializeSlider(m_slider_rot_y, Yrotation);
			initializeSlider(m_slider_rot_z, Zrotation);
			initializeCombobox(m_comboBox_rot_order);
		}

		void JointEditorWidget::initializeJoint(Layer* layer, Joint* joint){
			m_layer = layer;
			m_joint = joint;
			initializeSliderGroup();
		}

		void JointEditorWidget::initializeJoint(Layer* layer){
			initializeJoint(layer, layer->GetJoint(0));
		}

	}
}