#include <iostream>
#include "game.h"
#include <array>
#include <memory>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <chrono>

/*gets differences and returns normalized vector and it's "length" (number of movements in this direction before reaching destination)
* @param difference_x - represents x part of non-normalized vector (from start to finish)
* @param difference_y - represents y part of non-normalized vector (from start to finish)
*/
directions game_t::get_directions(coordinates difference) {
	int vector_x = 0;
	int vector_y = 0;
	int distance = 0;

	if (difference.x == 0) {
		vector_x = 0;
		distance = abs(difference.y);
		vector_y = difference.y / distance;
	}
	else if (difference.y == 0) {
		vector_y = 0;
		distance = abs(difference.x);
		vector_x = difference.x / distance;
	}
	else if (difference.x == difference.y || difference.x == -difference.y) {
		vector_x = difference.x / abs(difference.x);
		vector_y = difference.y / abs(difference.y);
		distance = abs(difference.x);
	}
	return directions(vector_x, vector_y, distance);
}

/* checks path - going backwards it checks if slide or jump_and slide is closer to "to square" than obstacle in path (friendly or enemy troop)
* the function is called only if there is no move on move board (or if "to square" is outside of the move board of "from square")
*/
types_of_moves game_t::check_path(coordinates from, coordinates to) {
	int difference_x = to.x - from.x;
	int difference_y = to.y - from.y;

	auto direction = get_directions(coordinates(difference_x, difference_y));

	if (direction.distance == 0) {
		return nothing;
	}

	for (int i = direction.distance - 1; i > 0; i--) {
		int tile_x = from.x + i * direction.x;
		int tile_y = from.y + i * direction.y;

		//check if there is troop in the way
		if (board[tile_x][tile_y] != NULL) {
			return nothing;
			break;
		}

		//get correct tile on move board - first player plays from up to down, left to right so we need to mirror difference around x and y axis
		int move_board_x = first_player_plays ? board[from.x][from.y]->position_on_move_board.x - i * direction.x :
			board[from.x][from.y]->position_on_move_board.x + i * direction.x;
		int move_board_y = first_player_plays ? board[from.x][from.y]->position_on_move_board.y - i * direction.y :
			board[from.x][from.y]->position_on_move_board.y + i * direction.y;


		if (coordinates_on_move_board(move_board_x, move_board_y)) {
			types_of_moves tile = board[from.x][from.y]->starting_position ? board[from.x][from.y]->starting_moves[move_board_x][move_board_y] : 
				board[from.x][from.y]->other_moves[move_board_x][move_board_y];


			if (tile == jump_and_slide || tile == slide) {
				return tile;
				break;
			}
		}
	}
	return nothing;
}

/* checks if coordinates represent square on the board - if they are within the boundries of the board
*/
bool game_t::coordinates_on_board(int x, int y) {
	return x >= 0 && x <= 5 && y >= 0 && y <= 5;
}

/* checks if coordinates represent square on the move board of any figure - if they are within the boundries of a move board
*/
bool game_t::coordinates_on_move_board(int x, int y) {
	return x >= 0 && x <= 4 && y >= 0 && y <= 4;
}

/* checks if figure on [x,y] can command figure to move from "from square" to "to square"
* this means that the figure has command (or walk_or_command) on move board (on both squares) and "from square" is occupied by friendly troop whilst "to square" is not.
*
*/
bool game_t::check_command(coordinates base, coordinates from, coordinates to) {
	//checks if there is friendly troop on "from square"
	if (board[from.x][from.y] == NULL || !belongs_to_current_player(from.x, from.y)) {
		return false;
	}
	//first player plays from up to down, left to right so we need to mirror difference around x and y axisy
	int difference_x_from = first_player_plays ? base.x - from.x : from.x - base.x;
	int difference_y_from = first_player_plays ? base.y - from.y : from.y - base.y;
	int difference_x_to = first_player_plays ? base.x - to.x : to.x - base.x;
	int difference_y_to = first_player_plays ? base.y - to.y :to.y - base.y;

	//gets coordinates on move board of troop on base square
	int from_x_on_board = difference_x_from + board[base.x][base.y]->position_on_move_board.x;
	int from_y_on_board = difference_y_from + board[base.x][base.y]->position_on_move_board.y;
	int to_x_on_board = difference_x_to + board[base.x][base.y]->position_on_move_board.x;
	int to_y_on_board = difference_y_to + board[base.x][base.y]->position_on_move_board.y;


	//checks if coordinates of all three are on board
	if (!coordinates_on_board(base.x, base.y) || !coordinates_on_board(from.x, from.y) || !coordinates_on_board(to.x, to.y)) {
		return false;
	}

	//checks if "from square" and "to_square" are on move board of the troop on position [x][y]
	if (!coordinates_on_move_board(from_x_on_board, from_y_on_board) || !coordinates_on_move_board(to_x_on_board, to_y_on_board)) {
		return false;
	}

	//checks if there is not friendly troop on "to square"
	if (board[to.x][to.y] == NULL || !belongs_to_current_player(to.x, to.y)) {

		auto move_board = board[base.x][base.y]->starting_position ? board[base.x][base.y]->starting_moves :board[base.x][base.y]->other_moves; //todo: COPY?

		if ((move_board[from_x_on_board][from_y_on_board] == command || move_board[from_x_on_board][from_y_on_board] == walk_or_command) &&
			(move_board[to_x_on_board][to_y_on_board] == command || move_board[to_x_on_board][to_y_on_board] == walk_or_command)) {
			return true;
		}
	}
	return false;
}

/* checks if there is obstacle between from and to square.
* by obstacle we mean any troop - friendly or enemy
*
*/
bool game_t::check_walk(coordinates from, coordinates to) {
	int difference_x = to.x - from.x;
	int difference_y = to.y - from.y;

	auto direction = get_directions(coordinates(difference_x, difference_y));

	if (direction.distance == 0) {
		return false;
	}

	for (int i = direction.distance - 1; i > 0; i--) {
		int tile_x = from.x + i * direction.x;
		int tile_y = from.y + i * direction.y;

		if (board[tile_x][tile_y] != NULL) {
			return false;

		}
	}
	return true;
}

//checks if we move friendly troop, and if move can be made - if path is clear (or we jump/strike) and if the destination is not occupied by friendly troop
types_of_moves game_t::get_move(coordinates from, coordinates to, bool strike_anywhere) {
	//check if both tiles are on the board
	if (!coordinates_on_board(from.x, from.y) || !coordinates_on_board(to.x, to.y)) {
		return nothing;
	}

	//checks if the start is occupied by friendly troop
	if (board[from.x][from.y] == NULL || !belongs_to_current_player(from.x, from.y)) {
		return nothing;
	}

	//checks if the destination is not occupied by friendly troop
	if (board[to.x][to.y] != NULL && belongs_to_current_player(to.x, to.y)) {
		return nothing;
	}

	int difference_x = first_player_plays ? from.x - to.x : to.x - from.x;
	int difference_y = first_player_plays ? from.y - to.y : to.y - from.y;

	//gets coordinations on move board 
	int cord_x = difference_x + board[from.x][from.y]->position_on_move_board.x;
	int cord_y = difference_y + board[from.x][from.y]->position_on_move_board.y;

	//checks if the destination is on troop's move board
	if (coordinates_on_move_board(cord_x, cord_y)) {

		//chooses current side of the board
		auto move_board = board[from.x][from.y]->starting_position ? board[from.x][from.y]->starting_moves : board[from.x][from.y]->other_moves;

		switch (move_board[cord_x][cord_y])
		{
		case nothing:
			return check_path(from, to);
			break;
		case walk:
		{
			if (check_walk(from, to)) {
				return walk;
			}
			return nothing;
			break;
		}
		case slide:
			return slide;
			break;
		case jump_and_slide:
			return jump_and_slide;
			break;
		case strike:
			if (!strike_anywhere && board[to.x][to.y] == NULL) {
				return nothing;
			}
			return strike;
			break;
		case jump:
			return jump;
			break;
		case walk_or_command:
		{
			//since command is treated separately, this results only to walk
			if (check_walk(from, to)) {
				return walk;
			}
			return nothing;
			break;
		}
		case command:
			//since command is treated separately, this results to nothing
			return check_path(from, to);
			break;
		default:
			break;
		}
	}
	else {
		return check_path(from, to);
	}
}

/* provides basic print of the current state of the board
*/
void game_t::print_board() {
	std::cout << "  0  1  2  3  4  5 " << std::endl;
	for (size_t i = 0; i < 6; i++)
	{
		std::cout << i << " ";
		for (size_t j = 0; j < 6; j++)
		{
			if (board[j][i] == NULL) {
				std::cout << "__ ";
			}
			else {
				auto tmp = board[j][i]->return_symbol();
				if (!board[j][i]->starting_position) {
					tmp[0] = tolower(tmp[0]);
				}
				std::cout << tmp << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

/* checks if troop on given square belongs to currently playing player. Checking for null is responsibility of caller
*/
bool game_t::belongs_to_current_player(int x, int y) {
	return (board[x][y]->owned_by_first_player && first_player_plays) || (!board[x][y]->owned_by_first_player && !first_player_plays);
}

/*checks if given square contains friendly Duke
*/
bool game_t::is_there_duke(int x, int y) {
	if (coordinates_on_board(x, y) && board[x][y] != NULL && board[x][y]->name == Duke && belongs_to_current_player(x, y)) {
		return true;
	}
	return false;
}

/*checks if given square is next to friendly duke
*/
bool game_t::is_next_to_duke(int x, int y) {
	return is_there_duke(x - 1, y) || is_there_duke(x + 1, y) || is_there_duke(x, y - 1) || is_there_duke(x, y + 1);

}

/* adds new figure with specific name to given square, only if the troop is available, if there is friendly duke next to the square and if the square is empty
* does not change current player
*/
bool game_t::add_new_figure(coordinates to, troop_name name_of_troop, bool anywhere) {
	if (!coordinates_on_board(to.x, to.y) || board[to.x][to.y] != NULL) {
		return false;
	}

	if (!anywhere && name_of_troop != Duke && !is_next_to_duke(to.x, to.y)) {
		std::cout << "add new troop next to your Duke" << std::endl;
		return false;
	}

	bool was_available = first_player_plays ? first_player.packs.deploy_troop(name_of_troop, to.x, to.y) :
		second_player.packs.deploy_troop(name_of_troop, to.x, to.y);


	if (was_available) {
		switch (name_of_troop) {
		case Duke:
			board[to.x][to.y] = std::unique_ptr<figure>(new duke(first_player_plays));
			break;
		case Pikeman:
			board[to.x][to.y] = std::unique_ptr<figure>(new pikeman(first_player_plays));
			break;
		case Marshall:
			board[to.x][to.y] = std::unique_ptr<figure>(new marshall(first_player_plays));
			break;
		case Ranger:
			board[to.x][to.y] = std::unique_ptr<figure>(new ranger(first_player_plays));
			break;
		case Assassin:
			board[to.x][to.y] = std::unique_ptr<figure>(new assassin(first_player_plays));
			break;
		case General:
			board[to.x][to.y] = std::unique_ptr<figure>(new general(first_player_plays));
			break;
		case Knight:
			board[to.x][to.y] = std::unique_ptr<figure>(new knight(first_player_plays));
			break;
		case Longbowman:
			board[to.x][to.y] = std::unique_ptr<figure>(new longbowman(first_player_plays));
			break;
		case Champion:
			board[to.x][to.y] = std::unique_ptr<figure>(new champion(first_player_plays));
			break;
		case Wizard:
			board[to.x][to.y] = std::unique_ptr<figure>(new wizard(first_player_plays));
			break;
		case Seer:
			board[to.x][to.y] = std::unique_ptr<figure>(new seer(first_player_plays));
			break;
		case Footman:
			board[to.x][to.y] = std::unique_ptr<figure>(new footman(first_player_plays));
			break;
		case Priest:
			board[to.x][to.y] = std::unique_ptr<figure>(new priest(first_player_plays));
			break;
		case Bowman:
			board[to.x][to.y] = std::unique_ptr<figure>(new bowman(first_player_plays));
			break;
		case Dragoon:
			board[to.x][to.y] = std::unique_ptr<figure>(new dragoon(first_player_plays));
			break;
		default:
			std::cout << "tried to add troop which was not recognized" << std::endl;
			return false;
			break;
		}
	}
	return true;
}

/* main function for moving troop. Gets which move can be done and does it.
* does not change current player
*/
types_of_moves game_t::move_troop(coordinates from, coordinates to) {
	auto move = get_move(from, to, false);
	switch (move) {
	case nothing:
		return nothing;
		break;
	case strike:
		remove_figure(to.x, to.y);
		board[from.x][from.y]->starting_position = !board[from.x][from.y]->starting_position; //troop stays on same position, but changes moves
		return strike;
		break;
	default:
		if (board[to.x][to.y] != NULL) {
			remove_figure(to.x, to.y);
		}
		board[to.x][to.y] = std::move(board[from.x][from.y]);
		if (first_player_plays) {
			first_player.change_coordinates(from.x, from.y, to.x, to.y);
		}
		else {
			second_player.change_coordinates(from.x, from.y, to.x, to.y);
		}
		board[to.x][to.y]->starting_position = !board[to.x][to.y]->starting_position; //troop has moved
		/*if duke can be taken
		if (first_player_plays) {

		}
		for(auto&&
		undo changes
		return nothing*/
		return move;
		break;
	}
}

/* checks if coordinates are on the board and removes troop on this square from the board and from player's active pack
*  if the square was empty nothing happens
*/
bool game_t::remove_figure(int x, int y) {
	if (!coordinates_on_board(x, y) || board[x][y] == NULL) {
		return false;
	}
	if (board[x][y]->name == Duke) {
		if (first_player_plays) {
			game_state = first_player_won;
		}
		else {
			game_state = second_player_won;
		}
	}
	if (board[x][y]->owned_by_first_player) {
		first_player.kill_troop(x, y);
	}
	else {
		second_player.kill_troop(x, y);
	}
	board[x][y] = NULL;
	return true;
}

/* prints active troops and backup troops
*/
void game_t::print_packs() {
	std::cout << "--------------active--------------" << std::endl;
	first_player.print_active();
	second_player.print_active();
	std::cout << "--------------backup--------------" << std::endl;
	first_player.print_backup();
	second_player.print_backup();
	std::cout << "-----------------------------" << std::endl << std::endl;
}

/** this function is used for interaction with the user for getting next coordinate
*
*/
int game_t::get_safely_next_number(std::stringstream& stream) {
	std::string num;
	if (stream >> num) {
		return std::stoi(num);
	}
	else {
		return -1;
	}
}

/* deals with user's turn - asks for input, checks validity and does corresponding event - moving troop, commanding troop or adding troop
*
*/
bool game_t::user_play() {
	std::string row = "";
	std::cout << "Please insert command and arguments separated by spaces" << std::endl;
	std::getline(std::cin, row);
	std::stringstream s(row);
	std::string command;
	if (!(s >> command)) {
		return false;
	}
	if (command == "move") {
		int fx, fy, tx, ty;
		fx = get_safely_next_number(s);
		fy = get_safely_next_number(s);
		tx = get_safely_next_number(s);
		ty = get_safely_next_number(s);
		if (coordinates_on_board(fx, fy) && coordinates_on_board(tx, ty)) {
			return move_troop(coordinates(fx, fy), coordinates(tx, ty)) != nothing;
		}
		else {
			return false;
		}
	}
	else if (command == "add") {
		int tx, ty;
		tx = get_safely_next_number(s);
		ty = get_safely_next_number(s);
		troop_name name;

		if (!coordinates_on_board(tx, ty)) {
			return false;
		}

		if (first_player_plays) {
			name = first_player.pick_random_backup_figure();
		}
		else {
			name = second_player.pick_random_backup_figure();
		}
		return add_new_figure(coordinates(tx, ty), name, false);
	}
	else if (command == "command") {
		int x, y, fx, fy, tx, ty;
		s >> command;
		x = get_safely_next_number(s);
		y = get_safely_next_number(s);
		fx = get_safely_next_number(s);
		fy = get_safely_next_number(s);
		tx = get_safely_next_number(s);
		ty = get_safely_next_number(s);
		return command_troop(coordinates(x, y), coordinates(fx, fy), coordinates(tx, ty));
	}
	/*else if (command == "print") {
		collect_all_possible_moves();
		for (auto& item : possible_moves) {

		}
		return false;
	}*/
	else {
		return false;
	}

}

/* checks if commanding is possible via check_command and moves the troop, removing enemy if necessary
* does not change current player
*/
bool game_t::command_troop(coordinates base, coordinates from, coordinates to) {
	if (board[base.x][base.y] == NULL) {
		return false;
	}
	if (check_command(base, from, to)) {
		if (board[to.x][to.y] != NULL) {
			remove_figure(to.x, to.y);
		}

		board[to.x][to.y] = std::move(board[from.x][from.y]);

		if (first_player_plays) {
			first_player.change_coordinates(from.x, from.y, to.x, to.y);
		}
		else {
			second_player.change_coordinates(from.x, from.y, to.x, to.y);
		}

		board[base.x][base.y]->starting_position = !board[base.x][base.y]->starting_position; //troop has commanded

		return true;
	}
	return false;
}

/* returns the difference between sums of troop values, from the view of currently playing player*/
double game_t::evaluate_troops(bool first_player_view) {
	parameters_t* parameters = nullptr;

	if (first_player_view) {
		parameters = first_player.parameters;
	}
	else {
		parameters = second_player.parameters;
	}

	double value = 0;
	for (auto& troop : first_player.packs.active) {
		auto it = parameters->troop_values.find(troop.name);
		if (it != parameters->troop_values.end()) {
			value += it->second;
		}
	}
	for (auto& troop : second_player.packs.active) {
		auto it = parameters->troop_values.find(troop.name);
		if (it != parameters->troop_values.end()) {
			value -= it->second;
		}
	}

	for (auto& troop_name : first_player.packs.backup) {
		auto it = parameters->troop_values.find(troop_name);
		if (it != parameters->troop_values.end()) {
			value += it->second / parameters->coeficient_active_passive;
		}
	}

	for (auto& troop_name : second_player.packs.backup) {
		auto it = parameters->troop_values.find(troop_name);
		if (it != parameters->troop_values.end()) {
			value -= it->second / parameters->coeficient_active_passive;
		}
	}

	if (!first_player_plays) {
		value = -value;
	}
	return value;
}

/* Evaluates current state - troop values, possible moves, duke possible moves */
double game_t::evaluate_state(bool maximize, double troops_value) {
	double value = 0;

	std::vector<move_t> possible_moves;
	size_t duke_moves = collect_all_possible_moves(possible_moves);

	double coefficient_moves = second_player.parameters->coefficient_possible_moves;
	double coefficient_duke_moves = second_player.parameters->coefficient_duke_moves;
	if ((first_player_plays && maximize) || (!first_player_plays && !maximize)) {
		double coefficient_moves = first_player.parameters->coefficient_possible_moves;
		double coefficient_duke_moves = first_player.parameters->coefficient_duke_moves;
	}

	if (maximize) {
		value += troops_value;
		value += possible_moves.size() * coefficient_moves;
		value += duke_moves * coefficient_duke_moves;
	}
	else {
		value -= troops_value;
		value -= possible_moves.size() * coefficient_moves;
		value -= duke_moves * coefficient_duke_moves;
	}


	return value;
}

/* main function for finding the best move, uses minimax and alpha beta prunning
* @param depth - number of moves considered
* @param maximize - do we play or is it the oponnent's turn?
* @param alpha - parameter for alpha beta prunning
* @param beta - parameter for alpha beta prunning
* @param troops_value - current troops value
* @param turns_without_change - number of turns without adding or removing any troop
*/
evaluation_and_move_t game_t::minimax(int depth, bool maximize, double alpha, double beta, double troops_value, size_t turns_without_change, considered_states_t& states) {
	std::string hash = create_hash();
	auto it = states.find(hash);
	if (it != states.end() && it->second.depth >= depth) {
		return evaluation_and_move_t(it->second.evaluation, it->second.move);
	}

	if (turns_without_change >= TURNS_WITHOUT_CHANGE_DRAW) {
		game_state = draw;
		return evaluation_and_move_t(0, move_t(std::vector<coordinates>(), add_it));
	}

	//if game is not running then the duke had to be taken previous turn, if current turn is ours then previous was not and we have lost, otherwise we have won 
	if (game_state != running) {
		if (maximize) {
			return evaluation_and_move_t(-EXTREME_EVALUATION * (depth+1), move_t(std::vector<coordinates>(), add_it));
		}
		else {
			return evaluation_and_move_t(EXTREME_EVALUATION * (depth+1), move_t(std::vector<coordinates>(), add_it));
		}
	}

	if (depth == 0) {
		double evaluation = evaluate_state(maximize, troops_value);
		states.emplace(std::pair<std::string, evaluation_depth_and_move_t>(hash, evaluation_depth_and_move_t(evaluation, move_t(std::vector<coordinates>(), add_it), depth)));
		return evaluation_and_move_t(evaluation, move_t(std::vector<coordinates>(), add_it));
	}

	std::vector<move_t> possible_moves;
	collect_all_possible_moves(possible_moves);
	if (possible_moves.size() == 0) {
		game_state = draw;
		return evaluation_and_move_t(0, move_t(std::vector<coordinates>(), add_it));
	}

	move_t best_move;
	if (maximize) {
		double maximal_evaluation = -INFINITY;
		for (auto&& move : possible_moves) {
			double evaluation = evaluate_move(move, depth, true, alpha, beta, troops_value, turns_without_change,states);
			if (evaluation > maximal_evaluation) {
				maximal_evaluation = evaluation;
				best_move = move;
			}
			if (evaluation > alpha) {
				alpha = evaluation;
			}
			if (beta <= alpha || maximal_evaluation >=EXTREME_EVALUATION) {
				break;
			}
		}
		if (it != states.end() && depth >= it->second.depth) {
			states.erase(it);
		}
		states.emplace(std::pair<std::string, evaluation_depth_and_move_t>(hash, evaluation_depth_and_move_t(maximal_evaluation, best_move, depth)));
		return evaluation_and_move_t(maximal_evaluation, best_move);
	}
	else {
		double minimal_evaluation = INFINITY;
		for (auto&& move : possible_moves) {
			double evaluation = evaluate_move(move, depth, false, alpha, beta, troops_value, turns_without_change,states);
			if (evaluation < minimal_evaluation) {
				minimal_evaluation = evaluation;
				best_move = move;
			}
			if (evaluation < beta) {
				beta = evaluation;
			}
			if (beta <= alpha || minimal_evaluation <= -EXTREME_EVALUATION) {
				break;
			}
		}
		if (it != states.end() && depth >= it->second.depth) {
			states.erase(it);
		}
		states.emplace(std::pair<std::string,evaluation_depth_and_move_t>(hash,evaluation_depth_and_move_t(minimal_evaluation, best_move, depth)));
		return evaluation_and_move_t(minimal_evaluation, best_move);
	}
}

/* Function called by evaluate_move function. Returns game object to state before move was made. For undoing a addition is used undo_addition()
* @param move - kind of move (command,move) ,coordinates
* @param type - type of move (strike,slide)
* @param figure_on_board - troop which was on board
*/
void game_t::undo(move_t move, types_of_moves type, std::unique_ptr<figure> figure_on_board) {
	game_state = running;
	first_player_plays = !first_player_plays;
	coordinates base = coordinates(0, 0);
	coordinates from = coordinates(0, 0);
	coordinates to = coordinates(0, 0);

	if (move.op == move_it) {
		from = move.coords[0];
		to = move.coords[1];
		if (type != strike) {
			board[from.x][from.y] = board[to.x][to.y]->clone();
		}
		board[from.x][from.y]->starting_position = !board[from.x][from.y]->starting_position;
	}

	if (move.op == command_it) {
		base = move.coords[0];
		from = move.coords[1];
		to = move.coords[2];
		board[from.x][from.y] = board[to.x][to.y]->clone();
		board[base.x][base.y]->starting_position = !board[base.x][base.y]->starting_position;
	}


	//change coordinates back
	if (move.op == move_it || move.op == command_it) {
		if (figure_on_board == NULL) {
			board[to.x][to.y] = NULL;
		}
		else {
			board[to.x][to.y] = figure_on_board->clone(); //@todo : move?

			if (first_player_plays) {
				second_player.packs.active.push_back(troop_t(to.x, to.y, figure_on_board->name));
			}
			else {
				first_player.packs.active.push_back(troop_t(to.x, to.y, figure_on_board->name));
			}
		}

		if (first_player_plays) {
			first_player.change_coordinates(to.x, to.y, from.x, from.y);
		}
		else {
			second_player.change_coordinates(to.x, to.y, from.x, from.y);
		}
	}
}

/* Function called by evaluate_move function. Returns game object to state before move was made. Used for undoing addition.
* @param to - coordinates where was troop added
* @param name - which troop was added
*/
void game_t::undo_add(coordinates to, troop_name name) {
	game_state = running;
	first_player_plays = !first_player_plays;
	board[to.x][to.y] = NULL;


	if (first_player_plays) {
		first_player.packs.backup.emplace(name);

		size_t index = 0;
		for (auto& item : first_player.packs.active) {
			if (item.name == name && item.x == to.x && item.y == to.y) {
				break;
			}
			index++;
		}
		std::vector<troop_t>::iterator it = first_player.packs.active.begin();
		std::advance(it, index);
		first_player.packs.active.erase(it); //@todo: smarter
	}
	else {
		size_t index = 0;
		for (auto& item : second_player.packs.active) {
			if (item.name == name && item.x == to.x && item.y == to.y) {
				break;
			}
			index++;
		}
		second_player.packs.backup.emplace(name);
		std::vector<troop_t>::iterator it = second_player.packs.active.begin();
		std::advance(it, index);
		second_player.packs.active.erase(it); //@todo: smarter
	}
}

double game_t::get_troop_value(bool maximize, troop_name name, bool addition) {
	double return_value = 0;
	if ((first_player_plays && maximize) || (!first_player_plays && !maximize)) {
		auto it = first_player.parameters->troop_values.find(name);
		return_value += it->second;
		if (addition) {
			return_value -= it->second / first_player.parameters->coeficient_active_passive;
		}
	}
	else {
		auto it = second_player.parameters->troop_values.find(name);
		return_value += it->second;
		if (addition) {
			return_value -= it->second / second_player.parameters->coeficient_active_passive;
		}
	}
	return return_value;
}

/* makes move, evaluates it and then undoes the changes
*/
double game_t::evaluate_move(move_t move, int depth, bool maximize, double alpha, double beta, double troops_value, size_t turns_without_change, considered_states_t& states) {
	auto next_turns_without_change = turns_without_change + 1;
	switch (move.op) {
	case move_it:
	{
		double next_troops_value = troops_value;
		coordinates from = move.coords[0];
		coordinates to = move.coords[1];
		//remember 
		std::unique_ptr<figure> figure_on_board = NULL;
		if (board[to.x][to.y] != NULL) {
			next_turns_without_change = 0;
			figure_on_board = board[to.x][to.y]->clone();
			next_troops_value += get_troop_value(maximize, figure_on_board->name, false);
		}

		types_of_moves type = move_troop(from, to);
		if (type == nothing) {
			move_troop(from, to);
		}
		first_player_plays = !first_player_plays;
		next_troops_value = -next_troops_value;

		auto return_value = minimax(depth - 1, !maximize, alpha, beta, next_troops_value, next_turns_without_change,states).evaluation;
		undo(move, type, std::move(figure_on_board));

		return return_value;
		break;
	}
	case command_it:
	{
		coordinates base = move.coords[0];
		coordinates from = move.coords[1];
		coordinates to = move.coords[2];
		double next_troops_value = troops_value;

		std::unique_ptr<figure> figure_on_board = NULL;
		if (board[to.x][to.y] != NULL) {
			next_turns_without_change = 0;
			figure_on_board = board[to.x][to.y]->clone();
			next_troops_value += get_troop_value(maximize, figure_on_board->name, false);
		}

		command_troop(base, from, to);

		first_player_plays = !first_player_plays;
		next_troops_value = -next_troops_value;

		auto return_value = minimax(depth - 1, !maximize, alpha, beta, next_troops_value, next_turns_without_change,states).evaluation;

		undo(move, walk, std::move(figure_on_board));

		return return_value;
		break;

	}
	case add_it:
	{
		next_turns_without_change = 0;
		auto collection = first_player_plays ? &first_player.packs.backup : &second_player.packs.backup;

		srand(time(NULL));
		std::unordered_set<troop_name> names = std::unordered_set<troop_name>();
		for (int i = 0; i < collection->size() / PART; i++) {
			bool new_name = false;
			while (!new_name) {
				int random_index = rand() % collection->size();
				std::multiset<troop_name>::iterator it = collection->begin();
				std::advance(it, random_index);
				if (names.find(*it) == names.end()) {
					new_name = true;
					names.emplace(*it);
				}
			}
		}

		double sum = 0;
		double ideal = INFINITY;
		for (auto& name : names) {
			double next_troops_value = troops_value;
			double best = maximize ? -INFINITY : INFINITY;

			next_troops_value += get_troop_value(maximize, name, true);
			next_troops_value = -next_troops_value;

			for (auto& to : move.coords) {
				add_new_figure(to, name, false);
				first_player_plays = !first_player_plays;
				double eval = minimax(depth - 1, !maximize, alpha, beta, next_troops_value, next_turns_without_change,states).evaluation;

				if ((maximize && eval > best) || (!maximize && eval < best)) {
					best = eval;
				}

				undo_add(to, name);
			}
			if (maximize && best < ideal) {
				ideal = best;
			}
			sum += best;
		}
		if ((first_player_plays && first_player.parameters->max_of_addition) || (!first_player_plays && second_player.parameters->max_of_addition)) {
			return ideal;
		}
		else {
			return sum / collection->size();
		}
		break;
	}
	}

}

/* Makes specific move, used in computer_play after determining the best move
*/
void game_t::play_specific_move(move_t move, size_t& turns_without_change, considered_states_t& states) {
	switch (move.op) {
	case move_it:
	{
		coordinates from = move.coords[0];
		coordinates to = move.coords[1];
		if (board[to.x][to.y] != NULL) {
			turns_without_change = 0;
		}
		move_troop(from, to);
		log->log(move);
		break;
	}
	case add_it:
	{
		turns_without_change = 0;
		troop_name name = first_player_plays ? first_player.pick_random_backup_figure() : second_player.pick_random_backup_figure();

		coordinates best_coordinates = coordinates(-1, 0);
		double best_score = -INFINITY;
		double troops_value = evaluate_troops(first_player_plays);
		for (auto&& to : move.coords) {
			double score = evaluate_move(move_t(std::vector<coordinates>{to}, add_it), DEPTH, true, -INFINITY, INFINITY, troops_value, turns_without_change,states);
			if (score >= best_score || best_coordinates.x == -1) {
				best_coordinates = to;
				best_score = score;
			}
		}
		if (best_coordinates.x == -1) {
			std::cout << "wut";
		}
		add_new_figure(best_coordinates, name, false);
		log->log_addition(best_coordinates, name);
		break;
	}
	case command_it:
	{
		coordinates base = move.coords[0];
		coordinates from = move.coords[1];
		coordinates to = move.coords[2];
		command_troop(base, from, to);
		log->log(move);
		break;
	}
	}
}

/* if current player is played by computer this function is called and chooses random move which is then done
*
*/
void game_t::computer_play(considered_states_t& states, size_t& turns_without_change) {
		std::vector<move_t> possible_moves = std::vector<move_t>();
		collect_all_possible_moves(possible_moves);
		number_of_situations++;
		number_of_possibilites += possible_moves.size();
		for (auto&& move : possible_moves) {
			if (move.op == add_it) {
				number_of_possibilites--;
				if (first_player_plays) {
					number_of_possibilites += first_player.packs.backup.size() * move.coords.size();
				}
				else {
					number_of_possibilites += second_player.packs.backup.size() * move.coords.size();
				}
				break;
			}
		}
		double troops_value = evaluate_troops(first_player_plays);
		auto best_branch = minimax(DEPTH, true, -INFINITY, INFINITY, troops_value, turns_without_change, states);
		if (game_state == running) {
			play_specific_move(best_branch.move, turns_without_change,states);
		}
}

/* if game is not running this function will print current state otherwise it does nothing*/
void game_t::print_state() {
	switch (game_state) {
	case first_player_won:
		std::cout << "First player won!" << std::endl;
		break;
	case second_player_won:
		std::cout << "Second player won!" << std::endl;
		break;
	case draw:
		std::cout << "It's a draw!" << std::endl;
		break;
	default:
		break;
	}
}

/* main function - while a player didn't win it will ask players to play and print the board, packs and troops evaluation
*
*/
void game_t::play() {
	auto start = std::chrono::high_resolution_clock::now();
	size_t turns = 1;
	size_t turns_without_change = 0;
	considered_states_t considered_states_first = considered_states_t();
	considered_states_t considered_states_second = considered_states_t();


	place_starting_troops();
	first_player_plays = true;
	while (game_state == running) {
		turns++;
		print_board();
		print_packs();
		std::cout << "troops evaluation " << evaluate_troops(first_player_plays) << std::endl;
		if (first_player_plays) {
			std::cout << turns / 2 << ". First player's turn" << std::endl;
		}
		else {
			std::cout << turns / 2 << ". Second player's turn" << std::endl;
		}

		if (first_player.played_by_pc && first_player_plays) {
			computer_play(considered_states_first, turns_without_change);
		}
		else if (second_player.played_by_pc && !first_player_plays) {
			computer_play(considered_states_second, turns_without_change);
		}
		else {
			while (!user_play()) {}
		}
		print_state();
		turns_without_change++;
		first_player_plays = !first_player_plays;
		if (turns > 250) { game_state = draw; }
	}
	print_board();
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << duration.count() << std::endl;
	std::cout << considered_states_first.size() + considered_states_second.size() << std::endl;
	std::cout << turns << std::endl;
	switch (game_state)
	{
	case first_player_won:
		log->log_message("First player won!");
		break;
	case second_player_won:
		log->log_message("Second player won!");
		break;
	default:
		log->log_message("Match ended in a draw!");
		break;	
	}
	log->update_branch_factor(number_of_situations, number_of_possibilites);
	std::cout << "situations = " << number_of_situations << std::endl;
	std::cout << "branches =" << number_of_possibilites << std::endl;
}

/* called from function play(). Deals with initial placing of troops (Duke and two footman for each player)
*/
void game_t::place_starting_troops() {
	srand(time(NULL));
	add_duke();
	add_footman();
	add_footman();
	first_player_plays = !first_player_plays;
	add_duke();
	add_footman();
	add_footman();
}

/* checks whatever is playing computer or user and calls corresponding function for adding the duke
*
*/
void game_t::add_duke() {
	if ((first_player_plays && first_player.played_by_pc) || !first_player_plays && second_player.played_by_pc) {
		computer_add_duke();
	}
	else {
		user_add_duke();
	}
	print_board();
}

/* checks whatever is playing computer or user and calls corresponding function for adding the footman
*
*/
void game_t::add_footman() {
	if ((first_player_plays && first_player.played_by_pc) || !first_player_plays && second_player.played_by_pc) {
		computer_add_footman();
	}
	else {
		user_add_footman();
	}
	print_board();
}

/* randomly chooses position for the duke and places it there
* @todo: strategy
*/
void game_t::computer_add_duke() {
	std::vector<coordinates> possibilites_first_player{ coordinates(2,0),coordinates(3,0) };
	std::vector<coordinates> possibilites_second_player{ coordinates(2,5),coordinates(3,5) };
	int random_index = rand() % possibilites_first_player.size();;
	if (first_player_plays) {
		add_new_figure(possibilites_first_player[random_index], Duke, false);
		log->log_addition(possibilites_first_player[random_index], Duke);
	}
	else {
		add_new_figure(possibilites_second_player[random_index], Duke, false);
		log->log_addition(possibilites_second_player[random_index], Duke);
	}
}


/* randomly chooses position for the footman and places it there
*/
//@todo: strategy - not interesting
void game_t::computer_add_footman() {
	std::vector<coordinates> possibilities;
	if (first_player_plays) {
		possibilities = std::vector<coordinates>{ coordinates(1,0),coordinates(2,0),coordinates(3,0),coordinates(4,0),coordinates(2,1),coordinates(3,1) };
	}
	else {
		possibilities = std::vector < coordinates>{ coordinates(1,5),coordinates(2,5),coordinates(3,5),coordinates(4,5),coordinates(2,4),coordinates(3,4) };
	}
	srand(time(NULL));
	bool succes = false;
	int random_index = 0;
	while (!succes) {
		random_index = rand() % possibilities.size();
		int x = possibilities[random_index].x;
		int y = possibilities[random_index].y;
		if (board[x][y] == NULL && is_next_to_duke(x, y)) {
			succes = true;
		}
	}
	add_new_figure(possibilities[random_index], Footman, false);
	log->log_addition(possibilities[random_index], Footman);
}

/* checks that x and y represents square on which duke can be placed at the start
*/
bool game_t::check_duke_placement(int x, int y, bool first) {
	if (!coordinates_on_board(x, y)) {
		return false;
	}

	return (first && y == 0 && (x == 2 || x == 3)) || (!first && y == 5 && (x == 2 || x == 3));
}

/* asks user for placement of the footman at the start. It asks till the user places footman next to its own duke
*/
void game_t::user_add_footman() {
	int x, y;
	bool succes = false;
	while (!succes) {
		std::cout << "please type coordinates for placing footman next to your duke" << std::endl;
		std::cin >> x >> y;
		succes = add_new_figure(coordinates(x, y), Footman, false);
	}
	log->log_addition(coordinates(x, y), Footman);
}

/** gets coordinates for placing duke from user and places it
*
*/
void game_t::user_add_duke() {
	int x, y;
	bool succes = false;
	while (!succes) {
		if (first_player_plays) {
			std::cout << "please type coordinates 2 0 or 3 0 for placing duke" << std::endl;
		}
		else {
			std::cout << "please type coordinates 2 5 or 3 5 for placing duke" << std::endl;
		}
		std::cin >> x >> y;
		succes = check_duke_placement(x, y, first_player_plays);
	}
	add_new_figure(coordinates(x, y), Duke, false);
	log->log_addition(coordinates(x, y), Duke);
}

/* This function gets coordinates of square on board and adds to possible moves each pair of squares from and to, representing squares from which and to which can be troop commanded
* caller is responsible for checking rights for using and existence of troop  on this square
* @param x - x coordinate of considered square
* @param y - y coordinate of considered square
* @param[output] possible_moves - reference on containeer which stores all possible moves
*/
void game_t::collect_commands(int x, int y, std::vector<move_t>& possible_moves) {
	auto command_squares = board[x][y]->starting_position ? &board[x][y]->command_squares_starting : &board[x][y]->command_squares_other;

	std::vector<coordinates> start_command = std::vector<coordinates>();
	std::vector<coordinates> end_command = std::vector<coordinates>();
	if (command_squares->size() != 0) {
		for (auto& cord : *command_squares)
		{

			coordinates difference = coordinates(cord.x - board[x][y]->position_on_move_board.x, cord.y - board[x][y]->position_on_move_board.y);
			if (first_player_plays) { //orientation
				difference.x = -difference.x;
				difference.y = -difference.y;
			}
			int bx = x + difference.x;
			int by = y + difference.y;

			if (!coordinates_on_board(bx, by)) {
				continue;
			}

			//check for emptines/enemyness
			if (board[bx][by] == NULL || !belongs_to_current_player(bx, by)) {
				end_command.push_back(coordinates(bx, by));
			}
			else {
				start_command.push_back(coordinates(bx, by));
			}

		}
		for (auto& start : start_command) {
			for (auto& end : end_command) {
				std::vector<coordinates> coords{ coordinates(x,y),start,end };
				possible_moves.push_back(move_t(coords, command_it));
			}
		}
	}
}

/* adds information that is possible to add new troop on given square to possible moves
* @param x - x coordinate of considered square
* @param y - y coordinate of considered square
* @param[output] possible_moves - container which stores possible moves
*/
void game_t::collect_addition(int x, int y, std::vector<coordinates>& squares) {
	if (coordinates_on_board(x, y) && board[x][y] == NULL) {
		squares.push_back(coordinates(x, y));

	}
}

/* collects all possible commands, additions and moves for current player
* @param[output] possible_moves - reference for container which stores possible moves
*/
size_t game_t::collect_all_possible_moves(std::vector<move_t>& possible_moves) {
	size_t duke_moves = 0;
	auto collection = first_player_plays ? &first_player.packs.active : &second_player.packs.active;

	//commands and moves
	for (auto& real_from : *collection) {
		if (real_from.name == Duke) {
			std::vector<coordinates> squares_for_additition = std::vector<coordinates>();
			collect_addition(real_from.x, real_from.y + 1, squares_for_additition);
			collect_addition(real_from.x, real_from.y - 1, squares_for_additition);
			collect_addition(real_from.x + 1, real_from.y, squares_for_additition);
			collect_addition(real_from.x - 1, real_from.y, squares_for_additition);
			if (!squares_for_additition.empty() &&
				((first_player_plays && !first_player.packs.backup.empty()) || (!first_player_plays && !second_player.packs.backup.empty())))
			{
				possible_moves.push_back(move_t(squares_for_additition, add_it));
			}
		}

		collect_commands(real_from.x, real_from.y, possible_moves);

		all_troops_sheet_t::iterator precomputed_moves;
		if (board[real_from.x][real_from.y]->starting_position) {
			precomputed_moves = sheet_odd->find(real_from.name);
			if (precomputed_moves == sheet_odd->end()) {
				exit(404);
			}
		}
		else {
			precomputed_moves = sheet_even->find(real_from.name);
			if (precomputed_moves == sheet_even->end()) {
				exit(404);
			}
		}

		int mirror_from_x = first_player_plays ? real_from.x : 5 - real_from.x;
		int mirror_from_y = first_player_plays ? real_from.y : 5 - real_from.y; //mirror


		for (coordinates& mirror_to : precomputed_moves->second[mirror_from_x][mirror_from_y]) {
			coordinates real_to = mirror_to;
			if (!first_player_plays) {
				int mirror_difference_x = mirror_to.x - mirror_from_x;
				int mirror_difference_y = mirror_to.y - mirror_from_y;
				real_to.x = real_from.x - mirror_difference_x;
				real_to.y = real_from.y - mirror_difference_y;
			}

			auto move = get_move(coordinates(real_from.x, real_from.y), real_to, false);
			if (move == nothing) {
				continue;
			}
			else {
				std::vector<coordinates> coords{ coordinates(real_from.x,real_from.y),real_to };
				possible_moves.push_back(move_t(coords, move_it));
				if (real_from.name == Duke) {
					duke_moves++;
				}
			}
		}
	}
	return duke_moves;
}

/**/
bool compare_troops(const troop_t& first, const troop_t& second) {
	return (first.x < second.x || (first.x == second.x && first.y < second.y));
}

/* appends active (on board) figures to the hash which is given as second parameter
* @param first - marks if we want to append active troops of first player (or second)
* @param[output] hash - current state of hash, which is modified by this function
*/
void game_t::append_active_to_hash(bool first, std::string& hash) {
	auto collection = first ? &first_player.packs.active : &second_player.packs.active; 
	
	std::sort(collection->begin(), collection->end(), compare_troops);

	for (auto& item : *collection) {
		char tmp = 'a' + item.name; //using that troopname is enum, so it takes its index

		if (board[item.x][item.y]->starting_position) {
			tmp = toupper(tmp);
		}
		hash.push_back(tmp);
		hash.push_back('0' + item.x);
		hash.push_back('0' + item.y);
	}
}

/* appends passive (backup) figures to the hash which is given as second parameter
* @param first - marks if we want to append backup of first player (or second)
* @param[output] hash - current state of hash, which is modified by this function
*/
void game_t::append_passive_to_hash(bool first, std::string& hash) {
	auto collection = first ? &first_player.packs.backup : &second_player.packs.backup;

	for (auto& item : *collection) {
		char tmp = 'a' + item;
		hash.push_back(tmp);
	}
}

/* Creates string which represents current state of the game
* whose turn it is, where are figures and what figures are in backup
*/
std::string game_t::create_hash() {
	std::string hash = first_player_plays ? "f" : "s";
	append_active_to_hash(true, hash);
	hash.push_back('|');
	append_active_to_hash(false, hash);
	hash.push_back('|');
	append_passive_to_hash(true, hash);
	hash.push_back('|');
	append_passive_to_hash(false, hash);
	return hash;
}

/* precomputes possible moves for each troop and for each position without regard on other troops (for example if it was the only piece on the board).
*  result contains list of square which should be considered for checking move, other squares are no valid since troop never can go to the square.
* @param[out] sheet_odd - sheet for odd moves of troops
* @param[out] sheet_even - sheet for even moves of troops
*/
void precomputations_t::prepare_possible_moves(all_troops_sheet_t& sheet_odd, all_troops_sheet_t& sheet_even) {
	logger log = logger();
	game_t tmp = game_t(&sheet_odd, &sheet_even, NULL, NULL, true, true,&log);
	size_t total = 0;

	std::vector<troop_name> troops = std::vector<troop_name>{ Duke, Footman,Pikeman,Marshall,General,Longbowman,Priest,Ranger,Knight,Assassin,Dragoon,Champion,Wizard,Seer,Bowman };

	std::vector<coordinates> default_vector = std::vector<coordinates>();
	std::vector<std::vector<coordinates>> default_vector_of_vectors = std::vector<std::vector<coordinates>>(6, default_vector);

	sheet_odd = all_troops_sheet_t();
	sheet_even = all_troops_sheet_t();

	for (auto& troop : troops) {
		single_troop_sheet_t positions_odd = single_troop_sheet_t(6, default_vector_of_vectors);
		single_troop_sheet_t positions_even = single_troop_sheet_t(6, default_vector_of_vectors);

		for (int fx = 0; fx < 6; fx++) {
			for (int fy = 0; fy < 6; fy++) {
				tmp.add_new_figure(coordinates(fx, fy), troop, true);
				for (int i = 0; i < 2; i++) {
					for (int tx = 0; tx < 6; tx++) {
						for (int ty = 0; ty < 6; ty++) {
							auto move = tmp.get_move(coordinates(fx, fy), coordinates(tx, ty), true);
							if (move != nothing) {
								total++;
								if (tmp.board[fx][fy]->starting_position) {
									positions_odd[fx][fy].push_back(coordinates(tx, ty));
								}
								else {
									positions_even[fx][fy].push_back(coordinates(tx, ty));
								}
							}
						}
					}
					tmp.board[fx][fy]->starting_position = false;
				}
				tmp.remove_figure(fx, fy);
				tmp.first_player.packs.backup.emplace(troop);
			}
		}
		sheet_odd.emplace(std::pair<troop_name, single_troop_sheet_t>(troop, positions_odd));
		sheet_even.emplace(std::pair<troop_name, single_troop_sheet_t>(troop, positions_even));
	}
	std::cout << total << std::endl;
}