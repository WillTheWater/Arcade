#pragma once

#include <cassert>

#ifdef NDEBUG
	#define ASSERT(expr) void(expr)
#else
	#define ASSERT(expr) assert(expr)	
#endif
