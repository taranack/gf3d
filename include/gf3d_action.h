#ifndef GF3D_GF3D_ACTION_H
#define GF3D_GF3D_ACTION_H

#include "gf3d_model.h"
typedef enum
{
    Idle = 0,
    Spiking = 1,
    Blocking = 2,
    Setting = 3,
    Receiving = 4,
    Serving = 5
}ActionState;

typedef struct action_ui{
    Model *model;
    Matrix4 modelMat;
}ActionUI;

void gf3d_action_init();

void gf3d_action_swap_action(ActionState action);

void gf3d_action_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

#endif //GF3D_GF3D_ACTION_H
