u8* GetTacticianNameString();
int Mod(int numerator, int denometer);

enum noises{
    NOISE_NONE,
    NOISE_CLASS,
    NOISE_MAXHP,
    NOISE_POW,
    NOISE_SKL,
    NOISE_SPD,
    NOISE_DEF,
    NOISE_RES,
    NOISE_LCK,
    NOISE_CHEST
};

int HashByte_N(int number, int noise, int max);
void SetTacticianNameString();
