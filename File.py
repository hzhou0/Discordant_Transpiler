import re


def readline(string):
    """
    read a line from string
    """
    x = ""
    i = 0
    while i < len(string):
        if string[i] != "\n":
            x += string[i]
            i += 1
        else:
            return x + "\n"
    return x


def whitespace(string):
    """
    return the number of whitespace chars
    in front or behind the string
    :param string: read from this
    """
    i = 0
    if string.strip() == "":
        return float("inf")
    while string and string[i] == " " and i < len(string):
        i += 1
    return i


class ExpBox:
    """
    Container for expressions targets
    with the same indentation value
    """

    def __init__(self, depth, first_str):
        self.depth = depth
        self.expr_list = []
        self.expr_list.append(first_str)


class File:
    """
    Container for discordant source code,
    and encapsulates compilation methods
    """

    def __init__(self, address):
        self.address = address
        self.file = open(address, "r")
        self.string = self.file.read()
        self.file.close()
        self.replaced_pre = []
        self.includes = [None]
        self.replaced_literals = []
        self.replaced_statements = []
        self.class_decl = []
        self.processed = False

    def sync(self, address):
        """
        synchronize self.string with file on disk
        """
        self.file = open(address, "w")
        self.file.write(self.string)
        self.file.close()

    def sanitize(self):
        """
        replaces tabs with 8 spaces, as per unix rules
        """
        regex = r"""//.*\n"""
        # delete all comments
        line_comments = re.findall(regex, self.string)
        for comment in line_comments:
            self.string = self.string.replace(str(comment), "\n", 1)
        regex = r"""\/\*(?:[\s\S]*?)\*\/"""
        block_comments = re.findall(regex, self.string)
        for comment in block_comments:
            self.string = self.string.replace(str(comment), "", 1)
        #  remove trailing whitespace
        self.string = re.sub(r"[ \t]+$", "\n", self.string, flags=re.MULTILINE)
        #  substitute tabs for 8 spaces
        self.string = self.string.replace("\t", "        ")
        #  regularize lines separators
        self.string = self.string.replace("\r\n", "\n")
        self.string = self.string.replace("\r", "\n")
        #  remove blank lines
        lines = self.string.split("\n")
        lines = filter(lambda x: not re.match(r'^\s*$', x), lines)
        self.string = "\n".join(lines)

    def mark_com(self):
        """
        replaces preprocessor commands and string literals to later replace
        """
        regex = r"""\#.*?\n"""
        token = "!!!"
        i = 0
        self.replaced_pre = re.findall(regex, self.string)
        for match in self.replaced_pre:
            #   self.string=re.sub(match, str(i) + token + str(i), self.string, 1)
            self.string = self.string.replace(str(match), str(i) + token + str(i) + "\n", 1)
            i += 1
        regex = r"""(?:'|\")(?:[^\"\\]|\\.)*?(?:\"|')"""
        token = "!!!!"
        i = 0
        self.replaced_literals = re.findall(regex, self.string)
        for match in self.replaced_literals:
            #   self.string=re.sub(match, str(i) + token + str(i), self.string, 1)
            self.string = self.string.replace(str(match), str(i) + token + str(i), 1)
            i += 1

    def replace_com(self):
        """
        replace tokenized string literals, comments
        and preprocessors
        """
        self.includes = []
        for preproc in self.replaced_pre:
            if re.search(r"#\s*include", preproc) and not re.search(r"<\w+>", preproc):
                x = re.sub(r"#\s*include", "", preproc)
                x = re.sub(r"\n", "", x)
                x = x.strip()
                if x[0] == x[-1] == "\"":
                    x = x[1:-1]
                else:
                    raise ValueError(
                        "Unknown include syntax for\n" + preproc + "Includes should be enclosed in brackets")
                self.includes.append(x)
        for i, s in enumerate(self.replaced_pre):
            if re.search(r"#\s*include", s):
                if s.strip()[-5:] == ".dis\"":
                    self.replaced_pre[i] = s.strip()[:-5] + ".h\"\n"
                self.replaced_pre[i] = "#hdr\n" + self.replaced_pre[i] + "#end\n"

        token = "!!!"
        for i in range(len(self.replaced_pre)):
            self.string = self.string.replace(str(i) + token + str(i) + "\n", self.replaced_pre[i], 1)
        token = "!!!!"
        for i in range(len(self.replaced_literals)):
            self.string = self.string.replace(str(i) + token + str(i), self.replaced_literals[i], 1)

    def mark_statements(self):
        """
        replaces preprocessor commands, string literals, and comments
        with tokens to later replace
        """
        regex = r"((?:if|for|class|while|do|else\s+?if)[\s(]+?[\s\S]*?:\n)" \
                r"|(else\s*?:\n)" \
                r"|\b[\w]*[^\w;]*[\w]+\s*\([^;]*?\)\s*:\n"
        token = "@~@"
        i = 0
        matches = re.finditer(regex, self.string)
        self.replaced_statements = []
        for match in matches:
            if match:
                regex = r"\b(?:else\s+?if|if|for|class|while|else|switch|case)\b" \
                        r"|\b[\w]*[^\w;]*[\w]+\s*\([^;]*?\)\s*:\n"
                if len(re.findall(regex, match.group())) < 2:
                    self.replaced_statements.append(match.group())
                    if re.search("class", match.group()):
                        self.class_decl.append(len(self.replaced_statements) - 1)
        for match in self.replaced_statements:
            #   self.string=re.sub(match, str(i) + token + str(i), self.string, 1)
            self.string = self.string.replace(str(match), str(i) + token + str(i) + "\n", 1)
            i += 1

    def replace_statements(self):
        """
        replace tokenized string literals, comments
        and preprocessors
        """
        token = "@~@"
        for i in range(len(self.replaced_statements)):
            self.string = self.string.replace(str(i) + token + str(i), self.replaced_statements[i], 1)

    def cat_by_indent(self, regex):
        """
        Returns a list of ExpBox classes,
        containing statements sorted by indentation level,
        from deepest to shallowest
        """
        matches = re.finditer(regex, self.string)
        sorted_expr = []
        for match in matches:
            depth = 0
            i = 1
            # walk back from start of match to get indentation
            while self.string[match.start() - i] == " ":
                i += 1
                depth += 1
            # see if current indentation box exists
            is_sorted = False
            # if so deposit in box
            for box in sorted_expr:
                if depth == box.depth:
                    is_sorted = True
                    box.expr_list.append(match.group())
            # if not create box with match
            if not is_sorted:
                sorted_expr.append(ExpBox(depth, match.group()))

        # convert starting from the most indented to the least indented
        def indent_depth(exp_box):
            """
            function to sort SORTED_EXPR
            by indentation depth
            """
            return exp_box.depth

        sorted_expr.sort(reverse=True, key=indent_depth)
        return sorted_expr

    def statements(self):
        """
            Compiles pythonic else if statement
            into c++ style
        """
        # swap statements for marks
        self.mark_statements()
        expBox_list = self.cat_by_indent(r"\d@~@\d\n")
        #file = open("marked.txt", "w")
        #file.write(self.string)
        #file.close()
        # iterate through all matches, deepest indented ones first
        for expBox in expBox_list:
            for expr in expBox.expr_list:
                # retry the match, swaps change match span
                exp = re.search(expr, self.string)
                lines = ""
                # start reading immediately after the match
                string = self.string[exp.end():]
                # keep reading until text unindents
                while True:
                    if not string:
                        break
                    line = readline(string)
                    string = string[len(line):]
                    indents = whitespace(line)
                    if indents <= expBox.depth:
                        break
                    lines += line
                # target is all text indented
                target = self.string[exp.start():exp.end() + len(lines)]
                # wrapped intended in curly brackets
                spacing = ""
                for i in range(expBox.depth):
                    spacing += " "
                if lines[-1] == "\n":
                    lines = lines[:-1]
                num = int(re.match(r"\d*", exp.group()).group())
                if num in self.class_decl:
                    payload = exp.group()[:-1] + "{\n" + lines + "\n" + spacing + "};\n"
                else:
                    payload = exp.group()[:-1] + "{\n" + lines + "\n" + spacing + "}\n"
                # replace
                self.string = self.string.replace(target, payload)
        # iterate through
        for i, statement in enumerate(self.replaced_statements):
            # else statement
            if re.search(r"else\s*?:\n", statement):
                self.replaced_statements[i] = statement[:-2]
            # if/for/while/do/else if statements
            elif re.search(r"^(?:if|for|while|do|else\s+?if)[\s(]", statement):
                self.replaced_statements[i] = statement[:-2] + " )"
                target = re.search(r"^(?:if|for|while|do|else\s+?if)[\s(]", statement).group()
                payload = target + "( "
                self.replaced_statements[i] = self.replaced_statements[i].replace(target, payload)
                #   for loop, swap in for : if range-based loop
                if re.search(r"^for[\s(]", self.replaced_statements[i]):
                    x = statement[:-2]
                    x = re.sub(r"^for[\s(]", "", x)
                    x = re.sub(r":", " in ", x)
                    x = re.split(r" in ", x)[0]
                    words = re.findall(r"[*&\w]+\b", x)
                    if len(words) == 1:
                        self.replaced_statements[i] = self.replaced_statements[i] \
                            .replace(" " + words[0] + " ", " auto " + words[0] + " ", 1)
                    if 'const' in words and len(words) == 2:
                        self.replaced_statements[i] = self.replaced_statements[i] \
                            .replace(" " + words[1] + " ", " auto " + words[1] + " ", 1)
                    self.replaced_statements[i] = re.sub(r"\bin\b", ":", self.replaced_statements[i])

            # function declaration
            else:
                self.replaced_statements[i] = statement[:-2]
        self.replace_statements()

    def dynamic_type(self):
        self.string = self.string.replace("@", "discordance::var ")

    def vector(self):
        # default initialization
        vector_decls = re.findall(r"(?:[\w:.]+\s+)*[^\w;]*[\w]+(?:\[[ ?\d]*\? *\])+\s*;", self.string)
        # decl = *type* var [?]
        for decl in vector_decls:
            decl = decl[:-1].strip()
            # isolate var[?]
            index_operators = re.findall(r"(?:\[[ ?\d]*\? *\])", decl)
            typename = ""
            constructor = ""
            # find type recursively
            for match in index_operators:
                # if approx length given
                size = 0
                if re.search(r"\[ *(\d+)\? *\]", match):
                    container = "discordance::vector"
                    size = re.search(r"\[ *(\d+)\? *\]", match).group(1)
                # if unknown length
                else:
                    container = "discordance::deque"
                    init = ""
                if typename:
                    typename = container + "<" + typename + ">"
                    constructor = typename + "(" + str(size) + "," + constructor + ")"
                else:
                    x = re.search(r"[\w]+(?:\[[ ?\d]*\? *\])+", decl).group()
                    x = decl[:-len(x)]
                    typename = container + "<" + x + ">"
                    constructor = typename + "(" + str(size) + ")"
            constructor = constructor[len(typename):]
            var = re.search(r"[\w]+(?:\[[ ?\d]*\? *\])+", decl).group()
            var = re.sub(r"(?:\[[ ?\d]*\? *\])+", "", var)
            target = decl
            payload = typename + " " + var + constructor
            self.string = self.string.replace(target, payload)
        # assignment initialization
        vector_decls = re.findall(r"(?:[\w:.]+\s+)*[^\w;]*[\w]+(?:\[[ ?\d]*\? *\])+\s*[={]", self.string)
        for decl in vector_decls:
            decl = decl[:-1].strip()
            # isolate var[?]
            index_operators = re.findall(r"(?:\[[ ?\d]*\? *\])", decl)
            typename = ""
            # find type recursively
            for match in index_operators:
                # if approx length given
                size = 0
                if re.search(r"\[ *(\d+)\? *\]", match):
                    container = "discordance::vector"
                    size = re.search(r"\[ *(\d+)\? *\]", match).group(1)
                # if unknown length
                else:
                    container = "discordance::deque"
                    init = ""
                if typename:
                    typename = container + "<" + typename + ">"
                else:
                    x = re.search(r"[\w]+(?:\[[ ?\d]*\? *\])+", decl).group()
                    x = decl[:-len(x)]
                    typename = container + "<" + x + ">"
            var = re.search(r"[\w]+(?:\[[ ?\d]*\? *\])+", decl).group()
            var = re.sub(r"(?:\[[ ?\d]*\? *\])+", "", var)
            target = decl
            payload = typename + " " + var
            self.string = self.string.replace(target, payload)

    def slice(self):
        slice_decls = re.finditer(r"\[([ \-\d]*)\:([ \-\d]*)\]", self.string)
        for decl in slice_decls:
            target = decl.group()
            begin = 0
            if decl.group(1).strip():
                begin = decl.group(1).strip()
            end = 0
            if decl.group(2).strip():
                end = decl.group(2).strip()
            payload = ".slice(" + str(begin) + "," + str(end) + ")"
            self.string = self.string.replace(target, payload)

    def link_c_lib(self):

        self.string = self.string.replace("std::deque", "discordance::deque")
        self.string = self.string.replace("std::vector", "discordance::vector")
        self.string = "#hdr\n" \
                      "#include \"__Discordance.h\"\n" \
                      "using namespace discordance;\n" \
                      "using discordance::deque; using discordance::vector; using discordance::var;\n" \
                      "#end\n" \
                      "#src\n" \
                      "#include \"__Discordance.h\"\n" \
                      "using namespace discordance;\n" \
                      "using discordance::deque; using discordance::vector; using discordance::var;\n" \
                      "#end\n" + self.string

    def process(self):
        self.sanitize()
        self.mark_com()
        self.dynamic_type()
        self.statements()
        self.vector()
        self.slice()
        self.replace_com()
        self.link_c_lib()
        self.processed = True

    # lines = []
    # for i in range(line_count):
    #    lines.append(self.string[expr.end() + 1:])
    # str_lines = ''.join(lines)
