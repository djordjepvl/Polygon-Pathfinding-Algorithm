#include <config.h>

#include <triangulate.h>
#include <geometry.h>
#include <algorithm>
#include <cmath>
#include <iostream>

bool isPointInTri(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c) {
    float area = 0.5f * (-b.y * c.x + a.y * (-b.x + c.x) + a.x * (b.y - c.y) + b.x * c.y);
    if (std::abs(area) < 1e-8f) return false; // degenerate triangle
    float s = 1.0f / (2.0f * area) * (a.y * c.x - a.x * c.y + (c.y - a.y) * p.x + (a.x - c.x) * p.y);
    float t = 1.0f / (2.0f * area) * (a.x * b.y - a.y * b.x + (a.y - b.y) * p.x + (b.x - a.x) * p.y);
    float u = 1.0f - s - t;
    float eps = 1e-6f;
    return s >= -eps && t >= -eps && u >= -eps;
}

void Triangulate::triangulatePolygon() {
    indices.clear();

    // Remove consecutive duplicates
    std::vector<glm::vec2> cleanPoly;
    for (auto& p : polygon) {
        if (cleanPoly.empty() || glm::distance(p, cleanPoly.back()) > 1e-6f)
            cleanPoly.push_back(p);
    }
    polygon = cleanPoly;

    if (polygon.size() < 3) return;

    std::vector<int> tmpIdx(polygon.size());
    for (int i = 0; i < polygon.size(); i++) tmpIdx[i] = i;

    int safetyCounter = 0;
    const int maxIterations = 10000; // prevent infinite loops

    while (tmpIdx.size() > 2) {
        bool earFound = false;

        for (int i = 0; i < tmpIdx.size(); i++) {
            int prev = tmpIdx[(i + tmpIdx.size() - 1) % tmpIdx.size()];
            int curr = tmpIdx[i];
            int next = tmpIdx[(i + 1) % tmpIdx.size()];

            glm::vec2 a = polygon[prev];
            glm::vec2 b = polygon[curr];
            glm::vec2 c = polygon[next];

            glm::vec2 edge1 = b - a;
            glm::vec2 edge2 = c - b;
            float crossZ = edge1.x * edge2.y - edge1.y * edge2.x;

            if (crossZ <= 1e-8f) continue; // skip concave or degenerate
            

            bool hasPointInside = false;
            for (int j = 0; j < tmpIdx.size(); j++) {
                int testIdx = tmpIdx[j];
                if (testIdx == prev || testIdx == curr || testIdx == next) continue;
                if (isPointInTri(polygon[testIdx], a, b, c)) {
                    hasPointInside = true;
                    break;
                }
            }

            if (!hasPointInside) {
                // found an ear
                indices.push_back(prev);
                indices.push_back(curr);
                indices.push_back(next);
                tmpIdx.erase(tmpIdx.begin() + i);
                earFound = true;
                break;
            }
        }

        safetyCounter++;
        if (!earFound || safetyCounter > maxIterations) {
            std::cerr << "Triangulation failed or reached safety limit. Polygon might be invalid!" << std::endl;
            break;
        }
    }
}
