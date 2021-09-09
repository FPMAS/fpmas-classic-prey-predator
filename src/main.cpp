#include "fpmas.h"
#include "model.h"
#include "output.h"
#include "agent.h"
#include "fpmas/model/spatial/grid_load_balancing.h"

void run_model(
		fpmas::api::model::SpatialModel<Grass>& model,
		fpmas::runtime::Date stop_at,
		bool enable_lb_output) {

	// Regular model output, with grass / prey / predator counts
	fpmas::io::FileOutput file("model.csv");
	ModelOutput model_output(model, file);
	model.scheduler().schedule(
			fpmas::api::scheduler::sub_step_end(0), 1, model_output.job()
			);
	if(enable_lb_output) {
		// Optional load balancint stats outputs

		// A file by process with local workload and communications
		fpmas::io::FileOutput lb_file(
				"lb.%r.csv", model.getMpiCommunicator().getRank()
				);
		LoadBalancingOutput lb_output(model, lb_file);

		// A single file with total workload and communications
		fpmas::io::FileOutput total_lb_file("total_lb.csv");
		TotalLoadBalancingOutput total_lb_output(model, total_lb_file);

		model.scheduler().schedule(
				fpmas::api::scheduler::sub_step_end(0), 1, lb_output.job()
				);

		model.scheduler().schedule(
				fpmas::api::scheduler::sub_step_end(0), 1, total_lb_output.job()
				);

		// Runs the model
		model.runtime().run(stop_at);
	} else {
		// Just runs the model without creating useless files
		model.runtime().run(stop_at);
	}
}

int main(int argc, char** argv) {
	FPMAS_REGISTER_AGENT_TYPES(Grass::JsonBase, Prey::JsonBase, Predator::JsonBase);

	fpmas::init(argc, argv);
	{
		YAML::Node config = YAML::LoadFile(argv[1]);
		int grid_width = config["Grid"]["width"].as<int>();
		int grid_height = config["Grid"]["height"].as<int>();
		SyncMode sync_mode = config["sync_mode"].as<SyncMode>();
		bool enable_lb_output = config["lb_output"].as<bool>();

		fpmas::scheduler::Scheduler scheduler;
		fpmas::runtime::Runtime runtime(scheduler);

		// Builds all possible LoadBalancing algorithms
		fpmas::model::GridLoadBalancing grid_lb(
				grid_width, grid_height,
				fpmas::communication::WORLD
				);
		fpmas::model::ZoltanLoadBalancing zoltan_lb(fpmas::communication::WORLD);
		fpmas::model::ScheduledLoadBalancing scheduled_lb(zoltan_lb, scheduler, runtime);
		fpmas::model::CellLoadBalancing zoltan_cell_lb(fpmas::communication::WORLD, zoltan_lb);

		// Selected LB
		fpmas::api::model::LoadBalancing* load_balancing;

		// Selects LB algorithm
		switch(config["lb_method"].as<LbMethod>()) {
			case ZOLTAN_LB:
				load_balancing = &zoltan_lb;
				break;
			case SCHEDULED_LB:
				load_balancing = &scheduled_lb;
				break;
			case ZOLTAN_CELL_LB:
				load_balancing = &zoltan_cell_lb;
				break;
			case GRID_LB:
				load_balancing = &grid_lb;
				break;
		}

		fpmas::runtime::Date stop_at = config["num_steps"].as<int>();

		switch(sync_mode) {
			case GHOST_MODE:
				{
					PreyPredatorModel<fpmas::synchro::GhostMode> model(
							config, scheduler, runtime, *load_balancing
							);
					run_model(model, stop_at, enable_lb_output);
				}
				break;
			case HARD_SYNC_MODE:
				{
					PreyPredatorModel<fpmas::synchro::HardSyncMode> model(
							config, scheduler, runtime, *load_balancing
							);
					run_model(model, stop_at, enable_lb_output);
				}
				break;
		}
	}
	fpmas::finalize();
}
