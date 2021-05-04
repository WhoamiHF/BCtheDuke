#pragma once
#include <iostream>
#include <array>
#include <memory>
#include <set> //possibly redundant
#include <vector>
#include "player.h"
#include "Move.h"


class directions {
public:
	directions(int x_, int y_, int distance_) {
		x = x_;
		y = y_;
		distance = distance_;
	}
	int x;
	int y;
	int distance;
};

class figure abstract {
public:
	figure() {
		for (size_t i = 0; i < 5; i++)
		{
			for (size_t j = 0; j < 5; j++)
			{
				starting_moves[i][j] = nothing;
				other_moves[i][j] = nothing;
			}
			command_squares_starting = std::vector<coordinates>();
			command_squares_other = std::vector<coordinates>();
			position_on_move_board = coordinates(2, 2);
			name = Duke;
			starting_position = true;
		}
	}

	virtual std::unique_ptr<figure> clone() = 0;
	virtual std::string return_symbol()= 0;
	bool has_command;
	std::vector<coordinates> command_squares_starting;
	std::vector<coordinates> command_squares_other;

	bool starting_position;
	types_of_moves starting_moves[5][5];
	types_of_moves other_moves[5][5];
	
	coordinates position_on_move_board;

	int position_x;
	int position_y;
	bool owned_by_first_player;
	troop_name name;

	void print_boards() {
		std::cout << "current" << std::endl;
		auto current = &other_moves;
		auto next = &starting_moves;
		if (starting_position) {
			current = &starting_moves;
			next = &other_moves;
		}
		for (size_t i = 0; i < 5; i++)
		{
			for (size_t j = 0; j < 5; j++)
			{
				switch ((*current)[j][i]) {
				case jump:
					std::cout << "jp ";
					break;
				case jump_and_slide:
					std::cout << "js ";
					break;
				case walk:
					std::cout << "wl ";
					break;
				case slide:
					std::cout << "sl ";
					break;
				case strike:
					std::cout << "sh ";
					break;
				default:
					std::cout << "__ ";
				}
			}
			std::cout << std::endl;
		}
		std::cout << "next" << std::endl;
		for (size_t i = 0; i < 5; i++)
		{
			for (size_t j = 0; j < 5; j++)
			{
				switch ((*next)[j][i]) {
				case jump:
					std::cout << "jp ";
					break;
				case jump_and_slide:
					std::cout << "js ";
					break;
				case walk:
					std::cout << "wl ";
					break;
				case slide:
					std::cout << "sl ";
					break;
				case strike:
					std::cout << "sh ";
					break;
				default:
					std::cout << "__ ";
				}
			}
			std::cout << std::endl;
		}
	}
};

//-----------------------------------------------------Figures declarations------------------------------------------------------------------//

class duke : public figure {
public:
	duke(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Duke;
	}

	duke(bool first_player,bool starting_position_) {
		starting_position = starting_position_;
		set_moves();		

		owned_by_first_player = first_player;
		name = Duke;
	}

	void set_moves();

	std::string return_symbol() {
		if (owned_by_first_player) {
			return "D1";
		}
		return "D2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<duke>(owned_by_first_player,starting_position); 
	}
};

class pikeman : public figure {
public:
	pikeman(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Pikeman;
	}

	pikeman(bool first_player,bool starting_position_) {
		starting_position = starting_position_;
		set_moves();
		
		owned_by_first_player = first_player;
		name = Pikeman;
	}

	void set_moves();

	std::string return_symbol() {
		if (owned_by_first_player) {
			return "P1";
		}
		return "P2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<pikeman>(owned_by_first_player,starting_position);
	}
};

class seer : public figure {
public:
	seer(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Seer;
	}

	seer(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();

		owned_by_first_player = first_player;
		name = Seer;
	}

	void set_moves();
	std::string return_symbol() {
		if (owned_by_first_player) {
			return "S1";
		}
		return "S2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<seer>(owned_by_first_player,starting_position); 
	}
};

class ranger : public figure {
public:
	ranger(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Ranger;
	}

	ranger(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();

		owned_by_first_player = first_player;
		name = Ranger;
	}

	void set_moves();
	std::string return_symbol() {
		if (owned_by_first_player) {
			return "R1";
		}
		return "R2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<ranger>(owned_by_first_player, starting_position);
	}

};

class knight : public figure {
public:
	knight(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Knight;
	}

	knight(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();

		owned_by_first_player = first_player;
		name = Knight;
	}
	void set_moves();
	std::string return_symbol() {
		if (owned_by_first_player) {
			return "K1";
		}
		return "K2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<knight>(owned_by_first_player, starting_position);
	}
};

class bowman : public figure {
public:
	bowman(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Bowman;
	}

	bowman(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();

		owned_by_first_player = first_player;
		name = Bowman;
	}

	void set_moves();

	std::string return_symbol() {
		if (owned_by_first_player) {
			return "B1";
		}
		return "B2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<bowman>(owned_by_first_player, starting_position);
	}

};

class dragoon : public figure {
public:
	dragoon(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Dragoon;
	}

	dragoon(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();

		owned_by_first_player = first_player;
		name = Dragoon;
	}

	void set_moves();
	std::string return_symbol() {
		if (owned_by_first_player) {
			return "X1";
		}
		return "X2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<dragoon>(owned_by_first_player, starting_position);
	}

};

class footman : public figure {
public:
	footman(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Footman;
	}

	footman(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();

		owned_by_first_player = first_player;
		name = Footman;
	}

	void set_moves();

	std::string return_symbol() {
		if (owned_by_first_player) {
			return "F1";
		}
		return "F2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<footman>(owned_by_first_player, starting_position);
	}

};

class assassin : public figure {
public:
	assassin(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Assassin;
	}

	assassin(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();

		owned_by_first_player = first_player;
		name = Assassin;
	}

	void set_moves();

	std::string return_symbol() {
		if (owned_by_first_player) {
			return "A1";
		}
		return "A2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<assassin>(owned_by_first_player, starting_position);
	}

};

class priest : public figure {
public:
	priest(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Priest;
	}

	priest(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();

		owned_by_first_player = first_player;
		name = Priest;
	}

	void set_moves();

	std::string return_symbol() {
		if (owned_by_first_player) {
			return "E1";
		}
		return "E2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<priest>(owned_by_first_player, starting_position);
	}
};

class wizard : public figure {
public:
	wizard(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Wizard;
	}

	wizard(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();

		owned_by_first_player = first_player;
		name = Wizard;
	}

	void set_moves();

	std::string return_symbol() {
		if (owned_by_first_player) {
			return "W1";
		}
		return "W2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<wizard>(owned_by_first_player, starting_position);
	}

};


class marshall : public figure {
public:
	marshall(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Marshall;
	}

	marshall(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();

		owned_by_first_player = first_player;
		name = Marshall;
	}

	void set_moves();

	std::string return_symbol() {
		if (owned_by_first_player) {
			return "M1";
		}
		return "M2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<marshall>(owned_by_first_player, starting_position);
	}
};


class champion : public figure {
public:
	champion(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		name = Champion;
	}

	champion(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();

		owned_by_first_player = first_player;
		name = Champion;
	}

	void set_moves();

	std::string return_symbol() {
		if (owned_by_first_player) {
			return "C1";
		}
		return "C2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<champion>(owned_by_first_player, starting_position);
	}
};

class general : public figure {
public:
	general(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		
		name = General;
	}

	general(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();

		owned_by_first_player = first_player;
		name = General;
	}

	void set_moves();

	std::string return_symbol() {
		if (owned_by_first_player) {
			return "G1";
		}
		return "G2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<general>(owned_by_first_player, starting_position);
	}
};

class longbowman : public figure {
public:
	longbowman(bool first_player) {
		set_moves();

		owned_by_first_player = first_player;
		position_on_move_board = coordinates(2, 3);
		name = Longbowman;
	}

	longbowman(bool first_player, bool starting_position_) {
		starting_position = starting_position_;
		set_moves();
		position_on_move_board = coordinates(2, 3);
		owned_by_first_player = first_player;
		name = Longbowman;
	}

	void set_moves();

	std::string return_symbol() {
		if (owned_by_first_player) {
			return "L1";
		}
		return "L2";
	}

	std::unique_ptr<figure> clone() override {
		return std::make_unique<longbowman>(owned_by_first_player, starting_position);
	}
};
