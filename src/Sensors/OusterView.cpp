
#include "OusterView.hpp"
#include "../Utilities/Constants.hpp"
#include "OusterModel.hpp"

#include <string>
#include <vtkGenericOpenGLRenderWindow.h>

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
    QVTKOpenGLWidget(parent, Qt::WindowMinMaxButtonsHint),
    mpModel(pModel)
{
    setWindowTitle("OUSTER LiDAR");

    mData = std::make_shared<pcl::PointCloud<pcl::PointXYZRGBA>>();;

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);

	// needed to ensure appropriate OpenGL context is created for VTK rendering.
	QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());


    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    mpViewer.reset(new PCLVisualizer(renderer, renderWindow, "lidar", false));

    setRenderWindow(renderWindow);

    displayData();

    update();

    mpViewer->addCoordinateSystem();
}

cOusterView::~cOusterView()
{

}

void cOusterView::displayData()
{
    if (!isVisible()) return;

    pcl::PointXYZRGBA pt;

    auto minEncoderCount = mpModel->minEncoderCount();
    auto maxEncoderCount = mpModel->maxEncoderCount();
    auto colPerFrame = mpModel->columnsPerFrame();
    auto pixPerColumn = mpModel->pixelsPerColumn();

    auto n = mpModel->lidar_origin_to_beam_origin_mm();

    auto& phi = mpModel->beamAltitudeAngles_rad();
    if (phi.empty()) return;

    auto& theta_a = mpModel->beamAzimuthAngles_rad();
    if (theta_a.empty()) return;

    auto lidar_returns = mpModel->lidarData();
    if (lidar_returns.empty()) return;

    mData->reserve(lidar_returns.size());
    mData->clear();

    for (std::size_t c = 0; c < colPerFrame; ++c)
    {
        for (std::size_t p = 0; p < pixPerColumn; ++p)
        {
            auto pixelId = (c * pixPerColumn) + p;
            auto& lidar_return = lidar_returns[pixelId];
            if (lidar_return.encoder_count < minEncoderCount) continue;
            if (lidar_return.encoder_count > maxEncoderCount) continue;
            if (lidar_return.range_mm < MIN_RANGE_MM) continue;

            auto color = mColorGradient.getColorAtValue( intensity2norm(lidar_return.intensity) );

            pt.r = static_cast<uint8_t>(color.r * 255);
            pt.g = static_cast<uint8_t>(color.g * 255);
            pt.b = static_cast<uint8_t>(color.b * 255);

            auto range_mm = (lidar_return.range_mm - n);
            auto theta_e = nConstants::TWO_PI * (1.0 - lidar_return.encoder_count / 90112.0);
            auto cos_alt = cos(phi[p]);
            auto cos_enc = cos(theta_e);
            pt.x = range_mm * cos(theta_e + theta_a[p]) * cos_alt + n * cos_enc;
            pt.y = range_mm * sin(theta_e + theta_a[p]) * cos_alt + n * cos_enc;
            pt.z = range_mm * sin(phi[p]);

            mData->push_back(pt);
        }
    }

    mpViewer->addCoordinateSystem();

    if (!mpViewer->updatePointCloud(mData))
    {
        mpViewer->addPointCloud(mData);
    }

    renderWindow()->Render();
}
