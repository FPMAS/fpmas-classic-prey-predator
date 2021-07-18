#include "config.h"
#include "agent.h"

fpmas::random::UniformRealDistribution<> PreyPredatorAgentBase::random_real(0, 1);

std::size_t distant_eat_prey = 0;

void PreyPredatorAgentBase::move() {
	this->energy -= move_cost;
	Grass* new_location = this->reachableCells().random();
	this->moveTo(new_location);

#if PP_LOG
	std::cout
		<< this->model()->runtime().currentDate() << ": "
		<< this->node()->getId() << " moves to " << new_location->location()
		<< " " << new_location->node()->getId()
		<< std::endl;
#endif
}

void PreyPredatorAgentBase::reproduce() {
	if(this->random_real(rd) <= reproduction_rate) {
		//std::cout << this->node()->getId() << " reproduces" << std::endl;
		this->energy /= 2;
		PreyPredatorAgentBase* child = this->buildChild(this->energy);
		for(auto group : this->groups())
			group->add(child);
		child->initLocation(this->locationCell());

#if PP_LOG
		std::cout
			<< this->model()->runtime().currentDate() << ": "
			<< "Agent " << this->node()->getId()
			<< " reproduces at " << this->locationCell()->location() << ". "
			<< "Child: " << child->node()->getId() << std::endl;
#endif
	}
}

void PreyPredatorAgentBase::die() {
	if(this->energy <= 0)
		kill();
	if(!isAlive()) {
#if PP_LOG
		std::cout
			<< this->model()->runtime().currentDate() << ": "
			<< this->node()->getId() << " dies." << std::endl;
#endif
		auto groups = this->groups();
		for(auto group : groups)
			group->remove(this);
	}
}

int Prey::init_energy = 4;
int Prey::energy_gain = 4;
int Prey::move_cost = 1;
float Prey::reproduction_rate = 0.05;
int Prey::mobility_range_size = 1;
const int Prey::perception_range_size = -1;

void Prey::eat() {
	AcquireGuard acq(this->locationCell());
	if(this->locationCell()->isGrown()) {
#if PP_LOG
		std::cout
			<< this->model()->runtime().currentDate() << ": "
			<< this->node()->getId() << " eats " << this->locationCell()->location()
			<< " " << this->locationCell()->node()->getId()
			<< std::endl;
#endif

		this->energy += Prey::energy_gain;
		this->locationCell()->reset();
	}
}

int Predator::init_energy = 20;
int Predator::energy_gain = 20;
int Predator::move_cost = 1;
float Predator::reproduction_rate = 0.04;
int Predator::mobility_range_size = 1;
int Predator::perception_range_size = 0;

void Predator::eat() {
	auto preys = this->perceptions<Prey>();
	if(!preys.empty()) {
		auto prey = preys.random();
		AcquireGuard acq(prey);
		if(prey->isAlive()) {
#if PP_LOG
			std::cout
				<< this->model()->runtime().currentDate() << ": "
				<< this->node()->getId() << " eats " << prey->node()->getId()
				<< std::endl;
#endif
			if(prey->node()->state() == fpmas::api::graph::DISTANT)
				distant_eat_prey++;

			prey->kill();
			this->energy+=Predator::energy_gain;
		}
	}
}
