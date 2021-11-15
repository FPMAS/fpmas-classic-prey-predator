#include "grass.h"

int Grass::growing_rate = 8;

void Grass::grow() {
	if(!_grown) {
		_countdown--;
		if(_countdown == 0) {
			_grown = true;
		}
	}
}

void Grass::to_json(nlohmann::json& j, const Grass* grass) {
	j[0] = grass->countdown();
	j[1] = grass->isGrown();
}

Grass* Grass::from_json(const nlohmann::json& j) {
	return new Grass(j[0].get<int>(), j[1].get<bool>());
}

void Grass::to_datapack(fpmas::io::datapack::ObjectPack &o, const Grass *grass) {
	o.allocate(
			fpmas::io::datapack::pack_size<int>() +
			fpmas::io::datapack::pack_size<bool>());
	o.write(grass->countdown());
	o.write(grass->isGrown());
}

Grass* Grass::from_datapack(const fpmas::io::datapack::ObjectPack &o) {
	return new Grass(o.read<int>(), o.read<bool>());
}
