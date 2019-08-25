#include <algorithm>
#include <cstdint>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <cstring>

namespace otp {

    struct Config 
    {
        Config()
            : blockSize(80)
            , numBlocks(0)
			, binary(false)
        {
        }

        std::size_t blockSize;
        std::size_t numBlocks;
		bool binary; 
    };

	void readBlockSizeAndNumBlocks(Config& config, size_t pos, size_t num, char const * const argv[])
	{
		std::stringstream ss;
		ss << argv[pos];
		ss >> config.numBlocks;

		if(pos+1 < num) 
		{
			std::stringstream ss2;
			ss2 << argv[pos+1];
			ss2 >> config.blockSize;
		}

		std::cerr << "numblocks=" << config.numBlocks
			<< "blockSize" << config.blockSize << "\n";
	}

    Config parseOptions(int argc, char const * const argv[])
    {
        Config config;

        if(argc < 2)
        {
            config.numBlocks = 24;
            return config;
        }

		if(std::strcmp(argv[1], "--binary") == 0)
		{
			config.binary = true;
			readBlockSizeAndNumBlocks(config, 2, argc, argv);
		}
		else 
		{
			readBlockSizeAndNumBlocks(config, 1, argc, argv);	
		}

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

    Block filterUnprintable(Block& block)
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
		auto block = createBlock(config);
		if(!config.binary) 
		{
			block = filterUnprintable(block);
		}

        std::cout << block << (config.binary ? "" : "\n");
    }

    std::cout << (config.binary ? "" : "\n");
    return 0;
}
