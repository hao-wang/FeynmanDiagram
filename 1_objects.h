/////////////// for model and process //////////////////
#define MAX_V_LEGTYPES 10								//max leg types of each interaction vertex, e.g. in (F1 F-1 U1:2) there are three leg types
#define MAX_V_LEGS 30									//max legs of each interaction vertex, e.g. in (F1 F-1 U1:2) there are four legs

struct particleType{									//this struct is used to describe a particle, e.g. 'F1' is described as: BorF='F', ID=1.
	char BorF;											//Boson 'U'/'C' or Fermion 'F';
	int ID;												//the particle type (ID number);
};

struct interaction{										//this struct is used to describe a interaction term, e.g. (F1 F-1 U1:2) has totLegTypes=3, totLegs=4...
	int totLegs;
	int totLegTypes;
	struct particleType legType[MAX_V_LEGTYPES];
	int multip[MAX_V_LEGTYPES];							//multiplicity of each leg type;
};														//Note, multiplicity of the fermion particle should never be larger than 1 in this program

/////////////// for feynman diagram ////////////////////
#define MAX_E_PARTICLES 100								//max external particles
#define MAX_I_VERTICES 50								//max internal interaction vertices

struct feynDiagExterPar{								//this struct describes how an external particle in the Feynman diagram is connected to a vertex
	int toVert;											//the vertex (ID number) this external particle point to
	int toLegTyp;										//the leg type (ID number) of the vertex this external particle point to
};

struct feynDiagVertex{									//this struct describes the temporary state of an internal vertex and how the vertex is conncected to another vertex
	int interType;										//the interaction type (ID number) of this vertex;
	int multip[MAX_V_LEGTYPES];							//the remaining multiplicity of each leg type after being taken (being pointed to by others).
	int totTo;
	int toVert[MAX_V_LEGS];								//similar as the 'toVert' in struct feynDiagExternal
	int toLegTyp[MAX_V_LEGS];							//similar as the 'toLeg' in struct feynDiagExternal
	int thisLegTyp[MAX_V_LEGS];							//
};

typedef
struct feynDiagram{
	struct feynDiagExterPar fP[MAX_E_PARTICLES];
	struct feynDiagVertex fV[MAX_I_VERTICES];
	int symmetryFactor;
	int fermionSign;
	int bookMark;										//used to identify the stage of the traversing process.
} feynDiag;

