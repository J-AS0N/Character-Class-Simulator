/*
Jason Weng
JASON.WENG76@MYHUNTER.CUNY.EDU
11/12/23
Project 6
This is a test file
*/

#include "Tavern.hpp"
#include "Character.hpp"
#include <iostream>

int main() {
Tavern tavern;

Character c1("MAINCHARACTER", "HUMAN", 1, 1, 1, false);

Character c2("NAAN", "HUMAN", 50, 50, 1, true);

tavern.enterTavern(&c1);
tavern.enterTavern(&c2);
tavern.setMainCharacter(&c1);
tavern.createCombatQueue();
tavern.actionSelection();
c1.getActionQueue();
std::queue<int> action_queue_copy = c1.getActionQueue();
std::cout << action_queue_copy.front();
action_queue_copy.pop();
std::cout << action_queue_copy.front();
action_queue_copy.pop();
std::cout << action_queue_copy.front();
action_queue_copy.pop();


//tavern.combat();





// Call the combat function

}