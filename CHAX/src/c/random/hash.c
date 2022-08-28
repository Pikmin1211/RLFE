#include "gbafe.h"
#include "random/hash.h"

// credit circles
int HashByte_N(int number, int noise, int max){
    if (max == 0){ 
        return 0;
    }

    u32 hash = 5381;

    SetTacticianNameString();
    u8* TacticianName = GetTacticianNameString();
    for (int i = 0; i < 8; ++i){
        hash = (hash ^ TacticianName[i]) * 33;
    };

    hash = (hash ^ number) * 33;
    hash = (hash ^ noise) * 0x7FFFFFFF;
    return Mod((u16)hash, max);
};

// debug
void SetTacticianNameString(){
    u8* TacticianName = GetTacticianNameString();

    if (TacticianName[0]){
        return;
    }

    for (int i = 0; i < 8; ++i){
        TacticianName[i] = NextRN_N(0xFF);
    };
}