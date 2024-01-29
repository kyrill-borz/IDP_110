// Pathfinding: Pass current location and desired location as arguments to GetPathToTarget
const int huge = 10000;
class Vector2 {
public:
    int X, Y;

    Vector2(int a = 0, int b = 0) {
        X = a;
        Y = b;
    };

    int operator|| (const Vector2& param) {
        return abs(X - param.X) + abs(Y - param.Y);
    }
};
class Node {
public:
    int ConnectedNodes[4];
    int Code;
    Vector2 Position;
    int h = huge;
    int parent = -1;

    Node(int junctCode, Vector2 pos, int n = -1, int s = -1, int e = -1, int w = -1) {
        Code = junctCode;
        Position = pos;
        ConnectedNodes[0] = n;
        ConnectedNodes[1] = s;
        ConnectedNodes[2] = e;
        ConnectedNodes[3] = w;
    }
};

const int numNodes = 16;
Node nodes[numNodes]{
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
    bool closedList[numNodes] = { false };
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
            if (closedList[i] == true) { continue; }
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
            if (checkIndex == -1) { continue; }
            if (closedList[checkIndex] == true) { continue; }
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
                    if (childIndex != 14 && childIndex != 15) {
                        if (parentNode->ConnectedNodes[0] == childIndex) {
                            directions = directions + "N";
                        }
                        else if (parentNode->ConnectedNodes[1] == childIndex) {
                            directions = directions + "S";
                        }
                        else if (parentNode->ConnectedNodes[2] == childIndex) {
                            directions = directions + "E";
                        }
                        else {
                            directions = directions + "W";
                        }
                    }

                    // check if the start node has been reached
                    if (parentNode->Code == startIndex) {
                        String reversed = "";
                        for (i = directions.length() - 1; i >= 0; i--) {
                            reversed = reversed + directions[i];
                        }
                        return reversed;
                    }

                    childIndex = parentNode->Code;
                }
            }

            // calculate the h to the node
            int toNode_h = currentNode->Position || checkNode->Position;
            if (toNode_h < checkNode->h) {
                checkNode->h = toNode_h;
                checkNode->parent = currentIndex;
            }
        }
    }
}
// -- End Of Pathfinding Stuff 

void setup()
{
    // Serial init
    Serial.begin(9600);
    Serial.println("Follow Directions: " + GetPathToTarget(1, 13));
    Serial.println("Follow Directions: " + GetPathToTarget(11, 13));
    Serial.println("Follow Directions: " + GetPathToTarget(13, 9));
}
void loop()
{

}
