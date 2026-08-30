
#include "HySpexSWIR_384_View.hpp"
#include "HySpexSWIR_384_Model.hpp"

#include <string>


cHySpexSWIR_384_View::cHySpexSWIR_384_View(cHySpexSWIR_384_Model* pModel, QWidget* parent)
	:
    cHyperspectralView(parent), mpModel(pModel)
{
    assert(mpModel);

    setWindowTitle("HySpex SWIR-384");
}

cHySpexSWIR_384_View::~cHySpexSWIR_384_View()
{
}


