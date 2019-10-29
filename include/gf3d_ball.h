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
    Location loc;
    Location target;
    int height;
}Ball;

void gf3d_ball_init();

void gf3d_ball_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

#endif //GF3D_GF3D_BALL_H
