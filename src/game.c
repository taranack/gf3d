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
    
    // main game loop
    slog("gf3d main loop begin");
    
    gf3d_entity_manager_init(128);
    gf3d_grid_manager_init();

    Entity *vulture = gf3d_entity_new();
    gf3d_entity_init(vulture, "vulture");
    gf3d_entity_rotate(vulture, M_PI/2, vector3d(1,0,0));
    gf3d_entity_rotate(vulture, M_PI, vector3d(0,1,0));

    int i;
    int j;
    Entity *cubes[3][3];
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            cubes[i][j] = gf3d_entity_new();
            gf3d_entity_init(cubes[i][j], "cube");
            gf3d_entity_translate(cubes[i][j], vector3d(i, j, 0));
        }
    }

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here

        gf3d_entity_rotate(vulture, .002, vector3d(0,1,0));

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
