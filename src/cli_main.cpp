//
// Created by 张晟铖 on 2025/4/25.
//
#include "query_db.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "Usage: query_db <db_file> <query_file> [pre_cnt]\n";
        return 0;
    }
    const char *query_file = argv[2];
    int64_t pre_cnt = 0;
    if (argc > 3) {
        for (int i = 0; i < strlen(argv[3]); ++i) {
            pre_cnt *= 10;
            pre_cnt += argv[3][i] - '0';
        }
    }
    const char *db_path = argv[1];
    std::vector<Sample> top10_similar_samples = query_db(query_file, db_path, pre_cnt);
    if (!top10_similar_samples.empty()) {
        std::cout << "ID\tChinese name\tEnglish name\tSpecies\tGroup\tCountry\tArea\tSimilarity\n";
        for (const Sample &smp: top10_similar_samples) {
            std::cout << smp.id << "\t" << smp.name << "\t" << smp.englishname << "\t" << smp.species << "\t" << smp.group << "\t" << smp.country << "\t" << smp.area << "\t" << smp.similarity << "\n";
        }
    } else {
        std::cout << "Query failed!\n";
    }
    return 0;
}