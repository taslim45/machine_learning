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

int attribs[MAX_AT];

struct data
{
    int attributes[MAX_AT];
    int which_class;
    data(int attribs[MAX_AT],int classs)
    {
        which_class = classs;
        for(int i=0; i<MAX_AT; i++) attributes[i] = attribs[i];
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
    for(int i=0; i<MAX_VAR; i++)
    {
        if (mat[i][0] == 0 || mat[i][1] == 0) continue;
        positives = mat[i][0];
        negatives = mat[i][1];
        entro += entropy(positives,negatives);
    }
    entro /= trainset.size();
    return entro;
}

double calcEntropy(vector <data> &trainset, int tattr)
{
    int cnt[MAX_VAR][2];
    int i;
    memset(cnt, 0, sizeof(cnt));
    for (i=0; i<trainset.size(); ++i)
    {
        ++cnt[trainset[i].attributes[tattr]][trainset[i].which_class];
    }
    double entropy = 0.0, p,n;
    for (i=0; i<MAX_VAR; ++i)
    {
        if (cnt[i][0] == 0 || cnt[i][1] == 0) continue;
        n = cnt[i][0] / (double) (cnt[i][0] + cnt[i][1]);
        p = cnt[i][1] / (double) (cnt[i][0] + cnt[i][1]);
        entropy -=  (n * log10(n) + p * log10(p)) * (cnt[i][0] + cnt[i][1]);
    }
    entropy /= log10(2.0);
    entropy /= trainset.size();
    return entropy;
}

int getTargetAttribute(vector < data > &trainset, int mask)
{
    int i;
    int cnt[2] = {0};
    double n,p;
    for (i=0; i<trainset.size(); ++i) ++cnt[trainset[i].which_class];
    double entropy = 0.0;
    n = cnt[0] / (double) (cnt[0] + cnt[1]);
    p = cnt[1] / (double) (cnt[0] + cnt[1]);
    entropy -=  n * log10(n) + p * log10(p);
    entropy /= log10(2.0);
    vector < pair <double,int> > G;
    for (i=0; i<MAX_AT; ++i)
    {
        if (mask&(1<<i)) continue;
        G.push_back(make_pair(entropy - calcEntropy(trainset,i), i));
    }
    sort (G.begin(), G.end());
    printf("7\n");
    return G.back().second;
}

TreeNode *ID3(vector <data> &trainset, int mask)
{
    int mat[2] = {0};
    int i;
    for (i=0; i<trainset.size(); ++i)
    {
        data d = trainset[i];
        mat[d.which_class]++;
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
    printf("5\n");
    node->tattr = target = getTargetAttribute(trainset, mask);
    printf("6\n");
    node->mask = mask | (1<<target);
    vector <data> V[MAX_VAR];
    for(i=0; i<trainset.size(); i++)
    {
        V[trainset[i].attributes[target]].push_back(trainset[i]);
    }
    for(i=0; i<MAX_VAR; i++)
    {
        if (V[i].size() == 0)
        {
            printf("3\n");
            node->children[i] = new TreeNode;
            int positives = contribution[target][i][0];
            int negatives = contribution[target][i][1];
            if(positives > negatives)
            {
                node->children[i]->cl = BENIGN;
            }
            else node->children[i]->cl = MALIGNANT;
            printf("4\n");
        }
        else node->children[i] = ID3(V[i], node->mask);
    }
    return node;
}


int result(TreeNode *node,data d)
{
    if(node->cl > -1) return node->cl;
    while(node->children[d.attributes[node->tattr]]!=NULL);
    return result(node->children[d.attributes[node->tattr]],d);
}

void init()
{
    int i,clas;
    while(scanf("%d",&attribs[0])!=EOF)
    {
        for(i=1; i<MAX_AT; i++)
        {
            scanf("%d",&attribs[i]);
            attribs[i]--;
        }
        scanf("%d",&clas);
        data d(attribs,clas);
        totalset.push_back(d);
    }
}

void calculate_contribution()
{
    int i,j;
    for(i=0; i<training.size(); i++)
    {
        for(j=0; j<MAX_AT; j++)
        {
            data tdata = training[i];
            contribution[j][tdata.attributes[j]][tdata.which_class];
        }
    }
}

void randomize_data()
{
    random_shuffle(totalset.begin(),totalset.end());
}

int main()
{
    freopen("data.txt","r",stdin);
    init();
    int i,j;
    for(i=0; i<TEST; i++)
    {
        training.clear();
        test.clear();
        randomize_data();
        for(j=0; j<totalset.size()*0.8; j++)
        {
            training.push_back(totalset[j]);
        }
        for(j=totalset.size()*0.8; j<totalset.size(); j++)
        {
            test.push_back(totalset[j]);
        }
        memset(contribution,0,sizeof(contribution));
        calculate_contribution();
        printf("1\n");
        TreeNode *root = ID3(training,0);
        printf("2\n");
        int match = 0;
        for(j=0; j<test.size(); j++)
        {
            int clas = result(root,test[j]);
            if(clas == test[j].which_class) match++;
        }
        printf("Test #%d, matched sample = %d, mismatched=%d\n",i,match,test.size() - match);
    }

    return 0;
}
