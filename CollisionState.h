//
//  CollisionState.h
//  COMP371_Labs
//
//  Created by Kenny on 2020-04-12.
//  Copyright © 2020 Concordia. All rights reserved.
//

class CollisionState {
public:
    bool hasWallCollision = false;
    vec3 collidedPosition;
};

class FloorNavigate {
public:
    bool hasEnteredElevator = false;
    int floor = 1;
};
