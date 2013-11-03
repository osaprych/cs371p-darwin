#include "Darwin.h"
#include <cassert>

void Species::add_instruction(Instruction* k){
	assert(!completed);
	instructions.push_back(k);
}

Instruction& Species::next_move(const int pc){
	return *instructions.at(pc);
}

void Species::complete(){
	completed = true;
	instructions.shrink_to_fit();
}

void Species::print_short_name(std::ostream& o){
	using namespace std;
	assert(name.length() > 0);
	o << name[0];
}

void Creature::go(int n){
	pc = n;
}

void Creature::increment_pc(){
	++pc;
}

void Creature::infect(Creature& target){
	target.pc = 0;
	target.behavior = behavior;
}

void Creature::turn_left(){
	switch (facing) {
		case west:
			facing = south;
			break;
		case south:
			facing = east;
			break;
		case east:
			facing = north;
			break;
		case north:
			facing = west;
			break;
	}
}

void Creature::turn_right(){
	switch (facing) {
		case west:
			facing = north;
			break;
		case south:
			facing = west;
			break;
		case east:
			facing = south;
			break;
		case north:
			facing = east;
			break;
	}
}

Instruction& Creature::next_move(){
	return behavior->next_move(pc);
}

void Creature::print(std::ostream& o){
	behavior->print_short_name(o);
}

void World::add_creature(Species* s, direction d, Location l){
	using namespace std;
	assert(l.within_bounds(width, height));
	Creature c(s, d);
	grid.at(l) = zoo.size();
	zoo.push_back(c);
}

Location Location::operator +(direction d) const {
	int x = this->x;
	int y = this->y;
	switch (d) {
		case west:
			x--;
			break;
		case south:
			y++;
			break;
		case east:
			x++;
			break;
		case north:
			y--;
			break;
	}
	return Location(x, y);
}

bool World::free_space(Location i){
	return i.within_bounds(width, height) &&
	  grid.find(i) == grid.end();
}

void World::move(const Location l, const direction d){
	using namespace std;
	map<Location, int>::iterator it = grid.find(l);
	assert(it != grid.end());
	const int index = it->second;
	Location intended = l + d;

	if (free_space(intended)){
		grid.erase(it);
		grid.at(intended) = index;
	}
}
