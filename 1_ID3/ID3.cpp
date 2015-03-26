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
int instance_check[MAX],attribute_check[10];
int instance_used;
vector < pair<double,int> > Gain;
int var_contribution[10][11][3];  // attribute - variable - positive/negative

struct TreeNode{
    int decision;
    bool isAttribute,isVariable;
    TreeNode *children[10];
    int attribute,variable;
    TreeNode(){
        attribute = variable = decision = -1;
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
    for(i=0; i<0.8*total_dataset-1; i++)
    {
        for(j=0; j<10; j++) training.attributes[j].push_back(input.attributes[j][i]);
        training_dataset++;
    }
    for(i=0.8*total_dataset-1; i<total_dataset; i++)
    {
        for(j=0; j<10; j++) test.attributes[j].push_back(input.attributes[j][i]);
    }
    test_dataset = total_dataset - training_dataset;
}


double entropy(int positives,int negatives)
{
    int denom = positives + negatives;
    if(!denom || !positives || !negatives) return 0;
    double entro = -1 * ((double)positives/denom) * log2((double)positives/denom) - ((double)negatives/denom) * log2((double)negatives/denom);
    return entro;
}

void calculate_gain(int attribute,int variable)
{
    Gain.clear();
    int i,j;
    memset(var_contribution,0,sizeof(var_contribution));
    for(i=0; i<9; i++)
    {
        if(!attribute_check[i])
        {
            int positives,negatives;
            positives = negatives = 0;
            for(j=0; j<training_dataset; j++)
            {
                if(!instance_check[j])
                {
                    int val = training.attributes[i][j];    // val ranges between 1-10
                    if(attribute>-1 && training.attributes[attribute][j]==variable)
                    {
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
                    else if(attribute == -1 && variable == -1)
                    {
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
            }
            int total = positives + negatives;
            double attribEntropy = entropy(positives,negatives);
            double varEntropy = 0.0;
            for(j=1; j<11; j++)
            {
                // attribute values
                positives = var_contribution[i][j][0];
                negatives = var_contribution[i][j][1];
                //printf("j%d p%d n%d\n",j,positives,negatives);
                varEntropy += entropy(positives,negatives) * ((double)(positives+negatives) / total);
            }
            double gain = attribEntropy - varEntropy;
            //printf("%0.2f %0.2f %0.2f\n",gain,attribEntropy,varEntropy);
            Gain.push_back(make_pair(gain,i));
        }
    }
}

void deleteData(int attribute,int variable)
{
    int i,j;
    for(i=0; i<total_dataset-test_dataset; i++)
    {
        if(training.attributes[attribute][i] == variable) instance_check[i]++,training_dataset--;
    }
}

bool gainsort(pair<double,int> p1,pair<double,int> p2)
{
    return p1.first < p2.first;
}
void sort_gain()
{
    sort(Gain.begin(),Gain.end(),gainsort);

}

void ID3(TreeNode *root)
{
    calculate_gain(root->attribute,root->variable);
    sort_gain();
    if(Gain.size()>0)
    {
        pair <double,int> current = Gain.back();
        int attribute = current.second;
        attribute_check[attribute]++;
        if(root->attribute == -1) root->attribute = attribute;
        root->isAttribute = true;
        for(int i=1; i<11; i++)
        {
            TreeNode *node;
            node->isVariable = true;
            node->variable = i;
            int positives = var_contribution[attribute][i][0];
            int negatives = var_contribution[attribute][i][1];
            if(positives+negatives > 0)
            {
                if(!positives)
                {
                    node->decision = MALIGNANT;
                    deleteData(attribute,i);
                }
                else if(!negatives)
                {
                    node->decision = BENIGN;
                    deleteData(attribute,i);
                }
            }
            root->children[i] = node;
            ID3(root->children[i]);
        }
    }
}

int main()
{
    freopen("data.txt","r",stdin);
    init();
    TreeNode *root;
    ID3(root);
    return 0;
}
