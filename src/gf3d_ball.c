#include <simple_logger.h>
#include "gf3d_ball.h"

static Ball ball = {0};

void gf3d_ball_init(){
    ball.model = gf3d_model_load("ball");
    gfc_matrix_identity(ball.modelMat);
    ball.height = 2;
    Location ballLoc;
    ballLoc.x = 2;
    ballLoc.y = 6;
    ball.loc = ballLoc;
    gfc_matrix_make_translation(ball.modelMat, vector3d(ball.loc.x - X / 2, ball.loc.y - Y / 2, ball.height));
}

void gf3d_ball_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer){
    gf3d_model_draw(ball.model,bufferFrame,commandBuffer,ball.modelMat);
}

Location gf3d_ball_get_loc() {
    return ball.loc;
}

void gf3d_ball_update(){
    //make it halfway to the new target
    ball.loc.x = (ball.loc.x + ball.target.x) / 2;
    ball.loc.y = (ball.loc.y + ball.target.y) / 2;
    ball.height--;

    gfc_matrix_make_translation(ball.modelMat, vector3d(ball.loc.x - X / 2, ball.loc.y - Y / 2, ball.height));
}


void gf3d_ball_serve() {
    Location target;
    target.x = 11;
    target.y = 7;
    ball.target = target;
    ball.height += 2;
}

void gf3d_ball_receive() {
    Location target;
    target.x = ball.loc.x + 3;
    target.y = ball.loc.y;
    ball.target = target;
    ball.height += 3;
}
