//Muhammad Khizr Shahid- 2413235

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//structure
typedef struct Champion {

char championName[50];
double expectedWinRate;
int NumberOfBattles;
int NumberOfWins;
double ActualWinRate;
double ExpectationSkew;

}CHAMPIONSARRAY;

//global variable which keeps track of size of array
//declared globally so that I can easily access directly in any function
//i am mainly using it for size of heap and heap operations and for adding the rest of the data to the array such as in getBattleData function
//and computeWinRate function. in order to run a for loop i needed to define a size so i used this variable to define size for the loop
int GlobalToTrackSize;

//parseInput function to parse the argv values
int parseInput(char *argv[],char* ChampionDataFile, char* BattlesDataFile ){

     if ( argv[1]!= NULL && argv[2]!= NULL && argv[3]!= NULL) {

          //atoi function to convert string to integers.
         int temp=atoi(argv[1]);
         strcpy(ChampionDataFile,argv[2]);
         strcpy(BattlesDataFile,argv[3]);
         return temp;
     }

     else{
         printf("Give Proper input through argv values.\n");
         exit(1);
     }

}

struct Champion* initializeChampions(char *ChampionDataFile) {

    char ch,space;
    int count=0,i,q;


    FILE *infile;
    infile = fopen(ChampionDataFile, "r");

    while ( (ch= fgetc(infile)) != EOF ) {
        if (ch == '\n') {
            count=count+1;
        }
    }
    count++; //doing this count++ after counting the lines in order to increment it one last time because the above while loop counts
            //until the end of file and at every next line character for each line so when it reaches the end of the  last line it needs to be incremented 1 last time
    fclose(infile);

    GlobalToTrackSize = count;

    FILE *infile3;
    infile3 = fopen(ChampionDataFile, "r");

    CHAMPIONSARRAY* temp = (CHAMPIONSARRAY*) malloc(count * sizeof(CHAMPIONSARRAY));

    //initialising
    for(q=0;q<count;q++) {

        strcpy(temp[q].championName, "empty");
        temp[q].expectedWinRate=0;
        temp[q].NumberOfBattles=0;
        temp[q].NumberOfWins=0;
        temp[q].ActualWinRate=0;
        temp[q].ExpectationSkew=0;
    }

    //reading values from file for names and expected win rate
    for(i=0;i<count;i++) {

        fscanf(infile3, "%s", &temp[i].championName);
        fscanf(infile3, "%lf", &temp[i].expectedWinRate);

    }

    fclose(infile3);

    return temp;
}


void getBattleData(CHAMPIONSARRAY *championArray, char *BattlesDataFile) {

    char ch,space;
    int count=0,i,k;

    FILE *infile;
    infile = fopen(BattlesDataFile, "r");

    while ( (ch= fgetc(infile)) != EOF ) {
        if (ch == '\n') {
            count=count+1;
        }
    }
    count++;//doing this count++ after counting the lines in order to increment it one last time because the above while loop counts
            //until the end of file and at every next line character for each line so when it reaches the end of the  last line it needs to be incremented 1 last time
    fclose(infile);

    int temporarySizeHolder = GlobalToTrackSize;

    FILE *infile3;
    infile3 = fopen(BattlesDataFile, "r");

    char battlenumber[50];
    char opponent1[50];
    char opponent2[50];
    char winner[50];
    //counting number of battles and winners
    for(i=0;i<count;i++) {

        fscanf(infile3, "%s", &battlenumber);
        fscanf(infile3, "%s", &opponent1);
        fscanf(infile3, "%s", &opponent2);
        fscanf(infile3, "%s", &winner);


        for (k=temporarySizeHolder; k >= 0; k--) {

            if (strcmp(opponent1,championArray[k].championName) == 0 )
                championArray[k].NumberOfBattles++;

            if (strcmp(opponent2,championArray[k].championName) == 0)
                championArray[k].NumberOfBattles++;

            if ( (strcmp(winner,championArray[k].championName) == 0) )
                championArray[k].NumberOfWins++;
            }

    }

    fclose(infile3);

}


void computeWinRate (CHAMPIONSARRAY *championArray) {

    int sizeforcomputation = GlobalToTrackSize;
    int k;
    double win_rate_ratio,temp;
    //calculating actual win rate, expectation skew
    //doing appropriate type casting
    for (k=0; k < sizeforcomputation; k++) {

         championArray[k].ActualWinRate = (double)championArray[k].NumberOfWins / (double)championArray[k].NumberOfBattles;
         win_rate_ratio = championArray[k].ActualWinRate / championArray[k].expectedWinRate;
         temp = win_rate_ratio - 1.00;

         if(temp < 0) {
            temp = temp * (-1);
         }

         championArray[k].ExpectationSkew = temp;
    }

}

void swap(CHAMPIONSARRAY *championArray,int i,int large) {

    CHAMPIONSARRAY temporary;

    strcpy(temporary.championName,championArray[i].championName);
    temporary.expectedWinRate=championArray[i].expectedWinRate;
    temporary.NumberOfBattles=championArray[i].NumberOfBattles;
    temporary.NumberOfWins=championArray[i].NumberOfWins;
    temporary.ActualWinRate=championArray[i].ActualWinRate;
    temporary.ExpectationSkew=championArray[i].ExpectationSkew;

    championArray[i].ActualWinRate=championArray[large].ActualWinRate;
    strcpy(championArray[i].championName,championArray[large].championName);
    championArray[i].ExpectationSkew=championArray[large].ExpectationSkew;
    championArray[i].expectedWinRate=championArray[large].expectedWinRate;
    championArray[i].NumberOfBattles=championArray[large].NumberOfBattles;
    championArray[i].NumberOfWins=championArray[large].NumberOfWins;


    championArray[large].ActualWinRate=temporary.ActualWinRate;
    strcpy(championArray[large].championName,temporary.championName);
    championArray[large].ExpectationSkew=temporary.ExpectationSkew;
    championArray[large].expectedWinRate=temporary.expectedWinRate;
    championArray[large].NumberOfBattles=temporary.NumberOfBattles;
    championArray[large].NumberOfWins=temporary.NumberOfWins;


}

void heapify(CHAMPIONSARRAY *championArray, int size, int i,int sortingCriteria) {

    int large=i;
    int leftchild= 2*i;
    int rightchild = 2*i+1;

    if (sortingCriteria==1) {

        if ((leftchild < size) && (championArray[leftchild].ActualWinRate < championArray[i].ActualWinRate)) {
            large = leftchild;
        }
        else
            large = i;

        if ((rightchild < size) && (championArray[rightchild].ActualWinRate < championArray[large].ActualWinRate)) {
            large = rightchild;
        }

        if (large != i) {
            swap (championArray,i,large);
            heapify(championArray,size,large,sortingCriteria);
        }

    }

    else if (sortingCriteria==2) {

        if ((leftchild < size) && (championArray[leftchild].expectedWinRate < championArray[i].expectedWinRate)) {
            large = leftchild;
        }
        else
            large = i;

        if ((rightchild < size) && (championArray[rightchild].expectedWinRate < championArray[large].expectedWinRate)) {
            large = rightchild;
        }

        if (large != i) {
            swap (championArray,i,large);
            heapify(championArray,size,large,sortingCriteria);
        }

    }


    else if (sortingCriteria==3) {

        if ((leftchild < size) && (championArray[leftchild].ExpectationSkew < championArray[i].ExpectationSkew)) {
            large = leftchild;
        }
        else
            large = i;

        if ((rightchild < size) && (championArray[rightchild].ExpectationSkew < championArray[large].ExpectationSkew)) {
            large = rightchild;
        }

        if (large != i) {
            swap (championArray,i,large);
            heapify(championArray,size,large,sortingCriteria);
        }


    }



}

void heapSort(CHAMPIONSARRAY *championArray,int sortingCriteria) {


    int i;
    int heap_size = GlobalToTrackSize;
    buildHeap(championArray,sortingCriteria);

    for(i=heap_size; i >1; i--) {
        swap(championArray,0,(i-1));
        heap_size=heap_size-1;
        heapify(championArray,heap_size,0,sortingCriteria);
    }


}

void buildHeap(CHAMPIONSARRAY *championArray,int sortingCriteria) {

    int heap_size= GlobalToTrackSize;
    int i;

    for(i=(GlobalToTrackSize/2); i>0; i--) {
        heapify(championArray,heap_size,i-1,sortingCriteria);
    }

}


void printLeaderboard(struct Champion* arrayofchamps) {

    int i;

    printf("Champion \t");
    printf("Battles \t");
    printf("Win \t\t");
    printf("AWR \t\t");
    printf("EWR \t\t");
    printf("Skew \t\t");
    printf("\n");

    for (i=0; i < 6; i++ ) {

        printf("%s \t\t", arrayofchamps[i].championName);
        printf("%d \t\t", arrayofchamps[i].NumberOfBattles);
        printf("%d \t\t", arrayofchamps[i].NumberOfWins);
        printf("%0.2f\t\t", arrayofchamps[i].ActualWinRate);
        printf("%0.2lf\t\t", arrayofchamps[i].expectedWinRate);
        printf("%0.2f \t\t", arrayofchamps[i].ExpectationSkew);
        printf("\n");

    }

}



int main (int argc, char *argv[]) {

    char ChampionDataFile[50];
    char BattlesDataFile[50];
    int sortingCriteria;


    sortingCriteria= parseInput(argv,ChampionDataFile,BattlesDataFile);

    struct Champion *championArray;

    championArray= initializeChampions(ChampionDataFile);

    getBattleData(championArray,BattlesDataFile);

    computeWinRate(championArray);

    heapSort(championArray,sortingCriteria);

    printLeaderboard(championArray);


return 0;
}


