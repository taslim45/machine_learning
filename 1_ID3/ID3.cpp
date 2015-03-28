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
#include <ctime>
using namespace std;
#define TEST 100
#define MAX_ATTR 9
#define MAX_VAR 10

#define BENIGN 0
#define MALIGNANT 1

class einfo
{
public:
    int attributes[MAX_ATTR];
    bool clas;
    einfo(int _attr[MAX_ATTR], bool _cl)
    {

        for(int i=0; i<MAX_ATTR; i++) attributes[i] = _attr[i];
        clas = _cl;
    }
};

int contribution[MAX_ATTR][MAX_VAR][2];
vector < einfo > data, train, test;

class TreeNode
{
public:
    int clas, targetattr, attribmask;
    TreeNode *child[MAX_VAR];
    TreeNode()
    {
        int i;
        for (i=0; i<MAX_VAR; i++) child[i] = NULL;
        targetattr = clas = attribmask = -1;
    }
};

double calcEntropy(vector < einfo > &trainset, int targetattr)
{
    int contrib[MAX_VAR][2];
    int i;
    memset(contrib, 0, sizeof(contrib));
    for (i=0; i<trainset.size(); i++)
    {
        contrib[trainset[i].attributes[targetattr]][trainset[i].clas]++;
    }
    double entropy = 0.0, positives,negatives;
    for (i=0; i<MAX_VAR; i++)
    {
        if (contrib[i][0] == 0 || contrib[i][1] == 0) continue;
        negatives = contrib[i][0] / (double) (contrib[i][0] + contrib[i][1]);
        positives = contrib[i][1] / (double) (contrib[i][0] + contrib[i][1]);
        entropy -=  (negatives * log10(negatives) + positives * log10(positives)) * (contrib[i][0] + contrib[i][1]);
    }
    entropy = entropy / log10(2.0);
    entropy /= trainset.size();
    return entropy;
}

bool isAttributeUsed(int attribmask,int k)
{
    return attribmask & (1<<k);
}

int findTarAttr(vector < einfo > &trainset, int attribmask)
{
    int i;
    int contrib[2] = {0};
    double negatives,positives;
    for (i=0; i<trainset.size(); i++) contrib[trainset[i].clas]++;
    double entropy = 0.0;
    negatives = contrib[0] / (double) (contrib[0] + contrib[1]);
    positives = contrib[1] / (double) (contrib[0] + contrib[1]);
    entropy -=  negatives * log10(negatives) + positives * log10(positives);
    entropy /= log10(2.0);
    vector < pair < double , int > > G;
    for (i=0; i<MAX_ATTR; i++)
    {
        if (isAttributeUsed(attribmask,i)) continue;
        G.push_back(make_pair(entropy - calcEntropy(trainset,i), i));
    }
    sort (G.begin(), G.end());
    return G.back().second;
}
TreeNode *createTree(vector < einfo > &trainset, int attribmask)
{
    int contrib[2] = {0}, i;
    for (i=0; i<trainset.size(); i++) contrib[trainset[i].clas]++;
    if (contrib[0] == 0 || contrib[1] == 0 || (attribmask+1 == (1<<MAX_ATTR)))
    {
        TreeNode *node = new TreeNode();
        node -> clas = (contrib[0] > contrib[1]) ? BENIGN:MALIGNANT;
        return node;
    }
    int targetattr;
    TreeNode *node = new TreeNode();
    targetattr = (node -> targetattr) = findTarAttr(trainset, attribmask);
    node -> attribmask = attribmask | (1<<targetattr);
    vector < einfo > V[MAX_VAR];
    for (i=0; i<trainset.size(); i++) V[trainset[i].attributes[targetattr]].push_back(trainset[i]);
    for (i=0; i<MAX_VAR; i++)
    {
        if (V[i].size() == 0)
        {
            node -> child[i] = new TreeNode();
            int n = contribution[targetattr][i][0], p = contribution[targetattr][i][1];
            node -> child[i] -> clas = (n > p) ? BENIGN:MALIGNANT;
        }
        else node -> child[i] = createTree(V[i], node -> attribmask);
    }
    return node;
}
bool runTest(TreeNode *node, einfo e)
{
    if ((node -> clas) != -1) return node->clas;
    if (node->child[e.attributes[node->targetattr]] == NULL) while(1);
    return runTest(node->child[e.attributes[node->targetattr]], e);
}

double getPrecision(int TP,int FP)
{
    return (double)TP / (TP+FP);
}
double getRecall(int TP,int FN)
{
    return (double)TP / (TP+FN);
}
double getGmean(int TP,int TN,int FP,int FN)
{
    double d1 = getRecall(TP,FN);
    double d2 = (double)TN/(FP+TN);
    return sqrt(d1*d2);
}
void init()
{
    int attr[MAX_ATTR], i;
    int clas;
    while (scanf("%d", &attr[0]) != EOF)
    {
        for (i=1; i<MAX_ATTR; i++)
        {
            scanf("%d", &attr[i]);

        }
        for (i=0; i<MAX_ATTR; i++) attr[i]--;
        scanf("%d", &clas);
        data.push_back(einfo(attr,clas));
    }
}
void randomize()
{
    srand(time(0));
    random_shuffle(data.begin(), data.end());
}

int main()
{
    int i;
    freopen("data.txt", "r", stdin);
    freopen("output.txt","w",stdout);
    init();
    int trainsize = data.size() * 0.8, t;
    int match;
    int TP,FP,FN,TN;
    double sum_accuracy=0,sum_precision=0,sum_recall=0,sum_fmeasure=0,sum_gmean=0;
    double accuracy,precision,recall,fmeasure,gmean;
    for(int k=0; k<TEST; k++)
    {
        train.clear(), test.clear();
        randomize();
        for (i=0; i<trainsize; i++) train.push_back(data[i]);
        for (; i<data.size(); i++) test.push_back(data[i]);
        memset(contribution, 0, sizeof(contribution));
        for (t=0; t<train.size(); t++)
        {
            for (i=0; i<MAX_ATTR; i++) contribution[i][train[t].attributes[i]][train[t].clas]++;
        }
        TreeNode *root = createTree(train, 0);
        match = 0;
        TP = FP = FN = TN = 0;
        for (t=0; t<test.size(); t++)
        {
            int res = runTest(root, test[t]);
            if(res == test[t].clas) match++;
            if(test[t].clas == MALIGNANT && res == MALIGNANT) TP++;
            else if(test[t].clas == MALIGNANT && res == BENIGN) FN++;
            else if(test[t].clas == BENIGN && res == BENIGN) TN++;
            else if(test[t].clas == BENIGN && res == MALIGNANT) FP++;
        }
        sum_precision += getPrecision(TP,FP);
        sum_accuracy += (double)match / test.size();
        sum_recall += getRecall(TP,FN);
        sum_fmeasure += (2*getPrecision(TP,FP)*getRecall(TP,FN))/ (getPrecision(TP,FP)+getRecall(TP,FN));
        sum_gmean += getGmean(TP,TN,FP,FN);
        printf("Test %d : Matched %d, Mismatched %d\n",k+1,match,test.size()-match);
    }
    puts("========");
    printf("Accuracy=%0.3f\n",sum_accuracy/TEST);
    printf("Precision=%0.3f\n",sum_precision/TEST);
    printf("Recall=%0.3f\n",sum_recall/TEST);
    printf("F-measure=%0.3f\n",sum_fmeasure/TEST);
    printf("G-mean=%0.3f\n",sum_gmean/TEST);
    return 0;
}
