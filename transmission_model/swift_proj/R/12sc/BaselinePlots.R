#rm(list=ls())

source("summarize_functions_ext6.R")
range <- c(18, 25, 26, 34)
dir_name ="../../experiments/CareInterventionPlaceholder" #Running the Care intervention placeholder experiment directory because the first scenario is the baseline scenario 90 day disruption  
sim_instance_dir <- paste0(dir_name, "/instance_")
sc_nb= 1
rseed_nb=30
sim_years=40
year_interv_start =0
list_inc_means = list()
list_res2_means = list_res3_means =list()
list_inc = list()
list_inc2 = list()
list_inc3 = list()

quantilelist<-quantilelist1<-quantilelist2<-vector(mode="list",length=sc_nb)

summarize_yearly_inc_released_partners <- function(filename="counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*(infected_released_partner[2:.N])/(uninfected_released_partner[1:(.N-1)]))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
}

summarize_yearly_inc_jailed_partners <- function(filename="counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*(infected_jailed_partner[2:.N])/(uninfected_jailed_partner[1:(.N-1)]))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
}

summarize_yearly_inc_rel_only <- function(filename="counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*((infected_released_partner_only[2:.N])/(uninfected_released_partner_only[1:(.N-1)])))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
}

summarize_yearly_inc_onemomentary <- function(filename="counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*((infected_not_pre_incarceration_not_post_release_at_least_one_partner[2:.N])/(uninfected_not_pre_incarceration_not_post_release_at_least_one_partner[1:(.N-1)])))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
}

summarize_yearly_inc_jail_only <- function(filename="counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*((infected_jailed_partner_only[2:.N])/(uninfected_jailed_partner_only[1:(.N-1)])))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
}

summarize_yearly_inc_jailrel_only <- function(filename="counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*((infected_jailed_and_released_partner[2:.N])/(uninfected_jailed_and_released_partner[1:(.N-1)])))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
}

for (j in 1: (sc_nb)) {
  next_sc <- (j-1)*rseed_nb
  res = list()  
  res2 = list() 
  res3=list
  res_ind=1
  for(i in next_sc:(next_sc+rseed_nb-1)){              
    dir_loc <- paste0(sim_instance_dir,toString(i+1))
    print(dir_loc)
    #				res.dt   <- summarize_yearly_inc_released_partners(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    #res.dt   <- summarize_yearly_prev_ever_jailed1(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res2.dt   <- summarize_yearly_inc_jailed_partners(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res.dt   <- summarize_yearly_inc_released_partners(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    
    res3.dt <- summarize_yearly_inc_onemomentary(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)	
    res[[res_ind]] <- res.dt$full_inc_mean
    res2[[res_ind]] <- res2.dt$full_inc_mean
    res3[[res_ind]] <- res3.dt$full_inc_mean
    res_ind <- res_ind+1
  }
  
  res.df  <- as.data.frame(res)
  quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  names(res.df) <- seq(1:(rseed_nb))
  res2.df  <- as.data.frame(res2)
  names(res2.df) <- seq(1:(rseed_nb))
  list_inc_means[[j]] <-rowMeans(res.df)
  list_res2_means[[j]] <-rowMeans(res2.df)
  
  list_inc[[j]] <-res.df
  list_inc2[[j]] <-res2.df
  res3.df  <- as.data.frame(res3)
  quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  quantilelist1[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res2.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  quantilelist2[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res3.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  names(res3.df) <- seq(1:(rseed_nb))
  res3.df  <- as.data.frame(res3)
  names(res3.df) <- seq(1:(rseed_nb))
  list_res3_means[[j]] <-rowMeans(res3.df)
  
  list_inc3[[j]] <-res3.df
}

inc_means.df  <- as.data.frame(list_inc_means)
names(inc_means.df) <- seq(1:(sc_nb))

res2_means.df  <- as.data.frame(list_res2_means)
names(res2_means.df) <- seq(1:(sc_nb))

inc_means.df["time"]<- seq(1:sim_years)
res2_means.df["time"]<- seq(1:sim_years)
res3_means.df  <- as.data.frame(list_res3_means)
names(res3_means.df) <- seq(1:(sc_nb))
res3_means.df["time"]<- seq(1:sim_years)

require(ggplot2)
require(reshape2)

inc_means_melt.df <- melt(inc_means.df ,  id.vars = 'time', variable.name = 'series')
inc_means_sd_melt.df <- melt(inc_means_sd.df ,  id.vars = 'time', variable.name = 'series')
inc_means_melt.df<-cbind(inc_means_melt.df,do.call(rbind,quantilelist))


res_means_melt.df <- melt(res2_means.df ,  id.vars = 'time', variable.name = 'series')
res_means_melt.df<-cbind(res_means_melt.df,do.call(rbind,quantilelist))

res2_means_melt.df <- melt(res3_means.df ,  id.vars = 'time', variable.name = 'series')
res2_means_melt.df<-cbind(res2_means_melt.df,do.call(rbind,quantilelist))

NewBaseline<-rbind(inc_means_melt.df,res_means_melt.df,res2_means_melt.df)
save(NewBaseline,file="BaselinePart1.1.Rdata")

list_inc_means = list()
list_res2_means = list_res3_means =list()
list_inc = list()
list_inc2 = list()
list_inc3 = list()

quantilelist<-quantilelist1<-quantilelist2<-vector(mode="list",length=sc_nb)

for (j in 1: (sc_nb)) {
  next_sc <- (j-1)*rseed_nb
  res = list()  
  res2 = list() 
  res3=list
  res_ind=1
  for(i in next_sc:(next_sc+rseed_nb-1)){              
    dir_loc <- paste0(sim_instance_dir,toString(i+1))
    print(dir_loc)
    #				res.dt   <- summarize_yearly_inc_released_partners(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    #res.dt   <- summarize_yearly_prev_ever_jailed1(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res2.dt   <- summarize_yearly_inc_jail_only(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res.dt   <- summarize_yearly_inc_rel_only(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
        res[[res_ind]] <- res.dt$full_inc_mean
    res2[[res_ind]] <- res2.dt$full_inc_mean
    res3[[res_ind]] <- res3.dt$full_inc_mean
    res_ind <- res_ind+1
  }
  
  res.df  <- as.data.frame(res)
  quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  names(res.df) <- seq(1:(rseed_nb))
  res2.df  <- as.data.frame(res2)
  names(res2.df) <- seq(1:(rseed_nb))
  list_inc_means[[j]] <-rowMeans(res.df)
  list_res2_means[[j]] <-rowMeans(res2.df)
  
  list_inc[[j]] <-res.df
  list_inc2[[j]] <-res2.df
  res3.df  <- as.data.frame(res3)
  quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  quantilelist1[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res2.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  quantilelist2[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res3.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  names(res3.df) <- seq(1:(rseed_nb))
  res3.df  <- as.data.frame(res3)
  names(res3.df) <- seq(1:(rseed_nb))
  list_res3_means[[j]] <-rowMeans(res3.df)
  
  list_inc3[[j]] <-res3.df
}

inc_means.df  <- as.data.frame(list_inc_means)
names(inc_means.df) <- seq(1:(sc_nb))

res2_means.df  <- as.data.frame(list_res2_means)
names(res2_means.df) <- seq(1:(sc_nb))

inc_means.df["time"]<- seq(1:sim_years)
res2_means.df["time"]<- seq(1:sim_years)
res3_means.df  <- as.data.frame(list_res3_means)
names(res3_means.df) <- seq(1:(sc_nb))
res3_means.df["time"]<- seq(1:sim_years)

require(ggplot2)
require(reshape2)

inc_means_melt.df <- melt(inc_means.df ,  id.vars = 'time', variable.name = 'series')
inc_means_sd_melt.df <- melt(inc_means_sd.df ,  id.vars = 'time', variable.name = 'series')
inc_means_melt.df<-cbind(inc_means_melt.df,do.call(rbind,quantilelist))


res_means_melt.df <- melt(res2_means.df ,  id.vars = 'time', variable.name = 'series')
res_means_melt.df<-cbind(res_means_melt.df,do.call(rbind,quantilelist))

res2_means_melt.df <- melt(res3_means.df ,  id.vars = 'time', variable.name = 'series')
res2_means_melt.df<-cbind(res2_means_melt.df,do.call(rbind,quantilelist))
load("BaselinePart1.1.Rdata")
NewBaseline<-rbind(NewBaseline,inc_means_melt.df,res_means_melt.df)
names(NewBaseline)[c(4,5)]<-c("Two","NinetySeven")

save(NewBaseline,file="BaselinePart1.Rdata")





for (j in 1: (sc_nb)) {
  next_sc <- (j-1)*rseed_nb
  res = list()  
  res2 = list() 
  res3=list
  res_ind=1
  for(i in next_sc:(next_sc+rseed_nb-1)){              
    dir_loc <- paste0(sim_instance_dir,toString(i+1))
    print(dir_loc)
    #				res.dt   <- summarize_yearly_inc_released_partners(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    #res.dt   <- summarize_yearly_prev_ever_jailed1(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res2.dt   <- summarize_yearly_inc_never_jailed(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res.dt   <- summarize_yearly_inc_ever_jailed(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    
    res3.dt <- summarize_yearly_inc(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)	
    res[[res_ind]] <- res.dt$full_inc_mean
    res2[[res_ind]] <- res2.dt$full_inc_mean
    res3[[res_ind]] <- res3.dt$full_inc_mean
    res_ind <- res_ind+1
  }
  
  res.df  <- as.data.frame(res)
  quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  names(res.df) <- seq(1:(rseed_nb))
  res2.df  <- as.data.frame(res2)
  names(res2.df) <- seq(1:(rseed_nb))
  list_inc_means[[j]] <-rowMeans(res.df)
  list_res2_means[[j]] <-rowMeans(res2.df)

  list_inc[[j]] <-res.df
  list_inc2[[j]] <-res2.df
  res3.df  <- as.data.frame(res3)
  quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  quantilelist1[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res2.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  quantilelist2[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res3.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  names(res3.df) <- seq(1:(rseed_nb))
  res3.df  <- as.data.frame(res3)
  names(res3.df) <- seq(1:(rseed_nb))
  list_res3_means[[j]] <-rowMeans(res3.df)

  list_inc3[[j]] <-res3.df
}

inc_means.df  <- as.data.frame(list_inc_means)
names(inc_means.df) <- seq(1:(sc_nb))

res2_means.df  <- as.data.frame(list_res2_means)
names(res2_means.df) <- seq(1:(sc_nb))

inc_means.df["time"]<- seq(1:sim_years)
res2_means.df["time"]<- seq(1:sim_years)
res3_means.df  <- as.data.frame(list_res3_means)
names(res3_means.df) <- seq(1:(sc_nb))
res3_means.df["time"]<- seq(1:sim_years)

require(ggplot2)
require(reshape2)

inc_means_melt.df <- melt(inc_means.df ,  id.vars = 'time', variable.name = 'series')
inc_means_sd_melt.df <- melt(inc_means_sd.df ,  id.vars = 'time', variable.name = 'series')
inc_means_melt.df<-cbind(inc_means_melt.df,do.call(rbind,quantilelist))


res_means_melt.df <- melt(res2_means.df ,  id.vars = 'time', variable.name = 'series')
res_means_melt.df<-cbind(res_means_melt.df,do.call(rbind,quantilelist))

res2_means_melt.df <- melt(res3_means.df ,  id.vars = 'time', variable.name = 'series')
res2_means_melt.df<-cbind(res2_means_melt.df,do.call(rbind,quantilelist))

a<-rbind(inc_means_melt.df,res_means_melt.df,res2_means_melt.df)
a<-a[c(30:40,70:80,110:120),]
names(a)[c(4,5)]<-c("Two","NinetySeven")
save(a,file="BaselinePlotPart2.Rdata")

t14<-NewBaseline[c(30:40,70:80,110:120,150:160,190:200,230:240),]
names(t14)[c(4,5)]<-c("Two","NinetySeven")
plot1<-ggplot(t14, aes(x = time,  color = series1, fill = series1)) + 
  xlab("Year") +
  ylab("HIV Incidence Rate (per 100 person-years)") +

  ylim(0, 15) +
  geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
  geom_line(aes(y= value))+
  guides(color=guide_legend("Population"))  +
  guides(fill=FALSE)

plot2<-ggplot(a, aes(x = time,  color = series, fill = series)) + 
  xlab("Year") +
  ylab("HIV Incidence Rate (per 100 person-years)") +
  scale_color_manual(values = c("red", "blue","orange"))+
  ylim(0, 15) +
  geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
  geom_line(aes(y= value))+
  guides(color=guide_legend("Population"))  +
  guides(fill=FALSE)
library(gridExtra)
grid.arrange(plot1,plot2,ncol=2)
