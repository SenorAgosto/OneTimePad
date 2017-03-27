#include <cstdint>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>

namespace otp {

    struct Config 
    {
        Config()
            : blockSize(80)
            , numBlocks(0)
        {
        }

        std::size_t blockSize;
        std::size_t numBlocks;
    };

    Config parseOptions(int argc, char const * const argv[])
    {
        Config config;

        if(argc < 2)
        {
            config.numBlocks = 24;
            return config;
        }

        std::stringstream ss;
        ss << argv[1];
        ss >> config.numBlocks; 

        if(config.numBlocks == 0)
        {
            std::cerr << "Invalid command line options, defaulting to 24 blocks." << std::endl;
            config.numBlocks = 24;
        }

        return config;
    }

    using Block = std::string;
    Block createBlock(const Config& config)
    {
        Block buffer(config.blockSize, 0);
        arc4random_buf(&buffer[0], buffer.size());

        return buffer;
    }

    char getPrintable() 
    {
        char c = 0;
        do 
        {
            c = arc4random_uniform(255);
        }
        while(!std::isprint(c));

        return c;
    }

    Block filterUnprintable(Block&& block)
    {
        std::for_each(block.begin(), block.end(), [](char& c) {

            if(!std::isprint(c)) 
            { 
                c = getPrintable();   
            }
        });

        return block;
    }
}

int main(int argc, char const * const argv[]) 
{
    using namespace otp;
    Config config = parseOptions(argc, argv);

    for(std::size_t i = 0; i < config.numBlocks; ++i)
    {
        std::cout << filterUnprintable(createBlock(config)) << "\n";
    }
 
    std::cout << std::endl;
    return 0;
}
