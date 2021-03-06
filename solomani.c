/******************************************************************************
*
* project name: Mongoose Traveller Solomani Name Generator
* file name:    solomani.c
* initial date: 01/11/2014
* author:       Shawn Driscoll
* email:        shawndriscoll@hotmail.com
*
* Compile with TI-GCC for execution on a TI-89
*
******************************************************************************/

#include <tigcclib.h>

int _ti92plus;
int _ti89;

// Characteristic values/indexes

int i, j, k, l;                 // loop counters
int proper, building;           // boolean use
int temp;

int c_Str = 0;
int c_Dex = 1;
int c_End = 2;
int c_Int = 3;
int c_Edu = 4;
int c_Soc = 5;

int characteristic[6];
char characteristic_name[6][4] = {"STR","DEX","END","INT","EDU","SOC"};
int characteristic_dm[] = {-3,-2,-2,-1,-1,-1,0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5};

char hex_code[25] = {"0123456789ABCDEFGHIJKL"};

char sex[7];

//  Sound Tables

int V   = 1;
int CV  = 2;
int VC  = 3;
int CVC = 4;
int CC  = 5;

char ic_sound[45][3] = {"b","bl","br","c","ch","cl","cr","d","dr","f","fl","fr","g","gl",
                        "gr","h","j","k","kl","l","m","n","p","ph","pl","pr","qu",
                        "r","s","sc","sh","sl","sk","st","sv",
                        "t","th","tr","tw","v","w","wr","x","z","zh"};
int ic_freq[45] = {10,3,5,7,6,5,6,10,8,7,3,4,4,3,4,7,7,5,4,10,9,9,10,5,
                    2,5,2,9,11,2,9,10,7,9,2,7,6,5,5,3,3,4,1,3,1};

char v_sound[8][3] = {"a","ai","e","ea","au","i","o","u"};
int v_freq[8] = {6,2,7,2,2,8,6,4};

char mc_sound[12][3] = {"kk","lf","lg","ll","ls","nn","ns","pp","rr","ss","tt","zz"};
int mc_freq[12] = {2,4,7,11,8,11,7,3,4,3,2,1};

char fc_sound[107][4] = {"bdi","ch","che","chi","chs","ck","ct","d","da","dai",
                         "dal","day","di","dro","dy","f","ft","g","gal","gh",
                         "ghi","ght","hai","hdi","hi","hn","hr","j","k","ke",
                         "ko","l","le","lf","li","lla","lle","ll","lt","ltz",
                         "lly","lm","los","m","man","mm","mma","mms","mmy","mon",
                         "n","nas","ndi","ndy","ne","nes","nez","ng","ni","nk",
                         "nn","nne","nny","nos","ns","nt","nuz","p","ph","que",
                         "r","rc","rie","rk","rl","rly","rr","rt","rx","ry",
                         "s","sen","sha","shi","ski","son","ss","st","t","th",
                         "ti","tti","tts","tz","v","w","wdi","x","xx","yy",
                         "z","za","ze","zo","zy","zyx","zz"};
int fc_freq[107] = {3,3,1,5,2,5,2,8,5,2,7,3,5,7,5,4,4,9,3,5,2,3,2,2,4,
                    6,2,6,9,4,3,10,4,5,3,4,3,6,3,2,4,6,5,9,4,2,2,2,5,6,
                   10,4,7,5,2,7,7,5,7,9,5,3,3,7,5,7,3,7,5,2,9,5,3,8,7,
                    4,3,7,2,8,14,5,2,3,6,7,2,7,6,5,3,6,4,3,3,8,2,2,1,1,
                    1,3,1,2,3,1,1};

int syllable_type[15] = {1,1,1,1,1,3,3,2,2,2,2,4,4,4,5};

char ic_sounds[257][3] = {"  "};
char v_sounds[37][3] = {"  "};
char mc_sounds[63][3] = {"  "};
char fc_sounds[484][4] = {"   "};

char word[64];
char first_name[64];
char last_name[64];
int syllable;
int letter;
int looping = TRUE;

int die_roll(int die)
{
    return random(die) + 1;
}

int roll(char dice[10])
{
    int value;
    int i;

    // String has to be 3 or 5 in length!
    if (strlen(dice) == 3 || ((strlen(dice) == 5) && (dice[3] == 43 || dice[3] == 45)))
    {
        if (dice[0] == 68)                                  // "D" is at beginning of roll string?
        {
            if (dice[1] == 54 && dice[2] == 54 && strlen(dice) == 3) // D66 rolled?
            {
                return die_roll(6) * 10 + die_roll(6);      // 11 - 66, no DMs allowed
            }
            if (dice[1] == 48 && dice[2] == 48)             // D00 rolled?
            {
                value = (die_roll(10) - 1) * 10 + die_roll(10);     // 1 - 100
                if (dice[3] == 43 || dice[3] == 45)         // Is there a +/- DM to apply?
                {
                    if (dice[3] == 43)                      // A +DM?
                    {
                        value += dice[4] - 48;
                    }
                    else                                    // A -DM?
                    {
                        value += -(dice[4] - 48);
                    }
                }
                return value;
            }
            if (dice[1] == 49 && dice[2] == 48)             // D10 rolled?
            {
                value = die_roll(10);                       // 1 - 10
                if (dice[3] == 43 || dice[3] == 45)         // Is there a +/- DM to apply?
                {
                    if (dice[3] == 43)                      // A +DM?
                    {
                        value += dice[4] - 48;
                    }
                    else                                    // A -DM?
                    {
                        value += -(dice[4] - 48);
                    }
                }
                return value;
            }
            if (dice[1] == 50 && dice[2] == 48)             // D20 rolled?
            {
                value = die_roll(20);                       // 1 - 20
                if (dice[3] == 43 || dice[3] == 45)         // Is there a +/- DM to apply?
                {
                    if (dice[3] == 43)                      // A +DM?
                    {
                        value += dice[4] - 48;
                    }
                    else                                    // A -DM?
                    {
                        value += -(dice[4] - 48);
                    }
                }
                return value;
            }
            if (dice[1] == 51 && dice[2] == 48)             // D30 rolled?
            {
                value = die_roll(30);                       // 1 - 30
                if (dice[3] == 43 || dice[3] == 45)         // Is there a +/- DM to apply?
                {
                    if (dice[3] == 43)                      // A +DM?
                    {
                        value += dice[4] - 48;
                    }
                    else                                    // A -DM?
                    {
                        value += -(dice[4] - 48);
                    }
                }
                return value;
            }
            if (dice[1] == 49 && dice[2] == 50)             // D12 rolled?
            {
                value = die_roll(12);                       // 1 - 12
                if (dice[3] == 43 || dice[3] == 45)         // Is there a +/- DM to apply?
                {
                    if (dice[3] == 43)                      // A +DM?
                    {
                        value += dice[4] - 48;
                    }
                    else                                    // A -DM?
                    {
                        value += -(dice[4] - 48);
                    }
                }
                return value;
            }
        }
        else
        {
            if (dice[1] == 68)                              // "D" is in middle of roll string?
            {
                if (dice[0] >= 49 && dice[0] <= 57 && (dice[2] == 52 || dice[2] == 54 || dice[2] == 56))
                {                                           // How many dice, and how many sides?
                    value = 0;
                    for (i = 1; i < dice[0] - 47; i++)      // Roll the number of dice
                        value += die_roll(dice[2] - 48);    // Add this die roll type to the total
                    if (dice[3] == 43 || dice[3] == 45)     // Is there a +/- DM to apply?
                    {
                        if (dice[3] == 43)                  // A +DM?
                        {
                            value += dice[4] - 48;
                        }
                        else                                // A -DM?
                        {
                            value += -(dice[4] - 48);
                        }
                    }
                    return value;
                }
            }
        }
    }
    printf("** DICE ERROR! **\n'%s' is unknown ", dice);
    return 0;
}


// -------------------------------------------------------------------------
//  START PROGRAM

void _main(void)
{

//  Make name

    k = 0;
    for (i = 0; i < 45; i++)
        for (j = 0; j < ic_freq[i]; j++)
        {
            for (l = 0; l < 4; l++)
                ic_sounds[k][l] = ic_sound[i][l];
            printf("%s ",ic_sounds[k]);
            k++;
        }
//  ngetchx();
    k = 0;
    for (i = 0; i < 8; i++)
        for (j = 0; j < v_freq[i]; j++)
        {
            for (l = 0; l < 4; l++)
                v_sounds[k][l] = v_sound[i][l];
            printf("%s ",v_sounds[k]);
            k++;
        }
//  ngetchx();
    k = 0;
    for (i = 0; i < 12; i++)
        for (j = 0; j < mc_freq[i]; j++)
        {
            for (l = 0; l < 4; l++)
                mc_sounds[k][l] = mc_sound[i][l];
            printf("%s ",mc_sounds[k]);
            k++;
        }
//  ngetchx();
    k = 0;
    for (i = 0; i < 107; i++)
        for (j = 0; j < fc_freq[i]; j++)
        {
            for (l = 0; l < 5; l++)
                fc_sounds[k][l] = fc_sound[i][l];
            printf("%s ",fc_sounds[k]);
            k++;
        }
//  ngetchx();

    randomize();

    while (looping)
    {
    //  roll characteristics

        for (i = 0; i < 6; i++)
            characteristic[i] = roll("2D6");
    //  characteristic[c_Soc] = 12;

        proper = FALSE;
        while (!proper)
        {
            word[0] = 0;
            temp = CC;
            while (temp == CC)
                temp = syllable_type[random(15)];
            if (temp == V)
                strcat(word,v_sounds[random(37)]);
                //sound = v_sounds[random(114)][3];
            if (temp == CV)
                strcat(strcat(word,ic_sounds[random(257)]),v_sounds[random(37)]);
                //sound = ic_sounds[random(789)] + v_sounds[random(114)];
            if (temp == VC)
                strcat(strcat(word,v_sounds[random(37)]),fc_sounds[random(484)]);
                //sound = v_sounds[random(114)] + fc_sounds[random(1465)];
            if (temp == CVC)
                strcat(strcat(strcat(word,ic_sounds[random(257)]),v_sounds[random(37)]),fc_sounds[random(484)]);
                //sound = ic_sounds[random(789)] + v_sounds[random(114)] + fc_sounds[random(1465)];
            if (temp == CC)
                strcat(word,mc_sounds[random(63)]);
                //sound = mc_sounds[random(176)];
            building = TRUE;
            while (building)
            {
                syllable = syllable_type[random(15)];
                while (temp == CC && (syllable == CV || syllable == CVC || syllable == CC))
                    syllable = syllable_type[random(15)];
                while (temp == V && (syllable == V || syllable == VC))
                    syllable = syllable_type[random(15)];
                while (temp == CV && (syllable == V || syllable == VC))
                    syllable = syllable_type[random(15)];
                while (temp == VC && (syllable == CV || syllable == CVC || syllable == CC))
                    syllable = syllable_type[random(15)];
                while (temp == CVC && (syllable == CV || syllable == CVC || syllable == CC))
                    syllable = syllable_type[random(15)];
                if (temp == VC || temp == CVC)
                    building = FALSE;
                else
                {
                    if (syllable == V)
                        strcat(word,v_sounds[random(37)]);
                        //sound = v_sounds[random(114)][3];
                    if (syllable == CV)
                        strcat(strcat(word,ic_sounds[random(257)]),v_sounds[random(37)]);
                        //sound = ic_sounds[random(789)] + v_sounds[random(114)];
                    if (syllable == VC)
                        strcat(strcat(word,v_sounds[random(37)]),fc_sounds[random(484)]);
                        //sound = v_sounds[random(114)] + fc_sounds[random(1465)];
                    if (syllable == CVC)
                        strcat(strcat(strcat(word,ic_sounds[random(257)]),v_sounds[random(37)]),fc_sounds[random(484)]);
                        //sound = ic_sounds[random(789)] + v_sounds[random(114)] + fc_sounds[random(1465)];
                    if (syllable == CC)
                        strcat(word,mc_sounds[random(63)]);
                        //sound = mc_sounds[random(176)];
                    temp = syllable;
                }
            }
            if (strlen(word) > 2 && strlen(word) < 10)
                proper = TRUE;
        }

    //  clear the screen
        clrscr();

    //  First name

        first_name[0] = 0;
        letter = word[0];
        letter = letter - 32;
        word[0] = letter;

        strcat(first_name,word);

        printf("%s ",first_name);

        sex[0] = 0;

        if (word[0] == 65 || word[0] == 69 || word[0] == 73 || word[0] == 85 \
               || word[0] == 89 || word[strlen(word) - 1] == 97 || word[strlen(word) - 1] == 105)
            strcat(sex,"Female");
        else
            strcat(sex,"Male");

        proper = FALSE;
        while (!proper)
        {
            word[0] = 0;
            temp = CC;
            while (temp == CC)
                temp = syllable_type[random(15)];
            if (temp == V)
                strcat(word,v_sounds[random(37)]);
                //sound = v_sounds[random(114)][3];
            if (temp == CV)
                strcat(strcat(word,ic_sounds[random(257)]),v_sounds[random(37)]);
                //sound = ic_sounds[random(789)] + v_sounds[random(114)];
            if (temp == VC)
                strcat(strcat(word,v_sounds[random(37)]),fc_sounds[random(484)]);
                //sound = v_sounds[random(114)] + fc_sounds[random(1465)];
            if (temp == CVC)
                strcat(strcat(strcat(word,ic_sounds[random(257)]),v_sounds[random(37)]),fc_sounds[random(484)]);
                //sound = ic_sounds[random(789)] + v_sounds[random(114)] + fc_sounds[random(1465)];
            if (temp == CC)
                strcat(word,mc_sounds[random(63)]);
                //sound = mc_sounds[random(176)];
            building = TRUE;
            while (building)
            {
                syllable = syllable_type[random(15)];
                while (temp == CC && (syllable == CV || syllable == CVC || syllable == CC))
                    syllable = syllable_type[random(15)];
                while (temp == V && (syllable == V || syllable == VC))
                    syllable = syllable_type[random(15)];
                while (temp == CV && (syllable == V || syllable == VC))
                    syllable = syllable_type[random(15)];
                while (temp == VC && (syllable == CV || syllable == CVC || syllable == CC))
                    syllable = syllable_type[random(15)];
                while (temp == CVC && (syllable == CV || syllable == CVC || syllable == CC))
                    syllable = syllable_type[random(15)];
                if (temp == VC || temp == CVC)
                    building = FALSE;
                else
                {
                    if (syllable == V)
                        strcat(word,v_sounds[random(37)]);
                        //sound = v_sounds[random(114)][3];
                    if (syllable == CV)
                        strcat(strcat(word,ic_sounds[random(257)]),v_sounds[random(37)]);
                        //sound = ic_sounds[random(789)] + v_sounds[random(114)];
                    if (syllable == VC)
                        strcat(strcat(word,v_sounds[random(37)]),fc_sounds[random(484)]);
                        //sound = v_sounds[random(114)] + fc_sounds[random(1465)];
                    if (syllable == CVC)
                        strcat(strcat(strcat(word,ic_sounds[random(257)]),v_sounds[random(37)]),fc_sounds[random(484)]);
                        //sound = ic_sounds[random(789)] + v_sounds[random(114)] + fc_sounds[random(1465)];
                    if (syllable == CC)
                        strcat(word,mc_sounds[random(63)]);
                        //sound = mc_sounds[random(176)];
                    temp = syllable;
                }
            }
            if (strlen(word) > 2 && strlen(word) < 14)
                proper = TRUE;
        }

    //  Last name

        last_name[0] = 0;
        letter = word[0];
        letter = letter - 32;
        word[0] = letter;

        strcat(last_name,word);

        printf("%s\n\n",last_name);

    //  Print character

        printf("UPP: [");
        for (i = 0; i < 6; i++)
            printf("%c", hex_code[characteristic[i]]);
        printf("]  Sex: %s\n\n", sex);

        for (i = 0; i < 3; i++)
            printf("%s:%2d  ", characteristic_name[i], characteristic[i]);
        printf("\n");

        for (i = 0; i < 3; i++)
            printf("Mod %2d  ", characteristic_dm[characteristic[i]]);
        printf("\n\n");

        for (i = 3; i < 6; i++)
            printf("%s:%2d  ", characteristic_name[i], characteristic[i]);
        printf("\n");

        for (i = 3; i < 6; i++)
            printf("Mod %2d  ", characteristic_dm[characteristic[i]]);

        ST_showHelp("Programmed by shawndriscoll@hotmail.com");
    //  wait for a key press before the program exits
        if (ngetchx () == 13)
            looping = FALSE;
    }
}
