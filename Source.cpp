#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <iomanip>
#include <thread>
#include <chrono>
#include <conio.h>
#include <cstdlib>
#include <time.h>
#include <vector>

using namespace std;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

//Default Class Stats
//Max HP							//Physical Damage							//Magic Damage								//Armor								//Magic Resist							//Dodge Chance								//Crit Chance								
const int WARRIOR_MAX_HP = 310;		const int WARRIOR_PHYSICAL_DAMAGE = 70;		const int WARRIOR_MAGIC_DAMAGE = 0;			const int WARRIOR_ARMOR = 20;		const int WARRIOR_MAGICRES = 10;		const float WARRIOR_DODGE_CHANCE = 0.2;		const float WARRIOR_CRIT_CHANCE = 0.1;
					
const int MAGE_MAX_HP = 240;		const int MAGE_PHYSICAL_DAMAGE = 0;			const int MAGE_MAGIC_DAMAGE = 70;			const int MAGE_ARMOR = 3;			const int MAGE_MAGICRES = 10;			const float MAGE_DODGE_CHANCE = 0.1;		const float MAGE_CRIT_CHANCE = 0.1;

const int ROGUE_MAX_HP = 180;		const int ROGUE_PHYSICAL_DAMAGE = 65;		const int ROGUE_MAGIC_DAMAGE = 0;			const int ROGUE_ARMOR = 5;			const int ROGUE_MAGICRES = 5;			const float ROGUE_DODGE_CHANCE = 0.35;		const float ROGUE_CRIT_CHANCE = 0.5;

const int TANK_MAX_HP = 500;		const int TANK_PHYSICAL_DAMAGE = 60;		const int TANK_MAGIC_DAMAGE = 40;			const int TANK_ARMOR = 50;			const int TANK_MAGICRES = 50;			const float TANK_DODGE_CHANCE = 0;			const float TANK_CRIT_CHANCE = 0.2;

const int PSYCHIC_MAX_HP = 260;		const int PSYCHIC_PHYSICAL_DAMAGE = 70;		const int PSYCHIC_MAGIC_DAMAGE = 70;		const int PSYCHIC_ARMOR = 10;		const int PSYCHIC_MAGICRES = 10;		const float PSYCHIC_DODGE_CHANCE = 0.2;		const float PSYCHIC_CRIT_CHANCE = 0.25;

//Miscellaneous constants.
const int POTION_HEAL = 100;
const int TANK_RESBREAK_HEAL = 25, TANK_REGENRES_HPCOST = 150, TANK_THROWROCK_DAMAGE = 80, TANK_THROWROCK_HPCOST = 50, TANK_MAX_REFLECT = 80, TANK_BODYSLAM_DAMAGE = TANK_PHYSICAL_DAMAGE;
const int MAGE_MAX_MANA = 500, MAGE_STARTING_MANA = 250, MAGE_FIREBALL_DAMAGE = 50, MAGE_FIREBALL_MANACOST = 150, MAGE_SHOOTENERGY_MANACOST = 50, MAGE_UNLEASHENERGY_MANACOST = 500, MAGE_UNLEASHENERGY_DAMAGE = 300, MAGE_MANAGAIN = 100, MAGE_SHIELD_AMOUNT = 50, MAGE_MANAPERTURN = 50;


//Function Prototypes 1
void playIntro();
void slowPrint(const string&, int = 8);				//Typewritting effect for standard output.
void WaitEnter(string text);							//Waits until user enters something.

class Hero {

private:
	string name, heroClass, lastAction;
	//Hero main stats.
	int health, physical_dmg, magic_dmg, armor, magic_res, mana;
	float dodge_chance, crit_chance;

	//Hero ablities.
	bool charge = 0, fireball = 0, hiddenblade = 0, reflect = 0, throwrock = 0, regenres = 0;
	bool charged = 0, fireballed = 0, hiddenbladded = 0, reflected = 0, regenedres = 0; //Determines whether an ability was used this battle or not.
	int handgunAmmo = 2; //For psychic's "/fire_handgun".
	int numofpotions = 1; //For warriors.
	bool isturn = 0; //For turn cycling.
	bool isrevealed = 0; //For ambigious battle results feature.

	//Hero situational stats.
	int damageOverTime = 0;
	bool ispoisoned = 0, isburning = 0, isshielded = 0;
	int turnsPoisoned = 0, turnsBurned = 0;

public:
	//Setters and getters.
	int getHealth() { return health; };							void setHealth(int health) { this->health = health; };
	int getPhysicalDmg() { return physical_dmg; };				void setPhysicalDmg(int physical_dmg) { this->physical_dmg = physical_dmg; };
	int getMagicDmg() { return magic_dmg; };					void setMagicDmg(int magic_dmg) { this->magic_dmg = magic_dmg; };
	int getArmor() { return armor; };							void setArmor(int armor) { this->armor = armor; };
	int getMagicRes() { return magic_res; };					void setMagicRes(int magic_res) { this->magic_res = magic_res; };
	float getDodgeChance() { return dodge_chance; };			void setDodgeChance(float dodge_chance) { this->dodge_chance = dodge_chance; };
	float getCritChance() { return crit_chance; };				void setCritChance(float crit_chance) { this->crit_chance = crit_chance; };
	int getMana() { return mana; }								void setMana(int mana) { this->mana = mana; }
	bool canCharge() { return charge; };						void allowCharge(bool charge) { this->charge = charge; };
	bool canFireball() { return fireball; };					void allowFireball(bool fireball) { this->fireball = fireball; };
	bool canHiddenBlade() { return hiddenblade; };				void allowHiddenBlade(bool hiddenblade) { this->hiddenblade = hiddenblade; };
	bool canReflect() { return reflect; };						void allowReflect(bool reflect) { this->reflect = reflect; };
	bool canThrowRock() { return throwrock; }					void allowThrowRock(bool throwrock) { this->throwrock = throwrock; }
	bool canRegenRes() { return regenres; }						void allowRegenRes(bool regenres) { this->regenres = regenres; }
	bool isTurn() { return isturn; }							void takeTurn(bool isturn) { this->isturn = isturn; }
	bool isPoisoned() { return ispoisoned; }					void setPoison(bool ispoisoned) { this->ispoisoned = ispoisoned; }
	bool isBurning() { return isburning; }						void setBurn(bool isburning) { this->isburning = isburning; }
	bool isShielded() { return isshielded; }					void setShielded(bool isshielded) { this->isshielded = isshielded; }
	int getTurnsPoisoned() { return turnsPoisoned; }			void setTurnsPoisoned(int turnsPoisoned) { this->turnsPoisoned = turnsPoisoned; }
	int getTurnsBurned() { return turnsBurned; }				void setTurnsBurned(int turnsBurned) { this->turnsBurned = turnsBurned; }
	int getHandgunAmmo() { return handgunAmmo; }				void setHandgunAmmo(int handgunAmmo) { this->handgunAmmo = handgunAmmo; }
	int getNumOfPotions() { return numofpotions; }				void setNumOfPotions(int numofpotions) { this->numofpotions = numofpotions; }
	bool didCharge() { return charged; }						void setdidCharge(bool charged) { this->charged = charged; }
	bool didFireball() { return fireballed; }					void setdidFireball(bool fireballed) { this->fireballed= fireballed; }
	bool didHiddenBlade() { return hiddenbladded; }				void setdidHiddenBlade(bool hiddenbladded) { this->hiddenbladded= hiddenbladded; }
	bool didReflect() { return reflected; }						void setdidReflect(bool reflected) { this->reflected= reflected; }
	bool didRegenRes() { return regenedres; }					void setdidRegenRes(bool regendres) { this->regenedres = regenedres; }
	string getName() { return name; }							void setName(string name) { this->name = name; }
	string getClass() { return heroClass; }						void setClass(string heroClass) { this->heroClass = heroClass; }
	string getLastAction() { return lastAction; }				void setLastAction(string lastAction) { this->lastAction = lastAction; }
	int getDamageOverTime() { return damageOverTime; }			void setDamageOverTime(int damageOverTime) { this->damageOverTime = damageOverTime; }
	bool isRevealed() { return isrevealed; }					void setRevealed(bool isrevealed) { this->isrevealed = isrevealed; }


	//Print all stats.
	void printStats()
	{
		sleep_for(milliseconds(50));
		cout << "\n" << "*" << getName() << "*" << "\n";
		sleep_for(milliseconds(50));
		cout << "Class: " << getClass() << endl;
		sleep_for(milliseconds(50));
		cout << "Health: " << getHealth() << endl;
		if (getMana() != 0)
		{
			cout << "Mana: " << getMana() << endl;
			sleep_for(milliseconds(50));
		}
		sleep_for(milliseconds(50));
		cout << "Physical Damage: " << getPhysicalDmg() << endl;
		sleep_for(milliseconds(50));
		cout << "Magic Damage: " << getMagicDmg() << endl;
		sleep_for(milliseconds(50));
		cout << "Armor: " << getArmor() << endl;
		sleep_for(milliseconds(50));
		cout << "Magic Resistance: " << getMagicRes() << endl;
		sleep_for(milliseconds(50));
		cout << "Dodge Chance: " << round(100 * getDodgeChance()) << "%" << endl;
		sleep_for(milliseconds(50));
		cout << "Crit Chance: " << round(100 * getCritChance()) << "%" << endl;
		sleep_for(milliseconds(50));
	}

	//Set all stats.
	void setAllStats(int hp, int ad, double ap, int armor, int magicres, float dodge, float crit, int mana = 0)
	{
		setHealth(hp); setPhysicalDmg(ad); setMagicDmg(ap); setArmor(armor); setMagicRes(magicres); setDodgeChance(dodge); setCritChance(crit); setMana(mana);
	}

	//Constructor
	Hero(string name, string heroClass)
	{
		this->name = name;
		this->heroClass = heroClass;
		if (this->heroClass == "6" || this->heroClass == "(6)" || this->heroClass == "Random" || this->heroClass == "random")
		{
			srand(time(0));
			setClass(to_string((rand() % 5) + 1));
		}
		if (this->heroClass == "1" || this->heroClass == "(1)" || this->heroClass == "Warrior" || this->heroClass == "warrior")
		{
			setClass("warrior");
			allowCharge(true);
			setAllStats(WARRIOR_MAX_HP, WARRIOR_PHYSICAL_DAMAGE, WARRIOR_MAGIC_DAMAGE, WARRIOR_ARMOR, WARRIOR_MAGICRES, WARRIOR_DODGE_CHANCE, WARRIOR_CRIT_CHANCE);
		}
		else if (this->heroClass == "2" || this->heroClass == "(2)" || this->heroClass == "Mage" || this->heroClass == "mage")
		{
			setClass("mage");
			allowFireball(true);
			setAllStats(MAGE_MAX_HP, MAGE_PHYSICAL_DAMAGE, MAGE_MAGIC_DAMAGE, MAGE_ARMOR, MAGE_MAGICRES, MAGE_DODGE_CHANCE, MAGE_CRIT_CHANCE, MAGE_STARTING_MANA);
		}
		else if (this->heroClass == "3" || this->heroClass == "(3)" || this->heroClass == "Rogue" || this->heroClass == "rogue")
		{
			setClass("rogue");
			allowHiddenBlade(true);
			setAllStats(ROGUE_MAX_HP, ROGUE_PHYSICAL_DAMAGE, ROGUE_MAGIC_DAMAGE, ROGUE_ARMOR, ROGUE_MAGICRES, ROGUE_DODGE_CHANCE, ROGUE_CRIT_CHANCE);
		}
		else if (this->heroClass == "4" || this->heroClass == "(4)" || this->heroClass == "Tank" || this->heroClass == "tank")
		{
			setClass("tank");
			allowReflect(true);
			setdidRegenRes(false);
			setAllStats(TANK_MAX_HP, TANK_PHYSICAL_DAMAGE, TANK_MAGIC_DAMAGE, TANK_ARMOR, TANK_MAGICRES, TANK_DODGE_CHANCE, TANK_CRIT_CHANCE);
		}
		else if (this->heroClass == "5" || this->heroClass == "(5)" || this->heroClass == "Psychic" || this->heroClass == "psychic")
		{
			setClass("psychic");
			setAllStats(PSYCHIC_MAX_HP, PSYCHIC_PHYSICAL_DAMAGE, PSYCHIC_MAGIC_DAMAGE, PSYCHIC_ARMOR, PSYCHIC_MAGICRES, PSYCHIC_DODGE_CHANCE, PSYCHIC_CRIT_CHANCE);
		}
	}

};

class Battle {

private:
	int turn = 1;
	string lastresult;

public:
	//Setters and getters.
	int getTurn() { return turn; }						void setTurn(int turn) { this->turn = turn; }
	string getLastResult() { return lastresult; }		void setLastResult(string lastresult) { this->lastresult = lastresult; }
	
	vector <string>resultLog = { "NULL(first turn)" };

	void takeTurn(Hero& attacker, Hero& defender)
	{
		//Turn cycling.
		attacker.takeTurn(false);
		defender.takeTurn(true);

		//RNG elements + things that happen every turn.
		bool crit = false, dodge = false;
		srand(time(NULL));
		if (((rand() % 100) + 1) <= (100 * attacker.getCritChance()))
			crit = true;
		sleep_for(milliseconds(1005));
		srand(time(NULL));
		if (((rand() % 100) + 1) <= (100 * defender.getDodgeChance()))
			dodge = true;
		int damageDealt;
		if (crit == true) attacker.setRevealed(true);

		//Turn start.
		if (attacker.isPoisoned() == false && attacker.isBurning() == false) slowPrint("Turn " + to_string(getTurn()) + " - " + attacker.getName() + " - " + to_string(attacker.getHealth()) + " HP\n" + defender.getName() + " has " + to_string(defender.getHealth()) + " HP.\n");
		else if (attacker.isPoisoned() == true)
		{
			attacker.setTurnsPoisoned(attacker.getTurnsPoisoned() + 1);
			if (float(float(attacker.getHealth()) / 10.0) < 100.0 && float(float(attacker.getHealth()) / 10.0) >= 10.0)
			{
				slowPrint("Turn " + to_string(getTurn()) + " - " + attacker.getName() + " - " + to_string(attacker.getHealth()) + "\b\b\b");
				sleep_for(milliseconds(500));
				attacker.setHealth(attacker.getHealth() - defender.getDamageOverTime());
				if (attacker.getHealth() < 0) attacker.setHealth(0);
				slowPrint(to_string(attacker.getHealth()) + " HP - Poisoned\n" + defender.getName() + " has " + to_string(defender.getHealth()) + " HP\n");
			}
			else if (float(float(attacker.getHealth()) / 10.0) < 10.0 && float(float(attacker.getHealth()) / 10.0) >= 1.0) 
			{
				slowPrint("Turn " + to_string(getTurn()) + " - " + attacker.getName() + " - " + to_string(attacker.getHealth()) + "\b\b");
				sleep_for(milliseconds(500));
				attacker.setHealth(attacker.getHealth() - defender.getDamageOverTime());
				if (attacker.getHealth() < 0) attacker.setHealth(0);
				slowPrint(to_string(attacker.getHealth()) + " HP - Poisoned\n" + defender.getName() + " has " + to_string(defender.getHealth()) + " HP\n");
			}
			else if (float(float(attacker.getHealth()) / 10.0) < 1) 
			{
				slowPrint("Turn " + to_string(getTurn()) + " - " + attacker.getName() + " - " + to_string(attacker.getHealth()) + "\b");
				sleep_for(milliseconds(500));
				attacker.setHealth(attacker.getHealth() - defender.getDamageOverTime());
				if (attacker.getHealth() < 0) attacker.setHealth(0);
				slowPrint(to_string(attacker.getHealth()) + " HP - Poisoned\n" + defender.getName() + " has " + to_string(defender.getHealth()) + " HP\n");
			}
			if (attacker.getHealth() == 0)
			{
				sleep_for(milliseconds(1000));
				slowPrint("\nYou died from poison...\n\n", 150);
				sleep_for(milliseconds(2000));
			}
		}
		else if (attacker.isBurning() == true)
		{
			attacker.setTurnsBurned(attacker.getTurnsBurned() + 1);
			if (float(float(attacker.getHealth()) / 10.0) < 100.0 && float(float(attacker.getHealth()) / 10.0) >= 10.0)
			{
				slowPrint("Turn " + to_string(getTurn()) + " - " + attacker.getName() + " - " + to_string(attacker.getHealth()) + "\b\b\b");
				sleep_for(milliseconds(500));
				attacker.setHealth(attacker.getHealth() - defender.getDamageOverTime());
				if (attacker.getHealth() < 0) attacker.setHealth(0);
				slowPrint(to_string(attacker.getHealth()) + " HP - Burning\n" + defender.getName() + " has " + to_string(defender.getHealth()) + " HP\n");
			}
			else if (float(float(attacker.getHealth()) / 10.0) < 10.0 && float(float(attacker.getHealth()) / 10.0) >= 1.0)
			{
				slowPrint("Turn " + to_string(getTurn()) + " - " + attacker.getName() + " - " + to_string(attacker.getHealth()) + "\b\b");
				sleep_for(milliseconds(500));
				attacker.setHealth(attacker.getHealth() - defender.getDamageOverTime());
				if (attacker.getHealth() < 0) attacker.setHealth(0);
				slowPrint(to_string(attacker.getHealth()) + " HP - Burning\n" + defender.getName() + " has " + to_string(defender.getHealth()) + " HP\n");
			}
			else if (float(float(attacker.getHealth()) / 10.0) < 1)
			{
				slowPrint("Turn " + to_string(getTurn()) + " - " + attacker.getName() + " - " + to_string(attacker.getHealth()) + "\b");
				sleep_for(milliseconds(500));
				attacker.setHealth(attacker.getHealth() - defender.getDamageOverTime());
				if (attacker.getHealth() < 0) attacker.setHealth(0);
				slowPrint(to_string(attacker.getHealth()) + " HP - Burning\n" + defender.getName() + " has " + to_string(defender.getHealth()) + " HP\n");
			}
			if (attacker.getHealth() == 0)
			{
				sleep_for(milliseconds(1000));
				slowPrint("\nYou burned to death...\n\n", 150);
				sleep_for(milliseconds(2000));
			}
		}
		
		if (attacker.getHealth() != 0)
		{
			sleep_for(milliseconds(500));
			string action;
			slowPrint("\nAvailable actions:\n\n");
			sleep_for(milliseconds(500));

			//Player actions based on their class.
			if (attacker.getClass() == "warrior")
			{
				if (attacker.getHealth() <= round((float)WARRIOR_MAX_HP * 0.3) && attacker.didCharge() == false) attacker.allowCharge(true);
				else attacker.allowCharge(false);
				//Available actions.
				{
					cout << "/sword_attack\t(20-60 physical damage)\n";
					sleep_for(milliseconds(50));
					if (attacker.getNumOfPotions() > 0 && attacker.getHealth() < WARRIOR_MAX_HP)
					{
						cout << "/use_potion\theals "+ to_string(POTION_HEAL) + " HP, and removes poison and burn effects.\n";
						sleep_for(milliseconds(50));
					}
					cout << "/hunker_down\tincrease dodge chance\n";
					sleep_for(milliseconds(50));
					cout << "/stats\tview your stats\n";
					sleep_for(milliseconds(50));
					cout << "/battle_log\n";
					sleep_for(milliseconds(50));
					cout << "/help\n";
					sleep_for(milliseconds(50));
					if (attacker.canCharge() == true) cout << "\n/chaarge!\tYou are below 30% HP, charge at the enemy to deal extra damage, and gain an take the next turn.\n";
				}

				//Input validation, and non-turn-endning actions for warrior.
				while (1)
				{
					slowPrint("\nWhat would you like to do, " + attacker.getName() + "?\n");
					cin >> action;
					if (action == "/stats")
					{
						attacker.printStats();
					}
					else if (action == "/battle_log")
					{
						cout << "**Battle log**\n\n";
						for (int i = 1; i < getTurn(); i++)
						{
							cout << resultLog[i] << endl;
						}
					}
					else if (action == "/help")
					{
						slowPrint("Available actions:\n\n");
						sleep_for(milliseconds(200));
						cout << "/sword_attack\t(20-60 physical damage)\n/hunker_down\n";
						if (attacker.getNumOfPotions() > 0) cout << "/use_potion\n";
						cout << "/stats\n/battle_log\n";
						if (attacker.canCharge() == true) cout << "\n/chaarge!";
					}
					else if (action == "/use_potion" && ((attacker.getNumOfPotions() <= 0) || attacker.getHealth() == WARRIOR_MAX_HP))
					{
						if (attacker.getNumOfPotions() <= 0)slowPrint("You're out of potions.\n");
						else if (attacker.getHealth() == WARRIOR_MAX_HP)slowPrint("You're at full health.\n");
						else slowPrint("You're at full HP, and you're out of potions.\n");
						sleep_for(milliseconds(100));
					}
					else if (action == "/chaarge!" && attacker.canCharge() == false)
					{
						if (attacker.didCharge() == false) slowPrint("You're not angry enough yet.\n");
						else if (attacker.didCharge() == true) slowPrint("You're too tired to chaarge again...\n");
					}
					else if (action == "/chaarge!" || action == "/sword_attack" || action == "/hunker_down" || action == "/use_potion") break;
					else if (cin.fail())
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Please choose a valid action for your hero.\n";
						sleep_for(milliseconds(500));
					}
					else
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Please choose a valid action for your hero.\n";
						sleep_for(milliseconds(500));
					}
				}

				//Turn-ending actions for warrior.
				if (action == "/sword_attack")
				{
					if (dodge == true)
					{
						slowPrint(attacker.getName() + " swings his sword at " + defender.getName() + " and misses.\n");
						if (attacker.isRevealed() == true) setLastResult(attacker.getName() + " swung his sword at " + defender.getName() + " and missed.\n");
						else setLastResult(attacker.getName() + " tried to attack " + defender.getName() + " with something sharp but missed.\n");
					}
					else if (crit == true && dodge != true)
					{
						damageDealt = ((rand() % (2 * attacker.getPhysicalDmg())) + 42) - defender.getArmor();
						if (defender.getClass() == "tank" && defender.getArmor() > 0)
						{
							damageDealt = ((rand() % (2 * attacker.getPhysicalDmg())) + 42);
							int reflectedDamage = damageDealt - defender.getArmor();
							if (reflectedDamage > 0)
							{
								slowPrint(attacker.getName() + " tries to attack " + defender.getName() + " with a sword, but it reflects off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
								attacker.setHealth(attacker.getHealth() - reflectedDamage);
								defender.setArmor(defender.getArmor() - reflectedDamage);
								if (defender.getArmor() < 0) defender.setArmor(0);
								setLastResult(attacker.getName() + " tried to attack " + defender.getName() + " with a sword, but it reflected off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
							}
							else if (reflectedDamage <= 0)
							{
								slowPrint(attacker.getName() + " tries to attack " + defender.getName() + " with a sword, but it seems too weak to break through their resistances.\n");
								setLastResult(attacker.getName() + " tried to attack " + defender.getName() + " with a sword, but it seems to have been too weak to break through their resistances.\n");
							}
						}
						else
						{
							defender.setHealth(defender.getHealth() - damageDealt);
							slowPrint(attacker.getName() + " swings his sword at " + defender.getName() + " and deals " + to_string(damageDealt) + " crit damage.\n");
							setLastResult(attacker.getName() + " swung his sword at " + defender.getName() + " and dealt " + to_string(damageDealt) + " crit damage.\n");
							attacker.setRevealed(true);
						}
					}
					else
					{
						damageDealt = ((rand() % (attacker.getPhysicalDmg())) + 21) - defender.getArmor();
						if (defender.getClass() == "tank" && defender.getArmor() > 0)
						{
							damageDealt = ((rand() % (attacker.getPhysicalDmg())) + 21);
							int reflectedDamage = damageDealt - defender.getArmor();
							if (reflectedDamage > 0)
							{
								slowPrint(attacker.getName() + " tries to attack " + defender.getName() + " with a sword, but it reflects off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
								attacker.setHealth(attacker.getHealth() - reflectedDamage);
								defender.setArmor(defender.getArmor() - reflectedDamage);
								if (defender.getArmor() < 0) defender.setArmor(0);
								setLastResult(attacker.getName() + " tried to attack " + defender.getName() + " with a sword, but it reflected off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
							}
							else if (reflectedDamage <= 0)
							{
								slowPrint(attacker.getName() + " tries to attack " + defender.getName() + " with a sword, but it seems too weak to break through their resistances.\n");
								setLastResult(attacker.getName() + " tried to attack " + defender.getName() + " with a sword, but it seems to have been too weak to break through their resistances.\n");
							}
						}
						else
						{
							defender.setHealth(defender.getHealth() - damageDealt);
							slowPrint(attacker.getName() + " swings his sword at " + defender.getName() + " and deals " + to_string(damageDealt) + " damage.\n");
							if (attacker.isRevealed() == true) setLastResult(attacker.getName() + " swung his sword at " + defender.getName() + " and dealt " + to_string(damageDealt) + " damage.\n");
							else setLastResult(attacker.getName() + " attacked " + defender.getName() + " with something sharp and dealt " + to_string(damageDealt) + " damage.\n");
						}
					}
					attacker.setLastAction("/sword_attack");
				}
				else if (action == "/use_potion")
				{
					attacker.setHealth(attacker.getHealth() + POTION_HEAL);
					if (attacker.getHealth() > WARRIOR_MAX_HP) attacker.setHealth(WARRIOR_MAX_HP);
					slowPrint("You have recovered 100 HP, and are now at " + to_string(attacker.getHealth()) + " HP.\n");
					if (attacker.isPoisoned() == true)
					{
						attacker.setPoison(false);
						slowPrint("You are no longer poisoned.\n");
					}
					if (attacker.isBurning() == true)
					{
						attacker.setBurn(false);
						attacker.setTurnsBurned(2);
					}
					setLastResult(attacker.getName() + " doesn't seem to have done anything...\n");
					attacker.setLastAction("/use_potion");
					if ( attacker.getNumOfPotions() != 0) attacker.setNumOfPotions(attacker.getNumOfPotions() - 1);
				}
				else if (action == "/hunker_down")
				{
					attacker.setDodgeChance(attacker.getDodgeChance() + 0.25);
					setLastResult(attacker.getName() + " doesn't seem to have done anything...\n");
					attacker.setLastAction("/hunker_down");
				}
				else if (action == "/chaarge!" && attacker.canCharge() == true)
				{
					attacker.setdidCharge(true);
					attacker.takeTurn(true);
					defender.takeTurn(false);
					attacker.setRevealed(true);
					if (dodge == true)
					{
						slowPrint(attacker.getName() + " charges, running at " + defender.getName() + " and misses, running right past and falling to the ground.\n");
						setLastResult(attacker.getName() + " charged, running at " + defender.getName() + ", but missed completely and fell down. But he returned angry to force an extra turn.\n");
					}
					else if (crit == true && dodge != true)
					{
						damageDealt = ((rand() % 2 * (int)round(1.4 * attacker.getPhysicalDmg())) + 70) - defender.getArmor();
						defender.setHealth(defender.getHealth() - damageDealt);
						slowPrint(attacker.getName() + " charges at  " + defender.getName() + " and slams into them dealing " + to_string(damageDealt) + " crit damage.\n");
						setLastResult(attacker.getName() + " charged at " + defender.getName() + " and slammed into them dealing " + to_string(damageDealt) + " crit damage. Then forced an extra turn.\n");
					}
					else
					{
						damageDealt = ((rand() % (int)round(1.4 * attacker.getPhysicalDmg())) + 35) - defender.getArmor();
						defender.setHealth(defender.getHealth() - damageDealt);
						slowPrint(attacker.getName() + " charges at  " + defender.getName() + " and slams into them dealing " + to_string(damageDealt) + " damage.\n");
						setLastResult(attacker.getName() + " charged at " + defender.getName() + " and slammed into them dealing " + to_string(damageDealt) + " damage. Then forced an extra turn.\n");
					}
					attacker.setLastAction("/chaarge!");
				}
			}
			else if (attacker.getClass() == "mage")
			{
				if (attacker.getMana() < MAGE_MAX_MANA) attacker.setMana(attacker.getMana() + MAGE_MANAPERTURN);
				if (attacker.getMana() > MAGE_MAX_MANA) attacker.setMana(MAGE_MAX_MANA);

				//Available actions.
				{
					if (attacker.getMana() < MAGE_MAX_MANA)
					{
						cout << "/build_energy\t(gain " << MAGE_MANAGAIN << " mana, and shield yourself of " << MAGE_SHIELD_AMOUNT << " physical or magic damage until your next turn)\n";
						sleep_for(milliseconds(50));
					}
					cout << "/shoot_energy\t(30-80 magic damage)(" + to_string(MAGE_SHOOTENERGY_MANACOST) + " mana)\n";
					sleep_for(milliseconds(50));
					cout << "/shoot_fireball\t(" << to_string(MAGE_FIREBALL_DAMAGE) << " magic damage, and burns enemy for 2 turns for 30 damage/turn)(" << to_string(MAGE_FIREBALL_MANACOST) << " mana)(reveals you)\n";
					sleep_for(milliseconds(50));
					sleep_for(milliseconds(50));
					cout << "/hunker_down\tincrease dodge chance\n";
					sleep_for(milliseconds(50));
					cout << "/stats\tview your stats\n";
					sleep_for(milliseconds(50));
					cout << "/battle_log\n";
					sleep_for(milliseconds(50));
					cout << "/help\n";
					sleep_for(milliseconds(50));
					cout << "\n/unleash_energy\t(" << to_string(MAGE_UNLEASHENERGY_DAMAGE) + " magic damage)(" << to_string(attacker.getMana()) << "/" << to_string(MAGE_UNLEASHENERGY_MANACOST) << " mana)(cannot be dodged)\n";
					sleep_for(milliseconds(50));
					if (attacker.getMana() >= MAGE_UNLEASHENERGY_MANACOST)
					{
						sleep_for(milliseconds(200));
						slowPrint("You have enough mana to unleash all that energy and KO your opponent.\n");
						sleep_for(milliseconds(50));
					}
					sleep_for(milliseconds(200));
					cout << "\nYou have " << attacker.getMana() << " mana.\n";
					sleep_for(milliseconds(50));
				}

				//Input validation, and non-turn-endning actions for mage.
				while (1)
				{
					slowPrint("\nWhat would you like to do, " + attacker.getName() + "?\n");
					cin >> action;
					if (action == "/stats")
					{
						attacker.printStats();
					}
					else if (action == "/battle_log")
					{
						cout << "**Battle log**\n\n";
						for (int i = 1; i < getTurn(); i++)	cout << resultLog[i] << endl;
					}
					else if (action == "/help")
					{
						slowPrint("Available actions:\n");
						sleep_for(milliseconds(200));
						if (attacker.getMana() >= MAGE_SHOOTENERGY_MANACOST)
						{
							cout << "/shoot_energy\t(30-80 magic damage)(" + to_string(MAGE_SHOOTENERGY_MANACOST) + " mana)\n";
							sleep_for(milliseconds(50));
						}
						if (attacker.getMana() >= MAGE_FIREBALL_MANACOST)
						{
							cout << "/shoot_fireball\t(" + to_string(MAGE_FIREBALL_DAMAGE) + " magic damage, and burns enemy for 2 turns for 30 damage / turn)(" + to_string(MAGE_FIREBALL_MANACOST) + " mana)\n";
							sleep_for(milliseconds(50));
						}
						cout << "/hunker_down\tincrease dodge chance\n";
						sleep_for(milliseconds(50));
						cout << "/stats\tview your stats\n";
						sleep_for(milliseconds(50));
						cout << "/battle_log\n";
						sleep_for(milliseconds(50));
						cout << "/help\n";
						sleep_for(milliseconds(50));
						cout << "\n/unleash_energy\t(" + to_string(MAGE_UNLEASHENERGY_DAMAGE) + " magic damage)(" + to_string(attacker.getMana()) + "/" + to_string(MAGE_FIREBALL_MANACOST) + " mana)(cannot be dodged)\n";
						sleep_for(milliseconds(50));

					}
					else if (action == "/build_energy" && attacker.getMana() == MAGE_MAX_MANA)
					{
						slowPrint("You're already at your maximum mana--you can unleash all of that energy to KO your opponent.\n");
						sleep_for(milliseconds(200));
					}
					else if ((action == "/shoot_fireball" && attacker.getMana() < MAGE_FIREBALL_MANACOST) || (action == "/shoot_energy" && attacker.getMana() < MAGE_SHOOTENERGY_MANACOST) || (action == "/unleash_energy" && attacker.getMana() < MAGE_UNLEASHENERGY_MANACOST))
					{
						slowPrint("You don't have enough mana to do that...\n");
						sleep_for(milliseconds(200));
					}
					else if (action == "/shoot_energy" || action == "/hunker_down" || action == "/shoot_fireball" || action == "/unleash_energy" || action == "/build_energy") break;
					else if (cin.fail())
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Please choose a valid action for your hero.\n";
						sleep_for(milliseconds(500));
					}
					else
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Please choose a valid action for your hero.\n";
						sleep_for(milliseconds(500));
					}
				}

				//Turn-ending actions for mage.
				if (action == "/shoot_energy")
				{
					attacker.setMana(attacker.getMana() - MAGE_SHOOTENERGY_MANACOST);
					if (dodge == true)
					{
						slowPrint(attacker.getName() + " shoots a blast of energy at " + defender.getName() + " but it misses.\n");
						if (attacker.isRevealed() == true) setLastResult(attacker.getName() + " shot a blast of energy at " + defender.getName() + " but it missed.\n");
						else setLastResult(attacker.getName() + " tried to shoot something at " + defender.getName() + " but it missed.\n");
					}
					else if (crit == true && dodge != true)
					{
						damageDealt = ((rand() % (2 * attacker.getMagicDmg())) + 62) - defender.getMagicRes();
						if (defender.getClass() == "tank" && defender.getMagicRes() > 0)
						{
							damageDealt = ((rand() % (2 * attacker.getMagicDmg())) + 62);
							int reflectedDamage = damageDealt - defender.getMagicRes();
							if (reflectedDamage > TANK_MAX_REFLECT) reflectedDamage = TANK_MAX_REFLECT;
							if (reflectedDamage > 0)
							{
								slowPrint(attacker.getName() + " shoots energy at " + defender.getName() + ", but it reflects off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of magic resistance off " + defender.getName() + ".\n");
								attacker.setHealth(attacker.getHealth() - reflectedDamage);
								defender.setMagicRes(defender.getMagicRes() - reflectedDamage);
								if (defender.getMagicRes() < 0) defender.setMagicRes(0);
								setLastResult(attacker.getName() + " shot energy at " + defender.getName() + ", but it reflected off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of magic resistance off " + defender.getName() + ".\n");
							}
							else if (reflectedDamage <= 0)
							{
								slowPrint(attacker.getName() + " tries to attack " + defender.getName() + " with a sword, but it seems too weak to break through their resistances.\n");
								setLastResult(attacker.getName() + " tried to attack " + defender.getName() + " with a sword, but it seems to have been too weak to break through their resistances.\n");
							}
						}
						else
						{
							defender.setHealth(defender.getHealth() - damageDealt);
							slowPrint(attacker.getName() + " shoots a blast of energy at " + defender.getName() + " and it explodes unexpectedly and deals " + to_string(damageDealt) + " crit damage.\n");
							setLastResult(attacker.getName() + " shot a blase of energy at " + defender.getName() + " and it exploded unexpectedly and dealt " + to_string(damageDealt) + " crit damage.\n");
						}
					}
					else
					{
						damageDealt = ((rand() % (attacker.getMagicDmg())) + 31) - defender.getMagicRes();
						if (defender.getClass() == "tank" && defender.getMagicRes() > 0)
						{
							damageDealt = ((rand() % (attacker.getMagicDmg())) + 31);
							int reflectedDamage = damageDealt - defender.getMagicRes();
							if (reflectedDamage > TANK_MAX_REFLECT) reflectedDamage = TANK_MAX_REFLECT;
							if (reflectedDamage > 0)
							{
								slowPrint(attacker.getName() + " shoots energy at " + defender.getName() + ", but it reflects off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of magic resistance off " + defender.getName() + ".\n");
								attacker.setHealth(attacker.getHealth() - reflectedDamage);
								defender.setMagicRes(defender.getMagicRes() - reflectedDamage);
								if (defender.getMagicRes() < 0) defender.setMagicRes(0);
								if (attacker.isRevealed() == true) setLastResult(attacker.getName() + " shot energy at " + defender.getName() + ", but it reflected off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of magic resistance off " + defender.getName() + ".\n");
								else setLastResult(attacker.getName() + " shot something at " + defender.getName() + ", but it reflected off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of magic resistance off " + defender.getName() + ".\n");
							}
							else if (reflectedDamage <= 0)
							{
								slowPrint(attacker.getName() + " shoots energy at " + defender.getName() + ", but it seems too weak too break through their resistances.\n");
								if (attacker.isRevealed() == false) setLastResult(attacker.getName() + " tried to shoot something at " + defender.getName() + ", but it seems to have been too weak to break through their resistances.\n");
								else setLastResult(attacker.getName() + " shot energy at " + defender.getName() + ", but it seems too weak too break through their resistances.\n");
							}
						}
						else
						{
							defender.setHealth(defender.getHealth() - damageDealt);
							slowPrint(attacker.getName() + " shoots a blast of energy at " + defender.getName() + " and it deals " + to_string(damageDealt) + " damage.\n");
							if (attacker.isRevealed() == true) setLastResult(attacker.getName() + " shot a blase of energy at " + defender.getName() + " and it dealt " + to_string(damageDealt) + " damage.\n");
							else setLastResult(attacker.getName() + " shot something at " + defender.getName() + " and it dealt " + to_string(damageDealt) + " damage.\n");
						}
					}
					attacker.setLastAction("/shoot_energy");
				}
				else if (action == "/build_energy")
				{
					attacker.setMana(attacker.getMana() + MAGE_MANAGAIN);
					if (attacker.getMana() > MAGE_MAX_MANA) attacker.setMana(MAGE_MAX_MANA);
					attacker.setArmor(attacker.getArmor() + MAGE_SHIELD_AMOUNT);
					attacker.setMagicRes(attacker.getMagicRes() + MAGE_SHIELD_AMOUNT);
					attacker.setShielded(true);
					slowPrint(attacker.getName() + " collects energy from their surroundings, and gains " + to_string(MAGE_MANAGAIN) + " mana--now has " + to_string(attacker.getMana()) + " mana.\nThey also gain a " + to_string(MAGE_SHIELD_AMOUNT) + " damage shield until their next turn.\n");
					setLastResult(attacker.getName() + " doesn't seem to have done anything...\n");
					attacker.setLastAction(action);
				}
				else if (action == "/hunker_down")
				{
					attacker.setDodgeChance(attacker.getDodgeChance() + 0.25);
					setLastResult(attacker.getName() + " doesn't seem to have done anything...\n");
					attacker.setLastAction("/hunker_down");
				}
				else if (action == "/shoot_fireball")
				{
					attacker.setMana(attacker.getMana() - MAGE_FIREBALL_MANACOST);
					attacker.setRevealed(true);
					if (dodge == true)
					{
						slowPrint(attacker.getName() + "shoots a fireball at " + defender.getName() + " but misses.\n");
					}
					else
					{
						attacker.setDamageOverTime(30);
						damageDealt = MAGE_FIREBALL_DAMAGE;
						defender.setHealth(defender.getHealth() - damageDealt);
						if (defender.isBurning() == true)
						{
							defender.setTurnsBurned(0);
							slowPrint(attacker.getName() + " shoots a fireball at " + defender.getName() + " and deals " + to_string(damageDealt) + " damage, and reapplies burn effect for 2 turns.\n");
							setLastResult(attacker.getName() + "shot a fireball at " + defender.getName() + " and dealt " + to_string(damageDealt) + " damage, and reapplied burn effect for 2 turns.\n");
						}
						else
						{
							defender.setBurn(true);
							slowPrint(attacker.getName() + " shoots a fireball at " + defender.getName() + " and deals " + to_string(damageDealt) + " damage, and applies burn effect for 2 turns.\n");
							setLastResult(attacker.getName() + "shot a fireball at " + defender.getName() + " and dealt " + to_string(damageDealt) + " damage, and applied burn effect for 2 turns.\n");
						}
					}
					attacker.setLastAction("/shoot_fireball");
				}
				else if (action == "/unleash_energy")
				{
					attacker.setRevealed(true);
					attacker.setMana(attacker.getMana() - MAGE_UNLEASHENERGY_MANACOST);
					damageDealt = MAGE_UNLEASHENERGY_DAMAGE - defender.getMagicRes();
					defender.setHealth(defender.getHealth() - damageDealt);
					slowPrint("\n...\n", 100);
					if (defender.getClass() == "tank" && defender.getMagicRes() > 0)
					{
						if (defender.getHealth() <= 0) slowPrint(attacker.getName() + " unleashes all of their built up energy, dealing a devastating " + to_string(MAGE_UNLEASHENERGY_DAMAGE) + " magic damage to " + defender.getName() + " and KO's them.\n");
						else
						{
							slowPrint(attacker.getName() + " unleashes all of their built up energy, dealing a devastating " + to_string(damageDealt) + " magic damage to " + defender.getName() + ", and destroying all of their magic resistance.\n");
							defender.setMagicRes(0);
							setLastResult(attacker.getName() + " unleashed all of their built up energy, dealing a devastating " + to_string(damageDealt) + " magic damage to " + defender.getName() + ", and destroying all of their magic resistance.\n");
						}
					}
					else
					{
						if (defender.getHealth() <= 0) slowPrint(attacker.getName() + " unleashes all of their built up energy, dealing a devastating " + to_string(MAGE_UNLEASHENERGY_DAMAGE) + " magic damage to " + defender.getName() + " and KO's them.\n");
						else
						{
							slowPrint(attacker.getName() + " unleashes all of their built up energy, dealing a devastating " + to_string(MAGE_UNLEASHENERGY_DAMAGE) + " magic damage to " + defender.getName() + ".\n");
							setLastResult(attacker.getName() + " unleashed all of their built up energy, dealing a devastating " + to_string(MAGE_UNLEASHENERGY_DAMAGE) + " magic damage to " + defender.getName() + ".\n");
						}
					}
				}
			}
			else if (attacker.getClass() == "rogue")
			{
				//Available actions.
				{
					cout << "/dagger_stab\t(30-70 physical damage)\n";
					sleep_for(milliseconds(50));
					cout << "/hunker_down\tincrease dodge chance\n";
					sleep_for(milliseconds(50));
					cout << "/stats\tview your stats\n";
					sleep_for(milliseconds(50));
					cout << "/battle_log\n";
					sleep_for(milliseconds(50));
					cout << "/help\n";
					sleep_for(milliseconds(50));
				}

				//Input validation, and non-turn-endning actions for rogue.
				while (1)
				{
					slowPrint("\nWhat would you like to do, " + attacker.getName() + "?\n");
					cin >> action;
					if (action == "/stats")
					{
						attacker.printStats();
					}
					else if (action == "/battle_log")
					{
						cout << "**Battle log**\n\n";
						for (int i = 1; i < getTurn(); i++)
						{
							cout << resultLog[i] << endl;
						}
					}
					else if (action == "/help")
					{
						slowPrint("Available actions:\n\n");
						sleep_for(milliseconds(200));
						cout << "/dagger_stab\t(30-70 physical damage)\n/hunker_down\n/stats\n/battle_log\n";
					}
					else if (action == "/dagger_stab" || action == "/hunker_down") break;
					else if (cin.fail())
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Please choose a valid action for your hero.\n";
						sleep_for(milliseconds(500));
					}
					else
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Please choose a valid action for your hero.\n";
						sleep_for(milliseconds(500));
					}
				}

				//Turn-ending actions for rogue.
				if (action == "/dagger_stab")
				{
					if (dodge == true)
					{
						slowPrint(attacker.getName() + " tries to stab " + defender.getName() + " with a dagger, but misses.\n");
						if (attacker.isRevealed() == true) setLastResult(attacker.getName() + " tried to stab " + defender.getName() + " with a dagger but missed.\n");
						else setLastResult(attacker.getName() + " tried to attack " + defender.getName() + " with something sharp but missed.\n");
					}
					else if (crit == true && dodge != true)
					{
						damageDealt = ((rand() % (2 * attacker.getPhysicalDmg())) + 50) - defender.getArmor();
						if (defender.getClass() == "tank" && defender.getArmor() > 0)
						{
							damageDealt = ((rand() % (2 * attacker.getPhysicalDmg())) + 50);
							int reflectedDamage = damageDealt - defender.getArmor();
							if (reflectedDamage > TANK_MAX_REFLECT) reflectedDamage = TANK_MAX_REFLECT;
							if (reflectedDamage > 0)
							{
								slowPrint(attacker.getName() + " tries to stab " + defender.getName() + " with a dagger, but it reflects off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
								attacker.setHealth(attacker.getHealth() - reflectedDamage);
								defender.setArmor(defender.getArmor() - reflectedDamage);
								if (defender.getArmor() < 0) defender.setArmor(0);
								setLastResult(attacker.getName() + " tried to stab " + defender.getName() + " with a dagger, but it reflected off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
							}
							else if (reflectedDamage <= 0)
							{
								slowPrint(attacker.getName() + " tries to stab " + defender.getName() + " with a dagger, but it seems too weak to break through their resistances.\n");
								setLastResult(attacker.getName() + " tried to stab " + defender.getName() + " with a dagger, but it seems to have been too weak to break through their resistances.\n");
							}
						}
						else
						{
							defender.setHealth(defender.getHealth() - damageDealt);
							if (attacker.getDamageOverTime() == 0) slowPrint(attacker.getName() + " stabs " + defender.getName() + " with a sharp dagger and crits for " + to_string(damageDealt) + " damage, and applies poison.\n");
							else slowPrint(attacker.getName() + " stabs " + defender.getName() + " with a sharp dagger and crits for " + to_string(damageDealt) + " damage, and increases poison by 10 damage/turn.\n");
							if (attacker.getDamageOverTime() == 0) setLastResult(attacker.getName() + " stabbed " + defender.getName() + " with a sharp dagger and crit for " + to_string(damageDealt) + " damage, and applied poison.\n");
							else setLastResult(attacker.getName() + " stabbed " + defender.getName() + " with a sharp dagger and crit for " + to_string(damageDealt) + " damage, and increased poison damage by 10 damage/turn.\n");
							defender.setPoison(true);
							attacker.setDamageOverTime(attacker.getDamageOverTime() + 10);
							attacker.setRevealed(true);
						}
					}
					else
					{
						damageDealt = ((rand() % (attacker.getPhysicalDmg())) + 25) - defender.getArmor();
						if (defender.getClass() == "tank" && defender.getArmor() > 0)
						{
							damageDealt = ((rand() % (attacker.getPhysicalDmg())) + 25);
							int reflectedDamage = damageDealt - defender.getArmor();
							if (reflectedDamage > TANK_MAX_REFLECT) reflectedDamage = TANK_MAX_REFLECT;
							if (reflectedDamage > 0)
							{
								slowPrint(attacker.getName() + " tries to stab " + defender.getName() + " with a dagger, but it reflects off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
								attacker.setHealth(attacker.getHealth() - reflectedDamage);
								defender.setArmor(defender.getArmor() - reflectedDamage);
								if (defender.getArmor() < 0) defender.setArmor(0);
								setLastResult(attacker.getName() + " tried to stab " + defender.getName() + " with a dagger, but it reflected off them, reflecting " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
							}
							else if (reflectedDamage <= 0)
							{
								slowPrint(attacker.getName() + " tries to stab " + defender.getName() + " with a dagger, but it seems too weak to break through their resistances.\n");
								setLastResult(attacker.getName() + " tried to stab " + defender.getName() + " with a dagger, but it seems to have been too weak to break through their resistances.\n");
							}
						}
						else
						{
							defender.setHealth(defender.getHealth() - damageDealt);
							slowPrint(attacker.getName() + " stabs " + defender.getName() + " for " + to_string(damageDealt) + " damage.\n");
							if (attacker.isRevealed() == true) setLastResult(attacker.getName() + " stabbed " + defender.getName() + " for " + to_string(damageDealt) + " damage.\n");
							else setLastResult(attacker.getName() + " attacked " + defender.getName() + " with something sharp and dealt " + to_string(damageDealt) + " damage.\n");
						}
					}
					attacker.setLastAction("/dagger_stab");
				}
				else if (action == "/hunker_down")
				{
					attacker.setDodgeChance(attacker.getDodgeChance() + 0.25);
					setLastResult(attacker.getName() + " doesn't seem to have done anything...\n");
					attacker.setLastAction("/hunker_down");
				}
			}
			else if (attacker.getClass() == "tank")
			{
				bool removedAnyRes = 0, removedArmor = 0, removedMagicRes = 0;
				if (attacker.getHealth() > TANK_THROWROCK_HPCOST && attacker.getArmor() == 0 && attacker.getMagicRes() == 0) attacker.allowThrowRock(true);
				else attacker.allowThrowRock(false);
				if (attacker.getHealth() <= ((float)TANK_MAX_HP * 0.5) && (attacker.getArmor() < TANK_ARMOR || attacker.getMagicRes() < TANK_MAGICRES) && attacker.getHealth() > TANK_REGENRES_HPCOST && attacker.didRegenRes() == false) attacker.allowRegenRes(true);
				else attacker.allowRegenRes(false);
				//Available actions.
				{
					cout << "/do_nothing\n";
					sleep_for(milliseconds(50));
					if (attacker.canRegenRes() == true)
					{
						cout << "/regen_res\trebuild all of your armor and magic resist by paying " << TANK_REGENRES_HPCOST << " HP (once only per battle).\n";
						sleep_for(milliseconds(50));
					}
					if (attacker.getArmor() == 0)
					{
						cout << "/bodyslam\trun at your opponent and deal " << TANK_BODYSLAM_DAMAGE << " physical damage to your opponent.\n";
						sleep_for(milliseconds(50));
					}
					if (attacker.canThrowRock() == true)
					{
						cout << "/throw_rock\tpay " << TANK_THROWROCK_HPCOST << " HP to deal " << TANK_THROWROCK_DAMAGE << " true damage. (Cannot be dodged).\n";
						sleep_for(milliseconds(50));
					}
					if (attacker.getArmor() > 0)
					{
						cout << "/destroy_armor\tdestroy all your armor to enable /bodyslam attack, and to gain " << TANK_RESBREAK_HEAL << " HP.\n";
						sleep_for(milliseconds(50));
					}
					if (attacker.getMagicRes() > 0)
					{
						cout << "/destroy_magicres\tdestroy all your magic resistance to gain " << TANK_RESBREAK_HEAL << " HP.\n";
						sleep_for(milliseconds(50));
					}
					cout << "/stats\tview your stats\n";
					sleep_for(milliseconds(50));
					cout << "/battle_log\n";
					sleep_for(milliseconds(50));
					cout << "/help\n";
					sleep_for(milliseconds(50));
				}


				//Input validation, and non-turn-endning actions for tank.
				while (1)
				{
					slowPrint("\nWhat would you like to do, " + attacker.getName() + "?\n");
					cin >> action;
					if (action == "/stats")
					{
						attacker.printStats();
					}
					else if (action == "/battle_log")
					{
						cout << "**Battle log**\n\n";
						for (int i = 1; i < getTurn(); i++)
						{
							cout << resultLog[i] << endl;
						}
					}
					else if ((action == "/destroy_armor" || action == "/destroy_magicres") && removedAnyRes == false)
					{
						if (action == "/destroy_armor")
						{
							if (attacker.getArmor() <= 0)
							{
								slowPrint("You have no armor left to destroy.\n");
								sleep_for(milliseconds(200));
							}
							else
							{
								removedAnyRes = true;
								attacker.setArmor(0);
								attacker.setHealth(attacker.getHealth() + TANK_RESBREAK_HEAL);
								if (attacker.getHealth() > TANK_MAX_HP) attacker.setHealth(TANK_MAX_HP);
								slowPrint("You've destroyed your armor and gained " + to_string(TANK_RESBREAK_HEAL) + " HP. You now have " + to_string(attacker.getHealth()) + " HP.\n");
								sleep_for(milliseconds(200));
								cout << "You can now use /bodyslam to deal 60 true damage to your opponent. (Can be dodged).\n";
								sleep_for(milliseconds(100));
								if (attacker.getMagicRes() == 0 && attacker.getHealth() > TANK_THROWROCK_HPCOST)
								{
									cout << "You can now use /throw_rock to pay " << TANK_THROWROCK_HPCOST << " HP and deal " << TANK_THROWROCK_DAMAGE << " true damage to your opponent.\n";
									attacker.allowThrowRock(true);
								}
								sleep_for(milliseconds(100));
							}
						}
						if (action == "/destroy_magicres")
						{
							if (attacker.getMagicRes() == 0)
							{
								slowPrint("You have no magic resistance left to destroy.\n");
							}
							else
							{
								removedAnyRes = true;
								attacker.setMagicRes(0);
								attacker.setHealth(attacker.getHealth() + TANK_RESBREAK_HEAL);
								if (attacker.getHealth() > TANK_MAX_HP) attacker.setHealth(TANK_MAX_HP);
								slowPrint("You've destroyed your magic resistance and gained " + to_string(TANK_RESBREAK_HEAL) + " HP. You now have " + to_string(attacker.getHealth()) + " HP.\n");
								sleep_for(milliseconds(100));
								if (attacker.getArmor() == 0 && attacker.getHealth() > TANK_THROWROCK_HPCOST)
								{
									cout << "You can now use /throw_rock to pay " << TANK_THROWROCK_HPCOST << " HP and deal " << TANK_THROWROCK_DAMAGE << " true damage to your opponent.\n";
									attacker.allowThrowRock(true);
								}
								sleep_for(milliseconds(100));
							}
						}
					}
					else if ((action == "/destroy_armor" || action == "/destroy_magicres") && removedAnyRes == true)
					{
						slowPrint("You can only destroy one type of your resistances per turn.\n");
						sleep_for(milliseconds(200));
					}
					else if (action == "/bodyslam" && attacker.getArmor() != 0)
					{
						slowPrint("You must first lose all your armor to do that--you're too heavy.\nYou currently have " + to_string(attacker.getArmor()) + " armor.\n");
						sleep_for(milliseconds(200));
					}
					else if (action == "/regen_res" && attacker.canRegenRes() == false)
					{
						if (attacker.getArmor() == TANK_ARMOR && attacker.getMagicRes() == TANK_MAGICRES)
						{
							slowPrint("Your resistances are already at their maximum.\n");
							sleep_for(milliseconds(200));
						}
						else if (attacker.getHealth() <= TANK_REGENRES_HPCOST)
						{
							slowPrint("There's not a piece of rock on your body that you'd be able to survive without.\n");
							sleep_for(milliseconds(200));
						}
						else if (attacker.getHealth() > (float)TANK_MAX_HP * 0.5)
						{
							slowPrint("You must be below 50% HP to do that. Your max HP is " + to_string(TANK_MAX_HP) + " and your current HP is " + to_string(attacker.getHealth()) + ".\n");
							sleep_for(milliseconds(200));
						}
						else if (attacker.didRegenRes() == true)
						{
							slowPrint("You can only regen your resistances once per battle.\n");
							sleep_for(milliseconds(200));
						}
					}
					else if (action == "/throw_rock" && attacker.canThrowRock() == false)
					{
						if (attacker.getArmor() != 0 || attacker.getMagicRes() != 0)
						{
							slowPrint("You must first destroy all your resistances in order to do that.\nYour currently have " + to_string(attacker.getArmor()) + " armor and " + to_string(attacker.getMagicRes()) + " magic resistance.\n");
							sleep_for(milliseconds(200));
						}
						else if (attacker.getHealth() < TANK_THROWROCK_HPCOST)
						{
							slowPrint("You don't have enough HP to do that...\n");
							sleep_for(milliseconds(200));
						}
					}
					else if (action == "/throw_rock" && attacker.getHealth() < TANK_THROWROCK_HPCOST)
					{
						attacker.allowThrowRock(false);
						slowPrint("You don't have enough HP to do that.\n");
						sleep_for(milliseconds(200));
					}
					else if (action == "/help")
					{
						slowPrint("Available actions:\n\n");
						sleep_for(milliseconds(200));
						cout << "/do_nothing\n";
						sleep_for(milliseconds(50));
						if (attacker.getArmor() == 0)
						{
							cout << "/bodyslam\t" << "(" << TANK_BODYSLAM_DAMAGE << " physical damage).\n";
							sleep_for(milliseconds(50));
						}
						if (attacker.canRegenRes() == true)
						{
							cout << "/regen_res\n\t (pay " << TANK_REGENRES_HPCOST << " HP to rebuild your resistances, once per battle).\n";
							sleep_for(milliseconds(50));
						}
						if (attacker.canThrowRock() == true)
						{
							cout << "/throw_rock\n\t (pay " << TANK_THROWROCK_HPCOST << " HP to deal " << TANK_THROWROCK_DAMAGE << " true damage).\n";
							sleep_for(milliseconds(50));
						}
						if (attacker.getArmor() > 0 && removedAnyRes == false)
						{
							cout << "/destroy_armor\n";
							sleep_for(milliseconds(50));
						}
						if (attacker.getMagicRes() > 0 && removedAnyRes == false)
						{
							cout << "/destroy_magicres\n";
							sleep_for(milliseconds(50));
						}
						cout << "/stats\n";
						sleep_for(milliseconds(50));
						cout << "/battle_log\n";
						sleep_for(milliseconds(50));
					}
					else if (action == "/regen_res" || action == "/do_nothing" || action == "/bodyslam" || action == "/throw_rock") break;
					else if (cin.fail())
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Please choose a valid action for your hero.\n";
						sleep_for(milliseconds(500));
					}
					else
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Please choose a valid action for your hero.\n";
						sleep_for(milliseconds(500));
					}
				}

				//Turn-ending actions for tank.
				if (action == "/do_nothing")
				{
					slowPrint(attacker.getName() + " naps like the rock he is.\n");
					attacker.setLastAction("/do_nothing");
					setLastResult(attacker.getName() + " doesn't seem to have done anything...\n");
				}
				else if (action == "/regen_res")
				{
					attacker.setdidRegenRes(true);
					attacker.setArmor(TANK_ARMOR);
					attacker.setMagicRes(TANK_MAGICRES);
					attacker.setHealth(attacker.getHealth() - TANK_REGENRES_HPCOST);
					slowPrint(attacker.getName() + " rebuilds all of his resistances, but pays " + to_string(TANK_REGENRES_HPCOST)+ " HP to do it-- now has " + to_string(attacker.getHealth()) + " HP.\n");
					setLastResult(attacker.getName() + " doesn't seem to have done anything.\n");
					attacker.setLastAction("/regen_res");
				}
				else if (action == "/bodyslam")
				{
					damageDealt = TANK_BODYSLAM_DAMAGE - defender.getArmor();
					if (dodge == true)
					{
						slowPrint(attacker.getName() + " runs at " + defender.getName() + " trying to bodyslam into them but misses.\n");
						setLastResult(attacker.getName() + " ran at " + defender.getName() + " and tried to bodyslam into them but missed.\n");
					}
					else
					{
						defender.setHealth(defender.getHealth() - damageDealt);
						slowPrint(attacker.getName() + " bodyslams into " + defender.getName() + " dealing " + to_string(damageDealt) + " physical damage.\n");
						setLastResult(attacker.getName() + " bodyslammed into " + defender.getName() + " and dealt " + to_string(damageDealt) + " physical damage.\n");
					}
					attacker.setLastAction(action);
				}
				else if (action == "/throw_rock")
				{
					defender.setHealth(defender.getHealth() - TANK_THROWROCK_DAMAGE);
					attacker.setHealth(attacker.getHealth() - TANK_THROWROCK_HPCOST);
					slowPrint(attacker.getName() + " breaks off a piece of rock off their body and throws it at " + defender.getName() + " dealing " + to_string(TANK_THROWROCK_DAMAGE) + " true damage.\n");
					setLastResult(attacker.getName() + " broke off a piece of rock off their body and threw it at " + defender.getName() + " dealing " + to_string(TANK_THROWROCK_DAMAGE) + " true damage.\n");
					attacker.setLastAction("/throw_rock");
				}
				
			}
			else if (attacker.getClass() == "psychic")
			{
				bool usedReveal_Enemy = false;

				//Available actions.
				{
					
					if (attacker.getHandgunAmmo() > 0)
					{
						cout << "/fire_handgun\t(30-70 psysical damage)\nHandhun ammo remaining: " + to_string(attacker.getHandgunAmmo()) + "\n";
						sleep_for(milliseconds(50));
					}
					cout << "/mind_invasion\t(15-50 magic damage)\n";
					sleep_for(milliseconds(50));
					if (defender.getMagicRes() > 0)
					{
						cout << "/break_magicres\t(breaks 10 magic resistance from your opponent).\n";
						sleep_for(milliseconds(50));
					}
					cout << "/hunker_down\tincrease dodge chance\n";
					sleep_for(milliseconds(50));
					cout << "/stats\tview your stats\n";
					sleep_for(milliseconds(50));
					cout << "/battle_log\n";
					sleep_for(milliseconds(50));
					cout << "/help\n";
					sleep_for(milliseconds(50));
					cout << "/reveal_enemy\t*use psychic powers to access your opponent's mind for 5s to get\ninformation to aid your tactical decisions.*\n";
				}

				//Input validation, and non-turn-endning actions for psychic.
				while (1)
				{
					slowPrint("\nWhat would you like to do, " + attacker.getName() + "?\n");
					cin >> action;

					if (action == "/stats")
					{
						attacker.printStats();
					}
					else if (action == "/battle_log")
					{
						cout << "**Battle log**\n\n";
						for (int i = 1; i < getTurn(); i++)
						{
							cout << resultLog[i] << endl;
						}
					}
					else if (action == "/help")
					{
						slowPrint("Available actions:\n\n");
						sleep_for(milliseconds(200));
						if (attacker.getHandgunAmmo() > 0) cout << "/fire_handgun\t(30-70 psysical damage)\nHandhun ammo remaining: " << to_string(attacker.getHandgunAmmo()) << "\n";
						cout << "/mind_invasion\t(15-50 magic damage)\n";
						if (defender.getMagicRes() > 0) cout << "/break_magicres\tbreak 10 of your opponent's magic resistance.\n";
						cout << "/hunker_down\n/stats\n/battle_log\n";
						if (usedReveal_Enemy == false) cout << "\n/reveal_enemy\n";
					}
					else if (action == "/fire_handgun" && attacker.getHandgunAmmo() <= 0)
					{
						slowPrint("You're out of ammor.\n");
					}
					else if (action == "/reveal_enemy" && usedReveal_Enemy == false)
					{
						usedReveal_Enemy = true;
						sleep_for(milliseconds(500));
						system("cls");
						slowPrint("Acessing enemy's mind...\n", 300);
						slowPrint("...\n", 450);
						cout << defender.getName() << "'s stats and last move:\n";
						sleep_for(milliseconds(500));
						defender.printStats();
						sleep_for(milliseconds(500));
						slowPrint(defender.getName() + "'s last action: " + defender.getLastAction() + "\n");
						sleep_for(milliseconds(5000));
						system("cls");
						sleep_for(milliseconds(500));
						slowPrint("Turn " + to_string(getTurn()) + " - Welcome back to reality, " + attacker.getName() + ". HP: " + to_string(attacker.getHealth()) + "\n\n");
						sleep_for(milliseconds(1000));
						slowPrint("Available actions:\n/fire_handgun\t(30-70 psysical damage)\nHandhun ammo remaining: " + to_string(attacker.getHandgunAmmo()) + "\n/mind_invasion\t(15-50 magic damage)\n/hunker_down\n/stats\n/battle_log\n", 10);
					}
					else if (action == "/reveal_enemy" && usedReveal_Enemy == true)
					{
						slowPrint("Psychic powers too used up to do that again...");
						sleep_for(milliseconds(1000));
					}
					else if (action == "/break_magicres" && (defender.getMagicRes() == 0))
					{
						slowPrint(defender.getName() + "'s magic resistance is already completely destroyed--you deal true damage.\n");
					}
					else if (action == "/fire_handgun" || action == "/mind_invasion" || action == "/hunker_down" || action == "/break_magicres") break;
					else if (cin.fail())
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Please choose a valid action for your hero.\n";
						sleep_for(milliseconds(500));
					}
					else
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Please choose a valid action for your hero.\n";
						sleep_for(milliseconds(500));
					}
				}

				//Turn-ending actions for psychic.
				if (action == "/mind_invasion")
				{
					attacker.setRevealed(true);
					if (defender.getClass() == "tank" && defender.getMagicRes() >= 25)
					{
						slowPrint("Mind invasion is currently ineffective against tanks...\n");
						sleep_for(milliseconds(1000));
						slowPrint("You should try and break their magic resistance next time.\n");
						sleep_for(milliseconds(1000));
					}
					if (dodge == true)
					{
						slowPrint(attacker.getName() + " tries to invade " + defender.getName() + "'s mind but fails.\n");
						setLastResult(attacker.getName() + " tried to invade " + defender.getName() + "'s mind but failed.\n");
					}
					else if (crit == true && dodge != true)
					{
						damageDealt = ((rand() % (2 * attacker.getMagicDmg())) + 50) - defender.getMagicRes();
						if (damageDealt < 0) damageDealt = 0;
						defender.setHealth(defender.getHealth() - damageDealt);
						slowPrint(attacker.getName() + " invades " + defender.getName() + "'s mind and causes severe nerve damage dealing " + to_string(damageDealt) + " crit damage.\n");
						setLastResult(attacker.getName() + " invaded " + defender.getName() + "'s mind and caused severse nerve damage, dealing " + to_string(damageDealt) + " crit damage.\n");
					}
					else
					{
						damageDealt = ((rand() % (attacker.getMagicDmg())) + 25) - defender.getMagicRes();
						if (damageDealt < 0) damageDealt = 0;
						defender.setHealth(defender.getHealth() - damageDealt);
						slowPrint(attacker.getName() + " invades " + defender.getName() + "'s mind and deals " + to_string(damageDealt) + " damage.\n");
						setLastResult(attacker.getName() + " invaded " + defender.getName() + "'s mind and dealt " + to_string(damageDealt) + " damage.\n");
					}
					attacker.setLastAction("/mind_invasion");
				}
				else if (action == "/fire_handgun")
				{
					if (dodge == true)
					{
						slowPrint(attacker.getName() + " fires his handgun at " + defender.getName() + " but misses.\n");
						if (attacker.isRevealed() == true) setLastResult(attacker.getName() + " fired his handgun at " + defender.getName() + " but missed.\n");
						else setLastResult(attacker.getName() + " tried to shoot something at " + defender.getName() + " but it missed.\n");
					}
					else if (crit == true && dodge != true)
					{
						damageDealt = ((rand() % (2 * attacker.getPhysicalDmg())) + 80) - defender.getArmor();
						if (defender.getClass() == "tank" && defender.getArmor() > 0)
						{
							damageDealt = ((rand() % (2 * attacker.getPhysicalDmg())) + 80);
							int reflectedDamage = damageDealt - defender.getArmor();
							if (reflectedDamage > 0)
							{
								slowPrint(attacker.getName() + " shoots " + defender.getName() + " with a handgun, but it reflects back dealing " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
								attacker.setHealth(attacker.getHealth() - reflectedDamage);
								defender.setArmor(defender.getArmor() - reflectedDamage);
								if (defender.getArmor() < 0) defender.setArmor(0);
								setLastResult(attacker.getName() + " shot " + defender.getName() + " with a handgun, but it reflected back dealing " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
							}
							else if (reflectedDamage <= 0)
							{
								slowPrint(attacker.getName() + " shoots " + defender.getName() + " with a handgun, but it seems too weak to break through their resistances.\n");
								setLastResult(attacker.getName() + " shot " + defender.getName() + " with a handgun, but it seems to have been too weak to break through their resistances.\n");
							}
						}
						else
						{
							attacker.setRevealed(true);
							defender.setHealth(defender.getHealth() - damageDealt);
							slowPrint(attacker.getName() + " fires his handgun at " + defender.getName() + " and land a critical strike for " + to_string(damageDealt) + " damage.\n");
							setLastResult(attacker.getName() + " fired his handgun at " + defender.getName() + " and landed a critical strike for " + to_string(damageDealt) + " damage.\n"); 
						}
					}
					else
					{
						damageDealt = ((rand() % (attacker.getPhysicalDmg())) + 40) - defender.getArmor();
						if (defender.getClass() == "tank" && defender.getArmor() > 0)
						{
							damageDealt = ((rand() % (attacker.getPhysicalDmg())) + 40);
							int reflectedDamage = damageDealt - defender.getArmor();
							if (reflectedDamage > 0)
							{
								slowPrint(attacker.getName() + " shoots " + defender.getName() + " with a handgun, but it reflects back dealing " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
								attacker.setHealth(attacker.getHealth() - reflectedDamage);
								defender.setArmor(defender.getArmor() - reflectedDamage);
								if (defender.getArmor() < 0) defender.setArmor(0);
								if (attacker.isRevealed() == true) setLastResult(attacker.getName() + " shot " + defender.getName() + " with a handgun, but it reflected back dealing " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
								else setLastResult(attacker.getName() + " shot " + defender.getName() + " with a something, but it reflected back dealing " + to_string(reflectedDamage) + " damage, and\nshredding the same amount of armor off " + defender.getName() + ".\n");
							}
							else if (reflectedDamage <= 0)
							{
								slowPrint(attacker.getName() + " shoots " + defender.getName() + " with a handgun, but it seems too weak to break through their resistances.\n");
								setLastResult(attacker.getName() + " shot " + defender.getName() + " with a handgun, but it seems to have been too weak to break through their resistances.\n");
							}
						}
						else
						{
							defender.setHealth(defender.getHealth() - damageDealt);
							slowPrint(attacker.getName() + " fires his handgun at " + defender.getName() + " and lands a bullet for " + to_string(damageDealt) + " damage.\n");
							if (attacker.isRevealed() == true) setLastResult(attacker.getName() + " fired his handgun at " + defender.getName() + " and landed a bullet for " + to_string(damageDealt) + " damage.\n");
							else setLastResult(attacker.getName() + " shot something at " + defender.getName() + " and it dealt " + to_string(damageDealt) + " damage.\n");
						}
					}
					if (attacker.getHandgunAmmo() != 0) attacker.setHandgunAmmo(attacker.getHandgunAmmo() - 1);
					attacker.setLastAction("/fire_handgun");
				}
				else if (action == "/hunker_down")
				{
					attacker.setDodgeChance(attacker.getDodgeChance() + 0.25);
					setLastResult(attacker.getName() + " doesn't seem to have done anything...\n");
					attacker.setLastAction("/hunker_down");
				}
				else if (action == "/break_magicres")
				{
					defender.setMagicRes(defender.getMagicRes() - 10);
					if (defender.getMagicRes() < 0) defender.setMagicRes(0);
					slowPrint(attacker.getName() + " uses his psychic powers to telepathically destroy 10 of " + defender.getName() + "'s magic resistance.\n");
					setLastResult(attacker.getName() + " doesn't seem to have done anything...\n");
				}
			}

			//Resetting temporarily-changed stats.
			if (defender.getLastAction() == "/hunker_down") defender.setDodgeChance(defender.getDodgeChance() - 0.25);
			if (attacker.getTurnsBurned() == 2)
			{
				attacker.setBurn(false);
				defender.setDamageOverTime(0);
				slowPrint(defender.getName() + "'s burn effect finally wears off...\n");
				sleep_for(milliseconds(700));
				attacker.setTurnsBurned(0);
			}
			if (defender.isShielded() == true)
			{
				defender.setArmor(defender.getArmor() - MAGE_SHIELD_AMOUNT);
				defender.setMagicRes(defender.getMagicRes() - MAGE_SHIELD_AMOUNT);
				if (defender.getArmor() < 0) defender.setArmor(0);
				if (defender.getMagicRes() < 0) defender.setMagicRes(0);
			}

			//Setting other hero's HP to zero--marks end of battle.
			if (defender.getHealth() < 0) defender.setHealth(0);
			if (attacker.getHealth() < 0) attacker.setHealth(0);
		}
		else;
	}

	//Constructor--from start of battle to its end.
	Battle(Hero& hero1, Hero& hero2)
	{
		while ((hero1.getHealth() > 0) && (hero2.getHealth() > 0))
		{
			if (getTurn() > 1) slowPrint("Last turn's results: " + resultLog[getTurn() - 1] + "\n\n");
			sleep_for(milliseconds(500));

			//Calling takeTurn(Hero&, Hero&) to take turns.
			if (hero1.isTurn() == true) takeTurn(hero1, hero2);
			else if (hero2.isTurn() == true) takeTurn(hero2, hero1);

			//Setting last result and advancing turn.
			resultLog.push_back("Turn " + to_string(getTurn()) + ": " + getLastResult());
			setTurn(getTurn() + 1);
			WaitEnter("Enter anything to end your turn.\n");
			system("CLS");
		}

		//Announcing victory.
		if (hero1.getHealth() == 0)
		{
			cout << hero1.getName() << " has died and " << hero2.getName() << " is victorious.\n";
		}
		else if (hero2.getHealth() == 0)
		{
			cout << hero2.getName() << " has died and " << hero1.getName() << " is victorious.\n";
		}
	}
};

//Function Prototypes 2
string shuffleFirstTurn(Hero& hero1, Hero& hero2);		//Prototype--shuffles, then returns a random hero name and displays it.

int main()
{
	bool playAgain = true; //Used at the end to loop the program after asking the user.
	while (playAgain == true)
	{
		string player1Name, player2Name, player1Class, player2Class; //players' names and classes to pass to <class>Hero and <class>Battle.
		system("cls");
		slowPrint("Welcome to CMD Hero Fights. Play intro? (Y/N): ");
		
		//Determine whether to play intro or not.
		string QplayIntro;
		cin >> QplayIntro;
		while (1)
		{
			if (QplayIntro == "Y" || QplayIntro == "y" || QplayIntro == "yes" || QplayIntro == "Yes")
			{
				playIntro();
				break;
			}
			else if (QplayIntro == "N" || QplayIntro == "n" || QplayIntro == "no" || QplayIntro == "No") break;
			else
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Please enter yes or no. (Y/N): ";
				cin >> QplayIntro;
			}
		}

		//Player 1 creation.
		slowPrint("Please, player 2 now look away, and player 1 prepare to create your hero.\n");
		sleep_for(milliseconds(2500));
		system("CLS");
		sleep_for(milliseconds(1000));
		slowPrint("Now, player 1, name your hero!\n");
		cin >> player1Name;
		while (1)
		{
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Please enter a valid hero name.\n";
				cin >> player1Name;
			}
			else
				break;
		}
		slowPrint(player1Name + " has been created. Now choose a suitable class!\n\n");
		cout << "(1) Warrior\n(2) Mage\n(3) Rogue\n(4) Tank\n(5) Psychic\n(6) Random\n";
		cin >> player1Class;
		while (1)
		{
			if (cin.fail() || (player1Class != "1" && player1Class != "(1)" && player1Class != "Warrior" && player1Class != "warrior"
				&& player1Class != "2" && player1Class != "(2)" && player1Class != "Mage" && player1Class != "mage" &&
				player1Class != "3" && player1Class != "(3)" && player1Class != "Rogue" && player1Class != "rogue" &&
				player1Class != "4" && player1Class != "(4)" && player1Class != "Tank" && player1Class != "tank"
				&& player1Class != "5" && player1Class != "(5)" && player1Class != "Psychic" && player1Class != "psychic"
				&& player1Class != "6" && player1Class != "(6)" && player1Class != "Random" && player1Class != "random"))
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Please choose a valid class.\n";
				cin >> player1Class;
			}
			else
				break;
		}
		Hero Player1(player1Name, player1Class);
		slowPrint(Player1.getName() + " is now a " + Player1.getClass() + ".\n");
		sleep_for(milliseconds(1000));
		slowPrint(Player1.getName() + "'s stats:\n");
		Player1.printStats();
		cout << endl;
		sleep_for(milliseconds(1000));

		//Player 2 creation.
		WaitEnter("Now, player 1 please look away, and player 2, prepare to create your hero.\nEnter anything when you're ready.\n");
		system("CLS");
		sleep_for(milliseconds(1000));
		slowPrint("Now, player 2, name your hero!\n");
		getline(cin, player2Name);
		while (1)
		{
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Please enter a valid hero name.\n";
				cin >> player2Name;
			}
			else
				break;
		}
		slowPrint(player2Name + " has been created. Now choose a suitable class!\n\n");
		cout << "(1) Warrior\n(2) Mage\n(3) Rogue\n(4) Tank\n(5) Psychic\n(6) Random\n";
		getline(cin, player2Class);
		while (1)
		{
			if (cin.fail() || (player2Class != "1" && player2Class != "(1)" && player2Class != "Warrior" && player2Class != "warrior"
				&& player2Class != "2" && player2Class != "(2)" && player2Class != "Mage" && player2Class != "mage" &&
				player2Class != "3" && player2Class != "(3)" && player2Class != "Rogue" && player2Class != "rogue" &&
				player2Class != "4" && player2Class != "(4)" && player2Class != "Tank" && player2Class != "tank"
				&& player2Class != "5" && player2Class != "(5)" && player2Class != "Psychic" && player2Class != "psychic"
				&& player2Class != "6" && player2Class != "(6)" && player2Class != "Random" && player2Class != "random"))
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Please choose a valid class.\n";
				cin >> player2Class;
			}
			else
				break;
		}
		Hero Player2(player2Name, player2Class);
		slowPrint(Player2.getName() + " is now a " + Player2.getClass() + ".\n");
		sleep_for(milliseconds(1000));
		slowPrint(Player2.getName() + "'s stats:\n");
		Player2.printStats();
		cout << endl;
		sleep_for(milliseconds(1000));

		//Battle start.
		WaitEnter("Now gather up, and enter anything when both heros are ready for battle.\n");
		sleep_for(milliseconds(500));
		slowPrint("The hero that will go first is now going to be chosen randomly:\n");
		sleep_for(milliseconds(1500));
		system("cls");
		slowPrint(shuffleFirstTurn(Player1, Player2) + " goes first!\n"); //Shuffle effect and RNG that sets Hero::isTurn to choose a random player to go first.
		sleep_for(milliseconds(2500));
		system("cls");
		Battle myBattle(Player1, Player2);
		//End of battle.

		sleep_for(milliseconds(2000));

		//Asking if the users want to play again--determines program loop continue/break.
		string QplayAgain;
		slowPrint("\n\nGG! Would you like to play again? (Y/N): ");
		cin >> QplayAgain;
		while (1)
		{
			if (QplayAgain == "Y" || QplayAgain == "y" || QplayAgain== "yes" || QplayAgain == "Yes")
			{
				slowPrint("Restarting game");
				slowPrint(".....", 350);
				break;
			}
			else if (QplayAgain == "N" || QplayAgain == "n" || QplayAgain == "no" || QplayAgain == "No")
			{
				slowPrint("Thank you for playing CMD Hero Fights, hope you had fun!! See you soon...\n\n");
				playAgain = false;
				sleep_for(milliseconds(5000));
				break;
			}
			else
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Please enter yes or no. (Y/N): ";
				cin >> QplayAgain;
			}
		}
	}
	return 0;
}

//Function Definitions
void playIntro() {

	slowPrint("\n\nWelcome to CMD Hero Fights, this is a 1v1 game with classes.\n");
	sleep_for(milliseconds(300));
	slowPrint("You will take turns making tactical desicions in order to deffeat your opponent.\nFirst hero to knock the other down to 0 HP wins.\n");
	slowPrint("...\n", 700);
	slowPrint("You have 5 classes to choose from, here's a quick overview:\n\n");
	sleep_for(milliseconds(1200));
	cout << "Warrior: physically aggressive heros who prefer attacking, but have decent defensive stats.\n";
	sleep_for(milliseconds(1200));
	cout << "Mage: spellcasters with magic powers and burning spell effects, but very immobile.\n";
	sleep_for(milliseconds(1200));
	cout << "Rouge: rule-breakers who deal massive amounts of physical damage and crit--very low defensive stats.\n";
	sleep_for(milliseconds(1200));
	cout << "Tank: rock creatures who move extreamely slow, but have reflexive surfaces that resist and reflect all kinds of damage.\n";
	sleep_for(milliseconds(1200));
	cout << "Psychic: supernatural beings who possess psychic powers and can fortell the future...\n";
	sleep_for(milliseconds(1200));
	slowPrint("\nFor the intended tactical gameplay experience, players should not see their opponent's classes during creation, nor \ninform each other of them.\n\n");
	sleep_for(milliseconds(1200));
	slowPrint("Enter anything when you're ready to start.\n");
	WaitEnter("");
}
void slowPrint(const string& text, int charDelay)
{
	for (const char c : text) {
		cout << c << flush;
		this_thread::sleep_for(chrono::milliseconds(charDelay));
	}
}
string shuffleFirstTurn(Hero& hero1, Hero& hero2) {

	srand(time(0));
	int endLoop = (rand() % 2) + 30;
	for (int i = rand() % 2; i <= endLoop; i++)
	{
		if (i % 2 == 0)
		{
			cout << hero1.getName();
			if (i == endLoop)
			{
				hero1.takeTurn(true);
				hero2.takeTurn(false);
				return "";
			}
			sleep_for(milliseconds(30 - (i * 4)));
			system("CLS");
		}
		else
		{
			cout << hero2.getName();
			if (i == endLoop)
			{
				hero2.takeTurn(true);
				hero1.takeTurn(false);
				return "";
			}
			sleep_for(milliseconds(30 - (i * 4)));
			system("CLS");
		}
	}
}
void WaitEnter(string text)
{
	string temp;
	cout << text;
	cin >> temp;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


//End of code... 
//Created with love by Nour Algendi - nourgaser2012@gmail.com - 2020