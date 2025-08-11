
#include "RappFieldModel.hpp"

#include "RappGroundModel.hpp"
#include "RappAerialModel.hpp"
#include "RappFieldBoundary.hpp"
#include "GpsFileReader.hpp"
#include "Constants.hpp"

#include <vtkDelaunay2D.h>
#include <vtkDoubleArray.h>
#include <vtkTable.h>
#include <vtkTableToPolyData.h>

#include <filesystem>


namespace
{
    cRappAerialModel gAerialModel;
    cRappGroundModel gGroundModel;

    static int gReferenceHeight_mm = rfm::INVALID_HEIGHT;
    static int gAerialMeasurementHeight_mm = rfm::INVALID_HEIGHT;

    std::vector<cRappTriangle> compute_mesh(const std::vector<rfm::rappPoint_t>& points, int32_t max_separation_mm = 10000)
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

}

bool nRFM::load_ground_data(const std::string& fileName)
{
    if (!std::filesystem::exists(fileName))
        return false;

    cGpsFileReader gps;
    gps.loadFromFile(fileName);

    auto points = gps.GetPoints();

    std::vector<rfm::rappPoint_t> rapp_points;

    for (const auto& point : points)
    {
        std::int32_t x_mm = point.x_m * nConstants::M_TO_MM;
        std::int32_t y_mm = point.y_m * nConstants::M_TO_MM;
        std::int32_t z_mm = point.z_m * nConstants::M_TO_MM;

        rapp_points.emplace_back(x_mm, y_mm, z_mm);
    }

    gGroundModel.clearGroundPoints();
    gGroundModel.addGroundPoints(rapp_points);

    auto data = gGroundModel.getGroundPoints();
    auto mesh = compute_mesh(data);

    gGroundModel.clearGroundMesh();
    gGroundModel.addMeshData(mesh);
    return true;
}

bool nRFM::load_aerial_data(const std::string& fileName)
{
    if (!std::filesystem::exists(fileName))
        return false;

    cGpsFileReader gps;
    gps.loadFromFile(fileName);

    auto points = gps.GetPoints();

    if (gps.GetRefPoint().has_value())
    {
        auto ref_point = gps.GetRefPoint().value();
        gReferenceHeight_mm = ref_point.z_m * nConstants::M_TO_MM;
        gAerialMeasurementHeight_mm = gReferenceHeight_mm;
    }

    std::vector<rfm::rappPoint_t> rapp_points;

    for (const auto& point : points)
    {
        std::int32_t x_mm = point.x_m * nConstants::M_TO_MM;
        std::int32_t y_mm = point.y_m * nConstants::M_TO_MM;
        std::int32_t z_mm = point.z_m * nConstants::M_TO_MM;

        rapp_points.emplace_back(x_mm, y_mm, z_mm);
    }

    gAerialModel.clearAerialPoints();
    gAerialModel.addAerialPoints(rapp_points);

    auto data = gAerialModel.getAerialPoints();
    auto mesh = compute_mesh(data, 15000);

    gAerialModel.clearAerialMesh();
    gAerialModel.addMeshData(mesh);
    return true;
}

int nRFM::reference_height_mm()
{
    return gReferenceHeight_mm;
}

void nRFM::set_reference_height_mm(int reference_height_mm)
{
    if (reference_height_mm > (gAerialMeasurementHeight_mm*1.5))
        return;

    if (reference_height_mm < (gAerialMeasurementHeight_mm*0.5))
        return;

    gReferenceHeight_mm = reference_height_mm;
}

int nRFM::set_reference_height_mm(int geometric_height_mm, int x_mm, int y_mm, int z_mm)
{
    int32_t ground_height_mm = static_cast<int32_t>(gGroundModel.getMeshHeight_mm(x_mm, y_mm)) + 1;
    int32_t aerial_height_mm = static_cast<int32_t>(gAerialModel.getMeshHeight_mm(x_mm, y_mm));

    if (aerial_height_mm == rfm::INVALID_HEIGHT)
    {
        return 0;
    }

    auto diff_height_mm = z_mm - (ground_height_mm + geometric_height_mm);

    int32_t reference_height_mm = aerial_height_mm + diff_height_mm;

    gReferenceHeight_mm = reference_height_mm;

    return reference_height_mm;
}

int nRFM::compute_reference_height_mm(int geometric_height_mm, int x_mm, int y_mm, int z_mm)
{
    int32_t ground_height_mm = static_cast<int32_t>(gGroundModel.getMeshHeight_mm(x_mm, y_mm)) + 1;
    int32_t aerial_height_mm = static_cast<int32_t>(gAerialModel.getMeshHeight_mm(x_mm, y_mm));

    if (aerial_height_mm == rfm::INVALID_HEIGHT)
    {
        return rfm::INVALID_HEIGHT;
    }

    auto diff_height_mm = z_mm - (ground_height_mm + geometric_height_mm);

    int32_t reference_height_mm = aerial_height_mm + diff_height_mm;

    return reference_height_mm;
}

int nRFM::ground_height_mm(int x_mm, int y_mm)
{
    return static_cast<int>(gGroundModel.getMeshHeight_mm(x_mm, y_mm));
}

int nRFM::dolly_offset_mm(int x_mm, int y_mm)
{
    return dolly_offset_mm(x_mm, y_mm, gReferenceHeight_mm);
}

int nRFM::dolly_offset_mm(int x_mm, int y_mm, int ref_height_mm)
{
    return static_cast<int>(gAerialModel.getDollyOffset_mm(x_mm, y_mm, ref_height_mm));
}

int nRFM::compute_dolly_height_mm(int height_mm, int x_mm, int y_mm, int ref_height_mm)
{
    int ground_height_mm = static_cast<int>(gGroundModel.getMeshHeight_mm(x_mm, y_mm));
    int dolly_offset_mm = static_cast<int>(gAerialModel.getDollyOffset_mm(x_mm, y_mm, ref_height_mm));

    int new_z_mm = ground_height_mm + height_mm + dolly_offset_mm;

    return new_z_mm;
}
