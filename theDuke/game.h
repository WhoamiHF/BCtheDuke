#pragma once
#include "figures.h"
#include "player.h"
#include <array>
#include <memory>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include "Logger.h"
#include "Move.h"


#define DEPTH 4
#define PART 4
#define TURNS_WITHOUT_CHANGE_DRAW 30
#define INFINITY 10000
#define EXTREME_EVALUATION INFINITY/10

class player_t;
class precomputations_t;
enum state_of_game {running,first_player_won,second_player_won,draw};


class evaluation_depth_and_move_t {
public:
	evaluation_depth_and_move_t(double eval, move_t move_, int depth_) {
		evaluation = eval;
		move = move_;
		depth = depth_;
	}
	double evaluation;
	move_t move;
	int depth;
};

class evaluation_and_move_t {
public:
	evaluation_and_move_t(double eval, move_t move_) {
		evaluation = eval;
		move = move_;
	}
	double evaluation;
	move_t move;
};

typedef std::unordered_map<std::string, evaluation_depth_and_move_t> considered_states_t;
typedef std::vector< std::vector<std::vector<coordinates>>> single_troop_sheet_t;
typedef std::map<troop_name, single_troop_sheet_t> all_troops_sheet_t;

class game_t {
public:
	friend precomputations_t;
	game_t() = default;
	game_t(all_troops_sheet_t* _sheet_odd,all_troops_sheet_t* _sheet_even,parameters_t* first_parameters,
		parameters_t* second_parameters, bool first_player_pc,bool second_player_pc,logger* _log) {
		first_player_plays = true;
		for (size_t i = 0; i < 6; i++)
		{
			for (size_t j = 0; j < 6; j++)
			{
				board[i][j] = NULL;

			}
		}
		first_player = player_t(true, first_player_pc,first_parameters);
		second_player = player_t(false, second_player_pc,second_parameters);
		game_state = running;
		sheet_even = _sheet_even;
		sheet_odd = _sheet_odd;
		log = _log;
		number_of_possibilites = 0;
		number_of_situations = 0;

	}

	game_t(const game_t& t) {
		for (size_t i = 0; i < 6; i++)
		{
			for (size_t j = 0; j < 6; j++)
			{
				if (t.board[i][j] == NULL) {
					this->board[i][j] = NULL;
				}
				else {
					this->board[i][j] = t.board[i][j]->clone();
				}
			}
		}

		this->first_player = t.first_player;
		this->second_player = t.second_player;
		this->first_player_plays = t.first_player_plays;
		this->sheet_even = t.sheet_even;
		this->sheet_odd = t.sheet_odd;
		this->log = t.log;
	}

	game_t& operator=(const game_t& t) {
		for (size_t i = 0; i < 6; i++)
		{
			for (size_t j = 0; j < 6; j++)
			{
				if (t.board[i][j] == NULL) {
					this->board[i][j] = NULL;
				}
				else {
					this->board[i][j] = t.board[i][j]->clone();
				}
			}
		}

		this->first_player = t.first_player;
		this->second_player = t.second_player;
		this->first_player_plays = t.first_player_plays;
		this->sheet_even = t.sheet_even;
		this->sheet_odd = t.sheet_odd;
		this->log = t.log;
		return *this;
	}

	bool add_new_figure(coordinates to, troop_name name_of_troop,bool anywhere);

	bool first_player_plays; //@todo: private

	void print_board();
	void print_packs();
	void print_state();

	types_of_moves move_troop(coordinates from, coordinates to);

	void play();
	size_t collect_all_possible_moves(std::vector<move_t>& moves);
	double evaluate_troops(bool first_player_view);
	std::unique_ptr<figure> board[6][6];
	state_of_game game_state;
	std::string create_hash();
	long long number_of_possibilites;
	long long number_of_situations;
	void computer_play(considered_states_t& states, size_t& turns_without_change);
private:
	void undo_add(coordinates to, troop_name name);
	void undo(move_t type, types_of_moves move, std::unique_ptr<figure> figure_on_board);
	evaluation_and_move_t minimax(int depth, bool maximize, double alpha, double beta,double troops_value, size_t turns_without_change, considered_states_t& states);
	void place_starting_troops();
	void user_add_footman();
	void user_add_duke();
	void computer_add_footman();
	void computer_add_duke();
	void add_duke();
	void add_footman();
	double get_troop_value(bool maximize,troop_name name,bool addition);

	bool user_play();
	int get_safely_next_number(std::stringstream& stream);
	bool check_duke_placement(int x, int y, bool first);


	void collect_addition(int x, int y, std::vector<coordinates>& squares);
	void collect_commands(int x, int y, std::vector<move_t>& possible_moves);

	double evaluate_state(bool maximize,double troops_value);
	double evaluate_move(move_t move, int depth, bool maximize, double alpha, double beta,double troops_value,size_t turns_without_change, considered_states_t& states);

	void append_active_to_hash(bool first, std::string& hash);
	void append_passive_to_hash(bool first, std::string& hash);

	void play_specific_move(move_t move, size_t& turns_without_change, considered_states_t& states);
	bool command_troop(coordinates base, coordinates from, coordinates to);
	types_of_moves get_move(coordinates from, coordinates to,bool strike_anywhere);
	bool remove_figure(int x, int y);

	directions get_directions(coordinates difference);
	types_of_moves check_path(coordinates from, coordinates to);
	bool coordinates_on_board(int x, int y);
	bool coordinates_on_move_board(int x, int y);

	bool check_walk(coordinates from, coordinates to);
	bool check_command(coordinates base, coordinates from, coordinates to);

	bool is_next_to_duke(int x, int y);
	bool is_there_duke(int x, int y);

	bool belongs_to_current_player(int x, int y);
	
	player_t first_player;
	player_t second_player;
	all_troops_sheet_t* sheet_odd;
	all_troops_sheet_t* sheet_even;
	logger* log;
};

class precomputations_t {
public:
	precomputations_t(){
		sheet_even = all_troops_sheet_t();
		sheet_odd = all_troops_sheet_t();
		prepare_possible_moves(sheet_odd, sheet_even);
	}
	all_troops_sheet_t sheet_odd;
	all_troops_sheet_t sheet_even;
private:
	void prepare_possible_moves(all_troops_sheet_t& sheet_odd, all_troops_sheet_t& sheet_even);
};