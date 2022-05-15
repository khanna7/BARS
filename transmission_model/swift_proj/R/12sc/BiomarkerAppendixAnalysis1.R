library(data.table)
#We want to ask the question, on day x after getting released, what is the average viral load (y)?

BMA<-function(i){
  a<-fread(paste0("/project2/ahotton/francis/sch_inter/BARS/transmission_model/swift_proj/experiments/BiomarkerAppendix/instance_",i,"/output/biomarker_log.csv"))
  a1<-a[in_jail==0][(tick-days_since_release)>10950][,.(acute=days_since_infection<90,viral_load,days_since_release)]
  a1$r<-(a1$acute*4+1)*0.00092*2.89^(a1$viral_load-4.2)
  #a2<-tapply(a1$viral_load,a1$art_status,mean)
  a2<-tapply(a1$r,a1$days_since_release,mean)
  print(i)
  return(a2)
}

BMAResults<-lapply(1:60,BMA)
mat1<-matrix(NA,nrow=60,ncol=3568)
for(i in 1:60){
  mat1[i,]<-BMAResults[[i]][1:3568]
}

vecb<-unlist(lapply(1:10,function(i){
  a<-fread(paste0("/project2/khanna7/francis/sch_inter/BARS/transmission_model/swift_proj/experiments/BioMarker8/instance_",i,"/output/biomarker_log.csv"))
  b<-a[never_jailed==1][,.(mean(viral_load))]
  return(b)
}))

x<-1500
BMR1<-mat1[1:30,]
BMR2<-mat1[31:60,]
a2<-apply(BMR1,2,mean)[1:x]
a2sd<-apply(BMR1,2,sd)[1:x]
plot(1:length(a2),a2,main="Average Transmission Probability",xlab="Days Since Last Release",ylab="Average Transmission Probability (per sex act)",xlim=c(1,x),type="l",col="red",ylim=c(0,0.002)
)
a3<-apply(BMR2,2,mean)[1:x]
a3sd<-apply(BMR2,2,sd)[1:x]
lines(1:length(a3),a3,col="blue")
polygon(c(1:length(a2),length(a2):1),c(a2+a2sd,rev(a2-a2sd)),col=rgb(1,0,0,.1),border=NA)
polygon(c(1:length(a3),length(a3):1),c(a3+a3sd,rev(a3-a3sd)),col=rgb(0,0,1,.1),border=NA)
#a[never_jailed==1][,.(mean(viral_load))]
b<-.00092*2.89^(3.460476-4.2) #Derived from Biomarker8 vecb

#.00092*2.89^(3.459453+.001825063-4.2)
#.00092*2.89^(3.459453-.001825063-4.2)





.00092*2.89^(2.3103-4.2)
.00092*2.89^(4.22135-4.2)
polygon(c(-100:3500,3500:-100),c(rep(.00092*2.89^(3.44902-4.2),3601),rep(.00092*2.89^(3.47074-4.2),3601)),col=rgb(0,1,0,.1),border=NA )

#lines(1:3568,b,col="green")
abline(h=b,col="green",lty=5)
legend("topleft",c("Mean Care Disruption: 90 Days","Mean Care Disruption: 720 Days","Never Jailed"),col=c("red","blue","green"),lty=c(1,1,5),lwd=c(3,3,1))



justinfected<-a[days_since_infection==1,.(p_id,never_jailed)][never_jailed==1,p_id]




BMR<-do.call(cbind,BMAResults)
mat1<-matrix(NA,nrow=3650,ncol=10)
mat1<-matrix(NA,nrow=200,ncol=10)
for(i in 1:10){
  mat1[,i]<-BMAResults[[i]][1:200]
}