//
// Created by christian on 10/10/21.
//

#ifndef MLX86_DARWIN2021_H
#define MLX86_DARWIN2021_H

class World {
public:
    class Biome {
    public:
        std::string name;
        bool cold;
        bool hot;
        bool water;
        bool land;
        std::array<double, 8> resource_production;
        std::array<double, 8> resources{0, 0, 0, 0, 0, 0, 0, 0};
        double total_population = 0;

        class Agent {
        public:
            const std::string name;
            double population = 0;
            double next_population = 0;
            double population_for_foraging = 0;
            double base_energy = 0;
            Biome &spawning_zone;
            const bool venom;
            const bool antivenom;
            const int weapons;
            const int armor;
            const int speed;
            const bool breathes_water;
            const bool heat_resistance;
            const bool cold_resistance;
            const std::array<bool, 8> can_consume_resources;
            std::vector<Agent *> prey_agents;
            std::vector<Agent *> predator_agents;

            Agent(
                    std::string name_in,
                    Biome &spawning_zone_in,
                    bool venom_in,
                    bool antivenom_in,
                    int weapons_in,
                    int armor_in,
                    int speed_in,
                    bool breathes_water_in,
                    bool heat_resistance_in,
                    bool cold_resistance_in,
                    std::array<bool, 8> can_consume_resources_in);
            void reproduce(double prey_energy, double instances);
            void calculateNextPopulation();
            void rotateToNextPopulation() {
                population = next_population;
            }
        };

        std::vector<Agent*> agents{};
        std::array<std::vector<Agent *>, 8> resource_consuming_agents;

        Biome(
                std::string name_in,
                bool cold_in,
                bool hot_in,
                bool water_in,
                bool land_in,
                std::array<double, 8> resource_production_in) :
                name(std::move(name_in)),
                resource_production(resource_production_in),
                cold(cold_in),
                hot(hot_in),
                water(water_in),
                land(land_in) {}

        void genAgentsFromCSV(std::istream &str);
        void update();
        void populationReport();
    };

    std::vector <Biome> biomes;

    int generation = 0;

    void populationReport()
    {
        std::cout << "Generation " << generation << std::endl;
        for (auto& biome : biomes)
        {
            biome.populationReport();
        }
    }

    World() {
        biomes.push_back(Biome("Grassland", false, false, false, true, {0, 100, 1000, 2000, 0, 0, 0, 50}));
        biomes.push_back(Biome("Rainforest", false, false, false, true, {0, 1000, 2000, 100, 0, 0, 0, 50}));
        biomes.push_back(Biome("Temperate Forest", false, false, false, true, {0, 2000, 100, 1000, 0, 0, 0, 50}));
        biomes.push_back(Biome("Ocean", false, false, true, false, {10, 0, 0, 0, 10, 0, 10000, 0}));
        biomes.push_back(Biome("Benthic", false, false, true, false, {10, 0, 0, 0, 1000, 0, 0, 0}));
        biomes.push_back(Biome("Tundra", true, false, false, true, {1, 1, 1, 1, 1, 0, 0, 300}));
        biomes.push_back(Biome("Desert", false, true, false, true, {100, 0, 1, 1, 1, 0, 0, 0}));
        biomes.push_back(Biome("Shore", false, false, true, true, {0, 0, 0, 0, 20, 1000, 1000, 10}));
        biomes.push_back(Biome("River", false, false, true, true, {500, 500, 500, 500, 500, 500, 500, 500}));
        biomes.push_back(
                Biome("Human Garbage Dump", false, false, false, true, {100, 100, 100, 100, 100, 100, 100, 100}));
    }

    void genAgentsFromCSV(std::istream &str) {
        for (auto &biome : biomes) {
            str.clear();
            str.seekg(0, std::ios::beg);
            biome.genAgentsFromCSV(str);
        }
    }

    void update();
};


#endif //MLX86_DARWIN2021_H
