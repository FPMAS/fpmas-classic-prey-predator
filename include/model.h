#ifndef PP_MODEL_H
#define PP_MODEL_H

#include "fpmas/model/spatial/grid.h"
#include "fpmas/synchro/ghost/ghost_mode.h"
#include "yaml-cpp/yaml.h"
#include "grass.h"
#include "agent.h"

FPMAS_DEFINE_GROUPS(GROW, MOVE, EAT, DIE, REPRODUCE);

class PreyPredatorModelBase : public virtual fpmas::api::model::SpatialModel<Grass> {
	private:
		// Behaviors must be declared at the class level
		Behavior<Grass> grow {&Grass::grow};
		Behavior<PreyPredatorAgentBase> move {&PreyPredatorAgentBase::move};
		Behavior<PreyPredatorAgentBase> eat {&PreyPredatorAgentBase::eat};
		Behavior<PreyPredatorAgentBase> die {&PreyPredatorAgentBase::die};
		Behavior<PreyPredatorAgentBase> reproduce {&PreyPredatorAgentBase::reproduce};

		static void load_static_config(const YAML::Node& config);

	protected:
		void initModel(const YAML::Node& config);

};

template<template<typename> class SyncMode>
class PreyPredatorModel :
	public PreyPredatorModelBase,
	public fpmas::model::GridModel<SyncMode, Grass> {
	public:
		PreyPredatorModel(const YAML::Node& config) {
			this->initModel(config);
		}
		PreyPredatorModel(
				const YAML::Node& config,
				fpmas::api::graph::LoadBalancing<fpmas::api::model::AgentPtr>& lb
				) : fpmas::model::GridModel<SyncMode, Grass>(lb) {
		this->initModel(config);
	}

};
#endif
