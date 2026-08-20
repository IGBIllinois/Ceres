
#include "HySpexVNIR_3000N_View.hpp"
#include "HySpexVNIR_3000N_Model.hpp"

#include <string>


cHySpexVNIR_3000N_View::cHySpexVNIR_3000N_View(cHySpexVNIR_3000N_Model* pModel, QWidget* parent)
	:
    cHyperspectralView(parent), mpModel(pModel)
{
    assert(mpModel);

    setWindowTitle("HySpex VNIR-3000N");
}

cHySpexVNIR_3000N_View::~cHySpexVNIR_3000N_View()
{
}

void cHySpexVNIR_3000N_View::connectToModel()
{
}


