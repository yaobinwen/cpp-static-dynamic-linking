#!/usr/bin/python3


import argparse
import io
import re


APP_DESC = "Parses the output of the program `nm`."


_SYMBOL_TYPES = set([
    "A",
    "B",
    "b",
    "C",
    "D",
    "d",
    "G",
    "g",
    "i",
    "I",
    "N",
    "n",
    "p",
    "R",
    "r",
    "S",
    "s",
    "T",
    "t",
    "U",
    "u",
    "V",
    "v",
    "W",
    "w",
    "-",
    "?",
])


def _syntax():
    parser = argparse.ArgumentParser(description=APP_DESC)

    parser.add_argument(
        "-t", "--symbol-types",
        help="The types of symbols that should be returned.",
    )

    parser.add_argument(
        "-i", "--input-file",
        help="The file to read the symbols from.",
    )

    parser.add_argument(
        "-o", "--output-file",
        help="The file to write the output to.",
    )

    return parser

def _reduce_types(s: str) -> str:
    dups = set()
    r = []
    for c in s:
        if c not in dups:
            r.append(c)
            dups.add(c)

    return "".join(r)


def _verify_types(s: str):
    invalid = set()
    for t in s:
        if t not in _SYMBOL_TYPES:
            invalid.add(t)

    if invalid:
        raise ValueError(f"invalid symbol types: {invalid}")


def main(symbol_types, input_file, output_file):
    if symbol_types is None:
        symbol_types = "".join(_SYMBOL_TYPES)
    else:
        # Make sure the given types are valid.
        symbol_types = _reduce_types(symbol_types)
        _verify_types(symbol_types)

    escaped = re.escape("".join(symbol_types))
    regex_pattern = f"^(?P<symbol_name>.+) [{escaped}] ([0-9a-hA-H ]*)*$"
    regex = re.compile(regex_pattern)

    symbols = []
    with io.open(input_file, mode="r", encoding="utf-8") as fh:
        for line in fh:
            line = fh.readline().strip("\n")
            m = regex.match(line)
            if m is not None:
                symbols.append(m.group("symbol_name"))

    if output_file is None:
        for s in symbols:
            print(s)
    else:
        with io.open(output_file, mode="w", encoding="utf-8") as fh:
            fh.write("\n".join(symbols))


def entry_point():
    main(**vars(_syntax().parse_args()))


if __name__ == "__main__":
    entry_point()
