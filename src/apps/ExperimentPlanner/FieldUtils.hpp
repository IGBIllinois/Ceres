
#pragma once

#include "RappFieldBoundary.hpp"
#include "RappTriangle.hpp"

#include <QObject>

#include <string>
#include <vector>


std::vector<cRappTriangle> computeGroundMesh(const std::vector<rfm::rappPoint_t>& ground_points);

