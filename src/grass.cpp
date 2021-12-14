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

std::size_t Grass::size(const fpmas::io::datapack::ObjectPack& p, const Grass*) {
	return p.size<int>() + p.size<bool>();
}

void Grass::to_datapack(fpmas::io::datapack::ObjectPack &o, const Grass *grass) {
	o.put(grass->countdown());
	o.put(grass->isGrown());
}

Grass* Grass::from_datapack(const fpmas::io::datapack::ObjectPack &o) {
	return new Grass(o.get<int>(), o.get<bool>());
}
