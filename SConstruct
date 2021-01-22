from pathlib import Path

env = Environment(CPPPATH=[], LIBS=["m", "pthread", "protobuf-c"])
env["CFLAGS"] = ["-g"]
env["LINKFLAGS"] = ["-g"]
env.VariantDir('build', 'src', duplicate=1)
 
env["CPPPATH"] = ["src/", "src/starcraft2/proto/"]
 
common_cfiles = ["src/utils.c", "src/testing.c"]
common_cfiles += ["src/solvers/parallel_tempering.c", 
                  "src/solvers/simulated_annealing.c", 
                  "src/solvers/simple_greedy.c",
                  "src/solvers/tabu_search.c"]
common_cfiles += ["src/starcraft2/starcraft2.c"]
common_cfiles += ["src/executors/kvm_executor.c"]
common_cfiles += ["src/reporters/cli_reporter.c"]
common_cfiles += Glob("src/starcraft2/proto/s2clientprotocol/*.c", strings=True, source=True)
common_cfiles += ["src/problems/common.c", 
                  "src/problems/hello_world.c", 
                  "src/problems/travelling_salesman.c", 
                  "src/problems/starcraft2_supplymax.c",
                  "src/problems/simulated_annealing_hyperparameters.c",
                  "src/problems/parallel_tempering_hyperparameters.c", 
                  "src/problems/x86/calculator.c",
                  "src/problems/x86/common.c",
                  "src/problems/x86/simple_meta.c"]

env.Program(target="build/mlx86", source=["src/main.c", common_cfiles]);
env.Program(target="build/testing_test", source=["src/tests/testing_test.c", common_cfiles]);
env.Program(target="build/executor_test", source=["src/tests/executor_test.c", common_cfiles]);
env.Program(target="build/sudoku_test", source=["src/tests/sudoku_test.c", common_cfiles]);