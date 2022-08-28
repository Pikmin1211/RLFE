#include "gbafe.h"
#include "save/save.h"

void PackUnitStructForSuspend(Unit* unit, SuspendUnit* susUnit){

	if(!unit->pCharacterData){
		susUnit->savedUnit.characterNumber = 0;
		return;
	}

	SaveUnitCore(&susUnit->savedUnit, unit);

	susUnit->curHP = unit->curHP;

	susUnit->torchDuration = unit->torchDuration;
	susUnit->barrierDuration = unit->barrierDuration;

	susUnit->ballistaIndex = unit->ballistaIndex;

	susUnit->statusIndex = unit->statusIndex;
	susUnit->statusDuration = unit->statusDuration;

	susUnit->rescueOtherUnit = unit->rescueOtherUnit;

	if (unit->index & FACTION_BLUE){
		SuspendUnitBlue* blueUnit = (SuspendUnitBlue*) susUnit;
		blueUnit->supportBits = unit->supportBits;
	}
	else {
		SuspendUnitRed* redUnit = (SuspendUnitRed*) susUnit;
		redUnit->aiFlag = unit->aiFlag;
		redUnit->ai1 = unit->ai1;
		redUnit->ai2 = unit->ai2;
		redUnit->ai1data = unit->ai1data;
		redUnit->ai2data = unit->ai2data;
		redUnit->ai3And4 = unit->ai3And4;
	}

}

void UnpackUnitStructFromSuspend(void* sramAddr, Unit* unit){

	SuspendUnitRed redUnit;
	ReadSramFast(sramAddr, &redUnit, sizeof(redUnit));
	SuspendUnit* susUnit = (SuspendUnit*) &redUnit;

	if(!susUnit->savedUnit.characterNumber){
		return;
	}

	LoadUnitCore(&susUnit->savedUnit, unit);

	unit->curHP = susUnit->curHP;

	unit->torchDuration = susUnit->torchDuration;
	unit->barrierDuration = susUnit->barrierDuration;

	unit->ballistaIndex = susUnit->ballistaIndex;

	unit->statusIndex = susUnit->statusIndex;
	unit->statusDuration = susUnit->statusDuration;

	unit->rescueOtherUnit = susUnit->rescueOtherUnit;

	if (unit->index & FACTION_BLUE){
		SuspendUnitBlue* blueUnit = (SuspendUnitBlue*) &redUnit;
		unit->supportBits = blueUnit->supportBits;
	}
	else {
		unit->aiFlag = redUnit.aiFlag;
		unit->ai1 = redUnit.ai1;
		unit->ai2 = redUnit.ai2;
		unit->ai1data = redUnit.ai1data;
		unit->ai2data = redUnit.ai2data;
		unit->ai3And4 = redUnit.ai3And4;
	}

}

void SaveUnit(void* sramAddr, Unit* unit){

	SavedUnit savedUnit;
	SaveUnitCore(&savedUnit, unit);
	WriteAndVerifySramFast(sramAddr, &savedUnit, sizeof(savedUnit));

}

void LoadSavedUnit(void* sramAddr, Unit* unit){

	SavedUnit savedUnit;
	ReadSramFast(sramAddr, &savedUnit, sizeof(savedUnit));
	LoadUnitCore(&savedUnit, unit);

}

void SaveUnitCore(SavedUnit* savedUnit, Unit* unit){

	savedUnit->characterNumber = unit->pCharacterData->number;
	savedUnit->classNumber = unit->pClassData->number;

	savedUnit->xPos = unit->xPos;
	savedUnit->yPos = unit->yPos;

	savedUnit->level = unit->level;
	savedUnit->exp = unit->exp;

	savedUnit->maxHP = unit->maxHP;
	savedUnit->fatigue = unit->fatigue;

	savedUnit->pow = unit->pow;
	savedUnit->mag = unit->mag;
	savedUnit->skl = unit->skl;
	savedUnit->spd = unit->spd;
	savedUnit->def = unit->def;
	savedUnit->res = unit->res;
	savedUnit->lck = unit->lck;
	savedUnit->conBonus = unit->conBonus;
	savedUnit->movBonus = unit->movBonus;

	for (int i = 0; i < 6; i++){
		savedUnit->supports[i] = unit->supports[i];
	}
	savedUnit->supports[6] = unit->unitLeader;

	for (int j = 0; j < 8; j++){
		savedUnit->ranks[j] = unit->ranks[j];
	}

	for (int k = 0; k < 5; k++){
		savedUnit->items[k] = unit->items[k];
	}

	savedUnit->state = unit->state;

}

void LoadUnitCore(SavedUnit* savedUnit, Unit* unit){

	unit->pCharacterData = GetCharacterData(savedUnit->characterNumber);
	unit->pClassData = GetClassData(savedUnit->classNumber);

	unit->xPos = savedUnit->xPos;
	unit->yPos = savedUnit->yPos;

	unit->level = savedUnit->level;
	unit->exp = savedUnit->exp;

	unit->maxHP = savedUnit->maxHP;
	unit->fatigue = savedUnit->fatigue;

	unit->pow = savedUnit->pow;
	unit->mag = savedUnit->mag;
	unit->skl = savedUnit->skl;
	unit->spd = savedUnit->spd;
	unit->def = savedUnit->def;
	unit->res = savedUnit->res;
	unit->lck = savedUnit->lck;
	unit->conBonus = savedUnit->conBonus;
	unit->movBonus = savedUnit->movBonus;

	for (int i = 0; i < 6; i++){
		unit->supports[i] = savedUnit->supports[i];
	}
	unit->unitLeader = savedUnit->supports[6];

	for (int j = 0; j < 8; j++){
		unit->ranks[j] = savedUnit->ranks[j];
	}

	for (int k = 0; k < 5; k++){
		unit->items[k] = savedUnit->items[k];
	}

	unit->state = savedUnit->state;

}

/*
void SaveSuspendedGame(int saveType){

}
*/