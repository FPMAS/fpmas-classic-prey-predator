#include "model.h"
#include "config.h"
#include "fpmas/model/spatial/grid_agent_mapping.h"

void PreyPredatorModelBase::load_static_config(const YAML::Node& config) {
	Prey::init_energy = config["Prey"]["initial_energy"].as<int>();
	Prey::move_cost = config["Prey"]["move_cost"].as<int>();
	Prey::energy_gain = config["Prey"]["energy_gain"].as<int>();
	Prey::reproduction_rate = config["Prey"]["reproduction_rate"].as<float>();
	Prey::mobility_range_size = config["Prey"]["mobility_range_size"].as<int>();

	Predator::init_energy = config["Predator"]["initial_energy"].as<int>();
	Predator::move_cost = config["Predator"]["move_cost"].as<int>();
	Predator::energy_gain = config["Predator"]["energy_gain"].as<int>();
	Predator::reproduction_rate = config["Predator"]["reproduction_rate"].as<float>();
	Predator::perception_range_size = config["Predator"]["perception_range_size"].as<int>();
	std::cout << Predator::perception_range_size << std::endl;
	Predator::mobility_range_size = config["Predator"]["mobility_range_size"].as<int>();
	
	Grass::growing_rate = config["Grass"]["growing_rate"].as<int>();
}

class GrassFactory : public fpmas::api::model::GridCellFactory<Grass> {
	private:
	public:
		float init_grass_rate;
		fpmas::random::UniformRealDistribution<float> init_grass;
		fpmas::random::UniformIntDistribution<> rd_countdown;

		GrassFactory(float init_grass_rate) :
			init_grass_rate(init_grass_rate), init_grass(0, 1),
			rd_countdown(1, Grass::growing_rate) {
		}

		Grass* build(fpmas::model::DiscretePoint location) {
			bool grown = init_grass(rd) < init_grass_rate;
			int countdown = 0;
			if(!grown)
				countdown = rd_countdown(rd);
			return new Grass(countdown, grown, location);
		}
};

void PreyPredatorModelBase::initModel(const YAML::Node& config) {
	load_static_config(config);

	int grid_width = config["Grid"]["width"].as<int>();
	int grid_height = config["Grid"]["height"].as<int>();
	int prey_count = config["Prey"]["count"].as<int>();
	int predator_count = config["Predator"]["count"].as<int>();

	auto& grow_group = this->buildGroup(GROW, grow);
	GrassFactory grass_factory(config["Grass"]["init_rate"].as<float>());
	GridType::Builder grid(
			grass_factory, grid_width, grid_height
			);

	grid.build(*this, {grow_group});

	auto& move_group = this->buildMoveGroup(MOVE, move);
	auto& eat_group = this->buildGroup(EAT, eat);
	auto& die_group = this->buildGroup(DIE, die);
	auto& reproduce_group = this->buildMoveGroup(REPRODUCE, reproduce);

	GridAgentBuilder<Grass> agent_builder;

	fpmas::model::UniformGridAgentMapping prey_mapping(
			grid_width, grid_height, prey_count
			);
	fpmas::model::DefaultSpatialAgentFactory<Prey> prey_factory;

	agent_builder.build(
			*this,
			{move_group, eat_group, die_group, reproduce_group},
			prey_factory, prey_mapping);

	fpmas::model::UniformGridAgentMapping predator_mapping(
			grid_width, grid_height, predator_count
			);
	fpmas::model::DefaultSpatialAgentFactory<Predator> predator_factory;

	agent_builder.build(
			*this,
			{move_group, eat_group, die_group, reproduce_group},
			predator_factory, predator_mapping);


	this->scheduler().schedule(0.0, 1, grow_group.jobs());
	this->scheduler().schedule(0.1, 1, move_group.jobs());
	this->scheduler().schedule(0.15, 1, this->loadBalancingJob());
	this->scheduler().schedule(0.2, 1, eat_group.jobs());
	this->scheduler().schedule(0.3, 1, die_group.jobs());
	this->scheduler().schedule(0.4, 1, reproduce_group.jobs());
}
