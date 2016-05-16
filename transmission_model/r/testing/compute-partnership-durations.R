## (fast method from jonathan)

# looking for mean partnership durations
require(data.table)
dt <- fread("partnership_events_36.csv")
dt_main <- dt[network_type == 0]

# group by p1, p2
dt_main[,.(counts = .N),by=.(p1,p2),][,unique(counts)] # 1 2 4 3dt_main[,.(counts = .N),by=.(p1,p2),][,.N,by=counts]

# counts     N
# 1:      1  5501
# 2:      2 21833
# 3:      4    10
# 4:      3     1

# so focus on just 2 is justified
    ## (old DONOTUSE: res <- dt_main[,.(tick, counts = .N),by=.(p1,p2),][counts == 2,abs(diff(tick))])

res <- dt_main[,.(tick, counts = .N),by=.(p1,p2),][counts == 2,.(dur = abs(diff(tick))), by=.(p1,p2)][,dur]

summary(res)
hist(res)
