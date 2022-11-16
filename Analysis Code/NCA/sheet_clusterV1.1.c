/*UPDATA:ADDED VMD OUTPUT FOR EACH STEP*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct particle{
  int index;
  int mol;
  int type; 
  double cord[3];
  int group;
  int searched;
};

#define FALSE 0;
#define TRUE 1;

const int N_CHAIN=9000;
const int N_FILLER=19*25;
const int SIZE_CHAIN=30;
const int SIZE_FILLER=19;
const int TYPECHAIN=2;
const int TYPESTICKER=2;
const int TYPESHEET=4;
const double CUTOFF = 1.12;
const int CLUSTERSTART = 0;//start calculating after reading 50 times,100,000*20=2,000,000
const int CLUSTERSTOP = 120;
const int DUMPSPAN = 50000;//dump span

void skipline(int l,FILE *f)
{
  int read,i;
  size_t nbytes = 200;
  char *line;
  line= (char *)malloc(nbytes);
  for(i=0;i<l;i++){
    read=getline(&line, &nbytes,f);
    //printf("%s\n",line);
    if(read == 0){
    printf("ERROR:cleartile fail");
    exit(1);
    }
  }
}

double* readin(int n,struct particle* p,FILE *c,FILE *f,FILE *out)
{
  int i,count_chain=0,count_sheet=0;
  int CurrentStep;
  double *pdb=(double*)malloc(6*sizeof(double));

  //printf("DEBUG 2\n");
  skipline(9,f);
  skipline(1,c);
  fscanf(c,"%d \n",&CurrentStep);
  printf("%d\n",CurrentStep);
  skipline(3,c);
  fscanf(c,"%f %f %f %f %f %f \n",&pdb[0],&pdb[1],&pdb[2],&pdb[3],&pdb[4],&pdb[5]);
  skipline(1,c);
  for(i=0;i<N_CHAIN;i++){
    fscanf(c,"%d %d %d %lf %lf %lf \n",&p[i].index,&p[i].mol,&p[i].type,&p[i].cord[0],&p[i].cord[1],&p[i].cord[2]);
    if(p[i].type==TYPECHAIN||p[i].type==TYPESTICKER)
      count_chain++;
  }
  for(i=N_CHAIN;i<N_CHAIN+N_FILLER;i++){
    fscanf(f,"%d %d %d %lf %lf %lf \n",&p[i].index,&p[i].mol,&p[i].type,&p[i].cord[0],&p[i].cord[1],&p[i].cord[2]);
    if(p[i].type==TYPESHEET)
      count_sheet++;
  }
  if(count_sheet!=N_FILLER||count_chain!=N_CHAIN){
    printf("ERROR READ IN\n");
    exit(1);
  }
  
  fprintf(out,"%d\t",CurrentStep);
  return pdb;
}

void markall(int n,struct particle thisP,struct particle* p)
{
  int i;
  int count =0;
  for(i=0;i<n;i++){
    if(thisP.type == TYPESHEET){
      if(p[i].mol == thisP.mol && p[i].type == TYPESHEET){
	p[i].group = thisP.group;
	count++;
      }
    }
    else if(thisP.type == TYPESTICKER||thisP.type == TYPECHAIN){
      if(p[i].mol == thisP.mol && p[i].type != TYPESHEET){
	p[i].group = thisP.group;
	count++;
      }
    }
  }
  // printf("marked %d \n",count);
}

double dis(struct particle P1,struct particle P2,int BOUND)
{
  double distance;
  double minus[3];
  double halfbound = BOUND/2.0;
  int i;
  for(i=0;i<3;i++){
    minus[i] = fabs(P1.cord[i]-P2.cord[i]);
    if(minus[i]>halfbound){
      minus[i] = BOUND - minus[i];
    }
  }

  distance = minus[0]*minus[0]+minus[1]*minus[1]+minus[2]*minus[2];
  distance = sqrt(distance);
  return distance;

}

int search(int n,struct particle thisP,struct particle* p,double BOUND)
{
  int i;
  int count=0;
  if(thisP.type == TYPESHEET){
    for(i=0;i<n;i++){
      if(p[i].type == TYPESTICKER && p[i].group == 0){
	if(dis(p[i],thisP,BOUND) < CUTOFF){
	  p[i].group = thisP.group;
	  markall(n,p[i],p);
	  count++;
	  /*printf("%lf\n",dis(p[i],thisP));
	  printf("%d %d %lf %lf %lf\n",thisP.index,thisP.type,thisP.cord[0],thisP.cord[1],thisP.cord[2]);
	  printf("%d %d %lf %lf %lf\n",p[i].index,p[i].type, p[i].cord[0],p[i].cord[1],p[i].cord[2]);
	  getchar();*/
	}
      }
    }
  }
  else if(thisP.type == TYPESTICKER){
    for(i=0;i<n;i++){
      if(p[i].group == 0 && p[i].type == TYPESHEET){
	if(dis(p[i],thisP,BOUND) < CUTOFF){
	    p[i].group = thisP.group;
	    markall(n,p[i],p);
	    count++;
	    /*printf("%lf\n",dis(p[i],thisP));
	    printf("%d %d %lf %lf %lf\n",thisP.index,thisP.type,thisP.cord[0],thisP.cord[1],thisP.cord[2]);
	    printf("%d %d %lf %lf %lf\n",p[i].index,p[i].type, p[i].cord[0],p[i].cord[1],p[i].cord[2]);
	    getchar();*/
	  }
      }
    }
  }
  return count; 
} 

void cluster(int n,struct particle* p,double bound)
{
  int i;
  int succ_search = 1;
  int group = 0;
  int notgrouped = 1;
  for(i=0;i<n;i++){
    p[i].group = 0;
    p[i].searched=0;
  }

  while( notgrouped != 0){    
    group++;

    for(i=0;i<n;i++){
      if(p[i].group==0){
	p[i].group = group;
	//printf("%d\n",group);
	markall(n,p[i],p);
	break;
      }
    }

    succ_search = 1; 
    while(succ_search != 0){
      succ_search = 0;
      for(i=0;i<n;i++){
	if(p[i].group == group&&p[i].searched==0){
	  succ_search += search(n,p[i],p,bound);
	  p[i].searched=1;
	}
      }
      //printf("%d successful search tried\n",succ_search);
    }

    notgrouped=0;
    for(i=0;i<n;i++){
      if(p[i].group == 0)
	notgrouped++;
      }
  }
}

//vmd1 for chain,vmd 2 for filler 
void vmdcluster(int parentcase,int groupid, struct particle *p,FILE *parentVMD1,FILE *parentVMD2)
{
  int i,thiscase;
  char *type;
  type=(char*)malloc(10*sizeof(char));

  for(i=0;i<N_CHAIN+N_FILLER;i++)
    {
      if(p[i].group == groupid){ //only out put if group ID match
	if(p[i].type == TYPESHEET){
	  thiscase = 1;
	}
	else{
	  thiscase = parentcase;
	}
	
	switch(thiscase)
	  {
	  case 1:type="H";break;
	  case 2:type="O";break;
	  case 3:type="N";break;
	  case 4:type="C";break;
	  case 5:type="S";break;
	  case 6:type="P";break;
	  case 7:type="Z";break;
	  case 8:type="LPA";break;
	  case 9:type="L";break;
	  case 10:printf("WARNING, color method\n");break;
	  }
	if(p[i].type == TYPESHEET)
	  fprintf(parentVMD2,"%s %lf %lf %lf\n",type,p[i].cord[0],p[i].cord[1],p[i].cord[2]);      
	else 
	  fprintf(parentVMD1,"%s %lf %lf %lf\n",type,p[i].cord[0],p[i].cord[1],p[i].cord[2]);     
      }
    }
}

void writeinfo(int n,struct particle *p,FILE *d,int step)
{
  int i;
  int group = 0,findgroup = 1;
  int LargeCluster=2;//start from 2, LargeCluster = color case, color case 1 and 2 reserved

  FILE *vmd1,*vmd2;//vmd1 for chain, vmd2 for filler
  vmd1=fopen("vmd_chain.xyz","a");
  vmd2=fopen("vmd_filler.xyz","a");
  //fprintf(vmd2,"\n");//new line for 
  fprintf(vmd2,"%d\n",N_FILLER);
  fprintf(vmd2,"Atoms. Timestep: %d\n",step*DUMPSPAN);
  fprintf(vmd1,"%d\n",N_CHAIN);
  fprintf(vmd1,"Atoms. Timestep: %d\n",step*DUMPSPAN);  

  while(findgroup > 0){
    findgroup = 0;
    group++;    
    for(i=0;i<n;i++)
      {
	if(p[i].group == group)
	  findgroup++;
      }

    if(findgroup!=0)
      fprintf(d,"%d\t",findgroup);

    if(findgroup<=SIZE_CHAIN+SIZE_FILLER){
      vmdcluster(2, group ,p,vmd1,vmd2);//color 1 reserved for sheet
    }

    else {
      LargeCluster++;
      vmdcluster(LargeCluster, group,p,vmd1,vmd2);
    }
  }
  if(LargeCluster >10){
    printf("ERROR: NUMBER OF CLUSTER EXCEED COLOR ID LIMIT\n");
    exit(1);
  }
  
  fprintf(d,"\n");//space line for file cluster_to_time
  fclose(vmd1);
  fclose(vmd2);
}


main()
{
  int n=N_CHAIN+N_FILLER;
  double *b,bound;
  struct particle* p;
  p = (struct particle*) malloc(n*sizeof(struct particle));

  printf("The group CUTOFF judge is set as %lf \n",CUTOFF);
  printf("CHECK WHERE ATTRACTION START AND STOP\n");
  printf("CURRENTLY START AT %d STOP AT %d\n",CLUSTERSTART*DUMPSPAN,CLUSTERSTOP*DUMPSPAN);
  getchar();

  FILE *datafiller,*datachain,*writedata;
  datafiller = fopen("filler_after.lammpstrj","r");
  datachain = fopen("chains_after.lammpstrj","r");
  writedata = fopen("clustering_time.txt","w");

  int ReadInCount=0;
  while(!feof(datafiller)&&!feof(datachain)){
    b=readin(n,p,datachain,datafiller,writedata);
    if(ReadInCount>=CLUSTERSTART&&ReadInCount<=CLUSTERSTOP+1){
      bound=b[1]-b[0];
      cluster(n,p,bound);
      writeinfo(n,p,writedata,ReadInCount);
    }
    ReadInCount++;
  }
  
  free(p);
  fclose(datachain);
  fclose(writedata);
  fclose(datafiller);
}
