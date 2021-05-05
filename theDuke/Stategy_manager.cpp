#include "Stategy_manager.h"
#include <fstream>
#include <iostream>
#include <string> 

bool strategy_manager_t::compare_chromozomes(parameters_t* stategy_a,parameters_t* stategy_b) {
	srand(time(NULL));
	int rnd = rand() % 2;
	return rnd == 1;
	int wins_strategy_a = 0;
	int draws = 0;
	int wins_strategy_b = 0;

	precomputations_t moves = precomputations_t();

	for (size_t i = 0; i < NUMBER_OF_ROUNDS/2; i++) {
		game_t gameLogic = game_t(&moves.sheet_odd, &moves.sheet_even, stategy_a, stategy_b);
		gameLogic.play();

		switch (gameLogic.game_state) {
		case first_player_won:
			wins_strategy_a++;
			break;
		case draw:
			draws++;
			break;
		case second_player_won:
			wins_strategy_b++;
			break;
		}
	}
	// now with switched first and second players - it is important that they are switched in switch also!
	for (size_t i = 0; i < NUMBER_OF_ROUNDS / 2; i++) {
		game_t gameLogic = game_t(&moves.sheet_odd, &moves.sheet_even, stategy_b, stategy_a); 
		gameLogic.play();

		switch (gameLogic.game_state) {
		case first_player_won:
			wins_strategy_b++;
			break;
		case draw:
			draws++;
			break;
		case second_player_won:
			wins_strategy_a++;
			break;
		}
	}
	std::cout << "Strategy a wins " << wins_strategy_a << std::endl;
	std::cout << "Draws " << draws << std::endl;
	std::cout << "strategy b wins " << wins_strategy_b << std::endl;
	return wins_strategy_a > wins_strategy_b;
}

void strategy_manager_t::load_parameters(std::map<troop_name, int>& troop_values) {
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

void strategy_manager_t::evolve() {
	auto parents = select_parents();
	bool first_is_better = compare_chromozomes(&parameters_pool[parents.first], &parameters_pool[parents.second]);
	//if one is super, copy?
	chromozome_t child = crossover(parents);
	child = mutate(child);
	if (first_is_better) {
		chromozome_pool[parents.second] = child;
		fitness_pool[parents.second] = STARTING_FITNESS;
		parameters_pool[parents.second] = convert_to_parameters(child);
		
		fitness_pool[parents.first]++;
	}
	else {
		chromozome_pool[parents.first] = child;
		fitness_pool[parents.first] = STARTING_FITNESS;
		parameters_pool[parents.first] = convert_to_parameters(child);

		fitness_pool[parents.second]++;
	}
}
void strategy_manager_t::print_chromozome(chromozome_t x) {
	for (size_t i = 0; i < SIZE_OF_CHROMOZOME; i++) {
		std::cout << x[i];
	}
	std::cout << std::endl;
}

chromozome_t strategy_manager_t::crossover(parents_t parents_indexes) {
	chromozome_t child = chromozome_t(SIZE_OF_CHROMOZOME, true);
	srand(time(NULL));
	size_t border = rand() % SIZE_OF_CHROMOZOME;
	for (size_t i = 0; i < border; i++) {
		child[i] = chromozome_pool[parents_indexes.first][i];
	}
	for (size_t i = border; i < SIZE_OF_CHROMOZOME; i++) {
		child[i] = chromozome_pool[parents_indexes.second][i];
	}
	print_chromozome(child);
	return child;
}

chromozome_t strategy_manager_t::mutate(chromozome_t child) {
	srand(time(NULL));
	for (size_t index = 0; index < SIZE_OF_CHROMOZOME; index++) {
		if (rand() % SIZE_OF_CHROMOZOME / PROBABILITY_MUTATION == 0) {
			std::cout << "mutation!";
			child[index] = !child[index];
		}
	}
	print_chromozome(child);
	return child;
}

size_t strategy_manager_t::choose_parent() {
	size_t sum = 0;
	for (size_t i = 0; i < chromozome_pool.size(); i++) {
		sum += fitness_pool[i];
	}
	int wheel = rand() % sum;
	size_t index = 0;
	while (wheel >= 0) {
		wheel -= fitness_pool[index];
		index++;
	}
	index--;
	return index;
}

parents_t strategy_manager_t::select_parents() {
	srand(time(NULL));
	size_t parent_a = choose_parent();
	size_t parent_b = parent_a;
	while (parent_a == parent_b && chromozome_pool.size() != 1) {
		parent_b = choose_parent();
	}
	return parents_t(parent_a, parent_b);
}

void strategy_manager_t::load_chromozomes() {
	std::string file_name = "chromozomes/generation" + std::to_string(generation) + ".txt";
	std::ifstream file(file_name);
	std::string line;
	while (std::getline(file, line)) {
		chromozome_t chromozome = chromozome_t();
		size_t fitness = 0;
		for (size_t i = 0; i < SIZE_OF_CHROMOZOME; i++) {
			chromozome.push_back(line[i] == '1');
		}
		for (size_t i = SIZE_OF_CHROMOZOME + 1; i < line.size(); i++) {
			fitness = fitness * 10;
			fitness = fitness + line[i] - '0';		
		}
		add_chromozome(chromozome, fitness);
		print_chromozome(chromozome);
	}

}

void strategy_manager_t::add_chromozome(chromozome_t chromozome,size_t fitness) {
	chromozome_pool.push_back(chromozome);
	fitness_pool.push_back(fitness);
	parameters_pool.push_back(convert_to_parameters(chromozome));
}

void strategy_manager_t::save_chromozomes() {
	std::string file_name = "chromozomes/generation" + std::to_string(generation) + ".txt";
	std::ofstream file(file_name);
	for (size_t i = 0; i < chromozome_pool.size(); i++) {
		for (size_t j = 0; j < SIZE_OF_CHROMOZOME; j++) {
			file << chromozome_pool[i][j];
		}
		file << " " << fitness_pool[i] << std::endl; 
	}
}

parameters_t strategy_manager_t::convert_to_parameters(chromozome_t chromozome) {
	std::map<troop_name, int> x = std::map<troop_name, int>();
	load_parameters(x);
	parameters_t p1 = parameters_t(x, true, 0.2, 0.1, 4, 4, 2);
	return p1;
}