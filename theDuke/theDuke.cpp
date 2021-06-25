// theDuke.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "figures.h"
#include "game.h"
#include "Logger.h"
#include "Stategy_manager.h"
#include "Tester.h"




int main()
{
	srand(time(NULL));
	/*tester_t tester;
	tester.test_1();
	tester.test_2();
	tester.test_3();
	tester.test_4();
	tester.test_5();
	tester.test_6();
	tester.test_7();
	tester.test_8();*/
	strategy_manager_t mng = strategy_manager_t();
	while (true) {
		mng.evolve();
	}
}

