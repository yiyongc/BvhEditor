#include "SkeletonMapDialog.h"

SkeletonMapDialog::SkeletonMapDialog(cacani::data::LayerGroup* layerGroup, unordered_multimap<int, pair<int, int>> *m, int selectedVert, int imageIndex) :
m_layerGroup(layerGroup),
m_joints(new QComboBox),
m_Button_Confirm(new QPushButton("Confirm")),
m_Button_Cancel(new QPushButton("Cancel")),
m_map(m),
selected(selectedVert),
selectedImage(imageIndex)
{
	//Setup link for the 2 buttons
	connect(m_Button_Confirm, SIGNAL(clicked(bool)), this, SLOT(confirmClicked()));
	connect(m_Button_Cancel, SIGNAL(clicked(bool)), this, SLOT(cancelClicked()));
	
	const cacani::data::LayerGroup* curLayerGroup;
	cacani::data::Layer* curLayer;

	curLayerGroup = dynamic_cast<const cacani::data::LayerGroup*> (m_layerGroup->childAtIndex(0));
	//curLayer = curLayerGroup->childAtIndex(0);
	curLayer = m_layerGroup->childAtIndex(0);
	
	//Populate combo box
	for (int i = 0; i < curLayer->GetNumJoint(); i++) {
		cacani::data::Joint* curJoint = curLayer->GetJoint(i);
		m_joints->addItem(QString::fromStdString(curJoint->name));
	}

	//Joint Selection
	QLabel* first = new QLabel("Select Joint to Map Vertex to:");
	QFormLayout* inputLayout = new QFormLayout;
	inputLayout->addRow(first, m_joints);
	QWidget* inputWidget = new QWidget;
	inputWidget->setLayout(inputLayout);

	//Buttons
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(m_Button_Confirm);
	buttonLayout->addWidget(m_Button_Cancel);
	QWidget* buttonWidget = new QWidget;
	buttonWidget->setLayout(buttonLayout);

	//Dialog
	QVBoxLayout* dialogLayout = new QVBoxLayout;
	dialogLayout->addWidget(inputWidget);
	dialogLayout->addWidget(buttonWidget);
	setLayout(dialogLayout);
	setWindowTitle("Map Vertex to Joint");
	resize(400, 200);
	
}

SkeletonMapDialog::~SkeletonMapDialog() {}

void SkeletonMapDialog::confirmClicked() {
	int jointIndex = m_joints->currentIndex();

	QMessageBox msgBox(QMessageBox::Question, tr("Map to Joint"), "Map Joint " + m_joints->currentText() +
		" to Vertex clicked?", QMessageBox::Ok | QMessageBox::Cancel, this);
	QPalette mainPalette = this->palette();
	mainPalette.setColor(QPalette::WindowText, QColor(0, 0, 0));
	msgBox.setPalette(mainPalette);
	int ret = msgBox.exec();

	if (ret == QMessageBox::Ok)	{
		//Pair keeps track of which image and vertex is mapped to the skeleton joint
		m_pair = std::make_pair(selectedImage, selected);
		//The unordered map uses the joint as the key
		(*m_map).insert({ jointIndex, m_pair });
		close();
		this->setResult(QDialog::Accepted);
	}
}

void SkeletonMapDialog::cancelClicked() {
	close();
	this->setResult(QDialog::Rejected);
}