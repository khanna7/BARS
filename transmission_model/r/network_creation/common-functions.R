## Some needed functions

classify.stage.numeric.rewrite <- function(time.since.infection){

  stage.of.infection <- rep(NA, length(time.since.infection))

  for (i in 1:length(time.since.infection)){

    if (time.since.infection[i] %in% acute.length){
      stage.of.infection[i] <- 0

    } else if (time.since.infection[i] %in% chronic.length){
      stage.of.infection[i] <- 1

    } else if (time.since.infection[i] %in% late.length){
      stage.of.infection[i] <- 2

    }

  }
  return(stage.of.infection)
}
