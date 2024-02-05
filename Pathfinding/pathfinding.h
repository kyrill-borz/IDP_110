#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <Arduino.h>

String ConvertToLocalPath(String path);
String GetPathToTarget(int startIndex, int endIndex);
void SetHeadingFromPath(String path);
void SetCurrentHeading(int heading);
#endif
