
#include "DFRobot_VL53L0X.h"


bool senseBlockIR(DFRobot_VL53L0X sensor){ //sensing block distance using IR
    float blockDistIR = sensor.getDistance();
    if(blockDistIR > 100){
        return 0;
    }
    else{
        return 1;
    }
}


int blockTypeIR(DFRobot_VL53L0X sensor ){//determing block type using IR
    float blockTypeDist = sensor.getDistance();
    if(blockTypeDist < 60){ //calibrate threshold for block identification
        return 0; //return 0 for a solid block
    }
    else{
        return 1; //return 1 for a foam block
    }
}
