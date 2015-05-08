#include <stdio.h>
#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <math.h>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
using namespace std;
#define MAXTOPICS 150
#define MAXDOCUMENTS 6550

map <string,int> Vocab;
map <string,int> topics;
map <int,int> documentTopic;
map <int,int> document_bags[MAXDOCUMENTS];
map <int,int> ::iterator intit;
map <string,int> ::iterator it;

vector <string> reverseTopics;
int ndocs = 0;
int wordTopicCount[MAXTOPICS];
int topicCount[MAXTOPICS];
set <string> ignorables;
set <string>::iterator sit;

vector<string> split(string s)
{
    char delim[20];
    int i,j;
    for(i=0,j=48; j<58; j++,i++)
    {
        delim[i] = j;
    }
    delim[i] = '"';
    delim[++i] = ' ';
    delim[++i] = ',';
    delim[++i] = '.';
    delim[++i] = '-';
    delim[++i] = ';';
    delim[++i] = '&';
    delim[++i] = '\0';

    int len = s.size();
    char str[len+1];
    strcpy(str,s.c_str());
    char *pch;
    pch = strtok (str,delim);             //   " \",.-;&");
    vector <string> vs;

    while(pch!=NULL)
    {
        vs.push_back(pch);
        pch = strtok (NULL, delim);
    }
    return vs;
}

void prep_ignorables()
{
    ignorables.insert("a");
    ignorables.insert("an");
    ignorables.insert("and");
    ignorables.insert("the");
    ignorables.insert("in");
    ignorables.insert("of");
    ignorables.insert("off");
    ignorables.insert("on");
    ignorables.insert("by");
    ignorables.insert("be");
    ignorables.insert("is");
    ignorables.insert("are");
    ignorables.insert("was");
    ignorables.insert("were");
    ignorables.insert("to");
    ignorables.insert("at");
    ignorables.insert("with");
}

void read_topics()
{
    topics.clear();
    string line;
    ifstream myfile( "topics.data" );
    if (myfile)
    {
        while (getline( myfile, line ))
        {
            //printf("before %d\n",topics.size());
            if(line.size()!=0) topics[line] = topics.size();
            //printf("after %d\n",topics.size());
        }
        myfile.close();
    }
    int k = 1;
    reverseTopics.clear();
    reverseTopics.push_back("");
    for(it=topics.begin(); it!=topics.end(); it++)
    {
        reverseTopics.push_back(it->first);
    }
}

void read_training()
{
    freopen("training.data", "r", stdin);
    int tp,i;
    char *str;
    char text[100006];
    while (gets(text))
    {
        it = topics.find(text);
        if(it!=topics.end()) tp = topics[text];
        else tp = -1;
        ndocs++;
        documentTopic[ndocs] = tp;
        for (i=0; i<5; i++) gets(text);
        while (gets(text) && text[0])
        {
            if(tp==-1) continue;
            string ss = text;
            vector <string> words = split(ss);
            int k;
            for(k=0; k<words.size(); k++)
            {
                string _word = words[k];
                std::transform(_word.begin(),_word.end(),_word.begin(),::tolower);
                sit = ignorables.find(_word);

                if(_word.size()>0 && sit==ignorables.end())
                {
                    it = Vocab.find(_word);
                    if(it==Vocab.end()) Vocab[_word] = Vocab.size();
                    int index = Vocab[_word];
                    document_bags[ndocs][index]++;
                }
            }
        }
        gets(text);
    }
    fclose(stdin);
}

void hammingDistance()
{
    freopen("test.data", "r", stdin);
    int ntest = 0, match = 0;
    int withoutblanks = 0;
    int tp,ftp,i;
    char *str;
    char text[100006];
    map <int,int> test_bags;
    while (gets(text))
    {
        ntest++;
        it = topics.find(text);
        if(it!=topics.end()) tp = topics[text];
        else tp = -1;
        map <string,int> trainWords;
        for (i=0; i<5; i++) gets(text);
        int offset_distance = 0;
        while (gets(text) && text[0])
        {
            if(tp==-1) continue;
            string ss = text;
            vector <string> words = split(ss);
            for(int k=0; k<words.size(); k++)
            {
                string word = words[k];
                std::transform(word.begin(),word.end(),word.begin(),::tolower);

                sit = ignorables.find(word);
                if(word.size()>0 && sit==ignorables.end())
                {
                    //trainWords[word]++;
                    it = Vocab.find(word);
                    int index;
                    if(it == Vocab.end()) offset_distance++;
                    else
                    {
                        index = Vocab[word];
                        test_bags[index]++;
                    }
                }
            }
        }
        gets(text);
        vector < pair <int,int> > results;
        if(test_bags.size()>0)
        {
            withoutblanks++;
            for(i=1; i<=ndocs; i++)
            {
                int cnt = 0;
                for(it=Vocab.begin(); it!=Vocab.end(); it++)
                {
                    bool pre=false,post=false;
                    int index = it->second;
                    map <int,int> ::iterator tempit;
                    tempit = document_bags[i].find(index);
                    if(tempit!=document_bags[i].end()) pre = true;
                    tempit = test_bags.find(index);
                    if(tempit!=test_bags.end()) post = true;
                    if((!pre && post) || (pre && !post)) cnt++;
                }
                results.push_back(make_pair(cnt+offset_distance,i));
            }
        }
        if(results.size()>0)
        {
            sort(results.begin(),results.end());
            pair <int,int> res = results[0];
            ftp = documentTopic[res.second];
            if(ftp == tp)
            {
                match++;
                //puts("matched");
            }
        }
    }
    printf("total = %d, without blanks=%d, match = %d\n", ntest,withoutblanks, match);
    //printf("Accuracy %f \%\n",match*100/(double)ntest);
    printf("Accuracy %f \%\n",match*100/(double)withoutblanks);
    fclose(stdin);

}

int main()
{
    prep_ignorables();
    read_topics();
    read_training();
    hammingDistance();
    return 0;
}

