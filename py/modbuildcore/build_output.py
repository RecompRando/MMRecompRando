import os, shutil
from pathlib import Path

from invoke import Context
from .job_base import JobBase
from .utils import invoke_subprocess_run, print_job_header, print_fl

class BuildOutputJob(JobBase):
    test_path: Path
    include_unresolved_jobs: bool
    include_all_resolved_jobs: bool
    
    def __init__(self, test_path: Path):
        super().__init__()
        self.test_path = test_path
        self.include_unresolved_jobs = False
        self.include_all_resolved_jobs = False
        
    def run(self, c: Context):
        print_job_header(f"Build Output Job: {self.test_path}")
        
        os.makedirs(self.test_path, exist_ok=True)
        
        for dst, src in self.get_recursive_mod_outputs(self.include_unresolved_jobs).items():
            if not dst.is_absolute():
                dst = self.test_path.joinpath(dst)
            
            print_fl(f"Copying '{str(src)}' to '{str(dst)}'...")
            shutil.copy(src, dst)
            
        if self.include_all_resolved_jobs:
            for dst, src in self.get_all_resolved_mod_outputs().items():
                if not dst.is_absolute():
                    dst = self.test_path.joinpath(dst)
                
                print_fl(f"Copying '{str(src)}' to '{str(dst)}'...")
                shutil.copy(src, dst)
        
        