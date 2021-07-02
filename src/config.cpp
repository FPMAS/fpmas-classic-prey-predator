#include "config.h"
#include "agent.h"

FPMAS_JSON_SET_UP(Grass::JsonBase, Prey::JsonBase, Predator::JsonBase);

fpmas::random::DistributedGenerator<> rd;
