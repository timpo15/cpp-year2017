#include <iostream>
#include <fstream>
#include <cstring>
#include "huffman_compressor/huffman_processor.h"

void print_help() {
    std::cerr << "USAGE:\n" << "./huffman_compressor <options> <input_file> <output_file>\n"
              << "Options:\n"
              << "\t-q quiet mode (no statistics), by default is verbose mod (full statistics)\n"
              << "\t-c create archive mode \n"
              << "\t-x extract archive mode \n"
              << "\t-h view help \n";
}

int main(int argc, const char **argv) {
    if (argc == 1 || argc == 3 ) {
        print_help();
        return 0;
    }

    clock_t start_time;
    clock_t finish_time;

    bool quiet = false,
            extract = false,
            compress = false,
            help = false;

    std::ifstream input;
    std::ofstream output;

    std::function<void(double)> handler = [](double progress) {
        std::printf("Progress: %4.2f% \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b", progress);
        std::cout.flush();
    };


    try {
        size_t tire_count = 0;
        size_t pos = 0;
        std::string option;
        if (argv[1][0] != '-') {
            print_help();
            return 0;
        }
        while (pos < std::strlen(argv[1])) {
            if (argv[1][pos] == '-') {
                pos++;
                continue;
            }
            option += argv[1][pos++];
        }

        if (option.find('q') != std::string::npos) {
            quiet = true;
            handler = [](double a) { return; };

        }
        if (option.find('x') != std::string::npos) {
            extract = true;

        }
        if (option.find('c') != std::string::npos) {
            compress = true;
        }
        if (option.find('h') != std::string::npos) {
            help = true;
        }
        if (option.find('h') != std::string::npos) {
            help = true;
        }
        if (help) {
            print_help();
            return 0;
        }

        if (compress & extract) {
            std::cerr << "Wrong combination of options" << "\n";
        }
        huffman_processor huffmanProcessor;
        input = std::ifstream(argv[2], std::ifstream::binary);
//        input.exceptions(std::ifstream::failbit);
        output = std::ofstream(argv[3], std::ifstream::binary);
//        output.exceptions(std::ofstream::failbit);

        std::pair<size_t, size_t> t;
        if (compress) {
            start_time = clock();
            t = huffmanProcessor.encode(input, output, handler);
            finish_time = clock();
        } else if (extract){
            start_time = clock();
            t = huffmanProcessor.decode(input, output, handler);
            finish_time = clock();
        } else {
            print_help();
            input.close();
            output.close();
            return 0;
        }
        if (!quiet) {
            double time = ((finish_time - start_time) * 1.0 / CLOCKS_PER_SEC);
            std::cout << "Successful!                   \n";
            std::cout << "Stats:\n";
            std::cout << "\tTime: " << time << "s\n";
            std::cout << "\tOriginal size: " << t.first * 1.0 / 1000 / 1000 << " Mb\n";
            std::cout << "\tCompressed size: " << t.second * 1.0 / 1000 /1000 << " Mb\n";
            std::cout << "\tCompress rate: " << t.first * 1.0 / t.second << "\n";
            std::cout << "\tAverage speed: " << (t.first / time) / 1000 / 1000 << " [Mb/s]\n";
        }
        input.close();
        output.close();

    } catch (std::runtime_error &e) {
        finish_time = clock();
        input.close();
        output.close();
        std::cerr << "Time: " << ((finish_time - start_time) * 1.0 / CLOCKS_PER_SEC) << "\n";
        std::cerr << "Error: " << e.what() << "\n";
    }
}




