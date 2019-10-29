#include <stdlib.h>
#include <string.h>
#include "simple_logger.h"
#include "gf3d_grid.h"

typedef struct {
    Tile *tile_list[Y][X];    /**<2D array to represent the world*/
} GridManager;

static GridManager gf3d_grid_manager = {0};

int inBounds(int x, int y) {
    return !(( x < 5 || x > X-6 ) || ( y < 3 || y > Y-4 ));
}

void gf3d_grid_manager_init() {
    int i;
    int j;
    for(i = 0; i < Y; i++){
        for(j = 0; j < X; j++){
            gf3d_grid_manager.tile_list[i][j] = (Tile*) malloc(sizeof(Tile));
            gf3d_grid_manager.tile_list[i][j]->hasEntity = false;
            if(i < Y/2){
                gf3d_grid_manager.tile_list[i][j]->team = Home;
            }else{
                gf3d_grid_manager.tile_list[i][j]->team = Away;
            }

            if(!inBounds(j, i)){
                gf3d_grid_manager.tile_list[i][j]->inBounds = false;
            }else{
                gf3d_grid_manager.tile_list[i][j]->inBounds = true;
            }
        }
    }
}

Tile* gf3d_grid_get_tile(int x, int y){
    return gf3d_grid_manager.tile_list[x][y];
}

void gf3d_grid_set_entity(int x, int y, Entity* entity) {
    gf3d_grid_manager.tile_list[x][y]->entity = entity;
    gf3d_grid_manager.tile_list[x][y]->hasEntity = true;

    entity->loc.x = x;
    entity->loc.y = y;

    entity->nextLoc.x = x;
    entity->nextLoc.y = y;
}

void gf3d_grid_clear_entity(int x, int y){
    gf3d_grid_manager.tile_list[x][y]->entity = NULL;
    gf3d_grid_manager.tile_list[x][y]->hasEntity = false;
}

int onGrid(int x, int y){
    return (x > -1 && x < X) && (y > -1 && y < Y);
}

void gf3d_grid_init_entity_position(int x, int y, Entity* entity){
    gf3d_entity_make_translation(entity, vector3d((entity->loc.x-X/2), (entity->loc.y-Y/2), 0));
    gf3d_grid_set_entity(x, y, entity);
}

void gf3d_grid_move_to_next_loc(Entity* entity){
    int curX = entity->loc.x;
    int curY = entity->loc.y;

    int nextX = entity->nextLoc.x;
    int nextY = entity->nextLoc.y;
    if(!onGrid(nextX, nextY) || gf3d_grid_manager.tile_list[nextX][nextY]->hasEntity){
        return;
    }

    gf3d_grid_clear_entity(curX, curY);
    gf3d_grid_set_entity(nextX, nextY, entity);
    gf3d_entity_make_translation(entity, vector3d((nextX-X/2), (nextY-Y/2), 0));
}

void gf3d_grid_move_entity(int x, int y, Entity* entity) {
    int curX = entity->loc.x;
    int curY = entity->loc.y;

    int nextX = curX+x;
    int nextY = curY+y;
    if(!onGrid(nextX, nextY) || gf3d_grid_manager.tile_list[nextX][nextY]->hasEntity){
        return;
    }

    gf3d_grid_clear_entity(curX, curY);
    gf3d_grid_set_entity(nextX, nextY, entity);
    gf3d_entity_make_translation(entity, vector3d((nextX-X/2), (nextY-Y/2), 0));
}

void gf3d_grid_prep_move(int x, int y, Entity* entity){
    entity->nextLoc.x += x;
    entity->nextLoc.y += y;

    int nextX = entity->nextLoc.x;
    int nextY = entity->nextLoc.y;
    if(!onGrid(nextX, nextY) || gf3d_grid_manager.tile_list[nextX][nextY]->hasEntity){
        return;
    }

    entity->hasMove = false;

    gfc_matrix_make_translation(entity->nextMoveMat, vector3d((nextX-X/2), (nextY-Y/2), 0));
}

void gf3d_grid_log_state(){
    char *boardState = malloc(1024);
    int i;
    int j;
    strcat(boardState, "\n");
    for(i = 0; i < Y; i++){
        for(j = 0; j < X; j++){
            strcat(boardState, "[");
            if(gf3d_grid_manager.tile_list[i][j]->hasEntity){
                strcat(boardState, "x");
            }else{
                if(gf3d_grid_manager.tile_list[i][j]->inBounds){
                    strcat(boardState, " ");
                }else{
                    strcat(boardState, "O");
                }

            }
            strcat(boardState, "]");
        }
        strcat(boardState, "\n");
    }
    slog(boardState);
    free(boardState);
}