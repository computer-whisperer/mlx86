from pathlib import Path

env = Environment(CPPPATH=[], LIBS=["m", "pthread", "protobuf-c"])
env["CFLAGS"] = ["-g"]
env["LINKFLAGS"] = ["-g"]
env.VariantDir('build', 'src', duplicate=1)
 
env["CPPPATH"] = ["src/", "src/starcraft2/proto/"]
 
common_cfiles = ["src/utils.c"]
common_cfiles += ["src/solvers/parallel_tempering.c", "src/solvers/simulated_annealing.c"]
common_cfiles += ["src/starcraft2/starcraft2.c"]
common_cfiles += ["src/executors/kvm_executor.c"]
common_cfiles += Glob("src/starcraft2/proto/s2clientprotocol/*.c", strings=True, source=True)
common_cfiles += ["src/problems/common.c", 
                  "src/problems/hello_world.c", 
                  "src/problems/starcraft2_supplymax.c", 
                  "src/problems/x86/calculator.c",
                  "src/problems/x86/common.c",
                  "src/problems/x86/simple_meta.c"]

env.Program(target="build/mlx86", source=["src/main.c", common_cfiles]);