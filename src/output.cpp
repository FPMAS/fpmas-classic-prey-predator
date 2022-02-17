#include "output.h"
#include "model.h"
#include "agent.h"

ModelOutput::ModelOutput(
		fpmas::api::model::Model& model,
		fpmas::api::io::OutputStream& output)
	: DistributedCsvOutput(
			model.getMpiCommunicator(), 0, output,
			{"time", [&model] {return model.runtime().currentDate();}},
			{"grass", [&model] {
			std::size_t num_grass = 0;
			for(auto grass : model.getGroup(GROW).localAgents())
				if(((Grass*) grass)->isGrown())
					num_grass++;
			return num_grass;
			}},
			{"prey", [&model] {
			std::size_t num_prey = 0;
			for(auto agent : model.getGroup(MOVE).localAgents())
				if(agent->typeId() == Prey::TYPE_ID)
					num_prey++;
			return num_prey;
			}},
			{"predator", [&model] {
			std::size_t num_prey = 0;
			for(auto agent : model.getGroup(MOVE).localAgents())
				if(agent->typeId() == Predator::TYPE_ID)
					num_prey++;
			return num_prey;
			}})
{
}

LoadBalancingOutput::LoadBalancingOutput(
		fpmas::api::model::Model& model,
		fpmas::api::io::OutputStream& output
		)
	: CsvOutput(
			output,
			// Local time
			{"time", [&model] () {return model.runtime().currentDate();}},
			// Local agent count
			{"agent_workload", [&model] () {
			return model.getGroup(MOVE).localAgents().size();
			}},
			// Local cell count
			{"cell_workload", [&model] () {
			return model.getGroup(GROW).localAgents().size();
			}},
			// DISTANT perception edges on the local process
			{"perception_communication", [&model] () {
			std::size_t cost = 0;
			for(auto agent : model.getGroup(MOVE).localAgents()) {
				for(auto edge : agent->node()->getOutgoingEdges(fpmas::api::model::PERCEPTION))
					// Predator perceptions (Preys have no perception)
					if(edge->state() == fpmas::api::graph::DISTANT)
						cost++;
				}
			return cost;
			}},
			// Cell related communications
			{"cell_communication", [&model] () {
			std::size_t cost = 0;
			for(auto agent : model.getGroup(GROW).localAgents()) {
				for(auto edge : agent->node()->getOutgoingEdges(fpmas::api::model::CELL_SUCCESSOR))
					if(edge->state() == fpmas::api::graph::DISTANT)
						cost++;
				for(auto edge : agent->node()->getIncomingEdges(fpmas::api::model::LOCATION))
					// This includes links between Preys and their location in
					// the context of Grass eating behavior
					if(edge->state() == fpmas::api::graph::DISTANT)
						cost++;
				for(auto edge : agent->node()->getIncomingEdges(fpmas::api::model::PERCEIVE))
					if(edge->state() == fpmas::api::graph::DISTANT)
						cost++;
				for(auto edge : agent->node()->getIncomingEdges(fpmas::api::model::MOVE))
					if(edge->state() == fpmas::api::graph::DISTANT)
						cost++;
			}
			return cost;
		   }}
) {
	}

TotalLoadBalancingOutput::TotalLoadBalancingOutput(
		fpmas::api::model::Model& model,
		fpmas::api::io::OutputStream& output)
	: DistributedCsvOutput(
			model.getMpiCommunicator(), 0, output,
			{"time", [&model] {return model.runtime().currentDate();}},
			{"total_agents", [&model] {
			return model.getGroup(MOVE).localAgents().size();
			}},
			{"total_cells", [&model] {
			return model.getGroup(GROW).localAgents().size();
			}},
			{"total_perceptions", [&model] {
			std::size_t local_count = 0;
			for(auto agent : model.getGroup(EAT).localAgents())
				local_count += agent->node()->getOutgoingEdges(fpmas::api::model::PERCEPTION).size();
			return local_count;
			}},
			{"total_cell_communication", [&model] {
			std::size_t local_count = 0;
			for(auto cell : model.getGroup(GROW).localAgents())
				local_count +=
					cell->node()->getOutgoingEdges(fpmas::api::model::CELL_SUCCESSOR).size()
					+ cell->node()->getIncomingEdges(fpmas::api::model::LOCATION).size()
					+ cell->node()->getIncomingEdges(fpmas::api::model::PERCEIVE).size()
					+ cell->node()->getIncomingEdges(fpmas::api::model::MOVE).size();
			return local_count;
			}}
) {
}



