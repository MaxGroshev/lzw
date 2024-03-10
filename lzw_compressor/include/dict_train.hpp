#pragma once

#include <unordered_map>
#include "config.hpp"
#include <algorithm>
#include "utils.hpp"

//-----------------------------------------------------------------------------------------

class dict_train_t {

    using um_vector = typename std::vector<std::pair<std::string, int>>;

    private:
        set_up_t config_;
        std::unordered_map<std::string, int> dictionary_;
    public:
        dict_train_t(set_up_t config) : config_(config) {}

        void train() {
            std::string train_dir = config_.get_src_dir();
            std::vector<std::string> samples =
                                        utils::get_file_name_from_dir(train_dir.c_str());

            for (const auto& sample : samples) {
                // std::cout << sample << '\n';
                train_on_sample(sample.c_str());
            }

            um_vector data = sort_dictionary();
            write_dictionary_to_file(data);
        };


        void train_on_sample(const char* path) {
            std::string sample_dat = utils::read_from_file_into_string(path); //const char* ?

            std::string fst_elem {};
            std::string snd_elem {};
            std::string concat   {};
            fst_elem += sample_dat[0];
            for (int i = 0; i < sample_dat.size(); ++i) {
                if (i != sample_dat.size())
                    snd_elem += sample_dat[i + 1];
                concat = fst_elem + snd_elem;

                auto um_elem = dictionary_.find(concat);
                if (um_elem != dictionary_.end()) {
                    fst_elem = concat;
                    um_elem->second++;
                }
                else {
                    dictionary_[concat] = 1;
                    fst_elem = snd_elem;
                }
                snd_elem = "";
            }
        }

        um_vector sort_dictionary() {
            um_vector data (dictionary_.begin(), dictionary_.end());
            std::sort(data.begin(), data.end(),
                      [](auto fst, auto snd){return (fst.second > snd.second);});

            return data;
        }

        void write_dictionary_to_file(um_vector& data) const {
            std::ofstream out(config_.get_dest_dir().c_str(), std::ios::binary);

            // for(const auto& elem : data) {
            //     // std::cout << "[" << elem.first << "]: " << elem.second << std::endl;
            // }

            for (int i = 0; i < std::min(int(data.size()), config_.get_maxdict()); i++) {
                data[i].first += "\n";
                out.write(data[i].first.data(), data[i].first.size());//probably too low
            }
            out.close();
        }

//-----------------------------------------------------------------------------------------

};
