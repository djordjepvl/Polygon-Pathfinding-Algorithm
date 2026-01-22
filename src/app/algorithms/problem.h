#pragma once

extern int currPolyIndex;

namespace Problem {
    void load(std::string fileName);
    void load(int index);
    void load();
    void loadNext();
    void loadPrev();
    void loadPolygon(std::string fileName);
    void randomEndPoints();
}
