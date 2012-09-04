/////////////// model //////////////////////////////////
int TotInterTypes;										//total interaction types
struct interaction * I;									//'I[...]' contains all the interaction types from the input file; interaction type k is represented by I[k];

/////////////// process ////////////////////////////////
int TotExterParticles;
struct particleType P[MAX_E_PARTICLES];					//'P[...]' contains all the external particles from the input file
int TotLoops;
int TotVertexes;

/////////////// feynman diagram ////////////////////////
struct feynDiagram FD;
int FDCount=0;

/////////////// output file ////////////////////////////
FILE * FileOut;