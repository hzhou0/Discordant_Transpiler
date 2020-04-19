# global vars
from Lexer import *

string_literals = []
# operations
dis_src = open("C:/Users/henry/CLionProjects/untitled/Discordance/outfile.cpp", "r")
src_code = dis_src.read()

test = open("C:/Users/henry/PycharmProjects/Discordant_Transpiler/venv/Include/sample_discord.txt", "r")
x = Lexer(test.read())
x.sanitize()
x.mark_com()
x.statements()
x.replace_com()
x.dynamic_type()
print(x.string)


# regex
# https://regex101.com/r/N5O1jq/1
