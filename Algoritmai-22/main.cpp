#include <vector>
#include <numeric>
#include <ostream>
#include <string>
#include <random>
#include <functional>
#include <iostream>
using namespace std;

typedef size_t uint;

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
	auto &stream = cout;
	auto words = RandomData(3, 10, 7);
	auto breaks = ComputeBreaks(words, 10);
	for (auto &word : words)
		stream << word << ' ';
	stream << endl << endl;
	Output(words, stream, breaks, words.size() - 1);
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
	vector<vector<uint>> extras, lc;
	vector<uint> p, c;
	const uint max = numeric_limits<uint>::max();
	uint n = words.size();
	for (uint i = 0; i < n; i++)
	{
		extras.emplace_back();
		lc.emplace_back();
		for (uint j = 0; j < n; j++)
		{
			extras[i].emplace_back();
			lc[i].emplace_back();
		}
	}
	for (uint i = 0; i < n; i++)
	{
		extras[i][i] = m - words[i].length();
		for (uint j = i + 1; j < n; j++)
		{
			extras[i][j] = extras[i][j - 1] - words[i].length() -1;
		}
	}
	for (uint i = 0; i < n; i++)
	{
		for (uint j = i; j < n; j++)
		{
			if (extras[i][j] < 0)
			{
				lc[i][j] = max;
			}
			else
			{
				if (j == n && extras[i][j] >= 0)
				{
					lc[i][j] = 0;
				}
				else
				{
					lc[i][j] = Pow<3>(extras[i][j]);
				}
			}
		}
	}
	c.push_back(0);
	for (uint j = 0; j < n; j++)
	{
		c.push_back(max);
		p.emplace_back();
		for (uint i = 0; i < j; i++)
		{
			if (c[i] + lc[i][j] < c[j+1])
			{
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