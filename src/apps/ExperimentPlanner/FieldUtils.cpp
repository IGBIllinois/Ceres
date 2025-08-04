
#include "FieldUtils.hpp"


#include <vtkDelaunay2D.h>
#include <vtkDoubleArray.h>
#include <vtkTable.h>
#include <vtkTableToPolyData.h>

#include <array>

#include "Constants.hpp"

namespace
{
    constexpr std::size_t MIN_SET_OF_POINTS = 10;

    template<typename T>
    T threshold(T min, T max, double level_pct)
    {
        if (level_pct > 100.0) return max;
        if (level_pct < 0.0) return min;

        return static_cast<T>((max - min) * (level_pct / 100.0)) + min;
    }
}


//-----------------------------------------------------------------------------
std::vector<cRappTriangle> computeGroundMesh(const std::vector<rfm::rappPoint_t>& ground_points)
{
    vtkDoubleArray* x = vtkDoubleArray::New();
    vtkDoubleArray* y = vtkDoubleArray::New();
    vtkDoubleArray* z = vtkDoubleArray::New();

    for (const auto& ground_point : ground_points)
    {
        x->InsertNextTuple1(ground_point.x_mm);
        y->InsertNextTuple1(ground_point.y_mm);
        z->InsertNextTuple1(ground_point.z_mm);
    }

    vtkNew<vtkTable> table;
    table->AddColumn(x);
    table->AddColumn(y);
    table->AddColumn(z);

    // Convert to a table view of the data
    vtkNew<vtkTableToPolyData> tablePoints;
    tablePoints->SetInputData(table);
    tablePoints->SetXColumnIndex(0);
    tablePoints->SetYColumnIndex(1);
    tablePoints->SetZColumnIndex(2);
    tablePoints->Update();

    // Triangulate the grid points.
    vtkNew<vtkDelaunay2D> delaunay;
    delaunay->SetInputData(tablePoints->GetOutput());
    delaunay->Update();
    vtkSmartPointer<vtkPolyData> mesh = delaunay->GetOutput();

    std::vector<cRappTriangle> data;

    auto m = mesh->GetNumberOfCells();
    vtkIdList* pts = vtkIdList::New();
    for (vtkIdType i = 0; i < m; ++i)
    {
        mesh->GetCellPoints(i, pts);
        auto p1_id = pts->GetId(0);
        auto p2_id = pts->GetId(1);
        auto p3_id = pts->GetId(2);

        double d_mm = (ground_points[p1_id].x_mm - ground_points[p2_id].x_mm) * (ground_points[p1_id].x_mm - ground_points[p2_id].x_mm);
        d_mm += (ground_points[p1_id].y_mm - ground_points[p2_id].y_mm) * (ground_points[p1_id].y_mm - ground_points[p2_id].y_mm);
        d_mm = sqrt(d_mm);

        if (d_mm > 10000)
        {
            continue;
        }

        d_mm = (ground_points[p1_id].x_mm - ground_points[p3_id].x_mm) * (ground_points[p1_id].x_mm - ground_points[p3_id].x_mm);
        d_mm += (ground_points[p1_id].y_mm - ground_points[p3_id].y_mm) * (ground_points[p1_id].y_mm - ground_points[p3_id].y_mm);
        d_mm = sqrt(d_mm);

        if (d_mm > 10000)
        {
            continue;
        }

        d_mm = (ground_points[p3_id].x_mm - ground_points[p2_id].x_mm) * (ground_points[p3_id].x_mm - ground_points[p2_id].x_mm);
        d_mm += (ground_points[p3_id].y_mm - ground_points[p2_id].y_mm) * (ground_points[p3_id].y_mm - ground_points[p2_id].y_mm);
        d_mm = sqrt(d_mm);

        if (d_mm > 10000)
        {
            continue;
        }

        data.emplace_back(ground_points[p1_id], ground_points[p2_id], ground_points[p3_id]);
    }

    pts->Delete();

    return std::move(data);
}


//-----------------------------------------------------------------------------
std::vector<cRappTriangle> computeMesh(const std::vector<rfm::rappPoint_t>& points, int32_t max_separation_mm)
{
    vtkDoubleArray* x = vtkDoubleArray::New();
    vtkDoubleArray* y = vtkDoubleArray::New();
    vtkDoubleArray* z = vtkDoubleArray::New();

    for (const auto& point : points)
    {
        x->InsertNextTuple1(point.x_mm);
        y->InsertNextTuple1(point.y_mm);
        z->InsertNextTuple1(point.z_mm);
    }

    vtkNew<vtkTable> table;
    table->AddColumn(x);
    table->AddColumn(y);
    table->AddColumn(z);

    // Convert to a table view of the data
    vtkNew<vtkTableToPolyData> tablePoints;
    tablePoints->SetInputData(table);
    tablePoints->SetXColumnIndex(0);
    tablePoints->SetYColumnIndex(1);
    tablePoints->SetZColumnIndex(2);
    tablePoints->Update();

    // Triangulate the grid points.
    vtkNew<vtkDelaunay2D> delaunay;
    delaunay->SetInputData(tablePoints->GetOutput());
    delaunay->Update();
    vtkSmartPointer<vtkPolyData> mesh = delaunay->GetOutput();

    std::vector<cRappTriangle> data;

    auto m = mesh->GetNumberOfCells();
    vtkIdList* pts = vtkIdList::New();
    for (vtkIdType i = 0; i < m; ++i)
    {
        mesh->GetCellPoints(i, pts);
        auto p1_id = pts->GetId(0);
        auto p2_id = pts->GetId(1);
        auto p3_id = pts->GetId(2);

        double d_mm = (points[p1_id].x_mm - points[p2_id].x_mm) * (points[p1_id].x_mm - points[p2_id].x_mm);
        d_mm += (points[p1_id].y_mm - points[p2_id].y_mm) * (points[p1_id].y_mm - points[p2_id].y_mm);
        d_mm = sqrt(d_mm);

        if (d_mm > max_separation_mm)
        {
            continue;
        }

        d_mm = (points[p1_id].x_mm - points[p3_id].x_mm) * (points[p1_id].x_mm - points[p3_id].x_mm);
        d_mm += (points[p1_id].y_mm - points[p3_id].y_mm) * (points[p1_id].y_mm - points[p3_id].y_mm);
        d_mm = sqrt(d_mm);

        if (d_mm > max_separation_mm)
        {
            continue;
        }

        d_mm = (points[p3_id].x_mm - points[p2_id].x_mm) * (points[p3_id].x_mm - points[p2_id].x_mm);
        d_mm += (points[p3_id].y_mm - points[p2_id].y_mm) * (points[p3_id].y_mm - points[p2_id].y_mm);
        d_mm = sqrt(d_mm);

        if (d_mm > max_separation_mm)
        {
            continue;
        }

        data.emplace_back(points[p1_id], points[p2_id], points[p3_id]);
    }

    pts->Delete();

    return std::move(data);
}


