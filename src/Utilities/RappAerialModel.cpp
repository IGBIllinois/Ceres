
#include "RappAerialModel.hpp"
#include "RappFieldBoundary.hpp"

#include <QString>


namespace
{
    cRappAerialModel* pDataModel = nullptr;
}

cRappAerialModel* cRappAerialModel::get()
{
    return pDataModel;
}

cRappAerialModel::cRappAerialModel()
{
    pDataModel = this;
}

cRappAerialModel::~cRappAerialModel()
{
}

//-----------------------------------------------------------------------------
std::size_t cRappAerialModel::numOfTriangles() const
{
    return mAerialMesh.size();
}

std::size_t cRappAerialModel::numOfAerialPoints() const
{
    return mAerialPoints.size();
}

void cRappAerialModel::clearAerialPoints()
{
    mAerialPoints.clear();
}

void cRappAerialModel::clearAerialMesh()
{
    mAerialMesh.clear();
    mQuadTree.reset();
}

void cRappAerialModel::addAerialPoint(const rfm::planePoint_t& gps_point)
{
    if (!rfb::withinBoundary(gps_point))
        return;

    auto point = rfb::toRappCoordinates(gps_point);

    if (point.x_mm < mMinX_mm) mMinX_mm = point.x_mm;
    if (point.x_mm > mMaxX_mm) mMaxX_mm = point.x_mm;

    if (point.y_mm < mMinY_mm) mMinY_mm = point.y_mm;
    if (point.y_mm > mMaxY_mm) mMaxY_mm = point.y_mm;

    mAerialPoints.push_back(point);
}

void cRappAerialModel::addAerialPoint(const rfm::rappPoint_t& rapp_point)
{
    if (!rfb::withinBoundary(rapp_point))
        return;

    if (rapp_point.x_mm < mMinX_mm) mMinX_mm = rapp_point.x_mm;
    if (rapp_point.x_mm > mMaxX_mm) mMaxX_mm = rapp_point.x_mm;

    if (rapp_point.y_mm < mMinY_mm) mMinY_mm = rapp_point.y_mm;
    if (rapp_point.y_mm > mMaxY_mm) mMaxY_mm = rapp_point.y_mm;

    mAerialPoints.push_back(rapp_point);
}

void cRappAerialModel::addAerialPoints(const std::vector<rfm::planePoint_t>& gps_points)
{
    for (const auto& gps_point : gps_points)
    {
        addAerialPoint(rfb::toRappCoordinates(gps_point));
    }
}

void cRappAerialModel::addAerialPoints(const std::vector<rfm::rappPoint_t>& rapp_points)
{
    for (const auto& rapp_point : rapp_points)
    {
        addAerialPoint(rapp_point);
    }
}

void cRappAerialModel::addMeshPoint(const rfm::rappPoint_t& p1, const rfm::rappPoint_t& p2, const rfm::rappPoint_t& p3)
{
    mAerialMesh.emplace_back(p1, p2, p3);
}

void cRappAerialModel::addMeshData(const std::vector<cRappTriangle>& mesh)
{
    if (!mQuadTree)
    {
        mQuadTree = std::make_unique<cQuadTree>(rfb::minX_mm(), rfb::maxX_mm(), rfb::minY_mm(), rfb::maxY_mm());
    }

    for (auto& triangle : mesh)
    {
        mAerialMesh.emplace_back(triangle);
        mQuadTree->addMeshData(triangle);
    }
}

const std::vector<rfm::rappPoint_t>& cRappAerialModel::getAerialPoints() const
{
    return mAerialPoints;
}

double cRappAerialModel::getMeshHeight_mm(std::int32_t x_mm, std::int32_t y_mm) const
{
    if (mQuadTree)
    {
        auto h = mQuadTree->height(x_mm, y_mm);

        if (h != rfm::INVALID_HEIGHT)
            return h;
    }

    for (auto triangle : mAerialMesh)
    {
        if (triangle.withinTriangle(x_mm, y_mm))
        {
            if (mQuadTree)
            {
                auto* pNode = mQuadTree->getNode(x_mm, y_mm);
                if (pNode)
                    pNode->addMeshData(triangle);
            }

            return triangle.height(x_mm, y_mm);
        }
    }

    if ((mMinX_mm <= x_mm) && (x_mm <= mMaxX_mm))
    {
        auto points = mAerialPoints;

        std::sort(points.begin(), points.end(), [](const auto& point1, const auto& point2) { return point1.x_mm < point2.x_mm; });

        auto i = points.size() / 2;

        if (points[i].x_mm < x_mm)
        {

        }
        else
        {
            double distance = 1'000'000'000;
            auto index = i;

            for (; i < points.size(); ++i)
            {
                double x = (points[i].x_mm - x_mm);
                double y = (points[i].y_mm - y_mm);

                double d = sqrt(x*x + y*y);

                if (d < distance)
                {
                    distance = d;
                    index = i;
                }
            }
        }
     }
    else if ((mMinY_mm <= y_mm) && (y_mm <= mMaxY_mm))
    {
        auto points = mAerialPoints;

        std::sort(points.begin(), points.end(), [](const auto& point1, const auto& point2) { return point1.y_mm < point2.y_mm; });


    }
    else if ((x_mm > mMaxX_mm) && (y_mm < mMinY_mm))
    {

    }
    else if ((x_mm > mMaxX_mm) && (y_mm > mMaxY_mm))
    {

    }
    else if (x_mm < mMinX_mm)
    {

    }
    else if (x_mm > mMaxX_mm)
    {

    }
    else if (y_mm < mMinY_mm)
    {

    }
    else if (y_mm > mMaxY_mm)
    {

    }

    return rfm::INVALID_HEIGHT;
}

double cRappAerialModel::getDollyOffset_mm(std::int32_t x_mm, std::int32_t y_mm, std::int32_t ref_height_mm)
{
    auto height_mm = getMeshHeight_mm(x_mm, y_mm);

    if (height_mm == rfm::INVALID_HEIGHT)
        return rfm::INVALID_HEIGHT;

    return ref_height_mm - height_mm;
}


