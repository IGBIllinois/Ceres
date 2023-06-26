
#include "HySpexVNIR_3000N_Model.hpp"
#include "HySpexFactory.hpp"
#include "Constants.hpp"

#include <optional>
#include <iostream>


cHySpexVNIR_3000N_Model::cHySpexVNIR_3000N_Model(QObject* parent)
:
    cHyperspectralModel("VNIR 3000N", parent)
{
    mConnected = false;
    mManufacturer = "HySpex";
    mModel = "VNIR 3000N";

    hyperspectral::spectral_major_data<int> test1;

    test1.resize(10, 5);
    test1.set(0, 0, -1);
    test1.set(1, 0, 1);
    test1.set(2, 0, 2);
    test1.set(3, 0, 3);
    test1.set(4, 0, 4);
    test1.set(5, 0, 5);
    test1.set(6, 0, 6);
    test1.set(7, 0, 7);
    test1.set(8, 0, 8);
    test1.set(9, 0, 9);

    test1.set(1, 1, 1);
    test1.set(1, 2, 1);
    test1.set(1, 3, 1);
    test1.set(1, 4, 1);

    {
        auto t = test1.wavelengths(1);
        assert(5 == t.size());
    }

    {
        auto t = test1.pixels(0);
        assert(10 == t.size());
    }

    hyperspectral::spectral_major_data_fixed<int, 10, 5> test2;
    test2.set(1, 0, 1);
    test2.set(1, 1, 1);
    test2.set(1, 2, 1);
    test2.set(1, 3, 1);
    test2.set(1, 4, 1);

    {
        auto t = test2.pixels(0);
        assert(10 == t.size());
    }

    hyperspectral::spatial_major_data<int> test3;

    test3.resize(10, 5);
    test3.set(0, 0, -1);
    test3.set(1, 0, 1);
    test3.set(2, 0, 2);
    test3.set(3, 0, 3);
    test3.set(4, 0, 4);
    test3.set(5, 0, 5);
    test3.set(6, 0, 6);
    test3.set(7, 0, 7);
    test3.set(8, 0, 8);
    test3.set(9, 0, 9);

    test3.set(1, 1, 1);
    test3.set(1, 2, 1);
    test3.set(1, 3, 1);
    test3.set(1, 4, 1);

    {
        auto t = test3.wavelengths(1);
        assert(5 == t.size());
    }

    {
        auto t = test3.pixels(0);
        assert(10 == t.size());
    }

}

const char* cHySpexVNIR_3000N_Model::descriptor() const
{
    return hyspex_id;
}

uint16_t cHySpexVNIR_3000N_Model::data_class_id() const
{
    return mSerializer.classID();
}

bool cHySpexVNIR_3000N_Model::configure(const nlohmann::json& jsonCfg)
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

bool cHySpexVNIR_3000N_Model::startCommunications()
{
    mConnected = true;

    return true;
}

void cHySpexVNIR_3000N_Model::stopCommunications()
{

    mConnected = false;
}

void cHySpexVNIR_3000N_Model::update()
{
    if (!mConnected) return;
}

void cHySpexVNIR_3000N_Model::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cHySpexVNIR_3000N_Model::disableDataRecording()
{
    cHyperspectralModel::disableDataRecording();
    mSerializer.detach();
}

void cHySpexVNIR_3000N_Model::writeDataHeader()
{
}




