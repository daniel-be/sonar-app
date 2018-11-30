//
// Created by Daniel on 30.11.2018.
//

#ifndef SONAR_APP_ENGINE_HPP
#define SONAR_APP_ENGINE_HPP

constexpr char* LOG_TAG = "engine";

namespace Engine
{
    void draw_frame(int distance, int angle);
    void init();
    void setup_graphics(unsigned int width, unsigned int height);
};

#endif //SONAR_APP_ENGINE_HPP
