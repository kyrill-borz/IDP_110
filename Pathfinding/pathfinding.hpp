#include<String.h>
#include<Arduino.h>
#include<stdlib.h>

using namespace std;

// Pathfinding: Pass current location and desired location as arguments to GetPathToTarget
const int huge = 10000;
int currentHeading = 0;
class Vector2 {
  public:
    int X,Y;

    Vector2 (int a=0, int b=0) {
      X = a;
      Y = b;
    };

    int operator|| (const Vector2& param) {
      return abs(X-param.X) + abs(Y-param.Y);
    }
};
class Node {
  public:
    int ConnectedNodes[4];
    int Code;
    Vector2 Position;
    int h = huge;
    int parent = -1; 

    Node (int junctCode, Vector2 pos, int n=-1, int s=-1, int e=-1, int w=-1) {
      Code = junctCode;
      Position = pos;
      ConnectedNodes[0] = n;
      ConnectedNodes[1] = s;
      ConnectedNodes[2] = e;
      ConnectedNodes[3] = w;
    }
};

const int numNodes = 16;
Node nodes[numNodes] {
  {0, Vector2(0, 0), 1, -1, -1, -1}, 
  {1, Vector2(0, 13), -1, 0, 5, 2},
  {2, Vector2(-19, 13), 3, -1, 1, 4},
  {3, Vector2(-19, 33), -1, 2, -1, -1},
  {4, Vector2(-63, 13), 6, 12, 2, -1},
  {5, Vector2(63, 13), 10, 13, -1, 1},
  {6, Vector2(-63, 69), 14, 4, 7, -1},
  {7, Vector2(0, 69), 11, -1, 8, 6},
  {8, Vector2(22, 69), -1, 9, 10, 7},
  {9, Vector2(22, 55), 8, -1, -1, -1},
  {10, Vector2(63, 69), 15, 5, -1, 8},
  {11, Vector2(0, 123), -1, 7, 15, 14},
  {12, Vector2(-63, 0), 4, -1, -1, -1},
  {13, Vector2(63, 0), 5, -1, -1, -1},
  {14, Vector2(-63, 123), -1, 6, 11, -1},
  {15, Vector2(63, 123), -1, 10, -1, 11}
};

String GetPathToTarget(int startIndex, int endIndex) {
  bool closedList[numNodes] = {false};
  int currentIndex;

  // reset the nodes
  for (int i = 0; i < numNodes; i++) {
    Node* node = &nodes[i];
    node->h = huge;
    node->parent = -1;
  }
  nodes[startIndex].h = 0;

  while (true) {
    // get the node with lowest h
    int lowest_h = huge;
    for (int i = 0; i < numNodes; i++) {
      if (closedList[i] == true) {continue;}
      Node* node = &nodes[i];
      if (node->h < lowest_h) {
        lowest_h = node->h;
        currentIndex = i;
      }
    }
    closedList[currentIndex] = true;

    // check the surrounding nodes
    Node* currentNode = &nodes[currentIndex];
    for (int i = 0; i < 4; i++) {
      int checkIndex = currentNode->ConnectedNodes[i];
      if (checkIndex == -1) {continue;}
      if (closedList[checkIndex] == true) {continue;}
      Node* checkNode = &nodes[checkIndex];

      // check if this node is the end node
      if (checkIndex == endIndex) {
        checkNode->parent = currentIndex;
        int childIndex = checkIndex;
        String directions = "";

        while (true) {
          Node* childNode = &nodes[childIndex];
          Node* parentNode = &nodes[childNode->parent];

          // get the direction to the child node
          if (parentNode->ConnectedNodes[0] == childIndex) {directions = directions + "0";} 
          else if (parentNode->ConnectedNodes[1] == childIndex) {directions = directions + "1";} 
          else if (parentNode->ConnectedNodes[2] == childIndex) {directions = directions + "2";} 
          else {directions = directions + "3";} 

          // check if the start node has been reached
          if (parentNode->Code == startIndex) {
            String reversed = "";
            for (i = directions.length()-1; i >= 0; i--) {
              reversed = reversed + directions[i];
            }
            return reversed;
          }
          childIndex = parentNode->Code;
        }
      }

      // calculate the h to the node
      // 2 heuistics (dist to next node and dist to final node)
      int toNode_h = currentNode->Position || checkNode->Position;
      toNode_h += checkNode->Position || nodes[endIndex].Position;

      if (toNode_h < checkNode->h) {
        checkNode->h = toNode_h;
        checkNode->parent = currentIndex;
      } 
    }
  }
}
String ConvertToLocalPath(String path) {
  // loop over the path
  int current = currentHeading;
  String LRC = "";
  for (int i = 0; i < path.length(); i++) {
    int targetAngle = 0;
    if (path[i] == '1') {targetAngle = 180;}
    else if (path[i] == '2') {targetAngle = 90;}
    else if (path[i] == '3') {targetAngle = -90;}

    // get the angle delta
    int delta = targetAngle - current;
    delta = delta%360;
    if (abs(delta) > 180) {delta = delta-(360 * (delta/abs(delta)));}

    if (delta == 0) {LRC += "C";}
    else if (delta == 90) {LRC += "R";}
    else if (abs(delta) == 180) {LRC += "B";}
    else if (delta == -90) {LRC += "L";}

    // set the current angle
    current = targetAngle;
  }
  
  return LRC;
}
void SetCurrentHeading(int heading) {
  currentHeading = heading;
  currentHeading = heading%360;
  if (abs(currentHeading) > 180) {currentHeading = currentHeading-(360 * (heading/abs(heading)));}
}

void SetHeadingFromPath(String path) {
    // set the new current heading
  for (int i = 0; i < path.length(); i++) {
    if (path[i] == 'L') {SetCurrentHeading(currentHeading - 90);}
    else if (path[i] == 'R') {SetCurrentHeading(currentHeading + 90);}
    else if (path[i] == 'B') {SetCurrentHeading(currentHeading + 180);}
  }
}


// -- End Of Pathfinding Stuff 

// void setup() 
// {
//   // Serial init
//   Serial.begin(9600);
//   Serial.println(ConvertToLocalPath(GetPathToTarget(0, 9)));
//   SetCurrentHeading(180);
//   Serial.println(ConvertToLocalPath(GetPathToTarget(9, 13)));
// }
// void loop(){}


