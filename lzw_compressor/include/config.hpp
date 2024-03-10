#include <fstream>
#include <stdexcept>
#include <vector>
#include <boost/program_options.hpp>

//-----------------------------------------------------------------------------------------

class set_up_t {
    private:
        std::string src_dir;
        std::string dest_dir;
        std::string train_dir;
        std::string dict_dir;
        bool train_mode = false; //better in set_up as it is responsible way of running
        bool dictionary_mode = false;

        boost::program_options::options_description set_option_description() {
            using namespace boost::program_options;
            options_description desc{"Options"};
            desc.add_options()
            ("help,h", "help screen")
            ("train",   value<std::string>(), "train that is based"
            "on samples from mentioned dir")
            ("keep,k",  value<std::string>(), "source_dir")
            ("force,f" , "will be added soon")
            ("dict,D", value<std::string>(), "dictionray_dir")
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
            if (args.count("OUTPUT"))
                dest_dir = std::move(args["OUTPUT"].as<std::string>());
            if (args.count("keep"))
                src_dir = std::move(args["keep"].as<std::string>());
            else
                throw std::runtime_error("No source dir was mentioned\n");
            if (args.count("train")) {
                std::cout << "I am here\n";
                train_mode = true;
                train_dir = std::move(args["train"].as<std::string>());
            }
            if (args.count("dict")) {
                dictionary_mode = true;
                dict_dir = std::move(args["dict"].as<std::string>());
            }
        }

//-----------------------------------------------------------------------------------------

        bool is_train_mode() const {return train_mode;}
        bool is_dict_mode()  const {return dictionary_mode;}
        void write_dictionary_to_file()    const;
        const std::string& get_src_dir()   const {return src_dir;};
        const std::string& get_train_dir() const {
            if (train_mode)
                return train_dir;
            else
                throw std::runtime_error("Request to train dir in train_mode=false\n");
        };
        const std::string& get_dict_dir() const {
            if (dictionary_mode)
                return dict_dir;
            else
                throw std::runtime_error("Request to dictionary dir in dict_mode=false\n");
        };
        const std::string& get_dest_dir() const {
            if (!dest_dir.empty()) {
                return dest_dir;
            }
            return src_dir;
        };

//-----------------------------------------------------------------------------------------

};
