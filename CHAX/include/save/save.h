
typedef struct SuspendUnit SuspendUnit;
typedef struct SavedUnit SavedUnit;
typedef struct SuspendUnitBlue SuspendUnitBlue;
typedef struct SuspendUnitRed SuspendUnitRed;

enum{
	SAVE_SLOT_1    = 0x0,
	SAVE_SLOT_2    = 0x1,
	SAVE_SLOT_3    = 0x2,
	SUSPEND_SAVE   = 0x3,
	SUSPEND_SAVE_2 = 0x4,
	LINK_ARENA     = 0x5,
	SAVE_MAP       = 0x6
};

struct __attribute__((packed, aligned(4))) SavedUnit {
	/* 0x00 */ u8 characterNumber;
	/* 0x01 */ u8 classNumber;
	/* 0x02 */ u8 maxHP;
	/* 0x03 */ u8 pow;
	/* 0x04 */ u8 mag;
	/* 0x05 */ u8 skl;
	/* 0x06 */ u8 spd;
	/* 0x07 */ u8 def;
	/* 0x08 */ u8 res;
	/* 0x09 */ u8 lck;
	/* 0x0A */ u8 ranks[8];
	/* 0x12 */ u16 items[5];
	/* 0x1C */ u32 xPos : 6;
			   u32 yPos : 6;
			   u32 level : 7;
			   u32 exp : 7;
			   u32 fatigue : 6;
	/* 0x20 */ u32 state;
	/* 0x24 */ u8 supports[7];
	/* 0x2B */ u8 conBonus : 4;
			   u8 movBonus : 4;
};

struct __attribute__((packed, aligned(4))) SuspendUnit{
	SavedUnit savedUnit;
	/* 0x2C */ u8 curHP;
	/* 0x2D */ u8 rescueOtherUnit;
	/* 0x2E */ u8 ballistaIndex;
	/* 0x2F */ u8 statusIndex : 4;
			   u8 statusDuration : 4;
	/* 0x30 */ u8 torchDuration : 4;
			   u8 barrierDuration : 4;
};

struct __attribute__((packed, aligned(4))) SuspendUnitBlue{
	SuspendUnit suspendUnit;
	/* 0x31 */ u8 supportBits;
};

struct __attribute__((packed, aligned(4))) SuspendUnitRed{
	SuspendUnit suspendUnit;
	/* 0x31 */ u8 aiFlag;
	/* 0x32 */ u8 ai1;
	/* 0x33 */ u8 ai2;
	/* 0x34 */ u8 ai1data;
	/* 0x35 */ u8 ai2data;
	/* 0x36 */ u16 ai3And4;
};

void PackUnitStructForSuspend(Unit* unit, SuspendUnit* susUnit);
void UnpackUnitStructFromSuspend(void* sramAddr, Unit* unit);
void SaveUnit(void* sramAddr, Unit* unit);
void LoadSavedUnit(void* sramAddr, Unit* unit);
void SaveUnitCore(SavedUnit* savedUnit, Unit* unit);
void LoadUnitCore(SavedUnit* savedUnit, Unit* unit);