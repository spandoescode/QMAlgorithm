#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct node
{
    int data[257],bin[26],setbits,isimplicant,minarr[1000];
    char term[26];
    struct node* next;
};

struct node *root,*head,*improot,*save,*fin;
int var,min,number=1,columns=2,check=1,limit,imptable[100][100],counter=0,essential[1000],t=0,no=0,minterms[1000];

char a[26],b[26];                                 //variable names

void group1();                                    //first group of minterms
void sort();                                      //ordered list of minterms
void swap(struct node*,struct node*);             //data of two nodes is swapped
void print();                                     //print data
void nextgrp();                                   //the minterms are further paired
void end_loop(struct node*);                      //the extra node in a list is deleted
void display_implicants();                        //the implicants are displayed
void implicants(struct node*);                    //initializes each term as an implicant
void collect();                                   //converts the term from binary notation to variables
void variables();                                 //the variables for the function are stored
void convert();                                   //reduces the prime implicants which occur more than once to one
void implicants_table();                          //the prime implicants table is formed and essential implicants are found
void printfunc();                                 //the minimized function is displayed
void other_implicants();                          //the prime implicants other than the essential ones are collected
void final_terms();                               //the final terms in the minimized function are noted
void store_minterms();                            //minterms are stored in an array

int main()
{
    double te = 0.0;
    clock_t start = clock();
    int i,j,k,x;
    struct node* temp;

    inputs:

    printf("\nEnter the number of variables : ");       //no. of variables and minterms are recorded
    scanf("%d",&var);
    printf("\nEnter the number of minterms : ");
    scanf("%d",&min);

    if(min > pow(2,var))
    {
        puts("Please enter valid inputs again.");
        goto inputs;
    }

    i=min-1;

    root=temp=(struct node*)malloc(sizeof(struct node));

    printf("\nEnter the minterms one by one\n\n");
    scanf("%d",&temp->data[0]);                     //first minterm is stored
    j=temp->data[0];
    temp->setbits=0;
    x=var;
    k=0;

    while(x--)      //converts minterm to binary notation
    {
        if(j%2==1)
        {
            temp->bin[k]=1;
            temp->setbits++;
        }
        else
        {
            temp->bin[k]=0;
        }
        j=j/2;
        k++;
    }

    while(i--)      //rest of the minterms are stored
    {
        temp=temp->next=(struct node*)malloc(sizeof(struct node));

        mt:

        scanf("%d",&temp->data[0]);

        if(temp->data[0] >= pow(2,var))
        {
            puts("Please enter a valid minterm");
            goto mt;
        }
        j=temp->data[0];
        temp->setbits=0;
        x=var;
        k=0;

        while(x--)
        {
            if(j%2==1)          //converts the minterms to binary notation
            {
                temp->bin[k]=1;
                temp->setbits++;       //the number of ones in binary notation
            }
            else
            {
                temp->bin[k]=0;
            }
            j=j/2;
            k++;
        }
    }

    temp->next=NULL;
    sort();      
    store_minterms();
    group1();
    print();
    end_loop(root);
    head=(struct node*)malloc(sizeof(struct node));

    while(check>0)
    {
        nextgrp();
    }

    save->next=NULL;           //storing null value in link field of list storing prime implicants
    printf("No pairs formed hence no further calculation required\n\n");

    end_loop(improot);
    collect();
    display_implicants();
    variables();

    other_implicants();
    final_terms();
    end_loop(fin);
    convert();


    clock_t end = clock();
    te = te + (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time elapsed is %f seconds. \n",te);
    return 0;
}

void sort()          
{
    struct node *temp1,*temp2;
    temp1=temp2=root;
    while(temp1!=NULL)
    {
        temp2=root;
        while(temp2!=NULL)
        {
            if(temp1->data[0]<temp2->data[0])       //if not in order their values are exchanged with swap function
            {
                swap(temp1,temp2);
            }
            temp2=temp2->next;
        }
        if(temp1->next==NULL)
        {
            limit=temp1->data[0];           //the magnitude of the last minterm is recorded later for prime implicants table
        }
        temp1=temp1->next;
    }
}

void store_minterms()       //array of minterms
{
    int i=0;
    struct node* temp;
    temp=root;

    while(temp!=NULL)
    {
        minterms[i]=temp->data[0];
        i++;
        temp=temp->next;
    }
}

void swap(struct node* temp1,struct node* temp2)      //Swap two nodes   
{
    int x,y,i=0;
    i=var;

    for(i=0;i<var;i++)      
    {
        y=temp1->bin[i];
        temp1->bin[i]=temp2->bin[i];
        temp2->bin[i]=y;
    }

    y=temp1->setbits;          
    temp1->setbits=temp2->setbits;
    temp2->setbits=y;
    x=temp1->data[0];           
    temp1->data[0]=temp2->data[0];
    temp2->data[0]=x;
}

void group1()       //Sort minterms according to setbits
{
    int i,j,k=0;
    struct node *temp,*next;
    temp=save=root;
    root=next=(struct node*)malloc(sizeof(struct node));

    for(i=0;i<=var;i++)
    {
        temp=save;

        while(temp!=NULL)
        {
            if(temp->setbits==i)       //minterms are arranged according to no. of ones , first 0 ones then 1 ones... and so on
            {
                next->data[0]=temp->data[0];
                k++;

                for(j=0;j<var;j++)
                {
                    next->bin[j]=temp->bin[j];
                }
                next->setbits=temp->setbits;
                next=next->next=(struct node*)malloc(sizeof(struct node));
            }
            temp=temp->next;
        }
    }
    minterms[k]=-1;
    next->next=NULL;
}

void print()     //print data
{

    int i,j=min;

    struct node* temp;

    temp=root;

    printf("\n\nColumn #%d\n\n\n",number);          //number tells us which column is being printed

    while(temp->next!=NULL)

    {

        printf("%d\t",temp->data[0]);

        for(i=var-1;i>=0;i--)

        {

            printf("%d",temp->bin[i]);

        }

        temp=temp->next;

        printf("\n");

    }

    temp->next=NULL;

    number++;

}

void end_loop(struct node* ptr)         //ending the list
{
    struct node* temp;
    temp=ptr;

    while(temp->next->next!=NULL)
    {
        temp=temp->next;
    }
    temp->next=NULL;
}

void nextgrp()    //grouping based on difference in binary notation
{
    int i,count,k,j,x;
    struct node *temp,*next,*p,*imp;
    check=0;

    if(columns==2)      //for second column
    {
        imp=improot=(struct node*)malloc(sizeof(struct node));
        p=head;
    }
    else        //for other columns
    {
        imp=save;
        root=head;
        p=head=(struct node*)malloc(sizeof(struct node));
    }

    temp=root;
    implicants(root);
    printf("\n\nColumn #%d\n\n\n",number);

    while(temp!=NULL)
    {
        next=temp->next;
        while(next!=NULL)
        {
            count=0;
            if(next->setbits-temp->setbits==1)        //if two terms differ in their no. of ones by one
            {
                for(i=0;i<var;i++)
                {
                    if(temp->bin[i]!=next->bin[i])
                    {
                        k=i;            //the place in which they differ is noted
                        count++;
                    }
                }
            }
            if(count==1)        //checks if the two terms differ by one place in binary notation
            {
                temp->isimplicant=0;        //if they do then they are not a prime implicant
                next->isimplicant=0;
                check++;
                for(i=0;i<var;i++)
                {
                    p->bin[i]=temp->bin[i];         //binary notation is stored
                }
                p->bin[k]=-1;
                x=0;
                for(j=0;j<columns/2;j++)            //data from first term is stored
                {
                    p->data[x]=temp->data[j];
                    x++;
                }
                for(j=0;j<columns/2;j++)            //data from second term is stored
                {
                    p->data[x]=next->data[j];
                    x++;
                }
                p->setbits=temp->setbits;
                for(j=0;j<columns;j++)      //the pair formed is displayed
                {
                    printf("%d,",p->data[j]);
                }

                printf("\b ");
                printf("\t");
                for(i=var-1;i>=0;i--)
                {
                    if(p->bin[i]==-1)
                        printf("-");
                    else
                        printf("%d",p->bin[i]);
                }

                printf("\n");
                p=p->next=(struct node*)malloc(sizeof(struct node));           // one extra node that is to be deleted
            }
            next=next->next;
        }
        temp=temp->next;
    }

    p->next=NULL;
    if(check!=0)

    {
        end_loop(head);     //extra node is deleted
    }

    temp=root;
    while(temp!=NULL)           //for selecting the prime implicants
    {
        if(temp->isimplicant==1)        // if term is a prime implicant it is stored separately in list with head pointer improot
        {
            i=0;
            for(i=0;i<columns/2;i++)
            {
                imp->data[i]=temp->data[i];
            }
            imp->data[i]=-1;
            for(i=0;i<var;i++)
            {
                imp->bin[i]=temp->bin[i];
            }
            imp=imp->next=(struct node*)malloc(sizeof(struct node));
        }
        temp=temp->next;
    }
    save=imp;
    columns=columns*2;
    number++;
}

void display_implicants()       //displays the implicants
{
    int i=0;
    struct node* temp;
    temp=improot;
    printf("\n\nThe prime implicants are:- \n\n");
    while(temp!=NULL)
    {
        i=0;
        i=var-1;
        while(i>=0)     //displays the binary notation
        {
            if(temp->bin[i]==-1)
            {
                printf("-");
            }
            else
            {
                printf("%d",temp->bin[i]);
            }
            i--;
        }
        printf("\t\t");
        i=0;

        while(temp->data[i]!=-1)        //displays the minterm pairs
        {
            printf("%d,",temp->data[i]);
            i++;
        }
        printf("\b ");
        temp=temp->next;
        printf("\n\n");
        counter++;
    }
}

void implicants(struct node* ptr)       //initializing each term as a prime implicant
{
    struct node* temp;
    temp=ptr;

    while(temp!=NULL)
    {
        temp->isimplicant=1;
        temp=temp->next;
    }
}

void collect()          //reduces the terms that occur more than once to a single
{

    int common=0,i;

    struct node *temp1,*temp2,*temp3;

    temp1=temp2=improot;

    while(temp1!=NULL)

    {

        temp2=temp1->next;

        while(temp2!=NULL)

        {

            common=0;

            for(i=0;i<var;i++)          //if their binary notation is same one will be deleted

            {

                if(temp2->bin[i]==temp1->bin[i])

                {

                    common++;

                }

            }

            if(common==var)

            {

                temp3=improot;

                while(temp3->next!=temp2)      //the repeated term is deleted

                {

                    temp3=temp3->next;

                }

                temp3->next=temp2->next;

                temp2=temp3;

            }

            temp2=temp2->next;

        }

        temp1=temp1->next;

    }

}

void variables()            //stores variables(alphabets)
{

    int i;

    for(i=0;i<26;i++)

    {

        a[i]=65+(var-i-1);      //variables

        b[i]=97+(var-i-1);      //their compliments

    }

}

void convert()          //it converts the binary notation of each term to variables
{

    int i,j;

    struct node* temp;

    temp=fin;

    while(temp!=NULL)

    {

        j=0;

        for(i=0;i<var;i++)

        {

            if(temp->bin[i]==0)

            {

                temp->term[j]=b[i];

                j++;

            }

            if(temp->bin[i]==1)

            {

                temp->term[j]=a[i];

                j++;

            }

        }

        temp=temp->next;

    }

}

void other_implicants()     //after finding the essential prime implicants other terms necessary are marked
{
    no=0;           //to check if any term is found in each iteration
    int count1=0,count2=0;
    int i,j;

    for(i=0;i<counter;i++)
    {
        count1=0;
        for(j=0;j<=limit;j++)
        {
            if(imptable[i][j]==1)       //no. of X's or 1's are calculated
            {
                no++;
                count1++;
            }
        }

        if(count1>count2)       //to find the term with maximum X's in a row
        {
            essential[t]=i;
            count2=count1;
        }
    }

    for(j=0;j<=limit;j++)           //removing the X's in the row as well a those X's which are in same column
    {
        if(imptable[essential[t]][j]==1)
        {
            for(i=0;i<counter;i++)
            {
                imptable[i][j]=0;
            }
        }
    }
    t++;
    essential[t]=-1;
    if(no>0)            //if one or more terms is found the function is called again otherwise not
    {
        other_implicants();
    }
}

void final_terms()          //in this function all the terms in the minimized expression are stored in a linked list
{
    int i=0,j,c=0,x;
    struct node *temp,*ptr;
    fin=temp=(struct node*)malloc(sizeof(struct node));
    while(essential[i]!=-1)
    {
        ptr=improot;
        x=essential[i];
        for(j=0;j<x;j++)        //so that pointer points to the node whose index was stored in array named essential
        {
            ptr=ptr->next;
        }
        j=0;
        while(ptr->data[j]!=-1)         // the data of the node is stored
        {
            temp->data[j]=ptr->data[j];
            j++;
        }
        temp->data[j]=-1;
        for(j=0;j<var;j++)          //the binary code is stored
        {
            temp->bin[j]=ptr->bin[j];
        }
        temp=temp->next=(struct node*)malloc(sizeof(struct node));
        i++;
        c++;
    }
    temp->next=NULL;
}