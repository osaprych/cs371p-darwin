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

/* Represents an individual creature */
class Creature {
    private:
        Species* behavior;
        int pc;
        direction facing;
        int turns;

        /*
         * Turns the creature counterclockwise in place.
         */

        void turn_left();

        /*
         * Turns the creature clockwise in place.
         */
        void turn_right();

    public:
        Creature (Species* s, direction d) :
            behavior(s), pc(0), facing(d), turns(0) {}

        /*
         * Changes the target creature's species to that of this creature
         * @param Creature target the other creature
         */
        void infect(Creature&);

        /*
         * Uses this creature's Species to decide and perform its next action
         * @param w the World containing this creature
         * @param l the Location of this creature
         */
        void take_turn(World&, Location);

        /*
         * Prints this creature's image to the specified stream
         * @param o the stream to print to
         */
        void print(std::ostream&) const;

        /*
         * Compares the species of two creatures
         * @return true if they are of the same species, false otherwise
         */
        bool operator ==(const Creature& o) const {
            return o.behavior == behavior;
        }

        /*
         * Tests whether the creature has taken a number of turns in its life
         * @param n the number of turns to check for
         * @return whether the creature has taken this many turns
         */
        bool has_taken_turns(int) const;
};

class Location {
    private:
        const int x;
        const int y;

    public:
        Location(int row, int column) : x(column), y(row) {}

        /*
         * Orders locations by their row, then their column
         * @param other the other location to compare to this one
         * @return whether this location is before the other
         */
        bool operator <(const Location& other) const {
            if (y == other.y)
                return x < other.x;
            else
                return y < other.y;
        }

        /*
         * Checks whether this location is within the specified bounds
         * @param w the width of the grid
         * @param h the height of the grid
         * @return true if this location is in [0..w) x [0..h)
         */
        bool within_bounds(int w, int h){
            return x >= 0 && x < w && y >= 0 && y < h;
        }

        /*
         * Computes a new location relative to the current location
         * @param d the direction relative to this location
         * @return a location adjacent to this in the d direction
         */
        Location operator +(direction d) const;
};

class World {
    private:
        std::vector<Creature> zoo;
        std::map<Location, int> grid;
        const int width;
        const int height;
        int turn;

        /*
         * Checks whether a location is both in the world and unoccupied
         * @param i the location to test
         * @return whether i is an empty space on the grid
         */
        bool free_space(Location) const;

    public:
        World(int h, int w) : width(w), height(h), turn(0) {}

        /*
         * Creates a new creature on the grid at the specified location,
         * or does nothing if a creature is already there.
         * @param s a pointer to a species to use (must not be null)
         * @param d the initial direction the creature is facing
         * @param l the location of the creature on the grid
         * @throw out_of_range when location is not in the World
         * @throw invalid_argument when species is invalid
         */
        void add_creature(Species*, direction, Location);

        /*
         * Computes the new state of the world and creatures after each
         * creature has taken its next turn.
         */
        void step();

        /*
         * Prints the current state of the board to the specified ostream,
         * including the turns taken and row/column headings.
         * @param o where the data will be printed
         */
        void print(std::ostream&) const;

        /*
         * Moves a creature at a location in the specified direction,
         * if possible.
         * @param l the current location of the creature
         * @param d the direction to the adjacent square
         */
        void move(Location, direction);

        /*
         * Creature at specified location infects an enemy
         * (specified by relative direction) if enemy is present.
         * @param l the current location of the creature
         * @param d the direction to the adjacent square
         */
        void infect(Location, direction);

        /*
         * Checks whether the location in the given direction is an empty space
         * @param l the location of the current square
         * @param d the direction to the adjacent square
         * @return whether the adjacent location is empty
         */
        bool if_empty(Location, direction) const;

        /*
         * Checks whether there is a wall adjacent to the current location
         * in the specified direction
         * @param l the location of the current square
         * @param d the direction to the adjacent square
         * @return whether there is a wall
         */
        bool if_wall(Location, direction) const;

        /*
         * Checks whether the creatures in the current location and in the adjacent
         * location are of different species (if the adjacent location in the
         * specified directioncontains an enemy)
         * @param l the location of the current square
         * @param d the direction to the adjacent square
         * @return true if an enemy is found, false otherwise
         */
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

        /*
         * Adds the specified instruction to the end of a Species' program.
         * The species must not be in the ready state.
         * @param k the instruction to add
         */
        void add_instruction(Instruction);

        /*
         * Marks the species as ready to be used
         * (requires at least one instruction).
         * May only be called once for an instance.
         */
        void complete();

        /*
         * Checks whether the species is ready to be used
         */
        bool ready() const;

        /*
         * Gets the instruction under the program counter
         * @param pc the position of the instruction
         * @return the next instruction to execute for the given pc value
         */
        Instruction next_move(int) const;

        /*
         * Prints the one-letter name of this species to the ostream
         * @param o the stream to print to
         */
        void print_short_name(std::ostream&);
};

enum creature_behavior {hop, left, right, infect,
                        if_empty, if_wall, if_random, if_enemy, go};

struct Instruction {
    creature_behavior h;
    int n;
};

#endif
