#include "gbafe.h"
#include "random/hash.h"
#include "unit/unit.h"

#define Cavalier 0x5
#define Knight 0x9
#define Mercenary 0xF

const u8 ValidClassList[] = {
    Cavalier,
    Knight,
    Mercenary
};

void UnitInitInventoryFromClass(Unit* unit, const ClassData* class){
    ClearUnitInventory(unit);
    int slot = 0;
    for(int i = 0; i < ITYPE_DARK+1; i++){
        if (slot == UNIT_ITEM_COUNT){
            break;
        }
        if (class->baseRanks[i]){
            switch(i){
                case ITYPE_SWORD:
                    unit->items[slot] = MakeNewItem(0x1);
                    slot++;
                    break;
                case ITYPE_LANCE:
                    unit->items[slot] = MakeNewItem(0x14);
                    slot++;
                    break;
                case ITYPE_AXE:
                    unit->items[slot] = MakeNewItem(0x1F);
                    slot++;
                    break;
                case ITYPE_BOW:
                    unit->items[slot] = MakeNewItem(0x2D);
                    slot++;
                    break;
                case ITYPE_STAFF:
                    unit->items[slot] = MakeNewItem(0x4B);
                    slot++;
                    break;
                case ITYPE_ANIMA:
                    unit->items[slot] = MakeNewItem(0x38);
                    slot++;
                    break;
                case ITYPE_LIGHT:
                    unit->items[slot] = MakeNewItem(0x3F);
                    slot++;
                    break;
                case ITYPE_DARK:
                    unit->items[slot] = MakeNewItem(0x45);
                    slot++;
                    break;
            }
        }
    }
}

void UnitInitFromDefinition(Unit* unit, const UnitDefinition* uDef){
    unit->pCharacterData = GetCharacterData(uDef->charIndex);

    const ClassData* class = GetClassData(ValidClassList[HashByte_N(uDef->classIndex + gChapterData.chapterIndex, NOISE_CLASS, sizeof(ValidClassList))]);

    unit->pClassData = class;
    unit->level = uDef->level;

    UnitInitInventoryFromClass(unit, class);

    GetPreferredPositionForUNIT(uDef, &unit->xPos, &unit->yPos, FALSE);
    CharStoreAI(unit, uDef);
};

void UnitLoadStatsFromChracter(Unit* unit, const CharacterData* character){

    const ClassData* class = unit->pClassData;

    unit->maxHP = character->baseHP + class->baseHP;
    unit->pow   = character->basePow + class->basePow;
    unit->skl   = character->baseSkl + class->baseSkl;
    unit->spd   = character->baseSpd + class->baseSpd;
    unit->def   = character->baseDef + class->baseDef;
    unit->res   = character->baseRes + class->baseRes;
    unit->lck   = character->baseLck;

    unit->maxHP = HashByte_N(unit->maxHP, NOISE_MAXHP, class->maxHP);
    unit->pow = HashByte_N(unit->pow, NOISE_POW, class->maxPow);
    unit->skl = HashByte_N(unit->skl, NOISE_SKL, class->maxSkl);
    unit->spd = HashByte_N(unit->spd, NOISE_SPD, class->maxSpd);
    unit->def = HashByte_N(unit->def, NOISE_DEF, class->maxDef);
    unit->res = HashByte_N(unit->res, NOISE_RES, class->maxRes);
    unit->lck = HashByte_N(unit->lck, NOISE_LCK, 30);

    for (int i = 0; i < 8; ++i) {
        unit->ranks[i] = unit->pClassData->baseRanks[i];

        if (unit->pCharacterData->baseRanks[i])
            unit->ranks[i] = unit->pCharacterData->baseRanks[i];
    }

    if (UNIT_FACTION(unit) == FACTION_BLUE && (unit->level != UNIT_LEVEL_MAX))
        unit->exp = 0;
    else
        unit->exp = UNIT_EXP_DISABLED;
}