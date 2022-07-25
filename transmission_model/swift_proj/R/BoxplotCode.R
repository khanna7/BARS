#Network Disruption Boxplot

load("JailedBoxPlot.Rdata")
jailed<-data.table(t(do.call(rbind,lapply(1:5,function(x){list_inc[[x]][40,]}))))
jailed$run<-1:30
jailed1<-melt(jailed,  id.vars = 'run', variable.name = 'series')
jailed1$series<-rep(c(".1",".25",".5",".75","1"),each=30)

load("JailedBoxPlotGen.Rdata")
inc<-data.table(t(do.call(rbind,lapply(1:5,function(x){list_inc2[[x]][40,]}))))
inc$run<-1:30
inc1<-melt(inc,  id.vars = 'run', variable.name = 'series')
inc1$series<-rep(c(".1",".25",".5",".75","1"),each=30)

gen<-rbind(jailed1,inc1)



load("Jailed95Simulation.Rdata")
load("Jailed95SimulationGen.Rdata")
jail.95<-data.frame(jail.95)
jail.95gen<-data.frame(jail.95gen)
gen.95<-rbind(jail.95,jail.95gen)
names(gen.95)<-c("Two","NinetySeven","value")

gen$series<-rep(rep(c(".1",".25",".5",".75","1"),each=30),2)
gen.95$series<-rep(c(".1",".25",".5",".75","1"),2)
gen.95$Population<-rep(c("General","Pre-Incarceration Partners"),each=5)


gen$Population<-rep(c("General","Pre-Incarceration Partners"),each=150)
ggplot(gen, aes(x=series, y=value,fill=Population)) + geom_boxplot()+xlab("Reconnection Probability Multiplier")+ylab("HIV Incidence (per 100 person-years)")+labs(title=paste(""))+
  guides(color=guide_legend(title="Population")) +
  geom_point(data=gen.95,mapping=aes(x=series,y=value,group=Population),color="yellow",position=position_dodge2(width=.75))+geom_linerange(data=gen.95,mapping=aes(ymin=Two,ymax=NinetySeven,group=Population),position=position_dodge2(width=.75),color="yellow")


#Care Disruption Box Plot



load("RelBoxPlot.Rdata")
load("RelBoxPlotGen.Rdata")


released<-data.table(t(do.call(rbind,lapply(1:4,function(x){list_inc[[x]][40,]}))))
released$run<-1:30
released1<-melt(released,  id.vars = 'run', variable.name = 'series')

released1$series<-rep(c("(C) Care Disruption: 90 Days","(D) Care Disruption: 720 Days","(B) No Change in Care","(A) Targeted and Sustained Care"),each=30)

inc<-data.table(t(do.call(rbind,lapply(1:4,function(x){list_inc2[[x]][40,]}))))
inc$run<-1:30
inc1<-melt(inc,  id.vars = 'run', variable.name = 'series')

inc1$series<-rep(c("(C) Care Disruption: 90 Days","(D) Care Disruption: 720 Days","(B) No Change in Care","(A) Targeted and Sustained Care"),each=30)

gen<-rbind(released1,inc1)

load("Released95Simulation.Rdata")
load("Released95SimulationGen.Rdata")

rel.95<-data.frame(rel.95)
rel.95gen<-data.frame(rel.95gen)
gen.95<-rbind(rel.95,rel.95gen)
names(gen.95)<-c("Two","NinetySeven","value")

#gen.95<-rbind(quantile(jail.95[c(1:5)*40,],c(0.25,.975)),quantile(inc.95[c(1:5)*40,],c(0.25,.975)))
gen.95$series<-rep(c("(C) Care Disruption: 90 Days","(D) Care Disruption: 720 Days","(B) No Change in Care","(A) Targeted and Sustained Care"),2)
gen.95$Population<-rep(c("Post-Incarceration Partners","General"),each=4)
gen$Population<-rep(c("Post-Incarceration Partners","General"),each=120)


ggplot(gen, aes(x=series, y=value,fill=Population)) + geom_boxplot()+xlab("Care Disruption Scenario")+ylab("HIV Incidence (per 100 person-years)")+labs(title=paste(""))+
  guides(color=guide_legend("Scenario")) +
  geom_point(data=gen.95,mapping=aes(x=series,y=value,group=Population),color="yellow",position=position_dodge2(width=.75))+geom_linerange(data=gen.95,mapping=aes(ymin=Two,ymax=NinetySeven,group=Population),position=position_dodge2(width=.75),color="yellow")

