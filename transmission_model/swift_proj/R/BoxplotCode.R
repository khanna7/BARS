#Network Disruption Boxplot

load("JailedBoxPlot.Rdata")
jailed<-data.table(t(do.call(rbind,lapply(1:5,function(x){list_inc[[x]][40,]}))))
jailed$run<-1:30
jailed1<-melt(jailed,  id.vars = 'run', variable.name = 'series')
jailed1$series<-rep(c(".1",".25",".5",".75","1"),each=30)

load("JailedBoxPlotGen.Rdata")
inc<-data.table(t(do.call(rbind,lapply(1:5,function(x){list_inc2[[x]][40,]}))))
inc$run<-1:30
inc1<-melt(jailed,  id.vars = 'run', variable.name = 'series')
jailed1$series<-rep(c(".1",".25",".5",".75","1"),each=30)

gen<-rbind(jailed1,inc1)



load("Jailed95Simulation.Rdata")
load("Jailed95SimulationGen.Rdata")
gen.95<-rbind(quantile(jail.95[c(1:5)*40,],c(0.25,.975)),quantile(inc.95[c(1:5)*40,],c(0.25,.975)))
gen<-rbind(jailed1,inc1)
gen.95$series<-rep(c(".1",".25",".5",".75","1"),2)
gen.95$variable<-rep(c("Pre-Incarceration Partners","General"),each=2)
gen$variable<-rep(c("Pre-Incarceration Partners","General"),each=30)


gen$variable<-rep(c("Pre-Incarceration Partners","General Population"),each=150)
ggplot(gen, aes(x=series, y=value,fill=variable)) + geom_boxplot()+xlab("Reconnection Probability Multiplier")+ylab("HIV Incidence (per 100 person-years)")+labs(title=paste(""))+
  guides(color=guide_legend("Population")) +
  geom_point(data=gen.95,mapping=aes(x=series,y=value,group=variable),color="yellow",position=position_dodge2(width=.75))+geom_linerange(data=gen.95,mapping=aes(ymin=Two,ymax=NinetySeven,group=variable),position=position_dodge2(width=.75),color="yellow")


#Care Disruption Box Plot



load("RelBoxPlot.Rdata")
load("RelBoxPlotGen.Rdata")


released<-data.table(t(do.call(rbind,lapply(1:5,function(x){list_inc[[x]][40,]}))))
released$run<-1:30
released1<-melt(released,  id.vars = 'run', variable.name = 'series')

released1$series<-rep(c("Care Disruption: 90 Days","Care Disruption: 720 Days","(B) No Change in Care","(A) Targeted and Sustained Care"),each=30)

inc<-data.table(t(do.call(rbind,lapply(1:5,function(x){list_inc2[[x]][40,]}))))
inc$run<-1:30
inc1<-melt(inc,  id.vars = 'run', variable.name = 'series')

inc1$series<-rep(c("(C) Care Disruption: 90 Days","(D) Care Disruption: 720 Days","(B) No Change in Care","(A) Targeted and Sustained Care"),each=30)

gen<-rbind(released1,inc1)

load("Released95Simulation.Rdata")
load("Released95SimulationGen.Rdata")
gen.95<-rbind(quantile(jail.95[c(1:5)*40,],c(0.25,.975)),quantile(inc.95[c(1:5)*40,],c(0.25,.975)))
gen.95$series<-rep(c("(C) Care Disruption: 90 Days","(D) Care Disruption: 720 Days","(B) No Change in Care","(A) Targeted and Sustained Care"),2)
gen.95$variable<-rep(c("Post-Incarceration Partners","General"),each=2)
gen$variable<-rep(c("Pre-Incarceration Partners","General"),each=30)


ggplot(gen, aes(x=series, y=value,fill=variable)) + geom_boxplot()+xlab("Care Disruption Scenario")+ylab("HIV Incidence (per 100 person-years)")+labs(title=paste(""))+
  guides(color=guide_legend("Scenario")) +
  geom_point(data=gen.95,mapping=aes(x=series,y=value,group=variable),color="yellow",position=position_dodge2(width=.75))+geom_linerange(data=gen.95,mapping=aes(ymin=Two,ymax=NinetySeven,group=variable),position=position_dodge2(width=.75),color="yellow")

