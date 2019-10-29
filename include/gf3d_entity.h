#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__

#include "gf3d_model.h"
#include "gf3d_map.h"
#include "gf3d_action.h"

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
    Model          *nextMove;          /**<the 3d model for this entity's next move*/
    Matrix4        nextMoveMat;        /**<the model matrix for this entity's next move*/
    Team           team;
    Location       loc;             /**<player location*/
    Location       nextLoc;         /**<player's move for the next turn*/
    Location       target;          /**<player's action target*/
    int            preparation;
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

void gf3d_entity_manager_turn_pass();

Entity *gf3d_entity_new();

/**
 * @brief set entity variables to defaults
 * @param entity entity to initialize
 * @param model model to load
 */
void gf3d_entity_init(Entity *entity, char model[], char exhausted[]);

/**
 * @brief free an active entity
 * @param self the entity to free
 */
void gf3d_entity_free(Entity *self);

void gf3d_entity_rotate(Entity *self, float degrees, Vector3D axis);

void gf3d_entity_make_translation(Entity *self, Vector3D move);

void gf3d_entity_change_model(Entity *entity, char model[]);

void gf3d_entity_set_team(Entity *entity, Team team);

void gf3d_entity_choose_action(Entity *entity, ActionState action);

Uint8 gf3d_location_compare(Location A, Location B);

#endif
