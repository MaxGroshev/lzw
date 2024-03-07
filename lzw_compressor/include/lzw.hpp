#pragma once

#include <fstream>
#include <stdexcept>
#include <vector>

#include "utils.hpp"
#include "time_control.hpp"
#include "compressor.hpp"
#include "decompressor.hpp"

#include <boost/program_options.hpp>

//-----------------------------------------------------------------------------------------

class set_up_t {
    private:
        boost::program_options::variables_map args_;
        bool train_mode = false; //better in set_up as it is responsible way of running

        boost::program_options::options_description set_option_description() {
            using namespace boost::program_options;
            options_description desc{"Options"};
            desc.add_options()
            ("help,h", "help screen")
            ("train",   value<std::string>(), "train that is based"
            "on samples from mentioned dir")
            ("src, s",  value<std::string>(), "source_dir")
            ("dict, d", value<std::string>()->default_value("../samples"), "dictionray_dir")
            ("OUTPUT,o",value<std::string>(), "dest_dir");

            return desc;
        }

    public:
        set_up_t() {};
        set_up_t(const int argc, char** argv) {
            using namespace boost::program_options;

            options_description desc = set_option_description() ;

            variables_map args;
            store(parse_command_line(argc, argv, desc), args);
            notify(args);

            if (args.count("help")) //TODO: remove here just for debug
                std::cout << desc << '\n';
            if (!args.count("src"))
                throw std::runtime_error("No source dir was mentioned\n");
            if (args.count("train")) {
                train_mode = true;
                std::cout << "Train: " << args["train"].as<std::string>() << '\n';
            }
            if (args.count("dict"))
                std::cout << "Dict: " << args["dict"].as<std::string>() << '\n';

            args_ = std::move(args);
        }
        bool is_train_mode() const {return train_mode;}
        const std::string& get_src_dir()  const {return args_["src"].as<std::string>();};
        const std::string& get_train_dir() const {
            if (train_mode) {
                return args_["train"].as<std::string>();
            }
            else
                throw std::runtime_error("Request to train dir in train_mode=false\n");
        };
        const std::string& get_dest_dir() const {
            if (args_.count("OUTPUT")) {
                return args_["OUTPUT"].as<std::string>();
            }
            return args_["src"].as<std::string>();
        };
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
            std::string write_dir =  config_.get_dest_dir() + ".gif"; //bad
            compressor.write_res_in_bin(res, write_dir.c_str());
            return res;
        };

//DECOMPRESSOR_WRAPER----------------------------------------------------------------------

        std::string decompress(const std::vector<int>& input_dat) {
            return decompressor.decompress(input_dat);
        };
        std::string decompress() {
            std::string res = decompressor.decompress(get_data_for_decompress());
            std::string write_dir =  config_.get_dest_dir() + ".decode";
            decompressor.write_res(res, write_dir.c_str());
            return res;
        };

//TRAIN_MODE-------------------------------------------------------------------------------

        void train() {
            std::string train_dir = config_.get_train_dir();
            std::vector<std::string> samples =
                                        utils::get_file_name_from_dir(train_dir.c_str());
            std::cout << "Here\n";

            for (const auto& sample : samples) {
                std::cout << sample << " : " << get_compress_dict_size() << std::endl   ;
                std::vector<int> res = compressor.compress(compressor.get_data(sample.c_str()));
            }
            // return decompressor.decompress(input_dat);
        };

//GETTERS----------------------------------------------------------------------------------

        size_t get_compress_dict_size()      const {return compressor.get_dict_size();};
        size_t get_decompress_dict_size()    const {return decompressor.get_dict_size();};
        bool   is_train_mode()               const {return config_.is_train_mode();};
        std::string get_data_for_compress() const {
            return compressor.get_data(config_.get_src_dir().c_str()); //TODO: make func ret const char*
        };
        std::vector<int> get_data_for_decompress() const {
            return decompressor.get_data_from_bin(config_.get_src_dir().c_str());
        };
};
}
//-----------------------------------------------------------------------------------------

