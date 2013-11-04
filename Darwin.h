#ifndef Darwin_h
#define Darwin_h

#include <vector>
#include <ostream>
#include <map>

enum direction {west, north, east, south};

class Instruction;

class Species;

class World;

class Location;

class Creature {
	private:
		Species* behavior;
		int pc;
		direction facing;
		int turns;

		void turn_left();
		void turn_right();
	public:
		Creature (Species* s, direction d) : 
			behavior(s), pc(0), facing(d), turns(0) {}

		void infect(Creature&);

		void take_turn(World&, Location);

		void print(std::ostream&) const;

		bool operator ==(const Creature& o) const {
			return o.behavior == behavior;
		}

        bool has_taken_turns(int) const;
};

class Location {
	private:
		const int x;
		const int y;

	public:
		Location(int row, int column) : x(column), y(row) {}

		bool operator <(const Location& other) const {
			if (y == other.y)
				return x < other.x;
			else
				return y < other.y;
		}

		bool within_bounds(int w, int h){
			return x >= 0 && x <= w && y >= 0 && y <= h;
		}

		Location operator +(direction d) const;
};

class World {
	private:
		std::vector<Creature> zoo;
		std::map<Location, int> grid;
		const int width;
		const int height;
		int turn;

		bool free_space(Location) const;

	public:
		World(int h, int w) : width(w), height(h), turn(0) {}

		void add_creature(Species*, direction, Location);
		void step();
		void print(std::ostream&) const;

		void move(Location, direction);
		void infect(Location, direction);

		bool if_empty(Location, direction) const;
		bool if_wall(Location, direction) const;
		bool if_enemy(Location, direction) const;
};

class Species {
	private:
		std::vector<Instruction> instructions;
		const std::string name;
		bool completed;

	public:
		Species(std::string n) : 
			name(n), completed(false) {}

		void add_instruction(Instruction);
		void complete();
		Instruction next_move(int pc) const;

		void print_short_name(std::ostream&);
};

enum creature_behavior {hop, left, right, infect,
                        if_empty, if_wall, if_random, if_enemy, go};

struct Instruction {
    creature_behavior h;
    int n;
};

#endif
