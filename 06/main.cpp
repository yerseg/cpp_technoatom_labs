//Linux Test
//g++ -pthread -g main.cpp -o check
//valgrind --leak-check=full ./check
//valgrind --tool=massif ./check
//ms_print ./massif.out.xxxxx

#include "external_sort.h"

int main()
{
	std::string test_filename = "./test.txt";
	binaryFileGenerate<uint64_t>(test_filename, 16777216); //размер файла в байтах
	auto result_filename = Sort<uint64_t>(test_filename, 4194304); //размер блока в байтах (примерно в
	printBinaryFile<uint64_t>(result_filename);               //в 2 раза меньше целевого значения памяти
	                                                          //(в нашем случае 8 МБ)
	return 0;
}
