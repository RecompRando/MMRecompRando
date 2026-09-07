import toml, pathlib, zipfile, os
from pathlib import Path

from invoke import Context
from .job_base import JobBase
from .utils import invoke_subprocess_run, print_job_header, print_fl
from deep_dict_update import deep_dict_update

class GenerateTomlJob(JobBase):
    toml_path: Path
    data: dict
    
    @classmethod
    def from_merged_dicts(cls, toml_path: Path, data_list: list[dict]):
        final_dict = {}
        for i in data_list:
            final_dict = deep_dict_update(final_dict, i)
            
        return cls(toml_path, final_dict)
        
    def __init__(self, toml_path: Path, data: dict):
        super().__init__()
        self.toml_path = toml_path
        self.data = data
    
    def run(self, c: Context):
        print_job_header(f"Generate Toml Job: {self.toml_path}")
        os.makedirs(self.toml_path.parent, exist_ok=True)
        self.toml_path.write_text(toml.dumps(self.data))

class ModToNRMJob(JobBase):
    mod_tool_path: Path
    toml_path: Path
    build_dir: Path
    delay_read: bool
    nrm_path_fix: bool
    inject_files: dict[Path, Path]
    
    def __init__(self, mod_tool_path: Path, toml_path: Path, build_dir: Path = None, 
                 *, delay_read: bool = False, nrm_path_fix: bool = False, inject_files: dict[Path, Path] = None):
        super().__init__()
        self.mod_tool_path = mod_tool_path    
        self.toml_path = toml_path
        self.build_dir = build_dir
        self.data = None
        self.delay_read = delay_read
        if not delay_read:
            self.read_toml()
            
        self.nrm_path_fix = nrm_path_fix
        self.inject_files = inject_files
        
    
    def read_toml(self):
        self.data = toml.loads(self.toml_path.read_text())
        
        if self.build_dir is None:
            self.build_dir = self.get_elf_path().parent
        
        self.mod_output_files[Path(self.get_output_path().name)] = self.get_output_path()
    
    def get_path_from_toml(self, rel_path: str | Path) -> Path:
        return self.toml_path.parent.joinpath(rel_path).resolve()
    
    def get_elf_path(self) -> Path:
        return self.get_path_from_toml(self.data["inputs"]["elf_path"])
    
    def get_output_path(self) -> Path:
        return self.build_dir.joinpath(self.data["inputs"]["mod_filename"]).with_suffix(".nrm")
    
    def run_nrm_path_fix(self):
        in_zip = zipfile.ZipFile(self.get_output_path(), 'r')
        out_file_path = self.get_output_path().with_suffix(".nrm_temp")        
        out_zip = zipfile.ZipFile(out_file_path, 'w', in_zip.compression)        
        
        for file in in_zip.filelist:
            new_path = file.filename.replace("\\", "/")
            out_zip.writestr(new_path, in_zip.read(file))
        
        in_zip.close()
        out_zip.close()
        
        os.remove(self.get_output_path())
        os.rename(out_file_path, self.get_output_path())
        
    def run_inject_files(self):
        nrm_file = zipfile.ZipFile(self.get_output_path(), 'a', compression=zipfile.ZIP_DEFLATED)
        
        for zip_path, file_path in self.inject_files.items():
            zip_path_string = str(zip_path).replace("\\", "/")
            print_fl(f"NRM File Injection: '{str(file_path)}' as '{zip_path_string}'")
            nrm_file.writestr(zip_path_string, file_path.read_bytes())
        
        nrm_file.close()
    
    def run(self, c: Context):
        print_job_header(f"Mod To NRM Job (Path Fix = {self.nrm_path_fix}, File Injection = {bool(self.inject_files)}): {self.toml_path}")
        
        if self.delay_read and self.data is None:
            self.read_toml()
        
        invoke_subprocess_run(c, True,
            [self.mod_tool_path, self.toml_path, self.build_dir]
        )
        
        if self.nrm_path_fix:
            self.run_nrm_path_fix()
            
        if self.inject_files is not None:
            self.run_inject_files()