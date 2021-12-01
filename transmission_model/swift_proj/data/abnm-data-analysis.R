############################################################################
### Analsis Codes for 30% Increase in PrEP-only, ART-only and ART & PrEP ###
############################################################################

newinf_full <- NULL
newinf_range <- NULL
ranges <- c(18, 34)
for(i in 1:120){
	counts <- read.csv(sprintf("/project2/khanna7/mert/devBARS/BARS/transmission_model/swift_proj/experiments/mert_abby_prep_art_40/instance_%d/output/counts.csv", i))
	counts <- counts[-1, ]
	counts_yr_chunks <- split(counts, ceiling(seq_along(counts$tick)/365))
	print(i)
	# print(calc_new_inf(counts_yr_chunks, c("infected_via_transmission")))
	# print("******************")
	newinf_full <- rbind(newinf_full, calc_new_inf(counts_yr_chunks, c("infected_via_transmission")))
	newinf_range_result <-  sapply(seq(1, length(ranges) - 1, by = 2), function(x) {
		numerators <- sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1]))
		calc_new_inf(counts_yr_chunks, numerators)
		})
	newinf_range <- rbind(newinf_range, t(newinf_range_result))
}

newinf_full <- cbind(rep(1:4, each = 30), newinf_full)
newinf_range <- cbind(rep(1:4, each = 30), newinf_range)

newinf_range <- aggregate(newinf_range[, 2:ncol(newinf_range)], by = list(newinf_range[, 1]), FUN = mean)
newinf_range <- newinf_range[, -1]

alldat <- rbind(alldat,
				cbind(Year = rep(2017:2030, 4), Intervention = c(rep("Base", 14), rep("PrEP-only (30%)", 14), rep("ART-only (30%)", 14), rep("ART & PrEP (30%)", 14)), Incidence = as.vector(t(newinf_range))))


############################################################################
### Analsis Codes for 20% Increase in PrEP-only, ART-only and ART & PrEP ###
############################################################################

newinf_full <- NULL
newinf_range <- NULL
ranges <- c(18, 34)
for(i in 1:120){
	counts <- read.csv(sprintf("/project2/khanna7/mert/devBARS/BARS/transmission_model/swift_proj/experiments/mert_abby_repl_v3/instance_%d/output/counts.csv", i))
	counts <- counts[-1, ]
	counts_yr_chunks <- split(counts, ceiling(seq_along(counts$tick)/365))
	print(i)
	# print(calc_new_inf(counts_yr_chunks, c("infected_via_transmission")))
	# print("******************")
	newinf_full <- rbind(newinf_full, calc_new_inf(counts_yr_chunks, c("infected_via_transmission")))
	newinf_range_result <-  sapply(seq(1, length(ranges) - 1, by = 2), function(x) {
		numerators <- sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1]))
		calc_new_inf(counts_yr_chunks, numerators)
		})
	newinf_range <- rbind(newinf_range, t(newinf_range_result))
}

newinf_full <- cbind(rep(1:4, each = 30), newinf_full)
newinf_range <- cbind(rep(1:4, each = 30), newinf_range)

newinf_range <- aggregate(newinf_range[, 2:ncol(newinf_range)], by = list(newinf_range[, 1]), FUN = mean)
newinf_range <- newinf_range[, -1]

alldat <- cbind(Year = rep(2017:2030, 4), Intervention = c(rep("Base", 14), rep("PrEP-only (20%)", 14), rep("ART-only (20%)", 14), rep("ART & PrEP (20%)", 14)), Incidence = as.vector(t(newinf_range)))

###################################################
### Figure 2 - Part (c) with CIs (ABNM Results) ###
###################################################

rm(list = ls())
library(ggplot2)

calc_new_inf <- function(counts_yr_chunks, numerators) {
  #print(length(numerators))
  
  if (length(numerators) == 1) {
    sum_new_inf <- lapply(counts_yr_chunks, function (x)
      sum(x[2:nrow(x), numerators])
    )
  } else {
    sum_new_inf <- lapply(counts_yr_chunks, function (x)
      sum(Reduce("+", (x[2:nrow(x), numerators])))
    )
  }
  return(unlist(sum_new_inf))
}

newinf_full <- NULL
newinf_range <- NULL
ranges <- c(18, 34)
for(i in 1:120){
	counts <- read.csv(sprintf("/project2/khanna7/mert/devBARS/BARS/transmission_model/swift_proj/experiments/mert_abby_repl_v3/instance_%d/output/counts.csv", i))
	counts <- counts[-1, ]
	counts_yr_chunks <- split(counts, ceiling(seq_along(counts$tick)/365))
	print(i)
	newinf_full <- rbind(newinf_full, calc_new_inf(counts_yr_chunks, c("infected_via_transmission")))
	newinf_range_result <-  sapply(seq(1, length(ranges) - 1, by = 2), function(x) {
		numerators <- sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1]))
		calc_new_inf(counts_yr_chunks, numerators)
		})
	newinf_range <- rbind(newinf_range, t(newinf_range_result))
}

newinf_full <- cbind(rep(1:4, each = 30), newinf_full)
newinf_range <- cbind(rep(1:4, each = 30), newinf_range)

se <- function(x) {
	x <- as.matrix(x) 
	return(apply(x, 2, sd) / sqrt(nrow(x)))
}

newinf_range_se <- aggregate(newinf_range[, 2:ncol(newinf_range)], by = list(newinf_range[, 1]), FUN = se)
newinf_range_se <- newinf_range_se[, -1]
newinf_range_mean <- aggregate(newinf_range[, 2:ncol(newinf_range)], by = list(newinf_range[, 1]), FUN = mean)
newinf_range_mean <- newinf_range_mean[, -1]

alldat1 <- data.frame(Intervention = c(rep("Base", 14), rep("PrEP-only (20%)", 14), rep("ART-only (20%)", 14), rep("ART & PrEP (20%)", 14)),
					year = rep(2017:2030, 4),
					incidence = as.vector(t(newinf_range_mean)),
					incidence_lb = as.vector(t(newinf_range_mean) - t(newinf_range_se)),
					incidence_ub = as.vector(t(newinf_range_mean) + t(newinf_range_se)))

newinf_full <- NULL
newinf_range <- NULL
ranges <- c(18, 34)
for(i in 1:120){
	counts <- read.csv(sprintf("/project2/khanna7/mert/devBARS/BARS/transmission_model/swift_proj/experiments/mert_abby_prep_art_40/instance_%d/output/counts.csv", i))
	counts <- counts[-1, ]
	counts_yr_chunks <- split(counts, ceiling(seq_along(counts$tick)/365))
	print(i)
	newinf_full <- rbind(newinf_full, calc_new_inf(counts_yr_chunks, c("infected_via_transmission")))
	newinf_range_result <-  sapply(seq(1, length(ranges) - 1, by = 2), function(x) {
		numerators <- sprintf("infected_via_transmission_%s", seq(ranges[x], ranges[x+1]))
		calc_new_inf(counts_yr_chunks, numerators)
		})
	newinf_range <- rbind(newinf_range, t(newinf_range_result))
}

newinf_full <- cbind(rep(1:4, each = 30), newinf_full)
newinf_range <- cbind(rep(1:4, each = 30), newinf_range)

se <- function(x) {
	x <- as.matrix(x) 
	return(apply(x, 2, sd) / sqrt(nrow(x)))
}

newinf_range_se <- aggregate(newinf_range[, 2:ncol(newinf_range)], by = list(newinf_range[, 1]), FUN = se)
newinf_range_se <- newinf_range_se[, -1]
newinf_range_mean <- aggregate(newinf_range[, 2:ncol(newinf_range)], by = list(newinf_range[, 1]), FUN = mean)
newinf_range_mean <- newinf_range_mean[, -1]

alldat2 <- data.frame(Intervention = c(rep("Base", 14), rep("PrEP-only (30%)", 14), rep("ART-only (30%)", 14), rep("ART & PrEP (30%)", 14)),
					year = rep(2017:2030, 4),
					incidence = as.vector(t(newinf_range_mean)),
					incidence_lb = as.vector(t(newinf_range_mean) - t(newinf_range_se)),
					incidence_ub = as.vector(t(newinf_range_mean) + t(newinf_range_se)))					
						
alldataa <- rbind(alldat1, alldat2) 					
alldataa <- alldataa[-c(57:70), ]
alldataa$Intervention <- factor(alldataa$Intervention, levels = c("Base", "PrEP-only (20%)", "ART-only (20%)", "ART & PrEP (20%)", "PrEP-only (30%)", "ART-only (30%)", "ART & PrEP (30%)"))

w <- 15
h <- 8

x11(width = w / 2.54, height = h / 2.54)

color <- c("black", "red", "blue", "green", "pink", "brown", "yellow", "orange")
p3 <- ggplot(alldataa, aes(x = year, color = Intervention, fill = Intervention)) + 
   xlab("Year") +
   ylab("New Infections") +
   labs(title = "Agent-Based Network Model") +
   ylim(0, 450) +
   geom_ribbon(aes(ymin = alldataa$incidence_lb, ymax = alldataa$incidence_ub), colour = NA, alpha = 0.1) +
   geom_line(aes(y = incidence)) +
   guides(color = guide_legend("Scenario")) +
   scale_color_manual(values = color) +
   scale_fill_manual(values = color) +
   guides(fill = FALSE) +
   theme(legend.position="bottom") +
   labs(tag = "(c)")

#################
### Figure A1 ###
#################

art <- NULL
onart <- NULL
for (i in 1:120){
	counts <- read.csv(sprintf("/project2/khanna7/mert/devBARS/BARS/transmission_model/swift_proj/experiments/mert_abby_prep_art_40/instance_%d/output/counts.csv", i))
	counts <- counts[-1, ]
	onart <- rbind(onart, counts[, "on_art"])
	art <- rbind(art, counts[, "on_art"] / (counts[, "vertex_count"] - counts[, "uninfected"]))
}

plot(art[1, ], type = "l", ylim = c(0, 1))
for (i in 2:30){
	lines(art[i, ])
}
for (i in 31:60){
	lines(art[i, ], col = "red")
}
for (i in 61:90){
	lines(art[i, ], col = "yellow")
}
for (i in 91:120){
	lines(art[i, ], col = "green")
}

art <- as.data.frame(art)
art_avg <- aggregate(art[, 1:ncol(art)], by = list(rep(1:4, each = 30)), FUN = mean)
art_base <- art_avg[1, ]
art_prep_inc30 <- art_avg[2, ]
art_art_inc30 <- art_avg[3, ]
art_artprep_inc30 <- art_avg[4, ]

art <- NULL
onart <- NULL
for (i in 1:120){
	counts <- read.csv(sprintf("/project2/khanna7/mert/devBARS/BARS/transmission_model/swift_proj/experiments/mert_abby_repl_v3/instance_%d/output/counts.csv", i))
	counts <- counts[-1, ]
	onart <- rbind(onart, counts[, "on_art"])
	art <- rbind(art, counts[, "on_art"] / (counts[, "vertex_count"] - counts[, "uninfected"]))
}

plot(art[1, ], type = "l", ylim = c(0, 1))
for (i in 2:30){
	lines(art[i, ])
}
for (i in 31:60){
	lines(art[i, ], col = "red")
}
for (i in 61:90){
	lines(art[i, ], col = "yellow")
}
for (i in 91:120){
	lines(art[i, ], col = "green")
}

art <- as.data.frame(art)
art_avg <- aggregate(art[, 1:ncol(art)], by = list(rep(1:4, each = 30)), FUN = mean)
art_prep_inc20 <- art_avg[2, ]
art_art_inc20 <- art_avg[3, ]
art_artprep_inc20 <- art_avg[4, ]

art_base <- art_base[, -1]
rownames(art_base) <- "value"
art_prep_inc30 <- art_prep_inc30[, -1]
rownames(art_prep_inc30) <- "value"
art_art_inc30 <- art_art_inc30[, -1]
rownames(art_art_inc30) <- "value"
art_artprep_inc30 <- art_artprep_inc30[, -1]
rownames(art_artprep_inc30) <- "value"
art_prep_inc20 <- art_prep_inc20[, -1]
rownames(art_prep_inc20) <- "value"
art_art_inc20 <- art_art_inc20[, -1]
rownames(art_art_inc20) <- "value"
art_artprep_inc20 <- art_artprep_inc20[, -1]
rownames(art_artprep_inc20) <- "value"
  
par(oma = c(7, 0, 0, 0), mar = c(0.5, 5, 0.5, 1), xpd = NA)
plot(unlist(art_base[1, ]), type = "l", ylim = c(0.4, 0.9), xlab = "Year", ylab = "Proportion of HIV-positives\non ART", xaxt = "n", lwd = 1.5)
lines(unlist(art_prep_inc30[1, ]), col = "pink", lwd = 1.5)
lines(unlist(art_art_inc30[1, ]), col = "brown", lwd = 1.5)
lines(unlist(art_artprep_inc30[1, ]), col = "yellow", lwd = 1.5)
lines(unlist(art_prep_inc20[1, ]), col = "red", lwd = 1.5)
lines(unlist(art_art_inc20[1, ]), col = "blue", lwd = 1.5)
lines(unlist(art_artprep_inc20[1, ]), col = "green", lwd = 1.5)
axis(1, at = c(1, 365 * 4, 365 * 8, 365 * 12), labels = c("2016", "2020", "2024", "2028"))

legend(-1.5, 0.15, ncol = 4, legend = c("Base", "ART-only (20%)", "PrEP-only (20%)", "ART & PrEP (20%)", "ART-only (30%)", "PrEP-only (30%)", "ART & PrEP (30%)"), 
		col=c("black", "blue", "red", "green", "brown", "pink", "yellow"), cex = 0.7, lty = 1, lwd = 1.5, title = "Scenario", bty = "n")
