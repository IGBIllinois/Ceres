
#include "OusterView.hpp"
#include "../../Utilities/Constants.hpp"
#include "OusterModel.hpp"

#include <string>
#include <vtkGenericOpenGLRenderWindow.h>


#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>


using namespace pcl::visualization;

namespace
{
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
    mpModel(pModel)
{
    setWindowTitle("OUSTER LiDAR");

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

    displayData();

    update();
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

void cOusterView::beamIntrinsicsChanged(ouster::beam_intrinsics_t beam_intrinsics)
{
    mLidarOriginToBeamOrigin_mm = beam_intrinsics.lidar_to_beam_origins_mm;
    for (auto azimuth_deg : beam_intrinsics.azimuth_angles_deg)
    {
        mBeamAzimuthAngles_rad.push_back(-1.0 * azimuth_deg * nConstants::DEG_TO_RAD);
    }
    for (auto altitude_deg : beam_intrinsics.altitude_angles_deg)
    {
        mBeamAltitudeAngles_rad.push_back(altitude_deg * nConstants::DEG_TO_RAD);
    }
}

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

void cOusterView::azimuthWindowChanged(ouster::azimuth_range_t azimuth_range)
{

}

void cOusterView::encoderCountChanged(uint32_t min, uint32_t max)
{
    mEncoderCountMin = min;
    mEncoderCountMax = max;
}

void cOusterView::imuDataChanged(ouster::imu_data_t data)
{
    data.acceleration_Xaxis_g;
    data.acceleration_Yaxis_g;
    data.acceleration_Zaxis_g;
}

void cOusterView::displayData()
{
    if (!isVisible()) return;

    pcl::PointXYZRGB pt;


    auto& phi = mpModel->beamAltitudeAngles_rad();
    if (phi.empty()) return;

    auto& theta_a = mpModel->beamAzimuthAngles_rad();
    if (theta_a.empty()) return;

    auto lidar_returns = mpModel->lidarData();
    if (lidar_returns.empty()) return;

    mData->reserve(lidar_returns.size());
    mData->clear();

    for (std::size_t c = 0; c < mColumnsPerFrame; ++c)
    {
        for (std::size_t p = 0; p < mPixelsPerColumn; ++p)
        {
            auto pixelId = (c * mPixelsPerColumn) + p;
            auto& lidar_return = lidar_returns[pixelId];
            if (lidar_return.encoder_count < mEncoderCountMin) continue;
            if (lidar_return.encoder_count > mEncoderCountMax) continue;
            if (lidar_return.range_mm < MIN_RANGE_MM) continue;

            auto color = mColorGradient.getColorAtValue( intensity2norm(lidar_return.intensity) );

            pt.r = static_cast<uint8_t>(color.r * 255);
            pt.g = static_cast<uint8_t>(color.g * 255);
            pt.b = static_cast<uint8_t>(color.b * 255);

            auto range_mm = (lidar_return.range_mm - mLidarOriginToBeamOrigin_mm);
            auto theta_e = nConstants::TWO_PI * (1.0 - lidar_return.encoder_count / 90112.0);
            auto cos_alt = cos(phi[p]);
            auto cos_enc = cos(theta_e);
            pt.x = range_mm * cos(theta_e + theta_a[p]) * cos_alt + mLidarOriginToBeamOrigin_mm * cos_enc;
            pt.y = range_mm * sin(theta_e + theta_a[p]) * cos_alt + mLidarOriginToBeamOrigin_mm * cos_enc;
            pt.z = range_mm * sin(phi[p]);

            mData->push_back(pt);
        }
    }

    if (!mpViewer->updatePointCloud(mData))
    {
        mpViewer->addPointCloud(mData);
    }

    renderWindow()->Render();

}


