
#pragma once

#include "RappFieldBoundary.hpp"
#include "RappTriangle.hpp"

#include <QObject>

#include <string>
#include <vector>


std::vector<cRappTriangle> computeGroundMesh(const std::vector<rfm::rappPoint_t>& ground_points);

std::vector<cRappTriangle> computeMesh(const std::vector<rfm::rappPoint_t>& points, int32_t max_separation_mm = 10000);
