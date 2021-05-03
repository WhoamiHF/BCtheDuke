#pragma once
#include <vector>
#include <set>
#include <string>
#include <map>
#include "Move.h"


class parameters_t {
public:
	parameters_t(std::map<troop_name,int> _troop_values, bool _max_of_addition, double _coefficient_possible_moves,
		double _coefficient_duke_moves, int _part_of_addition, int _depth, double _coefficient_active_passive) {
		troop_values = _troop_values;
		max_of_addition = _max_of_addition;
		coefficient_possible_moves = _coefficient_possible_moves;
		coefficient_duke_moves = _coefficient_duke_moves;
		part_of_addition = _part_of_addition;
		depth = _depth;
		coeficient_active_passive = _coefficient_active_passive;
	}
	std::map<troop_name,int> troop_values;
	bool max_of_addition;
	double coefficient_possible_moves;
	double coefficient_duke_moves;
	double coeficient_active_passive;
	int part_of_addition;
	int depth;
};

class troop_t {
public:
	troop_t(int x_, int y_, troop_name name_) {
		x = x_;
		y = y_;
		name = name_;
	}
	bool operator<( const troop_t& right) {
		return this->x < right.x || (this->x == right.x && this->y < right.y) || (this->x == right.x && this->y == right.y && this->name < right.name);
	}

	bool operator==(const troop_t& right) {
		return this->x == right.x  && this->y == right.x && this->name == right.name;
	}

	int x;
	int y;
	troop_name name;
};

class packs_t {
public:
	packs_t() {
		active = std::vector<troop_t>();
		backup = std::vector<troop_name>{ Duke, Footman,Footman,Footman,Pikeman,Pikeman,Pikeman,Marshall,General,Longbowman,Priest,Ranger,Knight,Assassin,Dragoon,Champion,Wizard,Seer,Bowman };
	}
	packs_t(std::vector<troop_t> active_,std::vector<troop_name> backup_) {
		active = active_;
		backup = backup_;
	}
	bool deploy_troop(troop_name figure, int x, int y);
	std::vector<troop_t> active;
	std::vector<troop_name> backup;
	
};

class player_t {
public:
	//friend game;
	player_t() = default;
	player_t(bool _first_player, bool played_by_pc_,parameters_t* _parameters) {
		first_player = _first_player;
		packs_t troop_packs = packs_t();
		played_by_pc = played_by_pc_;
		parameters = _parameters;
	}

	troop_name pick_random_backup_figure();
	//bool deploy_troop(troop_name figure, int x, int y);
	void change_coordinates(int fx, int fy, int tx, int ty);

	void kill_troop(int x, int y);

	void print_symbol(troop_name name);
	void print_active();
	void print_backup();
	bool first_player;
	bool played_by_pc;
	void sort_active_pack();
	//bool compare_troops(const troop& first,const troop& second);
	packs_t packs;
	parameters_t* parameters;
};