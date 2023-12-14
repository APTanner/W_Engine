#pragma once

#include <cstdlib>
#include <iostream>
#include <W_Engine/Log.h>

#ifdef RELEASE
#define ASSERT(value, message) ((void)0)
#else
#define ASSERT(value, message)                                                           \
		if (!(value)) {                                                                  \
			std::cerr << '[' << __FILENAME__ << "::" << __LINE__ << "] " << message << '\n'; \
			std::abort();                                                                \
		}
#endif

