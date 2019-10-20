#ifndef GF3D_GF3D_GRID_H
#define GF3D_GF3D_GRID_H

#define X 10
#define Y 10

#include "gf3d_entity.h"

typedef struct TILE_S
{
    Uint8          hasEntity;      /**<flag to keep track if this isntance has an entity in it*/
    Entity          *entity;       /**<the entity in the grid NULL if no entity is present*/
    Uint8          inBounds;      /**<flag to keep track if this isntance is in-bounds*/
    Uint8          team;          /**<flag to keep track which team the grid is on*/
    void *data;                    /**<additional entity specific data*/
}Tile;

void gf3d_grid_manager_init();

Tile* gf3d_grid_get_tile(int x, int y);

void gf3d_grid_init_entity_position(int x, int y, Entity* entity);

void gf3d_grid_set_entity(int x, int y, Entity* entity);

void gf3d_grid_move_entity(int x, int y, Entity* entity);

void gf3d_grid_log_state();

#endif //GF3D_GF3D_GRID_H
