#This file generates the simulated partnership length distribution for main and casual networks in the jail circulation model. Keep in mind you need to turn on the partnership_events.csv
#Pre-allocate
casual_result.grand<-main_result.grand<-matrix(NA,nrow=3650,ncol=30)
setwd("/project2/ahotton/francis/jcm-paper1/BARS/transmission_model/swift_proj/experiments/PartnershipRuns/instance_1/output/")

for(z in 7:30){
  load("/home/francislee/main_partnership_result.RData")
  load("/home/francislee/casual_partnership_result.RData")
  print(paste0("Currently on Run",z))
  library(tidyr)
  library(network)
  library(sna)
  setwd(paste0("../../instance_",z,"/output/")) #Replace PartnershipPLACEHOLDER with the actual experiment directory
  dt<-read.csv("partnership_events.csv")
  dt$tick<-dt$tick-10950
  dt<-dt[dt$tick>0,]
  x<-1:max(dt$tick) #overall time vector. this gets divided into 10 pieces.
  
  # Extract edgelists at the end of the burnin
  
  main_net<-readRDS("main_network_10950.RDS")
  main_net.el<-as.edgelist(main_net)
  main_matrix<-main_net.el[,c(1,2)]
  main_matrix<-mapvalues(main_matrix,main_net%v%"vertex_names",main_net%v%"c.id",warn_missing=FALSE)
  
  
  #main_matrix needs to be converted to a maniuplatable format.
  
  initial_main_registry<-list()
  for (i in 1:length(main_matrix[,1])){
    initial_main_registry[[i]]<-main_matrix[i,]
  }
  
  casual_net<-readRDS("casual_network_10950.RDS")
  casual_net.el<-as.edgelist(casual_net)
  casual_matrix<-casual_net.el[,c(1,2)]
  #casual_matrix<-readRDS("casual_network_10950.RDS")
  casual_matrix<-mapvalues(casual_matrix,casual_net%v%"vertex_names",casual_net%v%"c.id",warn_missing=FALSE)
  
  
  #casual_matrix needs to be converted to a maniuplatable format.
  
  initial_casual_registry<-list()
  for (i in 1:length(casual_matrix[,1])){
    initial_casual_registry[[i]]<-casual_matrix[i,]
  }
  
  relationship_registry<-function(interval, mr, type_index){ 
    #RETURN A SET OF ORDERED PAIRS P1 P2 OF RELATIONSHIPS 
    #(denoted mr for main registry) FOR ANY GIVEN TIME T in the interval. 
    #MAKE SURE YOUR REGISTRY CORRESPONDS TO THE MINIMUM VALUE IN YOUR INTERVAL.
    #Ex: If Interval=(200:400), make sure mr was updated for t=200.
    #if the relationship ended, it is set to NULL.
    #If it  was remade, it will have another entry.
    total<-length(mr)
    result<-list()
    decayed<-0
    survival_vector<-c()
    for (t in interval){
      #Below is a percent complete marker, helpful for assembling the larger registries
      cat((t-min(interval))/length(interval)*100, '% complete', min(interval), '\n')
      #I filter for time t and for the main network type. Note only p1,p2,type are needed.
      
      dt_filter_t_p1<-dt$p1[(dt$tick==t)&(dt$network_type==type_index)]#+1  #add 1 to account for C++ difference
      dt_filter_t_p2<-dt$p2[(dt$tick==t)&(dt$network_type==type_index)]#+1  #add 1 to account for C++ difference
      dt_filter_t_type<-dt$type[(dt$tick==t)&(dt$network_type==type_index)]
      
      if((length(dt_filter_t_p1)!=0)&(length(dt_filter_t_type)!=0)){
        for (i in 1:length(dt_filter_t_p1)){
          p1<-dt_filter_t_p1[i]
          p2<-dt_filter_t_p2[i]
          
          if(dt_filter_t_type[i]!=1){#if the relationship is breaking
            original_p1_in_registry<-which(sapply(mr,`%in%`,x=p1))
            #vector of indices of any matching p1s that started in the registry
            original_p2_in_registry<-which(sapply(mr,`%in%`,x=p2))
            #vector of indices of any matching p2s that started in the registry
            original_rows<-intersect(original_p1_in_registry,original_p2_in_registry)
            #vector of indices of any matching rows that started in the registry
            
            new_p1_in_registry<-which(sapply(mr,`%in%`,x=-p1))
            #same thing as above but if it's new, meaning it was added while the function ran.
            new_p2_in_registry<-which(sapply(mr,`%in%`,x=-p2))
            new_rows<-intersect(new_p1_in_registry,new_p2_in_registry) #Dead new relationships that died in the time that the function was running
            
            if(length(original_rows)!=0){ #the relationship is part of the original set
              mr[[original_rows]]<-NULL #delete it
              decayed<-decayed+length(original_rows)
            } else if(length(new_rows)!=0){ #the relationship is part of the new set
              mr[[new_rows]]<-NULL #don't count decayed for new relationships! Only if it is original
            }
            
          } else{mr[[length(mr)+1]]<-c(p1,p2)*(-1)} #add the relationship to the end.
        }
      }
      percent_remaining<-(total-decayed)/total
      #percent of original relationships remaining at a given time
      survival_vector<-append(survival_vector,
                              max(0,percent_remaining))
      cat(tail(survival_vector,n=1)*100,
          "% relationships remain in",z, 
          '\n')
    }
    for(i in 1:length(mr)){mr[[i]]<-abs(mr[[i]])}#restore the negatives back to positives.
    result[[1]]<-mr #registry. This just makes it so you can pick up where you left off.
    result[[2]]<-survival_vector #survival curve vector. the "important" part.
    return(result)
  }
  
  time0<-x
  time1<-1:3650   #(max(x)*(1/10))
  
  
  main_registry1<-initial_main_registry
  write.csv(main_registry1, file="main_registry1.csv", row.names=FALSE)
  main_result1<-relationship_registry(time1,main_registry1,0)
  write.csv(main_result1[[2]], file="main_result1.csv", row.names=FALSE)
  main_result.grand[,z]<-main_result1[[2]]
  save(main_result.grand,file="/home/francislee/main_partnership_result.RData") #Or whereever your directory is
  
  casual_registry1<-initial_casual_registry
  write.csv(casual_registry1, file="casual_registry1.csv", row.names=FALSE)
  casual_result1<-relationship_registry(time1,casual_registry1,1)
  casual_result.grand[,z]<-casual_result1[[2]]
  save(casual_result.grand,file="/home/francislee/casual_partnership_result.RData") #Ditto here
  
  
  write.csv(casual_result1[[2]], file="casual_result1.csv", row.names=FALSE)
  rm(list = ls())
}
#This is the empirical, for the bootsrap CI and not the actual CI computed here, go to BARSEmpiricalSurvivorshipCurve.R
#Confidence Bounds
x1<-c(20,
  36,
  80,
  98,
  100,
  102,
  135,
  147,
  160,
  172,
  267,
  277,
  343,
  369,
  372,
  562
)/365
y1<-c(1,
      1,
      0.96,
      0.92,
      0.84,
      0.76,
      0.72,
      0.6,
      0.48,
      0.4,
      0.355555556,
      0.266666667,
      0.222222222,
      0.133333333,
      0.088888889,
      0
)

mrg1<-apply(main_result.grand,1,quantile,.025)
mrg2<-apply(main_result.grand,1,quantile,.975)
crg1<-apply(casual_result.grand,1,quantile,.025)
crg2<-apply(casual_result.grand,1,quantile,.975)
lb<-c(1,
      1,
  0.748385024,
  0.716390337,
  0.628056082,
  0.542048122,
  0.500887201,
  0.384491393,
  0.278132675,
  0.212816006,
  0.177101377,
  0.112129815,
  0.083175932,
  0.034110083,
  0.01554597,
  0
)
ub<-c(
  1,
  1,
  0.994266951,
  0.979370585,
  0.93673324,
  0.884277638,
  0.855484297,
  0.761092652,
  0.656402854,
  0.581231729,
  0.539167716,
  0.45003591,
  0.402643486,
  0.300656217,
  0.244906285,
  0
)
load("/home/francislee/main_partnership_result.RData")
load("/home/francislee/casual_partnership_result.RData")

meanmrg<-apply(main_result.grand,1,mean)
meancrg<-apply(casual_result.grand,1,mean)

plot(x1,y1,type="l",xlim=c(0,5),lwd=1.5,ylim=c(0,1),xlab="Year",ylab="Proportion of Relationships Survived")
line
lines(1:3650/365,meanmrg,lwd=1.5,col=rgb(1,0,0,1))
lines(1:3650/365,meancrg,lwd=1.5,col=rgb(0,0,1,1))

polygon( c((1:3650)/365,rev(1:3650/365)),c(mrg1,rev(mrg2)),col=rgb(1,0,0,.3),border=NA  )
polygon( c((1:3650)/365,rev(1:3650/365)),c(crg1,rev(crg2)),col=rgb(0,0,1,.3),border=NA  )
legend("topright",c("Simulated Main Relationships","Simulated Casual Relationships","Empirical Relationships"),lwd=3,col=c(rgb(1,0,0,.3),rgb(0,0,1,.3),"black"  ))
polygon(c(x1,rev(x1)),c(lb,rev(ub)),col=rgb(0,0,0,.3),border=NA  )
