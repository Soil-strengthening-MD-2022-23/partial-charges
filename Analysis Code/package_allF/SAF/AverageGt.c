#include <stdio.h>
#include <stdlib.h>

main(){
  FILE *in,*out;
  in=fopen("q2_aggregate.txt","r");
  out=fopen("q2_Gt_avg.txt","w");
  float gt[4001][5];
  float sum[5],buff[4001];
  int i,j;
  for(i=0;i<4001;i++){
    fscanf(in,"%f %f %f %f %f\n",&buff[i],&gt[i][0],&gt[i][1],&gt[i][2],&gt[i][3]);
  }
  for(i=0;i<=40;i++){
    fprintf(out,"%f %f %f %f %f\n",buff[i],gt[i][0],gt[i][1],gt[i][2],gt[i][3]);
  }
  for(i=41;i<1000;i++){
    sum[0]=0;
    sum[1]=0;
    sum[2]=0;
    sum[3]=0;
    for(j=0;j<5;j++){
      if(j==1){fprintf(out,"%f ",buff[i+j]);}
      sum[0]+=gt[i+j][0];
      sum[1]+=gt[i+j][1];
      sum[2]+=gt[i+j][2];
      sum[3]+=gt[i+j][3];
    }
    fprintf(out,"%f %f %f %f\n",sum[0]/5.0,sum[1]/5.0,sum[2]/5.0,sum[3]/5.0);
    i=i+j-1;
  }
  for(i=1000;i<=4000;i++){
    sum[0]=0;
    sum[1]=0;
    sum[2]=0;
    sum[3]=0;
    for(j=0;j<10;j++){
      if(j==5){fprintf(out,"%f ",buff[i+j]);}
      sum[0]+=gt[i+j][0];
      sum[1]+=gt[i+j][1];
      sum[2]+=gt[i+j][2];
      sum[3]+=gt[i+j][3];
    }   
    fprintf(out,"%f %f %f %f\n",sum[0]/10.0,sum[1]/10.0,sum[2]/10.0,sum[3]/10.0);
    i=i+j-1;
  }

}
