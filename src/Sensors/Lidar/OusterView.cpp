
#include "OusterView.hpp"
#include "../../Utilities/Constants.hpp"
#include "OusterModel.hpp"

#include <ouster/ouster_utils.h>
#include <string>
#include <vtkGenericOpenGLRenderWindow.h>


#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>


using namespace pcl::visualization;

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
    QVTKOpenGLStereoWidget(parent, Qt::WindowMinMaxButtonsHint),
    mpModel(pModel),
    mEncoderCountMin(0),
    mEncoderCountMax(0)
{
    setWindowTitle("OUSTER LiDAR");

    mX_sensor[0] = 1.0;
    mX_sensor[1] = 0.0;
    mX_sensor[2] = 0.0;
    mX_sensor[3] = 0.0;

    mY_sensor[0] = 0.0;
    mY_sensor[1] = 1.0;
    mY_sensor[2] = 0.0;
    mY_sensor[3] = 0.0;

    mZ_sensor[0] = 0.0;
    mZ_sensor[1] = 0.0;
    mZ_sensor[2] = 1.0;
    mZ_sensor[3] = 0.0;


    mX_imu[0] = 1.0;
    mX_imu[1] = 0.0;
    mX_imu[2] = 0.0;
    mX_imu[3] = 0.0;

    mY_imu[0] = 0.0;
    mY_imu[1] = 1.0;
    mY_imu[2] = 0.0;
    mY_imu[3] = 0.0;

    mZ_imu[0] = 0.0;
    mZ_imu[1] = 0.0;
    mZ_imu[2] = 1.0;
    mZ_imu[3] = 0.0;

    mData = std::make_shared<pcl::PointCloud<pcl::PointXYZRGB>>();;

    // ------------------------------------
    // -----Create example point cloud-----
    // ------------------------------------
    // We're going to make an ellipse extruded along the z-axis. The colour for
    // the XYZRGB cloud will gradually go from red to green to blue.
    std::uint8_t r(255), g(15), b(15);
    for (float z(-1.0); z <= 1.0; z += 0.05)
    {
        for (float angle(0.0); angle <= 360.0; angle += 5.0)
        {
            pcl::PointXYZRGB point;
            point.x = 0.5 * std::cos(pcl::deg2rad(angle));
            point.y = sinf(pcl::deg2rad(angle));
            point.z = z;
            std::uint32_t rgb = (static_cast<std::uint32_t>(r) << 16 |
                static_cast<std::uint32_t>(g) << 8 | static_cast<std::uint32_t>(b));
            point.rgb = *reinterpret_cast<float*>(&rgb);
            mData->points.push_back(point);
        }
        if (z < 0.0)
        {
            r -= 12;
            g += 12;
        }
        else
        {
            g -= 12;
            b += 12;
        }
    }
    mData->width = mData->size();
    mData->height = 1;
    // ------------------------------------
    // -----Create example point cloud-----
    // ------------------------------------

/*
    pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
    viewer->setBackgroundColor(0, 0, 0);
    pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(cloud);
    viewer->addPointCloud<pcl::PointXYZRGB>(cloud, rgb, "sample cloud");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud");
*/


	// needed to ensure appropriate OpenGL context is created for VTK rendering.
	QSurfaceFormat::setDefaultFormat(QVTKOpenGLStereoWidget::defaultFormat());

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    auto* visualizer = new PCLVisualizer(renderer, renderWindow, "lidar", false);
    mpViewer.reset(visualizer);

    mpViewer->setBackgroundColor(0, 0, 0);
    mpViewer->addCoordinateSystem(1.0);
    mpViewer->initCameraParameters();

    setRenderWindow(mpViewer->getRenderWindow());
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

/*
void cOusterView::beamIntrinsicsChanged(ouster::beam_intrinsics_t beam_intrinsics)
{
    mLidarOriginToBeamOrigin_mm = beam_intrinsics.lidar_to_beam_origins_mm;
    for (auto azimuth_deg : beam_intrinsics.azimuth_angles_deg)
    {
        mTheta_rad.push_back(-1.0 * azimuth_deg * nConstants::DEG_TO_RAD);
    }
    for (auto altitude_deg : beam_intrinsics.altitude_angles_deg)
    {
        mPhi_rad.push_back(altitude_deg * nConstants::DEG_TO_RAD);
    }
}
*/

/*
void cOusterView::imuIntrinsicsChanged(ouster::imu_intrinsics_t imu_intrinsics)
{

}

void cOusterView::lidarIntrinsicsChanged(ouster::lidar_intrinsics_t lidar_intrinsics)
{

}

void cOusterView::dataFormatChanged(ouster::lidar_data_format_t lidar_data_format)
{
    mColumnsPerFrame = lidar_data_format.columns_per_frame;
    mPixelsPerColumn = lidar_data_format.pixels_per_column;
    mColumnWindowMin = lidar_data_format.column_window_min;
    mColumnWindowMax = lidar_data_format.column_window_max;
}
*/

void cOusterView::beamIntrinsicsChanged()
{
    ouster::beam_intrinsics_t beam_intrinsics = mpModel->getBeamIntrinsics();

    mLidarOriginToBeamOrigin_mm = beam_intrinsics.lidar_to_beam_origins_mm;
    for (auto azimuth_deg : beam_intrinsics.azimuth_angles_deg)
    {
        mTheta_rad.push_back(-1.0 * azimuth_deg * nConstants::DEG_TO_RAD);
    }
    for (auto altitude_deg : beam_intrinsics.altitude_angles_deg)
    {
        mPhi_rad.push_back(altitude_deg * nConstants::DEG_TO_RAD);
    }
}

void cOusterView::imuIntrinsicsChanged()
{
    ouster::imu_intrinsics_t imu_intrinsics = mpModel->getImuIntrinsics();

    mX_imu[0] = imu_intrinsics.imu_to_sensor_transform[0];
    mX_imu[1] = imu_intrinsics.imu_to_sensor_transform[1];
    mX_imu[2] = imu_intrinsics.imu_to_sensor_transform[2];
    mX_imu[3] = imu_intrinsics.imu_to_sensor_transform[3];

    mY_imu[0] = imu_intrinsics.imu_to_sensor_transform[4];
    mY_imu[1] = imu_intrinsics.imu_to_sensor_transform[5];
    mY_imu[2] = imu_intrinsics.imu_to_sensor_transform[6];
    mY_imu[3] = imu_intrinsics.imu_to_sensor_transform[7];

    mZ_imu[0] = imu_intrinsics.imu_to_sensor_transform[8];
    mZ_imu[1] = imu_intrinsics.imu_to_sensor_transform[9];
    mZ_imu[2] = imu_intrinsics.imu_to_sensor_transform[10];
    mZ_imu[3] = imu_intrinsics.imu_to_sensor_transform[11];
}

void cOusterView::lidarIntrinsicsChanged()
{
    ouster::lidar_intrinsics_t lidar_intrinsics = mpModel->getLidarIntrinsics();

    mX_sensor[0] = lidar_intrinsics.lidar_to_sensor_transform[0];
    mX_sensor[1] = lidar_intrinsics.lidar_to_sensor_transform[1];
    mX_sensor[2] = lidar_intrinsics.lidar_to_sensor_transform[2];
    mX_sensor[3] = lidar_intrinsics.lidar_to_sensor_transform[3];

    mY_sensor[0] = lidar_intrinsics.lidar_to_sensor_transform[4];
    mY_sensor[1] = lidar_intrinsics.lidar_to_sensor_transform[5];
    mY_sensor[2] = lidar_intrinsics.lidar_to_sensor_transform[6];
    mY_sensor[3] = lidar_intrinsics.lidar_to_sensor_transform[7];

    mZ_sensor[0] = lidar_intrinsics.lidar_to_sensor_transform[8];
    mZ_sensor[1] = lidar_intrinsics.lidar_to_sensor_transform[9];
    mZ_sensor[2] = lidar_intrinsics.lidar_to_sensor_transform[10];
    mZ_sensor[3] = lidar_intrinsics.lidar_to_sensor_transform[11];
}

void cOusterView::dataFormatChanged()
{
    ouster::lidar_data_format_t lidar_data_format = mpModel->getLidarDataFormat();

    mColumnsPerFrame = lidar_data_format.columns_per_frame;
    mPixelsPerColumn = lidar_data_format.pixels_per_column;
    mColumnWindowMin = lidar_data_format.column_window_min;
    mColumnWindowMax = lidar_data_format.column_window_max;
    mPixelShiftByRow = lidar_data_format.pixel_shift_by_row;
}

void cOusterView::azimuthWindowChanged(ouster::azimuth_range_t azimuth_range)
{

}

void cOusterView::encoderCountChanged(int min, int max)
{
    mEncoderCountMin = min;
    mEncoderCountMax = max;
}

//void cOusterView::imuDataChanged(ouster::imu_data_t data)
void cOusterView::imuDataChanged()
{
    ouster::imu_data_t data = mpModel->imuData();

    mGx = mX_imu[0] * data.acceleration_Xaxis_g + mX_imu[1] * data.acceleration_Yaxis_g + mX_imu[2] * data.acceleration_Zaxis_g + mX_imu[3];
    mGy = mY_imu[0] * data.acceleration_Xaxis_g + mY_imu[1] * data.acceleration_Yaxis_g + mY_imu[2] * data.acceleration_Zaxis_g + mY_imu[3];
    mGz = mZ_imu[0] * data.acceleration_Xaxis_g + mZ_imu[1] * data.acceleration_Yaxis_g + mZ_imu[2] * data.acceleration_Zaxis_g + mZ_imu[3];
}

void cOusterView::displayData()
{
    if (!isVisible()) return;

    if (mPhi_rad.empty()) return;
    if (mTheta_rad.empty()) return;

    auto lidar_data = mpModel->lidarData();
    if (lidar_data.empty()) return;

    auto lidar_returns = destagger(lidar_data, mPixelShiftByRow);

    mData->reserve(lidar_returns.num_rows() * lidar_returns.num_columns());
    mData->clear();

    pcl::PointXYZRGB pt;

    for (std::size_t c = 0; c < mColumnsPerFrame; ++c)
    {
        auto encoder_count = lidar_data.encoderPosition(c);
        if (encoder_count < mEncoderCountMin) continue;
        if (encoder_count > mEncoderCountMax) continue;
        auto theta_e = nConstants::TWO_PI - (encoder_count * ENCODER_TICS_TO_RAD);
        auto cos_enc = cos(theta_e);
        auto offset_enc = mLidarOriginToBeamOrigin_mm * cos(theta_e);

        for (std::size_t p = 0; p < mPixelsPerColumn; ++p)
        {
            auto pixelId = (c * mPixelsPerColumn) + p;
            const auto& lidar_return = lidar_returns.get(p, c);
            if (lidar_return.range_mm < MIN_RANGE_MM)
            {
 //               continue;
            }

            auto color = mColorGradient.getColorAtValue( intensity2norm(lidar_return.intensity) );

            pt.r = static_cast<uint8_t>(color.r * 255);
            pt.g = static_cast<uint8_t>(color.g * 255);
            pt.b = static_cast<uint8_t>(color.b * 255);

            auto range_mm = (lidar_return.range_mm - mLidarOriginToBeamOrigin_mm);
            auto cos_alt = cos(mPhi_rad[p]);
            double x = range_mm * cos(theta_e + mTheta_rad[p]) * cos_alt + offset_enc;
            double y = range_mm * sin(theta_e + mTheta_rad[p]) * cos_alt + offset_enc;
            double z = range_mm * sin(mPhi_rad[p]);

            pt.x = mX_sensor[0] * x + mX_sensor[1] * y + mX_sensor[2] * z + mX_sensor[3];
            pt.y = mY_sensor[0] * x + mY_sensor[1] * y + mY_sensor[2] * z + mY_sensor[3];
            pt.z = mZ_sensor[0] * x + mZ_sensor[1] * y + mZ_sensor[2] * z + mZ_sensor[3];

            mData->push_back(pt);
        }
    }

    if (!mpViewer->updatePointCloud(mData))
    {
        mpViewer->addPointCloud(mData);
    }

    renderWindow()->Render();

    update();
}


