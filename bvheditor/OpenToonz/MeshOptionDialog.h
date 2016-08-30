#include "qdialog"
#include "qslider.h"
#include <qpushbutton.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "meshbuilder.h"
#include <qlabel.h>
#include <qspinbox.h>

class MeshOptionDialog :
	public QDialog
{
	Q_OBJECT

public:
	MeshOptionDialog(MeshBuilderOptions* opts);
	~MeshOptionDialog();

private:
	MeshBuilderOptions* m_options;
	QPushButton* m_buttonConfirm;
	QPushButton* m_buttonCancel;
	QPushButton* m_buttonDefault;
	QSlider* m_sliderMargin;
	QSlider* m_sliderEdgeLength;
	QSpinBox* m_marginSpinBox;
	QDoubleSpinBox* m_edgeLengthSpinBox;

	private Q_SLOTS :
	void defaultClicked();
	void confirmClicked();
	void cancelClicked();
	void updateMarginSpinBox(int val);
	void updateMarginSlider(int val);
	void updateEdgeLengthSpinBox();
	void updateEdgeLengthSlider();

};