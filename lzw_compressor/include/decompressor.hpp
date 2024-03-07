#pragma once

#include "compressor.hpp"
#include "decompressor.hpp"
#include <unordered_map>
#include <string>

//-----------------------------------------------------------------------------------------

namespace my_compress {

class decompressor_t final {
    private:
        std::unordered_map<int, std::string> dictionary_;
        const int init_size_of_dictionary_ = 256;

    public:
        decompressor_t(int init_size_of_dictionary = 256) :
                      init_size_of_dictionary_(init_size_of_dictionary){
            for (int i = 0; i < init_size_of_dictionary_; ++i) {
                dictionary_[i] = std::string{char(i)};
            }
        }
        inline std::string decompress(const std::vector<int>& input_dat);
        size_t get_dict_size() const {return dictionary_.size();};
        size_t get_init_size_of_dictionary() const {return init_size_of_dictionary_;};
        std::vector<int> get_data_from_bin(const char* path) const;
        inline void write_res(std::string res, const char* path) const;
};

//-----------------------------------------------------------------------------------------

std::string decompressor_t::decompress(const std::vector<int>& input_dat) {
    std::string output = "";

    int code = init_size_of_dictionary_ + 1;
    int fst_elem_code = input_dat[0];

    std::string fst_elem = dictionary_[fst_elem_code];
    output += fst_elem;
    std::string snd_elem = "";
    snd_elem = fst_elem;
    for (int i = 0; i < input_dat.size() - 1; i++) {
        int snd_elem_code = input_dat[i + 1];
        if (!dictionary_.contains(snd_elem_code)) {
			fst_elem = dictionary_[fst_elem_code] + snd_elem;
		}
        else {
            fst_elem = dictionary_[snd_elem_code];
        }
        output += fst_elem;
        snd_elem = "";
        snd_elem += fst_elem[0];
        dictionary_[code] = dictionary_[fst_elem_code] + snd_elem ;
        ++code;
        fst_elem_code = snd_elem_code;
    }
    std::cout << output << "\n";
    return output;
}

void decompressor_t::write_res(std::string res, const char* path) const {
    std::ofstream out;
    out.open(path);

    out << res;
    out.close();
}


std::vector<int> decompressor_t::get_data_from_bin(const char* path) const {

    std::ifstream data_file(path, std::ios::binary);
    std::vector<int> data {};

    int elem = {};
    for (int i = 0; !data_file.eof(); i++) {
        data_file.read((char*)&elem, sizeof(int));
        data.push_back(elem);
    }
    data_file.close();

    return data;
}

}
