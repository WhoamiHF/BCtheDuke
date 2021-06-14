// theDuke.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "figures.h"
#include "game.h"
#include "Logger.h"
#include "Stategy_manager.h"

void load_parameters(std::map<troop_name, int>& troop_values) {
	troop_values.emplace(std::pair<troop_name, int>(Duke, 100));
	troop_values.emplace(std::pair<troop_name, int>(Seer, 2));
	troop_values.emplace(std::pair<troop_name, int>(Priest, 2));
	troop_values.emplace(std::pair<troop_name, int>(Longbowman, 2));
	troop_values.emplace(std::pair<troop_name, int>(Footman, 2));
	troop_values.emplace(std::pair<troop_name, int>(Pikeman, 2));
	troop_values.emplace(std::pair<troop_name, int>(General, 2));
	troop_values.emplace(std::pair<troop_name, int>(Knight, 2));
	troop_values.emplace(std::pair<troop_name, int>(Bowman, 2));
	troop_values.emplace(std::pair<troop_name, int>(Wizard, 2));
	troop_values.emplace(std::pair<troop_name, int>(Champion, 2));
	troop_values.emplace(std::pair<troop_name, int>(Ranger, 2));
	troop_values.emplace(std::pair<troop_name, int>(Dragoon, 2));
	troop_values.emplace(std::pair<troop_name, int>(Marshall, 2));
	troop_values.emplace(std::pair<troop_name, int>(Assassin, 2));
}

void print_all_possible_moves(std::vector<move_t> possible_moves) {
	for (auto&& item : possible_moves) {
		switch (item.op) {
		case add_it:
			std::cout << "add " << item.coords[0].x << " " << item.coords[0].y << std::endl;
			break;
		case move_it:
			std::cout << "move " << item.coords[0].x << " " << item.coords[0].y << ", " << item.coords[1].x << " " << item.coords[1].y << std::endl;
			break;
		case command_it:
			std::cout << "command " << item.coords[0].x << " " << item.coords[0].y << ", " << item.coords[1].x << " " << item.coords[1].y << ", " << item.coords[2].x << " " << item.coords[2].y << std::endl;
		}
	}
}

/* Tests command, slide and walk, all with and without obstacles
*/
void test_1() {
	precomputations_t moves = precomputations_t();
	std::map<troop_name, int> x = std::map<troop_name, int>();
	parameters_t p1 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	parameters_t p2 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	game_t gameLogic = game_t(&moves.sheet_odd, &moves.sheet_even,&p1,&p2,true,true);
	gameLogic.add_new_figure(coordinates(1, 1), Duke,false);
	gameLogic.add_new_figure(coordinates(1, 0), Marshall,false);
	gameLogic.move_troop(coordinates(1, 0), coordinates(2, 0));
	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(3, 1), Duke,false);
	gameLogic.add_new_figure(coordinates(2, 1), Pikeman,false);
	gameLogic.first_player_plays = true;
	gameLogic.print_board();
	std::vector<move_t> possible_moves = std::vector<move_t>();
	gameLogic.collect_all_possible_moves(possible_moves);
	print_all_possible_moves(possible_moves);
}

/* test same thing as test_1 and strike with and without target*/
void test_2() {
	precomputations_t moves = precomputations_t();
	std::map<troop_name, int> x = std::map<troop_name, int>();
	parameters_t p1 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	parameters_t p2 = parameters_t(x, true, 0.8, 0.3, 3, 4, 3);
	game_t gameLogic = game_t(&moves.sheet_odd, &moves.sheet_even,&p1,&p2,true,true);
	gameLogic.add_new_figure(coordinates(3, 0), Duke,false);
	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(2, 3), Duke,false);
	gameLogic.add_new_figure(coordinates(3, 3), Longbowman,false);
	gameLogic.add_new_figure(coordinates(2, 4), Marshall,false);
	gameLogic.board[2][4]->starting_position = false;
	gameLogic.board[3][3]->starting_position = false;
	gameLogic.print_board();

	std::vector<move_t> possible_moves = std::vector<move_t>();
	gameLogic.collect_all_possible_moves(possible_moves);
	print_all_possible_moves(possible_moves);
}

/* Tests jump, jump and slide*/
void test_3() {
	precomputations_t moves = precomputations_t();
	std::map<troop_name, int> x = std::map<troop_name, int>();
	parameters_t p1 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	parameters_t p2 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	game_t gameLogic = game_t(&moves.sheet_odd,&moves.sheet_even,&p1,&p2,true,true);
	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(1, 2), Duke,false);
	gameLogic.add_new_figure(coordinates(1, 3), Footman,false);
	gameLogic.move_troop(coordinates(1, 3), coordinates(1, 4));
	gameLogic.first_player_plays = true;
	gameLogic.add_new_figure(coordinates(2, 2), Duke,false);
	gameLogic.add_new_figure(coordinates(3, 2), General,false);
	gameLogic.add_new_figure(coordinates(2, 3), Assassin,false);
	gameLogic.board[2][2]->starting_position = false;
	gameLogic.board[2][3]->starting_position = false;
	gameLogic.board[3][2]->starting_position = false;
	gameLogic.print_board();

	std::vector<move_t> possible_moves = std::vector<move_t>();
	gameLogic.collect_all_possible_moves(possible_moves);
	print_all_possible_moves(possible_moves);


	//std::cout << "eval:" << gameLogic.evaluate_move(possible_move(tmp, add_it),3);
}

void test_4() {
	precomputations_t moves = precomputations_t();
	std::map<troop_name, int> x = std::map<troop_name, int>();
	parameters_t p1 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	parameters_t p2 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	game_t gameLogic = game_t(&moves.sheet_odd, &moves.sheet_even,&p1,&p2,true,true);
	gameLogic.first_player_plays = true;
	gameLogic.add_new_figure(coordinates(2, 0), Duke, false);
	gameLogic.add_new_figure(coordinates(3, 0), Footman, true);
	gameLogic.add_new_figure(coordinates(1, 0), Footman, true);

	gameLogic.add_new_figure(coordinates(2, 1), Champion, true);
	gameLogic.board[2][1]->starting_position = !gameLogic.board[2][1]->starting_position;

	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(2, 5), Duke, false);
	gameLogic.add_new_figure(coordinates(1, 5), Footman, true);
	gameLogic.add_new_figure(coordinates(3, 5), Footman, false);
	gameLogic.add_new_figure(coordinates(2, 4), Ranger, true);
	gameLogic.first_player_plays = true;
	gameLogic.move_troop(coordinates(2, 1), coordinates(0, 1));
	gameLogic.print_board();
	gameLogic.print_packs();

	std::vector<move_t> possible_moves = std::vector<move_t>();
	gameLogic.collect_all_possible_moves(possible_moves);
	print_all_possible_moves(possible_moves);
}

/* testing collect all possible moves, correct implementation of mirroring*/
void test_5() {
	precomputations_t moves = precomputations_t();
	std::map<troop_name, int> x = std::map<troop_name, int>();
	parameters_t p1 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	parameters_t p2 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	game_t gameLogic = game_t(&moves.sheet_odd, &moves.sheet_even,&p1,&p2,true,true);
	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(0, 0), Duke, false);
	gameLogic.add_new_figure(coordinates(1, 0), Pikeman, true);
	gameLogic.add_new_figure(coordinates(0, 1), Assassin, true);
	gameLogic.board[0][0]->starting_position = !gameLogic.board[0][0]->starting_position;
	gameLogic.print_board();
	gameLogic.print_packs(); 
	std::vector<move_t> possible_moves = std::vector<move_t>();
	gameLogic.collect_all_possible_moves(possible_moves);
	print_all_possible_moves(possible_moves);
}

void test_6() {
	precomputations_t moves = precomputations_t();
	std::map<troop_name, int> x = std::map<troop_name, int>();
	load_parameters(x);
	parameters_t p1 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	parameters_t p2 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	game_t gameLogic = game_t(&moves.sheet_odd, &moves.sheet_even, &p1, &p2,true,true);
	gameLogic.add_new_figure(coordinates(1, 0), Duke, false);
	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(0, 0), Duke, false);
	gameLogic.add_new_figure(coordinates(3, 0), Pikeman, true);
	considered_states_t t = considered_states_t();
	size_t turns = TURNS_WITHOUT_CHANGE_DRAW-1;
	gameLogic.first_player_plays = true;
	gameLogic.computer_play(t, turns);
	gameLogic.print_board();
	gameLogic.print_state();
	
}

//Testing correct creating of hash, especially sort function.
void test_7() {
	precomputations_t moves = precomputations_t();
	std::map<troop_name, int> x = std::map<troop_name, int>();
	load_parameters(x);
	parameters_t p1 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	parameters_t p2 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	game_t gameLogic = game_t(&moves.sheet_odd, &moves.sheet_even, &p1, &p2, true, true);
	gameLogic.add_new_figure(coordinates(1, 0), Duke, false);
	gameLogic.add_new_figure(coordinates(2, 0), Ranger, false);
	gameLogic.add_new_figure(coordinates(1, 1), Bowman, false);
	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(3, 1), Longbowman, true);
	gameLogic.add_new_figure(coordinates(0, 0), Duke, false);
	gameLogic.add_new_figure(coordinates(3, 0), Pikeman, true);

	std::cout << gameLogic.create_hash() << std::endl;
}

void test_8() {
	precomputations_t moves = precomputations_t();
	std::map<troop_name, int> x = std::map<troop_name, int>();
	load_parameters(x);
	parameters_t p1 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	parameters_t p2 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	game_t gameLogic = game_t(&moves.sheet_odd, &moves.sheet_even, &p1, &p2, true, true);
	gameLogic.add_new_figure(coordinates(1, 2), Duke, false);
	gameLogic.add_new_figure(coordinates(0, 0), Pikeman, true);
	gameLogic.add_new_figure(coordinates(1, 1), Pikeman, true);
	gameLogic.add_new_figure(coordinates(2, 0), Footman, true);
	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(1, 0), Duke, false);
	gameLogic.board[1][0]->starting_position = false;
	gameLogic.add_new_figure(coordinates(4, 4), Footman, true);
	gameLogic.board[4][4]->starting_position = false;
	considered_states_t t = considered_states_t();
	size_t turns = 10;
	gameLogic.computer_play(t,turns);

	gameLogic.print_board();
	gameLogic.print_state();
}


int main()
{
	srand(time(NULL));
	//test_1();
	//test_2();
	//test_3();
	//test_4();
	//test_5();
	//test_6();
	//test_7();
	test_8();
	//strategy_manager_t mng = strategy_manager_t();
	//mng.evolve();
}

