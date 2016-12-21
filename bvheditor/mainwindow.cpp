#include "mainwindow.h"
//#include "qdebug.h"


MainWindow::MainWindow(QApplication* app, QWidget *parent)
	: QMainWindow(parent),
	m_root(new LayerGroup(false)),
	m_FCCWidget(new QWidget),
	m_LayerGroup(new LayerGroup()),
	m_ViewSettings(new ViewSettings()),
	m_LayerController(new LayerController()),
	m_CACStateMachine(new CACStateMachine()),
	m_app(app),
	m_success(0),
	importedImage(NULL),
	convertedImage(NULL),
	meshImage(NULL),
	EditorState(STATE_CAMERA)
{
	QLOG_INFO() << "MainWindow:" << "Started";

	//-------------------------------------------------------------------------------------
	//								Menu bar set up
	//-------------------------------------------------------------------------------------
	QMenuBar* menubar = new QMenuBar();

	//First menu button
	QMenu* menu1 = new QMenu("File");
	QAction* action_import = new QAction("Import BVH File", menu1);
	QAction* action_export = new QAction("Export BVH File", menu1);
	
	menu1->addAction(action_import);
	menu1->addAction(action_export);
	
	//First menu connections
	connect(action_import, SIGNAL(triggered(bool)), this, SLOT(importNewFile()));
	connect(action_export, SIGNAL(triggered(bool)), this, SLOT(exportFile()));


	//second menu button
	QMenu* menu2 = new QMenu("Image");
	QAction* action_import_image = new QAction("Import Image File", menu2);
	QAction* action_create_mesh = new QAction("Build Mesh from Image", menu2);
	QAction* action_view_images = new QAction("View Images", menu2);
	action_view_images->setCheckable(true);
	action_view_images->setChecked(false);
	QAction* action_view_meshes = new QAction("View Meshes", menu2);
	action_view_meshes->setCheckable(true);
	action_view_meshes->setChecked(false);
	QAction* action_delete_images = new QAction("Delete All Images", menu2);


	menu2->addAction(action_import_image);
	menu2->addAction(action_create_mesh);
	menu2->addAction(action_view_images);
	menu2->addAction(action_view_meshes);
	menu2->addAction(action_delete_images);

	//second menu connections
	connect(action_import_image, SIGNAL(triggered(bool)), this, SLOT(importNewImage()));
	connect(action_create_mesh, SIGNAL(triggered(bool)), this, SLOT(createMesh()));
	connect(action_view_images, SIGNAL(triggered(bool)), this, SLOT(toggleViewImage()));
	connect(action_view_meshes, SIGNAL(triggered(bool)), this, SLOT(toggleViewMesh()));
	connect(action_delete_images, SIGNAL(triggered(bool)), this, SLOT(deleteAllImages()));

	//third menu button
	QMenu* menu3 = new QMenu("View");
	QAction* action_view_toolbar = new QAction("Toolbar", menu3);
	action_view_toolbar->setCheckable(true);
	action_view_toolbar->setChecked(true);

	menu3->addAction(action_view_toolbar);

	//third menu connections
	connect(action_view_toolbar, SIGNAL(triggered(bool)), this, SLOT(viewToolBar(bool)));

	//Mode menu button
	QMenu* menu4 = new QMenu("Mode");
	QAction* action_camera_mode = new QAction("Camera Mode", menu4);
	QAction* action_edit_mode = new QAction("Edit Mode", menu4);
	QAction* action_build_mode = new QAction("Build Skeleton Mode", menu4);

	menu4->addAction(action_camera_mode);
	menu4->addAction(action_edit_mode);
	menu4->addAction(action_build_mode);

	//Mode menu connections
	connect(action_camera_mode, SIGNAL(triggered(bool)), this, SLOT(cameraMode()));
	connect(action_edit_mode, SIGNAL(triggered(bool)), this, SLOT(editorMode()));
	connect(action_build_mode, SIGNAL(triggered(bool)), this, SLOT(builderMode()));


	//Play menu button
	QMenu* menu5 = new QMenu("Play");
	QAction* action_play_all = new QAction("Play Whole BVH File", menu5);
	QAction* action_play_selected = new QAction("Play Selected Frames", menu5);
	QAction* action_stop = new QAction("Stop and Restart Animation", menu5);

	menu5->addAction(action_play_all);
	menu5->addAction(action_play_selected);
	menu5->addAction(action_stop);

	//Play menu connections
	connect(action_play_all, SIGNAL(triggered(bool)), this, SLOT(playAnimation()));
	connect(action_play_selected, SIGNAL(triggered(bool)), this, SLOT(playSelected()));
	connect(action_stop, SIGNAL(triggered(bool)), this, SLOT(stopAnimation()));


	//Frames menu button
	QMenu* menu6 = new QMenu("Frames");
	QAction* action_add_frames = new QAction("Insert New", menu6);
	QMenu* menu61 = new QMenu("Quick Insert");
	QAction* action_add_1 = new QAction("Insert 1 Frame", menu61);
	QAction* action_add_5 = new QAction("Insert 5 Frames", menu61);
	QAction* action_add_10 = new QAction("Insert 10 Frames", menu61);
	QMenu* menu62 = new QMenu("Delete");
	QAction* action_delete_current = new QAction("Delete Current Frame", menu62);
	QAction* action_delete_all = new QAction("Delete All Frames", menu62);
	QAction* action_delete_selected = new QAction("Delete Selected Frames", menu62);
	QAction* action_make_T = new QAction("T Pose Current Frame", menu6);
	QAction* action_ground_current = new QAction("Ground Current Frame", menu6);
	
	//SubMenu - Add
	menu61->addAction(action_add_1);
	menu61->addAction(action_add_5);
	menu61->addAction(action_add_10);

	//SubMenu - Delete
	menu62->addAction(action_delete_current);
	menu62->addAction(action_delete_all);
	menu62->addAction(action_delete_selected);
	
	menu6->addAction(action_add_frames);
	menu6->addMenu(menu61);
	menu6->addMenu(menu62);
	menu6->addAction(action_make_T);
	menu6->addAction(action_ground_current);
	
	//Frames menu connections
	connect(action_add_frames, SIGNAL(triggered(bool)), this, SLOT(addFrames()));
	connect(action_add_1, SIGNAL(triggered(bool)), this, SLOT(addOne()));
	connect(action_add_5, SIGNAL(triggered(bool)), this, SLOT(addFive()));
	connect(action_add_10, SIGNAL(triggered(bool)), this, SLOT(addTen()));
	connect(action_delete_current, SIGNAL(triggered(bool)), this, SLOT(deleteCurrentFrame()));
	connect(action_delete_all, SIGNAL(triggered(bool)), this, SLOT(deleteAllFrames()));
	connect(action_delete_selected, SIGNAL(triggered(bool)), this, SLOT(deleteSelectedFrames()));
	connect(action_make_T, SIGNAL(triggered(bool)), this, SLOT(goToTPose()));
	connect(action_ground_current, SIGNAL(triggered(bool)), this, SLOT(groundModel()));

	
	//Help menu button
	QMenu* menu7 = new QMenu("Help");
	QAction* action_view_controls = new QAction("View Controls", menu7);

	//Help Menu Connections	
	connect(action_view_controls, SIGNAL(triggered(bool)), this, SLOT(viewControls()));

	menu7->addAction(action_view_controls);

	//Set up menubar
	menubar->addMenu(menu1);
	menubar->addMenu(menu2);
	menubar->addMenu(menu3);
	menubar->addMenu(menu4);
	menubar->addMenu(menu5);
	menubar->addMenu(menu6);
	menubar->addMenu(menu7);

	


	//-------------------------------------------------------------------------------------
	//								Toolbar set up
	//-------------------------------------------------------------------------------------
	toolbar = new QToolBar();
	toolbar->setOrientation(Qt::Vertical);
	toolbar->setMovable(false);
	setContextMenuPolicy(Qt::NoContextMenu);

	//Toolbar action creation
	QAction* action_zoom_in = new QAction(QIcon(":/images/zoomin.png"), "Zoom In", this);
	QAction* action_zoom_out = new QAction(QIcon(":/images/zoomout.png"), "Zoom Out", this);
	QAction* action_play = new QAction(QIcon(":/images/play.png"), "Play/Pause BVH File", this);
	QAction* action_revert_view = new QAction(QIcon(":/images/return.png"), "Revert to Original View", this);
	QAction* action_restart_animation = new QAction(QIcon(":/images/restart.png"), "Restart Animation to First Frame", this);
	QAction* action_repeat_play = new QAction(QIcon(":/images/repeat.png"), "Layers Will Play Repeatedly if Checked", this);
	action_repeat_play->setCheckable(true);
	action_repeat_play->setChecked(true);
	QAction* action_play_select = new QAction(QIcon(":/images/playselect.png"), "Select Specific Frames to Play", this);
	QAction* action_stop_all = new QAction(QIcon(":/images/stop.png"), "Stop and Restart Animation to First Frame", this);
	QAction* action_set_T = new QAction(QIcon(":/images/Tpose.png"), "Set current frame to a T pose", this);
	QAction* action_ground_model = new QAction(QIcon(":/images/ground.png"), "Reset Y Offset and Ground the Model", this);

	toolbar->addAction(action_play);
	toolbar->addAction(action_play_select);
	toolbar->addAction(action_stop_all);
	toolbar->addAction(action_repeat_play);
	toolbar->addAction(action_restart_animation);
	toolbar->addAction(action_zoom_in);
	toolbar->addAction(action_zoom_out);
	toolbar->addAction(action_revert_view);
	toolbar->addAction(action_set_T);
	toolbar->addAction(action_ground_model);
	
	addToolBar(Qt::LeftToolBarArea, toolbar);

	//Toolbar button connections
	connect(action_zoom_in, SIGNAL(triggered(bool)), this, SLOT(zoomIn()));
	connect(action_zoom_out, SIGNAL(triggered(bool)), this, SLOT(zoomOut()));
	connect(action_play, SIGNAL(triggered(bool)), this, SLOT(playAnimation()));
	connect(action_revert_view, SIGNAL(triggered(bool)), this, SLOT(revertView()));
	connect(action_restart_animation, SIGNAL(triggered(bool)), this, SLOT(restartAnimation()));
	connect(action_repeat_play, SIGNAL(triggered(bool)), this, SLOT(togglePlayOnce(bool)));
	connect(action_play_select, SIGNAL(triggered(bool)), this, SLOT(playSelected()));
	connect(action_stop_all, SIGNAL(triggered(bool)), this, SLOT(stopAnimation()));
	connect(action_set_T, SIGNAL(triggered(bool)), this, SLOT(goToTPose()));
	connect(action_ground_model, SIGNAL(triggered(bool)), this, SLOT(groundModel()));


	//-------------------------------------------------------------------------------------
	//								Initialize UI Components
	//-------------------------------------------------------------------------------------

	//set up layergroups and a demo layer
	m_LayerController->setCurrentRoot(m_root);

	startup();

	if (m_success){

		m_SheetCanvas = new cacani::ui::SheetCanvas(m_LayerController);


		QSplitter *split1 = new QSplitter;
		QSplitter *split2 = new QSplitter;
		QSplitter *split3 = new QSplitter;
		split3->setOrientation(Qt::Vertical);

		QVBoxLayout *layout = new QVBoxLayout;

		QWidget *container = new QWidget;

		QVBoxLayout *container_layout = new QVBoxLayout;

		Layer* initial_layer = dynamic_cast<const LayerGroup*>(m_root->childAtIndex(0))->childAtIndex(0);
		m_JointTreeListModel = new JointTreeListModel(initial_layer);
		
		// Left Column UI

		m_JointTreeWidget = new cacani::ui::JointTreeWidget(m_LayerController, m_SheetCanvas, m_JointTreeListModel);
		
		m_ImageListWidget = new ImageListWidget(m_SheetCanvas->m_imageGroup);

		QWidget* jointTreeContainer = new QWidget; 
		QVBoxLayout* jointTreeLayout = new QVBoxLayout;
		QLabel* jointTreeHeader = new QLabel("Skeleton Hierachy");
		jointTreeLayout->addWidget(jointTreeHeader);
		jointTreeLayout->addWidget(m_JointTreeWidget);
		jointTreeContainer->setLayout(jointTreeLayout);

		QWidget* imageListContainer = new QWidget;
		QVBoxLayout* imageListLayout = new QVBoxLayout;
		QLabel* imageListHeader = new QLabel("Image Files Imported");
		QPushButton* buttonImageImport = new QPushButton("Import New Image");
		QPushButton* buttonImageDelete = new QPushButton("Delete Image");
		QHBoxLayout* imageButtonLayout = new QHBoxLayout;
		imageButtonLayout->addWidget(buttonImageImport);
		imageButtonLayout->addWidget(buttonImageDelete);
		QWidget* imageButtonWidget = new QWidget;
		imageButtonWidget->setLayout(imageButtonLayout);
		imageListLayout->addWidget(imageListHeader);
		imageListLayout->addWidget(m_ImageListWidget);
		imageListLayout->addWidget(imageButtonWidget);
		imageListContainer->setLayout(imageListLayout);
		connect(buttonImageImport, SIGNAL(clicked()), this, SLOT(importNewImage()));
		connect(buttonImageDelete, SIGNAL(clicked()), this, SLOT(deleteImage()));

		split3->addWidget(jointTreeContainer);
		split3->addWidget(imageListContainer);
		split3->setStretchFactor(0, 1);
		split3->setStretchFactor(1, 0);

		QWidget* wnd = new QWidget;
		QWidget* mainArea = new QWidget;
		m_JointEditorWidget = new cacani::ui::JointEditorWidget(m_LayerController, m_SheetCanvas);
		m_LayerController->attachJointEditorWidget(m_JointEditorWidget);

		m_JointEditorWidget->initializeJoint(initial_layer);


		m_LayerSelector = new LayerSelector(m_LayerController, m_SheetCanvas, m_JointEditorWidget, m_JointTreeWidget);
		m_LayerSelector->setJointTreeListModel(m_JointTreeListModel);
		m_LayerController->attachLayerSelector(m_LayerSelector);
		m_JointEditorWidget->initializeLayerSelector(m_LayerSelector);

		

		split1->addWidget(split3);
		split1->addWidget(m_SheetCanvas);

		container_layout->addWidget(split1);
		container->setLayout(container_layout);

		split2->setOrientation(Qt::Vertical);
		split2->addWidget(container);

		m_FCContainer = new cacani::ui::FCContainer(m_root, m_ViewSettings, m_LayerController, m_LayerSelector, m_CACStateMachine, m_FCCWidget);


		m_JointEditorWidget->initializeFrameWidget(m_FCContainer->getFrameWidget());
		m_JointEditorWidget->initializeLayerCreatorWidget(m_FCContainer->getLayerCreatorWidget());

		m_LayerController->attachFCContainer(m_FCContainer);
		split2->addWidget(m_FCContainer);
		layout->addWidget(split2);

		//Testing
		/*myView = new QLabel;
		layout->addWidget(myView);*/

		mainArea->setLayout(layout);


		QHBoxLayout* mainLayout = new QHBoxLayout;
		mainLayout->addWidget(mainArea);
		mainLayout->addWidget(m_JointEditorWidget);
		mainLayout->setStretch(0, 3);
		mainLayout->setStretch(1, 1);

		QWidget* body = new QWidget;
		body->setLayout(mainLayout);
		QVBoxLayout* bodyAndMenu = new QVBoxLayout;
		bodyAndMenu->addWidget(menubar);
		bodyAndMenu->addWidget(body);

		wnd->setLayout(bodyAndMenu);

		setCentralWidget(wnd);
		setWindowTitle(tr("BVH Designer"));
		setWindowState(Qt::WindowMaximized);
	}
	else{
		m_success = 0;
		close(); 
	}

}

MainWindow::~MainWindow() {}

void MainWindow::startup(){
	StartupDialog* startupDialog = new StartupDialog(m_LayerController,this);
	startupDialog->exec();
}

//-----------------------------------------------------------------------------------------------------------
//                                       File Related Slots
//-----------------------------------------------------------------------------------------------------------

void MainWindow::importNewFile(){
	//Stop playing the file to prevent program from crashing when importing
	stopAnimation();

	QString filename = QFileDialog::getOpenFileName(
		this,
		tr("Open BVH File"),
		"",
		"BVH file (*.bvh);;"
		);

	if (filename != NULL){
		ImportDialog* layerControllerSetting = new ImportDialog(m_LayerController, filename);
		layerControllerSetting->exec();
	}

}

void MainWindow::exportFile(){
	stopAnimation();

	ExportDialog* exportDialog = new ExportDialog(m_LayerController);
	exportDialog->exec();
}

void MainWindow::importNewImage(){
	//Stop all playing
	stopAnimation();
	
	imageFilePath = QFileDialog::getOpenFileName(
		this,
		tr("Import PNG file"),
		"",
		"PNG file (*.png)");
	
	if (imageFilePath != NULL) {
		importedImage = new QImage;
		importedImage->load(imageFilePath);
		
		QFileInfo fileInfo(imageFilePath);
		imageFileName = fileInfo.completeBaseName();

		//Display in view
		//myView->setPixmap(QPixmap::fromImage(*importedImage));

		ImageFile img(imageFileName, *importedImage);

		m_SheetCanvas->m_imageGroup->m_images.push_back(img);

		m_ImageListWidget->update();
		m_ImageListWidget->item(0)->setSelected(true);
		m_ImageListWidget->setCurrentRow(0);
		
	}
}

void MainWindow::convertImage(QImage* img) {

	/*if (!importedImage)	{
		QErrorMessage* error = new QErrorMessage;
		error->showMessage("No imported image!");
	}
	else {*/
		convertedImage = rasterFromQImage(*img);
	/*}*/
}

void MainWindow::createMesh() {
	int imageSelection = m_ImageListWidget->currentIndex().row();

	if (imageSelection != -1) {
		QImage* selectedImage = &m_SheetCanvas->m_imageGroup->m_images.at(imageSelection).getQImage();
	
		MeshBuilderOptions(opts);

		//Set up options
		MeshOptionDialog* meshOptionDialog = new MeshOptionDialog(&opts);
		int retCode = meshOptionDialog->exec();

		if (retCode == QDialog::Accepted) {

			convertImage(selectedImage);

			meshImage = buildMesh(convertedImage, opts);
			TPointD meshDpi(300, 300);
			TPointD rasDpi(300, 300);
			TPointD worldOriginPos(0, 0);

			transform(meshImage, TScale(meshDpi.x / rasDpi.x, meshDpi.y / rasDpi.y) *	TTranslation(-worldOriginPos));

			meshImage->setDpi(meshDpi.x, meshDpi.y);
		

			m_SheetCanvas->m_imageGroup->m_images.at(imageSelection).setMeshImage(meshImage);
		}
	}
	else {
		int reply = QMessageBox::question(this, "Error!", "No selected/imported image to convert to mesh!", QMessageBox::Ok);

		if (reply)
			importNewImage();
	}
}

void MainWindow::toggleViewImage() {
	m_SheetCanvas->toggleViewImage();
	m_SheetCanvas->update();
}

void MainWindow::toggleViewMesh() {
	m_SheetCanvas->toggleViewMesh();
}

void MainWindow::deleteImage() {
	if (m_SheetCanvas->m_imageGroup->size() == 0) {
		QErrorMessage* error = new QErrorMessage;
		error->showMessage("No image to delete!");
	}
	else {
		int imageToDelete = m_ImageListWidget->currentIndex().row();
		
		if (imageToDelete != -1)
			m_SheetCanvas->m_imageGroup->m_images.erase(m_SheetCanvas->m_imageGroup->m_images.begin() + imageToDelete);
		else {
			QErrorMessage* error = new QErrorMessage;
			error->showMessage("No image selected for delete!");
		}

		m_ImageListWidget->update();
		m_ImageListWidget->item(0)->setSelected(true);
		m_ImageListWidget->setCurrentRow(0);
		m_SheetCanvas->update();
	}
}

void MainWindow::deleteAllImages() {
	if (m_SheetCanvas->m_imageGroup->size() == 0) {
		QErrorMessage* error = new QErrorMessage;
		error->showMessage("No image to delete!");
	}
	else {
		m_SheetCanvas->m_imageGroup->m_images.clear();
		QMessageBox* successMsg = new QMessageBox;
		successMsg->setText("Successfully removed all images");
		successMsg->show();
		m_ImageListWidget->update();
	}
}

//-----------------------------------------------------------------------------------------------------------
//                                       State Related Slots
//-----------------------------------------------------------------------------------------------------------

void MainWindow::cameraMode() {
	m_SheetCanvas->updateState(STATE_CAMERA);
}

void MainWindow::editorMode() {
	m_SheetCanvas->updateState(STATE_EDITOR);
}

void MainWindow::builderMode() {
	m_SheetCanvas->updateState(STATE_BUILDER);
}



//-----------------------------------------------------------------------------------------------------------
//                                       User-BVH Interactions
//-----------------------------------------------------------------------------------------------------------

void MainWindow::playAnimation() {
	bool playStatus = m_SheetCanvas->getPlayStatus();

	if (playStatus == false) {
		m_SheetCanvas->setPlayStatus(true);
	}
	else {
		m_SheetCanvas->setPlayStatus(false);
	}

	m_SheetCanvas->animate();

	//Make currentFrame show as activeframe in the framewidget and update view
	m_LayerSelector->setActiveFrame(m_SheetCanvas->currentFrame);
	m_LayerController->updateCananiUI();
}

void MainWindow::zoomIn(){ 
	m_SheetCanvas->zoomInCamera(); 
}

void MainWindow::zoomOut(){ 
	m_SheetCanvas->zoomOutCamera(); 
}

void MainWindow::restartAnimation() {
	m_SheetCanvas->restartAnimation();
	m_FCContainer->getFrameWidget()->update();
}


void MainWindow::stopAnimation() {
	m_SheetCanvas->stopAnimation();
	m_SheetCanvas->restartAnimation();
}

//-----------------------------------------------------------------------------------------------------------
//                                       User Interface Related
//-----------------------------------------------------------------------------------------------------------


void MainWindow::viewToolBar(bool isChecked) {
	this->toolbar->setHidden(!isChecked);
}

void MainWindow::viewControls() {
	ViewCtrlDialog* viewCtrlDialog = new ViewCtrlDialog();
	viewCtrlDialog->exec();
}


//-----------------------------------------------------------------------------------------------------------
//                                       BVH Editor Tools
//-----------------------------------------------------------------------------------------------------------


void MainWindow::togglePlayOnce(bool value) {
	m_SheetCanvas->togglePlayOnce(!value);
}

void MainWindow::playSelected() {
	if (m_SheetCanvas->getFramesFromCurrentLayer() == 1) {
		QMessageBox msgBox(QMessageBox::Warning, tr("Insufficient Frames"), tr("Sorry, there are insufficient frames to be played."),
			QMessageBox::Ok, this);

		QPalette mainPalette = this->palette();
		mainPalette.setColor(QPalette::WindowText, QColor(0, 0, 0));
		msgBox.setPalette(mainPalette);
		int ret = msgBox.exec();
	}
	else {
		SelectPlayDialog* selectPlayDialog = new SelectPlayDialog(m_SheetCanvas);
		selectPlayDialog->exec();
	}
}

void MainWindow::revertView() {
	m_SheetCanvas->revertView();
}



void MainWindow::addFrames() {
	InsertFrameDialog* insertFrameDialog = new InsertFrameDialog(m_SheetCanvas, m_LayerController, m_LayerSelector);
	insertFrameDialog->exec();
}


void MainWindow::addOne() {
	m_SheetCanvas->stopAnimation();
	
	m_LayerController->addFrames(m_LayerSelector->activeFrame(), 1);
	m_SheetCanvas->update();
	m_FCContainer->getFrameWidget()->update();
	
	//Show current frame within scroll window
	m_FCContainer->updateScrollView();
}

void MainWindow::addFive() {
	m_SheetCanvas->stopAnimation();

	m_LayerController->addFrames(m_LayerSelector->activeFrame(), 5);
	m_SheetCanvas->update();
	m_FCContainer->getFrameWidget()->update();

	//Show current frame within scroll window
	m_FCContainer->updateScrollView();
}

void MainWindow::addTen() {
	m_SheetCanvas->stopAnimation();

	m_LayerController->addFrames(m_LayerSelector->activeFrame(), 10);
	m_SheetCanvas->update();
	m_FCContainer->getFrameWidget()->update();

	//Show current frame within scroll window
	m_FCContainer->updateScrollView();
}

void MainWindow::deleteCurrentFrame() {
	m_SheetCanvas->stopAnimation();

	//Show current frame within scroll window
	m_FCContainer->updateScrollView();

	QMessageBox msgBox(QMessageBox::Question, tr("Delete Frames"), tr("Delete Current Frame?"),
		QMessageBox::Ok | QMessageBox::Cancel, this);

	QPalette mainPalette = this->palette();
	mainPalette.setColor(QPalette::WindowText, QColor(0, 0, 0));
	msgBox.setPalette(mainPalette);
	int ret = msgBox.exec();

	if (ret == QMessageBox::Ok)	{
		int maxFrames = m_LayerSelector->activeLayer()->GetNumFrame();
		int activeFrame = m_LayerSelector->activeFrame();
		
		//Check if there is only 1 frame
		if (maxFrames == 1) {
			addOne();
			activeFrame++;
		}
		else {
			//Check if it is last index that is being deleted
			if (activeFrame == (maxFrames - 1)) {
				m_LayerSelector->setActiveFrame(activeFrame - 1);
			}
		}

		m_LayerController->removeFrameSeq(m_LayerSelector->activeLayerId(), m_LayerSelector->activeLayerId().index(), activeFrame, 1);

		//update view
		m_LayerController->updateCananiUI();

		//Update Sliders
		m_LayerController->jointEditorWidget()->initializeSliderGroup();
	}
}

void MainWindow::deleteAllFrames() {
	m_SheetCanvas->stopAnimation();

	QMessageBox msgBox(QMessageBox::Question, tr("Delete Frames"), tr("Delete All Frames?"),
		QMessageBox::Ok | QMessageBox::Cancel, this);

	QPalette mainPalette = this->palette();
	mainPalette.setColor(QPalette::WindowText, QColor(0, 0, 0));
	msgBox.setPalette(mainPalette);
	int ret = msgBox.exec();

	if (ret == QMessageBox::Ok)	{
		int maxFrames = m_LayerSelector->activeLayer()->GetNumFrame();
		
		//Add 1 frame and remove the rest
		m_LayerController->addFrames(0, 1);

		//Make added frame show as activeframe
		m_LayerSelector->setActiveFrame(0);

		//Remove remaining frames
		m_LayerController->removeFrameSeq(m_LayerSelector->activeLayerId(), m_LayerSelector->activeLayerId().index(), 1, maxFrames);

		//update view
		m_LayerController->updateCananiUI();

		//Update Sliders
		m_LayerController->jointEditorWidget()->initializeSliderGroup();

		//Show current frame within scroll window
		m_FCContainer->updateScrollView();
	}

}

void MainWindow::deleteSelectedFrames() {
	m_SheetCanvas->stopAnimation();

	DeleteSelectedDialog* deleteSelectedDialog = new DeleteSelectedDialog(m_SheetCanvas, m_LayerController, m_LayerSelector);
	deleteSelectedDialog->exec();
}

void MainWindow::goToTPose() {
	//m_LayerController->makeTPose(m_LayerSelector->activeFrame());
	m_LayerController->makeTPose(m_SheetCanvas->currentFrame);

	//Update sheetcanvas view
	m_SheetCanvas->update();

	//Make currentFrame show as activeframe in the framewidget and update view
	m_LayerSelector->setActiveFrame(m_SheetCanvas->currentFrame);
	m_LayerController->updateCananiUI();

	//Update Sliders
	m_LayerController->jointEditorWidget()->initializeSliderGroup();
}

void MainWindow::groundModel() {
	//m_LayerController->layerSelector()->activeLayer()->groundModel(m_LayerController->layerSelector()->activeFrame());
	m_LayerController->layerSelector()->activeLayer()->groundModel(m_SheetCanvas->currentFrame);
	
	//Update sheetcanvas view
	m_SheetCanvas->update();

	//Make currentFrame show as activeframe in the framewidget and update view
	m_LayerSelector->setActiveFrame(m_SheetCanvas->currentFrame);
	m_LayerController->updateCananiUI();

	//Update Sliders
	m_LayerController->jointEditorWidget()->initializeSliderGroup();
}
