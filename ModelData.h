#pragma once
#include <vector>
#include "MaterialData.h"
#include "Vertex.h"

struct ModelData {
	std::vector<VertexData>vertices;
	MaterialData material;
};