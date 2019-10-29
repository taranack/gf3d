#include <stdlib.h>
#include <string.h>
#include <gf3d_grid.h>
#include "simple_logger.h"
#include "gf3d_entity.h"

typedef struct {
    Entity *entity_list;
    Uint32 entity_max;
} EntityManager;

static EntityManager gf3d_entity_manager = {0};

void gf3d_entity_manager_close() {
    if (gf3d_entity_manager.entity_list != NULL) {
        free(gf3d_entity_manager.entity_list);
    }
    memset(&gf3d_entity_manager, 0, sizeof(EntityManager));
}

void gf3d_entity_manager_init(Uint32 entity_max) {
    gf3d_entity_manager.entity_list = (Entity *) gfc_allocate_array(sizeof(Entity), entity_max);
    gf3d_entity_manager.entity_max = entity_max;
    if (!gf3d_entity_manager.entity_list) {
        slog("failed to allocate entity list");
        return;
    }
    atexit(gf3d_entity_manager_close);
}

void gf3d_entity_manager_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer) {
    int i;
    for (i = 0; i < gf3d_entity_manager.entity_max; i++) {
        if (gf3d_entity_manager.entity_list[i].model != NULL){
            gf3d_model_draw(gf3d_entity_manager.entity_list[i].model,bufferFrame,commandBuffer,gf3d_entity_manager.entity_list[i].modelMat);
            if(!gf3d_entity_manager.entity_list[i].hasMove){
                gf3d_model_draw(gf3d_entity_manager.entity_list[i].nextMove,bufferFrame,commandBuffer,gf3d_entity_manager.entity_list[i].nextMoveMat);
            }
        }
    }
}

Entity *gf3d_entity_new() {
    int i;
    for (i = 0; i < gf3d_entity_manager.entity_max; i++) {
        if (gf3d_entity_manager.entity_list[i]._inuse)continue;
        // found a free entity
        memset(&gf3d_entity_manager.entity_list[i], 0, sizeof(Entity));
        gf3d_entity_manager.entity_list[i]._inuse = 1;
        return &gf3d_entity_manager.entity_list[i];
    }
    slog("request for entity failed: all full up");
    return NULL;
}

void gf3d_entity_init(Entity *entity, char model[], char exhausted[]) {
    entity->model = gf3d_model_load(model);
    gfc_matrix_identity(entity->modelMat);

    entity->nextMove = gf3d_model_load(exhausted);
    gfc_matrix_identity(entity->nextMoveMat);

    entity->hasMove = true;
    entity->action = Idle;
}

void gf3d_entity_change_model(Entity *entity, char model[]){
    entity->model = gf3d_model_load(model);
}

void gf3d_entity_free(Entity *self) {
    if (!self) {
        slog("self pointer is not valid");
        return;
    }
    self->_inuse = 0;
    gf3d_model_free(self->model);
    if (self->data != NULL) {
        slog("warning: data not freed at entity free!");
    }
}

void gf3d_entity_manager_turn_pass(){
    int i;
    for (i = 0; i < gf3d_entity_manager.entity_max; i++) {
        gf3d_grid_move_to_next_loc(&gf3d_entity_manager.entity_list[i]);
        gf3d_entity_manager.entity_list[i].nextLoc = gf3d_entity_manager.entity_list[i].loc;
        gf3d_entity_manager.entity_list[i].hasMove = true;
    }
}



//Entity specific methods

void gf3d_entity_set_team(Entity *entity, Team team){
    entity->team = team;
}

void gf3d_entity_rotate(Entity *self, float degrees, Vector3D axis) {
    gfc_matrix_rotate(self->modelMat, self->modelMat, degrees, axis);
}

void gf3d_entity_make_translation(Entity *self, Vector3D move) {
    gfc_matrix_make_translation(self->modelMat, move);
}

void gf3d_entity_choose_action(Entity *entity, ActionState action){
    entity->action = action;
    gf3d_action_swap_action(action);
}

/*eol@eof*/
