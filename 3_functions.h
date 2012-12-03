void obtainInput()
{
	FILE * fIn=NULL;
	char mark[100];
	int i,j,totInP,totOutP;
	///////////////////////////
	if( (fIn=fopen("input.txt","r"))==NULL ){
		printf("cannot open file 'input.txt'\n");
		exit(0);}
	///////////////////////////
	do{
		fscanf(fIn,"%s",mark);
		if(strcmp(mark,">>>")==0){
			fscanf(fIn,"%d",&TotInterTypes);
			I=malloc(TotInterTypes*sizeof(struct interaction));
			break;}
	}while(!feof(fIn));
	do{
		fscanf(fIn,"%s",mark);
		if(strcmp(mark,">>>")==0){
			for(i=0; i<TotInterTypes; i++){
				fscanf(fIn,"%d>",&I[i].totLegTypes);
				I[i].totLegs=0;
				for(j=0; j<I[i].totLegTypes; j++){
					fscanf(fIn," %c",&I[i].legType[j].BorF);
					fscanf(fIn,"%d",&I[i].legType[j].ID);
					if(I[i].legType[j].BorF=='F')I[i].multip[j]=1;
					else if(I[i].legType[j].BorF=='U'||I[i].legType[j].BorF=='C'){
						fscanf(fIn,":%d",&I[i].multip[j]);}
					else{
						printf("data error in the input file\n");
						exit(0);}
					I[i].totLegs+=I[i].multip[j];}}
			break;}
	}while(!feof(fIn));
	do{
		fscanf(fIn,"%s",mark);
		if(strcmp(mark,">>>")==0){
			fscanf(fIn,"%d",&TotExterParticles);
			break;}
	}while(!feof(fIn));
	do{
		fscanf(fIn,"%s",mark);
		if(strcmp(mark,">>>")==0){
			fscanf(fIn,"%d>",&totInP);
			for(i=0; i<totInP; i++){
				fscanf(fIn," %c",&P[i].BorF);
				fscanf(fIn,"%d",&P[i].ID);
				if(P[i].BorF!='U')P[i].ID*=-1;}
			fscanf(fIn,"%d>",&totOutP);
			for(i=totInP; i<totOutP+totInP; i++){
				fscanf(fIn," %c",&P[i].BorF);
				fscanf(fIn,"%d",&P[i].ID);}
			break;}
	}while(!feof(fIn));
	do{
		fscanf(fIn,"%s",mark);
		if(strcmp(mark,">>>")==0){
			fscanf(fIn,"%d",&TotLoops);
			TotVertices=(TotExterParticles-2)+2*TotLoops;
			break;}
	}while(!feof(fIn));
	/////////////////////////////
	fclose(fIn);
}

void writeOutputHead()
{
	int i,j;
	///////////////////////
	fprintf(FileOut,"\
============================================================================\n\
             The physical model (i.e. the relevent interactions)\n\
============================================================================\n\
>>> the interactions\n\
");
	for(i=0; i<TotInterTypes; i++){
		fprintf(FileOut, "I%d:\t(", i+1);
		for(j=0; j<I[i].totLegTypes; j++){
			fprintf(FileOut, "%c%d", I[i].legType[j].BorF, I[i].legType[j].ID);
			if(I[i].legType[j].BorF=='U'||I[i].legType[j].BorF=='C')fprintf(FileOut,":%d", I[i].multip[j]);
			if(j==I[i].totLegTypes-1)fprintf(FileOut, ")\n");
			else fprintf(FileOut, ",");}}
	///////////////////////
	fprintf(FileOut,"\n\
============================================================================\n\
                          The physical process\n\
============================================================================\n\
>>> the external particles\n\
");
	for(i=0; i<TotExterParticles; i++){
		fprintf(FileOut, "P%d:\t(%c%d)\n", i+1, P[i].BorF, P[i].ID);}
	//////////////////////
	fprintf(FileOut, "\
>>> the internal vertices\n\
");
	for(i=0; i<TotVertices; i++){
		fprintf(FileOut, "H%d\n", i+1);}
	//////////////////////
	fprintf(FileOut, "\n\
============================================================================\n\
                          The Feynman diagrams\n\
============================================================================\n\
");
}

int factorial(int n)														//performs the calculation of 'n!'
{
	int out=1;
	for(; n>1; n--)out*=n;
	return(out);
}

void writeFeynDiag(struct feynDiagram * pFeynDiag)
{
	int i,j,factor=1;
	//////////////////////////////////////////////
	for(i=0; i<TotExterParticles; i++){
		fprintf(FileOut, "P%d-->H%d (I%d.leg[%d])\n"
			,i+1
			,pFeynDiag->fP[i].toVert+1
			,pFeynDiag->fV[pFeynDiag->fP[i].toVert].interType+1
			,pFeynDiag->fP[i].toLegTyp+1);}
	for(i=0; i<TotVertices; i++){
		for(j=0; j<pFeynDiag->fV[i].totTo; j++){
			fprintf(FileOut, "H%d-->H%d (I%d.leg[%d]-->I%d.leg[%d])\n"
				,i+1
				,pFeynDiag->fV[i].toVert[j]+1
				,pFeynDiag->fV[i].interType+1
				,pFeynDiag->fV[i].thisLegTyp[j]+1
				,pFeynDiag->fV[pFeynDiag->fV[i].toVert[j]].interType+1
				,pFeynDiag->fV[i].toLegTyp[j]+1);}}
	fprintf(FileOut,"symmetryFactor = %d ",pFeynDiag->symmetryFactor);
	for(i=0; i<TotVertices; i++){
		for(j=0; j<I[pFeynDiag->fV[i].interType].totLegTypes; j++){
			if(I[pFeynDiag->fV[i].interType].multip[j]>1){
				fprintf(FileOut, "*(1/%d!", I[pFeynDiag->fV[i].interType].multip[j]);
				factor*=factorial(I[pFeynDiag->fV[i].interType].multip[j]);
				break;}}
		if(j<I[pFeynDiag->fV[i].interType].totLegTypes){
			for(j++; j<I[pFeynDiag->fV[i].interType].totLegTypes; j++){
				if(I[pFeynDiag->fV[i].interType].multip[j]>1){
					fprintf(FileOut, "%d!", I[pFeynDiag->fV[i].interType].multip[j]);
					factor*=factorial(I[pFeynDiag->fV[i].interType].multip[j]);}}
			fprintf(FileOut, ")");}}
	if(factor>1)fprintf(FileOut, " = 1/%d\n", factor/=pFeynDiag->symmetryFactor);
	else fprintf(FileOut, "\n");
	fprintf(FileOut,"fermionSign=%d\n",pFeynDiag->fermionSign);
	fprintf(FileOut, "------------------------------------------------\n");
}

int judgeConnectivity(struct feynDiagram * pFeynDiag)
{
	int i,j,k;
	int group[MAX_I_VERTICES];
	int label[MAX_I_VERTICES];
	int totUnSettVert1,totUnSettVert2;
	/////////////////////////////////
	for(i=1; i<TotVertices; i++){
		group[i]=0;
		label[i]=0;}
	for(i=0; i<pFeynDiag->fV[0].totTo; i++){
		group[pFeynDiag->fV[0].toVert[i]]=1;}
	/////////////////////////////////
	totUnSettVert1=totUnSettVert2=TotVertices-1;
	do{
		for(i=1; i<TotVertices; i++){
			if(label[i]==0){
				if(group[i]==1){
					totUnSettVert2--;
					for(j=0; j<pFeynDiag->fV[i].totTo; j++){
						group[pFeynDiag->fV[i].toVert[j]]=1;}
					label[i]=1;}
				else{
					for(k=0; k<pFeynDiag->fV[i].totTo; k++){
						if(group[pFeynDiag->fV[i].toVert[k]]==1){
							totUnSettVert2--;
							group[i]=1;
							for(j=0; j<pFeynDiag->fV[i].totTo; j++){
								group[pFeynDiag->fV[i].toVert[j]]=1;}
							label[i]=1;
							break;}}}}}
		if(totUnSettVert2==0)return(1);
		if(totUnSettVert2==totUnSettVert1)return(0);
		totUnSettVert1=totUnSettVert2;
	}while(1);
	/////////////////////////////////
	return(0);
}

void finishADiag()
{
	if(judgeConnectivity(&FD)==1){
		fprintf(FileOut, "(%d)\n", ++FDCount);
		writeFeynDiag(&FD);}
}

int fermionAcrossSign(int startVN, int startVL, int endVN, int endVL)
{
	int sign=1;
	int i,j,jStart,jEnd;
	///////////////////////////////
	if(startVN<0){
		for(i=0; i<=endVN; i++){
			if(i==endVN)jEnd=endVL-1;
			else jEnd=I[FD.fV[i].interType].totLegTypes;
			for(j=0; j<=jEnd; j++){
				if(I[FD.fV[i].interType].legType[j].BorF=='F' && FD.fV[i].multip[j]==1)sign*=(-1);}}}
	else{
	///////////////////////////////
		for(i=startVN; i<=endVN; i++){
			if(i==startVN)jStart=startVL+1;
			else jStart=0;
			if(i==endVN)jEnd=endVL-1;
			else jEnd=I[FD.fV[i].interType].totLegTypes;
			for(j=jStart; j<=jEnd; j++){
				if(I[FD.fV[i].interType].legType[j].BorF=='F' && FD.fV[i].multip[j]==1)sign*=(-1);}}
		if(I[FD.fV[endVN].interType].legType[endVL].ID<0)sign*=(-1);}
	////////////////////
	return(sign);
}


void tryPairing(int bookMarkJump, int thisVN, int thisVL, int toVN, int toVL)
{
	if(FD.bookMark<TotExterParticles){
		FD.symmetryFactor*=FD.fV[toVN].multip[toVL];
		if(P[FD.bookMark].BorF=='F')FD.fermionSign*=fermionAcrossSign(-1, -1, toVN, toVL);
		FD.fP[FD.bookMark].toVert=toVN;
		FD.fP[FD.bookMark].toLegTyp=toVL;
		FD.fV[toVN].multip[toVL]--;}
	else{
		FD.symmetryFactor*=FD.fV[toVN].multip[toVL];
		if(I[FD.fV[thisVN].interType].legType[thisVL].BorF=='F')FD.fermionSign*=fermionAcrossSign(thisVN, thisVL, toVN, toVL);
		FD.fV[thisVN].toVert[FD.fV[thisVN].totTo]=toVN;
		FD.fV[thisVN].toLegTyp[FD.fV[thisVN].totTo]=toVL;
		FD.fV[thisVN].thisLegTyp[FD.fV[thisVN].totTo]=thisVL;
		FD.fV[thisVN].totTo++;
		FD.fV[toVN].multip[toVL]--;}
	FD.bookMark+=bookMarkJump;
}

void unTryPairing(int bookMarkJump, int thisVN, int thisVL, int toVN, int toVL)				//anti-operations to tryPairing(), i.e. resume to the state before the try. 
{
	FD.bookMark-=bookMarkJump;
	if(FD.bookMark<TotExterParticles){
		FD.fV[toVN].multip[toVL]++;
		FD.fP[FD.bookMark].toLegTyp=-1;
		FD.fP[FD.bookMark].toVert=-1;
		if(P[FD.bookMark].BorF=='F')FD.fermionSign/=fermionAcrossSign(-1, -1, toVN, toVL);
		FD.symmetryFactor/=FD.fV[toVN].multip[toVL];}
	else{
		FD.fV[toVN].multip[toVL]++;
		FD.fV[thisVN].totTo--;
		FD.fV[thisVN].thisLegTyp[FD.fV[thisVN].totTo]=-1;
		FD.fV[thisVN].toLegTyp[FD.fV[thisVN].totTo]=-1;
		FD.fV[thisVN].toVert[FD.fV[thisVN].totTo]=-1;
		if(I[FD.fV[thisVN].interType].legType[thisVL].BorF=='F')FD.fermionSign/=fermionAcrossSign(thisVN, thisVL, toVN, toVL);
		FD.symmetryFactor/=FD.fV[toVN].multip[toVL];}
}

void feynTraverse()
{
	int i,j,k,t,bookMarkJump;
	int thisVert=-1,thisLegTyp=-1;													//(thisVert,thisLegTyp) represents the field that is going to pair with another field;
	int thisID;																		//(thisID,thisBorF) describes the properties of the above field that is going to pair with another field;
	char thisBorF;
	int toVert0,toLegTyp0;															//(toVert0,toLegTyp0) represents the start point of the scanning procedure of the above field searching for another field to pair with.
	/////////////////////////////////////
	if(FD.bookMark<TotExterParticles){
		thisBorF=P[FD.bookMark].BorF;
		thisID=P[FD.bookMark].ID;
		bookMarkJump=1;
		toVert0=0;
		toLegTyp0=0;}
	else{																			//finding the (thisVert,thisLegTyp) field and the (toVert0,toLegTyp0) field
		for(i=(FD.bookMark-TotExterParticles); i<TotVertices&&thisVert==-1; i++){
			if(FD.fV[i].interType<0)return;											//Note: this means the diagrams must contains vacuum popo; we crudely stop the traversing here.
																					//The vacuum-to-vacuum diagrams are thus be excluded, please make modifications these diagrams are wanted.
			for(j=0; j<I[FD.fV[i].interType].totLegTypes; j++){
				if(FD.fV[i].multip[j]>0){
					thisVert=i;
					thisLegTyp=j;
					thisBorF=I[FD.fV[thisVert].interType].legType[thisLegTyp].BorF;
					thisID=I[FD.fV[thisVert].interType].legType[thisLegTyp].ID;
					if(I[FD.fV[thisVert].interType].legType[thisLegTyp].BorF!='U')thisID*=(-1);
					FD.fV[thisVert].multip[thisLegTyp]--;
					bookMarkJump=thisVert+TotExterParticles-FD.bookMark;
					toVert0  =thisVert;
					toLegTyp0=thisLegTyp;
					break;}}}
		//////////////////
		if(thisVert==-1){															//if (thisVert.thisLegTyp) is not find in the above, this means all the fields have been paired, i.e. a Feynman diagram is finished.
			finishADiag();
			return;}}
	//////////////////////////////////////
	for(i=toVert0; i<TotVertices; i++){												//This is the ordered pairing that can avoid the arising of identical Feynman diagrams from vertex-relabeling.
		if(FD.fV[i].interType>=0){
			k=FD.fV[i].interType;
			for(j=(i==toVert0?toLegTyp0:0); j<I[k].totLegTypes; j++){
				if(FD.fV[i].multip[j]>0 && thisBorF==I[k].legType[j].BorF && thisID==I[k].legType[j].ID){
					tryPairing(bookMarkJump, thisVert, thisLegTyp, i, j);
					feynTraverse();
					unTryPairing(bookMarkJump, thisVert, thisLegTyp, i, j);}}}
		else{																		//this means the interaction vertex for connecting is "new"
			for(k=0; k<TotInterTypes; k++){
				if(i>=TotVertices-(I[k].totLegs-3))continue;
				for(j=0; j<I[k].totLegTypes; j++){
					if(thisBorF==I[k].legType[j].BorF && thisID==I[k].legType[j].ID){
						FD.fV[i].interType=k;
						for(t=0; t<I[k].totLegTypes; t++)FD.fV[i].multip[t]=I[k].multip[t];
						TotVertices -= (I[k].totLegs-3);
						tryPairing(bookMarkJump, thisVert, thisLegTyp, i, j);
						feynTraverse();
						unTryPairing(bookMarkJump, thisVert, thisLegTyp, i, j);
						TotVertices += (I[k].totLegs-3);
						FD.fV[i].interType=-1;}}}
			break;}}
	/////////////////////////////////////////
	if(FD.bookMark>=TotExterParticles){												//additional operations that help resuming to the state before the connection try.
		FD.fV[thisVert].multip[thisLegTyp]++;}
}
