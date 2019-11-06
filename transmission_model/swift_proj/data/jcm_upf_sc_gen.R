# Example format
# global.random.seed=1,stop.at=3650


df <- expand.grid(global.random.seed = seq(1, 10, 1), stop.at = 3650)
ndf <- names(df)

l1 <- lapply(1:2,function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))
write(res,file = "baseline.txt")
