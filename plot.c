#include "pbPlots.h"
#include "supportLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 256 // Maximum string length this program can handle
#define MAX_NB_Data 100000 // Maximum number of data points

struct Soldier {
	int id, heartRate, Impacts;
};

double rate[MAX_NB_Data];
double imps[MAX_NB_Data];
double xaxis[MAX_NB_Data];

// The CSV parser
int next_field( FILE *f, char *buf, int max ) {
	int i=0, end=0, quoted=0;
	
	for(;;) {
		// fetch the next character from file		
		buf[i] = fgetc(f);
		// if we encounter quotes then flip our state and immediately fetch next char
		if(buf[i]=='"') { quoted=!quoted; buf[i] = fgetc(f); }
		// end of field on comma if we're not inside quotes
		if(buf[i]==',' && !quoted) { break; }
		// end record on newline or end of file
		if(feof(f) || buf[i]=='\n') { end=1; break; } 
		// truncate fields that would overflow the buffer
		if( i<max-1 ) { ++i; } 
	}

	buf[i] = 0; // null terminate the string
	return end; // flag stating whether or not this is end of the line
}

// Stuff to make life a bit neater in main
void fetch_soldier (  FILE *csv, struct Soldier *p) {
	char buf[MAX_BUFFER];

	next_field( csv, buf, MAX_BUFFER );      // load id into buffer as string
	p->id = atoi(buf);                       // then parse to integer
	next_field( csv, buf, MAX_BUFFER );
	p->heartRate = atoi(buf);          // atoi stands for ASCII to Integer
	next_field( csv, buf, MAX_BUFFER ); // It converts strings to numbers
	p->Impacts = atoi(buf);                  // It is not a totally safe function to use.

}

int
main ( int argc, char *argv[] ) {
	FILE *f;
	struct Soldier pArray[MAX_NB_Data];		
	struct Soldier p;

	// Users must pass the name of the input file through the command line. Make sure
	// that we got an input file. If not, print a message telling the user how to use
	// the program and quit
	if( argc < 2 ) { 
		printf("usage: csv FILE\n"); 
		return EXIT_FAILURE; 
	}

	// Try to open the input file. If there is a problem, report failure and quit
	f = fopen(argv[1], "r");
	if(!f) { 
		printf("unable to open %s\n", argv[1]); 
		return EXIT_FAILURE; 
	}
	
	fetch_soldier( f, &p ); // discard the header data in the first line

	// Now read until the end of the file
	int rows = 0;
	while(!feof(f)) {
		fetch_soldier( f, &pArray[rows] );
		rows++;
	}
	rows--;
	
	for(int j=0;j<rows;j++){
		rate[j]= (float)pArray[j].heartRate;
		imps[j] = (float)pArray[j].Impacts;
	}
	
	for(int j=0;j<=rows;j++){
		xaxis[j]=j;
		printf("%f \n",xaxis[j]);
	}
	
	RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
	DrawScatterPlot(canvasReference, 600, 400, xaxis, 20, imps, 20);
	size_t length;
	double *pngdata = ConvertToPNG(&length, canvasReference->image);
	WriteToFile(pngdata, length, "soldierImpact.png");
	DeleteImage(canvasReference->image);
	
	
	RGBABitmapImageReference *canvasReference1 = CreateRGBABitmapImageReference();
	DrawScatterPlot(canvasReference1, 600, 400, xaxis, 20, rate, 20);
	size_t length1;
	double *pngdata1 = ConvertToPNG(&length1, canvasReference1->image);
	WriteToFile(pngdata1, length1, "soldierHeart.png");
	DeleteImage(canvasReference1->image);

	// Always remember to close the file
	fclose(f);
	return EXIT_SUCCESS;
}














