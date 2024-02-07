
#include "DFRobot_VL53L0X.h"


bool senseBlockIR(DFRobot_VL53L0X sensor){ //sensing block distance using IR
    float blockDistIR = sensor.getDistance();
    if(blockDistIR > 80){
        return 0;
    }
    else{
        return 1;
    }
}


void blockTypeIR(DFRobot_VL53L0X sensor){//determing block type using IR
    float blockTypeDist = sensor.getDistance();
    if(blockTypeDist < 60){ //calibrate threshold for block identification
        isFoam = 0; //return 0 for a solid block
    }
    else{
        isFoam = 1; //return 1 for a foam block
    }
}
