cmake_minimum_required(VERSION 3.20)
project(mlx86 C CXX ASM)
set(CMAKE_POSITION_INDEPENDENT_CODE FALSE)
set(CMAKE_CXX_STANDARD 20)


set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g -funroll-loops")

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
#set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")

include(FetchContent)

#FetchContent_Declare(
#        AudioFile
#        GIT_REPOSITORY https://github.com/adamstark/AudioFile.git
#        GIT_TAG master
#)
#FetchContent_MakeAvailable(AudioFile)

include_directories(
   #     ${AudioFile_SOURCE_DIR}
        src
        src/executors
        src/problems
        src/problems/rts_buildorders
        src/problems/rts_buildorders/BAR_game
        src/problems/rts_buildorders/BAR_game/BAR_extractor_scripts
        src/problems/darwin2021
        src/problems/x86
        src/reporters
        src/solvers
        src/tests
        ${CMAKE_CURRENT_BUILD_DIR}
     #   src/starcraft2
     #   src/starcraft2/proto
     #   src/starcraft2/proto/s2clientprotocol
)

file(GLOB SC2_SRC_FILES src/starcraft2/proto/s2clientprotocol/*.c)

set(SOURCES
        src/christian_utils.cpp
        src/christian_utils.cpp
        src/testing.cpp
        src/zydis.cpp
        src/solvers/SolverParallelTempering.cpp
        src/solvers/SolverSimulatedAnnealing.cpp
        src/solvers/SolverSimpleGreedy.cpp
        src/solvers/SolverParallelGreedy.cpp
        src/solvers/SolverTabuSearch.cpp
        src/solvers/SolverHybridX86.cpp
        #src/starcraft2/starcraft2.c
        src/executors/KVMExecutor.cpp
        src/reporters/cli_reporter.cpp
        src/problems/problem.cpp
        #src/problems/ProblemBARBuildOrder.cpp
        src/problems/ProblemHelloWorld.cpp
        src/problems/ProblemTravellingSalesman.cpp
      #  src/problems/rts_buildorders/game.cpp
      #  src/problems/rts_buildorders/BAR_game/bar_game_config.cpp
       # src/problems/problem_audio_regen.cpp
        src/problems/ProblemX86StringMatch.cpp
        #src/problems/starcraft2_supplymax.cpp
     #   src/problems/x86/calculator.cpp
        src/problems/x86/X86Common.cpp
      #  ${CMAKE_CURRENT_LIST_DIR}/src/problems/rts_buildorders/BAR_game/BAR_extractor_scripts/bar_game_config_metadata.hpp
      #  ${CMAKE_CURRENT_LIST_DIR}/src/problems/rts_buildorders/BAR_game/bar_game_map_data.hpp
     #   src/problems/x86/simple_meta.cpp
      #  src/problems/x86/hybrid_x86_hyperparameters.cpp
       # ${SC2_SRC_FILES}
        )

#add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/src/problems/rts_buildorders/BAR_game/BAR_extractor_scripts/bar_game_config_metadata.hpp
#                    COMMAND ${CMAKE_CURRENT_LIST_DIR}/src/problems/rts_buildorders/BAR_game/BAR_extractor_scripts/create_c_header.sh > ${CMAKE_CURRENT_LIST_DIR}/src/problems/rts_buildorders/BAR_game/BAR_extractor_scripts/bar_game_config_metadata.hpp
#                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src/problems/rts_buildorders/BAR_game/BAR_extractor_scripts/
#                    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/problems/rts_buildorders/BAR_game/BAR_extractor_scripts/create_c_header.sh ${CMAKE_CURRENT_LIST_DIR}/src/problems/rts_buildorders/BAR_game/BAR_extractor_scripts/BAR_extractor.lua)

#add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/src/problems/rts_buildorders/BAR_game/bar_game_map_data.hpp
#                    COMMAND python3 ${CMAKE_CURRENT_LIST_DIR}/src/problems/rts_buildorders/BAR_game/map_extractor.py
#                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src/problems/rts_buildorders/BAR_game/
#                    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/problems/rts_buildorders/BAR_game/map_extractor.py)

add_executable(mlx86 src/main.cpp ${SOURCES})
add_executable(solver_benchmark_test src/tests/solver_benchmark_test.cpp ${SOURCES})
add_executable(testing_test src/tests/testing_test.c ${SOURCES})
add_executable(executor_test src/tests/executor_test.cpp ${SOURCES})
add_executable(bar_buildorder_test src/tests/bar_buildorder_test.cpp ${SOURCES})
add_executable(bar_scrambler_test src/tests/bar_scrambler_test.cpp ${SOURCES})
add_executable(bar_performance_test src/tests/bar_performance_test.cpp ${SOURCES})
add_executable(hybrid_x86_test src/tests/hybrid_x86_test.cpp ${SOURCES})


#find_package( OpenCV REQUIRED )
#include_directories( ${OpenCV_INCLUDE_DIRS} )

set(LIBS_TO_LINK m pthread protobuf-c rt Zydis )
#list(APPEND LIBS_TO_LINK kfr_dft kfr_capi kfr_io)
list(APPEND LIBS_TO_LINK ${OpenCV_LIBS})

target_link_libraries(mlx86 ${LIBS_TO_LINK})
target_link_libraries(solver_benchmark_test ${LIBS_TO_LINK})
target_link_libraries(testing_test ${LIBS_TO_LINK})
target_link_libraries(executor_test ${LIBS_TO_LINK})
target_link_libraries(bar_buildorder_test ${LIBS_TO_LINK})
target_link_libraries(bar_scrambler_test ${LIBS_TO_LINK})
target_link_libraries(bar_performance_test ${LIBS_TO_LINK})
target_link_libraries(hybrid_x86_test ${LIBS_TO_LINK})

add_executable(darwin_2021 src/problems/darwin2021/darwin2021.cpp)