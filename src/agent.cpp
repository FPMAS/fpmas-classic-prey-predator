#include "config.h"
#include "agent.h"

fpmas::random::UniformRealDistribution<> PreyPredatorAgentBase::random_real(0, 1);


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

			prey->kill();
			this->energy+=Predator::energy_gain;
		}
	}
}
