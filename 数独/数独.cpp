// 数独.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include"ConsoleColor.h"
using namespace std;

#include "stdio.h"


int InBlock[81], InRow[81], InCol[81];

const int BLANK = 0;
const int ONES = 0x3fe;   // Binary 1111111110

int Entry[81];  // Records entries 1-9 in the grid, as the corresponding bit set to 1
int Block[9], Row[9], Col[9]; // Each int is a 9-bit array

int SeqPtr = 0;
int Sequence[81];

int Count = 0;
int LevelCount[81];


void SwapSeqEntries(int S1, int S2)
{
	int temp = Sequence[S2];
	Sequence[S2] = Sequence[S1];
	Sequence[S1] = temp;
}


void InitEntry(int i, int j, int val)
{
	int Square = 9 * i + j;
	int valbit = 1 << val;
	int SeqPtr2;

	// add suitable checks for data consistency

	Entry[Square] = valbit;
	Block[InBlock[Square]] &= ~valbit;
	Col[InCol[Square]] &= ~valbit; // Simpler Col[j] &= ~valbit;
	Row[InRow[Square]] &= ~valbit; // Simpler Row[i] &= ~valbit;

	SeqPtr2 = SeqPtr;
	while (SeqPtr2 < 81 && Sequence[SeqPtr2] != Square)
		SeqPtr2++;

	SwapSeqEntries(SeqPtr, SeqPtr2);
	SeqPtr++;
}


void PrintArray()
{
	int i, j, valbit, val, Square;
	char ch;

	Square = 0;

	for (i = 0; i < 9; i++) {
		if (i % 3 == 0) putc('\n', stdout);
		for (j = 0; j < 9; j++) {
			if (j % 3 == 0) putc(' ', stdout);
			valbit = Entry[Square++];
			if (valbit == 0) ch = '-';
			else {
				for (val = 1; val <= 9; val++)
					if (valbit == (1 << val)) {
						ch = '0' + val;
						break;
					}
			}
			putc(ch, stdout);
		}
		putc('\n', stdout);
	}
}


void ConsoleInput()
{
	int i, j;
	char InputString[80];

	for (i = 0; i < 9; i++) {
		printf("Row[%d] : ", i + 1);
		scanf_s("%s", InputString);

		for (j = 0; j < 9; j++) {
			char ch = InputString[j];
			if (ch >= '1' && ch <= '9')
				InitEntry(i, j, ch - '0');
		}
	}

	PrintArray();
}


void PrintStats()
{
	int i, j, S;

	printf("\nLevel Counts:\n\n");

	S = 0;
	while (LevelCount[S] == 0) S++;

	i = 0;

	while (S < 81) {
		int Seq = Sequence[S];
		printf("(%d, %d):%4d ", Seq / 9 + 1, Seq % 9 + 1, LevelCount[S]);
		if (i++ > 4) {
			printf("\n");
			i = 0;
		}
		S++;
	}

	printf("\n\nCount = %d\n", Count);
}


void Succeed()
{
	PrintArray();
	PrintStats();
}


int NextSeq(int S)
{
	int S2, Square, Possibles, BitCount;
	int T, MinBitCount = 100;

	for (T = S; T < 81; T++) {
		Square = Sequence[T];
		Possibles = Block[InBlock[Square]] & Row[InRow[Square]] & Col[InCol[Square]];
		BitCount = 0;
		while (Possibles) {
			Possibles &= ~(Possibles & -Possibles);
			BitCount++;
		}

		if (BitCount < MinBitCount) {
			MinBitCount = BitCount;
			S2 = T;
		}
	}

	return S2;
}


void Place(int S)
{
	LevelCount[S]++;
	Count++;

	if (S >= 81) {
		Succeed();
		return;
	}

	int S2 = NextSeq(S);
	SwapSeqEntries(S, S2);

	int Square = Sequence[S];

	int   BlockIndex = InBlock[Square],
		RowIndex = InRow[Square],
		ColIndex = InCol[Square];

	int   Possibles = Block[BlockIndex] & Row[RowIndex] & Col[ColIndex];
	while (Possibles) {
		int valbit = Possibles & (-Possibles); // Lowest 1 bit in Possibles
		Possibles &= ~valbit;
		Entry[Square] = valbit;
		Block[BlockIndex] &= ~valbit;
		Row[RowIndex] &= ~valbit;
		Col[ColIndex] &= ~valbit;

		Place(S + 1);

		Entry[Square] = BLANK; // Could be moved out of the loop
		Block[BlockIndex] |= valbit;
		Row[RowIndex] |= valbit;
		Col[ColIndex] |= valbit;
	}

	SwapSeqEntries(S, S2);
}


int main(int argc, char* argv[])
{
	int i, j, Square;

	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++) {
			Square = 9 * i + j;
			InRow[Square] = i;
			InCol[Square] = j;
			InBlock[Square] = (i / 3) * 3 + (j / 3);
		}


	for (Square = 0; Square < 81; Square++) {
		Sequence[Square] = Square;
		Entry[Square] = BLANK;
		LevelCount[Square] = 0;
	}

	for (i = 0; i < 9; i++)
		Block[i] = Row[i] = Col[i] = ONES;

	ConsoleInput();
	Place(SeqPtr);
	printf("\n\nTotal Count = %d\n", Count);

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
