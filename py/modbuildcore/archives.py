import pathlib, os, shutil
from pathlib import Path

from invoke import Context
from .job_base import JobBase
from .utils import print_job_header
        
class ArchiveExtractJob(JobBase):
    archive_path: str
    extract_dir: Path
    force: bool
    
    def __init__(self, archive_path: Path, extract_dir: Path):
        super().__init__()
        self.archive_path = archive_path
        self.extract_dir = extract_dir
        self.force = False
        
    def needs_to_run(self, c: Context) -> bool:
        retVal = self.force or not self.extract_dir.exists()
        if not retVal:
            print_job_header(f"Archive Extraction Job: {self.extract_dir} already exists.")
        return retVal
    
    def run(self, c: Context):
        print_job_header(f"Archive Extraction Job: {self.archive_path} to {self.extract_dir}")
        if not self.extract_dir.parent.exists():
            os.makedirs(self.extract_dir.parent)
            
        shutil.unpack_archive(self.archive_path, self.extract_dir)