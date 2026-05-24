#pragma once

#include <SDL3/SDL_log.h>

#define LOG_TRACE(format, ...) SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, format, ##__VA_ARGS__)
#define LOG_CRIT(format, ...)  SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, format, ##__VA_ARGS__)
