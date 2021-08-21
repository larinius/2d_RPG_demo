cmake_minimum_required(VERSION 3.19)
project(Game)

set(CMAKE_CXX_STANDARD 17)
#set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/CMake")

#set(CMAKE_CXX_FLAGS "-Wall")
#set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG /Od /MP6 /EHs")
set(CMAKE_CXX_FLAGS_RELEASE "/O2 /MP6 /EHs")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -Wnone -EHa")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" /MP /EHs)
message("COMPILER: ${CMAKE_CXX_COMPILER_ID}")

if (MSVC)
    # For precompiled header.
    # Set 
    # "Precompiled Header" to "Use (/Yu)"
    # "Precompiled Header File" to "precompiled.h"
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Yuprecompiled.h /FIprecompiled.h")
endif()


if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /O3 -Xclang -fcxx-exceptions -Wno-everything")
ENDIF ()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()


# Find SFML
set(SFML_STATIC_LIBRARIES TRUE)

list(APPEND CMAKE_PREFIX_PATH
        "D:/Dev/Libs/LDtkLoader"
#        "D:/Dev/Libs/reckless-3.0.3"
        )

find_package(SFML REQUIRED COMPONENTS audio graphics window system)
find_package(RapidJSON CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc data i18n io)
find_package(LDtkLoader)
find_package(box2d CONFIG REQUIRED)
#find_package(Eigen3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
#find_path(LOGURU_INCLUDE_DIRS "loguru/loguru.cpp")


include_directories(
#        "D:/Dev/Libs/reckless-3.0.3/reckless/include"
        "D:/Dev/Libs/mathfu/include"
#        "D:/Dev/Libs/Thor/include"
        "D:/Dev/Libs/vld/include"
        "D:/Dev/Libs/TGUI/include"
        "D:/Dev/Libs/LDtkLoader/include"
#        "D:/Dev/Libs/DragonBones/include"
#        "D:/Dev/Libs/DragonBones/include/3rdParty"
#        external/dragonBones
        external/eventpp/include
        external/SFMLDebug
        external/iprof
        include
        include/Components
)

set(LIBS
#        "D:/Dev/Libs/Thor/lib"
        "D:/Dev/Libs/vld/lib/Win64"
        "D:/Dev/Libs/TGUI/lib"
        "D:/Dev/Src/Cpp/RPG/cmake-build-debug/TGUI/lib/Debug"
#        "D:/Dev/Libs/DragonBones/lib"
        "D:/Dev/Libs/LDtkLoader/lib"
#        "D:/Dev/Libs/reckless-3.0.3/reckless/lib"
        "D:/Dev/Libs/SelbaWard/lib"
        )

link_directories(${LIBS})

set_source_files_properties(src/precompiled.cpp
    PROPERTIES
    COMPILE_FLAGS "/Ycprecompiled.h"
)

set(SOURCES
        src/precompiled.cpp
        src/Components/AnimatedSprite.cpp
        src/Components/PhysicsBox.cpp
        src/Components/Component.cpp
        src/Components/PlayerController.cpp
        src/Components/Navigation.cpp
        src/Components/SimpleImage.cpp
        src/Components/CollisionBox.cpp
        src/Components/ActorState.cpp
        src/Components/Sound.cpp
        src/Components/ActorController.cpp
        #        src/Components/SkeletalAnimation.cpp
        src/State/State.cpp
        src/State/StateMachine.cpp
        src/State/StatePlay.cpp
        src/State/StateSettings.cpp
        src/State/StateTitle.cpp
        src/Audio.cpp
        src/Engine.cpp
        src/Entity.cpp
        src/Events.cpp
        src/Game.cpp
        src/Gui.cpp
        src/Helpers.cpp
        src/Layer.cpp
        src/Level.cpp
        src/NavSystem.cpp
        src/Object.cpp
        src/ECSystem.cpp
        src/Physics.cpp
        src/Tag.cpp
        src/TileMap.cpp
        src/CollisionSystem.cpp
        src/CommandQueue.cpp
        src/AI.cpp
#        external/dragonBones/SFMLArmatureDisplay.cpp
#        external/dragonBones/SFMLArmatureProxy.cpp
#        external/dragonBones/SFMLFactory.cpp
#        external/dragonBones/SFMLSlot.cpp
#        external/dragonBones/SFMLTextureAtlasData.cpp
        external/iprof/iprof.cpp
        external/SFMLDebug/SFMLDebugDraw.cpp

        include/Enums.h src/Enums.cpp include/Structs.h)


add_executable(${PROJECT_NAME} ${SOURCES})

# Ignore warnings about missing pdb
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4099")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4099")

target_link_libraries(${PROJECT_NAME} PRIVATE box2d::box2d)
#target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
        optimized LDtkLoader debug LDtkLoader-d
        ICU::uc
        ICU::i18n
        ICU::data
        ICU::io
#        optimized reckless debug reckless-d
        Synchronization
#        optimized DragonBones debug DragonBones-d
        optimized tgui debug tgui-d
        sfml-graphics
        sfml-window
        sfml-audio
        sfml-network
        sfml-system
        #        vld
        )

target_include_directories(${PROJECT_NAME} PRIVATE ${RAPIDJSON_INCLUDE_DIRS})
#target_include_directories(${PROJECT_NAME} PRIVATE ${LOGURU_INCLUDE_DIRS})

include_directories("D:/Dev/Libs/SelbaWard/include")
target_link_libraries(${PROJECT_NAME} PRIVATE optimized SelbaWard debug SelbaWard-d)

message("${CMAKE_CURRENT_BINARY_DIR}")

#add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#        ${CMAKE_SOURCE_DIR}./Assets/json/config.json
#        ${CMAKE_CURRENT_BINARY_DIR}/config.json)
