
vec<-c(562,562,372,369,369,343,277,267,172,172,NA,160,160,160,147,147,147,135,102,102,100,100,98,80,NA,NA)

set.seed(1000)
EmpiricalBootstrap<-function(){
a<-sample(vec,27,replace=TRUE)

b<-c(80,98,100,102,135,147,160,172,267,277,343,369,372,562)

d<-unlist(lapply(b,function(x){sum((x)<=a,na.rm=TRUE)}))
e<-d/max(d)
return(e)
}

f<-do.call(cbind,lapply(1:10,function(i){EmpiricalBootstrap()}))
lb<-c(1,apply(f,1,quantile,.025),0)
ub<-c(1,apply(f,1,quantile,.975),0)