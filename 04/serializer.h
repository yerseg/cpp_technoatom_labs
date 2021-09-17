#ifndef _SERIALIZER_H
#define _SERIALIZER_H

#include <sstream>

enum class Error
{
	NoError,
	CorruptedArchive
};

class Serializer
{
  public:
	explicit Serializer(std::ostream& out)
			: _out(out)
	{
	}
	
	template<class T>
	Error save(T& object)
	{
		return object.serialize(*this);
	}
	
	template<class... ArgsT>
	Error operator()(ArgsT&&... args)
	{
		return process(std::forward<ArgsT>(args)...);
	}
  
  private:
	Error process(bool val)
	{
		_out << (val ? "true" : "false") << separator;
		return Error::NoError;
	}
	
	Error process(uint64_t val)
	{
		_out << val << separator;
		return Error::NoError;
	}
	
	template<class T>
	Error process(T&& value)
	{
		return Error::CorruptedArchive;
	}
	
	template<class T, class... ArgsT>
	Error process(T&& value, ArgsT&& ... args)
	{
		if (process(std::forward<T>(value)) == Error::CorruptedArchive)
		{
			return Error::CorruptedArchive;
		}
		else
		{
			return process(std::forward<ArgsT>(args)...);
		}
	}
	
	static constexpr char separator = ' ';
	std::ostream& _out;
};

class Deserializer
{
  public:
	explicit Deserializer(std::istream& in)
			: _in(in) {};
	
	template<class T>
	Error load(T& object)
	{
		return object.serialize(*this);
	}
	
	template<class... ArgsT>
	Error operator()(ArgsT&&... args)
	{
		return process(std::forward<ArgsT>(args)...);
	}
  
  private:
	Error process(bool& value)
	{
		std::string text;
		_in >> text;
		
		if (text == "true")
			value = true;
		else if (text == "false")
			value = false;
		else
			return Error::CorruptedArchive;
		
		return Error::NoError;
	}
	
	Error process(uint64_t& value)
	{
		std::string text;
		_in >> text;
		
		if (text.empty() || text[0] == '-' || text[0] == '\0')
		{
			return Error::CorruptedArchive;
		}
		try
		{
			value = static_cast<uint64_t>(std::stoi(text));
		}
		catch(const std::invalid_argument& ia)
		{
			return Error::CorruptedArchive;
		}
		
		return Error::NoError;
	}
	
	template <class T, class... ArgsT>
	Error process(T&& value, ArgsT&&... args)
	{
		if (process(std::forward<T>(value)) == Error::CorruptedArchive)
		{
			return Error::CorruptedArchive;
		}
		else
		{
			return process(std::forward<ArgsT>(args)...);
		}
	}
	
	static constexpr char separator = ' ';
	std::istream& _in;
};

#endif //_SERIALIZER_H
