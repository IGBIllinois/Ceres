
#include "HySpexVNIR_3000N_Model_direct.hpp"
#include "Constants.hpp"

#include <HySpexConnect/VNIR3000N.hpp>

#include <QDebug>

#include <optional>

cHySpexVNIR_3000N_Model_direct::cHySpexVNIR_3000N_Model_direct(std::unique_ptr<hyspex::cVNIR3000N> camera, QObject* parent)
:
    cHySpexVNIR_3000N_Model(parent), mCamera(std::move(camera))
{
}

cHySpexVNIR_3000N_Model_direct::~cHySpexVNIR_3000N_Model_direct() {}

bool cHySpexVNIR_3000N_Model_direct::configure(const nlohmann::json& jsonCfg)
{
    try
    {
        cHyperspectralModel::configure(jsonCfg);
    }
    catch (const std::exception& e)
    {
        QString msg = "Error in the \"hyspex\" configuration: ";
        msg.append(e.what());
        emit logMessage(logERROR, q_name(), msg);
        return false;
    }

    return true;
}

bool cHySpexVNIR_3000N_Model_direct::initialize()
{
    emit statusMessage("Retrieving HySpex VNIR-3000N camera configuration...");

    // We need to initialize the connection to the physical camera.
    // This usually requires two calls to init!
    mCamera->init(mNumBuffersRaw, mNumBufferPreProcessing);

    auto status = mCamera->getInitStatus();

    if (status == hyspex::InitStatus::HYSPEX_INIT_NOT_STARTED)
    {
        mCamera->init(mNumBuffersRaw, mNumBufferPreProcessing);
    }

    status = mCamera->getInitStatus();

    switch (status)
    {
    case hyspex::InitStatus::HYSPEX_INIT_PENDING_DETECTION:
            break;
    case hyspex::InitStatus::HYSPEX_INIT_PENDING_ELECTRONICS:
            break;
    case hyspex::InitStatus::HYSPEX_INIT_PENDING_SENSOR:
            break;
    case hyspex::InitStatus::HYSPEX_INIT_FAILED_DETECTION:
            break;
    case hyspex::InitStatus::HYSPEX_INIT_FAILED_ELECTRONICS:
            break;
    case hyspex::InitStatus::HYSPEX_INIT_FAILED_SENSOR:
            break;
    case hyspex::InitStatus::HYSPEX_INIT_FAILED_TRANSPORT:
            break;
    }

    mID = mCamera->getId();
    mSerialNumber = mCamera->getSerialNumber();
    mWavelengthRangeId = mCamera->getWavelengthRangeId();

    mSpectralSize = mCamera->getSpectralSize();
    mSpatialSize = mCamera->getSpatialSize();

    mMaxSpatialSize = mCamera->getMaxSpatialSize();
    mMaxSpectralSize = mCamera->getMaxSpectralSize();

    mMaxPixelValue = mCamera->getMaxPixelValue();

    return cHySpexVNIR_3000N_Model::initialize();
}


bool cHySpexVNIR_3000N_Model_direct::startCommunications()
{
    return true;
}

void cHySpexVNIR_3000N_Model_direct::stopCommunications()
{
}

void cHySpexVNIR_3000N_Model_direct::update()
{
}

void cHySpexVNIR_3000N_Model_direct::writeDataHeader()
{
}




#if 0

camera->init();

auto status = camera->getInitStatus();
std::cout << "init = " << hyspex::to_string(status) << std::endl;
std::cout << std::endl;

auto spectralSize = camera->getSpectralSize();
auto spatialSize = camera->getSpatialSize();

std::cout << "spectralSize = " << spectralSize << ", spatialSize = " << spatialSize << std::endl;
std::cout << std::endl;

camera->initAcquisition();

std::cout << "*** Spectral Calibration Per Band ***" << std::endl;

auto spectralCal = camera->getSpectralCalibrationPerBand();

std::cout << "Spectral Calibration Per Band = " << spectralCal.size << std::endl;

for (size_t x = 0; x < spectralCal.size; ++x)
{
    double value = spectralCal.data[x];
    std::cout << "SpectralCal[" << x << "] = " << value << std::endl;
}

/*
    std::cout << "*** Spectral Delta Per Pixel ***" << std::endl;

    auto deltaPixel = camera->getSpectralDeltaPerPixel();

    std::cout << "Spectral Delta Per Pixel = " << deltaPixel.size << std::endl;

    // do stuff with image here.
    for (size_t y = 0; y < spectralSize; ++y)
    {
        for (size_t x = 0; x < spatialSize; ++x)
        {
            double value = deltaPixel.data[y * spatialSize + x];
            std::cout << "DeltaPixel[" << x << ", " << y << "] = " << value << std::endl;
        }
    }
*/

camera->closeShutter();
camera->startAcquisition();
camera->calculateBackground();

camera->stopAcquisition();
camera->openShutter();

auto bcMatrix = camera->getBackgroundMatrix();

std::cout << "*** Background Information ***" << std::endl;

std::cout << "Background Matrix Size = " << bcMatrix.size << std::endl;

// do stuff with image here.
for (size_t y = 0; y < spectralSize; ++y)
{
    for (size_t x = 0; x < spatialSize; ++x)
    {
        unsigned short value = bcMatrix.data[y * spatialSize + x];
        std::cout << "Background[" << x << ", " << y << "] = " << value << std::endl;
    }
}

#endif
