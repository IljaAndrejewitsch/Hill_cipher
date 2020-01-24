//
// Работа с пользователем (вывод в консоль)
//

#include <iostream>
#include "hillcipher.h"
#include "hillcipher_test.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	test_full(); // тесты

	string sstr;

	string key;
	string Result_str;
	char YorN;
	bool dontTry = false; // не стоит предлагать расшивровать что-то тем же ключом, если он уже необратим

	bool start = true; // указывает на то, что пользователь еще не взаимодействовал с программой
	int Jump = 1;

	while (Jump != 0) {

		if (start == false) {
			cout << "\n\n--------------" << endl;
			cout << "1 - Повторить;" << endl;
			cout << "\n0 - Закончить." << endl;
			cout << "--------------" << endl;
			
			cout << "\nВведите номер желаемого действия: ";
			cin >> Jump;
			
			while ((Jump != 1) && (Jump != 0)) {
				cout << "\nВведите номер действия 1 либо 0: ";
				cin >> Jump;
			}
		}

		if (Jump != 0) {
			cout << "\nВведите текст для зашифровки (допустимы латинские буквы, пробел, точка и знак вопроса):\n" << endl;

			if (start == false)
				cin.ignore(1);
			getline(cin, sstr);

			cout << "\nВведите строку-ключ:" << endl;
			cout << "(допустимы латинские буквы, пробел, точка и знак вопроса," << endl;
			cout << "количество букв должно быть квадратом целого числа (т.е. строка abcd подходит, т.к. в ней 4 буквы, 2^2 = 4))\n" << endl;
			getline(cin, key);
			while (abs(sqrt(key.length()) - uint(sqrt(key.length()))) >= precision) {
				cout << "\nВведите строку-ключ такую, чтобы количество букв в ней было квадратом целого числа:" << endl;
				getline(cin, key);
			}

			try {
				Result_str = crypt_text(sstr, key, 1);
				cout << "\nИсходный текст:\n" << sstr << endl;
				cout << "\nЗашифрованный текст:\n" << Result_str << endl;
			}
			catch (...) {
				cout << "\nКлюч необратим." << endl;
				dontTry = true;
			}

			cout << "\n\nВведите текст для расшифровки (допустимы латинские буквы, пробел, точка и знак вопроса):\n" << endl;
			getline(cin, sstr);

			if (dontTry == false) {
				cout << "\nДля расшифровки этого текста использовать только что введенную строку-ключ? (y/n): ";
				cin >> YorN;
			}
			else
				YorN == 'n';

			if (YorN == 'n') {
				cout << "\nВведите новую строку-ключ:" << endl;
				cout << "(допустимы латинские буквы, пробел, точка и знак вопроса," << endl;
				cout << "количество букв должно быть квадратом целого числа)\n" << endl;
				cin.ignore(1);
				getline(cin, key);
				while (abs(sqrt(key.length()) - uint(sqrt(key.length()))) >= precision) {
					cout << "\nВведите строку-ключ такую, чтобы количество букв в ней было квадратом целого числа:" << endl;
					getline(cin, key);
				}
			}

			try {
				Result_str = crypt_text(sstr, key, 0);
				cout << "\nЗашифрованный текст:\n" << sstr << endl;
				cout << "\nРасшифрованный текст:\n" << Result_str << endl;
			}
			catch (...) {
				cout << "\nКлюч необратим." << endl;
			}

			if (start == true)
				start = false;

			if (dontTry == true)
				dontTry = false;
		}
	}

	cout << "\nДо новых встреч!";

	cout << "\n\n";
	system("pause");

	return 0;
}