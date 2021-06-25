#pragma once
#include "figures.h"
#include "game.h"
#include "Logger.h"
#include "Stategy_manager.h"
class game_builder {
public:
	game_builder() = default;
	void add_logger() {
		this->log = logger();
	}
	void add_moves() {
		auto moves = precomputations_t();
	}
	void add_values(bool first, std::vector<int> vector_of_values) {
		std::vector<troop_name> names = std::vector<troop_name>{ Assassin, Bowman, Dragoon, Duke, Footman, General, Champion, Knight,
			Longbowman, Marshall, Pikeman, Priest, Ranger, Seer, Wizard };
		if (vector_of_values.size() != names.size()) {
			throw std::invalid_argument("received different size");
			return;
		}

		if (first) {
			this->values_first = std::map<troop_name, int>();
			for (size_t i = 0; i < names.size(); i++) {
				this->values_first.emplace(std::pair<troop_name, int>(names[i], vector_of_values[i]));
			}
		}
		else {
			this->values_second = std::map<troop_name, int>();
			for (size_t i = 0; i < names.size(); i++) {
				this->values_second.emplace(std::pair<troop_name, int>(names[i], vector_of_values[i]));
			}
		}
	}

	game_builder add_parameters(bool first_player, bool _max_of_addition, double _coefficient_possible_moves,
		double _coefficient_duke_moves, double _coefficient_active_passive) {
		if (first_player) {
			parameters_first = parameters_t(values_first, _max_of_addition, _coefficient_possible_moves, _coefficient_duke_moves, _coefficient_active_passive);
		}
		else {
			parameters_second = parameters_t(values_second, _max_of_addition, _coefficient_possible_moves, _coefficient_duke_moves, _coefficient_active_passive);
		}
		return *this;
	}

	void make_game() {
		game = game_t(&moves.sheet_odd, &moves.sheet_even, &parameters_first, &parameters_second, true,true, &log);
	}

	game_t get_game() {
		return game;
	}

private:
	precomputations_t moves;
	std::map<troop_name, int> values_first;
	std::map<troop_name, int> values_second;
	parameters_t parameters_first;
	parameters_t parameters_second;
	logger log;
	game_t game;
};

class game_director {
public:
	game_director(game_builder* builder_) {
		builder = builder_;
	}

	game_t create_default_game_for_testing() {
		std::vector<int> values = std::vector<int>(15, 2);
		values[3] = 1000;
		builder->add_moves();
		builder->add_values(true, values);
		builder->add_values(false, values);
		builder->add_parameters(true, true, 0.2, 0.1, 2);
		builder->add_parameters(false, true, 0.2, 0.1, 2);
		builder->add_logger();
		builder->make_game();
		return builder->get_game();
	}
private:
		game_builder* builder;

};


class tester_t
{
public:

	void print_all_possible_moves(std::vector<move_t> possible_moves);

	/* Tests command, slide and walk, all with and without obstacles
	*/
	void test_1();

	/* test same thing as test_1 and strike with and without target*/
	void test_2();

	/* Tests jump, jump and slide*/
	void test_3();

	void test_4();

	/* testing collect all possible moves, correct implementation of mirroring*/
	void test_5();

	void test_6();

	//Testing correct creating of hash, especially sort function.
	void test_7();

	void test_8();
};

