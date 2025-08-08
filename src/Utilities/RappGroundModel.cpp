
#include "RappGroundModel.hpp"
#include "RappFieldBoundary.hpp"

#include <QString>


namespace
{
    cRappGroundModel* pDataModel = nullptr;
}

cRappGroundModel* cRappGroundModel::get()
{
    return pDataModel;
}

cRappGroundModel::cRappGroundModel()
{
    pDataModel = this;
}

cRappGroundModel::~cRappGroundModel()
{
}

//-----------------------------------------------------------------------------
std::size_t cRappGroundModel::numOfTriangles() const
{
    return mGroundMesh.size();
}

std::size_t cRappGroundModel::numOfGroundPoints() const
{
    return mGroundPoints.size();
}

void cRappGroundModel::clearGroundPoints()
{
    mGroundPoints.clear();
}

void cRappGroundModel::clearGroundMesh()
{
    mGroundMesh.clear();
    mQuadTree.reset();
}

void cRappGroundModel::addGroundPoint(const rfm::planePoint_t& gps_point)
{
    if (!rfb::withinBoundary(gps_point))
        return;

    auto point = rfb::toRappCoordinates(gps_point);

    mGroundPoints.push_back(point);
}

void cRappGroundModel::addGroundPoint(const rfm::rappPoint_t& rapp_point)
{
    if (!rfb::withinBoundary(rapp_point))
        return;

    mGroundPoints.push_back(rapp_point);
}

void cRappGroundModel::addGroundPoints(const std::vector<rfm::planePoint_t>& gps_points)
{
    for (const auto& gps_point : gps_points)
    {
        addGroundPoint(rfb::toRappCoordinates(gps_point));
    }
}

void cRappGroundModel::addGroundPoints(const std::vector<rfm::rappPoint_t>& rapp_points)
{
    for (const auto& rapp_point : rapp_points)
    {
        addGroundPoint(rapp_point);
    }
}

void cRappGroundModel::addMeshPoint(const rfm::rappPoint_t& p1, const rfm::rappPoint_t& p2, const rfm::rappPoint_t& p3)
{
    mGroundMesh.emplace_back(p1, p2, p3);
}

void cRappGroundModel::addMeshData(const std::vector<cRappTriangle>& mesh)
{
    if (!mQuadTree)
    {
        mQuadTree = std::make_unique<cQuadTree>(rfb::minX_mm(), rfb::maxX_mm(), rfb::minY_mm(), rfb::maxY_mm());
    }

    for (auto& triangle : mesh)
    {
        mGroundMesh.emplace_back(triangle);
        mQuadTree->addMeshData(triangle);
    }
}

const std::vector<rfm::rappPoint_t>& cRappGroundModel::getGroundPoints() const
{
    return mGroundPoints;
}

double cRappGroundModel::getMeshHeight_mm(std::int32_t x_mm, std::int32_t y_mm) const
{
    if (mQuadTree)
    {
        auto h = mQuadTree->height(x_mm, y_mm);

        if (h != rfm::INVALID_HEIGHT)
            return h;
    }

    for (auto triangle : mGroundMesh)
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

    return rfm::INVALID_HEIGHT;
}



