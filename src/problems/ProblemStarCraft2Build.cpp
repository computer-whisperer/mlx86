//
// Created by christian on 11/1/21.
//

void ProblemStarCraft2Build::dataInit(U8 *data) {
    Problem::textDataInit(data);
}

void ProblemStarCraft2Build::prettyPrintData(U8 *data) {
    Problem::textPrettyPrint(data);
}

void ProblemTravellingSalesman::scrambler(U8 *data) {

}

double ProblemStarCraft2Build::scalarTrial(U8 * data)
{
    U32 build_len = data_len/sizeof(struct StarCraft2BuildStep);
    struct StarCraft2BuildStep * build = (struct StarCraft2BuildStep *)data;
    U32 step_idx = fast_rand()%build_len;
    if ((fast_rand()%2) == 0)
    {
        build[step_idx].wait = fast_rand()%100;
    }
    if ((fast_rand()%2) == 0)
    {
        build[step_idx].command = fast_rand()%StarCraft2BuildCommand_NumCommands;
    }
}