# CMakeList.txt : CMake project for Game, include source and define
# project specific logic here.
cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Add source to this project's executable.
add_executable (
	GameTests
	"../src/Game.h"
	"../src/Game.cpp"
    "../src/Glad.c"
	"../src/Tinyxml2.cpp"
	"../src/Input.h"
	"../src/Input.cpp"
	"../src/Logger.h"
	"../src/Logger.cpp"
	"../src/PlayerPrefs.h"
	"../src/Scene.h"
	"../src/Scene.cpp"
	"../src/SceneManager.h"
	"../src/SceneManager.cpp"
	"../src/ScriptFactory.h"
	"../src/ScriptFactory.cpp"
	"../src/Scripts.h"
	"../src/Sound.h"
	"../src/Sound.cpp"
	"../src/Systems.h"
	"../src/Events.h"
	"../src/EntityHelper.h"
	"../src/EntityHelper.cpp"
	"../src/ECS.h"
	"../src/ECS.cpp"
	"../src/CScript.h"
	"../src/CScript.cpp"
	"../src/Components.h"
	"../src/Components/Active.h"
	"../src/Components/Audio.h"
	"../src/Components/Camera.h"
	"../src/Components/Name.h"
	"../src/Components/RigidBody.h"
	"../src/Components/Script.h"
	"../src/Components/Shader.h"
	"../src/Components/SpriteVertices.h"
	"../src/Components/TempEnemyWeapons.h"
	"../src/Components/TextSprite.h"
	"../src/Components/Texture.h"
	"../src/Components/Transform.h"
	"../src/Events/SceneLoaded.h"
	"../src/Physics/ContactListener.h"
	"../src/Physics/PhysicsManager.h"
	"../src/Physics/PhysicsManager.cpp"
	"../src/Renderer/B2Renderer.h"
	"../src/Renderer/B2Renderer.cpp"
	"../src/Renderer/IndexBuffer.h"
	"../src/Renderer/IndexBuffer.cpp"
	"../src/Renderer/Shader.h"
	"../src/Renderer/Shader.cpp"
	"../src/Renderer/TextRenderer.h"
	"../src/Renderer/TextRenderer.cpp"
	"../src/Renderer/Texture.h"
	"../src/Renderer/Texture.cpp"
	"../src/Renderer/VertexArray.h"
	"../src/Renderer/VertexArray.cpp"
	"../src/Renderer/VertexBuffer.h"
	"../src/Renderer/VertexBuffer.cpp"
	"../src/Renderer/VertexBufferLayout.h"
	"../src/Systems/AudioSystem.h"
	"../src/Systems/PhysicsSystem.h"
	"../src/Systems/RenderingSystem.h"
	"../src/Systems/ScriptSystem.h"
	"../src/Scripts/EndScreenScript.h"
	"../src/Scripts/EndScreenScript.cpp"
	"../src/Scripts/EnemyScript.h"
	"../src/Scripts/EnemyScript.cpp"
	"../src/Scripts/LoadoutSelectionScript.h"
	"../src/Scripts/LoadoutSelectionScript.cpp"
	"../src/Scripts/MainmenuScript.h"
	"../src/Scripts/MainmenuScript.cpp"
	"../src/Scripts/PausemenuScript.h"
	"../src/Scripts/PausemenuScript.cpp"
	"../src/Scripts/PlayerScript.h"
	"../src/Scripts/PlayerScript.cpp"
	"../src/WeaponScripts/BowScript.h"
	"../src/WeaponScripts/BowScript.cpp"
	"../src/WeaponScripts/GauntletScript.h"
	"../src/WeaponScripts/GauntletScript.cpp"
	"../src/WeaponScripts/GrenadeScript.h"
	"../src/WeaponScripts/GrenadeScript.cpp"
	"../src/WeaponScripts/GunScript.h"
	"../src/WeaponScripts/GunScript.cpp"
	"../src/WeaponScripts/ShieldScript.h"
	"../src/WeaponScripts/ShieldScript.cpp"
	"../src/WeaponScripts/SwordScript.h"
	"../src/WeaponScripts/SwordScript.cpp"
	"../src/WeaponScripts/WeaponScript.h"
	"Test.cpp"
	"InputTests.h"
	"PhysicsTests.h"
	"SceneTests.h"
)

# Setup Google Test
include(FetchContent)
FetchContent_Declare (
	googletest
	URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Link libraries
target_link_libraries(GameTests gtest_main)
target_link_libraries(GameTests "${CMAKE_SOURCE_DIR}/lib/entityx.lib")
target_link_libraries(GameTests "${CMAKE_SOURCE_DIR}/lib/box2d.lib")
target_link_libraries(GameTests "${CMAKE_SOURCE_DIR}/lib/glfw3dll.lib")
target_link_libraries(GameTests "${CMAKE_SOURCE_DIR}/lib/bass.lib")
target_link_libraries(GameTests "${CMAKE_SOURCE_DIR}/lib/freetype.lib")

# Copy dlls to executable to avoid memory errors
if (MSVC)
  file(GLOB GAME_DLLS "${CMAKE_SOURCE_DIR}/lib/*.dll")
  add_custom_command(TARGET GameTests
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${GAME_DLLS}
                     $<TARGET_FILE_DIR:GameTests>)
endif (MSVC)

# Copy Game assets
add_custom_command(TARGET GameTests 
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E make_directory 
				   "${CMAKE_BINARY_DIR}/test/src/Assets")
add_custom_command(TARGET GameTests
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/src/Assets
                   $<TARGET_FILE_DIR:GameTests>/src/Assets)
