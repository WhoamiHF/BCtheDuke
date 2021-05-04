#pragma once
#include <vector>

enum operation { add_it, move_it, command_it };
enum types_of_moves { nothing, walk, slide, jump_and_slide, strike, jump, command, walk_or_command };
enum troop_name { Assassin, Bowman, Dragoon, Duke, Footman, General, Champion, Knight, Longbowman, Marshall, Nothing, Pikeman, Priest, Ranger, Seer, Wizard };

class coordinates {
public:
	coordinates() = default;
	coordinates(int _x, int _y) { x = _x; y = _y; }
	int x;
	int y;
};

class move_t {
public:
	move_t() {
		coords = std::vector<coordinates>();
		op = add_it;
	}
	move_t(std::vector<coordinates> coords_, operation operation_) {
		coords = coords_;
		op = operation_;
	}
	std::vector<coordinates> coords;
	operation op;
};