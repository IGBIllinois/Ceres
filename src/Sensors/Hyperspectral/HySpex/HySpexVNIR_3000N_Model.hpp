
#pragma once

#include "HySpexCameraModel.hpp"

#include <cbdf/HySpexVNIR_3000N_Serializer.hpp>

#include <QObject>


class cHySpexVNIR_3000N_Model : public cHySpexCameraModel
{
    Q_OBJECT

public:
    cHySpexVNIR_3000N_Model(QObject* parent = nullptr);
    virtual ~cHySpexVNIR_3000N_Model() = default;

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;

    void writeDataHeader() override;

signals:


protected:
	cHyperspectralImageBuffer<float> mBackground;


/*
	auto badPixels = vnir->getBadPixels();
	std::cout << "Num Bad Pixels = " << badPixels.size << std::endl;

	auto badCorrPixels = vnir->getBadPixelsWithCalculatedCorrections();
	std::cout << "Bad Pixels With Calculated Corrections = " << badCorrPixels.size << std::endl;

	auto badPixelsMatrix = vnir->getBadPixelsMatrix();
	std::cout << "Bad Pixels Matrix = " << badPixelsMatrix.size() << std::endl;
*/

	cHyperspectralImageBuffer<float> mResponsivityMatrix;
	cHyperspectralImageBuffer<float> mQuantumEfficiencyMatrix;

	cHyperspectralSpectralBuffer<float> SpectralCalibrationPerBand;



    cHySpexVNIR_3000N_Serializer mSerializer;
};

