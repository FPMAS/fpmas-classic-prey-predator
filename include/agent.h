#ifndef PP_AGENT_H
#define PP_AGENT_H

#include "grass.h"
#include "fpmas/model/spatial/moore.h"
#include "config.h"

class PreyPredatorAgentBase {
	protected:
		static fpmas::random::UniformRealDistribution<> random_real;

		int move_cost;
		float reproduction_rate;

		MooreRange<GridType> mobility_range;
		MooreRange<GridType> perception_range;

		int energy;
		bool alive;

	public:
		PreyPredatorAgentBase(
				int move_cost, float reproduction_rate,
				int mobility_range_size, int perception_range_size,
				int energy, bool alive) :
			move_cost(move_cost), reproduction_rate(reproduction_rate),
			mobility_range(mobility_range_size),
			perception_range(perception_range_size),
			energy(energy), alive(alive) {
			}

		int getEnergy() const {return energy;}

		void kill() {this->alive = false;}
		bool isAlive() const {return alive;}

		void update(int current_rank, int energy);

		virtual void move() = 0;
		virtual void reproduce() = 0;
		virtual void die() = 0;

		virtual void eat() = 0;
};

template<typename AgentType>
class PreyPredatorAgent : public PreyPredatorAgentBase, public GridAgent<AgentType, Grass> {
	public:
		FPMAS_MOBILITY_RANGE(mobility_range);
		FPMAS_PERCEPTION_RANGE(perception_range);

		PreyPredatorAgent() : PreyPredatorAgent(AgentType::init_energy, true) {
		}

		PreyPredatorAgent(int energy, bool alive) :
			PreyPredatorAgentBase(
					AgentType::move_cost, AgentType::reproduction_rate,
					AgentType::mobility_range_size, AgentType::perception_range_size,
					energy, alive
					) {
		}

		Neighbors<Grass> reachableCells() const {
			return this->mobilityField();
		}

		AgentType* buildChild(int energy) const {
			return new AgentType(energy, true);
		}

		void move() override {
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

		void reproduce() override {
			if(this->random_real(rd) <= reproduction_rate) {
				this->energy /= 2;
				AgentType* child = this->buildChild(this->energy);
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

		void die() override {
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

		static void to_json(nlohmann::json& j, const AgentType* agent) {
			j[0] = agent->getEnergy();
			j[1] = agent->isAlive();
		}

		static AgentType* from_json(const nlohmann::json& j) {
			return new AgentType(j[0].get<int>(), j[1].get<bool>());
		}

		static std::size_t size(const fpmas::io::datapack::ObjectPack& p, const AgentType* agent) {
			return p.size<int>() + p.size<bool>();
		}

		static void to_datapack(fpmas::io::datapack::ObjectPack& o, const AgentType* agent) {
			o.put(agent->getEnergy());
			o.put(agent->isAlive());
		}

		static AgentType* from_datapack(const fpmas::io::datapack::ObjectPack& o) {
			return new AgentType(o.get<int>(), o.get<bool>());
		}
};

class Prey : public PreyPredatorAgent<Prey> {
	public:
		static int init_energy;
		static int energy_gain;
		static int move_cost;
		static float reproduction_rate;
		static int mobility_range_size;
		static const int perception_range_size;

		using PreyPredatorAgent<Prey>::PreyPredatorAgent;

		void eat() override;
};

class Predator : public PreyPredatorAgent<Predator> {
	public:
		static int init_energy;
		static int energy_gain;
		static int move_cost;
		static float reproduction_rate;
		static int mobility_range_size;
		static int perception_range_size;

		using PreyPredatorAgent<Predator>::PreyPredatorAgent;

		void eat() override;
};
#endif
