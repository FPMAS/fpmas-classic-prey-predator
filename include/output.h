#ifndef PP_OUTPUT_H
#define PP_OUTPUT_H

#include "fpmas/io/csv_output.h"
#include "fpmas/api/model/model.h"

using namespace fpmas::io;

/**
 * Regular model output, that prints total grass, prey and predator counts to a
 * CSV file at each tick.
 */
class ModelOutput :
	public DistributedCsvOutput<
	Local<fpmas::scheduler::TimeStep>, // Current tick
	Reduce<std::size_t>, // Grass count
	Reduce<std::size_t>, // Prey count
	Reduce<std::size_t>> // Predator count
{
	public:
		ModelOutput(
				fpmas::api::model::Model& model,
				fpmas::api::io::OutputStream& output
				);



};

/**
 * Local load balancing statistics.
 */
class LoadBalancingOutput :
	public CsvOutput<
	fpmas::scheduler::TimeStep, // Current tick
	std::size_t, // Local agents (agent workload)
	std::size_t, // Local cells (cell worload)
	std::size_t, // Distant perceptions (communication cost)
	std::size_t // Distant outgoing edges (dist move algo)
	> {
		public:
			LoadBalancingOutput(
					fpmas::api::model::Model& model,
					fpmas::api::io::OutputStream& output
					);
	};

/**
 * Total load balancing statistics.
 */
class TotalLoadBalancingOutput :
	public DistributedCsvOutput<
	Local<fpmas::scheduler::TimeStep>, // Current tick
	Reduce<std::size_t>, // Total agents
	Reduce<std::size_t>, // Total cells
	Reduce<std::size_t>, // Total perception edges
	Reduce<std::size_t> // Total cell edges
	> {
		public:
			TotalLoadBalancingOutput(
					fpmas::api::model::Model& model,
					fpmas::api::io::OutputStream& output
					);
	};
#endif
