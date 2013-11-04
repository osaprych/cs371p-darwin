// --------
// includes
// --------

#include <cassert>   // assert
#include <cstdlib>   // rand, srand
#include <iostream>  // cout, endl
#include <stdexcept> // invalid_argument, out_of_range

#include "Darwin.h"

// ----
// main
// ----

int main (){

    // ----
    // food
    // ----
    
    Species food("food");
    food.add_instruction({left});
    food.add_instruction({go, 0});
    food.complete();

    // ------
    // hopper
    // ------

    Species hopper("hopper");
    hopper.add_instruction({hop});
    hopper.add_instruction({go, 0});
    hopper.complete();

    // -----
    // rover
    // -----

    Species rover("rover");
    rover.add_instruction({if_enemy, 9});  // 0: if_enemy 9
    rover.add_instruction({if_empty, 7});  // 1: if_empty 7
    rover.add_instruction({if_random, 5}); // 2: if_random 5
    rover.add_instruction({left});         // 3: left
    rover.add_instruction({go, 0});        // 4: go 0
    rover.add_instruction({right});        // 5: right
    rover.add_instruction({go, 0});        // 6: go 0
    rover.add_instruction({hop});          // 7: hop
    rover.add_instruction({go, 0});        // 8: go 0
    rover.add_instruction({infect});       // 9: infect
    rover.add_instruction({go, 0});        //10: go 0
    rover.complete();

    // ----
    // trap
    // ----

    Species trap("trap");
    trap.add_instruction({if_enemy, 3}); // 0: if_enemy 3
    trap.add_instruction({left});        // 1: left
    trap.add_instruction({go, 0});       // 2: go 0
    trap.add_instruction({infect});      // 3: infect
    trap.add_instruction({go, 0});       // 4: go 0
    trap.complete();

    using namespace std;
    //std::left conflicts with creature instruction left, so
    //do this after defining species

    // ----------
    // darwin 8x8
    // ----------

    try {
        cout << "*** Darwin 8x8 ***" << endl;
        
        World w(8, 8);
        w.add_creature(&food,   east,  Location(0, 0));
        w.add_creature(&hopper, north, Location(3, 3));
        w.add_creature(&hopper, east,  Location(3, 4));
        w.add_creature(&hopper, south, Location(4, 4));
        w.add_creature(&hopper, west,  Location(4, 3));
        w.add_creature(&food,   north, Location(7, 7));

        for (int i = 0; i <= 5; i++){
            w.print(cout);
            w.step();
            //cout << endl;
        }
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    // ----------
    // darwin 7x9
    // ----------

    try {
        cout << "*** Darwin 7x9 ***" << endl;
        srand(0);
        
        World w(7, 9);
        w.add_creature(&trap,   south, Location(0, 0));
        w.add_creature(&hopper, east,  Location(3, 2));
        w.add_creature(&rover,  north, Location(5, 4));
        w.add_creature(&trap,   west,  Location(6, 8));
        
        for (int i = 0; i <= 5; i++){
            w.print(cout);
            w.step();
            //cout << endl;
        }
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    // ------------
    // darwin 72x72
    // without best
    // ------------

    try {
        cout << "*** Darwin 72x72 without Best ***" << endl;
        srand(0);
        /*
        Randomly place the following creatures facing randomly.
        Call rand(), mod it with 5184 (72x72), and use that for the position
        in a row-major order grid.
        Call rand() again, mod it with 4 and use that for it's direction with
        the ordering: west, north, east, south.
        Do that for each kind of creature.
        10 Food
        10 Hopper
        10 Rover
        10 Trap
        Simulate 1000 moves.
        Print the first 10 grids          (i.e. 0, 1, 2...9).
        Print every 100th grid after that (i.e. 100, 200, 300...1000).
        */
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    // ------------
    // darwin 72x72
    // with best
    // ------------

    try {
        cout << "*** Darwin 72x72 with Best ***" << endl;
        srand(0);
        /*
        Randomly place the following creatures facing randomly.
        Call rand(), mod it with 5184 (72x72), and use that for the position
        in a row-major order grid.
        Call rand() again, mod it with 4 and use that for it's direction with
        the ordering: 0:west, 1:north, 2:east, 3:south.
        Do that for each kind of creature.
        10 Food
        10 Hopper
        10 Rover
        10 Trap
        10 Best
        Simulate 1000 moves.
        Best MUST outnumber ALL other species for the bonus pts.
        Print the first 10 grids          (i.e. 0, 1, 2...9).
        Print every 100th grid after that (i.e. 100, 200, 300...1000).
        */
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    return 0;
}
