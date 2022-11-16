#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 50 // number of zones


void cleartitle(int l,FILE *f) //l=number of line
{
  int read,i;
  size_t nbytes = 200;
  char *line;
  line= (char *)malloc(nbytes);
  for(i=0;i<l;i++){
    read=getline(&line, &nbytes,f);
    printf("%s\n",line);
    if(read == 0){
    printf("ERROR:cleartile fail");
    exit(1);
    }
  }
}

main()
{ float zone[N];
  double percent[N];
  int i,index;
  double buf;

  int buf1,buf2;
  FILE *data;
  data=fopen("tmpAB.rdf","r");
  FILE *output;
  output=fopen("radialAB.txt","w");

  int c;
  cleartitle(3,data);
  while(!feof(data)){
    fscanf(data,"%d %d",&buf1,&buf2);
    printf("%d  %d\n",buf1,buf2);
    for(i=0;i<N;i++){
      fscanf(data,"%d%f%lf%lf",&index,&zone[i],&percent[i],&buf);
    }
  }
  for(i=0;i<N;i++){
      fprintf(output,"%f \t %lf\n", zone[i],percent[i]);
  }
  fclose(data);
  fclose(output);
}
