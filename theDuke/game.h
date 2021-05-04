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
	game_t(all_troops_sheet_t* _sheet_odd,all_troops_sheet_t* _sheet_even,parameters_t* first_parameters, parameters_t* second_parameters) {
		first_player_plays = true;
		for (size_t i = 0; i < 6; i++)
		{
			for (size_t j = 0; j < 6; j++)
			{
				board[i][j] = NULL;

			}
		}
		first_player = player_t(true, true,first_parameters);
		second_player = player_t(false, true,second_parameters);
		game_state = running;
		sheet_even = _sheet_even;
		sheet_odd = _sheet_odd;
		log = logger();
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
		//this->log = t.log; @todo necessary constructor?
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
		this->log = logger();
		return *this;
	}

	bool add_new_figure(coordinates to, troop_name name_of_troop,bool anywhere);

	bool first_player_plays; //@todo: private

	void print_board();
	void print_packs();
	void print_state();

	types_of_moves move_troop(coordinates from, coordinates to);

	void play();
	void collect_all_possible_moves(std::vector<move_t>& moves);
	int evaluate_troops();
	std::unique_ptr<figure> board[6][6];
	state_of_game game_state;
	//void prepare_possible_moves(all_troops_sheet_t& sheet_odd, all_troops_sheet_t& sheet_even);

	void computer_play(considered_states_t& states, size_t& turns_without_change);
private:
	void undo_add(coordinates to, troop_name name);
	void undo(move_t type, types_of_moves move, std::unique_ptr<figure> figure_on_board);
	evaluation_and_move_t minimax(int depth, bool maximize, double alpha, double beta,int troops_value, size_t turns_without_change);
	void place_starting_troops();
	void user_add_footman();
	void user_add_duke();
	void computer_add_footman();
	void computer_add_duke();
	void add_duke();
	void add_footman();


	bool user_play();
	int get_safely_next_number(std::stringstream& stream);
	bool check_duke_placement(int x, int y, bool first);


	void collect_addition(int x, int y, std::vector<coordinates>& squares);
	void collect_commands(int x, int y, std::vector<move_t>& possible_moves);

	double evaluate_state(bool maximize,int troops_value);
	double evaluate_move(move_t move, int depth, bool maximize, double alpha, double beta,int troops_value,size_t turns_without_change);

	std::string create_hash();
	void append_active_to_hash(bool first, std::string& hash);
	void append_passive_to_hash(bool first, std::string& hash);

	void play_specific_move(move_t move, size_t& turns_without_change);
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
	logger log;
	//std::string previous_hash;
	//bool can_the_duke_be_taken();
	/*void mark_winning_state();
	void mark_losing_state();
	void load_winning_states();
	std::unordered_set<std::string> winning_states;
	std::vector<std::string> get_hash_after_turn(possible_move move);
	*/
	/*game(bool first_player_plays_, player first, player second) {
	first_player_plays = first_player_plays_;
	first_player = first;
	second_player = second;
	}*/
};

class precomputations_t {
public:
	precomputations_t(){
		sheet_even = all_troops_sheet_t();
		sheet_odd = all_troops_sheet_t();
		prepare_possible_moves(sheet_odd, sheet_even);
		/*game_t tmp = game_t(&sheet_odd, &sheet_even,NULL,NULL);
		tmp.prepare_possible_moves(sheet_odd,sheet_even);*/
	}
	all_troops_sheet_t sheet_odd;
	all_troops_sheet_t sheet_even;
	void prepare_possible_moves(all_troops_sheet_t& sheet_odd, all_troops_sheet_t& sheet_even);
};