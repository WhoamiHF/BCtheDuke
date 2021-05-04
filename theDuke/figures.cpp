#include "figures.h"
#include <iostream>

void duke::set_moves() {
	starting_moves[1][2] = slide;
	starting_moves[3][2] = slide;

	other_moves[2][1] = slide;
	other_moves[2][3] = slide;
}

void pikeman::set_moves() {
	starting_moves[0][0] = walk;
	starting_moves[1][1] = walk;
	starting_moves[3][1] = walk;
	starting_moves[4][0] = walk;

	other_moves[1][0] = strike;
	other_moves[3][0] = strike;
	other_moves[2][1] = walk;
	other_moves[2][3] = walk;
	other_moves[2][4] = walk;
}

void seer::set_moves() {
	starting_moves[2][0] = jump;
	starting_moves[0][2] = jump;
	starting_moves[4][2] = jump;
	starting_moves[2][4] = jump;

	starting_moves[1][1] = walk;
	starting_moves[1][3] = walk;
	starting_moves[3][1] = walk;
	starting_moves[3][3] = walk;

	other_moves[0][0] = jump;
	other_moves[4][0] = jump;
	other_moves[0][4] = jump;
	other_moves[4][4] = jump;

	other_moves[2][1] = walk;
	other_moves[1][2] = walk;
	other_moves[3][2] = walk;
	other_moves[2][3] = walk;
}

void ranger::set_moves() {
	starting_moves[2][1] = slide;
	starting_moves[2][3] = slide;

	starting_moves[0][1] = jump;
	starting_moves[1][0] = jump;
	starting_moves[3][0] = jump;
	starting_moves[4][1] = jump;

	other_moves[1][1] = slide;
	other_moves[3][1] = slide;

	other_moves[1][4] = jump;
	other_moves[3][4] = jump;
}

void knight::set_moves() {
	starting_moves[1][0] = jump;
	starting_moves[3][0] = jump;

	starting_moves[1][2] = walk;
	starting_moves[3][2] = walk;
	starting_moves[2][3] = walk;
	starting_moves[2][4] = walk;

	other_moves[2][1] = slide;

	other_moves[1][3] = walk;
	other_moves[0][4] = walk;
	other_moves[3][3] = walk;
	other_moves[4][4] = walk;
}

void bowman::set_moves() {
	starting_moves[2][1] = walk;
	starting_moves[1][2] = walk;
	starting_moves[3][2] = walk;

	starting_moves[0][2] = jump;
	starting_moves[4][2] = jump;
	starting_moves[2][4] = jump;

	other_moves[1][1] = strike;
	other_moves[3][1] = strike;
	other_moves[2][0] = strike;

	other_moves[1][3] = walk;
	other_moves[2][1] = walk;
	other_moves[3][3] = walk;
}

void dragoon::set_moves() {
	starting_moves[0][0] = strike;
	starting_moves[2][0] = strike;
	starting_moves[4][0] = strike;

	starting_moves[1][2] = walk;
	starting_moves[3][2] = walk;

	other_moves[1][3] = slide;
	other_moves[3][3] = slide;

	other_moves[1][0] = jump;
	other_moves[3][0] = jump;

	other_moves[2][0] = walk;
	other_moves[2][1] = walk;
}

void footman::set_moves() {
	other_moves[2][0] = walk;
	other_moves[1][1] = walk;
	other_moves[3][1] = walk;
	other_moves[1][3] = walk;
	other_moves[3][3] = walk;

	starting_moves[2][1] = walk;
	starting_moves[1][2] = walk;
	starting_moves[2][3] = walk;
	starting_moves[3][2] = walk;
}

void assassin::set_moves() {
	starting_moves[2][0] = jump_and_slide;
	starting_moves[0][4] = jump_and_slide;
	starting_moves[4][4] = jump_and_slide;

	other_moves[0][0] = jump_and_slide;
	other_moves[4][0] = jump_and_slide;
	other_moves[2][4] = jump_and_slide;
}

void priest::set_moves() {
	starting_moves[1][1] = slide;
	starting_moves[3][1] = slide;
	starting_moves[1][3] = slide;
	starting_moves[3][3] = slide;

	other_moves[1][1] = walk;
	other_moves[3][1] = walk;
	other_moves[1][3] = walk;
	other_moves[3][3] = walk;

	other_moves[0][0] = jump;
	other_moves[4][0] = jump;
	other_moves[0][4] = jump;
	other_moves[4][4] = jump;
}

void wizard::set_moves() {
	starting_moves[1][1] = walk;
	starting_moves[2][1] = walk;
	starting_moves[3][1] = walk;
	starting_moves[1][2] = walk;
	starting_moves[1][3] = walk;
	starting_moves[2][3] = walk;
	starting_moves[3][3] = walk;
	starting_moves[3][2] = walk;

	other_moves[0][0] = jump;
	other_moves[2][0] = jump;
	other_moves[4][0] = jump;
	other_moves[0][2] = jump;
	other_moves[0][4] = jump;
	other_moves[2][4] = jump;
	other_moves[4][4] = jump;
	other_moves[4][2] = jump;
}

void marshall::set_moves() {
	starting_moves[0][0] = jump;
	starting_moves[4][0] = jump;
	starting_moves[2][4] = jump;

	starting_moves[1][2] = slide;
	starting_moves[3][2] = slide;

	other_moves[1][1] = walk_or_command;
	other_moves[2][1] = walk_or_command;
	other_moves[3][1] = walk_or_command;

	other_moves[1][2] = walk;
	other_moves[1][3] = walk;
	other_moves[0][2] = walk;
	other_moves[3][3] = walk;
	other_moves[3][2] = walk;
	other_moves[4][2] = walk;

	command_squares_other.push_back(coordinates(1, 1));
	command_squares_other.push_back(coordinates(2, 1));
	command_squares_other.push_back(coordinates(3, 1));
}

void champion::set_moves() {
	starting_moves[2][1] = walk;
	starting_moves[1][2] = walk;
	starting_moves[2][3] = walk;
	starting_moves[3][2] = walk;

	starting_moves[2][0] = jump;
	starting_moves[0][2] = jump;
	starting_moves[4][2] = jump;
	starting_moves[2][4] = jump;

	other_moves[2][0] = jump;
	other_moves[2][4] = jump;
	other_moves[4][2] = jump;
	other_moves[0][2] = jump;

	other_moves[1][2] = strike;
	other_moves[2][1] = strike;
	other_moves[3][2] = strike;
	other_moves[2][3] = strike;
}

void general::set_moves() {
	starting_moves[2][1] = walk;
	starting_moves[0][2] = walk;
	starting_moves[3][2] = walk;
	starting_moves[2][3] = walk;

	starting_moves[1][0] = jump;
	starting_moves[3][0] = jump;

	other_moves[1][0] = jump;
	other_moves[3][0] = jump;

	other_moves[2][1] = walk;
	other_moves[0][2] = walk;
	other_moves[4][2] = walk;

	other_moves[1][2] = walk_or_command;
	other_moves[3][2] = walk_or_command;

	other_moves[1][3] = command;
	other_moves[2][3] = command;
	other_moves[3][3] = command;

	command_squares_other.push_back(coordinates(1, 2));
	command_squares_other.push_back(coordinates(1, 3));
	command_squares_other.push_back(coordinates(2, 3));
	command_squares_other.push_back(coordinates(3, 3));
	command_squares_other.push_back(coordinates(3, 2));
}

void longbowman::set_moves() {
	starting_moves[2][2] = walk;
	starting_moves[1][3] = walk;
	starting_moves[2][4] = walk;
	starting_moves[3][3] = walk;

	other_moves[2][0] = strike;
	other_moves[2][1] = strike;

	other_moves[1][4] = walk;
	other_moves[3][4] = walk;
}