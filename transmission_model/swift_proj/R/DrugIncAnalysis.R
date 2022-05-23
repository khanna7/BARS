rm(list=ls())
#Set the experiment directory to 
source("summarize_functions_ext6.R")
range <- c(18, 25, 26, 34)
dir_name ="../../experiments/DrugBMPLACEHOLDER" #USE Whatever the biomedical (mirtazapine) experiment directory is called  
sim_instance_dir <- paste0(dir_name, "/instance_")
sc_nb= 10
rseed_nb=30
sim_years=10
year_interv_start =0
list_inc_means = list()
list_res2_means = list()
list_inc_means_sd = list()
list_res2_means_sd = list()
list_inc = list()
list_inc2 = list()
quantilelist<-vector(mode="list",length=sc_nb)
quantilelist2<-vector(mode="list",length=sc_nb)

for (j in 1:sc_nb){
        next_sc <- (j-1)*rseed_nb
        res = list()  
        res2 = list() 
        res_ind=1
        for(i in next_sc:(next_sc+rseed_nb-1)){              
                dir_loc <- paste0(sim_instance_dir,toString(i+1))
  			res.dt   <- summarize_yearly_inc(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
			  res2.dt   <- summarize_yearly_prev(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
			         res[[res_ind]] <- res.dt$full_inc_mean
                res2[[res_ind]] <- res2.dt$mean
                res_ind <- res_ind+1
        }
        
        res.df  <- as.data.frame(res)
        quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
        means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=20000),1,quantile,c(.025,.975),na.rm=TRUE))
        names(res.df) <- seq(1:(rseed_nb))
        res2.df  <- as.data.frame(res2)
        quantilelist2[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
        means<-rowMeans(res2.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
        names(res2.df) <- seq(1:(rseed_nb))
        list_inc_means[[j]] <-rowMeans(res.df)
        list_res2_means[[j]] <-rowMeans(res2.df)
        list_inc[[j]] <-res.df
        list_inc2[[j]] <-res2.df
}

inc_means.df  <- as.data.frame(list_inc_means)
names(inc_means.df) <- seq(1:(sc_nb))

res2_means.df  <- as.data.frame(list_res2_means)
names(res2_means.df) <- seq(1:(sc_nb))

inc_means.df["time"]<- seq(1:sim_years)

res2_means.df["time"]<- seq(1:sim_years)


require(ggplot2)
require(reshape2)

inc_means_melt.df.bm <- melt(inc_means.df ,  id.vars = 'time', variable.name = 'series')
inc_means_melt.df.bm<-cbind(inc_means_melt.df.bm,do.call(rbind,quantilelist))

prev_means_melt.df.bm <- melt(res2_means.df ,  id.vars = 'time', variable.name = 'series')
prev_means_melt.df.bm<-cbind(prev_means_melt.df.bm,do.call(rbind,quantilelist))

t13$series<-rep(c(rep("0.2",10),rep("0.4",10),rep("0.6",10),rep("0.8",10),rep("1",10)),2)
names(t13)[c(4,5)]<-c("Two","NinetySeven")
t13<-t13[1:50,] #The first 50 datapoints are with time=90 on the intervention which is what's in the paper, the next 50 are with time=465 


ggplot(t13, aes(x = time,  color = series, fill = series)) + 
        xlab("Year") +
        ylab("Average HIV Incidence Rate (in 100 p/ys)") +
        labs() +
        ylim(0, 10) +
        geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
        geom_line(aes(y= value))+
        guides(color=guide_legend(""))  +
        guides(fill=FALSE)
save(inc_means_melt.df.bm,file="DrugBMInc.Rdata") #Incidence of Drug BM Data
save(prev_means_melt.df.bm,file="DrugBMPrev.Rdata") #Incidence of Drug BM Data

rm(list=ls())
#Set the experiment directory to 
source("summarize_functions_ext6.R")
range <- c(18, 25, 26, 34)
dir_name ="../../experiments/DrugBC20PLACEHOLDER" #USE Whatever the behavioral counseling experiment directory is called  
sim_instance_dir <- paste0(dir_name, "/instance_")
sc_nb= 8
rseed_nb=30
sim_years=10
year_interv_start =0
list_inc_means = list()
list_res2_means = list()
list_inc_means_sd = list()
list_res2_means_sd = list()
list_inc = list()
list_inc2 = list()
quantilelist<-vector(mode="list",length=sc_nb)
quantilelist2<-vector(mode="list",length=sc_nb)

for (j in 1:sc_nb){
  next_sc <- (j-1)*rseed_nb
  res = list()  
  res2 = list() 
  res_ind=1
  for(i in next_sc:(next_sc+rseed_nb-1)){              
    dir_loc <- paste0(sim_instance_dir,toString(i+1))
    res.dt   <- summarize_yearly_inc(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res2.dt   <- summarize_yearly_prev(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res[[res_ind]] <- res.dt$full_inc_mean
    res2[[res_ind]] <- res2.dt$mean
    res_ind <- res_ind+1
  }
  
  res.df  <- as.data.frame(res)
  quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=20000),1,quantile,c(.025,.975),na.rm=TRUE))
  names(res.df) <- seq(1:(rseed_nb))
  res2.df  <- as.data.frame(res2)
  quantilelist2[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res2.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  names(res2.df) <- seq(1:(rseed_nb))
  list_inc_means[[j]] <-rowMeans(res.df)
  list_res2_means[[j]] <-rowMeans(res2.df)
  list_inc[[j]] <-res.df
  list_inc2[[j]] <-res2.df
}

inc_means.df  <- as.data.frame(list_inc_means)
names(inc_means.df) <- seq(1:(sc_nb))

res2_means.df  <- as.data.frame(list_res2_means)
names(res2_means.df) <- seq(1:(sc_nb))

inc_means.df["time"]<- seq(1:sim_years)
res2_means.df["time"]<- seq(1:sim_years)


require(ggplot2)
require(reshape2)

inc_means_melt.df.bc <- melt(inc_means.df ,  id.vars = 'time', variable.name = 'series')
inc_means_melt.df.bc<-cbind(inc_means_melt.df.bc,do.call(rbind,quantilelist))

prev_means_melt.df.bc <- melt(res2_means.df ,  id.vars = 'time', variable.name = 'series')
prev_means_melt.df.bc <-cbind(prev_means_melt.df.bc,do.call(rbind,quantilelist))

t13$series<-rep(c(rep("0.1",10),rep("0.15",10),rep("0.2",10),rep("0.25",10)),2)
names(t13)[c(4,5)]<-c("Two","NinetySeven")
t13<-t13[41:80,] #The first 40 datapoints are with time=60 on the intervention, the next 40 are with time=90 which is what's in the paper


ggplot(t13, aes(x = time,  color = series, fill = series)) + 
  xlab("Year") +
  ylab("Average HIV Incidence Rate (in 100 p/ys)") +
  labs() +
  ylim(5, 15) +
  geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
  geom_line(aes(y= value))+
  guides(color=guide_legend(""))  +
  guides(fill=FALSE)
save(inc_means_melt.df.bc,file="DrugBCInc.Rdata") #Incidence of Drug BM Data
save(prev_means_melt.df.bc,file="DrugBCPrev.Rdata")



rm(list=ls())
#Set the experiment directory to 
source("summarize_functions_ext6.R")
range <- c(18, 25, 26, 34)
dir_name ="../../experiments/DrugBC20PLACEHOLDER" #USE Whatever the behavioral counseling experiment directory is called  
sim_instance_dir <- paste0(dir_name, "/instance_")
sc_nb= 8
rseed_nb=30
sim_years=10
year_interv_start =0
list_inc_means = list()
list_res2_means = list()
list_inc_means_sd = list()
list_res2_means_sd = list()
list_inc = list()
list_inc2 = list()
quantilelist<-vector(mode="list",length=sc_nb)
quantilelist2<-vector(mode="list",length=sc_nb)

for (j in 1:sc_nb){
  next_sc <- (j-1)*rseed_nb
  res = list()  
  res2 = list() 
  res_ind=1
  for(i in next_sc:(next_sc+rseed_nb-1)){              
    dir_loc <- paste0(sim_instance_dir,toString(i+1))
    res.dt   <- summarize_yearly_inc(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res2.dt   <- summarize_yearly_prev(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res[[res_ind]] <- res.dt$full_inc_mean
    res2[[res_ind]] <- res2.dt$mean
    res_ind <- res_ind+1
  }
  
  res.df  <- as.data.frame(res)
  quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=20000),1,quantile,c(.025,.975),na.rm=TRUE))
  names(res.df) <- seq(1:(rseed_nb))
  res2.df  <- as.data.frame(res2)
  quantilelist2[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res2.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  names(res2.df) <- seq(1:(rseed_nb))
  list_inc_means[[j]] <-rowMeans(res.df)
  list_res2_means[[j]] <-rowMeans(res2.df)
  list_inc[[j]] <-res.df
  list_inc2[[j]] <-res2.df
}

inc_means.df  <- as.data.frame(list_inc_means)
names(inc_means.df) <- seq(1:(sc_nb))

res2_means.df  <- as.data.frame(list_res2_means)
names(res2_means.df) <- seq(1:(sc_nb))

inc_means.df["time"]<- seq(1:sim_years)
res2_means.df["time"]<- seq(1:sim_years)


require(ggplot2)
require(reshape2)

inc_means_melt.df.bc.20 <- melt(inc_means.df ,  id.vars = 'time', variable.name = 'series')
inc_means_melt.df.bc.20<-cbind(inc_means_melt.df.bc.20,do.call(rbind,quantilelist))

prev_means_melt.df.bc.20 <- melt(res2_means.df ,  id.vars = 'time', variable.name = 'series')
prev_means_melt.df.bc.20 <-cbind(prev_means_melt.df.bc.20,do.call(rbind,quantilelist))

t13$series<-rep(c(rep("0.1",10),rep("0.15",10),rep("0.2",10),rep("0.25",10)),2)
names(t13)[c(4,5)]<-c("Two","NinetySeven")
t13<-t13[41:80,] #The first 40 datapoints are with time=60 on the intervention, the next 40 are with time=90 which is what's in the paper


ggplot(t13, aes(x = time,  color = series, fill = series)) + 
  xlab("Year") +
  ylab("Average HIV Incidence Rate (in 100 p/ys)") +
  labs() +
  ylim(5, 15) +
  geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
  geom_line(aes(y= value))+
  guides(color=guide_legend(""))  +
  guides(fill=FALSE)
save(inc_means_melt.df.bc.20,file="DrugBC20Inc.Rdata") #Incidence of Drug BM Data
save(prev_means_melt.df.bc.20,file="DrugBC20Prev.Rdata")

rm(list=ls())
#Set the experiment directory to 
source("summarize_functions_ext6.R")
range <- c(18, 25, 26, 34)
dir_name ="../../experiments/DrugBC30PLACEHOLDER" #USE Whatever the behavioral counseling experiment directory is called  
sim_instance_dir <- paste0(dir_name, "/instance_")
sc_nb= 8
rseed_nb=30
sim_years=10
year_interv_start =0
list_inc_means = list()
list_res2_means = list()
list_inc_means_sd = list()
list_res2_means_sd = list()
list_inc = list()
list_inc2 = list()
quantilelist<-vector(mode="list",length=sc_nb)
quantilelist2<-vector(mode="list",length=sc_nb)

for (j in 1:sc_nb){
  next_sc <- (j-1)*rseed_nb
  res = list()  
  res2 = list() 
  res_ind=1
  for(i in next_sc:(next_sc+rseed_nb-1)){              
    dir_loc <- paste0(sim_instance_dir,toString(i+1))
    res.dt   <- summarize_yearly_inc(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res2.dt   <- summarize_yearly_prev(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
    res[[res_ind]] <- res.dt$full_inc_mean
    res2[[res_ind]] <- res2.dt$mean
    res_ind <- res_ind+1
  }
  
  res.df  <- as.data.frame(res)
  quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=20000),1,quantile,c(.025,.975),na.rm=TRUE))
  names(res.df) <- seq(1:(rseed_nb))
  res2.df  <- as.data.frame(res2)
  quantilelist2[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
  means<-rowMeans(res2.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
  names(res2.df) <- seq(1:(rseed_nb))
  list_inc_means[[j]] <-rowMeans(res.df)
  list_res2_means[[j]] <-rowMeans(res2.df)
  list_inc[[j]] <-res.df
  list_inc2[[j]] <-res2.df
}

inc_means.df  <- as.data.frame(list_inc_means)
names(inc_means.df) <- seq(1:(sc_nb))

res2_means.df  <- as.data.frame(list_res2_means)
names(res2_means.df) <- seq(1:(sc_nb))

inc_means.df["time"]<- seq(1:sim_years)
res2_means.df["time"]<- seq(1:sim_years)


require(ggplot2)
require(reshape2)

inc_means_melt.df.bc.30 <- melt(inc_means.df ,  id.vars = 'time', variable.name = 'series')
inc_means_melt.df.bc.30 <-cbind(inc_means_melt.df.bc,do.call(rbind,quantilelist))

prev_means_melt.df.bc.30 <- melt(res2_means.df ,  id.vars = 'time', variable.name = 'series')
prev_means_melt.df.bc.30 <-cbind(prev_means_melt.df.bc.30,do.call(rbind,quantilelist))

t13$series<-rep(c(rep("0.1",10),rep("0.15",10),rep("0.2",10),rep("0.25",10)),2)
names(t13)[c(4,5)]<-c("Two","NinetySeven")
t13<-t13[41:80,] #The first 40 datapoints are with time=60 on the intervention, the next 40 are with time=90 which is what's in the paper

ggplot(t13, aes(x = time,  color = series, fill = series)) + 
  xlab("Year") +
  ylab("Average HIV Incidence Rate (in 100 p/ys)") +
  labs() +
  ylim(5, 15) +
  geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
  geom_line(aes(y= value))+
  guides(color=guide_legend(""))  +
  guides(fill=FALSE)
save(inc_means_melt.df.bc.30,file="DrugBC30Inc.Rdata") #Incidence of Drug BM Data
save(prev_means_melt.df.bc.30,file="DrugBC30Prev.Rdata")
