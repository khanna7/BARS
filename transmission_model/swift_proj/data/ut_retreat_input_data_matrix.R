rm(list=ls())

global.random.seed <- 1
acute.mult = seq(1, 10, 1)
late.mult = seq(1, 5, 1)
circum.mult = c(0.40, 0.70, 1)
mean.time.until.next.test <- c(365*1, 365*2, round(365*0.5))

df <- expand.grid(global.random.seed=global.random.seed,
                  acute.mult=acute.mult,
                  late.mult=late.mult,
                  circum.mult=circum.mult,
                  mean.time.until.next.test=mean.time.until.next.test
                  )
ndf <- names(df)

l1 <- lapply(1:3,function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
res <- do.call(paste,c(l1,c(sep = ",")))

write(res, file = "input_ut_retreat.txt")
