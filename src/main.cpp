#include "fpmas.h"
#include "model.h"
#include "output.h"
#include "agent.h"

int main(int argc, char** argv) {
	FPMAS_REGISTER_AGENT_TYPES(Grass::JsonBase, Prey::JsonBase, Predator::JsonBase);

	fpmas::init(argc, argv);
	{
		YAML::Node config = YAML::LoadFile(argv[1]);
		PreyPredatorModel model(config);

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
