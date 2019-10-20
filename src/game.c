#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_grid.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    
    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-validate") == 0)
        {
            validate = 1;
        }
    }
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                          //validation
    );

    gf3d_entity_manager_init(128);
    gf3d_grid_manager_init();

    Entity *floor[X][Y];
    for(int i = 0; i < X; i++){
        for(int j = 0; j < Y; j++){
            floor[i][j] = gf3d_entity_new();
            if(gf3d_grid_get_tile(i, j)->inBounds){
                gf3d_entity_init(floor[i][j], "planegreen");
            }else{
                gf3d_entity_init(floor[i][j], "planered");
            }
            gf3d_entity_make_translation(floor[i][j], vector3d(i-X/2, j-Y/2, 0));
            gf3d_entity_rotate(floor[i][j], 3*M_PI/2, vector3d(0,1,0));

        }
    }



    /*Entity *vulture = gf3d_entity_new();
    gf3d_entity_init(vulture, "vulture");
    gf3d_entity_rotate(vulture, M_PI/2, vector3d(1,0,0));
    gf3d_entity_rotate(vulture, M_PI, vector3d(0,1,0));
    */
    int teamSize = 3;

    Entity *team[teamSize];

    //Team monkey
    for(int i = 0; i < teamSize; i++){
        team[i] = gf3d_entity_new();
        gf3d_entity_init(team[i], "testmonkey");
        gf3d_grid_init_entity_position(i, i, team[i]);

    }

    int curEntity = 0;
    Entity *activeEntity = team[curEntity];

    Entity *highlight = gf3d_entity_new();
    gf3d_entity_init(highlight, "highlighted");
    gf3d_entity_make_translation(highlight, vector3d(0, 0, 0));

    int cycleTimer = 0;
    Uint8 startCycle = false;

    // main game loop
    slog("gf3d main loop begin");
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here

        gf3d_entity_make_translation(highlight, vector3d(activeEntity->x - X/2, activeEntity->y - Y/2, 0));

        //Inputs
        if(keys[SDL_SCANCODE_LEFT])gf3d_vgraphics_rotate_camera(0.002);
        if(keys[SDL_SCANCODE_RIGHT])gf3d_vgraphics_rotate_camera(-0.002);

        if(keys[SDL_SCANCODE_GRAVE])gf3d_grid_log_state();


        if(keys[SDL_SCANCODE_SPACE]){
            for(int i = 0; i < teamSize; i++){
                team[i]->hasMove = true;
                gf3d_entity_change_model(team[i], "testmonkey");
            }
        }

        if(startCycle){
            cycleTimer++;
            if(cycleTimer >= 300){
                cycleTimer = 0;
                startCycle = false;
            }
        }

        if(keys[SDL_SCANCODE_TAB]){
            if(!startCycle){
                startCycle = true;
                curEntity++;
                if(curEntity == teamSize){
                    curEntity = 0;
                }
                activeEntity = team[curEntity];
                slog("current entity is %i", curEntity);
            }
        }

        if(activeEntity->hasMove){
            if(keys[SDL_SCANCODE_A]){
                gf3d_grid_move_entity(-1, 0, activeEntity);
                activeEntity->hasMove = false;
                gf3d_entity_change_model(activeEntity, "testmonkeyexhausted");
                gf3d_grid_log_state();
            }
            if(keys[SDL_SCANCODE_D]){
                gf3d_grid_move_entity(1, 0, activeEntity);
                activeEntity->hasMove = false;
                gf3d_entity_change_model(activeEntity, "testmonkeyexhausted");
                gf3d_grid_log_state();
            }
            if(keys[SDL_SCANCODE_W]){
                gf3d_grid_move_entity(0, 1, activeEntity);
                activeEntity->hasMove = false;
                gf3d_entity_change_model(activeEntity, "testmonkeyexhausted");
                gf3d_grid_log_state();
            }
            if(keys[SDL_SCANCODE_S]){
                gf3d_grid_move_entity(0, -1, activeEntity);
                activeEntity->hasMove = true;
                gf3d_entity_change_model(activeEntity, "testmonkeyexhausted");
                gf3d_grid_log_state();
            }
        }

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);

                gf3d_entity_manager_draw_all(bufferFrame, commandBuffer);

            gf3d_command_rendering_end(commandBuffer);
        gf3d_vgraphics_render_end(bufferFrame);

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
