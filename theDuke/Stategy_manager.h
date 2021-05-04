#pragma once
#include "Logger.h"
#include "player.h"
#include "game.h"

#include <vector>

#define NUMBER_OF_ROUNDS 10
#define PROBABILITY_MUTATION 0.005
#define WINS_NEEDED_TO_CLONE 9
#define SIZE_OF_CHROMOZOME 10

typedef std::pair<size_t, size_t> parents_t;
typedef std::vector<bool> chromozome_t;
typedef std::vector<chromozome_t> chromozome_pool_t;

class strategy_manager_t {
public:
	strategy_manager_t() {
		chromozome_pool = chromozome_pool_t();
		chromozome_pool.push_back(chromozome_t(SIZE_OF_CHROMOZOME,true));
		chromozome_pool.push_back(chromozome_t(SIZE_OF_CHROMOZOME, false));
		chromozome_pool.push_back(chromozome_t(SIZE_OF_CHROMOZOME, true));
	}
	void compare_stategies(parameters_t* stategy_a, parameters_t* stategy_b);
	void evolve();
	void load_parameters(std::map<troop_name, int>& troop_values);
	void print_chromozome(chromozome_t x);
	chromozome_t crossover(parents_t parents_indexes);
	chromozome_t mutate(chromozome_t child);
	parameters_t convert_to_parameters(chromozome_t);
	void save_chromozome_pool();
	void load_chromozome_pool();

	parents_t select_parents();
private:
	chromozome_pool_t chromozome_pool;
};