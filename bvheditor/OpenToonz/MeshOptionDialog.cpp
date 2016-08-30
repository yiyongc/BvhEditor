#include "MeshOptionDialog.h"

MeshOptionDialog::MeshOptionDialog(MeshBuilderOptions* options) :
	m_options(options),
	m_buttonCancel(new QPushButton("Cancel")),
	m_buttonConfirm(new QPushButton("Confirm")),
	m_buttonDefault(new QPushButton("Default Options")),
	m_sliderMargin(new QSlider(Qt::Horizontal)),
	m_sliderEdgeLength(new QSlider(Qt::Horizontal)),
	m_marginSpinBox(new QSpinBox),
	m_edgeLengthSpinBox(new QDoubleSpinBox)
{
	//Set up buttons with slots
	connect(m_buttonConfirm, SIGNAL(clicked(bool)), this, SLOT(confirmClicked()));
	connect(m_buttonCancel, SIGNAL(clicked(bool)), this, SLOT(cancelClicked()));
	connect(m_buttonDefault, SIGNAL(clicked(bool)), this, SLOT(defaultClicked()));

	//Arrangement of buttons
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(m_buttonDefault);
	buttonLayout->addWidget(m_buttonCancel);
	buttonLayout->addWidget(m_buttonConfirm);
	QWidget* buttonsWidget = new QWidget;
	buttonsWidget->setLayout(buttonLayout);


	//Set up margin option
	QLabel* marginLabel = new QLabel("Select Margin for Mesh (Pixels): ");
	m_marginSpinBox->setValue(5);
	m_marginSpinBox->setRange(0, 20);
	m_sliderMargin->setValue(5);
	m_sliderMargin->setRange(0, 20);
	
	QHBoxLayout* marginLayout1 = new QHBoxLayout;
	marginLayout1->addWidget(marginLabel);
	marginLayout1->addWidget(m_marginSpinBox);
	QWidget* marginWidget = new QWidget;
	marginWidget->setLayout(marginLayout1);

	QVBoxLayout* marginLayoutMain = new QVBoxLayout;
	marginLayoutMain->addWidget(marginWidget);
	marginLayoutMain->addWidget(m_sliderMargin);
	QWidget* marginWidgetMain = new QWidget;
	marginWidgetMain->setLayout(marginLayoutMain);

	//Connection between slider and spinbox
	connect(m_sliderMargin, SIGNAL(valueChanged(int)), this, SLOT(updateMarginSpinBox(int)));
	connect(m_marginSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateMarginSlider(int)));

	//Set up edge length option
	QLabel* edgeLengthLabel = new QLabel("Select Target Edge Length(mm): ");
	m_edgeLengthSpinBox->setValue(5.08);
	m_edgeLengthSpinBox->setRange(0, 25.4);
	m_edgeLengthSpinBox->setSingleStep(0.01);
	m_sliderEdgeLength->setRange(0, 2540);
	m_sliderEdgeLength->setValue(508);
	

	QHBoxLayout* edgeLayout1 = new QHBoxLayout;
	edgeLayout1->addWidget(edgeLengthLabel);
	edgeLayout1->addWidget(m_edgeLengthSpinBox);
	QWidget* edgeWidget = new QWidget;
	edgeWidget->setLayout(edgeLayout1);

	QVBoxLayout* edgeLayoutMain = new QVBoxLayout;
	edgeLayoutMain->addWidget(edgeWidget);
	edgeLayoutMain->addWidget(m_sliderEdgeLength);
	QWidget* edgeWidgetMain = new QWidget;
	edgeWidgetMain->setLayout(edgeLayoutMain);
	
	//Connection between slider and spinbox
	connect(m_sliderEdgeLength, SIGNAL(valueChanged(int)), this, SLOT(updateEdgeLengthSpinBox()));
	connect(m_edgeLengthSpinBox, SIGNAL(valueChanged(QString)), this, SLOT(updateEdgeLengthSlider()));

	//Set up entire view
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(marginWidgetMain);
	mainLayout->addWidget(edgeWidgetMain);
	mainLayout->addWidget(buttonsWidget);

	setLayout(mainLayout);
	setWindowTitle("Select Mesh Builder Options");
	resize(500, 500);

}


MeshOptionDialog::~MeshOptionDialog() {
}

void MeshOptionDialog::defaultClicked() {
	m_marginSpinBox->setValue(5);
	m_edgeLengthSpinBox->setValue(5.08);
}

void MeshOptionDialog::confirmClicked() {

	m_options->m_margin = m_marginSpinBox->value();
	m_options->m_targetEdgeLength = (m_edgeLengthSpinBox->value() / 25.4 * 300);
	m_options->m_targetMaxVerticesCount = 1000;
	m_options->m_transparentColor = TPixel64::Transparent;

	accept();
}

void MeshOptionDialog::cancelClicked() {
	reject();
}

void MeshOptionDialog::updateMarginSlider(int val) {
	m_sliderMargin->setValue(val);
}

void MeshOptionDialog::updateMarginSpinBox(int val) {
	m_marginSpinBox->setValue(val);
}


void MeshOptionDialog::updateEdgeLengthSlider() {
	m_sliderEdgeLength->setValue(m_edgeLengthSpinBox->value() * 100);
}

void MeshOptionDialog::updateEdgeLengthSpinBox() {
	double valueToSet = m_sliderEdgeLength->value() / 100.0;
	m_edgeLengthSpinBox->setValue(valueToSet);
}