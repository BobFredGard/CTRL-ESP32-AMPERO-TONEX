void ScaleValueSwitch (byte val3) {
  view_tonex_value = (valminimax[1][global_count]-valminimax[0][global_count])*all_parameters[id][global_count]/global_control_change_max_values[global_count]+valminimax[0][global_count];
  switch (val3){
  case 1 :
    LCD.setCursor(12,1); LCD.print("    "); 
    if (view_tonex_value >=0 && view_tonex_value < 10) {LCD.setCursor(13,1);}
    if (view_tonex_value > 9.99 && view_tonex_value <= 100) {LCD.setCursor(12,1);}
    if (view_tonex_value < 0 && view_tonex_value < -100) {LCD.setCursor(13,1);}
    if (view_tonex_value < 0 && view_tonex_value >= -100) {LCD.setCursor(12,1);}
    LCD.print((valminimax[1][global_count]-valminimax[0][global_count])*all_parameters[id][global_count]/global_control_change_max_values[global_count]+valminimax[0][global_count]);
  break;
  case 2 :
    LCD.setCursor(12,1); LCD.print("    "); 
    if (view_tonex_value >=0 && view_tonex_value < 10) {LCD.setCursor(14,1);}
    if (view_tonex_value >=0 && view_tonex_value > 9 && view_tonex_value < 100) {LCD.setCursor(13,1);}
    if (view_tonex_value >=0 && view_tonex_value >= 100) {LCD.setCursor(12,1);}
    LCD.print(round((valminimax[1][global_count]-valminimax[0][global_count])*all_parameters[id][global_count]/global_control_change_max_values[global_count]+valminimax[0][global_count]));
    LCD.setCursor(15,1); LCD.print("%"); 
  break;
  case 3 :
    LCD.setCursor(9,1); LCD.print("       "); LCD.setCursor(9,1); 
    if (all_parameters[id][global_count] >= 0 && all_parameters[id][global_count] < 2) {LCD.print(tonex_reverb_names[0]);}
    if (all_parameters[id][global_count] >= 2 && all_parameters[id][global_count] < 4) {LCD.print(tonex_reverb_names[1]);}
    if (all_parameters[id][global_count] >= 4 && all_parameters[id][global_count] < 6) {LCD.print(tonex_reverb_names[2]);}
    if (all_parameters[id][global_count] >= 6 && all_parameters[id][global_count] < 8) {LCD.print(tonex_reverb_names[3]);}
    if (all_parameters[id][global_count] >= 8 && all_parameters[id][global_count] < 10) {LCD.print(tonex_reverb_names[4]);}
  break;
  }
}