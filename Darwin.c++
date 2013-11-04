#include "Darwin.h"
#include <cassert>

/* Species */

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

/* end Species */

/* Creature */

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


void Creature::print(std::ostream& o) const {
	behavior->print_short_name(o);
}

/* end Creature */

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

/* World */

bool World::free_space(Location i) const{
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
        assert(if_enemy(l, d));
		grid.erase(it);
		grid.at(intended) = index;
	}
}

void World::infect(const Location l, const direction d){
	using namespace std;
    if (if_enemy(l, d)){
        Creature& caller = zoo[grid.find(l)->second];
        Creature& target = zoo[grid.find(l + d)->second];
        caller.infect(target);
    }
}

void World::add_creature(Species* s, direction d, Location l){
	using namespace std;
	assert(l.within_bounds(width, height));
	Creature c(s, d);
	grid.at(l) = zoo.size();
	zoo.push_back(c);
}

const char empty_space = '.';

void World::print(std::ostream& o) const {
    using namespace std;
    o << "Turn = " << turn << "." << endl;
    // column heading
    o << "  ";
    for (int c = 0; c < width; c++){
        o << c % 10;
    }
    o << endl;
    // rows
    for (int r = 0; r < height; r++){
        o << r << " "; //row heading
        for (int c = 0; c < width; c++){
            Location current(c, r);
            map<Location, int>::const_iterator it = grid.find(current);
            if (it == grid.end()){
                o << empty_space;
            } else {
                zoo[it->second].print(o);
            }
        }
        o << endl;
    }
    o << endl;
}

bool World::if_empty(Location l, direction d) const {
    return free_space(l + d);
}

bool World::if_wall(Location l, direction d) const {
    Location query = l + d;
    return !query.within_bounds(width, height);
}

bool World::if_enemy(Location l, direction d) const {
    using namespace std;
    Location other = l + d;
    const Creature& caller = zoo[grid.at(l)];
    map<Location, int>::const_iterator it = grid.find(other);
    if (it == grid.end())
        return false;
    else
        return caller == zoo[it->second];
}

/* end World */
