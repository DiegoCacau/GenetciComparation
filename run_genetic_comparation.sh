#!/bin/bash

set -e

CC=g++
CFLAGS=-O

tamanho_populacao=8464
tamanho_matriz=92
tempo_de_teste=90
tempo_da_infecao=100
delta_tempo_da_infecao=0
infectados_inicialmente=1
probabilidade_infectar_usando_mascara=100 # nao usado
probabilidade_adoecer_usando_mascara=100  # nao usado
influencia_de_pessoa_perto=100
influencia_de_pessoa_longe=100
tempo_de_imunidade=0    
tem_movimentacao=0
probabilidade_infectado_se_mover=20  # nao usado
probabilidade_apresentar_sintomas=30 # nao usado
probabilidade_ficar_estado_grave=10  # nao usado
porcentagem_infectados_antes_de_politica_saude=100
metodo_de_saida=FILE
letalidade="$1"  # d
probabilidade_imune_se_infectar=0   #reinfeccao 
carga_viral_booleana=5   # nao mudar
tempo_ante_de_infectar=0 # nao usado
delta_tempo_ante_de_infectar=0 # nao usado
tempo_para_dia=1
numero_de_grupos=1
porcentagem_infectados_para_remover_politica_de_saude=0
somar_tempo_de_infeccao_com_incubacao=0
tamanho_vizinhanca_moore=1
constante_de_infeccao="$2"   # BETA
metodo_de_calcular_infeccao=1
probabilidade_recuperacao="$3" #0.93   GAMA
letalidade_da_populacao="0"  # mi
tipo_de_vizinhanca=0 #0 para Moore e 1 para Newmann
probabilidade_de_isolamento=0 #isolamento do individuo infectado
probabilidade_de_lockdown=0 #isolamento de toda a populacao
probabilidade_de_vacinacao=0
periodo_vacinacao_pulsada=0

$CC -o exec.out $CFLAGS main.cpp

if [ $? -eq 0 ]
then
  ./exec.out $tamanho_populacao $tamanho_matriz $tempo_de_teste $tempo_da_infecao \
             $delta_tempo_da_infecao $infectados_inicialmente $probabilidade_infectar_usando_mascara \
             $probabilidade_adoecer_usando_mascara $influencia_de_pessoa_perto \
             $influencia_de_pessoa_longe $tempo_de_imunidade $tem_movimentacao \
             $probabilidade_infectado_se_mover $probabilidade_apresentar_sintomas \
             $probabilidade_ficar_estado_grave $porcentagem_infectados_antes_de_politica_saude \
             $metodo_de_saida $letalidade $probabilidade_imune_se_infectar $carga_viral_booleana \
             $tempo_ante_de_infectar $delta_tempo_ante_de_infectar $tempo_para_dia $numero_de_grupos \
             $porcentagem_infectados_para_remover_politica_de_saude $somar_tempo_de_infeccao_com_incubacao \
             $tamanho_vizinhanca_moore $constante_de_infeccao $metodo_de_calcular_infeccao \
             $probabilidade_recuperacao $letalidade_da_populacao $tipo_de_vizinhanca $probabilidade_de_isolamento \
             $probabilidade_de_lockdown $probabilidade_de_vacinacao $periodo_vacinacao_pulsada

    # gnuplot -p infectados_porcent.p

    #gnuplot -p graph_infectados.p

    #gnuplot -p graph_reprodutividade.p

  exit 0
else
  exit 1
fi


