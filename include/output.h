#ifndef PP_OUTPUT_H
#define PP_OUTPUT_H

#include "fpmas/io/csv_output.h"
#include "fpmas/api/model/model.h"

using namespace fpmas::io;

class ModelOutput :
	public DistributedCsvOutput<
	Local<fpmas::scheduler::TimeStep>, // Current tick
	Reduce<std::size_t>, // Grass count
	Reduce<std::size_t>, // Prey count
	Reduce<std::size_t>> // Predator count
{
	public:
		ModelOutput(
				fpmas::api::model::Model& comm,
				fpmas::api::io::OutputStream& output
				);



};
#endif
