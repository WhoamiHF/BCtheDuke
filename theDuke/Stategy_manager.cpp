#include "Stategy_manager.h"
#include <fstream>
#include <iostream>
#include <string> 
#include <set>

/* lets two chromozomes play against each other and return if first chromozome was better*/
bool strategy_manager_t::compare_chromozomes(parameters_t* strategy_a, parameters_t* strategy_b,chromozome_t chromozome_a, chromozome_t chromozome_b) {

	int wins_strategy_a = 0;
	int draws = 0;
	int wins_strategy_b = 0;

	precomputations_t moves = precomputations_t();

	for (size_t i = 0; i < NUMBER_OF_ROUNDS / 2; i++) { //@todo: winning
		std::string filename = "Logs/Generation_"+std::to_string(generation)+"[game_"+std::to_string(i)+"]";
		logger log = logger(filename,chromozome_a,chromozome_b,*strategy_a,*strategy_b);
		game_t gameLogic = game_t(&moves.sheet_odd, &moves.sheet_even, strategy_a, strategy_b,true,true,&log);
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
	for (size_t i = 0; i < (NUMBER_OF_ROUNDS+1) / 2; i++) {
		std::string filename = "Logs/Generation_" + std::to_string(generation) + "[game_" + std::to_string(i+NUMBER_OF_ROUNDS/2) + "]";
		logger log = logger(filename, chromozome_b, chromozome_a, *strategy_b, *strategy_a);
		game_t gameLogic = game_t(&moves.sheet_odd, &moves.sheet_even, strategy_b, strategy_a,true,true,&log);
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
		if (wins_strategy_a > (NUMBER_OF_ROUNDS - draws) / 2 || wins_strategy_b > (NUMBER_OF_ROUNDS - draws) / 2) {
			break;
		}
	}
	std::cout << "Strategy a wins " << wins_strategy_a << std::endl;
	std::cout << "Draws " << draws << std::endl;
	std::cout << "strategy b wins " << wins_strategy_b << std::endl;
	return wins_strategy_a > wins_strategy_b;
}

/* returns parameters which have best fitness*/
parameters_t strategy_manager_t::get_parameters() {
	if (fitness_pool.size() == 0) {
		throw "No parameter!";
	}
	int best = 0;
	size_t index = 0;
	for (int i = 0; i < fitness_pool.size(); i++) {
		if (fitness_pool[i] > best) {
			best = fitness_pool[i];
			index = i;
		}
	}
	return parameters_pool[index];
}

/* main function for evolving. Chooses one or two parents, if two then it makes crossover. Either way it performs mutation (with some probability). Then it chooses which chrmozomes will
	continue and which will be erased. It saves another generation*/
void strategy_manager_t::evolve() {
	srand(time(NULL));
	int x = rand() % 3;
	if (x == 0) {
		auto parents = select_parents();
		bool first_is_better = compare_chromozomes(&parameters_pool[parents.first], &parameters_pool[parents.second],chromozome_pool[parents.first],chromozome_pool[parents.second]);
		chromozome_t child = crossover(parents);
		child = mutate(child,PROBABILITY_MUTATION,false);
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
	else {
		size_t parent = choose_parent();
		chromozome_t child = mutate(chromozome_pool[parent],PROBABILITY_MUTATION*4,true); 
		parameters_t child_parameters = convert_to_parameters(child);
		if (compare_chromozomes(&child_parameters, &parameters_pool[parent],child,chromozome_pool[parent])) {
			chromozome_pool[parent] = child;
			fitness_pool[parent] = STARTING_FITNESS;
			parameters_pool[parent] = child_parameters;
		}
		else {
			fitness_pool[parent]++;
		}
	}
	generation++;
	save_chromozomes();
}
/* simple function for printing chromozome*/
void strategy_manager_t::print_chromozome(chromozome_t x) {
	for (size_t i = 0; i < SIZE_OF_CHROMOZOME; i++) {
		std::cout << x[i];
	}
	std::cout << std::endl;
}

/* Makes crossover from two parents. Simply generates random index and takes first part from one parent and rest from another */
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

/* Gets chromozome and probability - expected number of changes in whole chromozome */
chromozome_t strategy_manager_t::mutate(chromozome_t parent, double probability,bool necessary_mutation) {
	if (probability == 0) {
		return parent;
	}
	chromozome_t child = chromozome_t(SIZE_OF_CHROMOZOME);
	srand(time(NULL));
	int entries = SIZE_OF_CHROMOZOME / probability;

	bool mutated = false;
	while (!mutated) {
		if (!necessary_mutation) {
			mutated = true;
		}

		for (size_t index = 0; index < SIZE_OF_CHROMOZOME; index++) {
			child[index] = parent[index];
			if ((rand() % entries) == 0) {
				std::cout << "mutation!";
				mutated = true;
				child[index] = !child[index];
			}
		}
	}
	print_chromozome(child);
	return child;
}

/* performs roulette wheel selection of a parent
	returns parent's index
*/
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

/* selects both parents. Basically just uses "choose_parent" but check that parents have different indexes*/
parents_t strategy_manager_t::select_parents() {
	srand(time(NULL));
	std::set<size_t> considered_indexes = std::set<size_t>();
	size_t parent_a = choose_parent();
	considered_indexes.emplace(parent_a);
	size_t parent_b = parent_a;
	while (chromozome_pool[parent_a] == chromozome_pool[parent_b] && (chromozome_pool.size() - considered_indexes.size() != 0)) {
		parent_b = choose_parent();
		considered_indexes.emplace(parent_b);
	}
	return parents_t(parent_a, parent_b);
}

/* Loads all chromozomes from last generation.*/
void strategy_manager_t::load_chromozomes() {
	std::string file_name = "chromozomes/generation_" + std::to_string(generation) + ".txt";
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
			size_t num = line[i] - '0';
			fitness = fitness + num;
		}
		add_chromozome(chromozome, fitness);
		print_chromozome(chromozome);
	}
	if (chromozome_pool.empty()) {
		throw std::invalid_argument("No chromozones");
	}
}

/* adds chromozome to current chromozome pool. Also it adds it fitness and parameters. */
void strategy_manager_t::add_chromozome(chromozome_t chromozome, size_t fitness) {
	chromozome_pool.push_back(chromozome);
	fitness_pool.push_back(fitness);
	parameters_pool.push_back(convert_to_parameters(chromozome));
}

/* saves all current chromozome into file*/
void strategy_manager_t::save_chromozomes() {
	std::string file_name = "chromozomes/generation_" + std::to_string(generation) + ".txt";
	std::ofstream file(file_name);
	for (size_t i = 0; i < chromozome_pool.size(); i++) {
		for (size_t j = 0; j < SIZE_OF_CHROMOZOME; j++) {
			file << chromozome_pool[i][j];
		}
		file << " " << fitness_pool[i] << std::endl;
	}
}

/* performs convertion from chromozome to parameters. */
parameters_t strategy_manager_t::convert_to_parameters(chromozome_t chromozome) {
	std::vector<troop_name> troops = std::vector<troop_name>{ Assassin, Bowman, Dragoon, Footman, General, Champion,
		Knight, Longbowman, Marshall, Pikeman, Priest, Ranger, Seer, Wizard };

	std::map<troop_name, int> troop_values = std::map<troop_name, int>();
	for (size_t i = 0; i < NUMBER_OF_TROOPS; i++) {
		size_t num = 0;
		for (size_t j = 0; j < TROOP_BITS; j++) {
			num = num * 2;
			if (chromozome[i * TROOP_BITS + j]) {
				num++;
			}
		}
		troop_values.emplace(troops[i], num);
	}
	troop_values.emplace(Duke, 1000);

	bool max_or_average = chromozome[NUMBER_OF_TROOPS * TROOP_BITS];
	double coefficient_moves = 0;
	for (size_t i = 0; i < COEFFICIENT_BITS; i++) {
		coefficient_moves = coefficient_moves * 2;
		if (chromozome[NUMBER_OF_TROOPS * TROOP_BITS + 1 + i]) {
			coefficient_moves++;
		}
	}
	coefficient_moves++; //protection for coefficient = 0;
	coefficient_moves = coefficient_moves / 16;

	double coefficient_duke_moves = 0;
	for (size_t i = 0; i < COEFFICIENT_BITS; i++) {
		coefficient_duke_moves = coefficient_duke_moves * 2;
		if (chromozome[NUMBER_OF_TROOPS * TROOP_BITS + 1 + COEFFICIENT_BITS + i]) {
			coefficient_duke_moves++;
		}
	}
	coefficient_duke_moves++;//protection for coefficient = 0;
	coefficient_duke_moves = coefficient_duke_moves / 16;

	double coefficient_active_passive = 0;
	for (size_t i = 0; i < COEFFICIENT_BITS; i++) {
		coefficient_active_passive = coefficient_active_passive * 2;
		if (chromozome[NUMBER_OF_TROOPS * TROOP_BITS + 1 + COEFFICIENT_BITS + COEFFICIENT_BITS + i]) {
			coefficient_active_passive++;
		}
	}
	coefficient_active_passive+=5; //coeficient is more than one
	coefficient_active_passive = coefficient_active_passive / 4;

	return parameters_t(troop_values, max_or_average, coefficient_moves, coefficient_duke_moves, coefficient_active_passive);
}