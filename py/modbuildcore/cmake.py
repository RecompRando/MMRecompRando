import shutil, os
from pathlib import Path

from invoke import Context
from .job_base import JobBase
from .utils import invoke_subprocess_run, print_job_header

# Class declaration before definition:
class CMakeProjectConfig:
    cmake_binary_path: Path
    project_working_dir: Path
    extended_env: dict[str, str]
    
    def __init__(self, project_working_dir: Path, expanded_env: dict[str, str], *, cmake_binary_path: Path = None):
        if cmake_binary_path is None:
            self.cmake_binary_path = shutil.which("cmake")
        else:
            self.cmake_binary_path = cmake_binary_path
        self.project_working_dir = project_working_dir
        self.extended_env = expanded_env


class CMakeBuildJob(JobBase):
    config_args: list[str]
    build_args: list[str]
    
    def __init__(self, cmake_project: CMakeProjectConfig, output_files: dict[Path, Path], config_args: list[str], build_args: list[str]):
        super().__init__()
        self.cmake_project = cmake_project
        self.mod_output_files = output_files
        self.config_args = config_args
        self.build_args = build_args
        
        
    @classmethod
    def from_preset_pair(cls, cmake_project: CMakeProjectConfig, output_files: dict[Path, Path], config_preset_name: str, build_preset_name: str = None):
        if build_preset_name is None:
            build_preset_name = config_preset_name
        
        return cls(
            cmake_project,
            output_files,
            ["--preset", config_preset_name, cmake_project.project_working_dir],
            ["--build", "--preset", build_preset_name]
        )
        
    def run_configure(self, c: Context):
        print_job_header(f"CMake Configure: {self.config_args}:")
        cmake_env = os.environ.copy()
        cmake_env.update(self.cmake_project.extended_env)
        
        invoke_subprocess_run(c, True,
            [self.cmake_project.cmake_binary_path] + self.config_args,
            env=cmake_env,
            cwd=self.cmake_project.project_working_dir
        )
    
    def run_build(self, c: Context):
        print_job_header(f"CMake Build: {self.build_args}:")
        cmake_env = os.environ.copy()
        cmake_env.update(self.cmake_project.extended_env)
        
        invoke_subprocess_run(c, True,
            [self.cmake_project.cmake_binary_path] + self.build_args,
            env=cmake_env,
            cwd=self.cmake_project.project_working_dir
        )

    def run(self, c: Context):
        self.run_configure(c)
        self.run_build(c)
