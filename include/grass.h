#ifndef PP_GRASS_H
#define PP_GRASS_H

#include "fpmas/model/spatial/grid.h"

using namespace fpmas::model;

class Grass : public GridCellBase<Grass> {
	private:
		int _countdown;
		bool _grown;

	public:
		static int growing_rate;

		Grass(int countdown, bool grown) : _countdown(countdown), _grown(grown) {}
		Grass() : _countdown(growing_rate), _grown(true) {}

		Grass(int countdown, bool grown, DiscretePoint location)
			:GridCellBase<Grass>(location), _countdown(countdown), _grown(grown)  {
			}

		bool isGrown() const {
			return _grown;
		}

		int countdown() const {
			return _countdown;
		}

		void reset() {
			_grown=false;
			_countdown = Grass::growing_rate;
		}

		void grow();

		static void to_json(nlohmann::json& j, const Grass* grass);
		static Grass* from_json(const nlohmann::json& j);
};
#endif
