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
#include<algorithm>

using namespace std;

struct Node
{
	string pattern;
	int id;
	Node* next;
};

int shift[729];
Node *hashtb[729];
int m = 257, B = 2;

int hashcpt(char first, char second);

vector<string> find_files(char* folder);

int Wu_manber(ifstream& file,int npats);

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
	if (folder[folder.size() - 1] != '/')
		folder += "/";

	string o(argv[3]);
	short pbegin = 3;
	if (o == "-s")
	{
		assert(argc > 5);
		pbegin = 5;
	}

	int npats = argc - pbegin;
	string pat[npats];

	//Get all patterns from argv[] to pat[]
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

		//Set m as the minimum length of searching pattern
		int sz = pat[i - pbegin].size();
		if (sz < m)
			m = pat[i - pbegin].size();
	}

	// Preprocessing patterns
	fill_n(shift, 729, -1);
	fill_n(hashtb, 729, nullptr);
	for (int i = 0; i < npats; ++i)
	{
		for (int j = 0; j < m - 1; ++j)
		{
			// Compute the hash value and save the shift value into hash array
			// If the shift value is 0, save the pattern into the linked list hash array
			int h = hashcpt(pat[i][j], pat[i][j + 1]);
			int sh = m - 2 - j;
			if (shift[h] == -1 || shift[h] > sh)
				shift[h] = sh;
			if (sh == 0)
			{
				if (hashtb[h] == nullptr)
				{

					Node *newnd = new Node;
					newnd->next = nullptr;
					newnd->pattern = pat[i];
					newnd->id = i;
					hashtb[h] = newnd;

				} else
				{
					Node *nd = hashtb[h];

					while (nd->next != nullptr)
						nd = nd->next;

					Node *newnd = new Node;
					newnd->next = nullptr;
					newnd->pattern = pat[i];
					newnd->id = i;

					nd->next = newnd;
				}
			}
		}
	}

//	for(int i = 0; i < 729;++i){
//		if(shift[i] != -1)
//			cout<<"shift["<<i<<"]: "<<shift[i];
//
//		if(hashtb[i] != nullptr){
//			Node *nd = hashtb[i];
//			cout << "----patterns: ";
//			while(nd != nullptr){
//				cout<<" ["<<nd->pattern<<"] ";
//				nd = nd->next;
//			}
//		}
//		if(shift[i] != -1)
//		cout<<endl;
//	}

	// Get all file names
	vector<string> files = find_files(argv[1]);

	vector<pair<string,int>> occ;

	for (auto i = files.begin(); i != files.end(); ++i)
	{
		ifstream tmp(folder + *i, ios::in);

		int count = Wu_manber(tmp,npats);
//		if (count != 0)
//			cout << "[" << *i << "] :" << count << ":" << endl;
		if (count != 0)
			occ.push_back(make_pair(*i, count));
		tmp.close();
	}
	sort(occ.begin(),occ.end(),[](const pair<string,int>& p1,const pair<string,int>& p2){
		if(p1.second == p2.second)
			return p1.first < p2.first;
		return p1.second > p2.second;
	});
	for (auto i = occ.begin();i!=occ.end();++i){
		cout<<i->first<<endl;
	}
}

int hashcpt(char first, char second)
{

	if (first <= 'Z' && first >= 'A')
		first = first - ('Z' - 'z');
	if (second <= 'Z' && second >= 'A')
		second = second - ('Z' - 'z');
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

			string f = "";
			int i = 0;
			while (dir->d_name[i] != '\0')
			{
				if (dir->d_name[i] == '[' || dir->d_name[i] == ']'
						|| dir->d_name[i] == ' ' || dir->d_name[i] == '<'
						|| dir->d_name[i] == '>')
				{
					f += "\\";
				}
				f += dir->d_name[i];
				i++;
			}
			files.push_back(f);
		}
		closedir(d);
	}

	return files;
}

int Wu_manber(ifstream& file,int npats)
{
	int c[npats];
	fill_n(c, npats, 0);
	// string tmp is to save the line of files
	string tmp = "";
	// int count counts the times of pattern matched
	int count = 0;
	while (getline(file, tmp))
	{
		int i = (m - 1) - 1;
		int sz = tmp.size();
		while (i < sz - 1)
		{
			// Check if two 'last' characters are special characters
			if ((tmp[i] < 65 && tmp[i] != ' ') || (tmp[i] > 90 && tmp[i] < 97)
					|| tmp[i] > 122)
			{
				i += m - 1;
				continue;
			}
			if ((tmp[i + 1] < 65 && tmp[i + 1] != ' ')
					|| (tmp[i + 1] > 90 && tmp[i + 1] < 97) || tmp[i + 1] > 122)
			{
				i += m;
				continue;
			}

			// Compute the hash value of two characters and get the shift value
			int h = hashcpt(tmp[i], tmp[i + 1]);

			if (shift[h] == -1)
			{
				// If shift value equals to -1, this combination is not in
				// any patterns, shift i
				i += m - 1;
				continue;
			} else if (shift[h] != 0)
			{
				// if shift value not equals to 0, shift i
				i += shift[h];
				continue;
			} else
			{
				// If shift value equals to 0, compare all the possible patterns from linked list
				Node *nd = hashtb[h];
				while (nd != nullptr)
				{
					int b = i - (m - 2);
					int f = 1;
					for (unsigned int j = 0; j < nd->pattern.size(); ++j)
					{
						if (b == sz)
						{
							f = 0;
							break;
						}
						char cmp = tmp[b];
						if (cmp <= 'Z' && cmp >= 'A')
							cmp = cmp - ('Z' - 'z');
						if (nd->pattern[j] != cmp)
						{
							f = 0;
							break;
						}
						b++;
					}
					if (f == 1)
						c[nd->id] = count++;

					nd = nd->next;
				}
				i++;
			}

		}
	}
	for(int i = 0;i<npats;++i){
		if(c[i] == 0)
			return 0;

	}
	return count;
}
