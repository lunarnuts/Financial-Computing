set term pngcairo size 1440,900
set output 'Probability.png'

set title "Alice's Probability of Winning over Number of Tosses"

set ylabel "Alice's Probability of Winning"
set yr [0.1:0.45]

set xlabel "Number of Coin Tosses in Each Game"
set xr [0:30]

set key box vertical width 1 height 0.5 maxcols 1 spacing 1.5

plot    "plotData.dat" using 1:3 title 'Experiment' w l lc rgb 'blue', \
        "plotData.dat" using 1:2 title 'Theory' w l lc rgb '#4C9900'