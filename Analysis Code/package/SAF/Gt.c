#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define N 4000
#define M 10//times of dump
main()
{   
    
    double press[N][3];
    int i,dump,j,k,dump2;
    float tiji,T,pre;
    struct autocre
    {
      double stress[N];
      int time[N];
    } gt;
    
    int read;
    size_t nbytes = 200;
    char *line;
    line= (char *)malloc(nbytes);
    
    
    FILE * data;
    FILE * result;
    data = fopen("Sauto.dat","r");
    result = fopen("str_cor_long.txt", "w");
    

    printf("what's the volume of system \n");
    scanf("%f",&tiji);
    printf("what's the temperature \n");
    scanf("%f",&T);
    
    pre=tiji/(3*T);
    printf("prefactor = %f\n", pre);
    
    read=getline(&line, &nbytes,data);
    read=getline(&line, &nbytes,data);
    read=getline(&line, &nbytes,data);
    
    free(line);

    for(i=0; i<N; i++)
    {   
	    gt.stress[i]=0;
    }

    for(k = 0; k <= M; k++)
    { 
      fscanf(data,"%d%d",&dump, &dump2);
      printf("%d\t%d\t %d\n",k, dump, dump2);
	    for(j=0; j< N; j++)
      {   
        fscanf(data,"%d%d%d%le%le%le", &i, &gt.time[j], &dump, &press[j][0],&press[j][1],&press[j][2]);
        gt.stress[j] = press[j][0]+press[j][1]+press[j][2];
        gt.stress[j] *= pre;
      }
    }
    
    double sum = 0;
    FILE *aveGt = fopen("ave_Gt.txt","w");
    for (j = 0; j < N; j++)
    {
      fprintf(result,"%d\t%lf\n",gt.time[j], gt.stress[j]);
      sum+=gt.stress[j];
      if((j+1)%10 == 0)
      {
	    fprintf(aveGt,"%d\t%lf\n",gt.time[j-5],sum/10);
	    sum = 0;
      }
    }
}

    
    
        
