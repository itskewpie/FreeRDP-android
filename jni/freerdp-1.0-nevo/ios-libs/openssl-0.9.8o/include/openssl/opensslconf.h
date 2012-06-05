#ifdef __arm__
#include "opensslconfarm.h"
#elif defined __ILP64__
#include "opensslconf64.h"
#else
#include "opensslconf32.h"
#endif
