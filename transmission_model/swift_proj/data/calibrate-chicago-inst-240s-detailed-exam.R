rm(list=ls())

## 2018-Jan-05: Add external infections parameter here: Vary min between 0.3 and 0.5 per 100 py, fix max at 1.0 per 100 py

## zoomin into region between values 2 and 3 for minimum chronic infecivity as per the march 20 analysis

   ## seed
   global.random.seed <- 1:30

   acute.mult=5 #params corresponding to instances 241-250 from prior calibration work
   late.mult=1
   min.chronic.infectivity.unadj=0.000922913278546713
   prop.steady.sex.acts=0.189357142857143
   prop.casual.sex.acts=0.053
   circum.mult=1

   ## make parameter grid
   df <- expand.grid(global.random.seed=global.random.seed,
                     acute.mult=acute.mult,
                     late.mult=late.mult,
                     min.chronic.infectivity.unadj=
                     min.chronic.infectivity.unadj,
                     prop.steady.sex.acts=prop.steady.sex.acts,
                     prop.casual.sex.acts=prop.casual.sex.acts,
                     circum.mult=circum.mult
                     )

   run.number <- 1:nrow(df)
   df <- cbind(run.number, df)

    ndf <- names(df)

    l1 <- lapply(1:ncol(df), function(x) paste0(ndf[x],"=",df[[ndf[x]]]))
    res <- do.call(paste,c(l1,c(sep = ",")))

## write as text
write(res, file = "calibrate-chicago-inst241-250-detailed-exam.txt")

## write df as separate csv to make it easier to search
write.csv(df, "calibrate-chicago-inst241-250-detailed-exam.csv")
