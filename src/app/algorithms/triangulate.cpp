#include <config.h>

#include <triangulate.h>
#include <geometry.h>
#include <algorithm>
#include <cmath>
#include <iostream>


bool isPointInTri(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c) {
    float area = 0.5f * (-b.y * c.x + a.y * (-b.x + c.x) + a.x*(b.y - c.y) + b.x*c.y);
    if (std::abs(area) < 1e-8f) return false;

    float s = 1.0f / (2.0f*area) * (a.y*c.x - a.x*c.y + (c.y - a.y)*p.x + (a.x - c.x)*p.y);
    float t = 1.0f / (2.0f*area) * (a.x*b.y - a.y*b.x + (a.y - b.y)*p.x + (b.x - a.x)*p.y);
    float u = 1.0f - s - t;

    float eps = 1e-6f;
    return s >= -eps && t >= -eps && u >= -eps;
}

// helper: check if triangle abc is convex (CCW assumed)
bool isConvex(glm::vec2 a, glm::vec2 b, glm::vec2 c) {
    float cross = (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
    return cross > 1e-8f;
}

void Triangulate::triangulatePolygon() {
    indices.clear();
    if (polygon.size() < 3) return;

    // make a working copy of indices
    std::vector<int> idx(polygon.size());
    for (int i = 0; i < polygon.size(); i++) idx[i] = i;

    int safety = 0;
    const int maxIters = 10000;

    while (idx.size() > 2 && safety++ < maxIters) {
        bool earFound = false;

        for (int i = 0; i < idx.size(); i++) {
            int prev = idx[(i + idx.size() - 1) % idx.size()];
            int curr = idx[i];
            int next = idx[(i + 1) % idx.size()];

            glm::vec2 a = polygon[prev];
            glm::vec2 b = polygon[curr];
            glm::vec2 c = polygon[next];

            if (!isConvex(a, b, c)) continue;

            bool hasInside = false;
            for (int j = 0; j < idx.size(); j++) {
                int test = idx[j];
                if (test == prev || test == curr || test == next) continue;
                if (isPointInTri(polygon[test], a, b, c)) {
                    hasInside = true;
                    break;
                }
            }

            if (!hasInside) {
                // ear found: clip it
                indices.push_back(prev);
                indices.push_back(curr);
                indices.push_back(next);
                idx.erase(idx.begin() + i);
                earFound = true;
                break;
            }
        }

        // fallback if no ear found (prevents infinite loop)
        if (!earFound && idx.size() >= 3) {
            indices.push_back(idx[0]);
            indices.push_back(idx[1]);
            indices.push_back(idx[2]);
            idx.erase(idx.begin() + 1);
        }
    }

    if (safety >= maxIters) {
        std::cerr << "Triangulation reached safety limit!" << std::endl;
    }
}