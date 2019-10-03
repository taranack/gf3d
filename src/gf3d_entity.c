#include <stdlib.h>
#include <string.h>
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

//Entity specific methods

void gf3d_entity_rotate(Entity *self, float degrees, Vector3D axis) {
    gfc_matrix_rotate(self->modelMat, self->modelMat, degrees, axis);
}

void gf3d_entity_translate(Entity *self, Vector3D move) {
    gfc_matrix_translate(self->modelMat, move);
}


/*eol@eof*/
