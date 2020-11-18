#pragma once

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

