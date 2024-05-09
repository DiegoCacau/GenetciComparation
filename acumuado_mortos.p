# set title 'Acumulador de Mortes'                       # plot title
set title 'Mortes Diarias'
set xlabel 'Data'                              # x-axis label
set ylabel 'Mortes'                          # y-axis label

# labels
#set label "boiling point" at 10, 212

# key/legend
set key top right
set key box
set key left bottom
set key bmargin 
set xdata time
set timefmt "%Y-%m-%d"

set format x "%m/%y" 
set xtics "2020-02-25", 4*2628000, "2022-12-30"

set yrange [ 0 : 4500 ] # apenas para o grafico de mortes diarias

set datafile separator ','
# plot "output.csv" using 2:6 with lines title "Acumulado"
plot "output.csv" using 2:6 with lines title ""


set terminal pngcairo
set output 'output_mortes.png'
replot
unset output