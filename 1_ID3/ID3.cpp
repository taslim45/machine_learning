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
#define MAX 1000

#define BENIGN 0
#define MALIGNANT 1

struct data{
    vector <int> attributes[10];
    data(){
        for(int i=0; i<10; i++) attributes[i].clear();
    }
};
data input,training,test;

int total_dataset,training_dataset,test_dataset;
int training_check[MAX];
int training_used;
double Gain[10];

struct TreeNode{
    int decision;
    bool isAttribute,isVariable;
    TreeNode *children[10];
    int attribute,variable;
    TreeNode(){
        decision = -1;
        isAttribute = isVariable = false;
        for(int i=0; i<10; i++)
        {
            children[i] = NULL;
        }
    }
};


void init()
{
    int inp[10];
    total_dataset = 0;
    int i,j;
    while(scanf("%d%d%d%d%d%d%d%d%d%d",&inp[0],&inp[1],&inp[2],&inp[3],&inp[4],&inp[5],&inp[6],&inp[7],&inp[8],&inp[9])!=EOF)
    {
        for(i=0; i<10; i++) input.attributes[i].push_back(inp[i]);
        total_dataset++;
    }
    //printf("%d\n",cnt);
    training_dataset = 0;
    for(i=0; i<0.8*total_dataset; i++)
    {
        for(j=0; j<10; j++) training.attributes[j].push_back(input.attributes[j][i]);
        training_dataset++;
    }
    for(i=0.8*total_dataset; i<total_dataset; i++)
    {
        for(j=0; j<10; j++) test.attributes[j].push_back(input.attributes[j][i]);
    }
    test_dataset = total_dataset - training_dataset;
}


double entropy(int positives,int negatives)
{
    int denom = positives + negatives;
    double entro = -1 * ((double)positives/denom) * log2((double)positives/denom) - ((double)negatives/denom) * log2((double)negatives/denom);
    return entro;
}

void calculate_gain()
{
    int i,j;
    int var_contribution[10][11][3];  // attribute - variable - positive/negative
    memset(var_contribution,0,sizeof(var_contribution));
    for(i=0; i<9; i++)
    {
        int positives,negatives;
        positives = negatives = 0;
        for(j=0; j<training_dataset; j++)
        {
            if(!training_check[j])
            {
                int val = training.attributes[i][j];    // val ranges between 1-10
                if(training.attributes[9][j] == BENIGN)
                {
                    positives++;
                    var_contribution[i][val][0] += 1;
                }
                else
                {
                    negatives++;
                    var_contribution[i][val][1] += 1;
                }
            }
        }
        int total = positives + negatives;
        double attribEntropy = entropy(positives,negatives);
        double varEntropy = 0.0;
        for(j=1; j<11; j++)
        {
            // attribute values
            positives = var_contribution[i][j][0];
            negatives = var_contribution[i][j][1];
            varEntropy += entropy(positives,negatives) * ((double)(positives+negatives) / total);
        }
        double gain = attribEntropy - varEntropy;
        Gain[i] = gain;
    }
}

void ID3(TreeNode root)
{
    calculate_gain();

}

int main()
{
    freopen("data.txt","r",stdin);
    init();
    TreeNode root;
    ID3(root);

    return 0;
}
