//
//  RadixTree.h
//  Project10
//
//  Created by Henry Guo on 3/5/22.
//

#ifndef RadixTree_h
#define RadixTree_h

#include <stdio.h>
#include <iostream>
#include <list>
#include <cctype>
#include <ctype.h>
using namespace std;

template <typename ValueType>
class RadixTree {
public:
 RadixTree();
 ~RadixTree();
 void insert(std::string key, const ValueType& value);
 ValueType* search(std::string key) const;
private:
struct RadixTreeNode{
    string word;
    bool endOfWord;
    ValueType item;
    list<int> occupied;
    RadixTreeNode* edges[63];
};
    RadixTreeNode* root;
    int match(string s1, string s2) const
    {
        int index = -1;
        for(int i = 0; i<s2.size()&&i<s1.size();i++)
        {
            if(s1[i] != s2[i])
            {
                break;
            }
            //cout<<s1[i]<<" is matched with "<<s2[i]<<endl;
            index = i;
        }
        return index;
    }
    
    void cleanUp(RadixTreeNode* p)
    {
        if(p->occupied.empty())
        {
            //cout<<p->word<<endl;
            delete p;
            return;
        }
        else
        {
            list<int>::iterator it = p->occupied.begin();
            for(;it!=p->occupied.end();it++)
            {
                //cout<<"going to clean up index "<<*it<<endl;
                //cout<<"going to clean up "<<p->word<<" ";
                cleanUp(p->edges[*it]);
            }
            //cout<<"deleted1 "<<p->word<<" "<<p->item<<endl;
            delete p;
            return;
        }
    }
};

template<typename ValueType> RadixTree<ValueType>::RadixTree()
{
    root = new RadixTreeNode;
    for(int i = 0;i<26;i++)
    {
        root->edges[i] = new RadixTreeNode;
        root->edges[i]->word = "";
        root->edges[i]->word += 'a'+i;
        //cout<<"word is1 "<<root->edges[i]->word<<endl;
        root->occupied.push_back(i);
    }
    for(int j = 0;j<10;j++)
    {
        root->edges[j+26] = new RadixTreeNode;
        root->edges[j+26]->word = "";
        root->edges[j+26]->word += '0'+j;
        //cout<<"word is2 "<<root->edges[j+26]->word<<endl;
        root->occupied.push_back(j+26);
    }
    for(int k = 0;k<26;k++)
    {
        root->edges[k+36] = new RadixTreeNode;
        root->edges[k+36]->word = "";
        root->edges[k+36]->word += 'A'+k;
        root->occupied.push_back(k+36);
        //cout<<"word is3 "<<root->edges[k+36]->word<<endl;
    }
    root->edges[62] = new RadixTreeNode;
    root->edges[62]->word = "";
    root->edges[62]->word += '@';
    root->occupied.push_back(62);
}

template<typename ValueType> RadixTree<ValueType>::~RadixTree()
{
    cleanUp(root);
}

template<typename ValueType>void RadixTree<ValueType>::insert(std::string key, const ValueType &value)
{
    int index;
    if(isalpha(key[0]))
    {
        if(key[0]==tolower(key[0]))
        {
            index = key[0]-'a';
        }
        else
        {
            index = key[0]-'A'+36;
        }
    }
    else
    {
        if(key[0]=='@')
        {
            index = 62;
        }
        else
        {
            index = key[0] -'0'+26;
        }
    }
    RadixTreeNode* p = root->edges[index];
    RadixTreeNode* last = root;
    //cout<<"going to insert "<<key<<endl;
    int judge = -1;
    bool occupy = false;
    bool clear = false;
    while(key.size()>0)
    {
        //cout<<"going to match "<<key<<" and "<<p->word<<endl;
        judge = match(key,p->word);
        //cout<<"current word is "<<p->word<<" match is "<<judge<<endl;
        if(judge==p->word.size()-1)
        {
            clear = true;
            //cout<<"matched! "<<key<<" and "<<p->word<<endl;
            occupy = false;
            key = key.substr(p->word.size());
            if(isalpha(key[0]))
            {
                if(key[0]==tolower(key[0]))
                {
                    index = key[0]-'a';
                }
                else
                {
                    index = key[0]-'A'+36;
                }
            }
            else
            {
                //cout<<"key here is "<<key<<endl;
                if(key[0]=='@')
                {
                    index = 62;
                }
                else
                {
                    index = key[0] -'0'+26;
                }
            }
            //cout<<"index is "<<index<<endl;
            list<int>::iterator it = p->occupied.begin();
            for(;it!=p->occupied.end();it++)
            {
                //cout<<*it<<" is occupied"<<endl;
                if((*it)==index)
                {
                    occupy = true;
                    break;
                }
            }
            if(occupy==false)
            {
                //cout<<"not occupied"<<endl;
                break;
            }
            else
            {
                //cout<<"advance!"<<endl;
                last = p;
                p = p->edges[index];
            }
        }
        else
        {
            clear = false;
            //cout<<"match fail"<<endl;
            break;
        }
    }
    //cout<<"now judge is "<<judge<<" and key is "<<key<<endl;
    string word = p->word;
    //cout<<"occupy is "<<occupy<<" clear is "<<clear<<endl;
    //cout<<"p->word is "<<word<<endl;
    if(key.size()==0)
    {
        //cout<<"case 0"<<endl;
        p->endOfWord = true;
        p->item = value;
        return;
    }
    else if(occupy==false&&clear==true)
    {
        //cout<<"case 1"<<endl;
        RadixTreeNode* nxt = new RadixTreeNode;
        if(isalpha(key[0]))
        {
            if(key[0]==tolower(key[0]))
            {
                index = key[0]-'a';
            }
            else
            {
                index = key[0]-'A'+36;
            }
        }
        else
        {
            if(key[0]=='@')
            {
                index = 62;
            }
            else
            {
                index = key[0] -'0'+26;
            }
        }
        //cout<<key<<" and index is "<<index<<endl;
        nxt->word = key;
        nxt->item = value;
        nxt->endOfWord = true;
        p->edges[index] = nxt;
        p->occupied.push_back(index);
        /*
        list<int>::iterator it = p->occupied.begin();
        for(;it!=p->occupied.end();it++)
        {
            cout<<*it<<endl;
        }*/
        return;
    }
    else if(judge!=key.size()-1&&judge!=p->word.size()-1)
    {
        string common = key.substr(0,judge+1);
        key = key.substr(judge+1);
        //cout<<"now common is "<<common<<" and key is "<<key<<endl;
        int index1;
        if(isalpha(key[0]))
        {
            if(key[0]==tolower(key[0]))
            {
                index1 = key[0]-'a';
            }
            else
            {
                index1 = key[0]-'A'+36;
            }
        }
        else
        {
            if(key[0]=='@')
            {
                index1 = 62;
            }
            else
            {
                index1 = key[0] -'0'+26;
            }
        }/*
        list<int>::iterator it = p->occupied.begin();
        bool check = false;
        for(;it!=p->occupied.end();it++)
        {
            if((*it)==index1)
            {
                check = true;
                break;
            }
        }*/
        if(p->occupied.empty())
        {
            //cout<<"case 3A: no conflict"<<endl;
            string back = p->word.substr(judge+1);
            //cout<<"back is "<<back<<endl;
            //ValueType old_item(p->item);
            p->word = common;
            RadixTreeNode* nxt = new RadixTreeNode;
            //cout<<key<<" and "<<back<<endl;
            if(isalpha(back[0]))
            {
                if(back[0]==tolower(back[0]))
                {
                    index = back[0]-'a';
                }
                else
                {
                    index = back[0]-'A'+36;
                }
            }
            else
            {
                if(back[0]=='@')
                {
                    index = 62;
                }
                else
                {
                    index = back[0] -'0'+26;
                }
            }
            //index = back[0]-'a';
            nxt->word = back;
            nxt->item = p->item;
            p->endOfWord = false;
            nxt->endOfWord = true;
            p->edges[index] = nxt;
            p->occupied.push_back(index);
            RadixTreeNode* ptr = new RadixTreeNode;
            ptr->word = key;
            ptr->item = value;
            ptr->endOfWord = true;
            p->edges[index1] = ptr;
            p->occupied.push_back(index1);
            //cout<<"nxt will be called "<<back<<" and ptr will be called "<<key<<endl;
            return;
        }
        else
        {
            p->word = p->word.substr(judge+1);
            RadixTreeNode* medium = new RadixTreeNode;
            medium->endOfWord = false;
            medium->word = common;
            RadixTreeNode* ptr = new RadixTreeNode;
            ptr->item = value;
            ptr->word = key;
            ptr->endOfWord = true;
            int index2;
            if(isalpha(ptr->word[0]))
            {
                if(ptr->word[0]==tolower(ptr->word[0]))
                {
                    index2 = ptr->word[0]-'a';
                }
                else
                {
                    index2 = ptr->word[0]-'A'+36;
                }
            }
            else
            {
                if(ptr->word[0]=='@')
                {
                    index2 = 62;
                }
                else
                {
                    index2 = ptr->word[0] -'0'+26;
                }
            }
            int index3;
            if(isalpha(p->word[0]))
            {
                if(p->word[0]==tolower(p->word[0]))
                {
                    index3 = p->word[0]-'a';
                }
                else
                {
                    index3 = p->word[0]-'A'+36;
                }
            }
            else
            {
                if(p->word[0]=='@')
                {
                    index3 = 62;
                }
                else
                {
                    index3 = p->word[0] -'0'+26;
                }
            }
            medium->occupied.push_back(index2);
            medium->occupied.push_back(index3);
            medium->edges[index2] = ptr;
            medium->edges[index3] = p;
            int index4;
            if(isalpha(medium->word[0]))
            {
                if(medium->word[0]==tolower(medium->word[0]))
                {
                    index4 = medium->word[0]-'a';
                }
                else
                {
                    index4 = medium->word[0]-'A'+36;
                }
            }
            else
            {
                if(medium->word[0]=='@')
                {
                    index4 = 62;
                }
                else
                {
                    index4 = medium->word[0] -'0'+26;
                }
            }
            last->edges[index4] = medium;
            /*
                string rest1 = p->word.substr(judge+1);
                string rest2 = p->edges[index1]->word;
                ValueType old_value = p->item;
                RadixTreeNode* ptr1 = new RadixTreeNode;
                ptr1->item = old_value;
                ptr1->word = rest1;
                ptr1->endOfWord = true;
                int index3 = rest1[0]-'a';
                p->occupied.push_back(index3);
                p->edges[index3] = ptr1;
                RadixTreeNode* ptr2 = new RadixTreeNode;
                ptr2->item = p->edges[index1]->item;
                ptr2->word = rest2;
                ptr2->endOfWord = true;
                ptr1->edges[index1] = ptr2;
                ptr1->occupied.push_back(index1);
            p->word = p->word.substr(0,judge+1);
            p->edges[index1]->item = value;*/
            
        }
    }
    else if(clear==false&&judge<p->word.size())
    {
        //cout<<"case 2"<<endl;
        string front = p->word.substr(0,judge+1);
        string back = p->word.substr(judge+1);
        ValueType old_item = p->item;
        p->endOfWord = true;
        p->word = front;
        p->item = value;
        RadixTreeNode* nxt = new RadixTreeNode;
        //cout<<key.substr(judge)<<endl;
        //index = back[0]-'a';
        if(isalpha(back[0]))
        {
            if(back[0]==tolower(back[0]))
            {
                index = back[0]-'a';
            }
            else
            {
                index = back[0]-'A'+36;
            }
        }
        else
        {
            if(back[0]=='@')
            {
                index = 62;
            }
            else
            {
                index = back[0] -'0'+26;
            }
        }
        nxt->word = back;
        nxt->item = old_item;
        nxt->endOfWord = true;
        p->edges[index] = nxt;
        p->occupied.push_back(index);
        return;
        
    }
}

template<typename ValueType>ValueType* RadixTree<ValueType>::search(std::string key) const
{
    int index;
    if(isalpha(key[0]))
    {
        if(key[0]==tolower(key[0]))
        {
            index = key[0]-'a';
        }
        else
        {
            index = key[0]-'A'+36;
        }
    }
    else
    {
        if(key[0]=='@')
        {
            index = 62;
        }
        else
        {
            index = key[0] -'0'+26;
        }
    }
    //cout<<"search: first index is "<<index<<endl;
    RadixTreeNode* p = root->edges[index];
    int judge;
    bool occupy = false;
    while(key.size()>0)
    {
        occupy = false;
        judge = match(key,p->word);
        //cout<<"current word is "<<p->word<<" match is "<<judge<<" and item is "<<p->item<<endl;
        if(judge==p->word.size()-1)
        {
            key = key.substr(p->word.size());
            //cout<<"after search cut, key is "<<key<<endl;
            if(key.size()==0)
            {
                //cout<<"find!"<<endl;
                return &p->item;
            }
            //index = key[0]-'a';
            if(isalpha(key[0]))
            {
                if(key[0]==tolower(key[0]))
                {
                    index = key[0]-'a';
                }
                else
                {
                    index = key[0]-'A'+36;
                }
            }
            else
            {
                if(key[0]=='@')
                {
                    index = 62;
                }
                else
                {
                    index = key[0] -'0'+26;
                }
            }
            list<int>::iterator it = p->occupied.begin();
            for(;it!=p->occupied.end();it++)
            {
                if((*it)==index)
                {
                    occupy = true;
                    break;
                }
            }
            //cout<<"stored index is "<<*it<<" and index is "<<index<<endl;
            /*
            if((*it)==index)
            {
                occupy = true;
            }*/
            if(occupy==false)
            {
                //cout<<"dont find! 1"<<endl;
                return nullptr;
            }
            else
            {
                p = p->edges[index];
            }
        }
        else
        {
            //cout<<"dont find! 2"<<endl;
            return nullptr;
        }
    }
    if(key.size()==0)
    {
        //cout<<"find!"<<endl;
        return &p->item;
    }
    //cout<<"dont find! 3"<<endl;
    return nullptr;
}

#endif /* RadixTree_h */

