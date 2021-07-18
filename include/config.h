#ifndef PP_CONFIG_H
#define PP_CONFIG_H

#include "fpmas/random/generator.h"
#include "fpmas/model/spatial/moore.h"
#include "grass.h"
#include "yaml-cpp/yaml.h"

#ifndef PP_LOG
#define PP_LOG false
#endif

#ifndef GRID_TYPE
#define GRID_TYPE VonNeumannGrid
#endif
#define GRID Grid

typedef fpmas::model::GRID_TYPE<Grass> GridType;

extern fpmas::random::DistributedGenerator<> rd;

enum SyncMode {
	GHOST_MODE, HARD_SYNC_MODE
};

namespace YAML {
	template<>
		struct convert<SyncMode> {
			static Node encode(const SyncMode& sync_mode) {
				Node node;
				switch(sync_mode) {
					case GHOST_MODE:
						node = "GHOST_MODE";
						break;
					case HARD_SYNC_MODE:
						node = "HARD_SYNC_MODE";
						break;
				}
				return node;
			}

			static bool decode(const Node& node, SyncMode& sync_mode) {
				if(node.as<std::string>() == "GHOST_MODE")
					sync_mode = GHOST_MODE;
				else if(node.as<std::string>() == "HARD_SYNC_MODE")
					sync_mode = HARD_SYNC_MODE;
				else
					return false;
				return true;
			}
		};
}

#endif
