#include "fpmas.h"
#include "model.h"
#include "output.h"
#include "agent.h"
#include "fpmas/model/spatial/grid_load_balancing.h"

int main(int argc, char** argv) {
	FPMAS_REGISTER_AGENT_TYPES(Grass::JsonBase, Prey::JsonBase, Predator::JsonBase);

	fpmas::init(argc, argv);
	{
		YAML::Node config = YAML::LoadFile(argv[1]);
		int grid_width = config["Grid"]["width"].as<int>();
		int grid_height = config["Grid"]["height"].as<int>();

		fpmas::model::GridLoadBalancing grid_lb(
				grid_width, grid_height,
				fpmas::communication::WORLD
				);
		PreyPredatorModel model(config, grid_lb);

		fpmas::runtime::Date stop_at = config["num_steps"].as<int>();

		fpmas::io::FileOutput file("model.csv");
		ModelOutput output(model, file);

		model.scheduler().schedule(
				fpmas::api::scheduler::sub_step_end(0), 1, output.job()
				);

		model.runtime().run(stop_at);
	}
	fpmas::finalize();
}
