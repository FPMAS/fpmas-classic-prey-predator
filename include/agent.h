#ifndef PP_AGENT_H
#define PP_AGENT_H

#include "grass.h"
#include "fpmas/model/spatial/moore.h"

class PreyPredatorAgentBase : public virtual fpmas::api::model::GridAgent<Grass> {
	private:
		static fpmas::random::UniformRealDistribution<> random_real;

		int move_cost;
		float reproduction_rate;

	protected:
		static const MooreRange<VonNeumannGrid<Grass>> mobility_range;
		static const MooreRange<VonNeumannGrid<Grass>> perception_range;

		int energy;
		bool alive;

		virtual PreyPredatorAgentBase* buildChild(int energy) const = 0;
		virtual Neighbors<Grass> reachableCells() const = 0;

	public:
		PreyPredatorAgentBase(
				int move_cost, float reproduction_rate,
				int energy, bool alive) :
			move_cost(move_cost), reproduction_rate(reproduction_rate),
			energy(energy), alive(alive) {
			}

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
					AgentType::move_cost, AgentType::reproduction_rate, energy, alive
					),
			fpmas::model::GridAgent<AgentType, Grass>(
				PreyPredatorAgentBase::mobility_range,
				PreyPredatorAgentBase::perception_range
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

		using PreyPredatorAgent<Prey>::PreyPredatorAgent;

		void eat() override;
};

class Predator : public PreyPredatorAgent<Predator> {
	public:
		static int init_energy;
		static int energy_gain;
		static int move_cost;
		static float reproduction_rate;

		using PreyPredatorAgent<Predator>::PreyPredatorAgent;

		void eat() override;
};
#endif
