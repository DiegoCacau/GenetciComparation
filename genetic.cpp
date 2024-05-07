#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Função de fitness (maximização)
double fitnessFunction(const std::vector<double> &predicted, const std::vector<double> &actual) {
    if (predicted.size() != actual.size()) {
        std::cerr << "Erro: As dimensões dos vetores são diferentes." << std::endl;
        return 0.0;
    }

    double sumSquaredError = 0.0;
    for (size_t i = 0; i < predicted.size(); ++i) {
        double error = predicted[i] - actual[i];
        sumSquaredError += error * error;
    }

    return sumSquaredError / predicted.size();
}

// Definição de parâmetros do algoritmo genético
const int POPULATION_SIZE = 100;
const int NUM_GENERATIONS = 1000;
const double MUTATION_RATE = 0.01;
const int PARENTS_SIZE = 30;

// Estrutura para representar um indivíduo
struct Individual {
    std::vector<double> params; // Parâmetros do indivíduo
    double fitness; // Valor de adequação (fitness)
};

// Função para inicializar a população inicial
void initializePopulation(std::vector<Individual> &population, const std::vector<std::pair<double, double>> &domains) {
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        std::vector<double> params;
        for (const auto &domain : domains) {
            double random_param = domain.first + static_cast<double>(rand()) / RAND_MAX * (domain.second - domain.first);
            params.push_back(random_param);
        }
        population.push_back({params, 0.0}); // Inicializa com fitness 0.0
    }
}

// Função para encontrar os N indivíduos com maior fitness
std::vector<Individual> findParents(const std::vector<Individual> &population, const int& size) {
    // Crie uma cópia da população para preservar os dados originais
    std::vector<Individual> sorted_population = population;

    // Use std::partial_sort para classificar apenas os N maiores elementos com base no fitness
    std::partial_sort(sorted_population.begin(), sorted_population.begin() + size, sorted_population.end(),
                      [](const Individual &a, const Individual &b) { return a.fitness > b.fitness; });

    // Crie um novo vetor contendo os N indivíduos com maior fitness
    std::vector<Individual> top_N(sorted_population.begin(), sorted_population.begin() + size);

    return top_N;
}

// Função para avaliar o fitness de cada indivíduo na população
void calculateFitness(std::vector<Individual> &population) {
    for (auto &individual : population) {
        individual.fitness = fitnessFunction(individual.params);
    }
}

Individual findDifferentParent(const std::vector<Individual> &population, const Individual& other_parent){
    Individual parent = population[rand() % population.size()];

    while(other_parent == parent){
        parent = population[rand() % population.size()];
    }

    return parent;
}

// Função de seleção de pais por torneio
Individual tournamentSelection(const std::vector<Individual> &population, const Individual& other_parent) {
    const int TOURNAMENT_SIZE = 5;
    Individual best_parent = findDifferentParent(population, other_parent);

    for (int i = 0; i < TOURNAMENT_SIZE; ++i) {
        Individual competitor = findDifferentParent(population, best_parent);
        if (competitor.fitness > best_parent.fitness) {
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
        double child_param = parent1.params[i] * crossover_point + parent2.params[i] * (1.0 - crossover_point);
        child_params.push_back(child_param);
    }
    return {child_params, 0.0}; // Inicializa com fitness 0.0
}

// Função para realizar mutação em um indivíduo
void mutate(Individual &individual, const std::vector<std::pair<double, double>> &domains) {
    for (size_t i = 0; i < individual.params.size(); ++i) {
        if (static_cast<double>(rand()) / RAND_MAX < MUTATION_RATE) {
            double mutation = (static_cast<double>(rand()) / RAND_MAX - 0.5) * 0.1; // Mutação gaussiana
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

    for (int generation = 0; generation < NUM_GENERATIONS; ++generation) {
        calculateFitness(population);

        std::vector<Individual> new_population;
        std::vector<Individual> parents = findParents(population, PARENTS_SIZE);

        for (int i = 0; i < POPULATION_SIZE; ++i) {
            Individual parent1 = tournamentSelection(parents);
            Individual parent2 = tournamentSelection(parents);
            Individual offspring = crossover(parent1, parent2);
            mutate(offspring, domains);
            new_population.push_back(offspring);
        }

        population = new_population;
    }

    // Após o número de gerações especificado, exibe o melhor indivíduo
    Individual best_individual = population[0];
    for (const auto &individual : population) {
        if (individual.fitness > best_individual.fitness) {
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
    std::vector<std::pair<double, double>> domains = {{-10.0, 10.0}, {-5.0, 5.0}}; // Exemplo: dois parâmetros com domínios [-10, 10] e [-5, 5]

    geneticAlgorithm(domains); // Execução do algoritmo genético

    return 0;
}