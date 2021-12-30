//
// Created by christian on 10/9/21.
//

#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <cmath>
#include <chrono>

#include "darwin2021.h"

using namespace std;

World::Biome::Agent::Agent(
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
        std::array<bool, 8> can_consume_resources_in) :
        name(std::move(name_in)),
        spawning_zone(spawning_zone_in),
        venom(venom_in),
        antivenom(antivenom_in),
        weapons(weapons_in),
        armor(armor_in),
        speed(speed_in),
        breathes_water(breathes_water_in),
        cold_resistance(cold_resistance_in),
        heat_resistance(heat_resistance_in),
        can_consume_resources(can_consume_resources_in) {
    base_energy = 0.1;
    if (venom) {
        base_energy += 6;
    }
    if (antivenom) {
        base_energy += 1;
    }
    if (heat_resistance) {
        base_energy += 2;
    }
    if (cold_resistance) {
        base_energy += 2;
    }
    base_energy += armor * 0.75;
    base_energy += weapons;
    base_energy += speed;
    double resource_cost[] = {6, 5, 3, 1, 3, 4, 2, 1};
    for (int i = 0; i < 8; i++) {
        if (can_consume_resources[i]) {
            base_energy += resource_cost[i];
        }
    }

    // Search for food chain elements
    for (auto &agent : spawning_zone.agents) {
        bool is_prey = false;
        bool is_predator = false;
        if (agent == this)
        {
            continue;
        }
        if (speed > agent->speed) {
            // We can catch them
            if (venom && !agent->antivenom) {
                is_prey = true;
            } else if (weapons > (agent->weapons + agent->armor)) {
                is_prey = true;
            }
        } else if (agent->speed > speed) {
            // They can catch us
            if (agent->venom && !antivenom) {
                is_predator = true;
            } else if (agent->weapons > (weapons + armor)) {
                is_predator = true;
            }
        }
        if (is_prey) {
            prey_agents.push_back(agent);
            agent->predator_agents.push_back(this);
        }
        if (is_predator) {
            predator_agents.push_back(agent);
            agent->prey_agents.push_back(this);
        }
    }

    // Register resource consumer
    for (uint32_t i = 0; i < spawning_zone.resource_consuming_agents.size(); i++)
    {
        if (can_consume_resources[i])
        {
            spawning_zone.resource_consuming_agents[i].push_back(this);
        }
    }

    // Calculate starting population
    population = 50000/base_energy;
}

void World::Biome::Agent::reproduce(double prey_energy, double instances) {
    double saved_energy = base_energy + 1.0 - 0.2;
    double total_energy = saved_energy + prey_energy;
    double reproduction_opportunities = floor(10 * total_energy) * instances;
    double success_ratio = 1 / (base_energy * 10);
    next_population += success_ratio * reproduction_opportunities;
    if (isinf(next_population) || isnan(next_population) || (next_population < 0))
    {
        throw std::runtime_error("NAN!");
    }
    if (next_population > 1000000)
    {
        cout << "Probably something wrong";
    }
}

void World::Biome::Agent::calculateNextPopulation() {
    double current_population_left = population;
    next_population = 0;

    for (auto &predator : predator_agents) {
        double pop_ratio = (predator->population / spawning_zone.total_population);
        double affected_population = population * pop_ratio;
        current_population_left -= affected_population;
    }
    for (auto &prey : prey_agents) {
        double pop_ratio = (prey->population / spawning_zone.total_population);
        double affected_population = population * pop_ratio;
        current_population_left -= affected_population;
        reproduce(0.95 * base_energy, affected_population);
    }
    population_for_foraging = current_population_left;
    if (population_for_foraging < 0)
    {
        population_for_foraging = 0;
    }

    if (isinf(population_for_foraging) || isnan(population_for_foraging) || isinf(next_population) || isnan(next_population))
    {
        throw std::runtime_error("NAN!");
    }
}

void World::Biome::genAgentsFromCSV(std::istream &str) {
    std::string line;
    std::getline(str, line);// Kill header
    while (std::getline(str, line)) {
        std::stringstream lineStream(line);
        std::string cell;

        // timestamp
        std::getline(lineStream, cell, ',');

        // Name
        std::string agent_name;
        std::getline(lineStream, agent_name, ',');

        // Spawning location
        std::getline(lineStream, cell, ',');
        if (cell.find(name) == std::string::npos) {
            continue; // Not for this biome
        }

        // Venomous
        bool venom = false;
        bool antivenom = false;
        std::getline(lineStream, cell, ',');
        if (cell.find("Venom") != std::string::npos) {
            venom = true;
        }
        if (cell.find("Antivenom") != std::string::npos) {
            antivenom = true;
        }

        // Weapons
        std::getline(lineStream, cell, ',');
        int weapons = stoi(cell);

        // Armor
        std::getline(lineStream, cell, ',');
        int armor = stoi(cell);

        // Speed
        std::getline(lineStream, cell, ',');
        int speed = stoi(cell);

        // Forager
        std::getline(lineStream, cell, ',');
        array<bool, 8> can_consume_resources{
            cell.find("Carrion") != std::string::npos,
            cell.find("Leaves") != std::string::npos,
            cell.find("Grass") != std::string::npos,
            cell.find("Seeds") != std::string::npos,
            cell.find("Detritus") != std::string::npos,
            cell.find("Coconuts") != std::string::npos,
            cell.find("Algae") != std::string::npos,
            cell.find("Lichen") != std::string::npos};

        // Temperature Tolerance
        std::getline(lineStream, cell, ',');
        bool cold_resistance = cell.find("Cold") != std::string::npos;
        bool heat_resistance = cell.find("Heat") != std::string::npos;

        if (cold && !cold_resistance) {
            continue; // Invalid creature
        }
        if (hot && !heat_resistance) {
            continue; // Invalid creature
        }
        auto agent = new Agent(agent_name, *this, venom, antivenom, weapons, armor, speed, cold_resistance,
                               heat_resistance, water, can_consume_resources);
        agents.push_back(agent);
    }
}

void World::Biome::update() {
    // Replenish resources
    for (int i = 0; i < resources.size(); i++) {
        if (resources[i] < 0)
        {
            resources[i] = 0;
        }
        resources[i] += resource_production[i];
        if (resources[i] > 100000)
        {
            cout << "Problem!";
        }
    }

    total_population = 0;
    for (auto &agent: agents) {
        total_population += agent->population;
    }

    for (auto &agent: agents) {
        agent->calculateNextPopulation();
    }

    for (uint32_t i = 0; i < resources.size(); i++)
    {
        if (resources[i] == 0)
        {
            continue;
        }

        double population_consuming_resource = 0;
        for (auto &agent: resource_consuming_agents[i])
        {
            population_consuming_resource += agent->population_for_foraging;
        }

        if (population_consuming_resource <= 0)
        {
            continue;
        }

        double resources_available = resources[i];
        double resources_consumed = resources_available;
        if (resources_available > population_consuming_resource)
        {
            resources_consumed = population_consuming_resource;
        }


        double ratio_satisfied = resources_consumed/population_consuming_resource;

        if (isnan(ratio_satisfied))
        {
            throw std::runtime_error("NAN!");
        }

        for (auto &agent: resource_consuming_agents[i])
        {
            double population_satisfied_of_agent = ratio_satisfied*agent->population_for_foraging;
            double energy_gained[] = {10, 7, 6, 5, 4, 3, 2, 1};
            agent->population_for_foraging -= population_satisfied_of_agent;
            agent->reproduce(energy_gained[i], population_satisfied_of_agent);
        }
        resources[i] -= resources_consumed;
    }

    for (auto &agent: agents) {
        agent->rotateToNextPopulation();
    }
}

void World::Biome::populationReport() {
    cout << "  " << name << endl;
    for (auto &agent: agents) {
        if (agent->population < 1)
        {
            continue;
        }
        cout << "    " << agent->name << ": " << agent->population << endl;
    }
}

void World::update() {
    auto start_time = std::chrono::high_resolution_clock::now();
    for (auto& biome : biomes){
        biome.update();
    }
    generation++;
    std::cout << "Simulation took " << duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count() << "us" << std::endl;
}

int main(int argc, char* argv[])
{
    World world;

    ifstream ifs("../src/problems/darwin2021/entries.csv");
    world.genAgentsFromCSV(ifs);
    ifs.close();
    world.populationReport();
    for (int i = 0; i < 100; i++)
    {
        world.update();
    }
    world.populationReport();
}


