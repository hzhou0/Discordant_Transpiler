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


class Lexer:
    """
    Container for discordant source code,
    and encapsulates compilation methods
    """

    def __init__(self, string):
        self.string = string
        self.replaced = []
        self.replaced_statements = []

    def sanitize(self):
        """
        replaces tabs with 8 spaces, as per unix rules
        """
        self.string = self.string.replace("\t", "        ")
        self.string = self.string.replace("\r\n", "\n")
        self.string = self.string.replace("\r", "\n")
        self.string = self.string.replace("\n\n", "\n")

    def mark_com(self):
        """
        replaces preprocessor commands, string literals, and comments
        with tokens to later replace
        """
        regex = r"""\/\*(?:[\s\S]*?)\*\/|//.*\n|\#.*?\n|\r"""
        token = "!!!"
        i = 0
        self.replaced = re.findall(regex, self.string)
        for match in self.replaced:
            #   self.string=re.sub(match, str(i) + token + str(i), self.string, 1)
            self.string = self.string.replace(str(match), str(i) + token + str(i), 1)
            i += 1

    def mark_statements(self):
        """
        replaces preprocessor commands, string literals, and comments
        with tokens to later replace
        """
        regex = r"((?:if|for|while|else\s+?if)[\s(]+?[\s\S]*?:\n)|(else\s*?:\n)"
        token = "@~@"
        i = 0
        matches = re.finditer(regex, self.string)
        self.replaced_statements = []
        for match in matches:
            self.replaced_statements.append(match.group())
        for match in self.replaced_statements:
            #   self.string=re.sub(match, str(i) + token + str(i), self.string, 1)
            self.string = self.string.replace(str(match), str(i) + token + str(i) + "\n", 1)
            i += 1

    def replace_com(self):
        """
        replace tokenized string literals, comments
        and preprocessors
        """
        token = "!!!"
        for i in range(len(self.replaced)):
            self.string = self.string.replace(str(i) + token + str(i), self.replaced[i], 1)

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
        file = open("marked.txt", "w")
        file.write(self.string)
        file.close()
        targets = []
        payloads = []
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
                payload = exp.group()[:-1] + "{\n" + lines[:-1] + "\n" + spacing + "}\n"
                # replace
                self.string = self.string.replace(target, payload)
        # iterate through
        print(self.replaced_statements)
        for i, statement in enumerate(self.replaced_statements):
            # if statement is else statement, cut : and skip
            regex = r"else\s*?:\n"
            if re.search(regex, statement):
                self.replaced_statements[i] = statement[:-2]
                continue
            self.replaced_statements[i] = statement[:-2] + " )"
            regex = r"^(?:if|for|while|else\s+?if)[\s(]"
            target = re.search(regex, statement).group()
            payload = target + "( "
            self.replaced_statements[i] = self.replaced_statements[i].replace(target, payload)
        self.replace_statements()
        print(self.string)

    # lines = []
    # for i in range(line_count):
    #    lines.append(self.string[expr.end() + 1:])
    # str_lines = ''.join(lines)
