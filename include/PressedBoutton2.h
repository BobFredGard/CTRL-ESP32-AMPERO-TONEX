void PressedBoutton2 () {
    if (initial_state == 1){
        tonex_count = 0; 
        initial_state = 0;
    }
    if (tonex_count > 0 && tonex_count < 6) {
        tonex_count = tonex_count+1;
    }
    else {tonex_count = 1; ampero_count = 1;}
    init_Encoders(1);
    if (all_parameters[id][tonex_quick_values[1][tonex_count]] > 1390){
        if (all_parameters[id][tonex_quick_values[1][tonex_count]] == 1399) {
            tmp = 1399;
        }
        else {
            tmp = 1400;
        }
    }
    if (all_parameters[id][tonex_quick_values[1][tonex_count]] > 1290 && all_parameters[id][tonex_quick_values[1][tonex_count]] > 1305) {
        if (all_parameters[id][tonex_quick_values[1][tonex_count]] == 1300) {
            tmp = 1300;
        }
        else {
            tmp = 1299;
        }
    }
    if (all_parameters[id][tonex_quick_values[1][tonex_count]] > 1000) {
        tmp = -1;
    }
    if (startscreen != 1) {
        Screens(3, tmp);
    } else {
        startscreen = 0;
    }
    // displayColor1(rgbcolor[tonex_count]);
}