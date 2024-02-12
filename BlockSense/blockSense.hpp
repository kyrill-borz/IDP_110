
#include "DFRobot_VL53L0X.h"


bool senseBlockIR(DFRobot_VL53L0X sensor){ //sensing block distance using IR
float blockDistIRAv = 200;
    float blockDistIR = sensor.getDistance();
    if(blockDistIR >= 10.0){
        blockDistIRAv = blockDistIR;
    }    ;
    Serial.print(blockDistIRAv);
    if(blockDistIRAv > 105){//100,100
        return 0;
    }
    else{
        return 1;
    }
}


int blockTypeIR(DFRobot_VL53L0X sensor ){//determing block type using IR
    float blockTypeDist = sensor.getDistance();
    if(blockTypeDist < 95){ // 95 is good distance to differentiate blocks
        return 0; //return 0 for a solid block
    }
    else{
        return 1; //return 1 for a foam block
    }
}
