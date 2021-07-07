#ifndef PP_CONFIG_H
#define PP_CONFIG_H

#include "fpmas/random/generator.h"
#include "fpmas/model/spatial/moore.h"
#include "grass.h"

#ifndef PP_LOG
#define PP_LOG false
#endif

#ifndef GRID_TYPE
#define GRID_TYPE VonNeumannGrid
#endif
#define GRID Grid

typedef fpmas::model::GRID_TYPE<Grass> GridType;

extern fpmas::random::DistributedGenerator<> rd;

#endif
