#ifndef PP_CONFIG_H
#define PP_CONFIG_H

#include "fpmas/random/generator.h"

#ifndef PP_LOG
#define PP_LOG false
#endif

extern fpmas::random::DistributedGenerator<> rd;

#endif
