# CMakeList.txt : CMake project for Game, include source and define
# project specific logic here.
cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Add source to this project's executable.
add_executable (
	Game
	"Main.cpp"
	"Game.h"
	"Game.cpp"
	"Glad.c"
	"Tinyxml2.cpp"
	"Input.h"
	"Input.cpp"
	"Logger.h"
	"Logger.cpp"
	"PlayerPrefs.h"
	"Scene.h"
	"Scene.cpp"
	"SceneManager.h"
	"SceneManager.cpp"
	"ScriptFactory.h"
	"ScriptFactory.cpp"
	"Scripts.h"
	"Sound.h"
	"Sound.cpp"
	"Systems.h"
	"Events.h"
	"EntityHelper.h"
	"EntityHelper.cpp"
	"ECS.h"
	"ECS.cpp"
	"CScript.h"
	"CScript.cpp"
	"Components.h"
	"Components/Active.h"
	"Components/Audio.h"
	"Components/Camera.h"
	"Components/Name.h"
	"Components/RigidBody.h"
	"Components/Script.h"
	"Components/Shader.h"
	"Components/SpriteVertices.h"
	"Components/TempEnemyWeapons.h"
	"Components/TextSprite.h"
	"Components/Texture.h"
	"Components/Transform.h"
	"Events/SceneLoaded.h"
	"Physics/ContactListener.h"
	"Physics/PhysicsManager.h"
	"Physics/PhysicsManager.cpp"
	"Renderer/B2Renderer.h"
	"Renderer/B2Renderer.cpp"
	"Renderer/IndexBuffer.h"
	"Renderer/IndexBuffer.cpp"
	"Renderer/Shader.h"
	"Renderer/Shader.cpp"
	"Renderer/TextRenderer.h"
	"Renderer/TextRenderer.cpp"
	"Renderer/Texture.h"
	"Renderer/Texture.cpp"
	"Renderer/VertexArray.h"
	"Renderer/VertexArray.cpp"
	"Renderer/VertexBuffer.h"
	"Renderer/VertexBuffer.cpp"
	"Renderer/VertexBufferLayout.h"
	"Scripts/EndScreenScript.h"
	"Scripts/EndScreenScript.cpp"
	"Scripts/EnemyScript.h"
	"Scripts/EnemyScript.cpp"
	"Scripts/LoadoutSelectionScript.h"
	"Scripts/LoadoutSelectionScript.cpp"
	"Scripts/MainmenuScript.h"
	"Scripts/MainmenuScript.cpp"
	"Scripts/PausemenuScript.h"
	"Scripts/PausemenuScript.cpp"
	"Scripts/PlayerScript.h"
	"Scripts/PlayerScript.cpp"
	"Systems/AudioSystem.h"
	"Systems/PhysicsSystem.h"
	"Systems/RenderingSystem.h"
	"Systems/ScriptSystem.h"
	"WeaponScripts/BowScript.h"
	"WeaponScripts/BowScript.cpp"
	"WeaponScripts/GauntletScript.h"
	"WeaponScripts/GauntletScript.cpp"
	"WeaponScripts/GrenadeScript.h"
	"WeaponScripts/GrenadeScript.cpp"
	"WeaponScripts/GunScript.h"
	"WeaponScripts/GunScript.cpp"
	"WeaponScripts/ShieldScript.h"
	"WeaponScripts/ShieldScript.cpp"
	"WeaponScripts/SwordScript.h"
	"WeaponScripts/SwordScript.cpp"
	"WeaponScripts/WeaponScript.h"
)

# Link libraries
target_link_libraries(Game "${CMAKE_SOURCE_DIR}/lib/entityx.lib")
target_link_libraries(Game "${CMAKE_SOURCE_DIR}/lib/box2d.lib")
target_link_libraries(Game "${CMAKE_SOURCE_DIR}/lib/glfw3dll.lib")
target_link_libraries(Game "${CMAKE_SOURCE_DIR}/lib/bass.lib")
target_link_libraries(Game "${CMAKE_SOURCE_DIR}/lib/freetype.lib")

# Copy dlls to executable to avoid memory errors
if (MSVC)
  file(GLOB GAME_DLLS "${CMAKE_SOURCE_DIR}/lib/*.dll")
  add_custom_command(TARGET Game
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${GAME_DLLS}
                     $<TARGET_FILE_DIR:Game>)
endif (MSVC)

# Copy Game assets
add_custom_command(TARGET Game 
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E make_directory 
				   "${CMAKE_BINARY_DIR}/src/src/Assets")
add_custom_command(TARGET Game
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/src/Assets
                   $<TARGET_FILE_DIR:Game>/src/Assets)
