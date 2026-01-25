#pragma once

#if defined __EMSCRIPTEN__
	#include <GLES3/gl3.h>
#elif defined __ANDROID__
	#include <GLES3/gl32.h>
#elif defined RMLUI_GL3_CUSTOM_LOADER
	 #error "TODO and FIX"
#else
	#include "RmlUiBackend/RmlUi_Include_GL3.h"
#endif