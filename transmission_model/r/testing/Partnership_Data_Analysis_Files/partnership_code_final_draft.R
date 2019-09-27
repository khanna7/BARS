#partnership data
rm(list = ls())
library(tidyr)
setwd("~/Desktop")
dt<-read.csv("partnership_events_248.csv")
x<-1:max(dt$tick) #overall time vector. this gets divided into 10 pieces.

# Extract edgelists at the end of the burnin

main_matrix<-readRDS("v54-calibrate-chicago-instance248_main_el.RDS")
#main_net<-readRDS("main_network_36500.RDS")
#main_net.el<-as.edgelist(main_net)
#main_matrix<-main_net.el[,c(1,2)]

#main_matrix needs to be converted to a maniuplatable format.

initial_main_registry<-list()
for (i in 1:length(main_matrix[,1])){
  initial_main_registry[[i]]<-main_matrix[i,]
}

casual_matrix<-readRDS("v54-calibrate-chicago-instance248_casual_el.RDS")
#casual_net<-readRDS("casual_network_36500.RDS")
#casual_net.el<-as.edgelist(casual_net)
#casual_matrix<-casual_net.el[,c(1,2)]

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
    dt_filter_t_p1<-dt$p1[(dt$tick==t)&(dt$network_type==type_index)]+1  #add 1 to account for C++ difference
    dt_filter_t_p2<-dt$p2[(dt$tick==t)&(dt$network_type==type_index)]+1  #add 1 to account for C++ difference
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
          new_rows<-intersect(new_p1_in_registry,new_p2_in_registry)
          
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
        "% relationships remain", 
        '\n')
  }
  for(i in 1:length(mr)){mr[[i]]<-abs(mr[[i]])}#restore the negatives back to positives.
  result[[1]]<-mr #registry. This just makes it so you can pick up where you left off.
  result[[2]]<-survival_vector #survival curve vector. the "important" part.
  return(result)
}

time0<-x
time1<-1:(max(x)*(1/10)-1)
time2<-(max(time1)+1):(max(x)*(2/10)-1)
time3<-(max(time2)+1):(max(x)*(3/10)-1)
time4<-(max(time3)+1):(max(x)*(4/10)-1)
time5<-(max(time4)+1):(max(x)*(5/10)-1)
time6<-(max(time5)+1):(max(x)*(6/10)-1)
time7<-(max(time6)+1):(max(x)*(7/10)-1)
time8<-(max(time7)+1):(max(x)*(8/10)-1)
time9<-(max(time8)+1):(max(x)*(9/10)-1)
time10<-(max(time9)+1):(max(x))


main_registry1<-initial_main_registry
write.csv(main_registry1, file="main_registry1.csv", row.names=FALSE)
main_result1<-relationship_registry(time1,main_registry1,0)
write.csv(main_result1[[2]], file="main_result1.csv", row.names=FALSE)

main_registry2<-main_result1[[1]]
write.csv(main_registry2, file="main_registry2.csv", row.names=FALSE)
main_result2<-relationship_registry(time2,main_registry2,0)
write.csv(main_result2[[2]], file="main_result2.csv", row.names=FALSE)

main_registry3<-main_result2[[1]]
write.csv(main_registry3, file="main_registry3.csv", row.names=FALSE)
main_result3<-relationship_registry(time3,main_registry3,0)
write.csv(main_result3[[2]], file="main_result3.csv", row.names=FALSE)

main_registry4<-main_result3[[1]]
write.csv(main_registry4, file="main_registry4.csv", row.names=FALSE)
main_result4<-relationship_registry(time4,main_registry4,0)
write.csv(main_result4[[2]], file="main_result4.csv", row.names=FALSE)

main_registry5<-main_result4[[1]]
write.csv(main_registry5, file="main_registry5.csv", row.names=FALSE)
main_result5<-relationship_registry(time5,main_registry5,0)
write.csv(main_result5[[2]], file="main_result5.csv", row.names=FALSE)

main_registry6<-main_result5[[1]]
write.csv(main_registry6, file="main_registry6.csv", row.names=FALSE)
main_result6<-relationship_registry(time6,main_registry6,0)
write.csv(main_result6[[2]], file="main_result6.csv", row.names=FALSE)

main_registry7<-main_result6[[1]]
write.csv(main_registry7, file="main_registry7.csv", row.names=FALSE)
main_result7<-relationship_registry(time7,main_registry7,0)
write.csv(main_result7[[2]], file="main_result7.csv", row.names=FALSE)

main_registry8<-main_result7[[1]]
write.csv(main_registry8, file="main_registry8.csv", row.names=FALSE)
main_result8<-relationship_registry(time8,main_registry8,0)
write.csv(main_result8[[2]], file="main_result8.csv", row.names=FALSE)

main_registry9<-main_result8[[1]]
write.csv(main_registry9, file="main_registry9.csv", row.names=FALSE)
main_result9<-relationship_registry(time9,main_registry9,0)
write.csv(main_result9[[2]], file="main_result9.csv", row.names=FALSE)

main_registry10<-main_result9[[1]]
write.csv(main_registry10, file="main_registry10.csv", row.names=FALSE)
main_result10<-relationship_registry(time10,main_registry10,0)
write.csv(main_result10[[2]], file="main_result10.csv", row.names=FALSE)


final_result_data_main<-data.frame(main_result1,
                                   main_result2,
                                   main_result3,
                                   main_result4,
                                   main_result5,
                                   main_result6,
                                   main_result7,
                                   main_result8,
                                   main_result9,
                                   main_result10,
                                   stringsAsFactors=FALSE)
#final result data frame.
write.csv(final_result_data_main, file="main_relationship_ratios.csv", row.names=FALSE)
  
casual_registry1<-initial_casual_registry
write.csv(casual_registry1, file="casual_registry1.csv", row.names=FALSE)
casual_result1<-relationship_registry(time1,casual_registry1,1)
write.csv(casual_result1[[2]], file="casual_result1.csv", row.names=FALSE)

casual_registry2<-casual_result1[[1]]
write.csv(casual_registry2, file="casual_registry2.csv", row.names=FALSE)
casual_result2<-relationship_registry(time2,casual_registry2,1)
write.csv(casual_result2[[2]], file="casual_result2.csv", row.names=FALSE)

casual_registry3<-casual_result2[[1]]
write.csv(casual_registry3, file="casual_registry3.csv", row.names=FALSE)
casual_result3<-relationship_registry(time3,casual_registry3,1)
write.csv(casual_result3[[2]], file="casual_result3.csv", row.names=FALSE)

casual_registry4<-casual_result3[[1]]
write.csv(casual_registry4, file="casual_registry4.csv", row.names=FALSE)
casual_result4<-relationship_registry(time4,casual_registry4,1)
write.csv(casual_result4[[2]], file="casual_result4.csv", row.names=FALSE)

casual_registry5<-casual_result4[[1]]
write.csv(casual_registry5, file="casual_registry5.csv", row.names=FALSE)
casual_result5<-relationship_registry(time5,casual_registry5,1)
write.csv(casual_result5[[2]], file="casual_result5.csv", row.names=FALSE)

casual_registry6<-casual_result5[[1]]
write.csv(casual_registry6, file="casual_registry6.csv", row.names=FALSE)
casual_result6<-relationship_registry(time6,casual_registry6,1)
write.csv(casual_result6[[2]], file="casual_result6.csv", row.names=FALSE)

casual_registry7<-casual_result6[[1]]
write.csv(casual_registry7, file="casual_registry7.csv", row.names=FALSE)
casual_result7<-relationship_registry(time7,casual_registry7,1)
write.csv(casual_result7[[2]], file="casual_result7.csv", row.names=FALSE)

casual_registry8<-casual_result7[[1]]
write.csv(casual_registry8, file="casual_registry8.csv", row.names=FALSE)
casual_result8<-relationship_registry(time8,casual_registry8,1)
write.csv(casual_result8[[2]], file="casual_result8.csv", row.names=FALSE)

casual_registry9<-casual_result8[[1]]
write.csv(casual_registry9, file="casual_registry9.csv", row.names=FALSE)
casual_result9<-relationship_registry(time9,casual_registry9,1)
write.csv(casual_result9[[2]], file="casual_result9.csv", row.names=FALSE)

casual_registry10<-casual_result9[[1]]
write.csv(casual_registry10, file="casual_registry10.csv", row.names=FALSE)
casual_result10<-relationship_registry(time10,casual_registry10,1)
write.csv(casual_result10[[2]], file="casual_result10.csv", row.names=FALSE)


final_result_data_casual<-data.frame(casual_result1,
                                     casual_result2,
                                     casual_result3,
                                     casual_result4,
                                     casual_result5,
                                     casual_result6,
                                     casual_result7,
                                     casual_result8,
                                     casual_result9,
                                     casual_result10,
                                     stringsAsFactors=FALSE)
#final result data frame.
write.csv(final_result_data_casual, file="casual_relationship_ratios.csv", row.names=FALSE)

main_data_analysis<-read.csv("main_relationship_ratios.csv")

a<-main_data_analysis[,-1]
medianvec<-rep(1,3650)
IQRvec<-rep(1,3650)
minvec<-rep(1,3650)
maxvec<-rep(1,3650)

for(i in 2:length(a[,1])){
  vec<-c(a[[i,1]],
         a[[i,2]],
         a[[i,3]],
         a[[i,4]],
         a[[i,5]],
         a[[i,6]],
         a[[i,7]],
         a[[i,8]])
  medianvec[i]<-median(vec)
  IQRvec[i]<-IQR(vec)
  minvec[i]<-min(vec)
  maxvec[i]<-max(vec)
}

a<-cbind(a,medianvec)
a<-cbind(a,IQRvec)
a<-cbind(a,minvec)
a<-cbind(a,maxvec)

write.csv(a, file="final_main_relationship_ratios+summarystats.csv")


casual_data_analysis<-read.csv("casual_relationship_ratios.csv")

a<-casual_data_analysis[,-1]
medianvec<-rep(1,3650)
IQRvec<-rep(1,3650)
minvec<-rep(1,3650)
maxvec<-rep(1,3650)

for(i in 2:length(a[,1])){
  vec<-c(a[[i,1]],
         a[[i,2]],
         a[[i,3]],
         a[[i,4]],
         a[[i,5]],
         a[[i,6]],
         a[[i,7]],
         a[[i,8]])
  medianvec[i]<-median(vec)
  IQRvec[i]<-IQR(vec)
  minvec[i]<-min(vec)
  maxvec[i]<-max(vec)   
}

a<-cbind(a,medianvec)
a<-cbind(a,IQRvec)
a<-cbind(a,minvec)
a<-cbind(a,maxvec)

write.csv(a, file="final_casual_relationship_ratios+summarystats.csv")

cres1<-read.csv("casual_result1.csv")
cres2<-read.csv("casual_result2.csv")
cres3<-read.csv("casual_result3.csv")
cres4<-read.csv("casual_result4.csv")
cres5<-read.csv("casual_result5.csv")

cres<-append(cres1[[1]],cres1[[1]][3649])
partial_casual_results<-data.frame(cres,
                                   cres2[[1]],
                                   cres3[[1]],
                                   cres4[[1]],
                                   cres5[[1]],
                                   stringsAsFactors = FALSE)

testing<-read.csv("main_relationship_ratios.csv")

