listinc1<-cbind(1:100,list_inc[[1]])
colnames(listinc1)[1]<-"time"
listinc1<-melt(listinc1,id="time")

p <- ggplot(data = listinc1, aes(x = time, y = value, group = variable,color=variable))
p+geom_line()+scale_colour_manual(values=c(rep("#FF0000",1),rep(rgb(0,0,0,.1),29)))+
  theme(legend.position = "None")+xlab("Year") +
  ylab("HIV Incidence (per 100 person years)") 

