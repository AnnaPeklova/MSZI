#include <iostream>
#include <fstream>

using namespace std;

void Peklova_to_encrypt(string key, string output_file, string input_file)
{
	char s;
	ofstream output;
	ifstream input;
	output.open(output_file + ".txt", ios::binary);
	input.open(input_file + ".txt", ios::binary);
	int number = 0;
	while (true)
	{
		input.get(s);
		if (input.eof())
		{
			input.close();
			output.close();
			break;
		}
		output << (unsigned char)(s + key[number % key.length()]);
		number++;
	}
}

void Peklova_to_decrypt(string key, string output_file, string input_file)
{
	char s;
	ofstream output;
	ifstream input;
	output.open(output_file + ".txt", ios::binary);
	input.open(input_file + ".txt", ios::binary);
	int number = 0;
	while (true)
	{
		input.get(s);
		if (input.eof())
		{
			input.close();
			output.close();
			break;
		}
		output << (unsigned char)(s - key[number % key.length()]);
		number++;
	}
}

void Peklova_file(string& key,string& input_file, string& output_file)
{
	cout << "Введите названиe входного файла: ";
	cin >> input_file;
	cout << "Введите название выходного файла: ";
	cin >> output_file;
	cout << "Введите пароль: ";
	cin >> key;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	string key, input_file, output_file;
	cout << "Введите 1 - зашифровать файл; 2 - расшифровать файл" << endl;
	int enter;
	cin >> enter;
	switch (enter)
	{
	case 1:
	{
		Peklova_file(key, input_file, output_file);
		Peklova_to_encrypt(key, output_file, input_file);
		break;
	}
	case 2:
	{
		Peklova_file(key, input_file, output_file);
		Peklova_to_decrypt(key, output_file, input_file);
		break;
	}
	}
}

