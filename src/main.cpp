#include "fpmas.h"
#include "model.h"
#include "output.h"
#include "agent.h"
#include "fpmas/model/spatial/grid_load_balancing.h"

void run_model(
		fpmas::api::model::SpatialModel<Grass>& model,
		fpmas::runtime::Date stop_at) {

		fpmas::io::FileOutput file("model.csv");
		ModelOutput output(model, file);

		model.scheduler().schedule(
				fpmas::api::scheduler::sub_step_end(0), 1, output.job()
				);

		model.runtime().run(stop_at);
}

int main(int argc, char** argv) {
	FPMAS_REGISTER_AGENT_TYPES(Grass::JsonBase, Prey::JsonBase, Predator::JsonBase);

	fpmas::init(argc, argv);
	{
		YAML::Node config = YAML::LoadFile(argv[1]);
		int grid_width = config["Grid"]["width"].as<int>();
		int grid_height = config["Grid"]["height"].as<int>();
		SyncMode sync_mode = config["sync_mode"].as<SyncMode>();

		fpmas::model::GridLoadBalancing grid_lb(
				grid_width, grid_height,
				fpmas::communication::WORLD
				);

		fpmas::runtime::Date stop_at = config["num_steps"].as<int>();

		switch(sync_mode) {
			case GHOST_MODE:
				{
					PreyPredatorModel<fpmas::synchro::GhostMode> model(config, grid_lb);
					run_model(model, stop_at);
				}
				break;
			case HARD_SYNC_MODE:
				{
					PreyPredatorModel<fpmas::synchro::HardSyncMode> model(config, grid_lb);
					run_model(model, stop_at);
				}
				break;
		}
	}
	fpmas::finalize();
}
