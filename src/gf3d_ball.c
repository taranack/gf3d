#include <simple_logger.h>
#include "gf3d_ball.h"

static Ball ball = {0};

void gf3d_ball_init(){
    ball.model = gf3d_model_load("ball");
    gfc_matrix_identity(ball.modelMat);

    ball.targetModel = gf3d_model_load("balltarget");
    gfc_matrix_identity(ball.targetMat);
    ball.height = 2;
    Location ballLoc;
    ballLoc.x = 2;
    ballLoc.y = 6;
    ball.fallspeed = 1;
    ball.fell = false;
    ball.loc = ballLoc;
    ball.target.x = ballLoc.x;
    ball.target.y = ballLoc.y;
    gfc_matrix_make_translation(ball.modelMat, vector3d(ball.loc.x - X / 2, ball.loc.y - Y / 2, ball.height));
}

void gf3d_ball_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer){
    gf3d_model_draw(ball.model,bufferFrame,commandBuffer,ball.modelMat);
    if(!gf3d_location_compare(ball.loc, ball.target)){
        gf3d_model_draw(ball.targetModel,bufferFrame,commandBuffer,ball.targetMat);
    }
}

Location gf3d_ball_get_loc() {
    return ball.loc;
}

void gf3d_ball_update(){
    Location oldLoc;
    oldLoc.x = ball.loc.x;
    oldLoc.y = ball.loc.y;

    //make it halfway to the new target
    ball.loc.x = (ball.loc.x + ball.target.x) / 2;
    ball.loc.y = (ball.loc.y + ball.target.y) / 2;

    //if it is one away, move it to the target
    if(gf3d_location_compare(oldLoc, ball.loc)){
        ball.loc.x = ball.target.x;
        ball.loc.y = ball.target.y;
    }
    if(ball.fallspeed < 1){
        if(!ball.fell){
            ball.fell = true;
            ball.height--;
        }else{
            ball.fell = false;
        }
    }else{
        ball.height -= ball.fallspeed;
    }
    gfc_matrix_make_translation(ball.targetMat, vector3d(ball.target.x - X / 2, ball.target.y - Y / 2, 1));
    gfc_matrix_make_translation(ball.modelMat, vector3d(ball.loc.x - X / 2, ball.loc.y - Y / 2, ball.height));
}


void gf3d_ball_serve() {
    Location target;
    target.x = 11;
    target.y = 7;
    ball.target = target;
    ball.height += 2;
    ball.fallspeed = .5;
}

void gf3d_ball_receive() {
    Location target;
    target.x = ball.loc.x - 2;
    target.y = ball.loc.y;
    ball.target = target;
    ball.fallspeed = 1;
    ball.height += 3;
}

void gf3d_ball_set() {
    Location target;
    target.x = ball.loc.x;
    target.y = ball.loc.y + 2;
    ball.target = target;
    ball.fallspeed = 1;
    ball.height += 5;
}

void gf3d_ball_spike() {
    Location target;
    target.x = ball.loc.x - 3;
    target.y = ball.loc.y;
    ball.target = target;
    ball.fallspeed = 2;
    ball.height += 0;
}