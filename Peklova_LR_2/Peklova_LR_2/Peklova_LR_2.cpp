#include <iostream>
#include "windows.h"
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

void decryption(string password, string input_file, string output_file)
{
    char sym;
    ofstream output;
    ifstream input;
    output.open(output_file + ".txt", ios::out);
    input.open(input_file + ".txt", ios::in);
    int number = 0;
    if (input.is_open() && output.is_open())
    {
        while (true)
        {
            input.get(sym);
            if (input.eof())
            {
                input.close();
                output.close();
                break;
            }
            output << (unsigned char)(sym - password[number % password.length()]);
            number++;
        }
    }
    else
        cout << "Один из файлов не открылся!" << endl;
}

vector<char> reading()
{
    cout << "Введите название файла с эталонным текстом: ";
    string in_file;
    cin >> in_file;
    ifstream in(in_file + ".txt", ios::binary);
    if (!in.is_open()) cout << endl << "Файл не открылся!" << endl;
    vector<char>v((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    return v;
}

vector<char> get_alphabet_of_text(vector<char>v)
{
    vector<char>vv = v;
    vector<char>::iterator k;
    sort(vv.begin(), vv.end());
    k = unique(vv.begin(), vv.begin() + vv.size());
    vv.resize(distance(vv.begin(), k));
    return vv;
}

int compl_index(vector<char>v)
{
    vector<char> alphabet_of_encoded;
    alphabet_of_encoded = get_alphabet_of_text(v);
    cout << endl << "Введите максимальную длину ключа: ";
    int max_key_length;
    cin >> max_key_length;
    cout << endl;
    vector<vector<int>> freq_of_chars;
    freq_of_chars.assign(max_key_length, vector<int>(alphabet_of_encoded.size()));
    vector<int> group_length(max_key_length);
    for (unsigned int key_length = 1; key_length <= max_key_length; key_length++)
    {
        for (unsigned int i = 0; i < alphabet_of_encoded.size(); i++)
        {
            freq_of_chars[key_length - 1][i] = 0;
            group_length[key_length - 1] = 0;
            for (unsigned int j = 0; j < v.size(); j++)
            {
                if (j % key_length == 0) group_length[key_length - 1]++;
                if ((j % key_length == 0) & (v[j] == alphabet_of_encoded[i]))
                {
                    freq_of_chars[key_length - 1][i]++;
                }
            }
        }
    }
    vector<double> sum(max_key_length);
    vector<double> index(max_key_length);
    for (unsigned int i = 1; i <= max_key_length; i++)
    {
        sum[i - 1] = 0;
        {
            for (unsigned int p = 0; p <= alphabet_of_encoded.size() - 1; p++)
            {
                sum[i - 1] = sum[i - 1] + ((double)freq_of_chars[i - 1][p] * ((double)freq_of_chars[i - 1][p] - 1.0));
            }
        }
        index[i - 1] = 0;
        index[i - 1] = sum[i - 1] / ((double)group_length[i - 1] * ((double)group_length[i - 1] - 1.0));
    }
    int key_length = 0;
    cout << "Полученные индексы соответствия: " << endl;
    for (int i = 0; i <= max_key_length - 1; i++) cout << i + 1 << "." << index[i] << endl;
    cout << "Определите длину ключа по индексам соответствия (первый наиболее отличающийся от остальных): ";
    cin >> key_length;
    return key_length;
}
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    while (1)
    {
        cout << "Введите название файла с закодированным текстом: ";
        string encoded_file;
        cin >> encoded_file;
        ifstream encoded(encoded_file + ".txt", ios::binary);
        if (!encoded.is_open()) cout << endl << "Файл не открылся!" << endl;
        vector<char>encoded_text((istreambuf_iterator<char>(encoded)), istreambuf_iterator<char>());

        vector<char> alphabet_of_encoded;
        alphabet_of_encoded = get_alphabet_of_text(encoded_text);


        vector<char>etalon_text;
        etalon_text = reading();
        int key_length = compl_index(encoded_text);
        cout << "Длина ключа закодированного текста: " << key_length << endl;
        vector<char> alphabet_of_etalon;
        alphabet_of_etalon = get_alphabet_of_text(etalon_text);
        map<char, int>unique_map;
        vector<unsigned int> freq_of_etalon(alphabet_of_etalon.size());
        {
            for (unsigned int i = 0; i < alphabet_of_etalon.size(); i++)
            {
                freq_of_etalon[i] = 0;
                for (unsigned int j = 0; j < etalon_text.size(); j++)
                {
                    if (etalon_text[j] == alphabet_of_etalon[i]) freq_of_etalon[i]++;
                }
                unique_map.emplace(alphabet_of_etalon[i], freq_of_etalon[i]);
            }
        }

        vector<unsigned int> freq_of_etalon_sort(alphabet_of_etalon.size());
        freq_of_etalon_sort = freq_of_etalon;
        sort(freq_of_etalon_sort.begin(), freq_of_etalon_sort.end(), greater<int>());
        auto iter = find(freq_of_etalon.begin(), freq_of_etalon.end(), freq_of_etalon_sort[0]);
        int num_max_sym_etalon = iter - freq_of_etalon.begin();
        unsigned char char_max_sym_etalon = alphabet_of_etalon[num_max_sym_etalon];
        cout << "Cамый встречающийся символ в эталонном тексте: " << (int)char_max_sym_etalon << ' ' << "' '" << endl;

        string key;
        vector<vector<unsigned int>>freq_in_groups, freq_in_groups_sort;
        freq_in_groups.assign(key_length, vector<unsigned int>(alphabet_of_encoded.size()));
        freq_in_groups_sort.assign(key_length, vector<unsigned int>(alphabet_of_encoded.size()));
        for (unsigned int key_len = 1; key_len <= key_length; key_len++)
        {
            for (unsigned int i = 0; i < alphabet_of_encoded.size(); i++)
            {
                freq_in_groups[key_len - 1][i] = 0;
                for (unsigned int j = 0; j < encoded_text.size() - key_length; j = j + key_length)
                {
                    if (j + key_len - 1 <= encoded_text.size() - 1)
                    {
                        if (encoded_text[j + key_len - 1] == alphabet_of_encoded[i]) freq_in_groups[key_len - 1][i]++;
                    }
                }
            }
        }

        freq_in_groups_sort = freq_in_groups;
        for (int key_len = 0; key_len <= key_length - 1; key_len++)
        {
            sort(freq_in_groups_sort[key_len].begin(), freq_in_groups_sort[key_len].end(), greater<int>());
            auto max_occur = find(freq_in_groups[key_len].begin(), freq_in_groups[key_len].end(), freq_in_groups_sort[key_len][0]);
            unsigned int num_occur = max_occur - freq_in_groups[key_len].begin();
            unsigned char max_sym = alphabet_of_encoded[num_occur];
            key.push_back((unsigned char)((int)max_sym - (int)char_max_sym_etalon));
        }
        cout << "Полученный ключ: ";
        cout << key << endl;

        string input_file, output_file;
        cout << "\nВведите название файла, в который вывести раскодированный текст: ";
        string decrypted;
        cin >> decrypted;
        decryption(key, encoded_file, decrypted);
        cout << "Хотите попробовать с другими текстами? y/n" << " ";
        string answer;
        cin >> answer;
        if (answer == "n")
            break;
    }
}