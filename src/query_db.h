//
// Created by 张晟铖 on 2025/4/25.
//

#ifndef TEADB_QUERY_DB_H
#define TEADB_QUERY_DB_H

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <sqlite3.h>
#include <string>
#include <vector>

#define SUFMAX 1048576

struct Record {
    std::string id;
    std::string name;
    double similarity;
    bool operator>(const Record &other) const {
        return similarity > other.similarity;
    }
};

struct Sample {
    std::string id;
    std::string name;
    std::string englishname;
    std::string species;
    std::string group;
    std::string country;
    std::string area;
    std::string similarity;
};

double get_similarity(std::string &ref, std::string &qry, int64_t pre_cnt);
std::vector<int64_t> get_jac(std::string temp);
std::vector<std::string> get_tables(sqlite3 *db);
std::vector<Sample> query_db(const char *query_file, const char *db_path, int64_t pre_cnt);

#endif//TEADB_QUERY_DB_H
