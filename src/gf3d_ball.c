#include "gf3d_ball.h"

static Ball ball = {0};

void gf3d_ball_init(){
    ball.model = gf3d_model_load("ball");
    gfc_matrix_identity(ball.modelMat);
    ball.height = 1;
    Location ballLoc;
    ballLoc.x = 2;
    ballLoc.y = 6;
    ball.loc = ballLoc;
    gfc_matrix_make_translation(ball.modelMat, vector3d(ball.loc.x - X / 2, ball.loc.y - Y / 2, ball.height));
}

void gf3d_ball_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer){
    gf3d_model_draw(ball.model,bufferFrame,commandBuffer,ball.modelMat);
}

