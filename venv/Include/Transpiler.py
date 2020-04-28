# global vars
from File import *
import os
import subprocess
import argparse


# x = File(os.getcwd() + "/sample_discord.dis")
# x.process()


# print(x.includes)
# print(x.string)


class project:

    def __init__(self, root_file_address, include_dir=None, output_exe=None):
        self.root = root_file_address
        if not output_exe:
            output_exe = os.path.splitext(root_file_address)[0] + ".exe"
        self.output_exe = output_exe
        if not include_dir:
            include_dir = [os.path.dirname(self.root)]
        include_dir.append(os.path.abspath(os.path.dirname(__file__)))
        self.include_dirs = include_dir
        self.files = []

    def find(self, name):
        for dir in self.include_dirs:
            if os.path.isfile(os.path.join(dir, name)):
                return os.path.join(dir, name)

    def transpile(self, address=None):
        if not address:
            address = self.root
        if os.path.isfile(os.path.splitext(address)[0]+".h"):
            os.remove(os.path.splitext(address)[0]+".h")
        if os.path.isfile(os.path.splitext(address)[0] + ".cpp"):
            os.remove(os.path.splitext(address)[0] + ".cpp")
        file = File(address)
        file.process()
        # if compiling a .dis file
        if address.split(".")[-1] == "dis":
            file.sync(address + "_")
            subprocess.call([os.path.join(os.path.abspath(os.path.dirname(__file__)), "lazycpp.exe"),
                             "-o", os.path.dirname(address),
                             os.path.normpath(address + "_")], shell=True)
            os.remove(address + "_")
            self.files.append(os.path.splitext(address)[0] + ".cpp")
        else:
            self.files.append(address)
        for include in file.includes:
            self.compile(find(include))

    def build(self):
        if os.path.isfile(self.output_exe):
            os.remove(self.output_exe)
        gcc_I = []
        for dir in self.include_dirs:
            gcc_I.append("-I " + dir + " ")
        subprocess.call(["g++", self.files,
                         "-o", self.output_exe,
                        gcc_I], shell=True)

    def run(self):
        subprocess.call([self.output_exe])


parser = argparse.ArgumentParser(description='Transpile, Compile, or Run Discordant Source Code')
parser.add_argument("-i", "-include", nargs="*")
parser.add_argument("action", choices=['transpile', 'make', 'run'])
parser.add_argument("infile")
parser.add_argument('outfile', nargs='?')
#args = parser.parse_args(["make", "test_proj/sample_discord.dis", "-i", "asd"])
args = parser.parse_args()
a = project(os.path.abspath(args.infile), args.i, args.outfile)

a.transpile()
if args.action == "make":
    a.build()
elif args.action == "run":
    a.build()
    a.run()
