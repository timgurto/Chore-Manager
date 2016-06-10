svg(filename="sprint/burndown.svg", width=8, height=10, pointsize=12, bg="black")
par(bg = "black")
par(fg="#999999")

people <- read.csv(file="people.dat", head=FALSE)
people <- as.vector(t(people))

input <- read.csv(file="sprint/burndown.dat", head=FALSE, sep=",")
colnames(input) <- c("time", people)

sprintStart <- read.table(file="sprint/start.dat", head=FALSE)
colnames(sprintStart) <- "a"
sprintStart <- sprintStart$a

sprintEnd <- read.table(file="sprint/end.dat", head=FALSE)
colnames(sprintEnd) <- "a"
sprintEnd <- sprintEnd$a

library("RColorBrewer")
colors = brewer.pal(max(3, length(people)), "Set1")

topRow = as.vector(input[1,])
topRow[1] <- NULL # exclude time field
maxDays <- max(topRow)



plot(
	NULL,
	xlim=c(sprintStart, sprintEnd),
	xlab="",
	ylim=c(0, maxDays),
	ylab="Work remaining",
	axes=FALSE,
	col.lab="#999999", col.main="#999999",
	#yaxs="i",
	xaxs="i"
)


first6h = ceiling(sprintStart / 21600) * 21600 - 14400
threeHourIncrements = seq(from=first6h, to=sprintEnd, by=21600)
threeHourIncrements = as.POSIXct(threeHourIncrements, origin="1970-01-01")

firstDay = ceiling(sprintStart / 86400) * 86400 + 7200 - 86400
dayIncrements = seq(from=firstDay, to=(sprintEnd + 86400), by=86400)
dayIncrements = as.POSIXct(dayIncrements, origin="1970-01-01")


firstMidnight = ceiling(sprintStart / 86400) * 86400 - 36000
if (firstMidnight > sprintEnd) {
    midnightIncrements = c();
} else {
    midnightIncrements = seq(from=firstMidnight, to=sprintEnd, by=86400)
}
abline(v=midnightIncrements, col="#444444")

for (i in 2:(length(people)+1)){
	lines(input$time, t(input[i]), col=colors[i-1], lwd=3)
	intercept = t(input[i])[1]
	slope = intercept / (sprintEnd - sprintStart) * -1
	intercept = intercept - slope * sprintStart
	abline(a=intercept, b=slope, col=colors[i-1], lty=3)
}

legend("topright", legend=people, fill=colors, bg = "black")

axis(
	1,
	at=dayIncrements,
	#labels=format(threeHourIncrements, format = "%a %l%p"),
	labels=format(dayIncrements, format = "%A"),
	col="#999999", col.ticks="#999999", col.axis="#999999",
	#las = 2
)
axis(
	2, las=2,
	col="#999999", col.ticks="#999999", col.axis="#999999"
)

#box()


dev.off()