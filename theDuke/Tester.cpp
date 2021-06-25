#include "Tester.h"
#include <vector>

void tester_t::print_all_possible_moves(std::vector<move_t> possible_moves) {
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


void tester_t::test_1() {
	game_builder builder = game_builder();
	game_director director = game_director(&builder);
	game_t gameLogic = director.create_default_game_for_testing();

	gameLogic.first_player_plays = true;
	gameLogic.add_new_figure(coordinates(1, 1), Duke, false);
	gameLogic.add_new_figure(coordinates(1, 0), Marshall, false);
	gameLogic.move_troop(coordinates(1, 0), coordinates(2, 0));

	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(3, 1), Duke, false);
	gameLogic.add_new_figure(coordinates(2, 1), Pikeman, false);

	gameLogic.print_board();

	std::vector<move_t> possible_moves = std::vector<move_t>();
	gameLogic.collect_all_possible_moves(possible_moves);
	print_all_possible_moves(possible_moves);
}

void tester_t::test_2() {
	game_builder builder = game_builder();
	game_director director = game_director(&builder);
	game_t gameLogic = director.create_default_game_for_testing();

	gameLogic.add_new_figure(coordinates(3, 0), Duke, false);
	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(2, 3), Duke, false);
	gameLogic.add_new_figure(coordinates(3, 3), Longbowman, false);
	gameLogic.add_new_figure(coordinates(2, 4), Marshall, false);
	gameLogic.board[2][4]->starting_position = false;
	gameLogic.board[3][3]->starting_position = false;
	gameLogic.print_board();

	std::vector<move_t> possible_moves = std::vector<move_t>();
	gameLogic.collect_all_possible_moves(possible_moves);
	print_all_possible_moves(possible_moves);
}

void tester_t::test_3() {
	game_builder builder = game_builder();
	game_director director = game_director(&builder);
	game_t gameLogic = director.create_default_game_for_testing();

	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(1, 2), Duke, false);
	gameLogic.add_new_figure(coordinates(1, 3), Footman, false);
	gameLogic.move_troop(coordinates(1, 3), coordinates(1, 4));
	gameLogic.first_player_plays = true;
	gameLogic.add_new_figure(coordinates(2, 2), Duke, false);
	gameLogic.add_new_figure(coordinates(3, 2), General, false);
	gameLogic.add_new_figure(coordinates(2, 3), Assassin, false);
	gameLogic.board[2][2]->starting_position = false;
	gameLogic.board[2][3]->starting_position = false;
	gameLogic.board[3][2]->starting_position = false;
	gameLogic.print_board();

	std::vector<move_t> possible_moves = std::vector<move_t>();
	gameLogic.collect_all_possible_moves(possible_moves);
	print_all_possible_moves(possible_moves);


	//std::cout << "eval:" << gameLogic.evaluate_move(possible_move(tmp, add_it),3);
}

void tester_t::test_4() {
	game_builder builder = game_builder();
	game_director director = game_director(&builder);
	game_t gameLogic = director.create_default_game_for_testing();

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

void tester_t::test_5() {
	game_builder builder = game_builder();
	game_director director = game_director(&builder);
	game_t gameLogic = director.create_default_game_for_testing();

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

void tester_t::test_6() {
	game_builder builder = game_builder();
	game_director director = game_director(&builder);
	game_t gameLogic = director.create_default_game_for_testing();

	gameLogic.add_new_figure(coordinates(1, 0), Duke, false);
	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(0, 0), Duke, false);
	gameLogic.add_new_figure(coordinates(3, 0), Pikeman, true);
	considered_states_t t = considered_states_t();
	size_t turns = TURNS_WITHOUT_CHANGE_DRAW - 1;
	gameLogic.first_player_plays = true;
	gameLogic.computer_play(t, turns);
	gameLogic.print_board();
	gameLogic.print_state();

}

void tester_t::test_7() {
	game_builder builder = game_builder();
	game_director director = game_director(&builder);
	game_t gameLogic = director.create_default_game_for_testing();

	gameLogic.add_new_figure(coordinates(1, 0), Duke, false);
	gameLogic.add_new_figure(coordinates(2, 0), Ranger, false);
	gameLogic.add_new_figure(coordinates(1, 1), Bowman, false);
	gameLogic.first_player_plays = false;
	gameLogic.add_new_figure(coordinates(3, 1), Longbowman, true);
	gameLogic.add_new_figure(coordinates(0, 0), Duke, false);
	gameLogic.add_new_figure(coordinates(3, 0), Pikeman, true);

	std::cout << gameLogic.create_hash() << std::endl;
}

void tester_t::test_8() {
	game_builder builder = game_builder();
	game_director director = game_director(&builder);
	game_t gameLogic = director.create_default_game_for_testing();

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
	gameLogic.computer_play(t, turns);

	gameLogic.print_board();
	gameLogic.print_state();
}