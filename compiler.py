import sys
import lark


with open('grammar.lark', "r") as fp:
    parser = lark.Lark(grammar=fp, start='program')

filename = 'examples/cpu/cpu_container.cpu'

with open(filename, 'r') as fp:
    tree = parser.parse(fp.read())
    print(f"tree = {tree}")
