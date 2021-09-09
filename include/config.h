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

enum LbMethod {
	ZOLTAN_LB, SCHEDULED_LB, ZOLTAN_CELL_LB, GRID_LB
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

	template<>
		struct convert<LbMethod> {
			static Node encode(const LbMethod& lb_method) {
				Node node;
				switch(lb_method) {
					case ZOLTAN_LB:
						node = "ZOLTAN_LB";
						break;
					case SCHEDULED_LB:
						node = "SCHEDULED_LB";
						break;
					case ZOLTAN_CELL_LB:
						node = "ZOLTAN_CELL_LB";
						break;
					case GRID_LB:
						node = "GRID_LB";
						break;
				}
				return node;
			}

			static bool decode(const Node& node, LbMethod& lb_method) {
				if(node.as<std::string>() == "ZOLTAN_LB")
					lb_method = ZOLTAN_LB;
				else if(node.as<std::string>() == "SCHEDULED_LB")
					lb_method = SCHEDULED_LB;
				else if(node.as<std::string>() == "ZOLTAN_CELL_LB")
					lb_method = ZOLTAN_CELL_LB;
				else if(node.as<std::string>() == "GRID_LB")
					lb_method = GRID_LB;
				else
					return false;
				return true;
			}
		};
}

#endif
