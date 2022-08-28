typedef struct UnitDefinition UnitDefinition;

void GetPreferredPositionForUNIT(const struct UnitDefinition* uDef, s8* xOut, s8* yOut, s8 findNearest);
void CharStoreAI(struct Unit* unit, const struct UnitDefinition* uDef);

void UnitInitInventoryFromClass(Unit* unit, const ClassData* class);
void UnitInitFromDefinition(Unit* unit, const UnitDefinition* uDef);
void UnitLoadStatsFromCharacter(Unit* unit, const CharacterData* character);
