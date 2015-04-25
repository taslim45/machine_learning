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
#define SMOOTHING 1

map <string,int> Vocab;
map <string,int> topics;
map <string,int> documentTopic;
map <string,int> bags[MAXTOPICS];
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
        //puts(text);
        it = topics.find(text);
        if(it!=topics.end()) tp = topics[text], topicCount[tp]++;
        else tp = -1;
        ndocs++;
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
                    Vocab[_word]++;
                    bags[tp][_word]++;
                    wordTopicCount[tp]++;

                }
            }
        }
        gets(text);
    }
    fclose(stdin);
}
void read_test()
{
    freopen("test.data", "r", stdin);
    int ntest = 0, match = 0;
    int withoutblanks = 0;
    int tp,ftp,i;
    char *str;
    vector < pair < double , int > > V;
    char text[100006];
    while (gets(text))
    {
        //puts(text);
        ++ntest;
        it = topics.find(text);
        if(it!=topics.end()) tp = topics[text];
        else tp = -1;
        map <string,int> trainWords;
        for (i=0; i<5; i++) gets(text);
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
                    trainWords[word]++;
                }
            }
        }
        gets(text);
        if(trainWords.size()>0)
        {
            withoutblanks++;
            double val = 0.0;
            int res = -1;
            vector < pair <double,int> > results;
            for(int k=1; k<=topics.size(); k++)
            {
                string topicname = reverseTopics[k];
                int topicFrequency = topicCount[k];
                //if(!topicFrequency) continue;
                val = log(topicFrequency /(double) ndocs)*-1;
                //printf("%f\n",val);
                int totalWordInTopicK = wordTopicCount[k];

                for(it=trainWords.begin(); it!=trainWords.end(); it++)
                {
                    map<string,int>::iterator nit;
                    string word = it->first;
                    int frequency = it->second;
                    nit = bags[k].find(word);
                    int wordInTopicK;
                    if(nit!=bags[k].end()) wordInTopicK = bags[k][word] + SMOOTHING;
                    else wordInTopicK = SMOOTHING;
                    double probability = log(wordInTopicK / (double)(totalWordInTopicK + Vocab.size())) * -1;
                    //cout<<val<<" before\n";
                    val *= pow(probability,(double)frequency);
                    //cout<<val<<" after\n";
                    //val *= probability;
                }
                //cout<<val<<"\n";
                results.push_back(make_pair(val,k));
            }
            sort(results.begin(),results.end());
            res = results.front().second;
            match += (tp == res);
        }
    }
    printf("total = %d, without blanks=%d, match = %d\n", ntest,withoutblanks, match);
    //printf("Accuracy %f \%\n",match*100/(double)ntest);
    printf("Accuracy %f \%\n",match*100/(double)withoutblanks);
    fclose(stdin);
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
int main()
{
    prep_ignorables();
    read_topics();
    read_training();
    read_test();
    return 0;
}
