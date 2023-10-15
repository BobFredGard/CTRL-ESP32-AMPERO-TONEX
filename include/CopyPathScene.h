void CopyPathScene () {
    String text2;
    if (calculPathScene  [0] [encoder1_copy_source] == encoder1_copy_source) {
        Serial.println(encoder1_copy_source);
        tx2 = calculPathScene [2] [calculPathScene [0] [encoder1_copy_source]];
        tx3 = calculPathScene [1] [calculPathScene [0] [encoder1_copy_source]] - 1;
    }
    else {
        Serial.println(encoder1_copy_source);
        tx3 = calculPathScene [1] [calculPathScene [0] [encoder1_copy_source]] - 1;
    }
    LCD.setCursor(13,0);
    text2 = String(tx2) + "-" + String(tx3);
    LCD.print(text2);
}
