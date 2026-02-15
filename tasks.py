## This file is a `tasks.py` file for the pyinvoke library, which modbuild.py is powered by. 
## See this URL for documentation: https://docs.pyinvoke.org/en/stable/

import sys
if __name__ == '__main__':
    print(f"Wrong file! This is the pyinvoke tasks file for the `modbuild.py` tool. Run `python[3] ./modbuild.py` to use it.")
    sys.exit(0)

import shutil, os, json
from pathlib import Path

from modbuildcore.jobs import *
from modbuildcore.utils import *

from invoke import Context, task, call

try: 
    import user_project as p # type: ignore
except ImportError as e:
    import project as p

ARG_SPLIT_CHAR = ","

def print_task_header(*args, **kwargs): 
    print_color('green', "\n-> ", *args, **kwargs)
    

@task(help={
    'skip_dependencies': "Do not try to resolve dependency jobs.",
    'force': "Redownloads any previously downloaded files.",
    'name': f"Only download specific files. Names should be the keys used in `project.downloads`, separated by '{ARG_SPLIT_CHAR}'.",
    'list': f"List all DownloadJob names in `project.downloads`, then exit."
})
def download(c: Context, skip_dependencies: bool = False, force: bool = False, name: str = None, list: bool = False):
    """
    Runs the downloads defined in `project.downloads`. 
    Entries in `project.downloads` should be instances of `modbuildcore.downloads.DownloadJob`. 
    
    By default, downloads will be skipped (and considered unresolved) if the destination file already exists.
    
    Unless manually specified, downloaded files do not count as mod_output_files and are not collected by BuildOutputJobs or ThunderstorePackageJobs.
    """
    if list:
        print_task_header("Listing download job names:")
        for key in p.downloads.keys():
            print_fl(key)
        return
    
    print_task_header("Performing downloads...")
    
    dl_list : list[DownloadJob] = None
    if name is None:
        dl_list = p.downloads.values()
    else:
        dl_list = [p.downloads[i] for i in name.split(ARG_SPLIT_CHAR)]
    
    for download in dl_list:
        download.force = force
        download.resolve(c, skip_dependencies)
    
@task(help={
    'skip_dependencies': "Do not try to resolve dependency jobs.",
    'force': "Re-extract archives even if the output folder already exists",
    'name': f"Only extract specific archives. Names should be the keys used in `project.archive_extractions`, separated by '{ARG_SPLIT_CHAR}'.",
    'list': f"List all ArchiveExtractJob names in `project.archive_extractions`, then exit."
})
def extract(c: Context, skip_dependencies: bool = False, force: bool = False, name: str = None, list: bool = False):
    """
    Runs the archive extractions defined in `project.archive_extractions`. 
    Entries in `project.archive_extractions` should be instances of `modbuildcore.archives.ArchiveExtractJob`. 
    
    By default, extractions will be skipped (and considered unresolved) if the destination folder already exists.
    
    Unless manually specified, extracted files/folders do not count as'mod_output_files and are not collected by BuildOutputJobs or ThunderstorePackageJobs.
    """
    if list:
        print_task_header("Listing extraction names:")
        for key in p.archive_extractions.keys():
            print_fl(key)
        return
    
    print_task_header("Extracting archives...")
    
    extract_list : list[ArchiveExtractJob] = None
    if name is None:
        extract_list = p.archive_extractions.values()
    else:
        extract_list = [p.archive_extractions[i] for i in name.split(ARG_SPLIT_CHAR)]
    
    for extraction in extract_list:
        extraction.force = force
        extraction.resolve(c, skip_dependencies)

@task(help={
    'skip_dependencies': "Do not try to resolve dependency jobs.",
    'name': f"Only run specific makefile configurations. Names should be the keys used in `project.makefiles`, separated by '{ARG_SPLIT_CHAR}'.",
    'list': f"List all MakefileJob names in `project.makefiles`, then exit."
})
def makefile(c: Context, skip_dependencies: bool = False, name: str = None, list: bool = False):
    """
    Builds the makefile configurations defined in `project.makefiles`.
    Entries in `project.makefiles` should be instances of `modbuildcore.makefiles.MakefileJob`. 
    """
    if list:
        print_task_header("Listing makefile names:")
        for key in p.makefiles.keys():
            print_fl(key)
        return
    
    print_task_header("Running makefiles...")
    
    makefile_list : list[MakefileJob] = None
    if name is None:
        makefile_list = p.makefiles.values()
    else:
        makefile_list = [p.makefiles[i] for i in name.split(ARG_SPLIT_CHAR)]
    
    for makefile in makefile_list:
        makefile.resolve(c, skip_dependencies)

@task(
    help={
        'skip_dependencies': "Do not try to resolve dependency jobs.",
        'name': f"Only generate specific registered .toml files. " \
            f"Names should be the keys used in `project.tomls`, separated by '{ARG_SPLIT_CHAR}'.",
        'list': f"List all GenerateTomlJob names in `project.tomls`, then exit."
    }
)
def toml(c: Context, skip_dependencies: bool = False, name: str = None, list: bool = False):
    """
    Generate .toml files, as specified in `project.tomls`.
    Entries in `project.tomls` should be instances of `modbuildcore.tomls.GenerateTomlJob`. 
    """
    if list:
        print_task_header("Listing mod .toml names:")
        for key in p.tomls.keys():
            print_fl(key)
        return
    
    print_task_header("Generating .toml files...")
    
    toml_list : list[GenerateTomlJob] = None
    if name is None:
        toml_list = p.tomls.values()
    else:
        toml_list = [p.tomls[i] for i in name.split(ARG_SPLIT_CHAR)]
    
    for mod in toml_list:
        mod.resolve(c, skip_dependencies)

@task(
    help={
        'skip_dependencies': "Do not try to resolve dependency jobs.",
        'name': f"Only generate specific registered .nrm files. " \
            f"Names should be the keys used in `project.nrms`, separated by '{ARG_SPLIT_CHAR}'.",
        'path_fix_force': "EXPERIMENTAL (AND NOT ENDORSED BY WISEGUY)! Reconstructs the .nrm file " \
            "after RecompModTool finishes in order to eliminate backslashes from filepaths.",
        'list': f"List all ModTomlJob names in `project.nrms`, then exit."
    }
)
def nrm(c: Context, skip_dependencies: bool = False, name: str = None, path_fix_force: bool = False, list: bool = False):
    """
    Builds .nrm files, as specified in `project.nrms`. The resultant .nrms are counted as 'mod_output_files'.
    Entries in `project.nrms` should be instances of `modbuildcore.tomls.ModToNRMJob`. 
    """
    if list:
        print_task_header("Listing mod NRM names:")
        for key in p.nrms.keys():
            print_fl(key)
        return
    
    print_task_header("Building NRM files...")
    
    nrm_list : list[ModToNRMJob] = None
    if name is None:
        nrm_list = p.nrms.values()
    else:
        nrm_list = [p.nrms[i] for i in name.split(ARG_SPLIT_CHAR)]
    
    for mod in nrm_list:
        if path_fix_force:
            mod.nrm_path_fix = True
        mod.resolve(c, skip_dependencies)

@task(help={
    'skip_dependencies': "Do not try to resolve dependency jobs.",
    'group_name': f"Build selected groups by name. Group names should be the keys used in `project.cmake_build_groups`, separated by '{ARG_SPLIT_CHAR}'.",
    'build_name': "Only run specific builds within selected groups. Build names should be the keys used in "\
        f"`project.cmake_build_groups[group]`, separated by '{ARG_SPLIT_CHAR}'. Will error if any build name is not in all specified groups.",
    'list': f"List all CMakeBuildJob groups and names in `project.cmake_build_groups`, then exit."
})
def cmake(c: Context, skip_dependencies: bool = False, group_name: str = None, build_name: str = None, list: bool = False):
    """
    Run CMakeBuildJobs by group, as specified in  `project.cmake_build_groups`. If no group argument is specified, all groups are run.
    Build groups are entries in `project.cmake_build_groups` and should be `dict[str, modbuildcore.cmake.CMakeBuildJobs]`.
    
    CMakeBuildJobs have 'mod_output_files' specified on creation.
    """
    if list:
        print_task_header("Listing CMake build groups and names:")
        for group_name, group in p.cmake_build_groups.items():
            print_fl(f"{group_name}")
            for build_name in group.keys():
                print_fl(f"\t{build_name}")
        return
    
    print_task_header("Running CMake builds:")
    
    selected_groups: dict[str, dict[str, CMakeBuildJob]] = {}

    if group_name is not None:
        for i in group_name.split(ARG_SPLIT_CHAR):
            selected_groups[i] = p.cmake_build_groups[i]
    else:
        selected_groups = p.cmake_build_groups
        
    for group_key, group in selected_groups.items():
        if build_name is None:
            for build_key, build_job in  group.items():
                build_job.resolve(c, skip_dependencies)
        else:
            for build_key, build_job in [(bkey, group[bkey]) for bkey in build_name.split(ARG_SPLIT_CHAR)]:
                build_job.resolve(c, skip_dependencies)
                    
@task (
    default=True,
    help={
        'skip_dependencies': "Do not try to resolve dependency jobs.",
        'name': "Update only selected build output folders. ames should be the keys used in `project.mod_tomls`, separated by '{ARG_SPLIT_CHAR}'.",
        'list': f"List all BuildOutputJob names in `project.build_outputs`, then exit."
    }
)
def build(c: Context, skip_dependencies: bool = False, unresolved_jobs: bool = False, all_resolved_jobs: bool = False, name: str=None, list: bool = False):
    """
    Updates BuildOutputJob folder(s) with their defined mod_output_files, and the output files from any jobs they depend on, as specified in `project.build_outputs`. 
    Entries in `project.build_outputs` should be instances of `modbuildcore.build_output.BuildOutputJob`. 
    
    By default, only dependency jobs that have been resolved this execution will have their files updated. 
    Use the `unresolved_jobs` argument to override this behavior. Alternatively, skipping dependencies will result in
    no dependent jobs having their files included.
    
    It is also possible to include output files from jobs that aren't dependencies a BuildOutputJob. The argument `all_resolved_jobs`
    will result in the output files from any job previously resolved in this execution.
    """
    if list:
        print_task_header("Listing build output folder names:")
        for key in p.build_outputs.keys():
            print_fl(key)
        return
    
    print_task_header("Preparing build output folders...")
    
    test_dir_list : list[BuildOutputJob] = None
    if name is None:
        test_dir_list = p.build_outputs.values()
    else:
        test_dir_list = [p.build_outputs[i] for i in name.split(ARG_SPLIT_CHAR)]
    
    for test_dir in test_dir_list:
        test_dir.include_unresolved_jobs = unresolved_jobs
        test_dir.include_all_resolved_jobs = all_resolved_jobs
        test_dir.resolve(c, skip_dependencies)

@task(
    help={
        'name': f"Only write manifests for specific Thunderstore packages. Names should be the keys used in `project.thunderstore_packages`, separated by '{ARG_SPLIT_CHAR}'.",
        'output_file': "Path to write the file to. Defaults to the manifest name in the current working directory.",
        'list': f"List all ThunderstorePackageJob names in `project.thunderstore_packages`, then exit."
    }
)
def manifest(c: Context, name: str = None, output_file: str = None, list: bool = False):
    """
    Write the manifest.json file for a ThunderstorePackageJob, as specified in `project.thunderstore_packages`.
    Entries in `project.thunderstore_packages` should be instances of `modbuildcore.thunderstore.ThunderstorePackageJob`. 
    
    This command primarily exists for use with the GitHub actions CI, and has minimal direct use.
    
    This command does not resolve any jobs.
    """
    if list:
        print_task_header("Listing Thunderstore Package names:")
        for key in p.thunderstore_packages.keys():
            print_fl(key)
        return
    
    package_list: list[ThunderstorePackageJob] = None
    if name is None:
        package_list = p.thunderstore_packages.values()
    else:
        package_list = [p.thunderstore_packages[i] for i in name.split(ARG_SPLIT_CHAR)]
        
    for package in package_list:
        if output_file is None:
            output = Path(package.manifest['name']).with_suffix(".json")
        else:
            output = Path(output_file)
            
        output.write_text(json.dumps(package.manifest, indent=4))


@task (
    help={
        'skip_dependencies': "Do not try to resolve dependency jobs. This will not effect included mod_output_files",
        'name': f"Only build specific Thunderstore packages. Names should be the keys used in `project.thunderstore_packages`, separated by '{ARG_SPLIT_CHAR}'. Do not use the package name in the Thunderstore manifest.",
        'list': f"List all ThunderstorePackageJob names in `project.package_list`, then exit."
    }
)
def thunderstore(c: Context, skip_dependencies: bool = False, name: str = None, list: bool = False):
    """
    Creates Thunderstore package zip archives, as specified in `project.thunderstore_packages`.
    Entries in `project.thunderstore_packages` should be instances of `modbuildcore.thunderstore.ThunderstorePackageJob`. 
    
    Unlike updating build outputs, Thunderstore packages will gather files from ALL dependent jobs, regardless of whether
    or not they were resolved this execution.
    """
    
    if list:
        print_task_header("Listing Thunderstore Package names:")
        for key in p.thunderstore_packages.keys():
            print_fl(key)
        return
    
    
    print_task_header("Preparing Thunderstore packages..")
    package_list: list[ThunderstorePackageJob] = None
    if name is None:
        package_list = p.thunderstore_packages.values()
    else:
        package_list = [p.thunderstore_packages[i] for i in name.split(ARG_SPLIT_CHAR)]
        
    for package in package_list:
        package.resolve(c, skip_dependencies)


@task (
    pre=[download, extract, makefile, toml, nrm, cmake, build, thunderstore]
)
def all(c: Context):
    """
    Shortcut for `./modbuild.py download extract makefile nrm cmake build thunderstore`.
    
    In effect, run all declared jobs.
    """
    pass


@task
def clean(c: Context):
    """
    Deletes files and folders specified in `project.clean_paths`. Used for deleting build folders.
    """
    print_task_header("Cleaning...")
    for path in p.clean_paths:
        if path.is_file():
            os.remove(path)
            print_fl(f"Deleted {path}")
        elif path.is_dir():
            shutil.rmtree(path)
            print_fl(f"Deleted {path}")
        else:
            print_fl(f"Could not delete {path}")
            
@task(
    pre=[clean]
)
def distclean(c: Context):
    """
    Deletes files and folders specified in `project.clean_paths` and `project.distclean_paths`. Used for deleting build folders and downloaded artifacts.
    """
    print_task_header("Distcleaning...")
    
    for path in p.distclean_paths:
        print_fl(f"Deleting '{path}'...")
        if path.is_file():
            os.remove(path)
        elif path.is_dir():
            shutil.rmtree(path)
        else:
            print_fl(f"Could not delete {path}")
