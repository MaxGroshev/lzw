#pragma once

#include <fstream>
#include <stdexcept>
#include <vector>

#include "utils.hpp"
#include "time_control.hpp"
#include "compressor.hpp"
#include "decompressor.hpp"

//-----------------------------------------------------------------------------------------

class set_up_t {
    private:
        std::string src_dir;
        std::string dest_dir = "";

    public:
        set_up_t() {};
        const std::string& get_src_dir()  const {return src_dir;};
        const std::string& get_dest_dir() const {return dest_dir;};
        set_up_t(const int argc, char** argv) {

            if (argc >= 2) {
                src_dir = argv[1];
            }
            else {
                throw std::runtime_error("ERROR: No source dir was mentioned\n");
            }
            if (argc >= 3) {
                dest_dir = argv[2];
            }
            // else {
            //     dest_dir = src_dir + ".gif";
            // }
        }
};

//-----------------------------------------------------------------------------------------

namespace my_compress {

class lzw_t final {
    private:
        set_up_t config_ = {};
        compressor_t compressor;
        decompressor_t decompressor;

    public:
        lzw_t(set_up_t config = set_up_t{}, int init_encode_dictionary_size = 256,
                                            int init_decode_dictionary_size = 256) :
            config_(config),
            compressor(init_encode_dictionary_size),
            decompressor(init_decode_dictionary_size) {}

//COMPRESSOR_WRAPER------------------------------------------------------------------------

        std::vector<int> compress(const std::string& input_dat) {
            return compressor.compress(input_dat);
        };
        std::vector<int> compress() {
            std::vector<int> res = compressor.compress(get_data_for_compress());
            // utils::dump_vect(res);
            if (config_.get_dest_dir().size())
                compressor.write_res_in_bin(res, config_.get_dest_dir().c_str());
            else {
                std::string write_dir =  config_.get_src_dir() + ".gif"; //bad
                compressor.write_res_in_bin(res, write_dir.c_str());
            }
            return res;
        };

//DECOMPRESSOR_WRAPER----------------------------------------------------------------------

        std::string decompress(const std::vector<int>& input_dat) {
            return decompressor.decompress(input_dat);
        };
        std::string decompress() {
            std::string res = decompressor.decompress(get_data_for_decompress());
            if (config_.get_dest_dir().size())
                decompressor.write_res(res, config_.get_dest_dir().c_str());
            else {
                std::string write_dir =  config_.get_src_dir() + ".decode";
                decompressor.write_res(res, write_dir.c_str());
            }
            return res;
        };

//GETTERS----------------------------------------------------------------------------------

        size_t get_compress_dict_size()      const {return compressor.get_dict_size();};
        size_t get_decompress_dict_size()    const {return decompressor.get_dict_size();};

        std::string get_data_for_compress() const {
            return compressor.get_data(config_.get_src_dir().c_str()); //TODO: make func ret const char*
        };
        std::vector<int> get_data_for_decompress() const {
            return decompressor.get_data_from_bin(config_.get_src_dir().c_str());
        };
};
}
//-----------------------------------------------------------------------------------------

