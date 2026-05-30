
#include "OusterView.hpp"
#include "Constants.hpp"
#include "OusterModel.hpp"

#include <ouster_connect/ouster_utils.h>
#include <string>


namespace
{
    const double ENCODER_TICS_TO_RAD = nConstants::TWO_PI / ouster::MAX_ENCODER_COUNT;

    float intensity2norm(uint16_t intensity)
    {
        const static float SCALE = 0.01f;
        const static float OFFSET = 1.0f;
        const static float MAX_VALUE = 1.225;

        if (intensity == 0) return 0.0f;

        float norm = log10f(intensity * SCALE + OFFSET) / MAX_VALUE;

        if (norm < 0.0f) return 0.0f;
        if (norm > 1.0f) return 1.0f;

        return norm;
    }
}

cOusterView::cOusterView(cOusterModel* pModel, QWidget* parent)
	:
    cOusterVizWidget(),
    mpModel(pModel)
{
    assert(pModel);
    setWindowTitle("OUSTER LiDAR");
}

cOusterView::~cOusterView()
{
}


void cOusterView::setFloatingSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cOusterView::setDockedSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cOusterView::dockLocationChanged(Qt::DockWidgetArea area)
{
    if ((area == Qt::LeftDockWidgetArea) || (area == Qt::RightDockWidgetArea))
    {
        setDockedSize();
        return;
    }

    if ((area == Qt::TopDockWidgetArea) || (area == Qt::BottomDockWidgetArea))
    {
        setDockedSize();
        return;
    }
}

void cOusterView::topLevelChanged(bool topLevel)
{
    // topLevel is true when our GPS view is floating. 
    if (topLevel)
    {
        setFloatingSize();
    }
}

void cOusterView::beamIntrinsicsChanged()
{
    ouster::beam_intrinsics_2_t beam_intrinsics = mpModel->getBeamIntrinsics();
    cOusterVizWidget::beamIntrinsicsChanged(beam_intrinsics);
}

void cOusterView::imuIntrinsicsChanged()
{
    ouster::imu_intrinsics_2_t imu_intrinsics = mpModel->getImuIntrinsics();
    cOusterVizWidget::imuIntrinsicsChanged(imu_intrinsics);
}

void cOusterView::lidarIntrinsicsChanged()
{
    ouster::lidar_intrinsics_2_t lidar_intrinsics = mpModel->getLidarIntrinsics();
    cOusterVizWidget::lidarIntrinsicsChanged(lidar_intrinsics);
}

void cOusterView::dataFormatChanged()
{
    ouster::lidar_data_format_2_t lidar_data_format = mpModel->getLidarDataFormat();
    cOusterVizWidget::dataFormatChanged(lidar_data_format);
}

//void cOusterView::azimuthWindowChanged(ouster::azimuth_range_t azimuth_range)
void cOusterView::azimuthWindowChanged()
{
}

/*
void cOusterView::encoderCountChanged(int min, int max)
{
    mEncoderCountMin = min;
    mEncoderCountMax = max;
}
*/

void cOusterView::imuDataChanged()
{
    ouster::imu_data_t data = mpModel->imuData();
    cOusterVizWidget::imuDataUpdated(data);

/*
    mGx = mX_imu[0] * data.acceleration_Xaxis_g + mX_imu[1] * data.acceleration_Yaxis_g + mX_imu[2] * data.acceleration_Zaxis_g + mX_imu[3];
    mGy = mY_imu[0] * data.acceleration_Xaxis_g + mY_imu[1] * data.acceleration_Yaxis_g + mY_imu[2] * data.acceleration_Zaxis_g + mY_imu[3];
    mGz = mZ_imu[0] * data.acceleration_Xaxis_g + mZ_imu[1] * data.acceleration_Yaxis_g + mZ_imu[2] * data.acceleration_Zaxis_g + mZ_imu[3];
*/
}

void cOusterView::displayData()
{
    if (!isVisible()) return;

    auto lidar_data = mpModel->lidarData();
    if (lidar_data.empty()) return;

    cOusterVizWidget::lidarDataUpdated(0, lidar_data);
}


