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

vector <int> inputs[10];
vector <int> training_index;
vector <int> test_index;
int total_dataset,training_dataset,test_dataset;
vector < pair <double,int> > Gain;

void clear_data()
{
    Gain.clear();
    training_index.clear();
    test_index.clear();
}
void init()
{
    clear_data();
    int inp[10];
    total_dataset = 0;
    int i,j;
    for(i=0; i<10; i++) inputs[i].clear();
    while(scanf("%d%d%d%d%d%d%d%d%d%d",&inp[0],&inp[1],&inp[2],&inp[3],&inp[4],&inp[5],&inp[6],&inp[7],&inp[8],&inp[9])!=EOF)
    {
        for(i=0; i<10; i++) inputs[i].push_back(inp[i]);
        total_dataset++;
    }
    training_dataset = 0;
    for(i=0; i<total_dataset*0.8; i++) training_index.push_back(i),training_dataset++;
    for(i=total_dataset*0.8; i<total_dataset; i++) test_index.push_back(i);
    test_dataset = total_dataset - training_dataset;
}


double entropy(int positives,int negatives)
{
    int denom = positives + negatives;
    if(!denom || !positives || !negatives) return 0;
    double entro = -1 * ((double)positives/denom) * log2((double)positives/denom) - ((double)negatives/denom) * log2((double)negatives/denom);
    return entro;
}

int _class(int index)
{
    return inputs[9][index];
}

void gain_calculator(vector <int> dataindex,int contribution[10][11][2],vector <int> seen)
{
    Gain.clear();
    int i,j;
    int index,var,positives,negatives,total;
    double attribEntropy,varEntropy;
    vector <int> ::iterator it;
    if(seen.size()>0)
    {
        // selective
        for(i=0; i<9; i++)
        {
            it = find(seen.begin(),seen.end(),i);
            if(it!=seen.end()) continue;
            positives = negatives = 0;
            for(j=0; j<dataindex.size(); j++)
            {
                index = dataindex[j];
                var = inputs[i][index];
                if(_class(index) == BENIGN)
                {
                    positives++;
                    contribution[i][var][0]++;
                }
                else
                {
                    negatives++;
                    contribution[i][var][1]++;
                }
            }
            attribEntropy = entropy(positives,negatives);
            total = positives + negatives;
            //printf("for %d total %d p%d n%d\n",i,total,positives,negatives);
            varEntropy = 0.0;
            for(j=1; j<11; j++)
            {
                positives = contribution[i][j][0];
                negatives = contribution[i][j][1];
                int subtotal = positives + negatives;
                //printf("total %d p%d n%d\n",subtotal,positives,negatives);
                varEntropy += entropy(positives,negatives) * ((double)subtotal/total);
            }
            double gain = attribEntropy - varEntropy;
            Gain.push_back(make_pair(gain,i));
        }
    }
    else
    {
        // calculate gain for all the attributes
        for(i=0; i<9; i++)
        {
            positives = negatives = 0;
            for(j=0; j<dataindex.size(); j++)
            {
                index = dataindex[j];
                var = inputs[i][index];
                if(_class(index) == BENIGN)
                {
                    positives++;
                    contribution[i][var][0]++;
                }
                else
                {
                    negatives++;
                    contribution[i][var][1]++;
                }
            }
            attribEntropy = entropy(positives,negatives);
            total = positives + negatives;
            //printf("for %d total %d p%d n%d\n",i,total,positives,negatives);
            varEntropy = 0.0;
            for(j=1; j<11; j++)
            {
                positives = contribution[i][j][0];
                negatives = contribution[i][j][1];
                int subtotal = positives + negatives;
                //printf("total %d p%d n%d\n",subtotal,positives,negatives);
                varEntropy += entropy(positives,negatives) * ((double)subtotal/total);
            }
            double gain = attribEntropy - varEntropy;
            Gain.push_back(make_pair(gain,i));
        }
    }
}

bool sorter(pair<double,int> P1,pair<double,int> P2)
{
    return P1.first<P2.first;
}
void sort_gain()
{
    sort(Gain.begin(),Gain.end(),sorter);
}

int getMostCommonValue()
{
    int positives,negatives;
    positives = negatives = 0;
    for(int i=0; i<training_index.size(); i++)
    {
        int index = training_index[i];
        if(inputs[9][index] == BENIGN) positives++;
        else negatives++;
    }
    if(positives>negatives) return BENIGN;
    return MALIGNANT;
}

struct TreeNode
{
    bool isAttribute,isVariable;
    int attribute,variable,decision;
    vector <int> dataIndex;
    vector <int> seenAttributes;
    vector <TreeNode*> children;
    int var_contribution[10][11][2];
    TreeNode()
    {
        dataIndex.clear(),children.clear();
        seenAttributes.clear();
        isAttribute = isVariable = false;
        attribute = variable = decision = -1;
        memset(var_contribution,0,sizeof(var_contribution));
    }
    TreeNode(int attrib)
    {
        attribute = attrib;
        dataIndex.clear(),children.clear();
        seenAttributes.clear();
        isAttribute = isVariable = false;
        variable = decision = -1;
        memset(var_contribution,0,sizeof(var_contribution));
    }
};
struct TreeElement
{
    TreeNode *elem;
};

stack <TreeElement> tree;

void ID3(TreeNode &root)
{
    int i,j,positives,negatives;
    root.isAttribute = true;
    gain_calculator(root.dataIndex,root.var_contribution,root.seenAttributes);
    /*for(i=0; i<10; i++)
    {
        for(j=1; j<11; j++)
        {
            for(int k=0; k<2; k++) printf("%d %d %d %d\n",i,j,k,root.var_contribution[i][j][k]);
        }
    }*/
    sort_gain();
    pair <double,int> highest_gain;
    if(Gain.size()>0)
    {
        highest_gain = Gain.back();
        root.attribute = highest_gain.second;
        root.seenAttributes.push_back(root.attribute);
    }
    TreeElement telm;
    telm.elem = &root;
    tree.push(telm);
    while(tree.empty()==false)
    {
        TreeElement treeelm = tree.top();
        tree.pop();
        TreeNode *node = treeelm.elem;
        //printf("%d %d %d\n",node->attribute,node->variable,node->isAttribute);
        if(node->isAttribute)
        {
            if(node->decision > -1) continue;
            for(i=1; i<=10; i++)
            {
                TreeNode child(node->attribute);
                //child.attribute = node->attribute;
                child.isVariable = true;
                child.variable = i;
                for(j=0; j<node->seenAttributes.size(); j++)
                {
                    int val = node->seenAttributes[j];
                    child.seenAttributes.push_back(val);
                }
                positives = node->var_contribution[node->attribute][i][0];
                negatives = node->var_contribution[node->attribute][i][1];
                printf("%d %d\n",positives,negatives);
                int sub_total = positives + negatives;
                if(!sub_total)
                {
                    child.decision = getMostCommonValue();
                }
                else
                {
                    if(!positives)
                    {
                        child.decision = MALIGNANT;
                    }
                    else if(!negatives)
                    {
                        child.decision = BENIGN;
                    }
                    else
                    {

                        for(j=0; j<node->dataIndex.size(); j++)
                        {
                            int index = node->dataIndex[j];
                            if(inputs[child.attribute][index] == i) child.dataIndex.push_back(index);
                        }
                        printf("index size %d\n",child.dataIndex.size());
                    }
                }
                node->children.push_back(&child);
                treeelm.elem = &child;
                tree.push(treeelm);
            }
            printf("size %d\n",node->children.size());
        }
        else if(node->isVariable)
        {
            if(node->decision > -1) continue;
            TreeNode child;
            child.isAttribute = true;
            for(j=0; j<node->seenAttributes.size(); j++)
            {
                int val = node->seenAttributes[j];
                child.seenAttributes.push_back(val);
            }
            for(i=0; i<node->dataIndex.size(); i++)
            {
                int index = node->dataIndex[i];
                child.dataIndex.push_back(index);
            }
            gain_calculator(child.dataIndex,child.var_contribution,child.seenAttributes);
            sort_gain();
            if(Gain.size()>0)
            {
                highest_gain = Gain.back();
                child.attribute = highest_gain.second;
                child.seenAttributes.push_back(child.attribute);
            }
            else
            {
                child.decision = getMostCommonValue();
            }
            node->children.push_back(&child);
            telm.elem = &child;
            tree.push(telm);
        }
    }
}

void print(TreeNode &node)
{
    queue <TreeNode*> q;
    q.push(&node);
    while(q.empty()==false)
    {
        TreeNode *nod = q.front();
        q.pop();
        if(nod->isAttribute)
        {
            printf("attribute %d\n",nod->attribute);
            for(int i=0; i<nod->children.size(); i++)
            {
                q.push(nod->children[i]);
            }
        }
        else
        {
            printf("variable %d attribute %d decision %d\n",nod->variable,nod->attribute,nod->decision);
            if(nod->decision == -1) q.push(nod->children[0]);
        }
    }
}

int main()
{
    freopen("data.txt","r",stdin);
    //freopen("output.txt","w",stdout);
    init();
    TreeNode root;
    for(int i=0; i<training_dataset; i++)
    {
        root.dataIndex.push_back(training_index[i]);
    }
    ID3(root);
    print(root);
    return 0;
}
