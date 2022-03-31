
#include "HySpexSWIR_384_View.hpp"
#include "../../Utilities/Constants.hpp"
#include "HySpexSWIR_384_Model.hpp"

#include <string>


cHySpexSWIR_384_View::cHySpexSWIR_384_View(QWidget* parent)
	:
    cHyperspectralView(parent),
    mpModel(nullptr)
{
    setWindowTitle("HySpex SWIR-384");
}

cHySpexSWIR_384_View::~cHySpexSWIR_384_View()
{
}



