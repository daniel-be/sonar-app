//
// Created by Daniel on 28.11.2018.
//
#include <thread>
#include <chrono>

#include <android_native_app_glue.h>

#include "log.hpp"
#include "main.hpp"

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
    // loop waiting for stuff to do.

    while (1) {
        LOG_E(LOG_TAG, "Hello %d", 11);
        std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds {1}));
    }
}
//END_INCLUDE(all)