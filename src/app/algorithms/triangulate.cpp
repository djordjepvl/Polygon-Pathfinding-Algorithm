#include <config.h>

#include <triangulate.h>
#include <geometry.h>

bool isPointInTri(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c) {
    float area = 0.5f * (-b.y * c.x + a.y * (-b.x + c.x) + a.x * (b.y - c.y) + b.x * c.y);
    float s = 1.0f / (2.0f * area) * (a.y * c.x - a.x * c.y + (c.y - a.y) * p.x + (a.x - c.x) * p.y);
    float t = 1.0f / (2.0f * area) * (a.x * b.y - a.y * b.x + (a.y - b.y) * p.x + (b.x - a.x) * p.y);
    return s > 0 && t > 0 && (1 - s - t) > 0;
}

void Triangulate::triangulatePolygon() {
    indices.clear();
    std::vector<int> tmpIdx(polygon.size());
    for (int i = 0; i < polygon.size(); i++) tmpIdx[i] = i;
    while (tmpIdx.size() > 2) {
        for (int i = 0; i < tmpIdx.size(); i++) {
            int prev = tmpIdx[(i + tmpIdx.size() - 1) % tmpIdx.size()];
            int curr = tmpIdx[i];
            int next = tmpIdx[(i + 1) % tmpIdx.size()];

            glm::vec2 a = polygon[prev], b = polygon[curr], c = polygon[next];
            
            glm::vec2 edge1 = b - a;
            glm::vec2 edge2 = c - b;
            float crossZ = edge1.x * edge2.y - edge1.y * edge2.x;

            if (crossZ <= 0.0f) continue;


            bool hasPoint = false;
            for (int j = 0; j < tmpIdx.size(); j++) {
                int testIdx = tmpIdx[j];
                if (testIdx == prev || testIdx == curr || testIdx == next) continue;
                if (isPointInTri(polygon[testIdx], a, b, c)) { hasPoint = true; break; }
            }

            if (!hasPoint) {
                indices.push_back(prev); indices.push_back(curr); indices.push_back(next);
                tmpIdx.erase(tmpIdx.begin() + i);
                break;
            }
        }
    }
}