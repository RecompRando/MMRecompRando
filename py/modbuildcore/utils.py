
import sys, subprocess, re
from pathlib import Path

from invoke import Context
from colors import *

def slugify(text: str) -> str:
    text = text.strip()
    text = re.sub(r'[\s_]+', '_', text)
    text = re.sub(r'[^a-zA-Z0-9_]', '', text)
    return text

def print_fl(*args, **kwargs):
    print(*args, flush=True, **kwargs)

def print_color(fg: str, *args, **kwargs):
    p_list = []
    for i in args:
        p_list.append(color(str(i), fg=fg))

    print_fl(*p_list, **kwargs)

def print_error(*args, **kwargs):
    print_color("red", *args, **kwargs)

def print_warning(*args, **kwargs):
    print_color("yellow", *args, **kwargs)

def print_task_header(*args, **kwargs): 
    print_color('green', "\n-> ", *args, **kwargs)
    
def print_job_header(*args, **kwargs):
    print_color('blue', f"\n--> ", *args, **kwargs)
    
# For a couple different reasons (primarily related to cross-platform compatability),
# it will usually be better for us to use subprocess instead of shell commands.
# This is a convienient helper function to make subprocess work better with the
# invoke content:
def invoke_subprocess_run(c: Context, required: bool, *args, **kwargs) -> subprocess.CompletedProcess:
    cwd: str = c.cwd
    echo: bool = c.config['run']['echo']
    echo_format: str = c.config['run']['echo_format']
    warn: bool = c.config['run']['warn']
    dry: bool = c.config['run']['dry']
    
    subprocess_str = str(args[0])
    
    if echo:
        print(echo_format.replace("{command}", f"subprocess.run({str(subprocess_str)})"))
    
    if dry:
        return None
    
    if 'cwd' not in kwargs:
        kwargs['cwd'] = Path(cwd)

    result: subprocess.CompletedProcess = subprocess.run(*args, **kwargs)
    
    if result.returncode != 0:
        if warn or not required:
            print_warning(f"WARNING! Command '{subprocess_str}' returned non-zero exit status {result.returncode}.")
        else:
            print_error(f"FATAL! Command '{subprocess_str}' returned non-zero exit status {result.returncode}. Aborting...")
            sys.exit(1)
    
    return result