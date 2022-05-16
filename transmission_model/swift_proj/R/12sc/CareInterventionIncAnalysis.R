#rm(list=ls())

source("summarize_functions_ext6.R")
range <- c(18, 25, 26, 34)
dir_name ="../../experiments/CareInterventionFinal" #Replace this with the network intervention experiment directory  
sim_instance_dir <- paste0(dir_name, "/instance_")
sc_nb= 4
rseed_nb=30
sim_years=40
year_interv_start =0
list_inc_means = list()
list_res2_means = list()
list_inc_means_sd = list()
list_res2_means_sd = list()
list_inc = list()
list_inc2 = list()
quantilelist<-vector(mode="list",length=sc_nb)

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


for (j in 1: (sc_nb)) {
        next_sc <- (j-1)*rseed_nb
        res = list()  
        res2 = list() 
        res_ind=1
        for(i in next_sc:(next_sc+rseed_nb-1)){
                dir_loc <- paste0(sim_instance_dir,toString(i+1))
				res2.dt   <- summarize_yearly_inc(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
				#res.dt   <- summarize_yearly_prev_ever_jailed1(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
				res.dt   <- summarize_yearly_inc_released_partners(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)
				#res2.dt <- summarize_yearly_inc(paste0(dir_loc,"/output/counts.csv"), range, interv_start_at=year_interv_start)	
                res[[res_ind]] <- res.dt$full_inc_mean
                res2[[res_ind]] <- res2.dt$full_inc_mean
                res_ind <- res_ind+1
                
        }
        res.df  <- as.data.frame(res)
        res2.df  <- as.data.frame(res2)
        quantilelist[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
        means<-rowMeans(res.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
        quantilelist1[[j]]<-t(apply(matrix(unlist(lapply(1:1000,function(i){res.df.sample.index<-sample(1:rseed_nb ,rseed_nb,replace=TRUE);
        means<-rowMeans(res2.df[,res.df.sample.index]);return(means)})),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE))
        names(res.df) <- seq(1:(rseed_nb))
        names(res2.df) <- seq(1:(rseed_nb))
        list_inc_means[[j]] <-rowMeans(res.df,na.rm=TRUE)
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


save(list_inc,file="RelBoxPlot.Rdata")


require(ggplot2)
require(reshape2)

inc_means_melt.df <- melt(inc_means.df ,  id.vars = 'time', variable.name = 'series')
inc_means_sd_melt.df <- melt(inc_means_sd.df ,  id.vars = 'time', variable.name = 'series')
inc_means_melt.df<-cbind(inc_means_melt.df,do.call(rbind,quantilelist))
names(t13)[c(4,5)]<-c("Two","NinetySeven")

#ribbon1<-apply(matrix(unlist(quantiles2.597.5),ncol=1000),1,quantile,c(.025,.975),na.rm=TRUE)


t13$series<-c(rep("90",40),rep("720",40),rep("No Disruption",40),rep("Targeted Care",40))
t14<-t13[c(30:40,70:80,110:120,150:160),]
ggplot(t13, aes(x = time,  color = series, fill = series)) + 
  xlab("Year") +
  ylab("Average HIV Incidence Rate") +
  labs(title=paste("Care Disruption Scenarios: Post Incarceration Partners")) +
  ylim(2.5, 17) +
  geom_ribbon(aes(ymin = Two, ymax = NinetySeven), colour = NA, alpha = 0.1) +
  geom_line(aes(y= value))+
  guides(color=guide_legend("Scenarios"))  +
  guides(fill=FALSE)



