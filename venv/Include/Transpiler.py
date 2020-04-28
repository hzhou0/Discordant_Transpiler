# global vars
from File import *
import os
import subprocess


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
        include_dir.append(os.getcwd())
        self.include_dirs = include_dir
        self.files = []

    def find(self, name):
        for dir in self.include_dirs:
            if os.path.isfile(os.path.join(dir, name)):
                return os.path.join(dir, name)

    def transpile(self, address=None):
        if not address:
            address = self.root
        file = File(address)
        file.process()
        # if compiling a .dis file
        if address.split(".")[-1] == "dis":
            file.sync(address + "_")
            subprocess.call([os.path.join(os.getcwd(), "lazycpp.exe"),
                             "-o", os.path.dirname(address),
                             os.path.normpath(address + "_")], shell=True)
            os.remove(address + "_")
            self.files.append(os.path.splitext(address)[0] + ".cpp")
        else:
            self.files.append(address)
        for include in file.includes:
            self.compile(find(include))

    def build(self):
        files = "".join(self.files)
        gcc_I = []
        for dir in self.include_dirs:
            gcc_I.append("-I " + dir + " ")
        subprocess.call(["g++", self.files,
                         "-o", self.output_exe,
                         gcc_I], shell=True)

    def run(self):
        subprocess.call([self.output_exe])

a = project("C:/Users/henry/PycharmProjects/Discordant_Transpiler/venv/Include/test_proj/sample_discord.dis")
a.transpile()
a.build()
a.run()
# print(a.root)
# print(a.include_dirs)
