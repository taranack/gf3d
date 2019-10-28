#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__

#include "gf3d_model.h"

typedef enum
{
    Idle = 0,
    Spiking = 1,
    Blocking = 2,
    Setting = 3,
    Recieving = 4,
    Serving = 5
}ActionState;

typedef enum
{
    Home = 0,
    Away = 1
}Team;

typedef struct Location_S
{
    int x;
    int y;
}Location;

typedef struct Entity_S
{
    Uint8           _inuse;         /**<flag to keep track if this isntance is in use and should not be reassigned*/
    Model          *model;          /**<the 3d model for this entity*/
    Matrix4        modelMat;        /**<the model matrix for this entity*/
    Team           team;
    Location       loc;             /**<player location*/
    Location       target;
    Uint8          hasMove;         /**<whether or not the entity has used it's turn*/
    ActionState    action;           /**<current state of the entity*/
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

void gf3d_entity_make_translation(Entity *self, Vector3D move);

void gf3d_entity_change_model(Entity *entity, char model[]);

void gf3d_entity_set_team(Entity *entity, Team team);

#endif
