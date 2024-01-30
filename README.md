# IDP_110

## How to store code

Each function should be in it's own .cpp/.ino file, and then incorporated into the main file, so that unit testing can work

## How the code should work

The Robot Starts in the box. There should be a function that drives and checks if the robot is following the line. At a crossroad, the robot should decide whether to go left, right, or forward. This decision is predetermined by the route the robot is taking (The robot will go to the 2 houses first, and then the two factories). When it gets to the block, it should sense what the block is, pick it up properly, and deliver it to the correct section, by following a predetermined route that gets to either delivery spot. The robot should then execute the return to home function.

## Explanation of functions

Summary of each function

### Line Follower

If line is on the left, go more right. If line is on the right, go more left. Stop every time a junction is detected.

### Path finder

Return an array of junction instructions, and then give the robot that instruction every time a junction is detected.

### Block Pick Up

Close arms on block until the block is sensed, then detect what block it is.

### Deliver the Block

Path finder to one of two locations
