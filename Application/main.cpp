#include <iostream>
#include <thread>
#include <string>

#include "lua.hpp"
#include "raylib.h"

#define MAX_COLUMNS 5
#define EPSILON 0.0001f

void DumpError(lua_State* L)
{
	//Hämta toppen av lua stacken och kolla om det är en sträng
	if (lua_gettop(L) > 0 && lua_isstring(L, -1))
	{
		std::cout << "Lua error: " << lua_tostring(L, -1) << std::endl;

		//Ta bort meddelandet från stacken
		lua_pop(L, 1);
	}
}

//Function som körs parallelt av tråd
void ConsoleThreadFunction(lua_State* L)
{

	//Läs console input
	std::string input;
	//while (GetConsoleWindow())
	//{
	//	std::cout << "> ";
	//	std::getline(std::cin, input);

	//	//Försök köra strängen som lua kod
	//	if (luaL_dostring(L, input.c_str()) != LUA_OK)
	//	{
	//		DumpError(L);
	//	}
	//}
}

int main()
{
	std::cout << "Hello Bergman!" << std::endl;

    // LUA SKIT
	//Rekommenderat att ha ett men går att ha flera om det behövs
	//lua_State* L = luaL_newstate();

	////Öppnar standardbibliotek för lua, gör så att kodsträngen går att köra
	//luaL_openlibs(L);

	////Skapa tråd
	//std::thread consoleThread(ConsoleThreadFunction, L);

	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Jonas Jump");

	// Define the camera to look into our 3d world (position, target, up vector)
	Camera camera = { 0 };
	camera.position = { 0.0f, 2.0f, 4.0f };    // Camera position
	camera.target = { 0.0f, 2.0f, 0.0f };      // Camera looking at point
	camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 60.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

	int cameraMode = CAMERA_FIRST_PERSON;

	// Generates some random columns
	float heights[MAX_COLUMNS] = { 0 };
	Vector3 positions[MAX_COLUMNS] = { 0 };
	Color colors[MAX_COLUMNS] = { 0 };
    Vector3 playerSize = { 1.0f, 2.0f, 1.0f };

    Vector3 previousPlayerPosition = { 0.0f, 1.0f, 2.0f };
    Vector3 playerPosition = { 0.0f, 1.0f, 2.0f };
    Vector3 playerSpeed = { 0, 0, 0 };

    bool collisionX = false;
    bool collisionZ = false;

	for (int i = 0; i < MAX_COLUMNS; i++)
	{
		heights[i] = (float)GetRandomValue(1, 10);
		positions[i] = { (float)GetRandomValue(-15, -5), heights[i] / 2.0f, (float)GetRandomValue(5, 15) };
		colors[i] = { (unsigned char)GetRandomValue(20, 255), (unsigned char)GetRandomValue(10, 55), 30, 255 };
	}

	DisableCursor();                    // Limit cursor to relative movement inside the window

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
        // Update
        //----------------------------------------------------------------------------------
        // Switch camera mode
        if (IsKeyPressed(KEY_ONE))
        {
            cameraMode = CAMERA_FREE;
            camera.up = { 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_TWO))
        {
            cameraMode = CAMERA_FIRST_PERSON;
            camera.up = { 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_THREE))
        {
            cameraMode = CAMERA_THIRD_PERSON;
            camera.up = { 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_FOUR))
        {
            cameraMode = CAMERA_ORBITAL;
            camera.up = { 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyDown(KEY_UP))
        {
            playerPosition.z -= 10.0f * GetFrameTime();

        }
        if (IsKeyDown(KEY_DOWN))
        {
            playerPosition.z += 10.0f * GetFrameTime();

        }
        if (IsKeyDown(KEY_LEFT))
        {
            playerPosition.x -= 10.0f * GetFrameTime();

        }
        if (IsKeyDown(KEY_RIGHT))
        {
            playerPosition.x += 10.0f * GetFrameTime();
        }

        for (size_t i = 0; i < MAX_COLUMNS; i++)
        {
            if (CheckCollisionBoxes(
                BoundingBox{
                Vector3 {
                    playerPosition.x - playerSize.x / 2,
                        previousPlayerPosition.y - playerSize.y / 2,
                        previousPlayerPosition.z - playerSize.z / 2
                    },
                    Vector3 {
                    playerPosition.x + playerSize.x / 2,
                        previousPlayerPosition.y + playerSize.y / 2,
                        previousPlayerPosition.z + playerSize.z / 2
                    }
                },
                BoundingBox{
                Vector3 {
                    float(positions[i].x - 1.0f),
                    float(0.0f),
                    float(positions[i].z - 1.0f)
                },
                Vector3 {
                    float(positions[i].x + 1.0f),
                    float(1.0f),
                    float(positions[i].z + 1.0f)
                }
                })) collisionX = true;

            if (CheckCollisionBoxes(
                BoundingBox{
                Vector3 {
                previousPlayerPosition.x - playerSize.x / 2,
                    previousPlayerPosition.y - playerSize.y / 2,
                    playerPosition.z - playerSize.z / 2
                },
                Vector3 {
                previousPlayerPosition.x + playerSize.x / 2,
                    previousPlayerPosition.y + playerSize.y / 2,
                    playerPosition.z + playerSize.z / 2
                }
                },
                BoundingBox{
                Vector3 {
                    float(positions[i].x - 1.0f),
                    float(0.0f),
                    float(positions[i].z - 1.0f)
                },
                    Vector3 {
                    float(positions[i].x + 1.0f),
                    float(1.0f),
                    float(positions[i].z + 1.0f)
                }
                })) collisionZ = true;
        }
        



        if (collisionX)
            playerPosition.x = previousPlayerPosition.x;
        else
            previousPlayerPosition.x = playerPosition.x;

        if (collisionZ)
            playerPosition.z = previousPlayerPosition.z;
        else
            previousPlayerPosition.z = playerPosition.z;

        collisionX = false;
        collisionZ = false;

        // Switch camera projection
        if (IsKeyPressed(KEY_P))
        {
            if (camera.projection == CAMERA_PERSPECTIVE)
            {
                // Create isometric view
                cameraMode = CAMERA_THIRD_PERSON;
                // Note: The target distance is related to the render distance in the orthographic projection
                camera.position = { 0.0f, 2.0f, -100.0f };
                camera.target = { 0.0f, 2.0f, 0.0f };
                camera.up = { 0.0f, 1.0f, 0.0f };
                camera.projection = CAMERA_ORTHOGRAPHIC;
                camera.fovy = 20.0f; // near plane width in CAMERA_ORTHOGRAPHIC
                
                
                /*CameraYaw(&camera, -135 * DEG2RAD, true);
                CameraPitch(&camera, -45 * DEG2RAD, true, true, false);*/
            }
            else if (camera.projection == CAMERA_ORTHOGRAPHIC)
            {
                // Reset to default view
                cameraMode = CAMERA_THIRD_PERSON;
                camera.position = { 0.0f, 2.0f, 10.0f };
                camera.target = { 0.0f, 2.0f, 0.0f };
                camera.up = { 0.0f, 1.0f, 0.0f };
                camera.projection = CAMERA_PERSPECTIVE;
                camera.fovy = 60.0f;
            }
        }

        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advanced camera controls, it's recommended to compute camera movement manually
        //UpdateCamera(&camera, cameraMode);                  // Update camera

        
                // Camera PRO usage example (EXPERIMENTAL)
                // This new camera function allows custom movement/rotation values to be directly provided
                // as input parameters, with this approach, rcamera module is internally independent of raylib inputs
                UpdateCameraPro(&camera,
                    Vector3{
                        (IsKeyDown(KEY_W))*0.1f -      // Move forward-backward
                        (IsKeyDown(KEY_S))*0.1f,
                        (IsKeyDown(KEY_D))*0.1f -   // Move right-left
                        (IsKeyDown(KEY_A))*0.1f,
                        (IsKeyDown(KEY_SPACE)) * 0.1f -   // Move right-left
                        (IsKeyDown(KEY_LEFT_SHIFT)) * 0.1f                                         // Move up-down
                    },
                    Vector3{
                        GetMouseDelta().x*0.05f,                            // Rotation: yaw
                        GetMouseDelta().y*0.05f,                            // Rotation: pitch
                        0.0f                                                // Rotation: roll
                    },
                    GetMouseWheelMove()*2.0f);                              // Move to target (zoom)
        
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
        DrawCube({ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
        DrawCube({ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
        DrawCube({ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

        // Draw some cubes around
        for (int i = 0; i < MAX_COLUMNS; i++)
        {
            //DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
            DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, RED);
        }

        // Draw player cube
        if (cameraMode == CAMERA_THIRD_PERSON)
        {
            DrawCube(playerPosition, playerSize.x, playerSize.y, playerSize.z, PURPLE);
            DrawCubeWires(playerPosition, playerSize.x, playerSize.y, playerSize.z, DARKPURPLE);
        }

        EndMode3D();

        // Draw info boxes
        DrawRectangle(5, 5, 330, 100, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(5, 5, 330, 100, BLUE);

        DrawText("Camera controls:", 15, 15, 10, BLACK);
        DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
        DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
        DrawText("- Camera mode keys: 1, 2, 3, 4", 15, 60, 10, BLACK);
        DrawText("- Zoom keys: num-plus, num-minus or mouse scroll", 15, 75, 10, BLACK);
        DrawText("- Camera projection key: P", 15, 90, 10, BLACK);

        DrawRectangle(600, 5, 195, 100, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(600, 5, 195, 100, BLUE);

        DrawText("Camera status:", 610, 15, 10, BLACK);
        DrawText(TextFormat("- Mode: %s", (cameraMode == CAMERA_FREE) ? "FREE" :
            (cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON" :
            (cameraMode == CAMERA_THIRD_PERSON) ? "THIRD_PERSON" :
            (cameraMode == CAMERA_ORBITAL) ? "ORBITAL" : "CUSTOM"), 610, 30, 10, BLACK);
        DrawText(TextFormat("- Projection: %s", (camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
            (camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"), 610, 45, 10, BLACK);
        DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, 10, BLACK);
        DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 10, BLACK);
        DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, 10, BLACK);

        EndDrawing();
	}

	CloseWindow();

	return 0;
}