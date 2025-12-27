#include <config.h>
#include <generator.h>

#include <util.h>
#include <geometry.h>
#include <triangulate.h>



void Generator::generateProblem() {
    polygon.clear();
    indices.clear();
    nodes.clear();
    Generator::generatePolygon();
    Triangulate::triangulatePolygon();
    Generator::generateEndPoints();
    Geometry::calculateBoundingBox();
}

void Generator::loadPolygon(std::string fileName) {
    polygon.clear();

    std::filesystem::path path = std::filesystem::path(examplesPath) / fileName;
    
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open polygon file: " + path.string());
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        float x, y;
        if (!(ss >> x >> y)) {
            throw std::runtime_error(
                "Invalid vertex format in file: " + path.string()
            );
        }

        polygon.emplace_back(x, y);
    }

    if (polygon.size() < 3) {
        throw std::runtime_error(
            "Polygon must have at least 3 vertices: " + path.string()
        );
    }
}

void Generator::generatePolygon() {
    std::string namePrefix = "polygon";
    std::string nameSuffix = ".txt";
    int exampleCount = 0;
    for (const auto& entry : std::filesystem::directory_iterator(examplesPath)) {
        if (entry.is_regular_file()) ++exampleCount;
    }
    std::string randIndex = std::to_string(getRandInt(1, exampleCount));
    //randIndex = "5";
    std::string fileName = namePrefix + randIndex + nameSuffix;
    loadPolygon(fileName);
}

void Generator::generateEndPoints() {
    int triCount = indices.size() / 3;
    if (triCount < 1) return;

    auto getPtInTri = [](glm::vec2 a, glm::vec2 b, glm::vec2 c) {
        float u = getRandFloat(0, 1), v = getRandFloat(0, 1);
        if (u + v > 1) { u = 1 - u; v = 1 - v; }
        return a + u * (b - a) + v * (c - a);
    };

    // pick random triangle for start
    startTriangle = getRandInt(0, triCount - 1);
    startPoint = getPtInTri(
        polygon[indices[startTriangle*3 + 0]],
        polygon[indices[startTriangle*3 + 1]],
        polygon[indices[startTriangle*3 + 2]]
    );

    // pick random triangle for end
    endTriangle = getRandInt(0, triCount - 1);
    endPoint = getPtInTri(
        polygon[indices[endTriangle*3 + 0]],
        polygon[indices[endTriangle*3 + 1]],
        polygon[indices[endTriangle*3 + 2]]
    );
}