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

void add_somewhere(World& w, Species* s, int rows, int columns){
    const int position = rand() % (rows * columns);
    Location l(position / columns, position % columns);
    direction d = static_cast<direction>(rand() % 4);
    w.add_creature(s, d, l);
}

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

    // ----
    // best
    // ----

    Species best("best");
    best.add_instruction({if_empty, 6});    // 0: if_empty 9
    best.add_instruction({if_enemy, 4});    // 1: if_enemy 2
    best.add_instruction({left});           // 2: left
    best.add_instruction({go, 0});          // 3: go 0
    best.add_instruction({infect});         // 4: infect
    best.add_instruction({go, 0});          // 5: go 0
    best.add_instruction({hop});            // 6: hop
    best.add_instruction({go, 0});          // 7: go 0
    best.complete();

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
        World w(72, 72);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &food,   72, 72);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &hopper, 72, 72);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &rover,  72, 72);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &trap,   72, 72);

        for (int s = 0; s <= 9; s++){
            w.print(cout);
            w.step();
        }
        for (int s = 10; s <= 1000; s++){
            if (s % 100 == 0){
                w.print(cout);
            }
            w.step();
        }
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
        World w(72, 72);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &food, 72, 72);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &hopper, 72, 72);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &rover, 72, 72);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &trap, 72, 72);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &best, 72, 72);

        for (int s = 0; s <= 9; s++){
            w.print(cout);
            w.step();
        }
        for (int s = 10; s <= 1000; s++){
            if (s % 100 == 0){
                w.print(cout);
            }
            w.step();
        }
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    // ----------------
    // Additional tests
    // ----------------

    try {
        cout << "*** Darwin 30x50 without Best ***" << endl;
        srand(0);
        World w(30, 50);
        for (int i = 1; i <= 100; i++) add_somewhere(w, &food,  30, 50);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &hopper, 30, 50);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &rover,  30, 50);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &trap,   30, 50);

        int s = 0;
        for (; s <= 20; s++){
            w.print(cout);
            w.step();
        }
        for (; s <= 2000; s++){
            w.step();
        }
        w.print(cout);
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    try {
        cout << "*** Darwin 2x100 without Best ***" << endl;
        srand(0);
        World w(2, 100);
        for (int i = 1; i <= 100; i++) add_somewhere(w, &food,  2, 100);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &hopper, 2, 100);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &rover,  2, 100);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &trap,   2, 100);

        int s = 0;
        for (; s <= 20; s++){
            w.print(cout);
            w.step();
        }
        for (; s < 2000; s++){
            w.step();
        }
        w.print(cout);
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    try {
        cout << "*** Darwin 2x2 without Best ***" << endl;
        srand(0);
        World w(2, 2);
        add_somewhere(w, &rover,  2, 2);
        add_somewhere(w, &hopper, 2, 2);

        int s = 0;
        for (; s <= 7; s++){
            w.print(cout);
            w.step();
        }
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    try {
        cout << "*** Darwin 10x10 without Best ***" << endl;
        srand(0);
        World w(10, 10);
        add_somewhere(w, &rover,  10, 10);

        int s = 0;
        for (; s <= 1000; s++){
            w.print(cout);
            w.step();
        }
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    try {
        cout << "*** Darwin 10x45 without Best ***" << endl;
        srand(0);
        World w(10, 45);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &hopper, 10, 45);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &rover,  10, 45);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &trap,   10, 45);

        int s = 0;
        for (; s <= 20; s++){
            w.print(cout);
            w.step();
        }
        for (; s < 2000; s++){
            w.step();
        }
        w.print(cout);
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    try {
        cout << "*** Darwin 25x25 without Best ***" << endl;
        srand(0);
        World w(25, 25);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &trap,   25, 25);

        int s = 0;
        w.print(cout);
        for (; s <= 100; s++){
            w.step();
        }
        w.print(cout);
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    try {
        cout << "*** Darwin 70x5 without Best ***" << endl;
        srand(0);
        World w(70, 5);
        for (int i = 1; i <= 50; i++) add_somewhere(w, &food, 70, 5);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &hopper, 70, 5);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &rover, 70, 5);

        int s = 0;
        for (; s <= 20; s++){
            w.print(cout);
            w.step();
        }
        for (; s < 2000; s++){
            w.step();
        }
        w.print(cout);
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    try {
        cout << "*** Darwin 7x5 without Best ***" << endl;
        srand(0);
        World w(7, 5);
        for (int i = 1; i <= 5; i++) add_somewhere(w, &food, 7, 5);
        for (int i = 1; i <= 10; i++) add_somewhere(w, &hopper, 7, 5);
        for (int i = 1; i <= 1; i++) add_somewhere(w, &rover, 7, 5);

        int s = 0;
        for (; s <= 20; s++){
            w.print(cout);
            w.step();
        }
        for (; s < 2000; s++){
            w.step();
        }
        w.print(cout);
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    try {
        cout << "*** Darwin 10x10 without Best ***" << endl;
        srand(0);
        World w(10, 10);
        for (int i = 1; i <= 25; i++) add_somewhere(w, &hopper, 10, 10);
        for (int i = 1; i <= 1; i++) add_somewhere(w, &rover, 10, 10);

        int s = 0;
        for (; s <= 20; s++){
            w.print(cout);
            w.step();
        }
        for (; s < 2000; s++){
            w.step();
        }
        w.print(cout);
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    try {
        cout << "*** Darwin 11x11 without Best ***" << endl;
        srand(5);
        World w(11, 11);
        for (int i = 1; i <= 1; i++) add_somewhere(w, &trap, 11, 11);
        for (int i = 1; i <= 25; i++) add_somewhere(w, &rover, 11, 11);

        int s = 0;
        for (; s <= 13; s++){
            w.print(cout);
            w.step();
        }
    } catch (const invalid_argument&) {
        assert(false);
    } catch (const out_of_range&) {
        assert(false);
    }

    return 0;
}
