
#include "HySpexVNIR_3000N_View.hpp"
#include "HySpexVNIR_3000N_Model.hpp"

#include <string>


cHySpexVNIR_3000N_View::cHySpexVNIR_3000N_View(QWidget* parent)
	:
    cHyperspectralView(parent),
    mpModel(nullptr)
{
    setWindowTitle("HySpex VNIR-3000N");
}

cHySpexVNIR_3000N_View::~cHySpexVNIR_3000N_View()
{
}



