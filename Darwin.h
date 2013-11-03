#ifndef Darwin_h
#define Darwin_h

#include <vector>
#include <ostream>
#include <map>

enum direction {west, north, east, south};

class Instruction;

class Species;

class Creature {
	private:
		Species* behavior;
		int pc;
		direction facing;
		int turns;

	public:
		Creature (Species* s, direction d) : 
			behavior(s), pc(0), facing(d), turns(0) {}

		void go(int);
		void increment_pc();
		void infect(Creature&);
		void turn_left();
		void turn_right();

		Instruction& next_move();

		void print(std::ostream&);

		bool operator ==(const Creature& o) const {
			return o.behavior == behavior;
		}
};

class Location {
	private:
		const int x;
		const int y;

	public:
		Location(int column, int row) : x(column), y(row) {}

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
		int width;
		int height;
		int turn;

		bool free_space(Location);

	public:
		World(int w, int h) : width(w), height(h), turn(0) {}

		void add_creature(Species*, direction, Location);
		void step();
		void print(std::ostream&);

		void move(Location, direction);
		void infect(Location, direction);

		bool if_empty(Location, direction);
		bool if_wall(Location, direction);
		bool if_enemy(Location, direction);
};

class Species {
	private:
		std::vector<Instruction*> instructions;
		const std::string name;
		bool completed;

	public:
		Species(std::string n) : 
			name(n), completed(false) {}

		void add_instruction(Instruction*);
		void complete();
		Instruction& next_move(int pc);

		void print_short_name(std::ostream&);
};

class Instruction {
	protected:
		Instruction() {}
	public:
		virtual void perform(World*, Creature&, Location) = 0;
};

class Command : public Instruction {};

class Action : public Instruction {};

#endif