/*
Jason Weng
JASON.WENG76@MYHUNTER.CUNY.EDU
11/12/23
Project 6
Taven.cpp declares the Tavern class along with its private and public members
*/
#include "Tavern.hpp"
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <random>

/** Default Constructor **/
Tavern::Tavern() : ArrayBag<Character*>(), level_sum_{0}, num_enemies_{0}
{
}



/**
    @param: the name of an input file
    @pre: Formatting of the csv file is as follows:
      Name: An uppercase string
      Race: An uppercase string [HUMAN, ELF, DWARF, LIZARD, UNDEAD]
      Vitality/Armor/Level: A positive integer
      Enemy: 0 (False) or 1 (True)
    
    @post: Each line of the input file corresponds to a Character that enters the Tavern.
  */
    Tavern::Tavern(const std::string& filename)
    {
      std::ifstream fin(filename);
      std::string line;
      std::getline(fin, line);
      std::string name, race, string_vitality, string_armor, string_level, string_enemy;
      int vitality, armor, level;
      bool enemy;
      
      while(std::getline(fin, line))
      {
        std::istringstream iss(line);
        std::getline(iss, name, ',');
        std::getline(iss, race, ',');

        std::getline(iss, string_vitality, ',');
        std::istringstream(string_vitality) >> vitality;

        std::getline(iss, string_armor, ',');
        std::istringstream(string_armor) >> armor;

        std::getline(iss, string_level, ',');
        std::istringstream(string_level) >> level;

        std::getline(iss, string_enemy, ',');
        std::istringstream(string_enemy) >> enemy;
        Character* character = new Character(name, race, vitality, armor, level, enemy);
        enterTavern(character);
      }
    }


/** 
    @param:   A reference to a Character entering the Tavern
    @return:  returns true if a Character was successfully added to items_, false otherwise
    @post:    adds Character to the Tavern and updates the level sum and the enemy count if the character is an enemy.
**/
bool Tavern::enterTavern(Character* a_character)
{
  if(add(a_character))
  {
    level_sum_ += a_character->getLevel();
    if(a_character->isEnemy())
      num_enemies_++;
     
    return true;
  }
  else
  {
    return false;
  }
}

/** @param:   A reference to a Character leaving the Tavern  
    @return:  returns true if a character was successfully removed from items_, false otherwise
    @post:    removes the character from the Tavern and updates the level sum and the enemy count if the character is an enemy.
**/
bool Tavern::exitTavern(Character* a_character)
{
  if(remove(a_character))
  {
    level_sum_ -= a_character->getLevel();
    if(a_character->isEnemy())
      num_enemies_--;
      
    return true;
  }
  return false;
}







/** 
    @return:  The integer level count of all the characters currently in the Tavern
    **/
    int Tavern::getLevelSum()
    {
      return level_sum_;
    }



/** 
    @return:  The average level of all the characters in the Tavern
    @post:    Considers every character currently in the Tavern, updates the average level of the Tavern rounded to the NEAREST integer, and returns the integer value.
**/
int Tavern::calculateAvgLevel()
{
   return (level_sum_>0) ? round(double(level_sum_) / item_count_) : 0.0;

}



/** 
    @return:  The integer enemy count of the Tavern
    **/
    int Tavern::getEnemyCount()
    {
      return num_enemies_;
    }



/** 
    @return:  The percentage (double) of all the enemy characters in the Tavern
    @post:    Considers every character currently in the Tavern, updates the enemy percentage of the Tavern rounded to 2 decimal places, and returns the double value.
**/
double Tavern::calculateEnemyPercentage()
{
  double enemy_percent = (num_enemies_>0) ?  (double(num_enemies_) / item_count_) * 100: 0.0;
  return std::ceil(enemy_percent*100.0) / 100.0; //round up to to decimal places
 
}


/** 
    @param:   A string reference to a race 
    @return:  An integer tally of the number of characters in the Tavern of the given race
**/
int Tavern::tallyRace(const std::string &race)
{
  int frequency = 0;
  int curr_index = 0;   
  while (curr_index < item_count_)
  {
    if (items_[curr_index]->getRace() == race)
    {
      frequency++;
    } 

    curr_index++; 
  }

  return frequency;
}


/**
  @post:    Outputs a report of the characters currently in the tavern in the form:
  "Humans: [x] \nElves: [x] \nDwarves: [x] \nLizards: [x] \nUndead: [x] \n\nThe average level is: [x] \n[x]% are enemies.\n\n"

  Example output: 
  Humans: 5
  Elves: 8
  Dwarves: 3
  Lizards: 7
  Undead: 2

  The average level is: 16
  24% are enemies.
*/
void Tavern::tavernReport()
{
  int humans = tallyRace("HUMAN");
  int elves = tallyRace("ELF");
  int dwarves = tallyRace("DWARF");
  int lizards = tallyRace("LIZARD");
  int undead = tallyRace("UNDEAD");
  
  std::cout << "Humans: " << humans << std::endl;
  std::cout << "Elves: " << elves << std::endl;
  std::cout << "Dwarves: " << dwarves << std::endl;
  std::cout << "Lizards: " << lizards << std::endl;
  std::cout << "Undead: " << undead << std::endl;
  std::cout << "\nThe average level is: " << calculateAvgLevel() << std::endl;
  std::cout << std::fixed << std::setprecision(2) << calculateEnemyPercentage() << "% are enemies.\n\n";
}


/**
    @post: For every character in the tavern, displays each character's information
*/
void Tavern::displayCharacters()
{
  for(int i = 0; i < item_count_; i++)
  {
    items_[i]->display();
  }
}


/**
    @param: a string reference to a race
    @post: For every character in the tavern of the given race, displays each character's information
*/
void Tavern::displayRace(const std::string &race)
{
  for(int i = 0; i < item_count_; i++)
  {
    if(items_[i]->getRace() == race)
    {
      items_[i]->display();
    }
  }
}






/**
    @post: all characters leave the tavern
*/
void Tavern::emptyTavern()
{
  for(int i = 0; i < item_count_; i++)
  {
    delete items_[i];
    items_[i] = nullptr;
  }
  item_count_ = 0;
}

/**
  @param: a Character pointer 
  @post: sets the Tavern's main character to the pointer that is given
*/
void Tavern::setMainCharacter(Character* main) {
  main_character_ = main;  // set the private member variable main_character_ equal to the value of the parameter
}

/**
  @return: a pointer to the main character in the Tavern, nullptr if there is no main character
*/
Character* Tavern::getMainCharacter() {
  if (!main_character_) {  // if there is no main character
    return nullptr;  // return nullptr

  } else {
    return main_character_;  // if there is a main character, return it
  }
}

/**
  @param: two Character pointers
  @return: true if the lhs Character's level is less than the rhs Character's level
*/
bool Tavern::levelIsLess(Character* left, Character* right) {
  if (left -> getLevel() < right -> getLevel()) {  // if the left character's level is less than the right character's level
    return true;
  }
  return false;
}

/**
  @param: two Character pointers
  @return: true if the lhs Character's level is greater than the rhs Character's level
*/
bool Tavern::levelIsGreater(Character* left, Character* right) {
  if (left -> getLevel() > right -> getLevel()) {  // if the left character's level is greater than the right character's level
    return true;
  }
  return false;
}

/**
  @param: two Character pointers
  @return: true if the lhs Character's vitality is less than the rhs Character's vitality
*/
bool Tavern::vitalityIsLess(Character* left, Character* right) {
  if (left -> getVitality() < right -> getVitality()) {  // if the left character's vitality is less than the right character's vitality
    return true;
  }
  return false;
}

/**
  @param: two Character pointers
  @return: true if the lhs Character's vitality is more than the rhs Character's vitality
*/
bool Tavern::vitalityIsGreater(Character* left, Character* right) {
  if (left -> getVitality() > right -> getVitality()) {  // if the left character's vitality is greater than the right character's vitality
    return true;
  }
  return false;
}

/**
  @pre  : The combat queue should be emptied before the characters are added to the queue
  @param  : A string reference to a sorting filter with a default value of "NONE"
  @post   : With default filter "NONE": add every character marked as an enemy to the combat queue
    : With the filter "LVLASC": add every character in the Tavern marked as an enemy to the combat queue, in ascending order based on their level.
    : With the filter "LVLDES": add every character in the Tavern marked as an enemy to the combat queue, in descending order based on their level.
    : With the filter "HPASC": add every character in the Tavern marked as an enemy to the combat queue, in ascending order based on their vitality.
    : With the filter "HPDES": add every character in the Tavern marked as an enemy to the combat queue, in descending order based on their vitality.
*/
void Tavern::createCombatQueue(const std::string& filter) {
  std::vector<Character*> Char_empty;  // empty vector of Characters

  while (!combat_queue_.empty()) {  // while there is characters in the queue
    combat_queue_.pop();  // clears the queue
    }
    for (int i = 0; i < getCurrentSize(); i++) {  // loops through the ArrayBag full of characters
      if (items_[i] -> isEnemy()) {  // if the character is an enemy
        Char_empty.push_back(items_[i]);  // push it back into the empty vector
      }
    }
    
    if (filter == "NONE") {
      for (int i = 0; i < Char_empty.size(); i++) {
        combat_queue_.push(Char_empty[i]);  // adds the character's in the vector into the queue
      }

    } else if (filter == "LVLASC") {  // smallest to largest (level)
      sort(Char_empty.begin(), Char_empty.end(), [](Character* left, Character* right) {
        return left -> getLevel() < right -> getLevel();
    });

      for (int i = 0; i < Char_empty.size(); i++) {  // after it sorts, it adds them to the queue
        combat_queue_.push(Char_empty[i]);
      }

    } else if (filter == "LVLDES") {  // largest to smallest (level)
      sort(Char_empty.begin(), Char_empty.end(), [](Character* left, Character* right) {
        return left -> getLevel() > right -> getLevel();
    });

      for (int i = 0; i < Char_empty.size(); i++) {
        combat_queue_.push(Char_empty[i]);
      }

    } else if (filter == "HPASC") {  // smallest to largest (vitality)
      sort(Char_empty.begin(), Char_empty.end(), [](Character* left, Character* right) {
        return left -> getVitality() < right -> getVitality();
    });

      for (int i = 0; i < Char_empty.size(); i++) {
        combat_queue_.push(Char_empty[i]);
      }

    } else if (filter == "HPDES") {  // largest to smallest (vitality)
      sort(Char_empty.begin(), Char_empty.end(), [](Character* left, Character* right) {
        return left -> getVitality() > right -> getVitality();
    });

      for (int i = 0; i < Char_empty.size(); i++) {
        combat_queue_.push(Char_empty[i]);
      }
    }
}

/**
  @post   : returns a pointer to the Character at the front of the Combat Queue. 
          : If there are no characters in the queue, return nullptr
*/
Character* Tavern::getTarget() {
  Character* temp = combat_queue_.front();  // assigns the first character at the front of the queue to TEMP

  if (combat_queue_.empty()) {
    return nullptr;

  } else {
    return temp;
  }
}

/**
  @post : Prints the details of each character in the combat queue in the following form:
        (ENEMY) [ENEMY NAME]: LEVEL [ENEMY LEVEL] [ENEMY RACE]. \nVITALITY: [ENEMY VITALITY] \nARMOR: [ENEMY ARMOR]\n 
*/
void Tavern::printCombatQueue() {
  std::queue<Character*> combat_queue_copy = combat_queue_;  // creates a copy of combat_queue_
  while (!combat_queue_copy.empty()) {  // while the combat queue is not empty
    std::cout << "(ENEMY) " << combat_queue_copy.front() -> getName() << ": "
    << "LEVEL " << combat_queue_copy.front() -> getLevel() << " " << combat_queue_copy.front() -> getRace() << "."
    << "\nVITALITY: " << combat_queue_copy.front() -> getVitality()
    << "\nARMOR: " << combat_queue_copy.front() -> getArmor() << "\n";

    combat_queue_copy.pop();  // removes the first queue
  }
}
/** THE FOLLOWING FUNCTIONS WILL MANAGE THE COMBAT - YOU SHOULD CONSIDER HOW THESE WILL AFFECT THE CHARACTERS AND WHAT ADDITIONAL MEMBER FUNCTION YOU NEED TO IMPLEMENT IN THE CHARACTER CLASS TO SUPPORT THESE ACTIONS */

// Helper Function - Prints out the different type of actions
void Tavern::actionHelp() {
  int action;
  std::cout << "Choose an action(1-4):"  // asks for user input
  << "\n1: BUFF_Heal"
  << "\t\t2: BUFF_MendMetal"
  << "\t\t3: ATT_Strike"
  << "\t\t4: ATT_ThrowTomato\n";
}

/** 
        @post : If there are no enemies in the combat queue, do nothing.
              : Otherwise do all of the following
              :     - The action queue must be emptied before the new actions are added.
              :     - Print out the start of turn and details of the target enemy in the form:
                      : YOUR TURN\n
                      : (ENEMY) [ENEMY NAME]: LEVEL [ENEMY LEVEL] [ENEMY RACE]. \nVITALITY: [ENEMY VITALITY] \nARMOR: [ENEMY ARMOR]\n\n 
    
              :     - Repeat the following process thrice (i.e. read three actions from input): 
              :           - Print the actions that are available to the main character, and read 
                            a non-negative number from input, corresponding to an action. 
                            Printing the available options should be of the following form. 
                            You may consider writing a helper function for this.
                            :Choose an action(1-4):\n1: BUFF_Heal\t\t2: BUFF_MendMetal\t\t3: ATT_Strike\t\t4: ATT_ThrowTomato\n
              :           - If the input is invalid (valid inputs will be 1,2,3, or 4 only.), 
                          keep prompting for a non-negative number that is within range, by
                          printing "Invalid input. Please enter a number between 1 and 4 inclusive.\n" and prompt for input again.
              :           - When a valid action is read, 
                          it is added to the main character's action queue.    
*/
void Tavern::actionSelection() {
  int action;
  
  if (combat_queue_.empty()) {  // if the combat_queue_ is empty, do nothing
    return;
  }

  main_character_ -> EmptyActionQueue();  // empties the action queue

  std::cout << "YOUR TURN\n";
  std::cout << "(ENEMY) " << combat_queue_.front() -> getName() << ": "
  << "LEVEL " << combat_queue_.front() -> getLevel() << " "
  << combat_queue_.front() -> getRace() << ". "
  << "\nVITALITY: " << combat_queue_.front() -> getVitality()
  << "\nARMOR: " << combat_queue_.front() -> getArmor() << "\n\n";

for (int i = 0; i < 3; i++) {  // loops three times
    actionHelp();  // asks for user input
    std::cin >> action;  // takes user input

    while (action != 1 && action != 2 && action != 3 && action != 4) {  // while the user's input is not equal to 1, 2, 3 or 4
      std::cout << "Invalid input. Please enter a number between 1 and 4 inclusive.\n";
      std::cin >> action;
    }
    main_character_ -> HelpPush(action);  // executes if the action is equal to 1, 2, 3 or 4 : (adds to the main character's action queue)
  } 
}

/**
        @post : If there are no actions in the main character's action queue, 
                or there are no enemies in the combat queue, do nothing.
                
                Otherwise do all of the following:            
                
                1. If the Buff stack is not empty, apply the buff once and decrement the turns.
                   Any time a Buff's turns_ goes to zero, it is removed
                   from the stack. Print out the results of the action as described below.
                2. While there are actions on the action queue:
                - take the next action off the queue
                - if the next action is  a buff, apply once and push it onto the buff stack after 
                  decrementing the turns. 
                  BUFF_Heal lasts for 3 turns in total, while BUFF_MendMetal lasts
                  for 2 turns in total. Print out the results of the action as described below.
                - if the next action is an attack, execute it against the enemy at the front of
                  the combat queue. Print out the results of the action as described below. 
                 - if after this action there are no more enemies to fight, clear the action
                   queue and return.
                3. Print "END OF YOUR TURN\n" 
          
            : More details:
        
            : After applying each action, print out the results of the action as follows:
            : [MAINCHARACTER NAME] used [ACTION NAME]!
            : \n(YOU) [MAINCHARACTER NAME]: LEVEL [MAINCHARACTER LEVEL] [MAINCHARACTER RACE]. \nVITALITY: [MAINCHARACTER VITALITY] \nARMOR: [MAINCHARACTER ARMOR]\n
            : \n(ENEMY) [ENEMY NAME]: LEVEL [ENEMY LEVEL] [ENEMY RACE]. \nVITALITY: [ENEMY VITALITY] \nARMOR: [ENEMY ARMOR]\n 

             Where [ACTION NAME] is one of [Heal, MendMetal, Strike, ThrowTomato]
            
            :   Whenever an enemy's vitality becomes <=0 after an attack, also print out 
                "[CHARACTER NAME] DEFEATED\n" 
                and remove the enemy from the combat queue AND let them exit the tavern. 
                Then carry out the remaining actions, if any, against the next enemy on the
                combat queue.
          
*/
void Tavern::turnResolution() {
  if (main_character_ -> getActionQueue().empty() || combat_queue_.empty()) {  // if there are no actions or enemies in combat queue
    return;
  }

  if (!main_character_ -> getBuffStack().empty()) {  // if the Buff stack is NOT empty
    if (main_character_ -> getBuffStack().top().name_ == "BUFF_Heal") {  // if the first buff (buff at the top)'s name is "BUFF_Heal"
      main_character_ -> heal();  // applies the buff
      main_character_ -> getBuffStack().top().turns_--;  // decrement the turns by 1
      if (main_character_ -> getBuffStack().top().turns_ == 0) {  // if the turns is equal to 0
        main_character_ -> getBuffStack().pop();  // remove the buff from the stack
      }
    } else if (main_character_ -> getBuffStack().top().name_ == "BUFF_MendMetal") {  // if the first buff's name is "BUFF_MendMetal"
      main_character_ -> mendMetal();  // applies the buff
      main_character_ -> getBuffStack().top().turns_--;  // decrement the turns by 1
      if (main_character_ -> getBuffStack().top().turns_ == 0) {   // if the turns_ is equal to 0
        main_character_ -> getBuffStack().pop();  // remove the buff from the stack
      }
    }
  }

  // part 2
  std::string tempString = "";

  while (!main_character_ -> getActionQueue().empty()) {  // while the action queue is not empty
    int action = main_character_ -> getActionQueue().front() - 1;
    if (action == 0) {  // Heal
      main_character_ -> addBuff("BUFF_Heal", 3);  // add the buff that lasts for 3 turns
      main_character_ -> heal();  // applies the buff
      main_character_ -> getBuffStack().top().turns_--;  // decrement the turns by 1
      if (main_character_ -> getBuffStack().top().turns_ == 0) {  // if the turns_ is equal to 0
        main_character_ -> getBuffStack().pop();  // remove it from the stack
      }
      tempString = "Heal";  // set the empty string to "Heal"

    } else if (action == 1) {  // MendMetal
      main_character_ -> addBuff("BUFF_MendMetal", 2);
      main_character_ -> mendMetal();
      main_character_ -> getBuffStack().top().turns_--;
      if (main_character_ -> getBuffStack().top().turns_ == 0) {
        main_character_ -> getBuffStack().pop();
      }

      tempString = "MendMetal";

    } else if (action == 2) {  // Strike
      main_character_ -> strike(getTarget());
      tempString = "Strike";

    } else if (action == 3) {  // ThrowTomato
      main_character_ -> throwTomato(getTarget());
      tempString = "ThrowTomato";
    }

    // YOU
    std::cout << "\n" << main_character_ -> getName() << " used " << tempString << "!";
    std::cout << "\n(YOU) " << main_character_ -> getName() << ":" << " LEVEL " << main_character_ -> getLevel() << " " << main_character_ -> getRace() << ". "
    << "\nVITALITY: " << main_character_ -> getVitality()
    << "\nARMOR: " << main_character_ -> getArmor() << "\n";

    // ENEMY
    std::cout << "\n(ENEMY) " << getTarget() -> getName() << ":" << " LEVEL " << getTarget() -> getLevel() << " " << getTarget() -> getRace() << ". "
    << "\nVITALITY: " << getTarget() -> getVitality()
    << "\nARMOR: " << getTarget() -> getArmor() << "\n";

    if (getTarget() -> getVitality() <= 0) {  // if the enemy's vitality is less than or equal to 0
      std::cout << getTarget() -> getName() << " DEFEATED\n";  // prints out their name and "DEFEATED"
      exitTavern(getTarget());  // let them exit the tavern
      combat_queue_.pop();  // removes the enemy from the combat queue
    }
    if (combat_queue_.empty()) {
      main_character_ -> EmptyActionQueue();
      return;
    }
    main_character_ -> actionPop();  // pops off a action in the action_queue_
  }
  std::cout << "\nEND OF YOUR TURN\n\n";
}
 
/**
        @param  : A pointer to the enemy
        @post   : Do all of the following:
                : 1. If the enemy's buff stack is not empty, apply buff once and decrement the
                  turns. Any time a Buff's turns_ goes to zero, it is removed
                  from the stack. Print out the results of the action as described below.
                : 2. Pick a random Action 
                    (one of BUFF_Heal, BUFF_MendMetal, ATT_Strike, ATT_ThrowTomato). 
                : If the selected action is a buff, apply once immediately and add it to the 
                  enemy's buff stack, performing the necessary turns update. 
                  Print out the results of the action as described below.
                : If the selected action is an attack, execute it against the main character.
                  Print out the results of the action as described below. 
                : More details:
                
                : After applying each action, print out the results of the action as follows:
                : [ENEMY NAME] used [ACTION NAME]!
                : \n(ENEMY) [ENEMY NAME]: LEVEL [ENEMY LEVEL] [ENEMY RACE]. \nVITALITY: [ENEMY VITALITY] \nARMOR: [ENEMY ARMOR]\n 
                : \n(YOU) [MAINCHARACTER NAME]: LEVEL [MAINCHARACTER LEVEL] [MAINCHARACTER RACE]. \nVITALITY: [MAINCHARACTER VITALITY] \nARMOR: [MAINCHARACTER ARMOR]\n
*/
void Tavern::enemyTurn(Character* enemy) {
  if (!combat_queue_.empty()) {  // if the combat_queue_ is not empty
    if (!enemy -> getBuffStack().empty()) {  // if the enemy's buffstack is not empty
      if (enemy -> getBuffStack().top().name_ == "BUFF_Heal") {  // if the name of the buff on the top of the stack is "BUFF_Heal"
        enemy -> heal();  // use the buff
        enemy -> getBuffStack().top().turns_--;  // decrement the turns by 1
        if (enemy -> getBuffStack().top().turns_ == 0) {  // if the enemy's turn is equal to 0
          enemy -> getBuffStack().pop();  // remove the buff from the stack
        }
      }
      else if (enemy -> getBuffStack().top().name_ == "BUFF_MendMetal") {
        enemy -> mendMetal();
        enemy -> getBuffStack().top().turns_--;
        if (enemy -> getBuffStack().top().turns_ == 0) {
          enemy -> getBuffStack().pop();
        }
      }
    }

  int randNum = rand() % 4;  // random number from 0 to 3
  std::string tempStr = "";

  if (randNum == 0) {  // if the random number is equal to 0
    enemy -> heal();  // use the buff HEAL
    enemy -> getBuffStack().push(Buff{"BUFF_Heal", 2});  //  pushes the buff into the stack
    tempStr = "Heal";

  } else if (randNum == 1) {
    enemy -> mendMetal();
    enemy -> getBuffStack().push(Buff{"BUFF_MendMetal", 1});
    tempStr = "MendMetal";

  } else if (randNum == 2) {
    enemy -> strike(main_character_);
    tempStr = "Strike";

  } else if (randNum == 3) {
    enemy -> throwTomato(main_character_);
    tempStr = "ThrowTomato";
  }

  // ENEMY
  std::cout << "ENEMY TURN\n";
  std::cout << enemy -> getName() << " used " << tempStr << "!"
  << "\n(ENEMY) " << enemy -> getName() << ": " << "LEVEL " << enemy -> getLevel() << " "
  << enemy -> getRace() << ". "
  << "\nVITALITY: " << enemy -> getVitality()
  << "\nARMOR: " << enemy -> getArmor() << "\n";

  
  // YOU
  std::cout << "\n(YOU) " << main_character_ -> getName() << ": "
  << "LEVEL " << main_character_ -> getLevel() << " " << main_character_ -> getRace() << ". "
  << "\nVITALITY: " << main_character_ -> getVitality()
  << "\nARMOR: " << main_character_ -> getArmor() << "\n";
  }
  std::cout << "END OF ENEMY TURN\n\n";
}

/**
        @post : While the main character's vitality is > 0, and there are still enemies 
                  in the combat queue, do the following:
              :     - Call actionSelection()
                    - Call turnResolution()
                    - Call enemyTurn() with a pointer to the first enemy on combat queue.

              : If the main character's vitality has fallen to 0, print "YOU HAVE PERISHED."
              : If there are no more enemies in the combat queue, print "NO MORE ENEMIES."
*/
void Tavern::combat() {
  // Character* first_enemy = combat_queue_.front();  // pointer to the first enemy in the combat queue

  while (main_character_ -> getVitality() > 0 && !combat_queue_.empty()) {  // while the main character's vitality is greater than 0 AND the combat queue is NOT empty
    actionSelection();  // function call
    turnResolution();  // function call
    if (!combat_queue_.empty()){  // if the combat queue is not empty
      enemyTurn(combat_queue_.front());  // function call to enemyTurn passing in the first enemy in the combat queue
    }
  }

  if (main_character_ -> getVitality() <= 0) {  // if the main character's vitality is less than or equal to 0
    std::cout << "YOU HAVE PERISHED.\n";

  }
  if (combat_queue_.empty()) {  // if the combat queue is empty (no more enemies)
    std::cout << "NO MORE ENEMIES.";
  }
}
