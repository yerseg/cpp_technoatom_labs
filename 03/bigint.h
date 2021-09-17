#ifndef _BIGINT_H
#define _BIGINT_H

#include <cmath>
#include <algorithm>

class BigInt
{
  public:
	BigInt() : _sign(0), _bits(nullptr), _bits_number(0) {}
	
	BigInt(const int64_t value);
	
	BigInt(const BigInt& value);
	
	~BigInt();
	
	friend std::ostream& operator<<(std::ostream& out, const BigInt& bigint);
	
	bool operator==(const BigInt& other) const;
	
	bool operator!=(const BigInt& other) const;
	
	bool operator<(const BigInt& other) const;
	
	bool operator<=(const BigInt& other) const;
	
	bool operator>(const BigInt& other) const;
	
	bool operator>=(const BigInt& other) const;
	
	BigInt& operator=(const BigInt& other);
	
	BigInt& operator=(const int64_t& value);
	
	BigInt operator-() const;
	
	BigInt operator+(const BigInt& other) const;
	
	BigInt operator-(const BigInt& other) const;
  
  private:
	const int32_t MaxValue = static_cast<const int32_t>(1E9); //основание СС
	const int32_t MinValue = static_cast<const int32_t>(-1E9); //(-) основание СС
	const int64_t MaxValueSq = static_cast<const int64_t>(1E18); //основание СС в квадрате
	
	//храним числа в Системе Счисления по основанию 10^9
	int32_t* _bits; //разряды числа
	int32_t _sign; //знак числа 1 или -1
	uint32_t _bits_number; //количество разрядов
};

BigInt::BigInt(const int64_t value) {
	if (MinValue < value && value < MaxValue)
	{
		if (value >= 0)
		{
			_sign = 1;
		}
		else
		{
			_sign = -1;
		}
		_bits = new int32_t[1];
		_bits[0] = static_cast<int32_t>(std::abs(value));
		_bits_number = 1;
	}
	else
	{
		if (value < 0)
		{
			_sign = -1;
		}
		else
		{
			_sign = 1;
		}
		
		if (value >= MaxValueSq)
		{
			_bits = new int32_t[3];
			_bits[2] = static_cast<int32_t>(std::abs(value) / MaxValueSq);
			_bits[1] = static_cast<int32_t>((std::abs(value) - _bits[2] * MaxValueSq) / MaxValue);
			_bits[0] = static_cast<int32_t>((std::abs(value) - _bits[2] * MaxValueSq) - _bits[1] * MaxValue);
			_bits_number = 3;
		}
		else
		{
			_bits = new int32_t[2];
			_bits[1] = static_cast<int32_t>(std::abs(value) / MaxValue);
			_bits[0] = static_cast<int32_t>(std::abs(value) - _bits[1] * MaxValue);
			_bits_number = 2;
		}
	}
}

BigInt::BigInt(const BigInt& value) : _sign(value._sign),
                                      _bits(new int32_t[value._bits_number]),
                                      _bits_number(value._bits_number)
{
	std::copy(value._bits, value._bits + _bits_number, _bits);
}

BigInt::~BigInt() {
	delete[] _bits;
}

std::ostream& operator<<(std::ostream& out, const BigInt& bigint)
{
	if (bigint._bits_number == 0 || bigint._bits_number == 1 && bigint._bits[0] == 0)
	{
		out << static_cast<int32_t>(0);
	}
	if (bigint._bits != nullptr)
	{
		if (bigint._sign == -1)
			out << '-';
		else;
	}
	for (int i = bigint._bits_number - 1; i >= 0; i--)
	{
		if (bigint._bits[i] != 0)
		{
			if (i != bigint._bits_number - 1)
			{
				auto s = std::to_string(bigint._bits[i]);
				for (int j = 0; j < 9 - s.size(); j++)
				{
					static_cast<int32_t>(0);
				}
			}
			out << static_cast<int32_t>(bigint._bits[i]);
		}
		else if (i != bigint._bits_number - 1)
		{
			for (int j = 0; j < 9; j++)
			{
				static_cast<int32_t>(0);
			}
		}
	}
	return out;
}

bool BigInt::operator==(const BigInt& other) const {
	if (_sign != other._sign)
	{
		return false;
	}
	if (_bits != nullptr && other._bits != nullptr)
	{
		if (_bits_number != other._bits_number)
		{
			return false;
		}
		for (int i = 0; i < _bits_number; i++)
		{
			if (_bits[i] != other._bits[i])
				return false;
		}
		return true;
	}
	else if (_bits == nullptr && other._bits == nullptr)
		return (_sign == other._sign);
	
	else
		return false;
	
}

bool BigInt::operator!=(const BigInt& other) const {
	return !(operator==(other));
}

bool BigInt::operator<(const BigInt& other) const {
	if (_sign < other._sign)
	{
		return true;
	}
	else if (_sign > other._sign)
	{
		return false;
	}
	if (_bits != nullptr && other._bits != nullptr)
	{
		if (_sign == -1 && other._sign == -1)
		{
			if (_bits_number < other._bits_number)
			{
				return false;
			}
			else if (_bits_number > other._bits_number)
			{
				return true;
			}
			
			for (int i = _bits_number - 1; i >= 0; i--)
			{
				if (_bits[i] > other._bits[i])
					return true;
				else if (_bits[i] < other._bits[i])
					return false;
			}
			return true;
		}
		else
		{
			if (_bits_number < other._bits_number)
			{
				return true;
			}
			else if (_bits_number > other._bits_number)
			{
				return false;
			}
			
			for (int i = _bits_number - 1; i >= 0; i--)
			{
				if (_bits[i] < other._bits[i])
					return true;
				else if (_bits[i] >= other._bits[i])
					return false;
			}
			return true;
		}
	}
	else if (_bits == nullptr && other._bits == nullptr)
		return (_sign < other._sign);
	
	else
		return (_bits == nullptr && other._bits != nullptr);
}

bool BigInt::operator<=(const BigInt& other) const {
	return (operator==(other) || operator<(other));
}

bool BigInt::operator>(const BigInt& other) const {
	return !(operator<=(other));
}

bool BigInt::operator>=(const BigInt& other) const {
	return (operator==(other) || operator>(other));
}

BigInt& BigInt::operator=(const BigInt& other) {
	_sign = other._sign;
	_bits_number = other._bits_number;
	delete[] _bits;
	_bits = new int32_t[_bits_number];
	std::copy(other._bits, other._bits + _bits_number, _bits);
	return *this;
}

BigInt& BigInt::operator=(const int64_t& value) {
	auto tmp = BigInt(value);
	*this = tmp;
	return *this;
}

BigInt BigInt::operator-() const {
	if (_bits_number == 1 && _bits[0] == 0)
		return *this;
	BigInt tmp = *this;
	tmp._sign = (-1) * _sign;
	return tmp;
}

BigInt BigInt::operator+(const BigInt& other) const
{
	BigInt tmp; // max(this, other)
	
	if (_sign == 1 && other._sign == 1 || _sign == -1 && other._sign == -1)
	{
		int flag_tmp; //0 - other, 1 - this
		
		//выбор максимального по модулю
		if (other < *this)
		{
			if (_sign == -1 && other._sign == -1)
			{
				tmp = other;
				flag_tmp = 0;
			}
			else
			{
				tmp = *this;
				flag_tmp = 1;
			}
			
		}
		else
		{
			if (_sign == -1 && other._sign == -1)
			{
				tmp = *this;
				flag_tmp = 1;
			}
			else
			{
				tmp = other;
				flag_tmp = 0;
			}
		}
		
		//работа с разрядами, ориентируемся по максимальному по модулю числу
		for (int i = 0; i < std::min(_bits_number, other._bits_number); i++)
		{
			if (flag_tmp == 0)
			{
				tmp._bits[i] = tmp._bits[i] + _bits[i];
			}
			else
			{
				tmp._bits[i] = tmp._bits[i] + other._bits[i];
			}
		}
		
		//выполнение переносов
		for (int j = 0; j < std::max(_bits_number, other._bits_number); j++)
		{
			if (tmp._bits[j] > MaxValue)
			{
				tmp._bits[j] = tmp._bits[j] - MaxValue;
				if (j + 1 < std::max(_bits_number, other._bits_number))
					tmp._bits[j + 1] = tmp._bits[j + 1] + 1;
				else //если переполнение, то выделяем память еще под один разряд
				{
					auto* buf = new int32_t[tmp._bits_number + 1];
					std::copy(tmp._bits, tmp._bits + tmp._bits_number, buf);
					buf[j + 1] = 1;
					std::swap(tmp._bits, buf);
					delete[] buf;
				}
			}
		}
	}
	else if (_sign == -1 && other._sign == 1)
	{
		if (*this == other)
		{
			return 0;
		}
		
		return other - (-(*this));
	}
	else if (_sign == 1 && other._sign == -1)
	{
		if (*this == other)
		{
			return 0;
		}
		
		return *this - (-(other));
	}
	return tmp;
}

BigInt BigInt::operator-(const BigInt& other) const
{
	BigInt tmp; // max(this, other)
	
	if (_sign == 1 && other._sign == 1)
	{
		if (*this == other)
		{
			return 0;
		}
		
		int flag_tmp; //0 - other, 1 - this
		//находим максимальное по модулю
		if (other < *this)
		{
			tmp = *this;
			flag_tmp = 1;
		}
		else
		{
			tmp = other;
			flag_tmp = 0;
			tmp._sign = -1;
		}
		//из максимального по модулю отнимаем разряды минимального
		for (int i = 0; i < std::min(_bits_number, other._bits_number); i++)
		{
			if (flag_tmp == 0)
			{
				tmp._bits[i] = tmp._bits[i] - _bits[i];
			}
			else
			{
				tmp._bits[i] = tmp._bits[i] - other._bits[i];
			}
		}
		//выполняем переносы
		for (int j = 0; j < std::max(_bits_number, other._bits_number); j++)
		{
			if (tmp._bits[j] < 0)
			{
				tmp._bits[j] = tmp._bits[j] + MaxValue;
				tmp._bits[j + 1] = tmp._bits[j + 1] - 1;
			}
		}
		
		int k = tmp._bits_number - 1;
		//удаляем нулевые старшие биты из памяти
		if (tmp._bits[k] == 0)
		{
			while (tmp._bits[k] == 0 && k >= 0)
			{
				k--;
			}
			tmp._bits_number = k + 1;
			auto* buf = new int32_t[tmp._bits_number];
			std::copy(tmp._bits, tmp._bits + tmp._bits_number, buf);
			std::swap(tmp._bits, buf);
			delete[] buf;
		}
	}
	else if (_sign == -1 && other._sign == 1)
	{
		return  -(other + (-(*this)));
	}
	else if (_sign == 1 && other._sign == -1)
	{
		return -other + (*this);
	}
	else
	{
		if (*this == other)
		{
			return 0;
		}
		
		return -other + *this;
	}
	return tmp;
}

#endif //_BIGINT_H
