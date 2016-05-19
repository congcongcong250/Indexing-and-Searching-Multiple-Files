/*
 * a3search.cpp
 *
 *  Created on: May 9, 2016
 *      Author: lirenxn
 */

#include<iostream>
#include<fstream>
#include<cassert>
#include<string>
#include<map>
#include<vector>
#include<dirent.h>

using namespace std;

struct Node
{
	string pattern;
	Node* next;
};

int shift[729];
Node *hashtb[729];
int m = 257, M, B = 2;

int hashcpt(char first, char second);

vector<string> find_files(char* folder);

int Wu_manber(ifstream& file, string* pat, int npats);

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		cout << "We need at least 3 argument" << endl;
		return 1;
	}

	// string folder for the folder
	// string idxfile is the name of the idxfile
	// string idxsize is the percentage of the idxfile size
	// string o is the third argument
	// pbegin is the begining position of pattern in argument array

	string folder(argv[1]);
	string idxfile(argv[2]);
	short idxsize = 20;
	string o(argv[3]);
	short pbegin = 3;
	if (o == "-s")
	{
		string s(argv[4]);
		//This is not a bug, c++11 func
		idxsize = (short) stoi(s);
		assert(argc > 5);
		pbegin = 5;
	}

	int npats = argc - pbegin;
	string pat[npats];

	//Get all patterns
	for (int i = pbegin; i < argc; ++i)
	{
		// Convert to lower string
		int j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] <= 'Z' && argv[i][j] >= 'A')
				argv[i][j] = argv[i][j] - ('Z' - 'z');
			j++;
		}

		pat[i - pbegin] = argv[i];
		if (pat[i - pbegin].size() < m)
			m = pat[i - pbegin].size();
		//cout << pat[i-pbegin].size()<<endl;
	}

	// Preprocessing patterns
	fill_n(shift, 729, -1);
	fill_n(hashtb, 729, nullptr);
	for (int i = 0; i < npats; ++i)
	{
		cout << "[" << i << "] :" << pat[i]<<":" << endl;
		for (int j = 0; j < m - 1; ++j)
		{
			//cout << "i:"<<i<<" l: "<<j <<endl;
			int h = hashcpt(pat[i][j], pat[i][j + 1]);
			int sh = m - 2 - j;
			if (shift[h] == -1 || shift[h] > sh)
				shift[h] = sh;
			if(sh == 0){
				cout << " gogo"<< endl;
				if(hashtb[h] == nullptr){

					Node *newnd = new Node;
					newnd->next = nullptr;
					newnd->pattern = pat[i];

					cout << " pattern:"<<newnd->pattern<< endl;

					hashtb[h] = newnd;
					cout << " okok"<< endl;
				}else{
					Node *nd = hashtb[h];

					while(nd->next != nullptr)
						nd = nd->next;

					Node *newnd = new Node;
					newnd->next = nullptr;
					newnd->pattern = pat[i];

					nd->next = newnd;
				}
			}
		}
	}

	for(int i = 0; i < 729;++i){
		if(shift[i] != -1)
			cout<<"shift["<<i<<"]: "<<shift[i];

		if(hashtb[i] != nullptr){
			Node *nd = hashtb[i];
			cout << "----patterns: ";
			while(nd != nullptr){
				cout<<" ["<<nd->pattern<<"] ";
				nd = nd->next;
			}
		}
		if(shift[i] != -1)
		cout<<endl;
	}

	// Get all file names
	vector<string> files = find_files(argv[1]);

	//debug
//	for (auto i = files.begin(); i != files.end(); ++i)
//	{
//		cout << "[" << *i << "] "  << endl;
//	}

	for (auto i = files.begin(); i != files.end(); ++i)
	{
		ifstream tmp(*i, ios::in);

		int count = Wu_manber(tmp, pat, npats);
	}

}

int hashcpt(char first, char second)
{

	if (first == ' ')
	{
		first = 26;
	} else
	{
		first = first - 'a';
	}

	if (second == ' ')
	{
		second = 26;
	} else
	{
		second = second - 'a';
	}

	return first * 27 + second;
}

vector<string> find_files(char *folder)
{
	vector<string> files;
	DIR *d;
	struct dirent *dir;
	d = opendir(folder);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (dir->d_name[0] == '.')
				continue;

			string f(dir->d_name);
			files.push_back(f);
		}
		closedir(d);
	}

	return files;
}

int Wu_manber(ifstream& file, string* pat, int npats)
{
//	for (int i = 0; i < npats; ++i)
//	{
//		cout << "[" << i << "] " << pat[i] << endl;
//	}

	return 1;
}
