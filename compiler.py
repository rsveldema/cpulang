import sys
import os
from typing import List
from lark import Lark, Token, Tree, ast_utils, Transformer, v_args
from lark.tree import Meta
from lark.visitors import Visitor_Recursive

from dataclasses import dataclass

this_module = sys.modules[__name__]

with open('grammar.lark', "r") as fp:
    parser = Lark(grammar=fp, start='program')

class _Ast(ast_utils.Ast):
    # This will be skipped by create_transformer(), because it starts with an underscore
    pass

@dataclass
class Name(_Ast):
    name: str

class _Decl(_Ast):
    # This will be skipped by create_transformer(), because it starts with an underscore
    pass

class ToAst(Transformer):
    # Define extra transformation functions, for rules that don't correspond to an AST class.

    def ESCAPED_STRING(self, s :  Token):
        # Remove quotation marks
        return s[1:-1]

    def SIGNED_NUMBER(self, n : Token):
        return int(n)

    def inc_file_spec(self, spec):
        return spec

    @v_args(inline=True)
    def start(self, x):
        return x


transformer = ast_utils.create_transformer(this_module, ToAst())

include_path = sys.argv[1]
main_filename = sys.argv[2]

def is_builtin(s:str) -> bool:
    if s == "cpp.hpp": return True
    return False

class Visitor_Includes(Visitor_Recursive):
    def include_stmt(self, inc: Tree):
        file_node = inc.children[1]
        filename = f"{file_node.children[0]}"
        if not is_builtin(filename):
            print(f" visit include: {filename}")
            parse(filename)
        return

inc_visitor = Visitor_Includes()

seen_files = {}


def parse(filename: str):
    global seen_files
    path =  os.path.join(include_path, filename)

    if path in seen_files:
        print("skip duplicate include")
        return

    seen_files[path] = True
    with open(path, 'r') as fp:
        tree = parser.parse(fp.read())
        ttree = transformer.transform(tree)
        #print(f"tree = {ttree}")
        inc_visitor.visit(ttree)

parse(main_filename)

