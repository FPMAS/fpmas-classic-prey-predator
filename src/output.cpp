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
				if(dynamic_cast<Grass*>(grass)->isGrown())
					num_grass++;
			return num_grass;
			}},
			{"prey", [&model] {
			std::size_t num_prey = 0;
			for(auto agent : model.getGroup(MOVE).localAgents())
				if(dynamic_cast<Prey*>(agent))
					num_prey++;
			return num_prey;
			}},
			{"predator", [&model] {
			std::size_t num_prey = 0;
			for(auto agent : model.getGroup(MOVE).localAgents())
				if(dynamic_cast<Predator*>(agent))
					num_prey++;
			return num_prey;
			}})
{
}
