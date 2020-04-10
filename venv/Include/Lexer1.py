import re


def identifiers(input_string):
    keywords = ["asm", "else", "new", "this", "auto", "enum", "operator", "throw",
                "bool", "explicit", "private", "true", "break", "export", "protected",
                "try", "case", "extern", "public", "typedef", "catch", "false",
                "register", "typeid", "char", "float", "reinterpret_cast", "typename",
                "class", "for", "return", "union", "const", "friend", "short",
                "unsigned",  "const_cast", "goto", "signed", "using", "continue", "if",
                "sizeof", "virtual", "default", "inline", "static", "void",
                "delete", "int", "static_cast", "volatile", "do", "long",
                "struct", "wchar_t", "double", "mutable", "switch", "while",
                "dynamic_cast", "namespace", "template", "And", "bitor", "not_eq", "xor",
                "and_eq", "compl", "or", "xor_eq", "bitand", "not", "or_eq", "thread_local",
                "static_assert", "requires", "nullptr", "noexcept", "decltype", "constexpr",
                "char16_t","alignof", "alignas", "char32_t", "concept"]
    #remove line comments
    x=re.sub("//.*\n", "\n", input_string)
    #remove block comments
    x=re.sub("\/\*([\s\S]*)\*\/", "", x)
    #removes empty lines
    x=re.sub("^\s*[\r\n]", "", x,flags= re.MULTILINE)
    #remove reserved words
    regex = r"\\\"|\"(?:\\\"|[^\"])*\"|((?<!(<-))(?<!(\.|\w))[a-zA-Z_][\w]*)"
    matches = re.finditer(regex, x)
    identifiers=[]


    for matchNum, match in enumerate(matches, start=1):
        if match.group(1):
                identifiers.append(match.group(1))
        # print("Match {matchNum} was found at {start}-{end}: {match}".format(matchNum=matchNum, start=match.start(),
        #                                                                     end=match.end(), match=match.group()))


    unique=list(dict.fromkeys(identifiers))    #remove duplicates
    for i in keywords:
        if i in unique:
            unique.remove(i)
    return unique


#swap string literals for tokens of !!!i, where i is the nth string
def mark_literals(string, list, i=0):
    regex = r"(?:'|\")(?:[^\"\\]|\\.)*?(?:\"|')"
    x=re.search(regex, string)
    if x:
        list.append(x.group(0))
    x=re.sub(regex,"!!!"+str(i), string, 1)
    if x is not string:
        i=i+1
        return mark_literals(x, list, i)
    return x
def replace_marked(string, list):
    for i in range(len(list)):
        regex="!!!"+str(i)
        string=string.replace(regex, list[i], 1)
    return string

