#include <iostream>
#include <vector>
#include "numbers.dat"

const static int MAX_VALUE = 10e5;

bool isPrime(int num) {
    if (num == 1)
        return false;
    int i = 2;
    int j = 0;
    while(i*i <= num && j != 1) {
        if (num % i == 0)
            j = 1;
        i += 1;
    }
    if (j == 0)
        return true;
    else
        return false;
}

int search_begin(const int arr[], int q){
    for (int i = 0; i < Size; i++)
        if (arr[i] == q)
            return i;
    return 0;
}

int search_end(const int arr[], int q){
    for (int i = Size - 1; i > 0; i--)
        if (arr[i] == q)
            return i;
    return 0;
}

int prime_count(const int arr[], std::vector<char>& prime, int begin, int end){
    int counter = 0;
    int first = search_begin(arr, begin);
    int last = search_end(arr, end);
    if (first == -1 || last == -1)
        return 0;
    for (int i = first; i < last; ++i){
        if (prime[arr[i]])
            ++counter;
    }
    return counter;
}

void sieve(std::vector<char>& prime){
    prime[0] = prime[1] = false;
    for (int i = 2; i <= MAX_VALUE; ++i)
        if (prime[i])
            if (i * 1ll * i <= MAX_VALUE)
                for (int j = i*i; j <= MAX_VALUE; j += i)
                    prime[j] = false;
}

int main(int argc, char* argv[]) {
    if (argc == 1 || argc % 2 != 1)
        return -1;

    std::vector<char> prime (MAX_VALUE + 1, true);
    sieve(prime);

    for (int j = 1; j < argc; j++){
        std::cout << prime_count(Data, prime, std::atoi(argv[j]), std::atoi(argv[j+1])) << std::endl;
        j++;
    }

    return 0;
}