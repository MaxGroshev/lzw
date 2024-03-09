#include "lzw.hpp"
#include "utils.hpp"

using namespace my_compress;

//-----------------------------------------------------------------------------------------

int main(int argc, char** argv) {

    auto start_time = time_control::chrono_cur_time ();
    try {
        set_up_t set_up {argc, argv};
        int encoder_init_size = 256;
        int decoder_init_size = 0;
        lzw_t encoder {set_up, encoder_init_size, decoder_init_size};

        if (encoder.is_train_mode()) {
            encoder.train();
        }
        // else {
            std::vector<int> res = encoder.compress();
            std::cout << encoder.get_compress_dict_size() << "\n";
        // }
        // std::vector<int> res1 = encoder.compress();
    } catch(std::runtime_error& err) {
        std::cout << "ERROR: " << err.what() << '\n';
    } catch(...) {
        std::cout << "Unexpected error\n";
    }
    auto end_time = time_control::chrono_cur_time ();
    time_control::show_run_time(start_time, end_time, "Compression wall time: ");

    return 0;
}
