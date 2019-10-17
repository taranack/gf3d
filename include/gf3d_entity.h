#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__

#include "gf3d_model.h"

typedef enum
{
    ES_Idle = 0,
    ES_Spiking = 1,
    ES_Blocking = 2,
    ES_Setting = 3
}EntityState;

typedef struct Entity_S
{
    Uint8           _inuse;         /**<flag to keep track if this isntance is in use and should not be reassigned*/
    Model          *model;          /**<the 3d model for this entity*/
    Matrix4        modelMat;        /**<the model matrix for this entity*/
    int            x;               /**<x position in the grid*/
    int            y;               /**<y position in the grid*/
    Uint8          hasMove;         /**<whether or not the entity has used it's turn*/
    Vector3D       position;        /**<position of the entity in 3d space*/
    EntityState    state;           /**<current state of the entity*/
    void (*think) (struct Entity_S* self);   /**<function called on entity think*/
    void (*update)(struct Entity_S* self);   /**<function called on entity update*/
    void (*touch) (struct Entity_S* self, struct Entity_S* other);   /**<function called on entity touch*/
    void *data;                     /**<additional entity specific data*/
}Entity;

void gf3d_entity_manager_close();

/**
 * @brief initializes the entity subsystem
 * @param entity_max maximum number of simultaneous entities you wish to support
 */
void gf3d_entity_manager_init(Uint32 entity_max);

/**
 * @brief get an empty entity from the system
 * @return NULL on out of space or a pointer to an entity otherwise
 */


/**
 * @brief draw every entity that is in use
 * @param bufferFrame
 * @param commandBuffer
 */
void gf3d_entity_manager_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

Entity *gf3d_entity_new();

/**
 * @brief set entity variables to defaults
 * @param entity entity to initialize
 * @param model model to load
 */
void gf3d_entity_init(Entity *entity, char model[]);

/**
 * @brief free an active entity
 * @param self the entity to free
 */
void gf3d_entity_free(Entity *self);


void gf3d_entity_rotate(Entity *self, float degrees, Vector3D axis);

void gf3d_entity_translate(Entity *self, Vector3D move);

void gf3d_entity_make_translation(Entity *self, Vector3D move);

#endif
