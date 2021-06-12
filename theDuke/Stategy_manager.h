#pragma once
#include "Logger.h"
#include "player.h"
#include "game.h"
#include "figures.h"
#include <vector>

#include <filesystem>
#include <iostream>
#include <string>
namespace fs = std::filesystem;

#define NUMBER_OF_ROUNDS 10
#define PROBABILITY_MUTATION 0.5
#define STARTING_FITNESS 20

#define NUMBER_OF_TROOPS 14 //without duke
#define TROOP_BITS 5
#define COEFFICIENT_BITS 4 
#define SIZE_OF_CHROMOZOME NUMBER_OF_TROOPS * TROOP_BITS + 1 +  COEFFICIENT_BITS * 3

typedef std::pair<size_t, size_t> parents_t;
typedef std::vector<bool> chromozome_t;
typedef std::vector<chromozome_t> chromozome_pool_t;

class strategy_manager_t {
public:
	strategy_manager_t() {
		chromozome_pool = chromozome_pool_t();
		parameters_pool = std::vector<parameters_t>();
		std::string path = "chromozomes";
		generation = 0;
		for (const auto& entry : fs::directory_iterator(path)) {
			std::string x = entry.path().string();
		    x = x.substr(x.find('_')+1,x.length()-1);
			x= x.substr(0, x.find('.'));
			if (std::stoi(x) > generation) {
				generation = std::stoi(x);
			}
		}
		load_chromozomes();
	}
	bool compare_chromozomes(parameters_t* stategy_a, parameters_t* stategy_b);
	void evolve();
	parameters_t get_parameters();
	void print_chromozome(chromozome_t x);
	chromozome_t crossover(parents_t parents_indexes);
	chromozome_t mutate(chromozome_t child,double probability);
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