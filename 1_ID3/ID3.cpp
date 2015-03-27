#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>
#include <queue>
#include <set>
#include <random>
#include <math.h>
using namespace std;
#define MAX_AT 9
#define MAX_VAR 10
#define TEST 100

#define BENIGN 0
#define MALIGNANT 1

struct data
{
    int attributes[MAX_AT];
    int which_class;
    data(int attribs[MAX_AT],int classs)
    {
        which_class = classs;
        for(int i=0; i<MAX_AT; i++) attributes[i] = atribs[i];
    }
};

vector <data> totalset,training,test;

struct TreeNode
{
    int cl,tattr,mask;
    TreeNode *children[MAX_VAR];
    TreeNode()
    {
        cl = tattr = mask = -1;
        for(int i=0; i<MAX_VAR; i++) children[i] = NULL;
    }
};

int contribution[MAX_AT][MAX_VAR][2];

double entropy(int positives,int negatives)
{
    int denom = positives + negatives;
    if(!denom || !positives || !negatives) return 0;
    double entro = -1 * ((double)positives/denom) * log2((double)positives/denom) - ((double)negatives/denom) * log2((double)negatives/denom);
    return entro;
}

double entropy_calculator(vector <data> &trainset, int target_attribute)
{
    int mat[MAX_VAR][2];
    memset(mat, 0, sizeof(mat));
    for(int i=0; i<trainset.size(); i++)
    {
        int val = trainset[i].attributes[target_attribute];
        int cls = trainset[i].which_class;
        mat[val][cls]++;
    }
    double entro = 0.0;
    int positives,negatives;
    for(int i=0; i<MAX_ATTR_V; i++)
    {
        if (mat[i][0] == 0 || mat[i][1] == 0) continue;
        positives = mat[i][0];
        negatives = mat[i][1];
        entro += entropy(positives,negatives);
    }
    entro /= trainset.size();
    return entropy;
}

int getTargetAttribute(vector < data > &trainset, int mask)
{
    int i;
    int clas[2] = {0};
    double negatives,positives;
    for(i=0; i<trainset.size(); i++)
    {
        clas[trainset[i].which_class]++;
    }
    double entro = 0.0;
    negatives = (double)clas[0];
    positives = (double)clas[1];
    int sub_total = negatives + positives;
    entro -=  (negatives/sub_total) * log2(negatives/sub_total) + (positives/sub_total) * log2(positives/sub_total);
    vector < pair < double,int > > Gain;
    for(i=0; i<MAX_AT; i++)
    {
        if (mask&(1<<i)) continue;
        Gain.push_back(make_pair(entro - entropy_calculator(trainset,i), i));
    }
    sort (Gain.begin(), Gain.end());
    return Gain.back().second;
}

TreeNode *createTree(vector < data > &trainset, int mask)
{
    int mat[2] = {0};
    int i;
    for (i=0; i<trainset.size(); ++i)
    {
        mat[trainset[i].which_class]++;
    }
    if (mat[0] == 0 || mat[1] == 0 || (mask+1 == (1<<MAX_AT)))
    {
        TreeNode *node;
        if(mat[0] > mat[1])
        {
            node->cl = BENIGN;
        }
        else node->cl = MALIGNANT;
        return node;
    }
    int target;
    TreeNode *node;
    target = (node -> target) = getTargetAttribute(trainset, mask);
    node->mask = mask | (1<<target);
    vector < data > V[MAX_VAR];
    for(i=0; i<trainset.size(); i++)
    {
        V[trainset[i].attributes[target]].push_back(trainset[i]);
    }
    for(i=0; i<MAX_VAR; i++)
    {
        if (V[i].size() == 0)
        {
            node->children[i];
            int positives = contribution[target][i][0];
            int negatives = contribution[target][i][1];
            if(positives > negatives)
            {
                node -> children[i] -> cl = BENIGN;
            }
            else node -> children[i] -> cl = MALIGNANT;
        }
        else node->child[i] = build(V[i], node->mask);
    }
    return node;
}


int main()
{
    freopen("data.txt","r",stdin);
    init();
    TreeNode root;

    return 0;
}
