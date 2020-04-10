from Lexer import *

# global vars
string_literals = []
# operations
dis_src = open("C:/Users/henry/CLionProjects/untitled/Discordance/outfile.cpp", "r")
src_code = dis_src.read()

test = open("C:/Users/henry/PycharmProjects/Discordant_Transpiler/venv/Include/sample_discord.txt", "r")
x = Lexer(test.read())
x.convert_tabs()
x.else_if()
# regex
# https://regex101.com/r/N5O1jq/1
