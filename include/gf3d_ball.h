//
// Created by tara on 10/28/19.
//
#include "gf3d_entity.h"

#ifndef GF3D_GF3D_BALL_H
#define GF3D_GF3D_BALL_H

typedef struct Ball_S
{
    Model *model;
    Matrix4 modelMat;
    Model *targetModel;
    Matrix4 targetMat;
    Location loc;
    Location target;
    int height;
    float fallspeed;
    Uint8 fell;
}Ball;

void gf3d_ball_init();

Location gf3d_ball_get_loc();

void gf3d_ball_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

void gf3d_ball_update();

//Actions on the ball
void gf3d_ball_serve();
void gf3d_ball_receive();
void gf3d_ball_set();
void gf3d_ball_spike();

#endif //GF3D_GF3D_BALL_H
