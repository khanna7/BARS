rm(list=ls())
#Incidence for the Calibration
#In order to run this, you need to change the Drug config file to run for 100 years
#The git commit Id on the substance use branch is 0fb345dcaba1ef42bcb0b22cc361f8d0cb33062f
#Note that this must also be used in order to get the baseline incidence
source("summarize_functions_ext6.R")
range <- c(18, 25, 26, 34)
dir_name ="../../experiments/PLACEHOLDER" #Replace PLACEHOLDER with whatever experiment name is used for the calibration
sim_instance_dir <- paste0(dir_name, "/instance_")
sc_nb= 1
rseed_nb=30
sim_years=100
year_interv_start =0
list_inc_means = list()

list_inc = list()



for (j in 1: (sc_nb)) {
  next_sc <- (j-1)*rseed_nb
  res = list()  
  res_ind=1
  for(i in next_sc:(next_sc+rseed_nb-1)){              
    dir_loc <- paste0(sim_instance_dir,toString(i+1))
    res.dt <- summarize_yearly_inc(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res[[res_ind]] <- res.dt$full_inc_mean#res.dt$full_inc_mean
    res_ind <- res_ind+1
  }
  res.df  <- as.data.frame(res)


  names(res.df) <- seq(1:(rseed_nb))
  
          
  list_inc_means[[j]] <-rowMeans(res.df)
  list_inc[[j]] <-res.df
}



listinc1<-cbind(1:100,list_inc[[1]])
colnames(listinc1)[1]<-"time"
listinc1<-melt(listinc1,id="time")
p <- ggplot(data = listinc1, aes(x = time, y = value, group = variable,color=variable))
p+geom_line()+scale_colour_manual(values=c(rep("#FF0000",1),rep(rgb(0,0,0,.1),29)))+
  theme(legend.position = "None")+xlab("Year") +
  ylab("HIV Incidence (per 100 person years)") 
Calibration<-listinc1


save(Calibration,file="Calibration.Rdata")
