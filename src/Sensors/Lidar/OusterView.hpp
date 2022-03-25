
#pragma once

#include "../../Utilities/ColorGradient.hpp"

#include <ouster/ouster_defs.h>

#include <QVTKOpenGLStereoWidget.h>

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


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class cOusterModel;

class cOusterView : public QVTKOpenGLStereoWidget
{
	Q_OBJECT

public:
	cOusterView(cOusterModel* pModel, QWidget* parent = nullptr);
	virtual ~cOusterView();

public slots:
	void beamIntrinsicsChanged();
	void imuIntrinsicsChanged();
	void lidarIntrinsicsChanged();
	void dataFormatChanged();

//	void beamIntrinsicsChanged(ouster::beam_intrinsics_t beam_intrinsics);
	
/*
	void imuIntrinsicsChanged(ouster::imu_intrinsics_t imu_intrinsics);
	void lidarIntrinsicsChanged(ouster::lidar_intrinsics_t lidar_intrinsics);
*/

	void azimuthWindowChanged(ouster::azimuth_range_t azimuth_range);
	void encoderCountChanged(int min, int max);
//	void imuDataChanged(ouster::imu_data_t data);
	void imuDataChanged();

	void displayData();

public slots:
	void dockLocationChanged(Qt::DockWidgetArea area);
	void topLevelChanged(bool topLevel);

private:
	void setFloatingSize();
	void setDockedSize();

private:

	uint16_t mColumnsPerFrame;
	uint16_t mPixelsPerColumn;
	uint16_t mColumnWindowMin;
	uint16_t mColumnWindowMax;
	uint32_t mEncoderCountMin;
	uint32_t mEncoderCountMax;

	std::vector<int> mPixelShiftByRow;

	std::array<double, 4> mX_sensor;
	std::array<double, 4> mY_sensor;
	std::array<double, 4> mZ_sensor;

	std::array<double, 4> mX_imu;
	std::array<double, 4> mY_imu;
	std::array<double, 4> mZ_imu;

	double mGx;
	double mGy;
	double mGz;

	double mLidarOriginToBeamOrigin_mm;
	std::vector<double> mTheta_rad;
	std::vector<double> mPhi_rad;

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr mData;
	pcl::visualization::PCLVisualizer::Ptr mpViewer;

	cColorGradient mColorGradient;

protected:
	const cOusterModel* mpModel;
};