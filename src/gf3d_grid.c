#include <stdlib.h>
#include <string.h>
#include "simple_logger.h"
#include "gf3d_grid.h"

typedef struct {
    Tile *tile_list[X][Y];    /**<2D array to represent the world*/
} GridManager;

static GridManager gf3d_grid_manager = {0};


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

            if(( i < 2 || i > X-2 ) || ( j < 2 || j > Y-2 )){
                gf3d_grid_manager.tile_list[i][j]->inBounds = false;
            }else{
                gf3d_grid_manager.tile_list[i][j]->inBounds = true;
            }
        }
    }
}