                                   //  |--------CC----------|      |--Positions dans BD--|
static short ampero_quick_values [2] [7] = {{-10, 7, 16, 18, 20, 13, 59}, {0, 17, 14, 15, 16, 13, 12}};
static short tonex_quick_values [2] [7] = {{-10, 103, 102, 19, 15, 106, 107}, {0, 27, 18, 23, 24, 25, 26}};

static const char* ampero_quick_names [7]  {"", "VOLUME     : ", "PARAM 1     : ", "PARAM 2    : ", "PARAM 3    : ", "PEDAL EXP  : ", "REVERB ON  : "};
static const char*  tonex_quick_names [7] {"", "MODEL.VOL  : ", "GAIN       : ", "COMPRESSOR : ", "GATE       : ", "PRESENCE   : ", "DEPHT      : "};
static short values_max_ampero_quick [7] = {0,100, 127, 127, 127, 1300, 1300};
static short values_mini_ampero_quick [7] = {0,0, 0, 0, 0, 1299, 1299};
static short values_max_tonex_quick [7] = {0,127, 127, 127, 127, 127, 127};
static short values_mini_tonex_quick [7] = {0,0, 0, 0, 0, 0, 0};


static const char* global_names [59] = {  "", 
  "SLOT A1    : ", "SLOT A2    : ", "SLOT A3    : ", "SLOT A4    : ", "SLOT A5    : ", "SLOT A6    : ",
  "SLOT B1    : ", "SLOT B2    : ", "SLOT B3    : ", "SLOT B4    : ", "SLOT B5    : ", "SLOT B6    : ",
  "PEDAL EXP  : ", "PARAM 1    : ", "PARAM 2    : ", "PARAM 3    : ", "VOLUME     : ",
  "GAIN       : ", "BASS       : ", "MID        : ", "TREBLE     : ", "REVERB     : ", "COMPRESSOR : ",
  "NOISE GATE : ", "PRESENCE   : ", "DEPTH      : ", "MODEL.VOL  : ", "NOISE GT   : ", "NG REL     : ",
  "NG DPTH    : ", "COMP       : ", "CMP GAIN   : ", "COMP ATK   : ", "COMP PATC  : ",
  "BASS FRQ   : ", "MID Q      : ", "MID FRQ    : ", "TRBL FRQ   : ", "EQ PATCH   : ", "EQ MIX     : ",
  "REVERB     : ", "REV TYPE   : ", "REV TIME   : ", "VB PREDELAY: ", "REV COLOR  : ", "REV RESO   : ",
  "MIC 1      : ", "MIC 1 X    : ", "MIC 1 Z    : ", "MIC 2      : ", "MIC 2 X    : ", "MIC 2 Z    : ", "MICS MIX   : ",
  "NB FX MAX  : ", "PC Tone X  : ", "PC Ampero  : ", "STOMP 1 VAL: ", "STOMP 2 VAL: "
};

static short global_control_change_max_values [57] = {0,
  1300,1300,1300,1300,1300,1300,
  1300,1300,1300,1300,1300,1300,
  1300,127,127,127,100,
  127,127,127,127,127,127,
  127,127,127,127,1400,127,
  127,1300,127,127,1400,
  127,127,127,127,1400,127,
  1300,9,127,127,127,127,
  127,127,127,127,127,127,127,
  61,149,300
};

static short id_values_for_6_foot [37] = {0,1,8,15,22,29,36,43,50,57,64,71,78,85,92,99,106,113,120,127,134,141,148,155,162,169,176,183,190,197,204,211,218,225,232,239,246};

static const char* tonex_reverb_names [5] = {"SPRING1", "SPRING2", "SPRING3", "ROOM ", "PLATE"};

static float valminimax [2] [57] = {
  {0,
    0,0,0,0,0,0,
    0,0,0,0,0,0,
    0,0,0,0,100,
    0,0,0,0,0,0,
    -100,0,0,0,1400,20,
    -20,1300,-30,1,1400,
    75,0.2,150,1000,1400,0,
    1300,1,0,0,0,0,
    0,0,0,0,0,0,0,
    61,150,300
  },
  {0,
    0,0,0,0,0,0,
    0,0,0,0,0,0,
    0,0,0,0,100,
    10,10,10,10,100,-40,
    0,10,10,10,1400,500,
    -100,1300,10,51,1400,
    600,3,5000,4000,1400,100,
    0,5,127,127,127,127,
    127,127,127,127,127,127,127,
    61,150,300
  }
};