#include <stdio.h>
#include <stdlib.h>

main(){
  FILE *in,*out;
  in=fopen("q2_stress.txt","r");
  out=fopen("q2_St_avg.txt","w");
  float gt[10001][5];
  float sum[5],buff[10001];
  int i,j;
  for(i=0;i<10001;i++){
    fscanf(in,"%f %f %f %f %f\n",&buff[i],&gt[i][0],&gt[i][1],&gt[i][2],&gt[i][3]);
  }
  for(i=0;i<=50;i++){
    fprintf(out,"%f %f %f %f %f\n",buff[i],gt[i][0],gt[i][1],gt[i][2],gt[i][3]);
  }
  for(i=51;i<1000;i++){
    sum[0]=0;
    sum[1]=0;
    sum[2]=0;
    sum[3]=0;
    for(j=0;j<5;j++){
      if(j==3){fprintf(out,"%f ",buff[i+j]);}
      sum[0]+=gt[i+j][0];
      sum[1]+=gt[i+j][1];
      sum[2]+=gt[i+j][2];
      sum[3]+=gt[i+j][3];
    }
    fprintf(out,"%f %f %f %f\n",sum[0]/5.0,sum[1]/5.0,sum[2]/5.0,sum[3]/5.0);
    i=i+j-1;
  }
  for(i=1000;i<=10000;i++){
    sum[0]=0;
    sum[1]=0;
    sum[2]=0;
    sum[3]=0;
    for(j=0;j<15;j++){
      if(j==8){fprintf(out,"%f ",buff[i+j]);}
      sum[0]+=gt[i+j][0];
      sum[1]+=gt[i+j][1];
      sum[2]+=gt[i+j][2];
      sum[3]+=gt[i+j][3];
    }   
    fprintf(out,"%f %f %f %f\n",sum[0]/15.0,sum[1]/15.0,sum[2]/15.0,sum[3]/15.0);
    i=i+j-1;
  }

}
