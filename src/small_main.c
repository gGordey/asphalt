#include<stdio.h>
#define p(w)(w>>8)%16+(w>>16)%16*10+(w>>24)%16*100
#define P(l)(d[c+l]%256>57?s[p(d[c+l])]:p(d[c+l]))
#define _(o)P(2)o P(4):d[c+3]>>24==
#define $;d[c]==
#define W;while(d[c]!='NDCE')
c,i,b,d[9995],s[999];main(){FILE*f=fopen("a","r")W(b=fgetc(f))>32&&(d[c]+=b<<24*i++)||(c+=i%4<1);c=0 W{$'MPIJ'&&P(1)&&(c=P(2)*5)$'RNTP'&&printf(d[c+1]=='HARC'?"%c":"%d",P(2))$'ETVS'&&(s[P(1)]=d[c+3]>>24==43?_(+)45?_(-)47?_(/)37?_(%)42?_(*)60?_(<)33?!P(2):0)$'ETVG'&&(s[P(1)]=s[P(2)]);c+=5;}}