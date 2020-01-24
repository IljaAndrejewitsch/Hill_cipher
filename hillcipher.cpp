//
// Реализация функций
//

#include <iostream>
#include <algorithm>
#include "hillcipher.h"

using namespace std;

string crypt_text(const string str, const string key, bool mode)
{
	string workable_str = str;
	string workable_key = key;
	transform(workable_str.begin(), workable_str.end(), workable_str.begin(), tolower); // понижение регистра строки
	transform(workable_key.begin(), workable_key.end(), workable_key.begin(), tolower);
	
	uint sizeMatrix = uint(sqrt(workable_key.length()));
	int** Word_code = new int*[sizeMatrix];
	for (uint rows = 0; rows < sizeMatrix; rows++) { // создание матрицы из строки-ключа
		Word_code[rows] = new int[sizeMatrix];
		for (uint cols = 0; cols < sizeMatrix; cols++) {
			if (workable_key[rows * sizeMatrix + cols] == ' ')
				Word_code[rows][cols] = 26;
			else if (workable_key[rows * sizeMatrix + cols] == '.')
				Word_code[rows][cols] = 27;
			else if (workable_key[rows * sizeMatrix + cols] == '?')
				Word_code[rows][cols] = 28;
			else
				Word_code[rows][cols] = workable_key[rows * sizeMatrix + cols] - 97; // вычет согласно таблице ASCII
		}
	}

	int detKey = determinant(Word_code, sizeMatrix); // вычисление определителя
	if (detKey && (detKey % 29 != 0)) { // ключ необратим, если его определитель равен 0 или он не взаимно прост с длиной алфавита
		string Result = "";
		string Block = "";
		int** Akey = nullptr; // для хранения обратной по модулю матрицы

		if (!mode) { // если необходимо дешифровать
			int x;
			int y;
			gcd(abs(detKey), 29, x, y); // в расширенном алгоритме Евклида нужен только x, поэтому сам НОД не запоминаем
			if (detKey < 0) // исправление погрешности в случае поиска НОД отрицательных чисел
				x = -x;

			int invertDet = (x % 29 + 29) % 29; // вывод обратного по модулю элемента определителя ключа

			Akey = AMatrix(Word_code, sizeMatrix);

			for (uint rows = 0; rows < sizeMatrix; rows++)
				for (uint cols = 0; cols < sizeMatrix; cols++)
					Akey[rows][cols] = ((Akey[rows][cols] % 29) * invertDet) % 29; // умножение по модулю матрицы на обратный элемент

			Akey = transMatrix(Akey, sizeMatrix);

			for (uint rows = 0; rows < sizeMatrix; rows++)
				for (uint cols = 0; cols < sizeMatrix; cols++)
					if (Akey[rows][cols] < 0)
						Akey[rows][cols] += 29; // устранение отрицательных чисел в обратной матрице
		}

		for (uint i = 0; i < workable_str.length() / sizeMatrix; i++) { // деление строки на блоки
			for (uint j = 0; j < sizeMatrix; j++)
				Block += workable_str[i * sizeMatrix + j];
			if (mode)
				Result += crypt_word(Block, Word_code);
			else
				Result += crypt_word(Block, Akey);
			Block = "";
		}

		if (workable_str.length() % sizeMatrix != 0) { // если текст нельзя поделить на равные блоки
			for (uint i = 0; i < workable_str.length() - workable_str.length() / sizeMatrix * sizeMatrix; i++)
				Block += workable_str[workable_str.length() / sizeMatrix * sizeMatrix + i];
			for (uint i = 0; i < sizeMatrix - (workable_str.length() - workable_str.length() / sizeMatrix * sizeMatrix); i++)
				Block += ' ';
			if (mode)
				Result += crypt_word(Block, Word_code);
			else
				Result += crypt_word(Block, Akey);
		}

		if (!mode)
			delete_matrix(Akey, sizeMatrix);
		delete_matrix(Word_code, sizeMatrix);
		return Result;
	}
	else
		throw -1;
}

string crypt_word(const string str, int const* const* key)
{
	uint sizeMatrix = str.length();
	int* Word_old = new int[sizeMatrix];
	for (uint i = 0; i < sizeMatrix; i++) { // перевод строки в код
		if (str[i] == ' ')
			Word_old[i] = 26;
		else if (str[i] == '.')
			Word_old[i] = 27;
		else if (str[i] == '?')
			Word_old[i] = 28;
		else
			Word_old[i] = str[i] - 97;
	}

	int* Word_new = new int[sizeMatrix]; // умножение строки на ключ и получение кода нового слова
	for (uint rows = 0; rows < sizeMatrix; rows++) {
		Word_new[rows] = 0;
		for (uint cols = 0; cols < sizeMatrix; cols++)
			Word_new[rows] += key[rows][cols] * Word_old[cols];
	}

	string Result = "";
	for (uint i = 0; i < sizeMatrix; i++) { // раскодирование нового слова
		if (Word_new[i] % 29 == 26)
			Result += ' ';
		else if (Word_new[i] % 29 == 27)
			Result += '.';
		else if (Word_new[i] % 29 == 28)
			Result += '?';
		else
			Result += (char)((Word_new[i] % 29) + 97);
	}

	delete[] Word_old;
	delete[] Word_new;
	Word_old = nullptr;
	Word_new = nullptr;

	return Result;
}

void delete_matrix(int**& m, const uint size)
{
	for (uint i = 0; i < size; i++)
		delete[] m[i];

	delete[] m;
	m = nullptr;
}

void getMinor(int const* const* m, int**& Minor, uint i, uint j, uint sizeMatrix)
{
	uint di = 0;
	uint dj = 0;
	for (uint ki = 0; ki < sizeMatrix - 1; ki++) {
		if (ki == i) // для пропуска строки i
			di = 1;
		dj = 0;
		for (uint kj = 0; kj < sizeMatrix - 1; kj++) {
			if (kj == j) // для пропуска столбца j
				dj = 1;
			Minor[ki][kj] = m[ki + di][kj + dj];
		}
	}
}

int determinant(int const* const* m, uint sizeMatrix)
{
	int** Minor = new int* [sizeMatrix - 1];

	for (uint i = 0; i < sizeMatrix - 1; i++)
		Minor[i] = new int[sizeMatrix - 1];

	int Det = 0;
	int k = 1; // (-1) в степени i (смена знака при прохождении строк для разложения по столбцу)

	if (sizeMatrix == 1) {
		Det = m[0][0];

		return(Det);
	}
	else if (sizeMatrix == 2) {
		Det = m[0][0] * m[1][1] - m[1][0] * m[0][1];

		delete_matrix(Minor, sizeMatrix - 1);
		return(Det);
	}
	else {
		for (uint i = 0; i < sizeMatrix; i++) { // разложение определителя по первому столбцу
			getMinor(m, Minor, i, 0, sizeMatrix);
			Det += k * m[i][0] * determinant(Minor, sizeMatrix - 1);
			k = -k;
		}
	}

	delete_matrix(Minor, sizeMatrix - 1);
	return Det;
}

int gcd(int a, int b, int& x, int& y)
{
	if (a == 0) {
		x = 0; y = 1;
		return b;
	}

	int x1;
	int y1;
	int d = gcd(b % a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;

	return abs(d);
}

int** AMatrix(int const* const* m, uint sizeMatrix)
{
	if (sizeMatrix > 1) {
		int** Minor = new int* [sizeMatrix - 1];
		for (uint i = 0; i < sizeMatrix - 1; i++)
			Minor[i] = new int[sizeMatrix - 1];

		int minus; // (-1) в степени rows + cols
		int** addit = new int* [sizeMatrix];
		for (uint rows = 0; rows < sizeMatrix; rows++) {
			addit[rows] = new int[sizeMatrix];
			for (uint cols = 0; cols < sizeMatrix; cols++) {
				if ((rows + cols) % 2)
					minus = -1;
				else
					minus = 1;
				getMinor(m, Minor, rows, cols, sizeMatrix);
				addit[rows][cols] = minus * determinant(Minor, sizeMatrix - 1);
			}
		}

		delete_matrix(Minor, sizeMatrix - 1);
		return addit;
	}

	int** addit = new int* [sizeMatrix];
	addit[0] = new int[sizeMatrix] { 1 }; // если матрица первого порядка, то возвращаем единичную матрицу
	return addit;
}

int** transMatrix(int const* const* m, uint sizeMatrix)
{
	int** transposed = new int* [sizeMatrix];
	for (uint rows = 0; rows < sizeMatrix; rows++) {
		transposed[rows] = new int[sizeMatrix];
		for (uint cols = 0; cols < sizeMatrix; cols++)
			transposed[rows][cols] = m[cols][rows];
	}

	return transposed;
}