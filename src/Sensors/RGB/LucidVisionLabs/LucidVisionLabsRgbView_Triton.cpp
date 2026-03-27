
#include "LucidVisionLabsRgbView_Triton.hpp"
#include "Constants.hpp"
#include "LucidVisionLabsRgbModel_Triton.hpp"

#include <QToolBar>
#include <QToolButton>
#include <QCamera>
#include <QLayout>

#include <string>


cLucidVisionLabsRgbView_Triton::cLucidVisionLabsRgbView_Triton(cLucidVisionLabsRgbModel_Triton* pModel, QWidget* parent)
    :
    cLucidVisionLabsRgbView(pModel),
    mpModel(pModel)
{}

cLucidVisionLabsRgbView_Triton::~cLucidVisionLabsRgbView_Triton()
{}

void cLucidVisionLabsRgbView_Triton::initialize()
{
    cLucidVisionLabsRgbView::initialize();
}
