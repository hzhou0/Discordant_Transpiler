# global vars
from File import *
import os
import subprocess
import argparse
import sys
import platform

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

class project:
    def __init__(self, root_file_address, include_dir=None, output_exe=None):
        self.root = root_file_address
        if not output_exe:
            output_exe = os.path.splitext(root_file_address)[0] + ".exe"
        self.output_exe = output_exe
        if not include_dir:
            include_dir = []
        include_dir.append(os.path.dirname(self.root))
        include_dir.append(resource_path(""))
        self.include_dirs = include_dir
        self.files = []

    def find(self, name):
        for dir in self.include_dirs:
            if os.path.isfile(os.path.join(dir, name)):
                return os.path.join(dir, name)

    def transpile(self, address=None):
        if not address:
            address = self.root
        if os.path.isfile(os.path.splitext(address)[0] + ".h"):
            os.remove(os.path.splitext(address)[0] + ".h")
        if os.path.isfile(os.path.splitext(address)[0] + ".cpp"):
            os.remove(os.path.splitext(address)[0] + ".cpp")
        file = File(address)
        file.process()
        # if compiling a .dis file
        if address.split(".")[-1] == "dis":
            file.sync(address + "_")
            if platform.system()=='Windows':
                lazycpp="lazycpp.exe"
            elif platform.system()=='Darwin':
                lazycpp="mac_lazycpp"
            elif platform.system()=='Linux':
                lazycpp="linux_lazycpp"
            else:
                raise ValueError("Unsupported Operating System")
            subprocess.call([resource_path(lazycpp),
                             "-o", os.path.dirname(address),
                             os.path.normpath(address + "_")], shell=True)
            os.remove(address + "_")
            self.files.append(os.path.splitext(address)[0] + ".cpp")
        else:
            self.files.append(address)
        if file.includes:
            for include in file.includes:
                self.transpile(self.find(include))

    def build(self):
        if os.path.isfile(self.output_exe):
            os.remove(self.output_exe)
        gcc_I = []
        for dir in self.include_dirs:
            gcc_I.append("-I " + dir + " ")
        gcc_files = " " + " ".join(self.files)
        os.system("g++" + " " + gcc_files + " -o " + self.output_exe + " " + "".join(gcc_I))

    def run(self):
        subprocess.call([self.output_exe])


parser = argparse.ArgumentParser(description='Transpile, Compile, or Run Discordant Source Code')
parser.add_argument("-i", "-include", nargs="*")
parser.add_argument("action", choices=['transpile', 'make', 'run'])
parser.add_argument("infile")
parser.add_argument('outfile', nargs='?')
# args = parser.parse_args(["run", "test_proj/discordance.dis", "-i", "asd"])
args = parser.parse_args()
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
