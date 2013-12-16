#include <vector>
#include <numeric>
#include <ostream>
#include <string>
using namespace std;

typedef size_t uint;

vector<uint> ComputeBreaks(const vector<string> &words, uint n, uint m);
void Output(const vector<string> &words, ostream &stream, vector<uint> p, uint j);

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
	return 0;
}

vector<uint> ComputeBreaks(const vector<string> &words, uint n, uint m)
{
	vector<vector<uint>> extras, lc;
	vector<uint> p, c;
	const uint max = numeric_limits<uint>::max();
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
			if (c[i - 1 + lc[i][j]] < c[j])
			{
				c[j] = c[i - 1] + lc[i][j];
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
		stream << words[l];
	stream << endl;
}