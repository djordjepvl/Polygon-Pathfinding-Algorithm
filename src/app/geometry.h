#pragma once

#define MAX_X 1000.0f
#define MAX_Y 500.0f
#define MIN_VERTEX_CNT 50
#define MAX_VERTEX_CNT 200

extern std::vector<glm::vec2> polygon;
extern glm::vec2 start;
extern glm::vec2 end;

void generateExample();