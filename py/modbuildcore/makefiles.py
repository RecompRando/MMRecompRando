import shutil, os
from pathlib import Path

from invoke import Context
from .job_base import JobBase

from .utils import invoke_subprocess_run, print_job_header
        
class MakefileJob(JobBase):
    make_binary_path: Path
    makefile_path: Path
    extended_env: dict[str, str]
    additional_args: list[str]
    
    def __init__(self, makefile_path: Path, extended_env: dict[str, str], *, make_binary_path: Path = None, make_cwd: Path = None, additional_args = None):
        super().__init__()
        if make_binary_path is None:
            self.make_binary_path = shutil.which("make")
        else:
            self.make_binary_path = make_binary_path
        self.makefile_path = makefile_path
        self.extended_env = extended_env
        
        self.make_cwd = make_cwd
        if self.make_cwd is None:
            self.make_cwd = os.getcwd()

        self.additional_args = additional_args
        if self.additional_args is None:
            self.additional_args = []
        
    def run(self, c: Context):
        print_job_header(f"Makefile Job: {self.makefile_path}")
        make_env = os.environ.copy()
        make_env.update(self.extended_env)
        
        invoke_subprocess_run(c, True,
            [self.make_binary_path, "-f", self.makefile_path] + self.additional_args,
            env=make_env,
            cwd=self.make_cwd
        )