import pathlib, subprocess, os, shutil, tomllib, zipfile

USING_ASSETS_ARCHIVE = True

project_root = pathlib.Path(__file__).parent

mod_data = tomllib.loads(project_root.joinpath("mod.toml").read_text())
mod_manifest_data = mod_data["manifest"]
# print(mod_data)
build_dir = project_root.joinpath(f"build")

rando_dir = project_root.joinpath("mm_recomp_rando")
build_nrm_file = rando_dir.joinpath(f"{mod_data['inputs']['mod_filename']}.nrm")

extlib_dir = project_root.parent.joinpath("MMRecompAPCppGlue")
extlib_build_dir = extlib_dir.joinpath("build_mod")
extlib_dll_file = extlib_build_dir.joinpath("RelWithDebInfo/APCpp-Glue.dll")

runtime_mods_dir = project_root.joinpath("runtime/mods")
runtime_nrm_file = runtime_mods_dir.joinpath(f"{mod_data['inputs']['mod_filename']}.nrm")
runtime_dll_file = runtime_mods_dir.joinpath(f"APCpp-Glue.dll")

def run_build():
    os.makedirs(extlib_build_dir, exist_ok=True)
    
    dll_config = subprocess.run(
        [
            shutil.which("cmake"),
            ".."
        ],
        cwd=extlib_build_dir
    )
    if dll_config.returncode != 0:
        raise RuntimeError("CMake failed to configure mod extlibs.")
    
    dll_build = subprocess.run(
        [
            shutil.which("cmake"),
            "--build",
            ".",
            "-j16",
            "--config",
            "RelWithDebInfo"
        ],
        cwd=extlib_build_dir
    )
    if dll_build.returncode != 0:
        raise RuntimeError("CMake failed to build mod extlibs.")
    
    make_run = subprocess.run(
        [
            shutil.which("make"),
        ],
        cwd=pathlib.Path(__file__).parent
    )
    if make_run.returncode != 0:
        raise RuntimeError("Make failed to build mod binaries.")


    # Copying files for debugging:
    os.makedirs(runtime_mods_dir, exist_ok=True)
    shutil.copy(build_nrm_file, runtime_nrm_file)
    shutil.copy(extlib_dll_file, runtime_dll_file)

if __name__ == '__main__':
    run_build()