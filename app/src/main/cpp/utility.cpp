//
// Created by Daniel on 06.12.2018.
//
#include <GLES2/gl2.h>

#include "log.h"
#include "utility.hpp"

void Utility::print_gl_version()
{
    LOG_I("Utility::print_gl_version", "OpenGL Version is: %s", glGetString(GL_VERSION));
}
