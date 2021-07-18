#ifndef PP_AGENT_H
#define PP_AGENT_H

#include "grass.h"
#include "fpmas/model/spatial/moore.h"
#include "config.h"

extern std::size_t distant_eat_prey;

class PreyPredatorAgentBase : public virtual fpmas::api::model::GridAgent<Grass> {
	private:
		static fpmas::random::UniformRealDistribution<> random_real;

		int move_cost;
		float reproduction_rate;

	protected:
		MooreRange<GridType> mobility_range;
		MooreRange<GridType> perception_range;

		int energy;
		bool alive;

		virtual PreyPredatorAgentBase* buildChild(int energy) const = 0;
		virtual Neighbors<Grass> reachableCells() const = 0;

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

		FPMAS_MOBILITY_RANGE(mobility_range);
		FPMAS_PERCEPTION_RANGE(perception_range);

		int getEnergy() const {return energy;}

		void kill() {this->alive = false;}
		bool isAlive() const {return alive;}

		void update(int current_rank, int energy);

		void move();
		void reproduce();
		void die();

		virtual void eat() = 0;
};

template<typename AgentType>
class PreyPredatorAgent : public PreyPredatorAgentBase, public GridAgent<AgentType, Grass> {
	public:
		PreyPredatorAgent() : PreyPredatorAgent(AgentType::init_energy, true) {
		}

		PreyPredatorAgent(int energy, bool alive) :
			PreyPredatorAgentBase(
					AgentType::move_cost, AgentType::reproduction_rate,
					AgentType::mobility_range_size, AgentType::perception_range_size,
					energy, alive
					) {
		}

		Neighbors<Grass> reachableCells() const override {
			return this->mobilityField();
		}

		PreyPredatorAgentBase* buildChild(int energy) const override {
			return new AgentType(energy, true);
		}

		static void to_json(nlohmann::json& j, const AgentType* agent) {
			j[0] = agent->getEnergy();
			j[1] = agent->isAlive();
		}

		static AgentType* from_json(const nlohmann::json& j) {
			return new AgentType(j[0].get<int>(), j[1].get<bool>());
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
