#pragma once

#include <SDL3/SDL_log.h>

#define LOG_CRIT(format, ...) SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, format, ##__VA_ARGS__)