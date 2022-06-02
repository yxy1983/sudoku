#include <iostream>
#include <vector>
using namespace std;

//const unsigned int CHIGH = 0xFFFF0000;
const unsigned int CLOW = 0x0000FFFF;
const unsigned int CBASE = 511; // (0000 0001 1111 1111)

typedef unsigned int BrickArray[9][9];

unsigned int Unite(unsigned int high, unsigned int low)
{
	unsigned int ret = high << 16;
	ret &= 0xFFFF0000;
	ret |= low;
	return ret;
}

void Split(unsigned int src, unsigned int& high, unsigned int& low)
{
	low = src & CLOW;
	high = (src >> 16) & CLOW;
}

void PrintArray(BrickArray& arr)
{
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			unsigned int ht = 0;
			unsigned int lt = 0;
			Split(arr[i][j], ht, lt);
			std::cout << lt << " ";
		}
		std:cout << std::endl;
	}
}

bool CheckOne(BrickArray& arr, const int lv,  const int i, const int j)
{
	int bi = i / 3 * 3;
	int bj = j / 3 * 3;

	for (int index = 0; index < 9; ++index)
	{
		int si = index / 3 + bi;
		int sj = index % 3 + bj;
		unsigned int ht = 0;
		unsigned int lt = 0;
		if (i != si || j != sj)
		{
			unsigned temp = arr[si][sj];
			Split(temp, ht, lt);
			if (ht != lt && lt == lv)
				return false;
		}

		if (index != j)
		{
			unsigned int temp = arr[i][index];
			ht = 0;
			lt = 0;
			Split(temp, ht, lt);
			if (ht != lt && lt == lv)
				return false;
		}
		
		if (index != i)
		{
			unsigned int temp = arr[index][j];
			ht = 0;
			lt = 0;
			Split(temp, ht, lt);
			if (ht != lt && lt == lv)
				return false;
		}

	}
	return true;
}

bool Try(BrickArray& arr, int index)
{
	if (index == 81)
		return true;

	int i = index / 9;
	int j = index % 9;
	unsigned int temp = arr[i][j];
	unsigned int hv = 0;
	unsigned int lv = 0;
	Split(temp, hv, lv);
	if (hv != CLOW)
	{
		for (int s = 0; s < 9; ++s)
		{
			unsigned int v = (hv >> s) & 1;
			if (0 != v)
			{
				if (CheckOne(arr, s + 1, i, j))
				{
					arr[i][j] = Unite(hv, s + 1);
					if (Try(arr, index + 1))
						return true;

				}

			}

		}
		arr[i][j] = Unite(hv, lv);
		return false;
	}
	else
	{
		return Try(arr, index + 1);
	}
}

void solveSudoku(vector<vector<char>>& board)
{
	BrickArray bricks;
	for (int i = 0; i < 9; ++i) 
	{
		for (int j = 0; j < 9; ++j)
		{
			bricks[i][j] = 0;
			unsigned int hv = 0;
			unsigned int lv = 0;
			char c = board[i][j];
			if ('.' == c)
			{
				hv = 0;
				lv = CBASE;
			}
			else
			{
				hv = CLOW;
				lv = unsigned int(c - 48);
			}
			bricks[i][j] = Unite(hv, lv);
		}
	}

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			unsigned int hv = 0;
			unsigned int lv = 0;
			Split(bricks[i][j], hv, lv);

			if (hv == CLOW)
				continue;

			unsigned int ht = 0;
			unsigned int lt = 0;

			int bi = i / 3 * 3;
			int bj = j / 3 * 3;

			for (int k = 0; k < 9; ++k) 
			{
				// ÐÐ
				if (k != j)
				{
					ht = 0;
					lt = 0;
					Split(bricks[i][k], ht, lt);
					if (ht == CLOW)
						lv &= ~(1 << (lt - 1));
				}
				// ÁÐ
				if (k != i)
				{
					ht = 0;
					lt = 0;
					Split(bricks[k][j], ht, lt);
					if (ht == CLOW)
						lv &= ~(1 << (lt - 1));
				}
				// ¾Å¹¬¸ñ
				int si = k / 3 + bi;
				int sj = k % 3 + bj;
				if (si != i || sj != j)
				{
					ht = 0;
					lt = 0;
					Split(bricks[si][sj], ht, lt);
					if (ht == CLOW)
						lv &= ~(1 << (lt - 1));
				}				
			}

			bricks[i][j] = Unite(lv, lv);
		}
	}

	Try(bricks, 0);
	PrintArray(bricks);
}


int main()
{
	std::string exp[9][9] = {
		{".", ".", ".", "5", ".", ".", ".", ".", "."},
		{".", ".", "9", ".", ".", ".", ".", ".", "6"},
		{"8", "7", ".", ".", ".", "6", "4", ".", "9"},
		{".", ".", "6", "7", ".", ".", ".", ".", "3"},
		{".", "1", "3", ".", ".", ".", "5", "8", "."},
		{"4", ".", ".", ".", ".", "8", "7", ".", "."},
		{"6", ".", "2", "8", ".", ".", ".", "9", "5"},
		{"1", ".", ".", ".", "4", ".", "6", ".", "."},
		{".", ".", ".", ".", ".", "3", ".", ".", "."},
	};

	vector<vector<char>> board;
	for (int i = 0; i < 9; ++i)
	{
		vector<char> vc;
		for (int j = 0; j < 9; ++j)
		{
			std::string& str = exp[i][j];
			vc.push_back(str[0]);
		}
		board.push_back(vc);
	}

	solveSudoku(board);

	return 0;
}



