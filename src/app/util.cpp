#include <config.h>
#include <util.h>

int getRandInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(min, max);
    return distr(gen);
}

float getRandFloat(float min, float max) {
    static std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

float lerpFloat(float a, float b, float amount) {
    return a * (1.0f-amount) + b * amount;
}