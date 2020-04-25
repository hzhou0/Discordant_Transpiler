# global vars
from File import *
import os

x = File(os.getcwd()+"\sample_discord.txt")
x.process()
print(x.string)
print(x.includes())


class project:

    def __init__(self, root_file, directories):
        self.root = root_file
        self.directories = directories
        self.token = []
# regex
# https://regex101.com/r/N5O1jq/1
