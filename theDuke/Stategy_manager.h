#pragma once
#include "Logger.h"
#include "player.h"
#include "game.h"
#include "figures.h"
#include <vector>

#define NUMBER_OF_ROUNDS 10
#define PROBABILITY_MUTATION 0.5
#define WINS_NEEDED_TO_CLONE 9
#define SIZE_OF_CHROMOZOME 10
#define STARTING_FITNESS 10

typedef std::pair<size_t, size_t> parents_t;
typedef std::vector<bool> chromozome_t;
typedef std::vector<chromozome_t> chromozome_pool_t;

class strategy_manager_t {
public:
	strategy_manager_t() {
		chromozome_pool = chromozome_pool_t();
		parameters_pool = std::vector<parameters_t>();
		generation = 0;
	}
	bool compare_chromozomes(parameters_t* stategy_a, parameters_t* stategy_b);
	void evolve();
	void load_parameters(std::map<troop_name, int>& troop_values);
	void print_chromozome(chromozome_t x);
	chromozome_t crossover(parents_t parents_indexes);
	chromozome_t mutate(chromozome_t child);
	parameters_t convert_to_parameters(chromozome_t chromozome);
	void save_chromozomes();
	void load_chromozomes();

	parents_t select_parents();
private:
	void add_chromozome(chromozome_t chromozome, size_t fitness);
	size_t choose_parent();
	size_t generation;
	std::vector<int> fitness_pool;
	chromozome_pool_t chromozome_pool;
	std::vector<parameters_t> parameters_pool;
};