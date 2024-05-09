set title 'Novos Casos'                       # plot title
set xlabel 'Data'                              # x-axis label
set ylabel 'Infectados'                          # y-axis label

# labels
#set label "boiling point" at 10, 212

# key/legend
set key top right
set key box
set key left bottom
set key bmargin 
set xdata time
set timefmt "%Y-%m-%d"

#set timefmt "%Y-%m"
set format x "%m/%y" # Or some other output format you prefer
set xtics "2020-02-25", 4*2628000, "2022-12-30"


set datafile separator ','
plot "output.csv" using 2:5 with lines title "Infectados"

set terminal pngcairo
set output 'output.png'
replot
unset output