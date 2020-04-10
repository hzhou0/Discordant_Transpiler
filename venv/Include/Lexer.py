import re


def readline(string):
    """
    read a line from string
    """
    x = ""
    i = 0
    while string[i] != "\n" and i - 1 <= len(string):
        i += 1
        x = x + string[i]
    return x


def whitespace(string, front=True):
    """
    return the number of whitespace chars
    in front or behind the string
    :param string: read from this
    :param front: if to start reading
    from front of string
    """
    if front:
        x = 1
        i = 0
    else:
        x = -1
        i = -1
    while string[i] == " " and abs(i) <= len(string):
        i += x
    return abs(i)


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

    def convert_tabs(self):
        """
        replaces tabs with 8 spaces, as per unix rules
        """
        self.string = self.string.replace("\t", "        ")

    def mark(self):
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

    def replace(self):
        """
        replace tokenized string literals, comments
        and preprocessors
        """
        token = "!!!"
        for i in range(len(self.replaced)):
            regex = str(i) + token + str(i)
            self.string = self.string.replace(str(i) + token + str(i), self.replaced[i], 1)

    def else_if(self):
        """
            Compiles pythonic else if statement
            into c++ style
        """
        regex = r"else\s+?if[^:]*?:\n"
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
                    box.expr_list.append(match)
            # if not create box with match
            if not is_sorted:
                sorted_expr.append(ExpBox(depth, match))

        # convert starting from the most indented to the least indented

        def indent_depth(exp_box):
            """
            function to sort SORTED_EXPR
            by indentation depth
            """
            return exp_box.depth

        sorted_expr.sort(reverse=True, key=indent_depth)
        for box in sorted_expr:
            #  print(box.depth)
            #  print(box.expr_list)
            for expr in box.expr_list:
                # read one line from end of else_if statement
                line = readline(self.string[expr.end() + 1:])
                line_count = 0
                spaces = whitespace(line)
                # if indented continue
                while spaces > box.depth:
                    line = readline(self.string[expr.end() + 1:])
                    spaces = whitespace(line)
                print(spaces)
