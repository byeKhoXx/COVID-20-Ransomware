#include <iostream>
#include <windows.h>
#include <fstream>
#include <future>
#include <string>
#include <experimental/filesystem>
#include "PE.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 4)
	{
		cout << "Usage: " << argv[0] << " <path_exe> <patched_path_exe> shellcode\n";
		return EXIT_FAILURE;
	}
    std::experimental::filesystem::path input_file{ argv[1] };
	if (exists(input_file))
	{
		if (!is_regular_file(input_file))
		{
			std::cout << "It's not a regular file\n";
			return EXIT_FAILURE;
		}
	}
	else
	{
		std::cout << "File does not exist\n";
		return EXIT_FAILURE;
	}
	auto outfile = std::experimental::filesystem::path{ argv[2] }.generic_string();

	tuple<bool, char*, fstream::pos_type>  bin = PE::OpenBinary(input_file.generic_string());
	if (!get<0>(bin))
	{
		cout << "Error to open file";
		return EXIT_FAILURE;
	}
	char* PE_file = get<1>(bin);
	size_t size_of_pe = get<2>(bin);

	// Get shellcode
	string xcode = argv[3];
    printf("string is %s",xcode);
    printf("ASI EH");
    
}