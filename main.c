#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <ctype.h>

int bruteForce(char* toSearch, char* motive,int toSearchL, int motiveL){

    int forCounter,forCounter2;
    int counter = 0;
    for(forCounter = 0; forCounter <toSearchL-motiveL;forCounter++){
        for(forCounter2 = 0; forCounter2 < motiveL; forCounter2++){
            if(toSearch[forCounter2+forCounter] == motive[forCounter2]){
                counter++;
                if(forCounter2 == motiveL - 1){
                    fprintf(stdout,"Number of comparisons: %d\n",counter);
                    return forCounter;
                }
            } else{
                counter++;
                break;
            }
        }
    }

    return -1;

}

int kmpPrefix(char* motive, int motiveL,int* prefixArray){
    int counter = 0;
    prefixArray[0] = 0;
    int i = 1;
    int j = 0;
    while (i<motiveL){
        if(motive[i] == motive[j]){
            counter++;
            prefixArray[i] = j +1;
            i++;
            j++;
        } else if(j>0){
            counter++;
            j = prefixArray[j-1];
        } else{
            counter++;
            prefixArray[i] = 0;
            i++;
        }
    }
    return counter;

}

int kmpA(char* toSearch, char* motive,int toSearchL, int motiveL){
    int prefixCount;
    int count = 0;
    int prefixArray[motiveL];
    prefixCount = kmpPrefix(motive,motiveL,prefixArray);
    int i = 0;
    int j = 0;
    while(i<toSearchL-motiveL){
        if(toSearch[i] == motive[j]){
            count++;
            if(j == motiveL -1){
                fprintf(stdout,"Number of comparisons for failure function = %d , Number of comparisons for finding matching string index = %d\nTotal number of comparisons = %d\n",prefixCount,count,prefixCount+count);
                return i - j;
            } else{
                i++;
                j++;
            }
        } else{
            count++;
            if(j > 0){
                j = prefixArray[j-1];
            } else{
                i++;
                j = 0;
            }
        }
    }
    fprintf(stdout,"Number of comparisons for failure function = %d , Number of comparisons for finding matching string index = %d\n Total number of comparisons = %d\n",prefixCount,count,prefixCount+count);
    return -1;

}

int boyerMoorePrefix(int *prefArr1, int *prefArr2, char *motive, int motiveL){
    int counter = 1;
    int i = motiveL;
    int j = i+1;
    prefArr2[i] = j;
    while(i>0){
        while(j<=motiveL && motive[i-1] != motive[j-1]){
            counter++;
            if(prefArr1[j] == 0){
                prefArr1[j] = j-i;
            }
            j=prefArr2[j];
        }
        i--;
        j--;
        prefArr2[i] = j;
    }
/*
    for (int j = 0; j < 10; ++j) {
        printf("prefArr1[%d] is %d \n",j,prefArr1[j]);
    } for (int j = 0; j < 10; ++j) {
        printf("prefArr2[%d] is %d\n",j,prefArr2[j]);
    }
*/
    int forCounter,b;
    b = prefArr2[0];
    for(forCounter = 0; forCounter<motiveL;forCounter++){
        if(prefArr1[forCounter] == 0){
            prefArr1[forCounter] = b;
        }
        if(forCounter ==b){
            b = prefArr2[b];
        }
    }
    /*
    printf("\n\n---------------After--------------------\n\n");

    for (int j = 0; j < 10; ++j) {
        printf("prefArr1[%d] is %d \n",j,prefArr1[j]);
    } for (int j = 0; j < 10; ++j) {
        printf("prefArr2[%d] is %d\n",j,prefArr2[j]);
    }
*/
    return counter;
}

int boyerMoore(char* motive, int motiveL,char* searchArray,int toSearchL){

    int s = 0;
    int prefixCount;
    int j;
    int counter = 0;
    int prefArr1[motiveL+1];
    int prefArr2[motiveL+1];
    for(int i=0;i<motiveL+1;i++) {
        prefArr1[i]=0;
    }

    boyerMoorePrefix(prefArr1, prefArr2, motive, motiveL);

    while(s <= toSearchL-motiveL){

        j = motiveL-1;


        while(j >= 0 && motive[j] == searchArray[s+j]){
            counter++;
            j--;
        }
        counter++;


        if (j<0)
        {
            fprintf(stdout,"Number of comparisons for failure function = %d , Number of comparisons for finding matching string index = %d\nTotal number of comparisons = %d\n",prefixCount,counter,prefixCount+counter);
            //printf("pattern occurs at index = %d no of comp = %d \n", s,counter);
            return s;
            //s += prefArr1[0];
        }
        else

            s += prefArr1[j+1];
    }

    return -1;
}

int rabinKarpValueOfChar(char c){
    if(c == 'A'){
        return 0;
    } else if(c == 'C'){
        return 1;
    } else if(c == 'G'){
        return 2;
    } else if(c == 'T'){
        return 3;
    }
    else{
        fprintf(stderr,"There is no nucleotide %c check input\n",c);
    }
}

int rabinKarp(char* motive, int motiveL,char* searchArray,int toSearchL){
    int counter = 0;
    int q = 997;
    int forCounter;
    int c = 1;
    int fingerPrint = 0;
    for (forCounter = 0; forCounter <motiveL; forCounter++) {
        fingerPrint = ((fingerPrint<<2) + rabinKarpValueOfChar(motive[forCounter])) % q;
        if(forCounter != 0){
            c = (c<<2) % q;
        }

    }

    int fingerPrintSearch = 0;
    for (forCounter = 0; forCounter < motiveL; forCounter++) {
        //fingerPrintSearch = (int)(pow(4,motiveL-1-forCounter) * rabinKarpValueOfChar(searchArray[forCounter])) % q ;
        fingerPrintSearch = ((fingerPrintSearch<<2) + rabinKarpValueOfChar(searchArray[forCounter])) % q;
    }
    for(forCounter = 0; forCounter<toSearchL-motiveL;forCounter++ ){
        if(fingerPrint == fingerPrintSearch){
            for (int i = 0; i < motiveL; i++) {
                if(motive[i] == searchArray[i+forCounter]){
                    counter++;
                    if(i == motiveL-1){
                        fprintf(stdout,"Number of comparisons: %d\n",counter);
                        //fuck forgot to compare todo
                        return forCounter;
                    }
                } else{
                    //fingerPrintSearch = ((fingerPrintSearch - (int)(pow(4,motiveL-1)*rabinKarpValueOfChar(searchArray[forCounter])))<<2) + rabinKarpValueOfChar(searchArray[forCounter+motiveL]);
                    fingerPrintSearch = (((fingerPrintSearch - ((c*rabinKarpValueOfChar(searchArray[forCounter])))) <<2) + rabinKarpValueOfChar(searchArray[forCounter+motiveL])) % q;
                    if(fingerPrintSearch<0){
                        fingerPrintSearch +=q;
                    }
                    break;
                }
            }

        } else{
           // printf("%d \n",((c*rabinKarpValueOfChar(searchArray[forCounter]))%q));
            fingerPrintSearch = (((fingerPrintSearch - ((c*rabinKarpValueOfChar(searchArray[forCounter])))) <<2) + rabinKarpValueOfChar(searchArray[forCounter+motiveL])) % q;
            if(fingerPrintSearch<0){
                fingerPrintSearch +=q;
            }
            //fingerPrintSearch = ((fingerPrintSearch - (int)(pow(4,motiveL-1)*rabinKarpValueOfChar(searchArray[forCounter])))<<2) + rabinKarpValueOfChar(searchArray[forCounter+motiveL]);
        }
    }

    return -1;
}

int main(int argc,char **argv) {

    struct timeval start,timeEnd;
    int forCounter1,forCounter2;
    char* search = "TAAGTCTATACCATCGTAGTCTAATTAACGTTATGGTAGGATATCAAGGACGGAATGACCGCAGAGGCGACGTTAATGCGCCGTCAGAGACGCCCTAAAGATTGCGGTAGGGTCCCGTTGTTAAAGAGACTTGAGTGGGTGCTTGATGGGAGTGTATTAAGGGCATGTATAAGTGTTGCTGGGTCTAAGGCATTAAAGCTGAGTCAATAGTTACATTGCAGATTAACGAGATCTGAAATTAAGGGAGAGATTCCCAGAGTGGCCTAGTACTTAAGGGCACCCACGCCGCAGGCGGCCCTACGCCCGTTAATGGTTCGAGTGCTATTCACTAACACATTAACGGACGTTTAGTGTGGATTATAGGTGAAGGGTCTGCGCCACTCCAAGGCAGGGAACATATGTGTTGTTACTATCTTAACG";
    char* patern = "TGGGTCTAAGGCATTAAAGCTGAGTCAATAGT";
    char* patern2 = "AAAAAG";
    char* search2 = "AAAAAAAAAAAAAAG";
    //void readFiles(char* searchSeqs, char* motives,int* numberOfSearchFiles,int* searchFileSizes, int* numberOfMotives, int* motiveLengths,char** actualSearchArrays,char** actualMotiveArrays;
    int numberOfSearchFiles, *searchFileSizes, numberOfMotives, *motiveLengths;
    char **actualSearchArrays, **actualMotiveArrays;
    char* f1 = argv[1];
    char* f2 = argv[2];


    //////////////////////////////////////////////////////////////


    FILE *fp1, *fp2;

    fp1 = fopen(f1,"r");
    fp2 = fopen(f2,"r");



    numberOfMotives = 0;
    numberOfSearchFiles = 0;

    char c;

    while(!feof(fp1)){
        fscanf(fp1,"%c",&c);
        if(c=='>'){

            numberOfSearchFiles = numberOfSearchFiles +1;


        }

    }


    searchFileSizes = (int*)calloc(numberOfSearchFiles,sizeof(int));

    rewind(fp1);
    //read the first line
    fscanf(fp1,"%c",&c); // c must be '>'
    while(!feof(fp1) && c != '\n'){
        fscanf(fp1,"%c",&c);

    }



    int counter = 0;
    int counter2 = 0;

    while(!feof(fp1)){

        fscanf(fp1,"%c",&c);

        if(c == '>') {

            searchFileSizes[counter2] = counter;
            counter = 0;
            counter2++;
            while(!feof(fp1) && c != '\n'){
                fscanf(fp1,"%c",&c);

            }

            if(!feof(fp1)){
                fscanf(fp1,"%c",&c);
            }

        }

        if(c != '\n'){
            counter++;
        }

    }

    searchFileSizes[counter2] = counter;

    //char** actualSearchArrays;
    //char* temArr;
    actualSearchArrays = (char**)calloc(numberOfSearchFiles, sizeof(char*));
    int forCounter;
    for(forCounter = 0; forCounter<numberOfSearchFiles;forCounter++){
        actualSearchArrays[forCounter] = (char*)calloc(searchFileSizes[forCounter] , sizeof(char));
    }


    //here sequences to be searched hopefully allocated

    rewind(fp1);
    //read the first line

    fscanf(fp1,"%c",&c); // c must be '>'
    while(!feof(fp1) && c != '\n'){
        fscanf(fp1,"%c",&c);

    }

    counter = 0;
    counter2 = 0;


    while(!feof(fp1)){

        fscanf(fp1,"%c",&c);

        if(c == '>') {

            counter = 0;
            counter2++;
            while(!feof(fp1) && c != '\n'){
                fscanf(fp1,"%c",&c);

            }

            if(!feof(fp1)){
                fscanf(fp1,"%c",&c);
            }

        }

        if(!isspace(c)){
           //fprintf(stderr,"Adding %c to actualSearchArrays[%d][%d]\n",c,counter2,counter);
            actualSearchArrays[counter2][counter] = c;
            //fprintf(stderr,"added\n");

            counter++;
        }


    }



    //here sequences to be searched hopefully allocated

    //just to be sure

    for(forCounter = 0; forCounter<numberOfSearchFiles; forCounter++){
        for(int i = 0; i < searchFileSizes[forCounter]; i++){
            //fprintf(stdout,"%c", actualSearchArrays[forCounter][i]);
        }
        //fprintf(stdout,"\n");
    }

    fclose(fp1);

    ////////////////////////////////////////////////////motives


    while(!feof(fp2)){
        fscanf(fp2,"%c",&c);
        if(c=='>'){
            numberOfMotives = numberOfMotives + 1;
        }

    }


    motiveLengths = (int*)calloc(numberOfMotives ,sizeof(int));

    rewind(fp2);
    //read the first line
    fscanf(fp2,"%c",&c); // c must be '>'
    while(!feof(fp2) && c != '\n'){
        fscanf(fp2,"%c",&c);

    }




    counter = 0;
    counter2 = 0;

    while(!feof(fp2)){

        fscanf(fp2,"%c",&c);

        if(c == '>') {

            motiveLengths[counter2] = counter;
            counter = 0;
            counter2++;
            while(!feof(fp2) && c != '\n'){
                fscanf(fp2,"%c",&c);

            }

            if(!feof(fp2)){
                fscanf(fp2,"%c",&c);
            }

        }

        if(c != '\n'){
            counter++;
        }

    }


    motiveLengths[counter2] = counter;

    //char** actualMotiveArrays;
    actualMotiveArrays = (char**)calloc(numberOfMotives , sizeof(char*));
    forCounter;
    for(forCounter = 0; forCounter<numberOfMotives;forCounter++){
        actualMotiveArrays[forCounter] = (char*)calloc(motiveLengths[forCounter] , sizeof(char));
    }



    //here sequences to be searched hopefully allocated

    rewind(fp2);
    //read the first line

    fscanf(fp2,"%c",&c); // c must be '>'
    while(!feof(fp2) && c != '\n'){
        fscanf(fp2,"%c",&c);

    }


    counter = 0;
    counter2 = 0;

    while(!feof(fp2)){

        fscanf(fp2,"%c",&c);

        if(c == '>') {

            counter = 0;
            counter2++;
            while(!feof(fp2) && c != '\n'){
                fscanf(fp2,"%c",&c);

            }

            if(!feof(fp2)){
                fscanf(fp2,"%c",&c);
            }

        }

        //fprintf(stderr,"c = %c \n" , c);
        if(!isspace(c)){
            //fprintf(stderr,"Adding %c to actualMotiveArrays[%d][%d]\n",c,counter2,counter);
            actualMotiveArrays[counter2][counter] = c;
            //fprintf(stderr,"added\n");

            counter++;
        }


    }


    //here sequences to be searched hopefully allocated

    //just to be sure

   /* for(forCounter = 0; forCounter<numberOfMotives; forCounter++){
        for(int i = 0; i < motiveLengths[forCounter]; i++){
            fprintf(stdout,"%c", actualMotiveArrays[forCounter][i]);
        }
        fprintf(stdout,"\n");
    }*/
    fclose(fp2);

    ///////////////////////////////////////////////////////////////

    //readFiles(f1,f2,&numberOfSearchFiles,searchFileSizes,&numberOfMotives,motiveLengths,actualSearchArrays,actualMotiveArrays);

/*
    for(forCounter1 = 0;forCounter1<numberOfSearchFiles;forCounter1++) {

        for (forCounter2 = 0; forCounter2 < numberOfMotives; forCounter2++) {
            fprintf(stderr,"herem\n");

            fprintf(stdout,"%c", actualMotiveArrays[forCounter1][forCounter2]);
        }
    }
*/


    //fprintf(stderr,"f1 = %s , f2 = %s \n",f1,f2);
    //fprintf(stderr,"numberOfSearchFiles = %d , numberOfMotives = %d \n" , numberOfSearchFiles, numberOfMotives );

    for(forCounter1 = 0;forCounter1<numberOfSearchFiles;forCounter1++){
        for(forCounter2=0;forCounter2<numberOfMotives;forCounter2++) {

            //fprintf(stdout,"search = %d , pattern = %d", )
            if (searchFileSizes[forCounter1] >= motiveLengths[forCounter2]) {

/*
                for (int t1 = 0; t1 < searchFileSizes[forCounter1]; t1++) {
                    fprintf(stdout, "%c", actualSearchArrays[forCounter1][t1]);
                }
                fprintf(stdout, "\n");

                for (int t1 = 0; t1 < motiveLengths[forCounter2]; t1++) {
                    fprintf(stdout, "%c", actualMotiveArrays[forCounter2][t1]);
                }
                fprintf(stdout, "\n");
*/
                fprintf(stdout, "Brute Force Search: \n");
                gettimeofday(&start, NULL);
                int bf = bruteForce(actualSearchArrays[forCounter1], actualMotiveArrays[forCounter2],
                                    searchFileSizes[forCounter1], motiveLengths[forCounter2]);
                gettimeofday(&timeEnd, NULL);
                fprintf(stdout, "Run time: %ld\n",
                        ((timeEnd.tv_sec - start.tv_sec) * 1000000L + (timeEnd.tv_usec - start.tv_usec)));
                if (bf == -1) {
                    fprintf(stdout, "P[%d] is not in T[%d]\n",forCounter2,forCounter1);
                } else {
                    fprintf(stdout, "P[%d] is in T[%d] at index: %d\n",forCounter2,forCounter1, bf);
                }

                fprintf(stdout, "Knut-Morris-Prat Algorithm: \n");
                gettimeofday(&start, NULL);
                int kmp = kmpA(actualSearchArrays[forCounter1], actualMotiveArrays[forCounter2],
                               searchFileSizes[forCounter1], motiveLengths[forCounter2]);
                gettimeofday(&timeEnd, NULL);
                fprintf(stdout, "Run time: %ld\n",
                        ((timeEnd.tv_sec - start.tv_sec) * 1000000L + (timeEnd.tv_usec - start.tv_usec)));

                if (kmp == -1) {
                    fprintf(stdout, "P[%d] is not in T[%d]\n",forCounter2,forCounter1);
                } else {
                    fprintf(stdout, "P[%d] is in T[%d] at index: %d\n",forCounter2,forCounter1, bf);
                }

                fprintf(stdout, "Boyer-Moore Algorithm: \n");
                gettimeofday(&start, NULL);
                int bm = boyerMoore(actualMotiveArrays[forCounter2], motiveLengths[forCounter2],
                                    actualSearchArrays[forCounter1], searchFileSizes[forCounter1]);
                gettimeofday(&timeEnd, NULL);
                fprintf(stdout, "Run time: %ld\n",
                        ((timeEnd.tv_sec - start.tv_sec) * 1000000L + (timeEnd.tv_usec - start.tv_usec)));
                if (bm == -1) {
                    fprintf(stdout, "P[%d] is not in T[%d]\n",forCounter2,forCounter1);
                } else {
                    fprintf(stdout, "P[%d] is in T[%d] at index: %d\n",forCounter2,forCounter1, bf);
                }

                fprintf(stdout, "Rabin-Karp Algorithm: \n");
                gettimeofday(&start, NULL);
                int rk = rabinKarp(actualMotiveArrays[forCounter2], motiveLengths[forCounter2],
                                   actualSearchArrays[forCounter1], searchFileSizes[forCounter1]);
                gettimeofday(&timeEnd, NULL);
                fprintf(stdout, "Run time: %ld\n",
                        ((timeEnd.tv_sec - start.tv_sec) * 1000000L + (timeEnd.tv_usec - start.tv_usec)));

                if (rk == -1) {
                    fprintf(stdout, "P[%d] is not in T[%d]\n",forCounter2,forCounter1);
                } else {
                    fprintf(stdout, "P[%d] is in T[%d] at index: %d\n",forCounter2,forCounter1, bf);
                }
                fprintf(stdout,"\n\n\n");
            }
            else{
                fprintf(stdout,"Search Pattern can not be shorter that the string to be searched\n");
                fprintf(stdout,"\n\n\n");
            }
        }
    }
/*
    fprintf(stdout,"Brute Force Search: \n");
    gettimeofday(&start,NULL);
    int bf = bruteForce(search,patern,420,32);
    gettimeofday(&timeEnd,NULL);
    fprintf(stdout,"Run time: %ld\n",((timeEnd.tv_sec - start.tv_sec)*1000000L + (timeEnd.tv_usec - start.tv_usec)));
    if(bf==-1){
        fprintf(stdout,"P is not in T\n");
    } else{
        fprintf(stdout,"P is in T at index: %d\n",bf);
    }

    fprintf(stdout,"Knut-Morris-Prat Algorithm: \n");
    gettimeofday(&start,NULL);
    int kmp = kmpA(search,patern,420,32);
    gettimeofday(&timeEnd,NULL);
    fprintf(stdout,"Run time: %ld\n",((timeEnd.tv_sec - start.tv_sec)*1000000L + (timeEnd.tv_usec - start.tv_usec)));

    if(kmp==-1){
        fprintf(stdout,"P is not in T\n");
    } else{
        fprintf(stdout,"P is in T at index: %d\n",kmp);
    }

    fprintf(stdout,"Boyer-Moore Algorithm: \n");
    gettimeofday(&start,NULL);
    int bm = boyerMoore(patern,32,search,420);
    gettimeofday(&timeEnd,NULL);
    fprintf(stdout,"Run time: %ld\n",((timeEnd.tv_sec - start.tv_sec)*1000000L + (timeEnd.tv_usec - start.tv_usec)));
    if(bm==-1){
        fprintf(stdout,"P is not in T\n");
    } else{
        fprintf(stdout,"P is in T at index: %d\n",bm);
    }

    fprintf(stdout,"Rabin-Karp Algorithm: \n");
    gettimeofday(&start,NULL);
    int rk = rabinKarp(patern2,6,search2,15);
    gettimeofday(&timeEnd,NULL);
    fprintf(stdout,"Run time: %ld\n",((timeEnd.tv_sec - start.tv_sec)*1000000L + (timeEnd.tv_usec - start.tv_usec)));

    if(rk==-1){
        fprintf(stdout,"P is not in T\n");
    } else{
        fprintf(stdout,"P is in T at index: %d\n",rk);
    }
    */
    //int rk2 = rabinKarp(patern2,4,search2,15);

    //fprintf(stdout," bf = %d , bm = %d, kmp = %d, rk = %d\n",bf,bm,kmp,rk);


    //readFiles(f1,f2);
    return 0;
}