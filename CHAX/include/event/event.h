typedef struct EventInfo EventInfo;
typedef struct ChestEventDefinition ChestEventDefinition;

typedef u32 EventTriggerScript;
typedef u32 EventScript;

struct EventInfo {
    /* 00 */ EventTriggerScript* triggerScript;
    /* 04 */ EventScript* script;
    /* 08 */ u32 eId;
    /* 0C */ u32 commandId;
    /* 10 */ u32 unk_10;
    /* 14 */ u32 unk_14;
    /* 18 */ s8 xPos;
    /* 19 */ s8 yPos;
    /* 1A */ u8 unk_1A;
    /* 1B */ u8 unk_1B;
};

struct ChestEventDefinition {
    /* 00 */ u16 eventType; // 0700
    /* 02 */ u16 eId;
    /* 04 */ u16 item;
    /* 06 */ u16 unk_06;
    /* 08 */ u8 xPos;
    /* 09 */ u8 yPos;
    /* 0A */ u16 commandId; // 1400
};

int EvCheck07_CHES(EventInfo* info);
