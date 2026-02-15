import pathlib, os, shutil, urllib.request, urllib.parse
from pathlib import Path

from invoke import Context
from .job_base import JobBase
from .utils import print_job_header

class DownloadJob(JobBase):
    url: str
    download_path: Path
    force: bool
    
    def __init__(self, url: str, download_path: Path, *, append_url_filename: bool=True):
        super().__init__()
        self.url = url
        self.download_path = download_path
        self.force = False
        
        if (append_url_filename):
            self.download_path = self.download_path.joinpath(self.get_filename_from_url())
        
        
    def get_filename_from_url(self) -> Path:
        parsed_url = urllib.parse.urlparse(self.url)
        return Path(os.path.basename(parsed_url.path))
    
    # Override:
    def needs_to_run(self, c: Context):
        retVal = self.force or not self.download_path.exists()
        if not retVal:
            print_job_header(f"Download Job: {self.download_path} already downloaded.")
        return retVal
        
    
    def run(self, c):
        print_job_header(f"Download Job: {self.url} to {self.download_path}")
        if not self.download_path.parent.exists():
            os.makedirs(self.download_path.parent)
            
        urllib.request.urlretrieve(
            self.url,
            self.download_path
        )