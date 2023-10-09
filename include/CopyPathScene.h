/*static byte calculPathScene [3] [43]{
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42},
    {0,1,2,3,4,5,6,7,1,2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7},
    {0,1,1,1,1,1,1,1,2,2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6}
};*/


void CalculPathCopy (byte cpt) {
    if (calculPathScene  [0] [encoder1_copy_source - cpt] == encoder1_copy_source - cpt) {
        Serial.println(encoder1_copy_source);
        tx2 = calculPathScene [2] [calculPathScene [0] [encoder1_copy_source - cpt]];
        tx3 = calculPathScene [1] [calculPathScene [0] [encoder1_copy_source - cpt]];
    }
    else {
        Serial.println(encoder1_copy_source);
        tx3 = calculPathScene [1] [calculPathScene [0] [encoder1_copy_source - cpt]];
    }
}

void CopyPathScene () {
    String text2;
    if (bank == 1) {
        CalculPathCopy (0);
    }
    if (bank == 2) {
        CalculPathCopy (42);
    }
    if (bank == 3) {
        CalculPathCopy (42*2);
    }
    if (bank == 4) {
        CalculPathCopy (42*3);
    }
    if (bank == 5) {
        CalculPathCopy (42*4);
    }
    if (bank == 6) {
        CalculPathCopy (42*5);
    }
    
    LCD.setCursor(13,0);
    text2 = String(tx2) + "-" + String(tx3);
    LCD.print(text2);
}