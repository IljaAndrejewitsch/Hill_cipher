//
// ������ � ������������� (����� � �������)
//

#include <iostream>
#include "hillcipher.h"
#include "hillcipher_test.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	test_full(); // �����

	string sstr;

	string key;
	string Result_str;
	char YorN;
	bool dontTry = false; // �� ����� ���������� ������������ ���-�� ��� �� ������, ���� �� ��� ���������

	bool start = true; // ��������� �� ��, ��� ������������ ��� �� ���������������� � ����������
	int Jump = 1;

	while (Jump != 0) {

		if (start == false) {
			cout << "\n\n--------------" << endl;
			cout << "1 - ���������;" << endl;
			cout << "\n0 - ���������." << endl;
			cout << "--------------" << endl;
			
			cout << "\n������� ����� ��������� ��������: ";
			cin >> Jump;
			
			while ((Jump != 1) && (Jump != 0)) {
				cout << "\n������� ����� �������� 1 ���� 0: ";
				cin >> Jump;
			}
		}

		if (Jump != 0) {
			cout << "\n������� ����� ��� ���������� (��������� ��������� �����, ������, ����� � ���� �������):\n" << endl;

			if (start == false)
				cin.ignore(1);
			getline(cin, sstr);

			cout << "\n������� ������-����:" << endl;
			cout << "(��������� ��������� �����, ������, ����� � ���� �������," << endl;
			cout << "���������� ���� ������ ���� ��������� ������ ����� (�.�. ������ abcd ��������, �.�. � ��� 4 �����, 2^2 = 4))\n" << endl;
			getline(cin, key);
			while (abs(sqrt(key.length()) - uint(sqrt(key.length()))) >= precision) {
				cout << "\n������� ������-���� �����, ����� ���������� ���� � ��� ���� ��������� ������ �����:" << endl;
				getline(cin, key);
			}

			try {
				Result_str = crypt_text(sstr, key, 1);
				cout << "\n�������� �����:\n" << sstr << endl;
				cout << "\n������������� �����:\n" << Result_str << endl;
			}
			catch (...) {
				cout << "\n���� ���������." << endl;
				dontTry = true;
			}

			cout << "\n\n������� ����� ��� ����������� (��������� ��������� �����, ������, ����� � ���� �������):\n" << endl;
			getline(cin, sstr);

			if (dontTry == false) {
				cout << "\n��� ����������� ����� ������ ������������ ������ ��� ��������� ������-����? (y/n): ";
				cin >> YorN;
			}
			else
				YorN == 'n';

			if (YorN == 'n') {
				cout << "\n������� ����� ������-����:" << endl;
				cout << "(��������� ��������� �����, ������, ����� � ���� �������," << endl;
				cout << "���������� ���� ������ ���� ��������� ������ �����)\n" << endl;
				cin.ignore(1);
				getline(cin, key);
				while (abs(sqrt(key.length()) - uint(sqrt(key.length()))) >= precision) {
					cout << "\n������� ������-���� �����, ����� ���������� ���� � ��� ���� ��������� ������ �����:" << endl;
					getline(cin, key);
				}
			}

			try {
				Result_str = crypt_text(sstr, key, 0);
				cout << "\n������������� �����:\n" << sstr << endl;
				cout << "\n�������������� �����:\n" << Result_str << endl;
			}
			catch (...) {
				cout << "\n���� ���������." << endl;
			}

			if (start == true)
				start = false;

			if (dontTry == true)
				dontTry = false;
		}
	}

	cout << "\n�� ����� ������!";

	cout << "\n\n";
	system("pause");

	return 0;
}