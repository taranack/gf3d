#include <stdlib.h>
#include <string.h>
#include "simple_logger.h"
#include "gf3d_grid.h"

typedef struct {
    Tile *tile_list[X][Y];    /**<2D array to represent the world*/
} GridManager;

static GridManager gf3d_grid_manager = {0};

int inBounds(int x, int y){
    return !(( x < 2 || x > X-3 ) || ( y < 2 || y > Y-3 ));
}

void gf3d_grid_manager_init() {
    int i;
    int j;
    for(i = 0; i < X; i++){
        for(j = 0; j < Y; j++){
            gf3d_grid_manager.tile_list[i][j] = (Tile*) malloc(sizeof(Tile));
            gf3d_grid_manager.tile_list[i][j]->hasEntity = false;
            if(j < Y/2){
                gf3d_grid_manager.tile_list[i][j]->team = 1;
            }

            if(!inBounds(i, j)){
                gf3d_grid_manager.tile_list[i][j]->inBounds = false;
            }else{
                gf3d_grid_manager.tile_list[i][j]->inBounds = true;
            }
        }
    }
}

void gf3d_grid_set_entity(int x, int y, Entity* entity) {
    gf3d_grid_manager.tile_list[x][y]->entity = entity;
    gf3d_grid_manager.tile_list[x][y]->hasEntity = true;

    entity->x = x;
    entity->y = y;
}

void gf3d_grid_clear_entity(int x, int y){
    gf3d_grid_manager.tile_list[x][y]->entity = NULL;
    gf3d_grid_manager.tile_list[x][y]->hasEntity = false;
}

int onGrid(int x, int y){
    return (x > -1 && x < X) && (y > -1 && y < Y);
}

void gf3d_grid_move_entity(int x, int y, Entity* entity) {
    int curX = entity->x;
    int curY = entity->y;

    int nextX = curX+x;
    int nextY = curY+y;
    if(!onGrid(nextX, nextY)){
        return;
    }

    gf3d_grid_clear_entity(curX, curY);
    gf3d_grid_set_entity(nextX, nextY, entity);
    gf3d_entity_make_translation(entity, vector3d(nextX, nextY, 0));
}

void gf3d_grid_log_state(){
    char *boardState = malloc(1024);
    int i;
    int j;
    strcat(boardState, "\n");
    for(i = 0; i < X; i++){
        for(j = 0; j < Y; j++){
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