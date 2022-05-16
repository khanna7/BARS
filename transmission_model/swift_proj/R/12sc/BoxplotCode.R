load("JailedBoxPlot.Rdata")
jailed<-data.table(t(do.call(rbind,lapply(1:5,function(x){list_inc[[x]][40,]}))))
jailed$run<-1:30
jailed1<-melt(jailed,  id.vars = 'run', variable.name = 'series')


jailed1$series<-rep(c(.1,25,.5,.7,1),each=30)
jailed1$series<-rep(c(".1",".25",".5",".75","1"),each=30)
p<-ggplot(jailed1,aes(y=value,group=series,color=series))
p<-p+geom_boxplot()+ylab("HIV Incidence")+  guides(color=guide_legend("Scenarios")) +guides(fill=FALSE)+labs(title=paste("Pre-Incarceration Partners"))+
ylim(0,NA)  

load("RelBoxPlot.Rdata")


released<-data.table(t(do.call(rbind,lapply(1:5,function(x){list_inc2[[x]][40,]}))))
released$run<-1:30
released1<-melt(released,  id.vars = 'run', variable.name = 'series')

released1$series<-rep(c(.1,25,.5,.7,1),each=30)
released1$series<-rep(c(".1",".25",".5",".75","1"),each=30)
p1<-ggplot(released1,aes(y=value,group=series,color=series))
p1<-p1+geom_boxplot()+ylab("HIV Incidence")+  guides(color=guide_legend("Scenarios")) +guides(fill=FALSE)+labs(title=paste("Post-Incarceration Partners")) 

inc1<-data.table(t(do.call(rbind,lapply(1:5,function(x){list_inc[[x]][40,]}))))
inc1$run<-1:30
inc1<-melt(inc1,  id.vars = 'run', variable.name = 'series')

inc1$series<-rep(c(.1,25,.5,.7,1),each=30)
inc1$series<-rep(c(".1",".25",".5",".75","1"),each=30)
p2<-ggplot(inc1,aes(y=value,group=series,color=series))
p2<-p2+geom_boxplot()+ylab("HIV Incidence")+  guides(color=guide_legend("Scenarios")) +guides(fill=FALSE)+labs(title=paste("General")) 


multiplot <- function(..., plotlist=NULL, file, cols=1, layout=NULL) {
  library(grid)
  
  # Make a list from the ... arguments and plotlist
  plots <- c(list(...), plotlist)
  
  numPlots = length(plots)
  
  # If layout is NULL, then use 'cols' to determine layout
  if (is.null(layout)) {
    # Make the panel
    # ncol: Number of columns of plots
    # nrow: Number of rows needed, calculated from # of cols
    layout <- matrix(seq(1, cols * ceiling(numPlots/cols)),
                     ncol = cols, nrow = ceiling(numPlots/cols))
  }
  
  if (numPlots==1) {
    print(plots[[1]])
    
  } else {
    # Set up the page
    grid.newpage()
    pushViewport(viewport(layout = grid.layout(nrow(layout), ncol(layout))))
    
    # Make each plot, in the correct location
    for (i in 1:numPlots) {
      # Get the i,j matrix positions of the regions that contain this subplot
      matchidx <- as.data.frame(which(layout == i, arr.ind = TRUE))
      
      print(plots[[i]], vp = viewport(layout.pos.row = matchidx$row,
                                      layout.pos.col = matchidx$col))
    }
  }
}


multiplot(p,p1,p2,cols=3)


gen<-rbind(jailed1,released1,inc1)
gen$variable<-rep(c("Pre-Incarceration Partners","Post-Incarceration Partners","Overall"),each=150)

ggplot(gen,aes(y=value,group=series,color=series))+geom_boxplot()+ylab("HIV Incidence")+  guides(color=guide_legend("Scenarios")) +guides(fill=FALSE)+labs(title=paste("Network Disruption Scenarios")) +facet_wrap(~variable)+theme(axis.ticks.x = element_blank(),axis.text.x = element_blank())


inc1<-data.table(t(do.call(rbind,lapply(1:4,function(x){list_inc[[x]][40,]}))))
inc1$run<-1:30
inc1<-melt(inc1,  id.vars = 'run', variable.name = 'series')

inc1$series<-rep(c("Disruption: 90 Days",".Disruption: 720 Days","No Disruption","Targeted and Sustained Care"),each=30)

jailed<-data.table(t(do.call(rbind,lapply(1:4,function(x){list_inc[[x]][40,]}))))
jailed$run<-1:30
jailed1<-melt(jailed,  id.vars = 'run', variable.name = 'series')

jailed1$series<-rep(c("Disruption: 90 Days",".Disruption: 720 Days","No Disruption","Targeted and Sustained Care"),each=30)

released<-data.table(t(do.call(rbind,lapply(1:4,function(x){list_inc[[x]][40,]}))))
released$run<-1:30
released1<-melt(released,  id.vars = 'run', variable.name = 'series')
released1$series<-rep(c("Disruption: 90 Days",".Disruption: 720 Days","No Disruption","Targeted and Sustained Care"),each=30)





gen.95<-rbind(jail.95[c(1:4)*40,],prepost.95[c(1:4)*40,],rel.95[c(1:4)*40,])
gen.95$variable<-rep(c("(A) Pre-Incarceration Partners Only","(B) Pre&Post Release Partners","(C) Post Release Partners Only","(D) General Population"),each=4)
gen.95$series<-rep(c("Disruption: 90 Days",".Disruption: 720 Days","No Disruption","Targeted and Sustained Care"),3)
gen<-rbind(jailed1,inc1,released1)
gen$variable<-rep(c("(A) Pre-Incarceration Partners Only","(B) Pre&Post Release Partners","(C) Post Release Partners Only"),each=120)
ggplot(gen,aes(y=value,group=series,color=series))+geom_boxplot()+ylab("HIV Incidence")+  guides(color=guide_legend("Scenarios")) +guides(fill=FALSE)+labs(title=paste("Care Disruption Scenarios")) +facet_wrap(~variable)+#+theme(axis.ticks.x = element_blank(),axis.text.x = element_blank())
geom_line(aes(y=gen.95$value,group=gen.95$series,color=gen.95$series,facet_wrap(~gen.95$variable)))
      
names(gen.95)[c(4,5)]<-c("Two","NinetySeven")
ggplot(gen[c(1:150,301:450),],mapping=aes(x=series,y=value,group=series,color=series))+geom_boxplot()+xlab("")+ylab("HIV Incidence")+  guides(color=guide_legend("Scenarios")) +guides(fill=FALSE)+labs(title=paste("Care Disruption Scenarios")) +facet_wrap(~variable)+theme(axis.ticks.x = element_blank(),axis.text.x = element_blank())+
geom_point(data=gen.95,mapping=aes(x=series,y=value),color="black")+geom_linerange(data=gen.95,mapping=aes(ymin=Two,ymax=NinetySeven),color="black")

ggplot(gen,mapping=aes(x=series,y=value,group=series,color=series))+geom_boxplot()+xlab("")+ylab("HIV Incidence")+  guides(color=guide_legend("Scenarios")) +guides(fill=FALSE)+labs(title=paste("Care Disruption Scenarios")) +facet_wrap(~variable)+theme(axis.ticks.x = element_blank(),axis.text.x = element_blank())+
  geom_point(data=gen.95,mapping=aes(x=series,y=value),color="black")+geom_linerange(data=gen.95,mapping=aes(ymin=Two,ymax=NinetySeven),color="black")



gen<-rbind(jailed1,inc1,released1)
gen.95$series<-rep(c(".1",".25",".5",".75","1"),3)


gen.95<-rbind(jail.95[c(1:5)*40,],prepost.95[c(1:5)*40,],rel.95[c(1:5)*40,])
gen$variable<-rep(c("(A) Pre-Incarceration Partners Only","(B) Pre&Post Release Partners","(C) Post Release Partners Only","(D) General Population"),each=150)

#+geom_ribbon(data=gen.95,mapping=aes(ymin = Two, ymax = NinetySeven), colour = "black", alpha = 0.1)

ggplot(gen,mapping=aes(x=series,y=value,group=series,color=series))+geom_boxplot()+xlab("")+ylab("HIV Incidence")+  guides(color=guide_legend("Scenarios")) +guides(fill=FALSE)+labs(title=paste("Care Disruption Scenarios")) +facet_wrap(~variable)+theme(axis.ticks.x = element_blank(),axis.text.x = element_blank())+

  
gen.95$variable<-rep(c("(A) Pre-Incarceration Partners Only","(B) Pre&Post Release Partners","(C) Post Release Partners Only"),each=5)

overall<-data.table(t(do.call(rbind,lapply(1:5,function(x){list_inc[[x]][40,]}))))
overall$run<-1:30
overall1<-melt(overall,  id.vars = 'run', variable.name = 'series')

load("/project2/khanna7/francis/sch_inter/BARS/transmission_model/swift_proj/experiments/12sc/NetworkDisruptionFinalYear.Rdata")
ggplot(gen[c(1:150,301:450),], aes(x=series, y=value,fill=variable)) + geom_boxplot()+xlab("Network Disruption Parameter")+ylab("HIV Incidence")+labs(title=paste("Network Disruption Scenarios"))+
  guides(color=guide_legend("Scenarios")) +
  geom_point(data=gen.95,mapping=aes(x=series,y=value),color="black")+geom_linerange(data=gen.95,mapping=aes(ymin=Two,ymax=NinetySeven,group=variable),color="black")+facet_wrap(~variable)


ggplot(gen, aes(x=series, y=value,fill=Population)) + geom_boxplot()+xlab("")+ylab("HIV Incidence (per 100 person-years)")+labs(title=paste(""))+
  guides(color=guide_legend("Population")) +
  geom_point(data=gen.95,mapping=aes(x=series,y=value,group=Population),color="yellow",position=position_dodge2(width=.75))+geom_linerange(data=gen.95,mapping=aes(ymin=Two,ymax=NinetySeven,group=variable),position=position_dodge2(width=.75),color="yellow")



ggplot(gen[c(1:150,301:450),], aes(x=series, y=value,fill=Population)) + geom_boxplot()+xlab("Reconnection Probability Multiplier")+ylab("HIV Incidence (per 100 person-years)")+labs(title=paste(""))+
  guides(color=guide_legend("Population")) +
  geom_point(data=gen.95,mapping=aes(x=series,y=value,group=Population),color="yellow",position=position_dodge2(width=.75))+geom_linerange(data=gen.95,mapping=aes(ymin=Two,ymax=NinetySeven,group=Population),position=position_dodge2(width=.75),color="yellow")



overall1$series<-rep(c(".1",".25",".5",".75","1"),each=30)
overall1$variable<-"(D) General Population"


ggplot(gen,mapping=aes(x=series,y=value,group=series,color=series))+geom_boxplot()+xlab("")+ylab("HIV Incidence")+  guides(color=guide_legend("Scenarios")) +guides(fill=FALSE)+labs(title=paste("Network Disruption Scenarios")) +facet_wrap(~variable)+theme(axis.ticks.x = element_blank(),axis.text.x = element_blank())+
  geom_point(data=gen.95,mapping=aes(x=series,y=value,group=variable),color="black")+geom_linerange(data=gen.95,mapping=aes(ymin=Two,ymax=NinetySeven),color="black")
