#pragma once

#include <unordered_map>
#include <string>
#include "utils.hpp"

//-----------------------------------------------------------------------------------------

namespace my_compress {

class compressor_t final {
    private:
        std::unordered_map<std::string, int> dictionary_;
        const int init_size_of_dictionary_ = 256;

    public:
        compressor_t(int init_size_of_dictionary = 256) :
                        init_size_of_dictionary_(init_size_of_dictionary){
            for (int i = 0; i < init_size_of_dictionary_; ++i) {
                dictionary_[std::string{char(i)}] = i;
            }
        }
        inline std::vector<int> compress(const std::string& input_dat);
        size_t get_dict_size() const {return dictionary_.size();};
        size_t get_init_size_of_dictionary() const {return init_size_of_dictionary_;};

        inline std::string get_data(const char* path) const;
        inline void write_res_in_bin(std::vector<int> res, const char* path) const;

};

//-----------------------------------------------------------------------------------------

std::vector<int> compressor_t::compress(const std::string& input_dat) {
    std::vector<int> output;

    std::string fst_elem {};
    std::string snd_elem {};
    std::string concat   {};
    fst_elem += input_dat[0];
    int code = dictionary_.size() + 1;
    for (int i = 0; i < input_dat.size(); ++i) {
        if (i != input_dat.size())
            snd_elem += input_dat[i + 1];
        concat = fst_elem + snd_elem;
        if (dictionary_.contains(concat) && input_dat[i + 1] != 0) {

            fst_elem = concat;
        }
        else {
            output.push_back(dictionary_[fst_elem]);
            dictionary_[concat] = code;
            ++code;
            fst_elem = snd_elem;
        }
        snd_elem = "";
    }

    return output;
}

//-----------------------------------------------------------------------------------------

void compressor_t::write_res_in_bin(std::vector<int> res, const char* path) const {
    std::ofstream out(path, std::ios::binary);

    for (auto& elem : res)
        out.write((char*)&elem, sizeof(int));
    out.close();
}

std::string compressor_t::get_data(const char* path) const {
    std::ifstream data_file(path, std::ios::binary);
    if (!data_file.good()) {
        std::string msg = "Input file for compression does not exist\n";
        throw std::runtime_error(msg + path);
    }

    ssize_t f_size = utils::get_file_size(path);
    char* char_buf = new char[f_size];
    data_file.read(char_buf, f_size);
    std::string data(char_buf, char_buf + f_size);
    delete [] char_buf;
    data_file.close();
    return data;
}

}
