#include <iostream>
#include <map>
#include <fstream>
#include <stdint-gcc.h>
#include <vector>
#include <algorithm>

class A
{
  public:
	virtual void hello()
	{
		std::cout << "Hello_A";
	}
	
};

class B : public A
{
  public:

	void hello()
	{
		std::cout << "Hello_B";
	}
	
};

bool compare(const std::pair<std::string, uint32_t>& a, const std::pair<std::string, uint32_t>& b)
{
	return a.second < b.second;
}

int main()
{
	std::string filename = "D:\\Dev\\CLion\\exam\\in.txt";
	std::map<std::string, uint32_t> dict;
	std::ifstream in(filename, std::ios::in);
	std::string word;
	
	while (in >> word)
	{
		dict[word] += 1;
	}
	
	std::vector<std::pair<std::string, uint32_t>> vec(dict.begin(), dict.end());
	std::sort(vec.begin(), vec.end(), compare);
	
	int32_t vec_size = vec.size();
	
	std::cout << "Полный словарь" << std::endl;
	for (const auto& v : vec)
	{
		std::cout << v.first << ": " << v.second << std::endl;
	}
	std::cout << std::endl;
	
	std::cout << "ТОП-5 наиболее часто встречаемых" << std::endl;
	for (int32_t i = vec_size - 1; i >= vec_size - 5; i--)
	{
		std::cout << vec[i].first << ": " << vec[i].second << std::endl;
	}
	std::cout << std::endl;
	
	std::cout << "ТОП-5 наиболее редко встречаемых" << std::endl;
	for (int32_t i = 0; i < 5; i++)
	{
		std::cout << vec[i].first << ": " << vec[i].second << std::endl;
	}
	
	A a;
	B b;
	b.hello();
	
	return 0;
}