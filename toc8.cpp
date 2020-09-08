#include<iostream>
#include<string>
#include<stdlib.h> 
#include <boost/algorithm/string.hpp> 
#include <bits/stdc++.h> 
#include <queue> 
using namespace std;
ofstream fo;

class Mid
{
	public:
		int i1;//stores level of block in the pyramid
		int i2;
		int i3;
		Mid *next;//linked list to store all the non terminals that can be achieved for a particular string length
				  //example  C->A->B->D->A->B->null
				  //this exists for every block in the pyramid 
		Mid *next1;//point to left part of the current string length
		Mid *next2;//point to right part of the current string length
		           //example A->BD   then next1 points to A and next2 points to B 
		//the two variables above exist for every character in a pyramid block
		string state;
		string str;
};

/*Print_func reads data from a queue prints them and stores the child of the nodes accessed to another queue. From flag it is known 
which queue is having data.*/

void print_func(queue<Mid*> myqueue1,queue<Mid*> myqueue2,int flag)//prints the output in form of tree using 2 queues
{
	Mid *p;
	int check=0;//if check becomes 1, there is atleast one element pushed in the empty queue. So, we have to call print_func again.
	int count=0;
	if(flag==1)//flag=1 means queue1 is having data and queue2 is empty.
	{
		fo << "|| ";
		while (!myqueue1.empty())
		{
			p=myqueue1.front();//find the first element from queue
			myqueue1.pop();//pop the element
			if(p->next1!=NULL)
			{
				myqueue2.push(p->next1);//push left child to queue2
				check=1;
			}
			if(p->next2!=NULL)
			{
				myqueue2.push(p->next2);//push right child to queue2
				check=1;
			}
			fo << p->state << " |";
			count++;
			if(count%2==0)//After every two output print '|'.
			{
				fo << "| ";
			}
			else
			{
				fo << " ";
			}
		}
		fo << endl;
		if(check==1)
		{
			print_func(myqueue1,myqueue2,2);//atleast one element pushed in the empty queue. So, we have to call print_func again.
		}
	}
	
	/*if flag = 2, just reverse the functions of queue1 and queue2.*/
	
	if(flag==2)//flag=2 means queue2 is having data and queue1 is empty.
	{
		fo << "|| ";
		while (!myqueue2.empty())
		{
			p=myqueue2.front();
			myqueue2.pop();
			if(p->next1!=NULL)
			{
				myqueue1.push(p->next1);
				check=1;
			}
			if(p->next2!=NULL)
			{
				myqueue1.push(p->next2);
				check=1;
			}
			fo << p->state << " |";
			count++;
			if(count%2==0)
			{
				fo << "| ";
			}
			else
			{
				fo << " ";
			}
		}
		fo << endl;
		if(check==1)
		{
			print_func(myqueue1,myqueue2,1);
		}
	}
}


int main()
{
	int step_num,i,i_size,j,k,n,p,file_count=1;//file_count is used to track the loop no(actually input set number)
	string start,input;
	
	
		/*queue1 and queue2 are used for printing in tree fashion*/
		fstream fio;
		fio.open("input.txt",ios::in);//input file
		fo.open("output.txt",ios::in);//output file
		queue<Mid*> myqueue1;
		queue<Mid*> myqueue2;
		
		
		
	while(getline(fio, start))//get name of starting non terminal
	{	
		
		fo << "For input set " << file_count << endl;
		file_count++;
		string sn_temp;
		getline(fio, sn_temp);//get number of non terminals
		step_num=stoi(sn_temp);//convert number of non terminals from string to integer
		string step[step_num];
		vector<string> word[step_num];//split every transition 
   									  // eg S -> AB | BC  stored as [S AB BC]  

		for(i=0;i<step_num;i++)
		{
			getline(fio,step[i]); //get transition for every non terminal
		}
		getline(fio,input);//input string

		for(i=0;i<step_num;i++)
		{
			boost::split(word[i], step[i], boost::is_any_of(" "));//slpit every transition and store in word
		}
		i_size=input.size();
		char input_array[i_size+1]; 
		strcpy(input_array, input.c_str()); //store input sttring in character array 
		vector<string> temp_tree;
		for(i=0;i<i_size;i++)
		{
			string str="";
			str=str+input_array[i];//get character from input
			temp_tree.push_back(str);//put character from input in the vector
		}
		vector<Mid*> tree[i_size];
		
		for(i=0;i<i_size;i++)//runs for every input character
		{
			Mid *ttt=NULL;
			tree[0].push_back(ttt);//initially put NULL pointer for the input character 
			int cc=0;
			for(j=0;j<step_num;j++)//traverse through every non terminal
			{
			
				for(k=0;k<word[j].size();k++)//for the current non terminal , traverse through every possible traversal
				{
					if(word[j][k].compare(temp_tree[i])==0)//if the traversal becomes equal to the currently required traversal (eg AB or in this case a terminal like 'a')
					{
						
						Mid *tt=new Mid();
						
						tt->i1=0;//stores level of block in the pyramid(in this case bottommost i.e 0)
						
						tt->i2=i;//length of the left half of string 
						tt->i3=cc;//length of the right half of string
						cc+=1;
						
						tt->next=NULL;
						
						tt->next2=NULL;
						
						tt->next1=NULL;
						tt->str="" + word[j][0];//stores current non terminal(eg A or B)
						tt->state="" + word[j][0] + " -> " + word [j][k];//stores the transition eg S -> A
						if(tree[0][i]!=NULL)//if there is already such transition 
						{
							tt->next=tree[0][i];//insert in the list
						}
						tree[0][i]=tt;
						
						break;//we got a transition in current non terminal(S -> AB | BC),so no need to traverse it further 
					}
				
				}
			
			}
		}

		for(i=1;i<i_size;i++)//work for every pyramid level
		{
		
		
			for(j=0;j<i_size-i;j++)//since we take strings + 1 togeter at every level , number of blocks for each level decreases by 1
			{
				Mid *ttt=NULL;
				
				tree[i].push_back(ttt);//initialize
				k=0;//to traverse pyramid levels below the current level
				int l=i-1;//index to refer to right half of the string
				int m=j+1;
				int cc=0;
				while(k<i)//check for all pyramid levels below the current
				{
					Mid *obj1=tree[k][j];//get left half of transition
					Mid *obj2=tree[l][m];//get right half of transition
					//similar to 2 nested loops(comoare every element of 1st loop with every element of second loop)
					while(obj1!=NULL)
					{
						Mid *obj3=obj2;
						while(obj3!=NULL)
						{
							string s_temp="" +obj1->str +obj3->str;//form the non terminal transition(eg AB  , BC)
							for(int jj=0;jj<step_num;jj++)//traverse through every non terminal
							{
								for(int kk=0;kk<word[jj].size();kk++)//for the current non terminal , traverse through every possible traversal
								{
									if(word[jj][kk].compare(s_temp)==0)//if the traversal becomes equal to the currently required traversal (eg AB)
									{
										Mid *tt=new Mid();
						
										tt->i1=i;//stores level of block in the pyramid
										tt->i2=j;//length of the left half of string 
										tt->i3=cc;//length of the right half of string
										cc+=1;
										tt->next=NULL;
										tt->next1=obj1;//stores pointer to left half of transition
										tt->next2=obj3;//stores pointer to right half of transition
										
										
										tt->str="" + word[jj][0];//stores current non terminal(eg A or B)
										tt->state="" + word[jj][0] + " -> " + word [jj][kk];//stores the transition eg S -> A
										if(tree[i][j]!=NULL)//if there is already such transition 
										{
											tt->next=tree[i][j];//insert in the list
										}
										tree[i][j]=tt;
										break;
									}
								}
							}
							obj3=obj3->next;//similar to loop updation
						}
						obj1=obj1->next;//similar to loop updation
					}
					k++;
					m++;
					l--;
				
				}
			
			}
		}
		//loop to print the pyramid stored in vector tree
		for(int iii=i_size-1;iii>=0;iii--)//for rows
		{
			for(int jjj=0;jjj<tree[iii].size();jjj++)//for columns
			{	
				Mid *obj=tree[iii][jjj];
				int flag=0;
				while(obj!=NULL)	
				{	
					fo << obj->str;//print the onject stored in iii,jjj
					flag=1;//flag=1 means some string is there in the particular space
		   			obj=obj->next;
				}
				if(flag==0)//if nothing is stored print a null character('@')
				{
					fo <<"@";
				}
				fo <<" ";
			}
			fo << "\n";
		}
		fo << "----------------------------------"<<endl;
		fo << "----------------------------------"<<endl;
		int tree_count=0;
		Mid *obj=tree[i_size-1][0];
		while(obj!=NULL)
		{
			if(obj->str.compare(start)==0)//if start is there in the topmost row and column then solution is possible
			{
				tree_count++;//count how many trees are possible
				fo << "|| " << obj->state << " || " <<endl;
				myqueue1.push(obj->next1);//push left child to myqueue1
				myqueue1.push(obj->next2);//push right child to my queue1
				print_func(myqueue1,myqueue2,1);//1 means data stored in myqueue1
				fo << "----------------------------------"<<endl;
				fo << "----------------------------------"<<endl;
			}
			obj=obj->next;
		
		}
		fo << "Total trees possible: " << tree_count << endl;
		
		fo << "********************************************************************************************"<<endl;
		fo << endl;
		fo << endl;
		fo << endl;		
    }
    fo.close();
	fio.close();
    return 0;
}




