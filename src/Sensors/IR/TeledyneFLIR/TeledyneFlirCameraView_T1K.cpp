
#include "TeledyneFlirCameraView_T1K.hpp"
#include "Constants.hpp"
#include "TeledyneFlirCameraModel_T1K.hpp"

#include <QPaintEvent>
#include <QPainter>
#include <QLineEdit>

#include <string>


cTeledyneFlirCameraView_T1K::cTeledyneFlirCameraView_T1K(cTeledyneFlirCameraModel_T1K* pModel, QWidget* parent)
	:
    cTeledyneFlirCameraView(pModel, parent), mpModel(pModel)
{
}

cTeledyneFlirCameraView_T1K::~cTeledyneFlirCameraView_T1K()
{
}

void cTeledyneFlirCameraView_T1K::initialize()
{
	cTeledyneFlirCameraView::initialize();

	QString image_size = QString::number(mpModel->maxImageWidth());
	image_size += " x ";
	image_size += QString::number(mpModel->maxImageHeight());

	mpImageSize->setText(image_size);
}

void cTeledyneFlirCameraView_T1K::connectToModel()
{
	cTeledyneFlirCameraView::connectToModel();
}
