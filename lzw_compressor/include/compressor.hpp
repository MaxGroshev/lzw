#pragma once

#include "decompressor.hpp"
#include <unordered_map>
#include <string>

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

    // std::cout << "Size before: " << dictionary_.size() << std::endl;

    std::string fst_elem {};
    std::string snd_elem {};
    std::string concat   {};
    fst_elem += input_dat[0];
    int code = dictionary_.size() + 1;
    for (int i = 0; i < input_dat.size(); ++i) {
        if (i != input_dat.size())
            snd_elem += input_dat[i + 1];
        // std::cout <  < snd_elem << '\n';
        concat = fst_elem + snd_elem;
        if (dictionary_.contains(concat)) {
            fst_elem = concat;
        }
        else {
            // std::cout << "HERE: " << concat << std::endl;
            output.push_back(dictionary_[fst_elem]);
            dictionary_[concat] = code;
            ++code;
            fst_elem = snd_elem;
        }
        snd_elem = "";
    }
    // std::cout << "Size after: " << dictionary_.size() << std::endl;
    return output;
}

//-----------------------------------------------------------------------------------------

void compressor_t::write_res_in_bin(std::vector<int> res, const char* path) const {
    std::ofstream out(path, std::ios::binary);

    for (const auto& elem : res)
        out.write((char*)&elem, sizeof(int));
    out.close();
}

std::string compressor_t::get_data(const char* path) const {
    std::string data;
    std::ifstream data_file;
    data_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    data_file.open(path);
    std::stringstream data_stream;
    data_stream << data_file.rdbuf();
    data_file.close();
    data = data_stream.str();
    return data;
}

}
