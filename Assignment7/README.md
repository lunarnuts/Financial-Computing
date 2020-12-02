### Losing as Little as Possible

this is a solution based on Prof. Sreenivas's source files
Algorithm is very fast if pseudo uniform RV generator is used, takes 6 seconds to process 10^6 samples.
to use it, run:
`make -f Makefile`
this will generate executable named "Loosing as Little as Possible"
to run executable u have to call it with 2 additional parameters, first is probability for alice, second for bob. For example:
`./Loosing\ as\ Little\ as\ Possible 0.18 0.2`
this generates and output file named `plotData.dat`, a structured data with values for theoretical and experimental data.
to plot graph it uses free opensource software GNUPLOT. You have to install it to run plotter script named `plot.p`
to run it using GNUPLOT run command:
`gnuplot\ngnuplot> load 'plot.p'`
which will generate image named `Probability.png`
