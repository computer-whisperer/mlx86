cmake_minimum_required(VERSION 3.20)
project(mlx86 C)

set(CMAKE_C_STANDARD 99)

include_directories(
        src
        src/executors
        src/problems
        src/problems/x86
        src/reporters
        src/solvers
        src/tests
        src/starcraft2
        src/starcraft2/proto
        src/starcraft2/proto/s2clientprotocol
)

file(GLOB SC2_SRC_FILES src/starcraft2/proto/s2clientprotocol/*.c)

set(SOURCES
        src/utils.c
        src/utils.c
        src/testing.c
        src/zydis.c
        src/solvers/parallel_tempering.c
        src/solvers/simulated_annealing.c
        src/solvers/simple_greedy.c
        src/solvers/tabu_search.c
        src/solvers/hybrid_x86.c
        src/starcraft2/starcraft2.c
        src/executors/kvm_executor.c
        src/reporters/cli_reporter.c
        src/problems/common.c
        src/problems/hello_world.c
        src/problems/travelling_salesman.c
        src/problems/starcraft2_supplymax.c
        src/problems/x86/calculator.c
        src/problems/x86/common.c
        src/problems/x86/simple_meta.c
        src/problems/x86/hybrid_x86_hyperparameters.c
        ${SC2_SRC_FILES}
        )


add_executable(mlx86 src/main.c ${SOURCES})
add_executable(solver_benchmark_test src/tests/solver_benchmark_test.c ${SOURCES})
add_executable(testing_test src/tests/testing_test.c ${SOURCES})
add_executable(executor_test src/tests/executor_test.c ${SOURCES})

set(LIBS_TO_LINK  m pthread protobuf-c rt Zydis)

target_link_libraries(mlx86 ${LIBS_TO_LINK})
target_link_libraries(solver_benchmark_test ${LIBS_TO_LINK})
target_link_libraries(testing_test ${LIBS_TO_LINK})
target_link_libraries(executor_test ${LIBS_TO_LINK})