rm(list=ls())

## seed
global.random.seed <- 1:10

## incarceration prob
incarceration.prob <- c(0.0000398, 0.0000787, 0.0001559) 

## incarceration prob (with previoius history)
incarceration.with.cji.prob  <- c(0.0003583, 0.0005173, 0.0007469)

jail.serving.time.mean <- c(19.1, 58.4, 97.7)


## make parameter grid
df <- expand.grid(global.random.seed=global.random.seed,
                  incarceration.prob=incarceration.prob,
                  incarceration.with.cji.prob=incarceration.with.cji.prob,
                  jail.serving.time.mean=jail.serving.time.mean,
                  is.network.disruption.on = "true",
                  is.care.disruption.on = "true"
)

run.number <- 1:nrow(df)
df <- cbind(run.number, df)

ndf <- names(df)

l1 <- lapply(1:ncol(df), function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
upf <- do.call(paste,c(l1,c(sep = ",")))

## write as text
write(upf, file = "calibrate-jcm.txt")

## write df as separate csv to make it easier to search
write.csv(df, "calibrate-jcm.csv")