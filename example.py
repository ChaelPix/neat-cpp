import build.neat as neat

expected: int = 10
inputs: list[int] = [1, 2, 3, 4, 5, 6, 7, 8, 9,
                     10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]


def evaluate_genome(g: neat.Genome, generation: int):
    result: list[float] = g.feed_forward(inputs)
    g.fitness = 1 / abs(expected - result[0])


def callback_generation(p: neat.Population, generation: int):
    print(f"Generation: {generation}")
    print(f"Best fitness: {p.best_fitness}")


def main():

    c: neat.NeatConfig = neat.load_config("default_config.txt")
    p: neat.Population = neat.Population(c)

    generations: int = 100
    p.run(evaluate_genome, generations, callback_generation)

    p.best_genome.print()

    # Display the results of the best genome
    result = p.best_genome.feed_forward(inputs)
    if result[0] - expected >= 0:
        solution = expected - 1 / p.best_genome.fitness
        print(f"Solution found: {solution}\n")
    else:
        solution = expected + 1 / p.best_genome.fitness
        print(f"Solution found: {solution}\n")

    # Save this genome
    id = p.best_genome.id
    p.best_genome.save("./cache/genomes/genome_" + id + ".json")

    # Load the genome
    g = neat.Genome.load("./cache/genomes/genome_" + id + ".json")
    g.print()


if __name__ == '__main__':
    main()
