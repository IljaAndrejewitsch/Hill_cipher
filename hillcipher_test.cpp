//
// Тестирование реализованных функций
//

#include <cassert>
#pragma warning(disable:4996)
#include <cstring>
#include <iostream>
#include "hillcipher.h"
#include "hillcipher_test.h"

using  std::cerr;
using  std::endl;

bool test_crypt_text()
{
	cerr << "test crypt_text: ";

	// --------------
	string key = "e";

	string Text = "polygraphic substitution cipher based on linear algebra";
	string Result = "c.pjykac?diroweosdswsd.xridc?qkreaoqmr.xrpdxqakrapyqeka";
	string realResult = crypt_text(Text, key, 1);
	assert(realResult == Result);

	swap(Text, Result);
	realResult = crypt_text(Text, key, 0);
	assert(realResult == Result);

	// -------------------------------------------------------
	key = "Do not trouble trouble until trouble troubles you";

	try {
		crypt_text(Text, key, 1);
		assert(false);
	}
	catch (...) {
		assert(true);
	}

	// -------------------------------
	key = "Custom is a second nature";

	try {
		crypt_text(Text, key, 1);
		assert(false);
	}
	catch (...) {
		assert(true);
	}

	// ---------------
	key = "gybnqkurp";

	Text = "algebra"; // здесь шифр добавляет два незначащих пробела в конце слова из-за невозможности разбить текст на равные блоки
	Result = "jeqhgemju";
	realResult = crypt_text(Text, key, 1);
	assert(realResult == Result);

	swap(Text, Result);
	Result += "  "; // шифр вернет исходное слово с пробелами, которые добавил
	realResult = crypt_text(Text, key, 0);
	assert(realResult == Result);

	// -------------
	Text = "cipher";
	Result = "qoykg?";
	realResult = crypt_text(Text, key, 1);
	assert(realResult == Result);

	swap(Text, Result);
	realResult = crypt_text(Text, key, 0);
	assert(realResult == Result);

	// ------------------------------------------
	key = "Speech is silver but silence is gold";

	Text = "a bad beginning makes a bad ending";
	Result = "cyqsrktq fur.hizu?svjtfwbecqdtutnvgf";
	realResult = crypt_text(Text, key, 1);
	assert(realResult == Result);

	swap(Text, Result);
	Result = Result + "  ";
	realResult = crypt_text(Text, key, 0);
	assert(realResult == Result);

	cerr << "OK" << endl;
	return true;
}

bool test_full() {
	return test_crypt_text();
}