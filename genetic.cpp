#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>


// Definição de parâmetros do algoritmo genético
const int POPULATION_SIZE = 15;
const int NUM_GENERATIONS = 10;
const double MUTATION_RATE = 0.4;
const int PARENTS_SIZE = 5;
const int MUTATED_SIZE = 3;
const double MAX_FITNESS = 999999.9;

// Estrutura para representar um indivíduo
struct Individual {
    std::vector<double> params; // Parâmetros do indivíduo
    double fitness; // Valor de adequação (fitness)
};

inline bool operator== (const Individual &a, const Individual &b) {
      return (a.params[0] == b.params[0]) 
         && (a.params[1] == b.params[1])
         && (a.params[2] == b.params[2])
         && (a.params[3] == b.params[3]);
}

void parserCSV(std::string file, std::vector<int>& values){
    std::string line;       /* string to hold each line read from file  */
    //std::vector<std::vector<int>> values {};    /* vector vector of int */
    std::ifstream f(file);                  /* file stream to read  */

    while (getline(f, line)) { /* read each line into line */
        /* if no digits in line - get next */
        if (line.find_first_of("0123456789") == std::string::npos)
            continue;
        int itmp;                               /* temporary int */
        std::stringstream ss (line);            /* stringstream from line */
        while (ss >> itmp) {                    /* read int from stringstream */
            std::string tmpstr;                 /* temporary string to ',' */
            values.push_back(itmp);                /* add int to tmp */
            if (!getline (ss, tmpstr, ','))     /* read to ',' w/tmpstr */
                break;                          /* done if no more ',' */
        } 
    }
}

// Função de fitness (maximização)
double fitnessFunction(const std::vector<int> &predicted, const std::vector<int> &actual) {
    int size = 3;
    if (predicted.size() != actual.size() &&
        predicted.size() != size ) {
        std::cerr << "Erro: As dimensões dos vetores são diferentes." << std::endl;
        return MAX_FITNESS;
    }

    double sumSquaredError = 0.0;
    for (size_t i = 0; i < actual.size(); ++i) {
        double error = predicted[i] - actual[i];
        sumSquaredError += error * error;
    }

    return sumSquaredError / predicted.size();
}

// Função para inicializar a população inicial
void initializePopulation(std::vector<Individual> &population, const std::vector<std::pair<double, double>> &domains) {
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        std::vector<double> params;
        for (const auto &domain : domains) {
            double random_param = domain.first + static_cast<double>(rand()) / RAND_MAX * (domain.second - domain.first);
            params.push_back(random_param);
        }
        population.push_back({params, MAX_FITNESS}); // Inicializa com fitness 9999999
    }
}

// Função para encontrar os N indivíduos com maior fitness
std::vector<Individual> findParents(const std::vector<Individual> &population, const int& size) {
    // Crie uma cópia da população para preservar os dados originais
    std::vector<Individual> sorted_population = population;

    // Use std::partial_sort para classificar apenas os N maiores elementos com base no fitness
    std::partial_sort(sorted_population.begin(), sorted_population.begin() + size, sorted_population.end(),
                      [](const Individual &a, const Individual &b) { return a.fitness < b.fitness; });

    // Crie um novo vetor contendo os N indivíduos com maior fitness
    std::vector<Individual> top_N(sorted_population.begin(), sorted_population.begin() + size);

    return top_N;
}

// Função para avaliar o fitness de cada indivíduo na população
void calculateFitness(std::vector<Individual> &population, std::vector<int>& expected_values) {
    for (auto &individual : population) {

        std::ostringstream strs;
        strs << individual.params[0] << " ";
        strs << individual.params[1] << " ";
        strs << individual.params[2] << " ";
        strs << individual.params[3];
        
        std::string values = "cd /home/diegocacau/Documents/TCC/Codigo && ./run_genetic_comparation.sh " + strs.str();

        std::cout << values << "\n\n\n";

        std::system(values.c_str());

        std::vector<int> actual_values {};
        parserCSV("/home/diegocacau/Documents/TCC/Codigo/deathAccumulated.csv", actual_values);

        individual.fitness = fitnessFunction(actual_values, expected_values);

        std::cout << "Individual fitness: " << individual.fitness << "\n\n\n";
    }
}

Individual findDifferentParent(const std::vector<Individual> &population, const Individual& other_parent){
    Individual parent = population[rand() % population.size()];

    int i = 0;
    while((other_parent.params[0] == parent.params[0]) 
         && (other_parent.params[1] == parent.params[1])
         && (other_parent.params[2] == parent.params[2])
         && (other_parent.params[3] == parent.params[3]) ){
        
        parent = population[rand() % population.size()];
        i++;

        if(i > 2*POPULATION_SIZE){
            break;
        }
    }

    return parent;
}

Individual findElementNotIn( std::vector<Individual> &population, 
                                std::vector<Individual>& notHere){
    Individual element = population[rand() % population.size()];

     
    int i = 0;
    while(std::find(notHere.begin(), notHere.end(), element) != notHere.end()){
        element = population[rand() % population.size()];
        i++;

        if(i > 2*POPULATION_SIZE){
            break;
        }
    }

    return element;
}

// Função de seleção de pais por torneio
Individual tournamentSelection(const std::vector<Individual> &population, const Individual& other_parent) {
    const int TOURNAMENT_SIZE = 5;
    Individual best_parent = findDifferentParent(population, other_parent);

    for (int i = 0; i < TOURNAMENT_SIZE; ++i) {
        Individual competitor = findDifferentParent(population, best_parent);
        if (competitor.fitness < best_parent.fitness) {
            best_parent = competitor;
        }
    }

    return best_parent;
}                                

// Função para realizar o cruzamento (crossover) entre dois pais
Individual crossover(const Individual &parent1, const Individual &parent2) {
    std::vector<double> child_params;
    for (size_t i = 0; i < parent1.params.size(); ++i) {
        double crossover_point = static_cast<double>(rand()) / RAND_MAX;
        // std::cout << "Crossover: " << crossover_point << "\n";
        double child_param = parent1.params[i] * crossover_point + parent2.params[i] * (1.0 - crossover_point);
        child_params.push_back(child_param);
    }
    return {child_params, MAX_FITNESS}; // Inicializa com fitness 999999
}

// Função para realizar mutação em um indivíduo
void mutate(Individual &individual, const std::vector<std::pair<double, double>> &domains) {
    for (size_t i = 0; i < individual.params.size(); ++i) {
        if (static_cast<double>(rand()) / RAND_MAX < MUTATION_RATE) {
            double mutation = (static_cast<double>(rand()) / RAND_MAX - 0.5) * 0.1; // Mutação gaussiana
            // std::cout << "Mutation: " << mutation << "\n";
            individual.params[i] += mutation;
            // Garantir que os parâmetros permaneçam dentro dos limites dos domínios
            individual.params[i] = std::max(domains[i].first, std::min(domains[i].second, individual.params[i]));
        }
    }
}

// Função principal do algoritmo genético
void geneticAlgorithm(const std::vector<std::pair<double, double>> &domains) {
    std::vector<Individual> population;
    initializePopulation(population, domains);

    std::vector<int> expected_values {};
    //std::string file = "/home/diegocacau/Documents/TCC/Comparacao/GenetciComparation/infectados_araruama.csv";
    std::string file = "/home/diegocacau/Documents/TCC/Comparacao/GenetciComparation/mortos_italia.csv";
    parserCSV(file, expected_values);

    std::cout << "Entrando no for \n";
    for (int generation = 0; generation < NUM_GENERATIONS; ++generation) {

        std::cout << "calculateFitness\n";
        calculateFitness(population, expected_values);

        std::vector<Individual> new_population;
        std::vector<Individual> parents = findParents(population, PARENTS_SIZE);

        new_population.insert(new_population.end(), parents.begin(), parents.end());

        std::cout << "for interior\n";
        for (int i = 0; i < POPULATION_SIZE - PARENTS_SIZE - MUTATED_SIZE; ++i) {
            Individual dummy;
            dummy.fitness = MAX_FITNESS;
            dummy.params.push_back(0);
            dummy.params.push_back(0);
            dummy.params.push_back(0);
            dummy.params.push_back(0);


            Individual parent1 = tournamentSelection(parents, dummy);
            Individual parent2 = tournamentSelection(parents, parent1);
            Individual offspring = crossover(parent1, parent2);

            new_population.push_back(offspring);
        }   

        std::cout << "for mutate\n";
        for (int i = 0; i < MUTATED_SIZE; ++i) {
            Individual element = findElementNotIn(population, parents);

            mutate(element, domains);
            new_population.push_back(element);
        }


        population = new_population;
    }

    // Após o número de gerações especificado, exibe o melhor indivíduo
    Individual best_individual = population[0];
    for (const auto &individual : population) {
        if (individual.fitness < best_individual.fitness) {
            best_individual = individual;
        }
    }

    // Exibe os melhores parâmetros e o valor de adequação correspondente
    std::cout << "Melhor solução encontrada:\n";
    for (size_t i = 0; i < best_individual.params.size(); ++i) {
        std::cout << "Parâmetro " << i+1 << ": " << best_individual.params[i] << std::endl;
    }
    std::cout << "Fitness: " << best_individual.fitness << std::endl;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed do gerador de números aleatórios

    // Definição dos domínios para cada parâmetro
    std::vector<std::pair<double, double>> domains = {
                                    {0.001, 0.05}, // letalidade doenca
                                    {0.001, 1.5}, // constante de infecção
                                    {0.05, 0.30}, // probabilidade de recuperacao
                                    //{0.05, 0.30}, // letalidade da populacao nao relacionada a doenca
                                    }; 
    geneticAlgorithm(domains); // Execução do algoritmo genético

    return 0;
}