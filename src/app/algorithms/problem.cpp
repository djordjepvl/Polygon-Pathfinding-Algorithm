#include <config.h>
#include <problem.h>

#include <util.h>
#include <geometry.h>
#include <triangulate.h>

int currPolyIndex = -1;
int exampleCount = -1;

void exampleCountUpdate() {
    exampleCount = 0;
    for (const auto& entry : std::filesystem::directory_iterator(examplesPath)) {
        if (entry.is_regular_file()) ++exampleCount;
    }
}
bool indexValid(int index) {
    exampleCountUpdate();
    return (index >= 0 && index < exampleCount);
}
std::string getFileAtIndex(int index) {
    int count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(examplesPath)) {
        if (entry.is_regular_file()) {
            if (count++ == index) return entry.path().filename().string();
        }
    }
    throw std::runtime_error("Polygon file at index " + std::to_string(index) + " not found");
}

void Problem::create() {
    if (!Geometry::polyIsCCW()) std::reverse(polygon.begin(), polygon.end());
    Triangulate::triangulatePolygon();
    Problem::randomEndPoints();
    Geometry::calculateBoundingBox();
}

void Problem::load(std::string fileName) {
    polygon.clear();
    indices.clear();
    nodes.clear();
    Problem::loadPolygon(fileName);
    create();
}


void Problem::load(int index) {
    std::string namePrefix = "polygon";
    std::string nameSuffix = ".txt";
    if (!indexValid(index)) {
        std::cout << "Index out of range!";
        return;
    }
    std::string fileName = getFileAtIndex(index);
    currPolyIndex = index;
    load(fileName);
}

void Problem::load() {
    std::string namePrefix = "polygon";
    std::string nameSuffix = ".txt";
    exampleCountUpdate();
    int randIndex = getRandInt(0, exampleCount-1);
    std::string fileName = getFileAtIndex(randIndex);
    currPolyIndex = randIndex;
    load(fileName);
}

void Problem::loadNext() {
    exampleCountUpdate();
    load(currPolyIndex + 1 >= exampleCount ? 0 : std::max(currPolyIndex + 1,0));
}
void Problem::loadPrev() {
    exampleCountUpdate();
    load(currPolyIndex - 1 < 0 ? exampleCount - 1 : std::min(currPolyIndex - 1, exampleCount-1));
}

void Problem::loadPolygon(std::string fileName) {
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
    
    std::cout << "Loaded polygon \"" << fileName << "\", file index: " << currPolyIndex << std::endl;
}

void Problem::randomEndPoints() {
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

void Problem::setStart(glm::vec2 pos){
    int triCount = indices.size() / 3;
    for (int i = 0; i < triCount; i++) {
        glm::vec2 a = polygon[indices[i*3 + 0]];
        glm::vec2 b = polygon[indices[i*3 + 1]];
        glm::vec2 c = polygon[indices[i*3 + 2]];
        
        glm::vec2 v0 = c - a, v1 = b - a, v2 = pos - a;
        float dot00 = glm::dot(v0, v0);
        float dot01 = glm::dot(v0, v1);
        float dot02 = glm::dot(v0, v2);
        float dot11 = glm::dot(v1, v1);
        float dot12 = glm::dot(v1, v2);
        
        float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
        
        if ((u >= 0) && (v >= 0) && (u + v <= 1)) {
            startTriangle = i;
            startPoint = pos;
            return;
        }
    }
}
void Problem::setEnd(glm::vec2 pos){
    int triCount = indices.size() / 3;
    for (int i = 0; i < triCount; i++) {
        glm::vec2 a = polygon[indices[i*3 + 0]];
        glm::vec2 b = polygon[indices[i*3 + 1]];
        glm::vec2 c = polygon[indices[i*3 + 2]];
        
        glm::vec2 v0 = c - a, v1 = b - a, v2 = pos - a;
        float dot00 = glm::dot(v0, v0);
        float dot01 = glm::dot(v0, v1);
        float dot02 = glm::dot(v0, v2);
        float dot11 = glm::dot(v1, v1);
        float dot12 = glm::dot(v1, v2);
        
        float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
        
        if ((u >= 0) && (v >= 0) && (u + v <= 1)) {
            endTriangle = i;
            endPoint = pos;
            return;
        }
    }
}