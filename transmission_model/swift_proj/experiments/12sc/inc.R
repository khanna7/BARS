rm(list=ls())

source("summarize_functions_ext6.R")
range <- c(18, 25, 26, 34)
dir_name ="../DrugBaseline101"  
sim_instance_dir <- paste0(dir_name, "/instance_")
sc_nb= 1
rseed_nb=30
sim_years=100
year_interv_start =0
list_inc_means = list()
list_res2_means = list()
list_inc_means_sd = list()
list_res2_means_sd = list()
list_inc = list()
list_inc2 = list()
quantilelist<-vector(mode="list",length=sc_nb)
quantilelist2<-vector(mode="list",length=sc_nb)

#c(((j-1)*30)+1):c(j*30)

summarize_yearly_inc_crack<- function(filename="counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*(infected_via_transmission_crack[2:.N])/((pop_crack-pop_infected_crack)[1:(.N-1)]))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
}


summarize_yearly_prev_crack  <- function(filename = "counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  prev <- dt_counts[,lapply(c(mean,sd), function(x) x(100*(pop_infected_crack)/(pop_crack))),by = ycat]
  setnames(prev,c("V1","V2"),c("full_inc_mean","sd"))
}











summarize_yearly_inc_released_partners <- function(filename="counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*(infected_released_partner[2:.N])/(uninfected_released_partner[1:(.N-1)]))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
}

summarize_yearly_inc_ever<- function(filename="counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*(infected_ever_jailed[2:.N])/(uninfected_ever_jailed[1:(.N-1)]))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
}

summarize_yearly_inc_never<- function(filename="counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*(infected_never_jailed[2:.N])/(uninfected_never_jailed[1:(.N-1)]))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
}

summarize_yearly_inc_released_partners <- function(filename="counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*(infected_released_partner[2:.N])/(uninfected_released_partner[1:(.N-1)]))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
}

summarize_yearly_inc_jailed_and_released_partners <- function(filename="counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  full_inc <- dt_counts[,lapply(c(mean), function(x) x(365*100*(infected_jailed_and_released_partner[2:.N])/(uninfected_jailed_and_released_partner[1:(.N-1)]))),by = ycat]
  setnames(full_inc,c("V1"),c("full_inc_mean"))
}

summarize_yearly_prev_ever_jailed  <- function(filename = "counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  prev <- dt_counts[,lapply(c(mean,sd), function(x) x(100*(vertex_count_ever_jailed - uninfected_ever_jailed)/(vertex_count_ever_jailed))),by = ycat]
  setnames(prev,c("V1","V2"),c("mean","sd"))
}


summarize_yearly_prev_never_jailed  <- function(filename = "counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)  
  prev <- dt_counts[,lapply(c(mean,sd), function(x) x(100*(vertex_count_never_jailed - uninfected_never_jailed)/(vertex_count_never_jailed))),by = ycat]
  setnames(prev,c("V1","V2"),c("mean","sd"))
}


summarize_yearly_prev_ever_jailed  <- function(filename = "counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)
  prev <- dt_counts[,lapply(c(mean,sd), function(x) x(100*(vertex_count_ever_jailed - uninfected_ever_jailed)/(vertex_count_ever_jailed))),by = ycat]
  setnames(prev,c("V1","V2"),c("mean","sd"))
}

infected_jailed_partner

summarize_yearly_prev_never_jailed  <- function(filename = "counts.csv", ranges, interv_start_at=0){
  dt_counts <- create_dt_counts(filename,ranges, interv_start_at)  
  prev <- dt_counts[,lapply(c(mean,sd), function(x) x(100*(vertex_count_never_jailed - uninfected_never_jailed)/(vertex_count_never_jailed))),by = ycat]
  setnames(prev,c("V1","V2"),c("mean","sd"))
}



for (j in 1:sc_nb){
        next_sc <- (j-1)*rseed_nb
        res = list()  
        res2 = list() 
        res_ind=1
        for(i in next_sc:(next_sc+rseed_nb-1)){              
                dir_loc <- paste0(sim_instance_dir,toString(i+1))
	#			res.dt   <- summarize_yearly_inc_released_partners(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
  			res.dt   <- summarize_yearly_inc(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
                
                
  #              				res.dt   <- summarize_yearly_prev_ever_jailed(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
			res2.dt   <- summarize_yearly_prev_crack(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
				
				
				#res2.dt <- summarize_yearly_inc_jailed_and_released_partners(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)	
                res[[res_ind]] <- res.dt$full_inc_mean
                res2[[res_ind]] <- res2.dt$full_inc_mean
                res_ind <- res_ind+1
        }
        
        res.df  <- as.data.frame(res)
        quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:20000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
        means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=20000),1,quantile,c(.025,.975),na.rm=TRUE))
        names(res.df) <- seq(1:(rseed_nb))
        res2.df  <- as.data.frame(res2)
        #quantilelist2[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
        #means<-rowMeans(res2.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
        names(res2.df) <- seq(1:(rseed_nb))
        list_inc_means[[j]] <-rowMeans(res.df)
        list_res2_means[[j]] <-rowMeans(res2.df)
        list_inc_means_sd[[j]] <- apply(res.df, 1, sd, na.rm = TRUE) 
        list_res2_means_sd[[j]] <- apply(res2.df, 1, sd, na.rm = TRUE) 
        list_inc[[j]] <-res.df
        list_inc2[[j]] <-res2.df
}

inc_means.df  <- as.data.frame(list_inc_means)
names(inc_means.df) <- seq(1:(sc_nb))
inc_means_sd.df  <- as.data.frame(list_inc_means_sd)
names(inc_means_sd.df) <- seq(1:(sc_nb))
res2_means.df  <- as.data.frame(list_res2_means)
names(res2_means.df) <- seq(1:(sc_nb))
res2_means_sd.df  <- as.data.frame(list_res2_means_sd)
names(res2_means_sd.df) <- seq(1:(sc_nb))
inc_means.df["time"]<- seq(1:sim_years)
inc_means_sd.df["time"]<- seq(1:sim_years)
res2_means.df["time"]<- seq(1:sim_years)
res2_means_sd.df["time"]<- seq(1:sim_years)

require(ggplot2)
require(reshape2)

inc_means_melt.df <- melt(inc_means.df ,  id.vars = 'time', variable.name = 'series')
inc_means_melt.df<-cbind(inc_means_melt.df,do.call(rbind,quantilelist))
inc_means_sd_melt.df <- melt(inc_means_sd.df ,  id.vars = 'time', variable.name = 'series')

t1 <- inc_means_melt.df[-c(1:30),]
t2   <- t1[-c(11:40),]
t3   <- t2[-c(21:50),]
t4   <- t3[-c(31:60),]
t5   <- t4[-c(41:70),]
t6   <- t5[-c(51:80),]
t7   <- t6[-c(61:90),]
t8   <- t7[-c(71:100),]
t9   <- t8[-c(81:110),]
t10   <- t9[-c(91:120),]
t11   <- t10[-c(101:130),]
t12   <- t11[-c(111:140),]
t13   <- t12[-c(121:150),]

t1_sd <- inc_means_sd_melt.df[-c(1:30),]
t2_sd   <- t1_sd[-c(11:40),]
t3_sd   <- t2_sd[-c(21:50),]
t4_sd   <- t3_sd[-c(31:60),]
t5_sd   <- t4_sd[-c(41:70),]
t6_sd   <- t5_sd[-c(51:80),]
t7_sd   <- t6_sd[-c(61:90),]
t8_sd   <- t7_sd[-c(71:100),]
t9_sd   <- t8_sd[-c(81:110),]
t10_sd   <- t9_sd[-c(91:120),]
t11_sd   <- t10_sd[-c(101:130),]
t12_sd   <- t11_sd[-c(111:140),]
t13_sd   <- t12_sd[-c(121:150),]
i<-180
t13$series<-c(rep("22.5, 90",10),rep("45, 90",10),rep("67.5, 90",10),rep("90, 90",10),rep("22.5, 180",10),rep("45, 180",10),rep("67.5, 180",10),rep("90, 180",10),rep("22.5, 360",10),rep("45, 360",10),rep("67.5, 360",10),rep("90, 360",10))
names(t13)[c(4,5)]<-c("Two","NinetySeven")
#ribbon1<-apply(matrix(unlist(quantiles2.597.5),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE)

ggplot(subset(t13, series %in% c(paste("22.5,",i), paste("45,",i), paste("67.5,",i), paste("90,",i) )), aes(x = time,  color = series, fill = series)) + 
        xlab("Year") +
        ylab("Average Incidence Rate") +
        labs(title=paste("Post Release partners, T3=",i)) +
        ylim(5, 15) +
        geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
        geom_line(aes(y= value))+
        guides(color=guide_legend("T1, T3"))  +
        guides(fill=FALSE)
# 
# ggplot(t13, aes(x = time,  color = series, fill = series)) + 
#   xlab("Year") +
#   ylab("Average Incidence Rate") +
#   labs(title="Post Incarceration partners") +
#   ylim(0, 20) +
#   geom_ribbon(aes(ymin = value-t13_sd$value, ymax = value+t13_sd$value), colour = NA, alpha = 0.1) +
#   geom_line(aes(y= value))+
#   guides(color=guide_legend("Interventions"))  +
#   guides(fill=FALSE)
# 


inc_means_melt.df <- melt(res2_means.df ,  id.vars = 'time', variable.name = 'series')
inc_means_melt.df<-cbind(inc_means_melt.df,do.call(rbind,quantilelist2))
inc_means_sd_melt.df <- melt(res2_means_sd.df ,  id.vars = 'time', variable.name = 'series')

t1 <- inc_means_melt.df[-c(1:30),]
t2   <- t1[-c(11:40),]
t3   <- t2[-c(21:50),]
t4   <- t3[-c(31:60),]
t5   <- t4[-c(41:70),]
t6   <- t5[-c(51:80),]
t7   <- t6[-c(61:90),]
t8   <- t7[-c(71:100),]
t9   <- t8[-c(81:110),]
t10   <- t9[-c(91:120),]
t11   <- t10[-c(101:130),]
t12   <- t11[-c(111:140),]
t13   <- t12[-c(121:150),]


t1_sd <- inc_means_sd_melt.df[-c(1:30),]
t2_sd   <- t1_sd[-c(11:40),]
t3_sd   <- t2_sd[-c(21:50),]
t4_sd   <- t3_sd[-c(31:60),]
t5_sd   <- t4_sd[-c(41:70),]
t6_sd   <- t5_sd[-c(51:80),]
t7_sd   <- t6_sd[-c(61:90),]
t8_sd   <- t7_sd[-c(71:100),]
t9_sd   <- t8_sd[-c(81:110),]
t10_sd   <- t9_sd[-c(91:120),]
t11_sd   <- t10_sd[-c(101:130),]
t12_sd   <- t11_sd[-c(111:140),]
t13_sd   <- t12_sd[-c(121:150),]
i<-180
#t13$series<-c(rep("22.5",10),rep("45",10),rep("67.5",10),rep("90",10))
t13$series<-c(rep("22.5, 90",10),rep("45, 90",10),rep("67.5, 90",10),rep("90, 90",10),rep("22.5, 180",10),rep("45, 180",10),rep("67.5, 180",10),rep("90, 180",10),rep("22.5, 360",10),rep("45, 360",10),rep("67.5, 360",10),rep("90, 360",10))

names(t13)[c(4,5)]<-c("Two","NinetySeven")

ggplot(subset(t13, series %in% c(paste("22.5,",i), paste("45,",i), paste("67.5,",i), paste("90,",i) )), aes(x = time,  color = series, fill = series)) + 
  xlab("Year") +
  ylab("Average Incidence Rate") +
  labs(title="Pre incarceration partners") +
  ylim(5, 15) +
  geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1)  +
  geom_line(aes(y= value))+
  guides(color=guide_legend("T1, T3"))  +
  guides(fill=FALSE)


# 
# ggplot(t13, aes(x = time,  color = series, fill = series)) + 
#   xlab("Year") +
#   ylab("Average Incidence Rate") +
#   labs(title="Pre incarceration partners") +
#   ylim(0, 20) +
#   geom_ribbon(aes(ymin = value-t13_sd$value, ymax = value+t13_sd$value), colour = NA, alpha = 0.1) +
#   geom_line(aes(y= value))+
#   guides(color=guide_legend("Interventions"))  +
#   guides(fill=FALSE)
# 
# 
# #setwd("/project2/khanna7/bma/BARS/transmission_model/swift_proj/experiments/ME_FL_83120_1/instance_1/output/")
# #counts_data<-read.csv("counts.csv")
# 
# 




ggplot(t13, aes(x = ycat,  color = V2, fill = V2)) + 
  xlab("Year") +
  ylab("Average Prevalence Rate") +
  labs(title="Ever/Never Jailed") +
  ylim(25, 45) +
  geom_ribbon(aes(ymin = mean-1.96*sd, ymax = mean+1.96*sd), colour = NA, alpha = 0.1) +
  geom_line(aes(y= mean))+
  guides(color=guide_legend("Jailed"))  +
  guides(fill=FALSE)
# 
