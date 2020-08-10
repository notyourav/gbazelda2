#ifndef PLAYER_H
#define PLAYER_H

void SetupPlayer();
void PlayerLoop();

typedef struct {
    Pos pos;
    Pos vel;
} Entity;

#endif