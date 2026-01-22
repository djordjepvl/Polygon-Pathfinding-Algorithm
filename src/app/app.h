#pragma once

extern double mouseX, mouseY;

namespace App {


    void init();
    void input(int key, int action);
    void update();
    void render();
    void calculateView();
}