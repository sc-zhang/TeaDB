#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <queue>

#define SUFMAX 1048576
struct Record
{
    std::string id;
    std::string name;
    double similarity;
    bool operator>(const Record &other) const
    {
        return similarity > other.similarity;
    }
};

double get_similarity(std::string &ref, std::string &qry, int64_t pre_cnt)
{
    uint64_t match = 0;
    uint64_t mismatch = 0;
    int64_t ridx = 0;
    int64_t qidx = 0;
    int64_t rnum = 0;
    int64_t qnum = 0;
    int64_t rsign = 1;
    int64_t qsign = 1;
    int64_t cnt = 0;
    while (ridx < ref.size() && qidx < qry.size())
    {
        rnum = 0;
        qnum = 0;
        while (ref[ridx] != ',' && ridx < ref.size())
        {
            if (ref[ridx] == '-')
            {
                rsign = -1;
            }
            else if (ref[ridx] >= '0' && ref[ridx] <= '9')
            {
                rnum *= 10;
                rnum += ref[ridx] - '0';
            }
            ++ridx;
        }
        ++ridx;
        while (qry[qidx] != ',' && qidx < qry.size())
        {
            if (qry[qidx] == '-')
            {
                qsign = -1;
            }
            if (qry[qidx] >= '0' && qry[qidx] <= '9')
            {
                qnum *= 10;
                qnum += qry[qidx] - '0';
            }
            ++qidx;
        }
        ++qidx;
        if (rnum * rsign == qnum * qsign)
        {
            ++match;
        }
        else
        {
            ++mismatch;
        }
        rsign = 1;
        qsign = 1;
        cnt += 1;
        if (pre_cnt != 0 && cnt >= pre_cnt)
        {
            break;
        }
    }
    return match * 1.0 / (match + mismatch * 2);
}

std::vector<int64_t> get_jac(std::string temp)
{
    int64_t temp_num = 0;
    std::vector<int64_t> res = {};
    int64_t sign = 1;
    for (int i = 0; i < temp.size(); ++i)
    {
        if (temp[i] >= '0' && temp[i] <= '9')
        {
            if (temp_num == -1)
            {
                temp_num = 0;
            }
            temp_num *= 10;
            temp_num += temp[i] - '0';
        }
        else if (temp[i] == '-')
        {
            sign = -1;
        }
        else
        {
            if (temp_num != -1)
            {
                res.emplace_back(sign * temp_num);
            }
            sign = 1;
            temp_num = -1;
        }
    }
    return res;
}

std::vector<std::string> get_tables(sqlite3 *db)
{
    const char *query_sql = "select name from sqlite_master where type='table'";
    sqlite3_stmt *query_stmt;
    if (sqlite3_prepare_v2(db, query_sql, -1, &query_stmt, nullptr) != SQLITE_OK)
    {
        std::cout << "Prepare SQL failed\n";
        sqlite3_finalize(query_stmt);
        sqlite3_close(db);
        return {};
    }
    std::vector<std::string> res = {};
    while (sqlite3_step(query_stmt) == SQLITE_ROW)
    {
        std::string table_name = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 0));
        if (table_name != "tea_info")
        {
            res.emplace_back(table_name);
        }
    }
    sqlite3_finalize(query_stmt);
    return res;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: query_db <db_file> <query_file> [pre_cnt]\n";
        return 0;
    }
    const char *query_file = argv[2];
    int64_t pre_cnt = 0;
    if (argc > 3)
    {
        for (int i = 0; i < strlen(argv[3]); ++i)
        {
            pre_cnt *= 10;
            pre_cnt += argv[3][i] - '0';
        }
    }
    std::ifstream fin;
    std::string temp;
    std::vector<uint64_t> query_similarity(SUFMAX, -1);
    std::string query_similarity_string = "";
    fin.open(query_file);
    int cnt = 0;
    if (fin)
    {
        while (getline(fin, temp))
        {
            std::vector<int64_t> rec = get_jac(temp);
            if (rec.size() != 2)
            {
                continue;
            }
            query_similarity[rec[0]] = rec[1];
        }
        fin.close();
    }
    else
    {
        std::cout << "Open query file failed\n";
        return -1;
    }
    for (int i = 0; i < query_similarity.size(); ++i)
    {
        query_similarity_string += std::to_string(query_similarity[i]);
        if (i < query_similarity.size() - 1)
        {
            query_similarity_string += ",";
        }
    }

    const char *db_path = argv[1];
    sqlite3 *db = nullptr;
    int res = sqlite3_open_v2(db_path, &db, SQLITE_OPEN_READONLY, nullptr);
    if (res == SQLITE_OK)
    {

        std::vector<std::string> tables = get_tables(db);

        std::priority_queue<Record, std::vector<Record>, std::greater<Record>> pq;
        for (std::string &table : tables)
        {
            std::string query_sql = "select id, name, similarity from " + table;
            sqlite3_stmt *query_stmt;
            if (sqlite3_prepare_v2(db, query_sql.c_str(), -1, &query_stmt, nullptr) != SQLITE_OK)
            {
                std::cout << "Prepare SQL failed\n";
                sqlite3_finalize(query_stmt);
                sqlite3_close(db);
                return -1;
            }

            while (sqlite3_step(query_stmt) == SQLITE_ROW)
            {
                Record rec;
                rec.id = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 0));
                rec.name = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 1));
                std::string ref_similarity = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 2));
                rec.similarity = get_similarity(ref_similarity, query_similarity_string, pre_cnt);
                if (pq.empty() || pq.size() < 10)
                {
                    pq.push(rec);
                }
                else
                {
                    if (rec.similarity > pq.top().similarity)
                    {
                        pq.pop();
                        pq.push(rec);
                    }
                }
            }
            sqlite3_finalize(query_stmt);
        }
        std::vector<Record> result = {};
        while (!pq.empty())
        {
            Record rec = pq.top();
            result.emplace_back(rec);
            pq.pop();
        }
        sort(result.begin(), result.end(), [](Record &x, Record &y)
             { return x.similarity > y.similarity; });
        std::cout << "ID\tChinese name\tEnglish name\tSpecies\tGroup\tCountry\tArea\tSimilarity\n";
        for (Record &rec : result)
        {
            std::string englishname;
            std::string species;
            std::string group;
            std::string country;
            std::string area;

            std::string query_sql = "select englishname, species, groups, country, area from tea_info where id=\'" + rec.id + "\';";
            sqlite3_stmt *query_stmt;
            if (sqlite3_prepare_v2(db, query_sql.c_str(), -1, &query_stmt, nullptr) != SQLITE_OK)
            {
                std::cout << "Prepare SQL failed\n";
                sqlite3_finalize(query_stmt);
                sqlite3_close(db);
                return -1;
            }
            while (sqlite3_step(query_stmt) == SQLITE_ROW)
            {
                englishname = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 0));
                species = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 1));
                group = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 2));
                country = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 3));
                area = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 4));
            }
            std::cout
                << rec.id << "\t" << rec.name << "\t" << englishname << "\t" << species << "\t" << group << "\t" << country << "\t" << area << "\t" << rec.similarity << "\n";
            sqlite3_finalize(query_stmt);
        }
        sqlite3_close(db);
    }
    else
    {
        std::cout << "Open database failed\n";
    }
    return 0;
}