#include <bits/stdc++.h>
using namespace std;

class SpellingCorrector
{
private:
	typedef vector<string> Vector;
	typedef map<string, int> Dictionary;

	Dictionary dictionary;

	void edits(const string& word, Vector& result);
	void known(Vector& results, Dictionary& candidates);

public:
	void load(const string& filename);
	string correct(const string& word);
};

bool sortBySecond(const pair<string, int>& left, const pair<string, int>& right)
{
  return left.second < right.second;
}

char filterNonAlphabetic(char& letter)
{
  if (letter < 0)
    return '-';
  if (isalpha(letter))
    return tolower(letter);
  return '-';
}

void SpellingCorrector::load(const string& filename)
{
	FILE *fp=fopen("big.txt","r");
	char str[1000];
	string ss;
	while(fscanf(fp,"%s",str) != EOF)
	{
		char temp_buff[50];
		int i,index = 0;
		for(i = 0 ; str[i] ; ++i)
		{
			if(str[i] >= 'a' && str[i] <= 'z')
				temp_buff[index++] = str[i];
			else if(str[i] >= 'A' && str[i] <= 'Z')
				temp_buff[index++] = str[i]+32;
		}		
		temp_buff[index] = '\0';
		ss=temp_buff;
		dictionary[ss]++;
	}
	
}

string SpellingCorrector::correct(const string& word)
{
  Vector result;
  Dictionary candidates;

  if (dictionary.find(word) != dictionary.end()) { return word; }

  edits(word, result);
  known(result, candidates);

  if (candidates.size() > 0) { return max_element(candidates.begin(), candidates.end(), sortBySecond)->first; }

  for (unsigned int i = 0;i < result.size();i++)
  {
    Vector subResult;

    edits(result[i], subResult);
    known(subResult, candidates);
  }

  if (candidates.size() > 0) { return max_element(candidates.begin(), candidates.end(), sortBySecond)->first; }

  return "";
}

void SpellingCorrector::known(Vector& results, Dictionary& candidates)
{
  Dictionary::iterator end = dictionary.end();

  for (unsigned int i = 0;i < results.size();i++)
  {
    Dictionary::iterator value = dictionary.find(results[i]);

    if (value != end) candidates[value->first] = value->second;
  }
}

void SpellingCorrector::edits(const string& word, Vector& result)
{
  for (string::size_type i = 0;i < word.size(); i++)    result.push_back(word.substr(0, i) + word.substr(i + 1)); //deletions
  for (string::size_type i = 0;i < word.size() - 1;i++) result.push_back(word.substr(0, i) + word[i + 1] + word[i] + word.substr(i + 2)); //transposition

  for (char j = 'a';j <= 'z';++j)
  {
    for (string::size_type i = 0;i < word.size(); i++)    result.push_back(word.substr(0, i) + j + word.substr(i + 1)); //alterations
    for (string::size_type i = 0;i < word.size() + 1;i++) result.push_back(word.substr(0, i) + j + word.substr(i)); //insertion
  }
}

SpellingCorrector corrector;

void Correct(const string& wrong, const string& expected)
{
  const bool isCorrect = corrector.correct(wrong) == expected;
  cout << "(" << wrong << ") == (" << expected << ") = (" << boolalpha << isCorrect << ")" << endl;
}

int main()
{
  corrector.load("big.txt");

 /* Correct("speling", "spelling"); // insert
  Correct("korrectud", "corrected"); // replace 2
  Correct("bycycle", "bicycle"); // replace
  Correct("inconvient", "inconvenient"); // insert 2
  Correct("arrainged", "arranged"); // delete
  Correct("peotry", "poetry"); // transpose
  Correct("peotryy", "poetry"); // transpose + delete
  Correct("word", "word"); // known
  Correct("quintessential", ""); // unknown
*/
	string request;
	while (request != "quit")
	{
		cout << "Enter a word\n";
		cin >> request;

		string correct(corrector.correct(request));

		if (correct != "")
			cout << "You meant: " << correct << "?\n\n\n";
		else
			cout << "No correction suggestion\n\n\n";
	}

	cin.get();

	return 0;
}
