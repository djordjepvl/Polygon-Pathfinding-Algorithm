#pragma once

int getRandInt(int min, int max);
float getRandFloat(float min, float max);

template<typename... Args>
int avgInt(Args... args) {
    int sum = (args + ...); 
    return sum / sizeof...(args);
}
template<typename... Args>
float avgFloat(Args... args) {
    float sum = (args + ...); 
    return sum / sizeof...(args);
}