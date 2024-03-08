import subprocess
import argparse
import random
import numpy as np
import os
import sys

# -----------------------------------------------------------------------------------------

def print_test_data(args, test_data):
    dat_file = open(args.file, "w")

    dat_file.write("%d "%  args.count)
    dat_file.write("\n")

    for i in range (args.count):
        dat_file.write("%d "% test_data[i])
        if ((i % 10) == 0):
            dat_file.write("\n")

    test_data.sort()
    dat_file.write("(")
    for i in range (args.count):
        dat_file.write("%d "% test_data[i])
    dat_file.write(")")
    dat_file.close()

def split_file(args):
    filename = args.source
    with open(filename) as fin:
        fout = open(args.destination_dir + "/0.samp", "w")
        for i, line in enumerate(fin):
            fout.write(line)
            if (i + 1) % args.n_of_lines_on_file == 0:
                fout.close()
                fout = open(args.destination_dir + "/%d.samp"% (i / args.n_of_lines_on_file + 1),"w")

        fout.close()


def generate_test_data(args):
    test_data = []

    for i in range(args.count):
        test_data.append(random.randint(args.diaposon[0], args.diaposon[1]))

    return test_data

# -----------------------------------------------------------------------------------------

def add_parse_arguments(parser):
    parser.add_argument("-nl",  "--n_of_lines_on_file", type = int, default = 500)
    parser.add_argument('-src',  "--source", type = str, default =
                    os.path.dirname(os.path.abspath(__file__)) + '/big_samples/big_lit.samp')
    parser.add_argument('-dest',  '--destination_dir',          type = str, default =
                    os.path.dirname(os.path.abspath(__file__)) + '/tmp_samples/')

# -----------------------------------------------------------------------------------------

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    add_parse_arguments(parser)
    args = parser.parse_args()

    split_file(args)
    # test_data = generate_test_data(args)
    # print_test_data(args, test_data)
