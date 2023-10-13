static int i, j, k, l, m, tmp, choiced_foot_on_pacer, tmp2 = 0;
static int data_base_choice, old_bank, menus, stomp = 0; 
static int id, id_init, idcopy, global_count, startscreen, bank, ampero_count, tonex_count = 1;
static int initial_state, boutton_choice, encoder1_copy_source, encoder2_copy_destination = 1;
static short canal_midi_2 = 7; static short canal_midi_1 = 8;
static int preid = -1;
static int ampero_bank_choice, tonex_bank_choice = 0;
static byte tx1, tx2, tx3 = 1;
static byte tmpstomp1, tmpstomp2 = 0;

unsigned long previousMillis = 0;
unsigned long interval = 3000;
float view_tonex_value, all_data_base_parameters;

static int all_parameters [43][ 60];
static int all_data_base_parameters_copy [43] [60];
static int temp_transfert_datas [60];

static const char* line1_lcd_text;
static const char* line2_lcd_text;
static String slq_request;

static byte calculPathScene [3] [43]{
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42},
    {0,1,2,3,4,5,6,7,1,2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7},
    {0,1,1,1,1,1,1,1,2,2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6}
};

static short global_control_change [54] =  {
  //  |------CC------>>> AMPERO II STOMPS <<<-------CC-----------------| |------------CC------------>>> TONE X <<<--------------------------CC---------------------------------------------------------------------------------------------|
   0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 13, 16, 18, 20, 7, 102, 23, 25, 28, 79, 19, 15, 106, 107, 103, 14, 16, 17, 18, 20, 21, 22, 24, 26, 27, 29, 30, 104, 75, 85, 76, 77, 78, 108, 109, 110, 111, 112, 113, 114, 115};
//{0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,  25,  26,  27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,  40, 41, 42, 43, 45, 45,  46,  47,  48,  49,  50,  51,  52,  53}
  //  Numéro colonne de la base de donnée