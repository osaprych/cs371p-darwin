#include "gtest/gtest.h"
#include <sstream>
#include <stdexcept>

#define private public
#include "Darwin.h"
#undef private

// Creature
TEST(Creature_tests, infect_basic){
    Species food("a");
    Species rover("b");
    Creature a(&food, north);
    Creature b(&rover, east);

    a.infect(b);
    ASSERT_EQ(a.behavior, b.behavior);
    ASSERT_EQ(&food, b.behavior);
}

TEST(Creature_tests, infect_pc){
    Species food("a");
    Species rover("b");
    Creature a(&food, north);
    Creature b(&rover, east);

    b.pc = 2;
    a.infect(b);
    ASSERT_EQ(0, b.pc);
}

TEST(Creature_tests, infect_direction){
    Species food("a");
    Species rover("b");
    Creature a(&food, north);
    Creature b(&rover, east);

    a.infect(b);
    ASSERT_EQ(east, b.facing);
}

TEST(Creature_tests, infect_post){
    Species food("a");
    Species rover("b");
    Creature a(&food, north);
    Creature b(&rover, east);

    a.infect(b);
    ASSERT_EQ(a, b);
}

TEST(Creature_tests, turn_move){
    World w(2, 2);
    Species s("s");
    s.add_instruction({hop});
    s.complete();
    Location l(0, 0);
    w.add_creature(&s, east, l);
    Creature& a = w.zoo[0];

    a.take_turn(w, l);
    ASSERT_EQ(w.grid.end(), w.grid.find(l));
}

TEST(Creature_tests, turn_facing){
    World w(2, 2);
    Species s("s");
    s.add_instruction({left});
    s.complete();
    Location l(0, 0);
    w.add_creature(&s, east, l);
    Creature& a = w.zoo[0];

    a.take_turn(w, l);
    ASSERT_EQ(north, a.facing);
}

TEST(Creature_tests, turn_condition){
    World w(2, 2);
    Species s("s");
    s.add_instruction({go, 1});
    s.add_instruction({left});
    s.complete();
    Location l(0, 0);
    w.add_creature(&s, east, l);
    Creature& a = w.zoo[0];

    a.take_turn(w, l);
    ASSERT_EQ(north, a.facing);
}

TEST(Creature_tests, turn_random){
    World w(2, 2);
    Species s("s");
    s.add_instruction({if_random, 2});
    s.add_instruction({left});
    s.add_instruction({right});
    s.complete();
    Location l(0, 0);
    w.add_creature(&s, east, l);
    Creature& a = w.zoo[0];
    srand(0);
    a.take_turn(w, l);
    ASSERT_EQ(south, a.facing);
}

TEST(Creature_tests, turn_count){
    World w(2, 2);
    Species s("s");
    s.add_instruction({go, 1});
    s.add_instruction({left});
    s.complete();
    Location l(0, 0);
    w.add_creature(&s, east, l);
    Creature& a = w.zoo[0];

    a.take_turn(w, l);
    ASSERT_EQ(1, a.turns);
}

TEST(Creature_tests, turn_complex){
    World w(2, 2);
    Species s("s");
    s.add_instruction({go, 1});
    s.add_instruction({if_wall, 3});
    s.add_instruction({hop});
    s.add_instruction({left});
    s.add_instruction({go, 0});
    s.complete();
    Location l(0, 0);
    w.add_creature(&s, east, l);
    Creature& a = w.zoo[0];

    w.step();
    w.step();
    ASSERT_EQ(4, a.pc);
}

TEST(Creature_tests, turn_complex2){
    World w(2, 2);
    Species s("s");
    s.add_instruction({if_empty, 3});
    s.add_instruction({left});
    s.add_instruction({go, 0});    
    s.add_instruction({hop});
    s.add_instruction({go, 0});
    s.complete();
    Location l(0, 0);
    w.add_creature(&s, east, l);
    Creature& a = w.zoo[0];

    w.step();
    w.step();
    ASSERT_EQ(north, a.facing);
}

TEST(Creature_tests, printing){
    const char* const name = "s";
    Species s(name);
    Creature a(&s, north);
    std::ostringstream w;
    a.print(w);

    ASSERT_STREQ(name, w.str().c_str());
}

TEST(Creature_tests, same_species){
    Species food("a");
    Creature a(&food, north);
    Creature b(&food, east);

    ASSERT_EQ(a, b);
}

TEST(Creature_tests, different_species){
    Species food("a");
    Species rover("b");
    Creature a(&food, north);
    Creature b(&rover, east);

    ASSERT_FALSE(a == b);
}

TEST(Creature_tests, taken_turns){
    Species food("a");
    Creature a(&food, north);

    ASSERT_TRUE(a.has_taken_turns(0));
}

TEST(Creature_tests, taken_one_turn){
    Species food("a");
    Creature a(&food, north);

    ASSERT_FALSE(a.has_taken_turns(1));
}

TEST(Creature_tests, taken_two_turns){
    Species food("a");
    Creature a(&food, north);
    a.turns = 2;

    ASSERT_TRUE(a.has_taken_turns(1));
}

TEST(Creature_tests, facing_left_n){
    Species food("a");
    Creature a(&food, north);
    a.turn_left();

    ASSERT_EQ(west, a.facing);
}

TEST(Creature_tests, facing_left_e){
    Species food("a");
    Creature a(&food, east);
    a.turn_left();

    ASSERT_EQ(north, a.facing);
}

TEST(Creature_tests, facing_left_s){
    Species food("a");
    Creature a(&food, south);
    a.turn_left();

    ASSERT_EQ(east, a.facing);
}

TEST(Creature_tests, facing_left_w){
    Species food("a");
    Creature a(&food, west);
    a.turn_left();

    ASSERT_EQ(south, a.facing);
}

TEST(Creature_tests, facing_right_n){
    Species food("a");
    Creature a(&food, north);
    a.turn_right();

    ASSERT_EQ(east, a.facing);
}

TEST(Creature_tests, facing_right_e){
    Species food("a");
    Creature a(&food, east);
    a.turn_right();

    ASSERT_EQ(south, a.facing);
}

TEST(Creature_tests, facing_right_s){
    Species food("a");
    Creature a(&food, south);
    a.turn_right();

    ASSERT_EQ(west, a.facing);
}

TEST(Creature_tests, facing_right_w){
    Species food("a");
    Creature a(&food, west);
    a.turn_right();

    ASSERT_EQ(north, a.facing);
}

TEST(Location_tests, order_before){
    Location a(0, 0);
    Location b(4, 5);

    ASSERT_LT(a, b);
}

TEST(Location_tests, order_self){
    Location a(0, 0);

    ASSERT_FALSE(a < a);
}

TEST(Location_tests, order_before_same_row){
    Location a(0, 0);
    Location b(0, 3);

    ASSERT_LT(a, b);
}

TEST(Location_tests, order_before_same_column){
    Location a(0, 0);
    Location b(1, 0);

    ASSERT_LT(a, b);
}

TEST(Location_tests, order_row_major){
    Location a(0, 5);
    Location b(5, 0);

    ASSERT_LT(a, b);
}

TEST(Location_tests, bounds_nonnegative){
    Location a(-5, 2);

    ASSERT_FALSE(a.within_bounds(1, 1));
}

TEST(Location_tests, bounds_over){
    Location a(5, 2);

    ASSERT_FALSE(a.within_bounds(3, 3));
}

TEST(Location_tests, bounds_pass){
    Location a(5, 2);

    ASSERT_TRUE(a.within_bounds(10, 10));
}

TEST(Location_tests, bounds_edge){
    Location a(5, 0);

    ASSERT_TRUE(a.within_bounds(10, 10));
}

TEST(Location_tests, bounds_corner){
    Location a(9, 9);

    ASSERT_TRUE(a.within_bounds(10, 10));
}

TEST(Location_tests, navigate_north){
    Location b = Location(1, 0) + north;

    ASSERT_EQ(0, b.x);
    ASSERT_EQ(0, b.y);
}

TEST(Location_tests, navigate_east){
    Location b = Location(3, 5) + east;

    ASSERT_EQ(6, b.x);
    ASSERT_EQ(3, b.y);
}

TEST(Location_tests, navigate_west){
    Location b = Location(3, 5) + west;

    ASSERT_EQ(4, b.x);
    ASSERT_EQ(3, b.y);
}

TEST(Location_tests, navigate_south){
    Location b = Location(3, 5) + south;

    ASSERT_EQ(5, b.x);
    ASSERT_EQ(4, b.y);
}

TEST(World_tests, add_creature){
    World w(10, 10);
    Species s("s");
    s.add_instruction({hop});
    s.complete();
    w.add_creature(&s, south, Location(5, 4));

    ASSERT_EQ(1, w.zoo.size());
}

TEST(World_tests, add_null_species){
    World w(10, 10);
    try {
        w.add_creature(0, south, Location(5, 4));
        FAIL();
    } catch(std::invalid_argument& a){}
}

TEST(World_tests, add_out_of_bounds){
    World w(10, 10);
    Species s("s");
    s.add_instruction({hop});
    s.complete();

    try {
        w.add_creature(&s, south, Location(53, 4));
        FAIL();
    } catch(std::out_of_range& a){}
}

TEST(World_tests, add_same_place){
    World w(10, 10);
    Species s("s");
    s.add_instruction({hop});
    s.complete();

    try {
        w.add_creature(&s, south, Location(5, 4));
        w.add_creature(&s, south, Location(5, 4));
    } catch(...){
        FAIL();
    }
}

TEST(World_tests, step){
    World w(10, 10);
    Species s("s");
    s.add_instruction({hop});
    s.add_instruction({go, 0});
    s.complete();

    w.add_creature(&s, south, Location(0, 5));
    w.step();
    ASSERT_TRUE(w.zoo[0].has_taken_turns(1));
}

TEST(World_tests, step_two){
    World w(10, 10);
    Species s("s");
    s.add_instruction({hop});
    s.add_instruction({go, 0});
    s.complete();

    w.add_creature(&s, south, Location(0, 5));
    w.add_creature(&s, south, Location(0, 8));
    w.step();
    ASSERT_TRUE(w.zoo[0].has_taken_turns(1));
    ASSERT_TRUE(w.zoo[1].has_taken_turns(1));
}

TEST(World_tests, printing){
    World w(2, 2);
    Species s("s");
    s.add_instruction({hop});
    s.complete();
    w.add_creature(&s, south, Location(0, 1));

    std::ostringstream out;
    w.print(out);
    ASSERT_STREQ("Turn = 0.\n  01\n0 .s\n1 ..\n\n", out.str().c_str());
}

TEST(World_tests, printing_mod){
    World w(1, 11);

    std::ostringstream out;
    w.print(out);
    ASSERT_STREQ("Turn = 0.\n  01234567890\n0 ...........\n\n", out.str().c_str());
}

TEST(World_tests, printing_step){
    World w(2, 2);
    Species s("s");
    s.add_instruction({hop});
    s.complete();
    w.add_creature(&s, south, Location(0, 1));
    w.step();

    std::ostringstream out;
    w.print(out);
    ASSERT_STREQ("Turn = 1.\n  01\n0 ..\n1 .s\n\n", out.str().c_str());
}

TEST(World_tests, move){
    World w(5, 5);
    Species s("s");
    s.add_instruction({hop});
    s.complete();
    direction d = south;
    Location l(0, 1);
    w.add_creature(&s, d, l);

    w.move(l, d);
    ASSERT_EQ(1, w.grid.count(l + d));
    ASSERT_EQ(0, w.grid.count(l));
}

TEST(World_tests, move_wall){
    World w(5, 5);
    Species s("s");
    s.add_instruction({hop});
    s.complete();
    direction d = south;
    Location l(4, 4);
    w.add_creature(&s, d, l);

    w.move(l, d);
    ASSERT_EQ(1, w.grid.count(l));
}

TEST(World_tests, move_other){
    World w(5, 5);
    Species s("s");
    s.add_instruction({hop});
    s.complete();
    direction d = south;
    Location l(0, 2);
    w.add_creature(&s, d, l);
    w.add_creature(&s, east, l + d);

    w.move(l, d);
    ASSERT_EQ(1, w.grid.count(l));
    ASSERT_EQ(1, w.grid.count(l + d));
}

TEST(World_tests, free_space_found){
    World w(5, 5);
    ASSERT_TRUE(w.free_space(Location(0, 3)));
}

TEST(World_tests, free_space_bounds){
    World w(5, 5);
    ASSERT_FALSE(w.free_space(Location(0, 6)));
}

TEST(World_tests, free_space_creature){
    World w(5, 5);
    Species s("s");
    s.add_instruction({hop});
    s.complete();
    Location l(0, 1);
    w.add_creature(&s, south, l);

    ASSERT_FALSE(w.free_space(l));
}

TEST(World_tests, infect){
    World w(5, 5);
    Species food("a");
    food.add_instruction({hop});
    food.complete();
    Species rover("b");
    rover.add_instruction({left});
    rover.complete();
    Location l(1, 2);
    w.add_creature(&food, south, l);
    w.add_creature(&rover, south, l + south);
    w.infect(l, south);

    ASSERT_EQ(w.zoo[0], w.zoo[1]);
}

TEST(World_tests, infect_nothing){
    World w(5, 5);
    Species food("a");
    food.add_instruction({hop});
    food.complete();
    Location l(1, 2);
    w.add_creature(&food, south, l);
    w.infect(l, south);
}

TEST(World_tests, infect_friend){
    World w(5, 5);
    Species food("a");
    food.add_instruction({hop});
    food.complete();
    Location l(1, 2);
    w.add_creature(&food, south, l);
    w.add_creature(&food, south, l + south);
    w.zoo[1].pc = 1;
    w.infect(l, south);

    ASSERT_NE(0, w.zoo[1].pc);
}

TEST(World_tests, wall_interior){
    World w(5, 5);
    Location l(2, 3);

    ASSERT_FALSE(w.if_wall(l, north));
}

TEST(World_tests, wall_facing){
    World w(5, 5);
    Location l(2, 4);

    ASSERT_TRUE(w.if_wall(l, east));
}

TEST(World_tests, wall_facing_away){
    World w(5, 5);
    Location l(2, 4);

    ASSERT_FALSE(w.if_wall(l, west));
}

TEST(World_tests, wall_corner){
    World w(5, 5);
    Location l(4, 4);

    ASSERT_TRUE(w.if_wall(l, east));
}

TEST(World_tests, wall_all){
    World jail(1, 1);
    Location l(0, 0);

    ASSERT_TRUE(jail.if_wall(l, east));
    ASSERT_TRUE(jail.if_wall(l, west));
    ASSERT_TRUE(jail.if_wall(l, south));
    ASSERT_TRUE(jail.if_wall(l, north));
}

TEST(World_tests, enemy_found){
    World w(4, 4);
    Species food("a");
    food.add_instruction({hop});
    food.complete();
    Species rover("b");
    rover.add_instruction({left});
    rover.complete();
    Location l(1, 2);
    w.add_creature(&food, south, l);
    w.add_creature(&rover, south, l + south);

    ASSERT_TRUE(w.if_enemy(l, south));
}

TEST(World_tests, enemy_friend){
    World w(4, 4);
    Species food("a");
    food.add_instruction({hop});
    food.complete();
    Location l(1, 2);
    w.add_creature(&food, south, l);
    w.add_creature(&food, south, l + south);

    ASSERT_FALSE(w.if_enemy(l, south));
}

TEST(World_tests, enemy_paranoia){
    World w(4, 4);
    Species food("a");
    food.add_instruction({hop});
    food.complete();
    Location l(1, 2);
    w.add_creature(&food, south, l);

    ASSERT_FALSE(w.if_enemy(l, south));
}

TEST(Species_tests, add_instruction1){
    Species s("s");
    s.add_instruction({hop});

    ASSERT_EQ(1, s.instructions.size());
}

TEST(Species_tests, add_instruction2){
    Species s("s");
    s.add_instruction({go, 2});
    s.add_instruction({hop});

    ASSERT_EQ(2, s.instructions.size());
}

TEST(Species_tests, completion){
    Species s("s");
    s.add_instruction({hop});
    s.complete();

    ASSERT_TRUE(s.completed);
}

TEST(Species_tests, completion_check){
    Species s("s");
    s.add_instruction({hop});
    ASSERT_FALSE(s.ready());
    s.complete();
    ASSERT_TRUE(s.ready());
}

TEST(Species_tests, completion_fails){
    Species s("s");
    try {
        s.complete();
        FAIL();
    } catch (std::invalid_argument&){}
}

TEST(Species_tests, printing){
    Species s("s");
    std::ostringstream w;
    s.print_short_name(w);

    ASSERT_STREQ("s", w.str().c_str());
}

TEST(Species_tests, printing_short){
    Species s("st");
    std::ostringstream w;
    s.print_short_name(w);

    ASSERT_STREQ("s", w.str().c_str());
}
