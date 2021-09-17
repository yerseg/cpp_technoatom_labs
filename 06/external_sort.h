#ifndef EXTERNAL_SORT_H
#define EXTERNAL_SORT_H

#include <fstream>
#include <vector>
#include <iostream>
#include <thread>
#include <algorithm>
#include <ctime>
#include <cstdio>
#include <cmath>

std::ifstream::pos_type fileSize(const std::string& filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

template<class T>
void binaryFileGenerate(const std::string& filename, size_t size)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	
	std::ofstream out(filename, std::ios::binary);
	
	for (int i = 0; i < size / sizeof(T); i++)
	{
		T x = rand() % 100000;
		out.write((char*) &x, sizeof(x));
	}
//	out.close();
}

template<class T>
void printBinaryFile(const std::string& filename)
{
	std::ifstream in(filename, std::ios::binary);
	
	for (int i = 0; i < fileSize(filename) / sizeof(T); i++)
	{
		T x;
		in.read((char*) &x, sizeof(x));
		std::cout << x << std::endl;
	}
//	in.close();
}

template<class T>
void splitBinaryFile(const std::string& oldFilename, const std::string& filename1, const std::string& filename2)
{
	std::ifstream old_file(oldFilename, std::ios::binary);
	std::ofstream new_file(filename1, std::ios::binary);
	
	for (int i = 0; i < fileSize(oldFilename) / (2 * sizeof(T)); i++)
	{
		T x;
		old_file.read((char*) &x, sizeof(x));
		new_file.write((char*) &x, sizeof(x));
	}
//	new_file.close();
	
	std::ofstream new_file2(filename2, std::ios::binary);
	for (int i = fileSize(oldFilename) / (2 * sizeof(T)); i < fileSize(oldFilename) / sizeof(T); i++)
	{
		T x;
		old_file.read((char*) &x, sizeof(x));
		new_file2.write((char*) &x, sizeof(x));
	}
//	new_file2.close();
//	old_file.close();
}

template<class T>
void merge(std::vector<T>& chunk, size_t s, size_t q, size_t r)
{
	auto* chunk2 = new std::vector<T>(q - s + 1);
	auto it2 = chunk2->begin(),
			it1 = chunk.begin() + q + 1,
			it = chunk.begin() + s;
	
	std::copy(it, it1, it2);
	while (it2 != chunk2->end() && it1 != chunk.begin() + r + 1)
	{
		if (*it1 > *it2)
		{
			*it = *it2;
			++it2;
		}
		else
		{
			*it = *it1;
			++it1;
		}
		++it;
	}
	if (it1 == chunk.begin() + r + 1)
		std::copy(it2, chunk2->end(), it);
	else
		std::copy(it1, chunk.begin() + r + 1, it);
	delete chunk2;
}

template<class T>
void external_sort(const std::string& filename, size_t memory)
{
	const size_t type_size = sizeof(T);
	const uint64_t file_size = static_cast<const uint64_t>(fileSize(filename));
	
	std::fstream data(filename, std::ios::in | std::ios::out | std::ios::binary);
	
	const auto chunk_number = static_cast<uint64_t>(ceilf(static_cast<float>(file_size) / static_cast<float>(memory)));
	const size_t chunk_size = memory / type_size - (memory / type_size) % 2;
	const size_t chunk_byte_size = chunk_size * type_size;
	const size_t half_chunk_byte_size = chunk_byte_size / 2;
	const size_t half_chunk_size = chunk_size / 2;
	
	auto* chunk = new std::vector<T>(chunk_size);
	
	for (uint64_t i = 0; i < chunk_number; ++i)
	{
		data.seekg(chunk_byte_size * i);
		data.read((char*) chunk->data(), chunk_byte_size);
		
		std::sort(chunk->begin(), chunk->end());
		
		data.seekp(chunk_byte_size * i);
		data.write((char*) chunk->data(), chunk_byte_size);
	}
	
	
	int64_t s = chunk_number, start = 0;
	while (s > 0)
	{
		//Нижняя половина
		data.seekp(half_chunk_byte_size * start);
		data.read((char*) chunk->data(), half_chunk_byte_size);
		
		for (int64_t q = 1; q < s; ++q)
		{
			data.seekg(half_chunk_byte_size * start + chunk_byte_size * q);
			data.read((char*) (chunk->data() + half_chunk_size), half_chunk_byte_size);
			
			merge<T>(*chunk, 0, half_chunk_size - 1, chunk_size - 1);
			
			data.seekp(half_chunk_byte_size * start + chunk_byte_size * q);
			data.write((char*) (chunk->data() + half_chunk_size), half_chunk_byte_size);
		}
		
		data.seekp(half_chunk_byte_size * start);
		data.write((char*) chunk->data(), half_chunk_byte_size);
		
		//Верхняя половина
		data.seekp(half_chunk_byte_size * start + chunk_byte_size * s - half_chunk_byte_size);
		data.read((char*) (chunk->data() + half_chunk_size), half_chunk_byte_size);
		for (int64_t q = s - 2; q >= 0; --q)
		{
			data.seekg(half_chunk_byte_size * (start + 1) + chunk_byte_size * q);
			data.read((char*) chunk->data(), half_chunk_byte_size);
			
			merge(*chunk, 0, half_chunk_size - 1, chunk_size - 1);
			
			data.seekp(half_chunk_byte_size * (start + 1) + chunk_byte_size * q);
			data.write((char*) chunk->data(), half_chunk_byte_size);
		}
		
		data.seekg(half_chunk_byte_size * start + chunk_byte_size * s - half_chunk_byte_size);
		data.write((char*) (chunk->data() + half_chunk_size), half_chunk_byte_size);
		
		--s;
		++start;
		
		for (int64_t p = 0; p < s; ++p)
		{
			data.seekp(half_chunk_byte_size * start + chunk_byte_size * p);
			data.read((char*) chunk->data(), chunk_byte_size);
			
			merge(*chunk, 0, half_chunk_size - 1, chunk_size - 1);
			
			data.seekg(half_chunk_byte_size * start + chunk_byte_size * p);
			data.write((char*) chunk->data(), chunk_byte_size);
		}
	}
	delete chunk;
}

template<class T>
void mergeFiles(const std::string& filename1, const std::string& filename2,
                const std::string& filename_final,
                const size_t memory)
{
	const size_t type_size = sizeof(T);
	const uint64_t file_size1 = static_cast<const uint64_t>(fileSize(filename1));
	const uint64_t file_size2 = static_cast<const uint64_t>(fileSize(filename2));
	
	std::ifstream data1(filename1, std::ios::binary);
	std::ifstream data2(filename2, std::ios::binary);
	std::ofstream data3(filename_final, std::ios::binary);
	
	const auto chunk_number1 = static_cast<uint64_t>(ceilf(
			static_cast<float>(file_size1) / static_cast<float>(memory)));
	const auto chunk_number2 = static_cast<uint64_t>(ceilf(
			static_cast<float>(file_size2) / static_cast<float>(memory)));
	const size_t chunk_size = memory / type_size - (memory / type_size) % 2,
			chunk_byte_size = chunk_size * type_size;
	
	auto* chunk = new std::vector<T>(2 * chunk_size);
	
	uint64_t i = 0, k = 0;
	
	data1.seekg(chunk_byte_size * i);
	data1.read((char*) chunk->data(), chunk_byte_size);
	
	data2.seekg(chunk_byte_size * k);
	data2.read((char*) (chunk->data() + chunk_size), chunk_byte_size);
	
	auto it1 = chunk->begin(),
			it2 = chunk->begin() + chunk_size;
	
	while (i < chunk_number1 && k < chunk_number2)
	{
		while (it1 != (chunk->begin() + chunk_size) && it2 != (chunk->begin() + chunk_size * 2))
		{
			if (*it1 > *it2)
			{
				data3.write((char*) &(*it2), sizeof(T));
				++it2;
			}
			else
			{
				data3.write((char*) &(*it1), sizeof(T));
				++it1;
			}
		}
		if (it1 == (chunk->begin() + chunk_size))
		{
			++i;
			if (i < chunk_number1)
			{
				data1.seekg(chunk_byte_size * i);
				data1.read((char*) chunk->data(), chunk_byte_size);
				it1 = chunk->begin();
			}
		}
		else
		{
			++k;
			if (k < chunk_number2)
			{
				data2.seekg(chunk_byte_size * k);
				data2.read((char*) (chunk->data() + chunk_size), chunk_byte_size);
				it2 = chunk->begin() + chunk_size;
			}
		}
	}
	if (i == chunk_number1)
	{
		while (it2 != (chunk->begin() + chunk_size * 2))
		{
			data3.write((char*) &(*it2), sizeof(T));
			++it2;
		}
		++k;
		
		for (uint64_t j = k; j < chunk_number2; j++)
		{
			data2.seekg(chunk_byte_size * j);
			data2.read((char*) (chunk->data() + chunk_size), chunk_byte_size);
			data3.write((char*) (chunk->data() + chunk_size), chunk_byte_size);
		}
	}
	else
	{
		while (it1 != chunk->begin() + chunk_size)
		{
			data3.write((char*) &(*it1), sizeof(T));
			++it1;
		}
		++i;
		for (uint64_t j = i; j < chunk_number1; j++)
		{
			data2.seekg(chunk_byte_size * j);
			data2.read((char*) (chunk->data() + chunk_size), chunk_byte_size);
			data3.write((char*) (chunk->data() + chunk_size), chunk_byte_size);
		}
	}
//	data1.close();
//	data2.close();
//	data3.close();
	delete chunk;
	remove(filename1.c_str());
	remove(filename2.c_str());
}

template<class T>
const size_t fillZeroes(const std::string& filename, size_t memory)
{
	std::fstream data(filename, std::ios::in | std::ios::out | std::ios::binary);
	const uint64_t file_size = static_cast<const uint64_t>(fileSize(filename));
	const size_t type_size = sizeof(T);
	const size_t chunk_size = memory / type_size;
	const size_t size = (file_size % memory) / type_size;
	const size_t zeroes = size == 0 ? 0 : chunk_size - size;
	auto* chunk = new std::vector<T>(zeroes);
	const size_t chunk_byte_size = chunk_size * type_size;
	const uint64_t chunk_number = file_size / memory;
	
	data.seekp(chunk_byte_size * chunk_number + size * type_size);
	data.write((char*) chunk->data(), type_size * zeroes);
//	data.close();
	delete chunk;
	return zeroes;
}

template<class T>
std::string deleteZeroes(std::string filename, const size_t zeroes)
{
	std::string new_name = "./result.txt";
	
	if (zeroes == 0)
	{
		remove(new_name.c_str());
		return filename;
	}
	else
	{
		
		std::ifstream old(filename, std::ios::binary);
		std::ofstream new_file(new_name, std::ios::binary);
		old.seekg(zeroes * sizeof(T));
		for (size_t i = 0; i < (fileSize(filename) / sizeof(T)) - zeroes; i++)
		{
			T x;
			old.read((char*) &x, sizeof(x));
			new_file.write((char*) &x, sizeof(x));
		}
		printBinaryFile<T>(new_name);
//		old.close();
//		new_file.close();
		
		remove(filename.c_str());
		return new_name;
	}
}

template<class T>
std::string Sort(std::string filename, size_t memory)
{
	std::string f1_name = "./help_1.txt";
	std::string f2_name = "./help_2.txt";
	std::string f3_name = "./res.txt";
	splitBinaryFile<T>(filename, f1_name, f2_name);
	const size_t zeroes = fillZeroes<T>(f2_name, memory);
	
	std::thread t1(external_sort<T>, f1_name, memory / 2);
	std::thread t2(external_sort<T>, f2_name, memory / 2);
	
	t1.join();
	t2.join();
	
	mergeFiles<T>(f1_name, f2_name, f3_name, memory / 2);
	
	return deleteZeroes<T>(f3_name, zeroes);
}

#endif // EXTERNAL_SORT