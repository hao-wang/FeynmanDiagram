#include "0_include.h"
#include "1_objects.h"
#include "2_globals.h"
#include "3_functions.h"

main()
{
	int i;
	FileOut=fopen("output-feynman-diagrams.txt","w");
	////// obtain the information from the input file ///////
	////// and rewrite them on the output file as a check ///
	obtainInput();
	writeOutputHead();
	////// initialize the Feynman diagram ///////////////////
	for(i=0; i<TotExterParticles; i++){
		FD.fP[i].toVert=-1;
		FD.fP[i].toLegTyp=-1;}
	for(i=0; i<TotVertices; i++){
		FD.fV[i].interType=-1;
		FD.fV[i].totTo=0;}
	FD.symmetryFactor=1;
	FD.fermionSign=1;
	FD.bookMark=0; 
	/////// now, start the traversing! ///////////////////////
	feynTraverse();
	////////////////////////
	fclose(FileOut);
	printf("total Feynman diagrams: %d\n", FDCount);
}
