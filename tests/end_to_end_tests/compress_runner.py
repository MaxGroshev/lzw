import subprocess
import argparse
import os
import sys
from   subprocess import Popen, PIPE, STDOUT

# -----------------------------------------------------------------------------------------

def get_files_names(files_names, dir_path):
    print (dir_path)
    data_files_counter = 0

    for path in os.listdir(dir_path):
        if os.path.isfile(os.path.join(dir_path, path)):
            data_files_counter += 1
            files_names.append(os.path.join(dir_path, path))

# -----------------------------------------------------------------------------------------

def run_compressor(args, compressors, data_files_names):


    print('————————————————————————START OF COMPRESSION———————————————————————')


    for (compressor) in range(0, len(compressors)):
        extention = ""
        if (compressors[compressor] == "zstd"):
            extention = ".zstd"

        if (args.count_of_files == 1):
            source = args.source_dir + args.relative_path
            dest   = args.destination_dir + args.relative_path + extention
            # print (source)
            # print (dest)
            pipe = Popen([compressors[compressor], "-f", "-k", source, "-o" , dest], stdout = PIPE)

            stdout_data = pipe.communicate()
            string_data = stdout_data[0].decode()
            print (string_data)

        else:
            for (i, file) in zip(range(args.count_of_files), data_files_names):

                dest = file.replace(args.source_dir, args.destination_dir) + extention
                pipe = Popen([compressors[compressor], "-f", file,
                          "-o" +  dest], stdout = PIPE)

                stdout_data = pipe.communicate()
                string_data = stdout_data[0].decode()


    print('————————————————————————END OF COMPRESSION—————————————————————————\n\n')

# -----------------------------------------------------------------------------------------

def add_parse_arguments(parser):
    parser.add_argument("-c",  "--count_of_files",   type = int, default = 1)
    parser.add_argument("-rp",  "--relative_path",   type = str, default = "10.dat")
    parser.add_argument('-D',  "--dictionary",       type = str, default = "no")
    parser.add_argument('-src',  "--source_dir",     type = str, default =
                    os.path.dirname(os.path.abspath(__file__)) + '/my_input_dat/')
    parser.add_argument('-dest',  '--destination_dir',          type = str, default =
                    os.path.dirname(os.path.abspath(__file__)) + '/my_compress_dat/')

# -----------------------------------------------------------------------------------------

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    add_parse_arguments(parser)
    args = parser.parse_args()

    data_files_names = []
    compressors = ["zstd", "./lzw"]

    get_files_names(data_files_names, args.source_dir)
    run_compressor(args, compressors, data_files_names)
