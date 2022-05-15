rm(list=ls())

source("summarize_functions_ext6-copy.R")
range <- c(18, 25, 26, 34)
dir_name ="../MedicaidExpansionIncreasedPrepART9"
sim_instance_dir <- paste0(dir_name, "/instance_")
sc_nb= 1
rseed_nb=30
sim_years=10
year_interv_start =0
list_inc_means = list()
list_prev_means = list()
list_inc_means_sd = list()
list_prev_means_sd = list()
list_inc = list()
list_prev = list()
quantilelist=list()


for (j in 1: (sc_nb)) {
        next_sc <- (j-1)*rseed_nb
        res = list()  
        res_prev = list() 
        res_ind=1
        for(i in next_sc:(next_sc+rseed_nb-1)){              
                dir_loc <- paste0(sim_instance_dir,toString(i+1))
                res.dt <- summarize_yearly_prev_prep(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
                res_prev.dt <- summarize_yearly_prev_art(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)   
}
                res[[res_ind]] <- res.dt$mean#res.dt$full_inc_mean
                res_prev[[res_ind]] <- res_prev.dt$mean
                res_ind <- res_ind+1
        }
        res.df  <- as.data.frame(res)
        quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
        means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
        names(res.df) <- seq(1:(rseed_nb))
        res_prev.df  <- as.data.frame(res_prev)
        names(res_prev.df) <- seq(1:(rseed_nb))            
        list_inc_means[[j]] <-rowMeans(res.df)
        list_prev_means[[j]] <-rowMeans(res_prev.df)
        list_inc_means_sd[[j]] <- apply(res.df, 1, sd, na.rm = TRUE) 
        list_prev_means_sd[[j]] <- apply(res_prev.df, 1, sd, na.rm = TRUE) 
        list_inc[[j]] <-res.df
        list_prev[[j]] <-res_prev.df
}

inc_means.df  <- as.data.frame(list_inc_means)
names(inc_means.df) <- seq(1:(sc_nb))
inc_means_sd.df  <- as.data.frame(list_inc_means_sd)
names(inc_means_sd.df) <- seq(1:(sc_nb))
prev_means.df  <- as.data.frame(list_prev_means)
names(prev_means.df) <- seq(1:(sc_nb))
prev_means_sd.df  <- as.data.frame(list_prev_means_sd)
names(prev_means_sd.df) <- seq(1:(sc_nb))
inc_means.df["time"]<- seq(1:sim_years)
inc_means_sd.df["time"]<- seq(1:sim_years)
prev_means.df["time"]<- seq(1:sim_years)
prev_means_sd.df["time"]<- seq(1:sim_years)

require(ggplot2)
require(reshape2)

inc_means_melt.df <- melt(inc_means.df ,  id.vars = 'time', variable.name = 'series')
inc_means_sd_melt.df <- melt(inc_means_sd.df ,  id.vars = 'time', variable.name = 'series')
inc_means_melt.df<-cbind(inc_means_melt.df,do.call(rbind,quantilelist))

prev_means_melt.df <- melt(prev_means.df ,  id.vars = 'time', variable.name = 'series')
prev_means_sd_melt.df <- melt(prev_means_sd.df ,  id.vars = 'time', variable.name = 'series')

t13<-inc_means_melt.df
names(t13)[c(4,5)]<-c("Two","NinetySeven")
a1<-t13




source("summarize_functions_ext6-copy.R")
range <- c(18, 25, 26, 34)
dir_name ="../MedicaidExpansionIncreasedPrepART9"
sim_instance_dir <- paste0(dir_name, "/instance_")
sc_nb= 4
rseed_nb=30
sim_years=10
year_interv_start =0
list_inc_means = list()
list_prev_means = list()
list_inc_means_sd = list()
list_prev_means_sd = list()
list_inc = list()
list_prev = list()
quantilelist=list()


for (j in 1: (sc_nb)) {
        next_sc <- (j-1)*rseed_nb
        res = list()  
        res_prev = list() 
        res_ind=1
        for(i in next_sc:(next_sc+rseed_nb-1)){              
                dir_loc <- paste0(sim_instance_dir,toString(i+1))
                res.dt <- summarize_yearly_prev_art(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
                res_prev.dt <- summarize_yearly_prev_art(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)   
                res[[res_ind]] <- res.dt$mean#res.dt$full_inc_mean
                res_prev[[res_ind]] <- res_prev.dt$mean
                res_ind <- res_ind+1
     
        }
        res.df  <- as.data.frame(res)
        quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
        means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
        names(res.df) <- seq(1:(rseed_nb))
        res_prev.df  <- as.data.frame(res_prev)
        names(res_prev.df) <- seq(1:(rseed_nb))            
        list_inc_means[[j]] <-rowMeans(res.df)
        list_prev_means[[j]] <-rowMeans(res_prev.df)
        list_inc_means_sd[[j]] <- apply(res.df, 1, sd, na.rm = TRUE) 
        list_prev_means_sd[[j]] <- apply(res_prev.df, 1, sd, na.rm = TRUE) 
        list_inc[[j]] <-res.df
        list_prev[[j]] <-res_prev.df
}

inc_means.df  <- as.data.frame(list_inc_means)
names(inc_means.df) <- seq(1:(sc_nb))
inc_means_sd.df  <- as.data.frame(list_inc_means_sd)
names(inc_means_sd.df) <- seq(1:(sc_nb))
prev_means.df  <- as.data.frame(list_prev_means)
names(prev_means.df) <- seq(1:(sc_nb))
prev_means_sd.df  <- as.data.frame(list_prev_means_sd)
names(prev_means_sd.df) <- seq(1:(sc_nb))
inc_means.df["time"]<- seq(1:sim_years)
inc_means_sd.df["time"]<- seq(1:sim_years)
prev_means.df["time"]<- seq(1:sim_years)
prev_means_sd.df["time"]<- seq(1:sim_years)

require(ggplot2)
require(reshape2)

inc_means_melt.df <- melt(inc_means.df ,  id.vars = 'time', variable.name = 'series')
inc_means_sd_melt.df <- melt(inc_means_sd.df ,  id.vars = 'time', variable.name = 'series')
inc_means_melt.df<-cbind(inc_means_melt.df,do.call(rbind,quantilelist))

prev_means_melt.df <- melt(prev_means.df ,  id.vars = 'time', variable.name = 'series')
prev_means_sd_melt.df <- melt(prev_means_sd.df ,  id.vars = 'time', variable.name = 'series')


t13<-inc_means_melt.df
names(t13)[c(4,5)]<-c("Two","NinetySeven")
t13<-rbind(t13,a1)
t13$series<-c(rep("1.Baseline",10),rep("2.Medicaid Exp.",10),rep("3.Medicaid Exp. + 20% PrEP",10),rep("4.Medicaid Exp. + 30% PrEP",10))

ggplot(Inc, aes(x = time,  color = series, fill = series)) + 
        xlab("Year") +
        ylab("Average Incidence Rate") +
        ylim(0, 6.5) +
        geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
        geom_line(aes(y= value))+
        guides(color=guide_legend("Interventions"))  +
        guides(fill=FALSE)


ggplot(t13, aes(x = time,  color = series, fill = series)) + 
        xlab("Year") +
        ylab("Average ART Uptake") +
        ylim(0, 100) +
        geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
        geom_line(aes(y= value))+
        guides(color=guide_legend("Interventions"))  +
        guides(fill=FALSE)
PrepPrev$series<-c(rep("1. Baseline",10),rep("2. ME 1",10),rep("3. ME 2",10),rep("4. ME 3",10))



Case$series<-c(rep("1.Baseline",10),rep("2.Medicaid Exp.",10),rep("3.Medicaid Exp. + \n 20% PrEP + \n 15% ART",10),rep("4.Medicaid Exp. + \n 30% PrEP + \n 15% ART",10))

ggplot(Case, aes(x = time,  color = series, fill = series)) + 
        xlab("Year") +
        ylab("Average New HIV Infections") +
        ylim(0, 400) +
        geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
        geom_line(aes(y= value))+
        guides(color=guide_legend("Interventions"))  +
        guides(fill=FALSE) 

ARTPrev$series<-c(rep("1.Baseline",10),rep("2.Medicaid Exp.",10),rep("3.Medicaid Exp. + \n 20% PrEP + \n 15% ART",10),rep("4.Medicaid Exp. + \n 30% PrEP + \n 15% ART",10))

ggplot(Prev, aes(x = time,  color = series, fill = series)) + 
        xlab("Year") +
        ylab("Average HIV Prevalence") +
        ylim(0, 100) +
        geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
        geom_line(aes(y= value))+
        guides(color=guide_legend("Interventions"))  +
        guides(fill=FALSE) 

ggplot(PrepPrev, aes(x = time,  color = series, fill = series)) + 
        xlab("Year") +
        ylab("Average PrEP Uptake") +
        ylim(0, 100) +
        geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
        geom_line(aes(y= value))+
        guides(color=guide_legend("Interventions"))  +
        guides(fill=FALSE) 

names(PrepPrev)[c(4,5)]<-c("Two","NinetySeven")

paste0()