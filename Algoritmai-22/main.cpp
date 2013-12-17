#include <vector>
#include <numeric>
#include <ostream>
#include <string>
#include <random>
#include <functional>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

typedef size_t uint;
uint ops = 0;

vector<uint> ComputeBreaks(const vector<string> &words, uint m);
void Output(const vector<string> &words, ostream &stream, vector<uint> p, uint j);
vector<string> RandomData(uint diffWords, uint wordCount, uint maxLength);

template<uint N>
inline uint Pow(uint a)
{
	return a * Pow<N - 1>(a);
}

template<>
inline uint Pow<0>(uint a)
{
	return 1;
}

int main()
{
	const uint counts[] = { 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };
	ofstream out("rez.csv", ios::trunc);
	uint time;
	for (auto count : counts)
	{
		ops = 0;
		auto words = RandomData(100, count, 10);
		time = clock();
		auto breaks = ComputeBreaks(words, 20);
		time = clock() - time;
		cout << " count " << count << " time " << time << " ops " << ops << endl;
		out << count << ';' << time << ';' << ops << ";\n";
	}
	system("pause");
	return 0;
}

vector<string> RandomData(uint diffWords, uint wordCount, uint maxLength)
{
	vector<string> things;
	default_random_engine gen;
	uniform_int_distribution<uint> charDist(97, 122);
	uniform_int_distribution<uint> lengthDist(0, maxLength);
	uniform_int_distribution<uint> shuffleDist(0, diffWords - 1);
	string temp;
	for (uint i = 0; i < diffWords; i++)
	{
		uint l = lengthDist(gen);
		for (uint j = 0; j < l; j++)
		{
			temp += charDist(gen);
		}
		things.push_back(move(temp));
	}
	vector<string> ret;
	for (uint i = 0; i < wordCount; i++)
	{
		ret.push_back(things[shuffleDist(gen)]);
	}
	return ret;
}

vector<uint> ComputeBreaks(const vector<string> &words, uint m)
{
	const int max = numeric_limits<int>::max();
	const uint n = words.size();
	vector<uint> p, c;
	vector<vector<int>> extras(n, vector<int>(n));
	vector<vector<uint>> lc(n, vector<uint>(n));

	ops += 7;

	for (uint i = 0; i < n; i++)
	{
		ops += 3;
		extras[i][i] = m - words[i].length();
		for (uint j = i + 1; j < n; j++)
		{
			ops += 4;
			extras[i][j] = extras[i][j - 1] - words[j].length() - 1;
		}
	}
	for (uint i = 0; i < n; i++)
	{
		ops += 2;
		for (uint j = i; j < n; j++)
		{
			ops += 2;
			if (extras[i][j] < 0)
			{
				ops += 2;
				lc[i][j] = max;
			}
			else
			{
				if (j == n - 1 && extras[i][j] >= 0)
				{
					ops += 2;
					lc[i][j] = 0;
				}
				else
				{
					ops += 2;
					lc[i][j] = Pow<3>(extras[i][j]);
				}
			}
		}
	}
	c.push_back(0);
	for (uint j = 0; j < n; j++)
	{
		ops += 4;
		c.push_back(max);
		p.emplace_back();
		for (uint i = 0; i <= j; i++)
		{
			ops += 2;
			if (c[i] + lc[i][j] < c[j+1])
			{
				ops += 3;
				c[j+1] = c[i] + lc[i][j];
				p[j] = i;
			}
		}
	}
	return p;
}

void Output(const vector<string> &words, ostream &stream, vector<uint> p, uint j)
{
	uint i = p[j];
	if (i != 0)
		Output(words, stream, p, i - 1);
	for (uint l = i; l <= j; l++)
		stream << words[l] << ' ';
	stream << endl;
}