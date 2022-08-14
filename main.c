/* COP 3502C Assignment 1This program is written by: Ricardo Ferreira Carvalheira */
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "leak_detector_c.h"

#define MAX_TEST_CASES 25
#define MAX_TOTAL_TREES 8
#define ZERO 0
#define POSITIVE_LIMIT 10000
#define NEGATIVE_LIMIT -10000

int *usedIdx, *coordXOfTree, *coordYOfTree, *permutation;

FILE *input;

double distance;

float calcDistance(int indexTreeOne, int indexTreeTwo)
{ //calculates the distance of that permutation
    return sqrt(pow(coordXOfTree[indexTreeOne] - coordXOfTree[indexTreeTwo], 2) + pow(coordYOfTree[indexTreeOne] - coordYOfTree[indexTreeTwo],2));
}

void createPermutation(int perm[],int index, int numOfTrees,int usedIdx[])
{
  if(index==numOfTrees)
  {
      float tempMinDistance = 0;
      for(int i=0;i<numOfTrees;i+=2)
      {

        tempMinDistance += calcDistance(perm[i], perm[i+1]);
      }
      if (distance > tempMinDistance)
      {
        distance = tempMinDistance; //Distance is a global variable. So, its value can be accessed in from the main
      }
      return;
    } 

    if(index%2==0)
        {
          for(int i=(perm[index-2]+1);i<numOfTrees;i++)
          {
              if(usedIdx[i]) continue;
              perm[index]=i;
              usedIdx[i]=1;
              createPermutation(perm, index+1,numOfTrees,usedIdx);
              usedIdx[i]=0;
          }
        }
        else
        {
          for(int i=(perm[index-1]+1);i<numOfTrees;i++)
          {
              if(usedIdx[i]) continue;
              perm[index]=i;
              usedIdx[i]=1;
              createPermutation(perm, index+1,numOfTrees,usedIdx);
              usedIdx[i]=0;
          }
        }
}

void getPermOfTestCase(int numOfTrees)
{   
    for(int i = 1; i < numOfTrees; i++)
    {
        permutation[1] = i;
        usedIdx[i] = 1; //Set current index to 1. So, it wont be used in the next call

        createPermutation(permutation,2, numOfTrees,usedIdx);
        usedIdx[i] = 0; //Free the index so we can use it again
    }
}

int main()
{
    atexit(report_mem_leak);
    input = fopen("in.txt", "r");
    if(input!=NULL) //If the input file is different than null, start the code.
	  	{

        int numTestCases, numPairsOfTrees, numOfTrees;
        fscanf(input,"%d", &numTestCases);

        if((numTestCases>MAX_TEST_CASES)||(numTestCases<ZERO)) //If test cases is out of limits, exit code.
        {
          fclose(input);
          printf("Test Cases should be a positive integer c (c<=25). Your current number of test cases is %d.\n", numTestCases);
          exit(1);
        }

        for(int i = 0; i < numTestCases; i++)
        {   
            distance = FLT_MAX; //In each test case in the for loop, we set the distance to the highest float value possible because the distance will be lower than that.
            fscanf(input,"%d", &numPairsOfTrees); //scan the pair of trees in this test case
            if((numPairsOfTrees>MAX_TOTAL_TREES) || (numPairsOfTrees<ZERO))
            { //If the number of pair of trees in the backyeard is out of limits, we exit the code
              fclose(input);
              printf("The number of your backyard trees should be a positive integer n (n<=8). Your current number of pairs of trees in the backyard is %d.\n", numPairsOfTrees);
              exit(1);
            }
            numOfTrees=numPairsOfTrees*2; //Each pair has two trees. So, we have to double that number to find the number of trees.
            usedIdx = malloc(numOfTrees*sizeof(int)); //Used array to check if combination was used already
            for(int j = 1; j < numOfTrees; j++) //Creates an array of Index to keep track of used/unused index
            {
                usedIdx[j]=0;
            }
            coordXOfTree = malloc(numOfTrees*sizeof(int)); //Allocate memory for the array of x location of each tree
            coordYOfTree = malloc(numOfTrees*sizeof(int));//Allocate memory for the array of y location of each tree

            for(int j = 0; j < numOfTrees; j++)
            { //Reads the coordinates x and y of each tree 
                
                fscanf(input,"%d %d", &coordXOfTree[j], &coordYOfTree[j]);
                
                if((coordXOfTree[j]>POSITIVE_LIMIT)||(coordXOfTree[j]<NEGATIVE_LIMIT) || (coordYOfTree[j]>POSITIVE_LIMIT)||(coordYOfTree[j]<NEGATIVE_LIMIT)) //If test cases is out of limits, exit code.
                {
                  fclose(input);
                  printf("The ith treetop should  be located at the coordinate (xi, yi), with -10000<=xi, yi <=10000.\n");
                  free(usedIdx);
                  free(coordXOfTree);
                  free(coordYOfTree);
                  exit(1);
                }
                
            }
            
            permutation = malloc( numOfTrees*sizeof(int) );
            
            permutation[0] = 0;
            usedIdx[0] = 1;//As an optimization, we'll set to 1 the first value of the usedIndex array


            getPermOfTestCase(numOfTrees); //gets the permutations for each test case according to the number of trees
            free(usedIdx); //free the index array
            free(coordXOfTree);
            free(coordYOfTree);
            free(permutation);
            printf("%.3lf\n",distance);

        }
    }
     else  //if in.txt is not found, exit code
      {          
        printf("Please provide correct input file");
        exit(-1);     
      } 
}
