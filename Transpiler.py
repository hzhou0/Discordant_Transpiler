# global vars
from File import *
import os
import subprocess
import argparse
import sys
import platform
import pickle
import time


# x = File("test_proj/discordance.dis")
# .process()
# print(x.string)
def resource_path(relative_path):
    """ Get absolute path to resource, works for dev and for PyInstaller """
    try:
        # PyInstaller creates a temp folder and stores path in _MEIPASS
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")
    return os.path.join(base_path, relative_path)


def src_changed(path):
    """
    return true if the source file's last modified date
    is after the object file's last modified date,
    else return false
    :param path: the path to the source file
    """
    try:
        src_mtime = os.path.getmtime(path)
        if os.path.exists(os.path.splitext(path)[0] + ".dis"):
            src_mtime = os.path.getmtime(os.path.splitext(path)[0] + ".dis")
        obj_mtime = os.path.getmtime(
            os.path.join(
                os.path.split(path)[0],
                ".obj", os.path.splitext(os.path.split(path)[1])[0] + ".o")
        )
    except:
        return True
    return src_mtime > obj_mtime


class project:
    """
    Represents a collection of Files required
    to assemble an executable
    """

    def __init__(self, root_file_address, include_dir=None, output_exe=None):
        self.root = root_file_address
        if not output_exe:
            if platform.system() == 'Windows':
                output_exe = os.path.splitext(root_file_address)[0] + ".exe"
            else:
                output_exe = os.path.splitext(root_file_address)[0]
        self.output_exe = output_exe
        if not include_dir:
            include_dir = []
        include_dir.append(os.path.dirname(self.root))
        include_dir.append(resource_path(""))
        # directories to search in
        self.include_dirs = include_dir
        # header file paths linked in the project
        self.hds_path = []
        # source file paths linked in the project
        self.srcs_path = []
        # all file source paths to compile
        self.compiles_path = []
        # all File classes processed to find include paths for headers
        self.file_objs = []

    def find(self, name):
        """
        Search for files of name under
        given directories
        :param name: the name of the file
        :return: first match of file
        """
        for i in self.include_dirs:
            if os.path.isfile(os.path.join(i, name)):
                return os.path.join(i, name)
        raise ValueError("file \"" + name + "\" could not be found")

    def update_register(self):
        register_path = resource_path("register")
        if os.path.exists(register_path):
            project_register = open(register_path, 'rb')
        else:
            temp = open(register_path, 'wb')
            pickle.dump({}, temp)
            temp.close()
            project_register = open(register_path, 'rb')
        # load dict of projects
        projects = pickle.load(project_register)
        project_register.close()
        # update last modified time of current project
        projects[self.output_exe] = time.time()
        # push to pickle file
        project_register = open(register_path, 'wb')
        pickle.dump(projects, project_register)
        project_register.close()

    def find_chg_hds(self):
        """
        Find changed headers
        .dis files count as headers, as they can be included
        Headers are counted as changed when they have been modified
        more recently then the last time the project was built
        """
        register_path = resource_path("register")
        if os.path.exists(register_path):
            project_register = open(register_path, 'rb')
        else:
            temp = open(register_path, 'wb')
            pickle.dump({}, temp)
            temp.close()
            project_register = open(register_path, 'rb')
        projects = pickle.load(project_register)
        if self.output_exe in projects:
            mtime = projects[self.output_exe]
        else:
            mtime = time.time()
        changed = []
        for hd in self.hds_path:
            hd_mtime = os.path.getmtime(hd)
            if os.path.exists(os.path.splitext(hd)[0] + ".dis"):
                hd_mtime = os.path.getmtime(os.path.splitext(hd)[0] + ".dis")
            if hd_mtime > mtime:
                changed.append(hd)
        # if any headers includes changed headers, they are also changed
        for hd in changed:
            hd_local = os.path.split(hd)[1]
            for obj in self.file_objs:
                if hd_local in obj.includes:
                    if os.path.splitext(obj.address)[1] == (".h" or ".dis"):
                        changed.append(os.path.splitext(obj.address)[0] + ".h")
        return changed

    def find_compile_files(self):
        """
        find all changed files and append to self.compile_files
        """
        self.compiles_path = []
        for file in self.srcs_path:
            if src_changed(file):
                self.compiles_path.append(file)
        changed_headers = self.find_chg_hds()
        for hd in changed_headers:
            hd = os.path.split(hd)[1]
            for obj in self.file_objs:
                if hd in obj.includes:
                    if os.path.splitext(obj.address)[1] == ".h":
                        pass
                    elif os.path.splitext(obj.address)[1] == ".dis":
                        self.compiles_path.append(os.path.splitext(obj.address)[0] + ".cpp")
                    else:
                        self.compiles_path.append(obj.address)

    def transpile(self, address=None):
        """
        Recursively transpile all Files
        under projects
        :param address: used to call recursively for all
        included files in main file
        """
        # address of first file
        if not address:
            address = self.root
        if address in self.srcs_path or address in self.hds_path:
            return
        file = File(address)
        file.process()
        # if compiling a .dis file
        if address.split(".")[-1] == "dis":
            # clean previous .h files
            if os.path.isfile(os.path.splitext(address)[0] + ".h"):
                os.remove(os.path.splitext(address)[0] + ".h")
            if os.path.isfile(os.path.splitext(address)[0] + ".cpp"):
                os.remove(os.path.splitext(address)[0] + ".cpp")
            # push processed file onto disk
            file.sync(address + "_")
            # split processed file into source and header
            if platform.system() == 'Windows':
                lazycpp = os.path.join("lazycpp", "lazycpp.exe")
            elif platform.system() == 'Darwin':
                lazycpp = os.path.join("lazycpp", "mac_lazycpp")
            elif platform.system() == 'Linux':
                lazycpp = os.path.join("lazycpp", "linux_lazycpp")
            else:
                raise ValueError("Unsupported Operating System")
            subprocess.call([resource_path(lazycpp),
                             "-o", os.path.dirname(address),
                             os.path.normpath(address + "_")], shell=True)
            # remove temp processed file
            os.remove(address + "_")
            # change address of .dis file to .cpp to feed to gcc
            self.srcs_path.append(os.path.splitext(address)[0] + ".cpp")
            self.hds_path.append(os.path.splitext(address)[0] + ".h")
        elif address.split(".")[-1] == "h":
            self.hds_path.append(address)
            try:
                # append src file associated with header
                if self.find(os.path.splitext(address)[0] + ".cpp"):
                    assoc_src = self.find(os.path.splitext(address)[0] + ".cpp")
                    if assoc_src not in self.srcs_path:
                        self.transpile(assoc_src)
                        self.srcs_path.append(assoc_src)
                elif self.find(os.path.splitext(address)[0] + ".cxx"):
                    assoc_src = self.find(os.path.splitext(address)[0] + ".cxx")
                    if assoc_src not in self.srcs_path:
                        self.transpile(assoc_src)
                        self.srcs_path.append(assoc_src)
            except ValueError:
                pass
        #  if includes other files
        if file.includes:
            # transpile each file included recursively
            for include in file.includes:
                if include not in self.srcs_path and include not in self.hds_path:
                    self.transpile(self.find(include))
        self.file_objs.append(file)

    def build(self):
        """
        Link and build all files included recursively by main file
        with g++
        """
        # clean executable
        if os.path.isfile(self.output_exe):
            os.remove(self.output_exe)
        # source files to object files
        self.find_compile_files()
        print(self.compiles_path)
        for i in self.compiles_path:
            obj_dir = os.path.join(os.path.split(i)[0], ".obj")
            if not os.path.exists(obj_dir):
                os.makedirs(obj_dir)
            os.system("g++ " + " -c " + " -o " +
                      os.path.join(
                          obj_dir,
                          os.path.splitext(os.path.split(i)[1])[0] + ".o")
                      + " " + os.path.normpath("-I " + " -I ".join(self.include_dirs)) + " " + i)
        # object files to executable
        gcc_files = []
        for i in self.srcs_path:
            gcc_files.append(os.path.join(os.path.split(i)[0],
                                          ".obj", os.path.splitext(os.path.split(i)[1])[0] + ".o"))
        os.system("g++ " + " -o " + self.output_exe + " " + " ".join(gcc_files))
        if os.path.isfile(self.output_exe):
            self.update_register()

    def run(self):
        """
        Run produced executable
        """
        subprocess.call([self.output_exe])


parser = argparse.ArgumentParser(description='Transpile, Compile, or Run Discordant Source Code')
parser.add_argument("-i", "-include", nargs="*")
parser.add_argument("action", choices=['transpile', 'make', 'run'])
parser.add_argument("infile")
parser.add_argument('outfile', nargs='?')
args = parser.parse_args(["run", "test_proj/discordance.dis"])
# args = parser.parse_args()
a = project(os.path.abspath(args.infile), args.i, args.outfile)

a.transpile()
if args.action == "make":
    a.build()
elif args.action == "run":
    a.build()
    a.run()

# __________________________________________
# Build
# With
# pyinstaller -y  --onefile --icon=app.ico --add-data lazycpp.exe;. --add-data __Discordance.h;. Transpiler.py --clean
# __________________________________________
