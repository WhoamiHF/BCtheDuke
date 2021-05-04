#include "Stategy_manager.h"

void strategy_manager_t::compare_stategies(parameters_t* stategy_a,parameters_t* stategy_b) {
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
	//compare parents
	//if one is super, copy?
	chromozome_t child = crossover(parents);
	child = mutate(child);
	//remove worse parent, add child
}
void strategy_manager_t::print_chromozome(chromozome_t x) {
	for (size_t i = 0; i < SIZE_OF_CHROMOZOME; i++) {
		std::cout << x[i];
	}
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
	size_t entires = 1 / PROBABILITY_MUTATION;
	srand(time(NULL));
	for (size_t index = 0; index < SIZE_OF_CHROMOZOME; index++) {
		if (rand() % SIZE_OF_CHROMOZOME == 0) {
			child[index] = !child[index];
		}
	}
	print_chromozome(child);
	return child;
}

parents_t strategy_manager_t::select_parents() {
	return parents_t();
}