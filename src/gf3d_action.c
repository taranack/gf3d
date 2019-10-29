#include "gf3d_action.h"

static ActionUI actionUILeft = {0};
static ActionUI actionUIRight = {0};


void gf3d_action_init(){
    actionUILeft.model = gf3d_model_load("idle");
    gfc_matrix_identity(actionUILeft.modelMat);
    gfc_matrix_make_translation(actionUILeft.modelMat, vector3d(0, 7, 2));

    actionUIRight.model = gf3d_model_load("idle");
    gfc_matrix_identity(actionUIRight.modelMat);
    gfc_matrix_make_translation(actionUIRight.modelMat, vector3d(0, -7, 2));
    gfc_matrix_rotate(actionUIRight.modelMat, actionUIRight.modelMat, M_PI, vector3d(0, 0, 1));
}

void gf3d_action_swap_model(char model[]){
    actionUILeft.model = gf3d_model_load(model);
    actionUIRight.model = gf3d_model_load(model);
}

void gf3d_action_swap_action(ActionState action){
    switch (action)
    {
        case Idle:
            gf3d_action_swap_model("idle");
            break;
        case Spiking:
            gf3d_action_swap_model("spiking");
            break;
        case Setting:
            gf3d_action_swap_model("setting");
            break;
        case Receiving:
            gf3d_action_swap_model("receiving");
            break;
        case Serving:
            gf3d_action_swap_model("serving");
            break;
        case Blocking:
            gf3d_action_swap_model("blocking");
            break;
    }
}

void gf3d_action_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer){
    gf3d_model_draw(actionUILeft.model,bufferFrame,commandBuffer,actionUILeft.modelMat);
    gf3d_model_draw(actionUIRight.model,bufferFrame,commandBuffer,actionUIRight.modelMat);
}