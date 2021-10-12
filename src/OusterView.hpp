
#pragma once

#include "ColorGradient.hpp"

#include <QVtkOpenGlWidget.h>

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLight.h>
#include <vtkCamera.h>
#include <vtkActor2D.h>
#include <vtkTextActor.h>
#include <vtkProperty2D.h>
#include <vtkTextProperty.h>
#include <vtkBoxWidget.h>
#include <pcl/visualization/pcl_visualizer.h>

#include <memory>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class cLidarModelOuster;

class cOusterView : public QVTKOpenGLWidget
{
public:
	cOusterView(cLidarModelOuster* pModel, QWidget* parent = nullptr);
	virtual ~cOusterView();

public slots:
	void displayData();

private:
	std::shared_ptr<pcl::PointCloud<pcl::PointXYZRGBA>> mData;
	std::shared_ptr<pcl::visualization::PCLVisualizer> mpViewer;

	cColorGradient mColorGradient;

protected:
	const cLidarModelOuster* mpModel;
};