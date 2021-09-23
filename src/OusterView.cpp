
#include "OusterView.hpp"
#include "Constants.hpp"

#include <string>
#include <vtkGenericOpenGLRenderWindow.h>

using namespace pcl::visualization;

cOusterView::cOusterView(QWidget* parent)
	:
    QVTKOpenGLWidget(parent),
    mpModel(nullptr)
{
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
}

cOusterView::~cOusterView()
{

}

void cOusterView::displayData()
{
    pcl::PointXYZRGBA pt;
    pt.g = 255;
    pt.x = 100.0;

    mData->push_back(pt);

    if (!mpViewer->updatePointCloud(mData))
    {
        mpViewer->addPointCloud(mData);
    }

    renderWindow()->Render();
}
