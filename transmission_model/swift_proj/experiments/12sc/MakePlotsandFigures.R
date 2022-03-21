library(data.table)
load("/project2/khanna7/francis/subuse/BARS1/BARS/transmission_model/swift_proj/experiments/12sc/BMandBCLabels.Rdata")
e<-d[c(1:50,141:180),]
e$series<-c(rep("1. Baseline",10),rep("2. 25%",10),rep("3. 50%",10),rep("4. 75%",10),rep("5. 100%",10),rep("2. 10%",10),rep("3. 15%",10),rep("4. 20%",10),rep("5. 25%",10))
library(ggplot2)
e1<-e[c(1:50),]
e2<-e[c(1:10,51:90),]




Baseline.1<-data.frame(cbind(rep(0,5),c("1. Baseline","2. 25%","3. 50%","4. 75%","5. 100%"),rep(6.965,5),rep(6.965,5),rep(6.965,5)),stringsAsFactors = FALSE)
names(Baseline.1)<-c("time","series","value","Two","NinetySeven")
Baseline.1$time<-as.numeric(Baseline.1$time)
Baseline.1$value<-as.numeric(Baseline.1$value)
Baseline.1$Two<-as.numeric(Baseline.1$Two)
Baseline.1$NinetySeven<-as.numeric(Baseline.1$NinetySeven)

Baseline.2<-data.frame(cbind(rep(0,5),c("1. Baseline","2. 10%","3. 15%","4. 20%","5. 25%"),rep(6.965,5),rep(6.965,5),rep(6.965,5)),stringsAsFactors = FALSE)
names(Baseline.2)<-c("time","series","value","Two","NinetySeven")
Baseline.2$time<-as.numeric(Baseline.2$time)
Baseline.2$value<-as.numeric(Baseline.2$value)
Baseline.2$Two<-as.numeric(Baseline.2$Two)
Baseline.2$NinetySeven<-as.numeric(Baseline.2$NinetySeven)

e1<-rbind(Baseline.1,e1)
e2<-rbind(Baseline.2,e2)

ggplot(e1, aes(x = time,  color = series, fill = series)) + 
  xlab("Year") +
  ylab("Average Incidence Rate") +
  ylim(4, 8) +
  geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
  geom_line(aes(y= value))+
  guides(color=guide_legend("Mirtazapine Initiation Prop."))  +
  guides(fill=FALSE)


ggplot(e2, aes(x = time,  color = series, fill = series)) + 
  xlab("Year") +
  ylab("Average Incidence Rate") +
  ylim(4, 8) +
  geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
  geom_line(aes(y= value))+
  guides(color=guide_legend("Residential Rehabilitation Prop."))  +
  guides(fill=FALSE)

names(e)[c(4,5)]<-c("Two","NinetySeven")


paste0(round(e[c(1:9)*10,]$value,2)," (",round(e[c(1:9)*10,]$Two,2),",",round(e[c(1:9)*10,]$NinetySeven,2),")")


paste0(round(e[c(1:9)*10,]$value,1)," (",round(e[c(1:9)*10,]$Two,1),",",round(e[c(1:9)*10,]$NinetySeven,1),")")

paste0(round(e[c(1:4)*10,]$value,1)," (",round(e[c(1:4)*10,]$Two,1),",",round(e[c(1:4)*10,]$NinetySeven,1),")")

paste0(round(e[c(1:4)*10,]$value,2)," (",round(e[c(1:4)*10,]$Two,2),",",round(e[c(1:4)*10,]$NinetySeven,2),")")

paste0(round(e[c(1:5)*10,]$value,2)," (",round(e[c(1:5)*10,]$Two,2),",",round(e[c(1:5)*10,]$NinetySeven,2),")")

