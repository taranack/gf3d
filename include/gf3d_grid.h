#ifndef GF3D_GF3D_GRID_H
#define GF3D_GF3D_GRID_H

#include "gf3d_ball.h"

typedef struct TILE_S
{
    Uint8          hasEntity;      /**<flag to keep track if this isntance has an entity in it*/
    Entity          *entity;       /**<the entity in the grid NULL if no entity is present*/
    Uint8          inBounds;      /**<flag to keep track if this isntance is in-bounds*/
    Team           team;          /**<which team the grid is on*/
    void *data;                    /**<additional entity specific data*/
}Tile;

void gf3d_grid_manager_init();

Tile* gf3d_grid_get_tile(int x, int y);

void gf3d_grid_init_entity_position(int x, int y, Entity* entity);

void gf3d_grid_set_entity(int x, int y, Entity* entity);

void gf3d_grid_move_entity(int x, int y, Entity* entity);

void gf3d_grid_move_to_next_loc(Entity* entity);

void gf3d_grid_prep_move(int x, int y, Entity* entity);

void gf3d_grid_log_state();

#endif //GF3D_GF3D_GRID_H
