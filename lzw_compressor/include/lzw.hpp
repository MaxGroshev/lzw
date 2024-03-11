#pragma once

#include "time_control.hpp"
#include "compressor.hpp"
#include "decompressor.hpp"
#include "config.hpp"

//-----------------------------------------------------------------------------------------

namespace my_compress {

class lzw_t final {
    private:
        set_up_t config_ = {};
        compressor_t compressor;
        decompressor_t decompressor;

    public:
        lzw_t(set_up_t config, int init_encode_dictionary_size,
                               int init_decode_dictionary_size) :
            config_(config),
            compressor(init_encode_dictionary_size),
            decompressor(init_decode_dictionary_size) {}

        lzw_t(set_up_t config) :
            config_(config),
            compressor(config.get_dict_dir().c_str()),
            decompressor(0) {}


//COMPRESSOR_WRAPER------------------------------------------------------------------------

        std::vector<int> compress(const std::string& input_dat) {
            return compressor.compress(input_dat.begin(), input_dat.end());
        };
        std::vector<int> compress() {
            std::string input_dat = get_data_for_compress();
            std::vector<int> res = compressor.compress(input_dat.begin(), input_dat.end());
            // utils::dump_vect(res);
            std::string write_dir = config_.get_dest_dir() + ".lzw"; //bad
            compressor.write_res_in_bin(res, write_dir.c_str());
            return res;
        };

//DECOMPRESSOR_WRAPER----------------------------------------------------------------------

        std::string decompress(const std::vector<int>& input_dat) {
            return decompressor.decompress(input_dat.begin(), input_dat.end());
        };
        std::string decompress() {
            std::vector<int> input_dat = get_data_for_decompress();
            std::string res = decompressor.decompress(input_dat.begin(), input_dat.end());
            std::string write_dir = config_.get_dest_dir() + ".decode";
            decompressor.write_res(res, write_dir.c_str());
            return res;
        };

//GETTERS----------------------------------------------------------------------------------

        size_t get_compress_dict_size()     const {return compressor.get_dict_size();};
        size_t get_decompress_dict_size()   const {return decompressor.get_dict_size();};
        bool   is_train_mode()              const {return config_.is_train_mode();};
        bool   is_dict_mode()               const {return config_.is_dict_mode();};

        std::string get_data_for_compress() const {
            return utils::read_from_file_into_string(config_.get_src_dir().c_str());
        };
        std::vector<int> get_data_for_decompress() const {
            return decompressor.get_data_from_bin(config_.get_src_dir().c_str());
        };
        void compress_show_res() const {
            size_t comp_size =  utils::get_file_size((config_.get_dest_dir() + ".lzw").c_str());
            size_t input_size =  utils::get_file_size(config_.get_src_dir().c_str());
            std::clog << input_size / 1024 << " Kbts" <<
                        " ==> " << comp_size / 1024 << " Kbts" << "\n";
            std::clog << "Compression factor: " << double(comp_size * 100) /
                                                   double(input_size) << " %\n";
        }
};
}
//-----------------------------------------------------------------------------------------

