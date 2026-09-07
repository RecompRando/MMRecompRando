from invoke import Context
from pathlib import Path

## Some older version of python don't like the self-referential annotation. This is a work-around.
class JobBase:
    ...
    
class JobBase:
    # Class
    _resolved_jobs: list[JobBase] = []
    
    @classmethod
    def get_all_resolved_mod_outputs(cls) -> dict[Path, Path]:
        retVal = {}
        for i in cls._resolved_jobs:
            retVal.update(i.mod_output_files)
        
        return retVal
    
    # Instance:
    _has_been_resolved: bool
    no_duplication: bool
    dependencies: list[JobBase]
    mod_output_files: dict[Path, Path]
    
    def __init__(self):
        self._has_been_resolved = False
        self.no_duplication = True
        self.dependencies = []
        self.mod_output_files = {}
    
    # Overridable Functions:
    def needs_to_run(self, c: Context) -> bool:
        return True
    
    def run(self, c: Context):
        pass
    
    # Not to override:
    def depends_on(self, new_dependencies: list[JobBase]) -> JobBase:
        self.dependencies.extend(new_dependencies)
        return self
    
    def resolve(self, c: Context, skip_dependencies: bool = False):            
        if self.no_duplication and self._has_been_resolved:
            return
        
        if self.needs_to_run(c):
            if not skip_dependencies:
                for i in self.dependencies:
                    i.resolve(c)
            self.run(c)
        
        self._has_been_resolved = True
        self._resolved_jobs.append(self)
        
    def get_recursive_mod_outputs(self, include_unresolved_jobs: bool = True, root_call: bool = True) -> dict[Path, Path]:
        if not (root_call or self._has_been_resolved or include_unresolved_jobs):
            return {}
        
        retVal = self.mod_output_files
        
        for i in self.dependencies:
            retVal.update(i.get_recursive_mod_outputs(include_unresolved_jobs, False))
        
        return retVal
    
    def add_mod_output_files(self, files: dict[Path, Path]):
        self.mod_output_files.update(files)