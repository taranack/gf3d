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

int main(int argc, char *argv[]) {
    int done = 0;
    int a;
    Uint8 validate = 0;
    const Uint8 *keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;

    for (a = 1; a < argc; a++) {
        if (strcmp(argv[a], "-validate") == 0) {
            validate = 1;
        }
    }

    init_logger("gf3d.log");
    slog("gf3d begin");
    gf3d_vgraphics_init(
            "Volley All Stars",    //program name
            1200,                   //screen width
            700,                    //screen height
            vector4d(0.51, 0.75, 1, 1),//background color
            0,                      //fullscreen
            validate                          //validation
    );

    gf3d_entity_manager_init(256);
    gf3d_grid_manager_init();

    Model *floorModels[Y][X];
    Matrix4 floorMatrices[Y][X];
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (gf3d_grid_get_tile(i, j)->inBounds) {
                if(gf3d_grid_get_tile(i, j)->team == Home){
                    floorModels[i][j] = gf3d_model_load("planegreen");
                }else{
                    floorModels[i][j] = gf3d_model_load("planeblue");
                }

            } else {
                floorModels[i][j] = gf3d_model_load("planered");
            }
            gfc_matrix_identity(floorMatrices[i][j]);
            gfc_matrix_make_translation(floorMatrices[i][j], vector3d(i - X / 2, j - Y / 2, 0));
            gfc_matrix_rotate(floorMatrices[i][j], floorMatrices[i][j], 3 * M_PI / 2, vector3d(0, 1, 0));

        }
    }



    /*Entity *vulture = gf3d_entity_new();
    gf3d_entity_init(vulture, "vulture");
    gf3d_entity_rotate(vulture, M_PI/2, vector3d(1,0,0));
    gf3d_entity_rotate(vulture, M_PI, vector3d(0,1,0));
    */
    int teamSize = 6;
    Location homeSpawns[teamSize];
    homeSpawns[0].x = 6;
    homeSpawns[0].y = 4;

    homeSpawns[1].x = 8;
    homeSpawns[1].y = 4;

    homeSpawns[2].x = 10;
    homeSpawns[2].y = 4;

    homeSpawns[3].x = 6;
    homeSpawns[3].y = 6;

    homeSpawns[4].x = 8;
    homeSpawns[4].y = 6;

    homeSpawns[5].x = 10;
    homeSpawns[5].y = 6;

    Location awaySpawns[teamSize];
    awaySpawns[0].x = 6;
    awaySpawns[0].y = 9;

    awaySpawns[1].x = 8;
    awaySpawns[1].y = 9;

    awaySpawns[2].x = 10;
    awaySpawns[2].y = 9;

    awaySpawns[3].x = 6;
    awaySpawns[3].y = 11;

    awaySpawns[4].x = 8;
    awaySpawns[4].y = 11;

    awaySpawns[5].x = 10;
    awaySpawns[5].y = 11;

    Entity *homeTeam[teamSize];
    Entity *awayTeam[teamSize];

    //Team monkey
    for (int i = 0; i < teamSize; i++) {
        homeTeam[i] = gf3d_entity_new();
        gf3d_entity_init(homeTeam[i], "testmonkey");
        gf3d_entity_set_team(homeTeam[i], Home);
        gf3d_grid_init_entity_position(homeSpawns[i].y, homeSpawns[i].x, homeTeam[i]);
        gf3d_grid_move_entity(-1, 0, homeTeam[i]);
        gf3d_grid_move_entity(1, 0, homeTeam[i]);
    }

    //Team bevelled cube
    for (int i = 0; i < teamSize; i++) {
        awayTeam[i] = gf3d_entity_new();
        gf3d_entity_init(awayTeam[i], "bevel");
        gf3d_entity_set_team(awayTeam[i], Away);
        gf3d_grid_init_entity_position(awaySpawns[i].y, awaySpawns[i].x, awayTeam[i]);
        gf3d_grid_move_entity(-1, 0, awayTeam[i]);
        gf3d_grid_move_entity(1, 0, awayTeam[i]);
    }

    int curEntity = 0;
    Entity **activeTeam = homeTeam;
    Entity *activeEntity = activeTeam[curEntity];

    Model *highlight = gf3d_model_load("highlighted");
    Matrix4 highlightMatrix;
    gfc_matrix_identity(highlightMatrix);
    gfc_matrix_make_translation(highlightMatrix, vector3d(0, 0, 0));

    int cycleTimer = 0;
    Uint8 startCycle = false;

    Team curTeam = Home;

    // main game loop
    slog("gf3d main loop begin");
    while (!done) {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here

        gfc_matrix_make_translation(highlightMatrix, vector3d(activeEntity->loc.x - X / 2, activeEntity->loc.y - Y / 2, 0));

        //Inputs
        if (keys[SDL_SCANCODE_LEFT])gf3d_vgraphics_rotate_camera(0.002);
        if (keys[SDL_SCANCODE_RIGHT])gf3d_vgraphics_rotate_camera(-0.002);

        if (keys[SDL_SCANCODE_GRAVE])gf3d_grid_log_state();


        if (keys[SDL_SCANCODE_SPACE]) {
            for (int i = 0; i < teamSize; i++) {
                homeTeam[i]->hasMove = true;
                gf3d_entity_change_model(homeTeam[i], "testmonkey");
            }
            if(curTeam == Home){
                curTeam = Away;
                activeTeam = awayTeam;
                activeEntity = homeTeam[0];
            }else{
                curTeam = Home;
                activeTeam = homeTeam;
                activeEntity = awayTeam[0];
            }

        }

        if (startCycle) {
            cycleTimer++;
            if (cycleTimer >= 150) {
                cycleTimer = 0;
                startCycle = false;
            }
        }

        if (keys[SDL_SCANCODE_TAB]) {
            if (!startCycle) {
                startCycle = true;
                curEntity++;
                if (curEntity == teamSize) {
                    curEntity = 0;
                }
                activeEntity = activeTeam[curEntity];
                slog("current entity is %i", curEntity);
            }
        }

        if (activeEntity->hasMove) {
            if (keys[SDL_SCANCODE_A]) {
                gf3d_grid_move_entity(-1, 0, activeEntity);
                activeEntity->hasMove = false;
                gf3d_entity_change_model(activeEntity, "testmonkeyexhausted");
                gf3d_grid_log_state();
            }
            if (keys[SDL_SCANCODE_D]) {
                gf3d_grid_move_entity(1, 0, activeEntity);
                activeEntity->hasMove = false;
                gf3d_entity_change_model(activeEntity, "testmonkeyexhausted");
                gf3d_grid_log_state();
            }
            if (keys[SDL_SCANCODE_W]) {
                gf3d_grid_move_entity(0, 1, activeEntity);
                activeEntity->hasMove = false;
                gf3d_entity_change_model(activeEntity, "testmonkeyexhausted");
                gf3d_grid_log_state();
            }
            if (keys[SDL_SCANCODE_S]) {
                gf3d_grid_move_entity(0, -1, activeEntity);
                activeEntity->hasMove = true;
                gf3d_entity_change_model(activeEntity, "testmonkeyexhausted");
                gf3d_grid_log_state();
            }
        }

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(), bufferFrame);
        commandBuffer = gf3d_command_rendering_begin(bufferFrame);

        for (int i = 0; i < Y; i++) {
            for (int j = 0; j < X; j++) {
                gf3d_model_draw(floorModels[i][j], bufferFrame, commandBuffer, floorMatrices[i][j]);
            }
        }
        gf3d_model_draw(highlight, bufferFrame, commandBuffer, highlightMatrix);

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
