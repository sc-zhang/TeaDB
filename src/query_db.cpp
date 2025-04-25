#include "query_db.h"

double get_similarity(std::string &ref, std::string &qry, int64_t pre_cnt) {
    double match = 0;
    double mismatch = 0;
    int64_t ridx = 0;
    int64_t qidx = 0;
    int64_t rnum = 0;
    int64_t qnum = 0;
    int64_t rsign = 1;
    int64_t qsign = 1;
    int64_t cnt = 0;
    while (ridx < ref.size() && qidx < qry.size()) {
        rnum = 0;
        qnum = 0;
        while (ref[ridx] != ',' && ridx < ref.size()) {
            if (ref[ridx] == '-') {
                rsign = -1;
            } else if (ref[ridx] >= '0' && ref[ridx] <= '9') {
                rnum *= 10;
                rnum += ref[ridx] - '0';
            }
            ++ridx;
        }
        ++ridx;
        while (qry[qidx] != ',' && qidx < qry.size()) {
            if (qry[qidx] == '-') {
                qsign = -1;
            } else if (qry[qidx] >= '0' && qry[qidx] <= '9') {
                qnum *= 10;
                qnum += qry[qidx] - '0';
            }
            ++qidx;
        }
        ++qidx;
        if (rnum * rsign == qnum * qsign) {
            ++match;
        } else {
            ++mismatch;
        }
        rsign = 1;
        qsign = 1;
        ++cnt;
        if (pre_cnt != 0 && cnt >= pre_cnt) {
            break;
        }
    }
    return match * 1.0 / (match + mismatch * 2);
}

std::vector<int64_t> get_jac(std::string temp) {
    int64_t temp_num = 0;
    std::vector<int64_t> res = {};
    int64_t sign = 1;
    for (int i = 0; i < temp.size(); ++i) {
        if (temp[i] >= '0' && temp[i] <= '9') {
            if (temp_num == -1) {
                temp_num = 0;
            }
            temp_num *= 10;
            temp_num += temp[i] - '0';
        } else if (temp[i] == '-') {
            sign = -1;
        } else {
            if (temp_num != -1) {
                res.emplace_back(sign * temp_num);
            }
            sign = 1;
            temp_num = -1;
        }
    }
    return res;
}

std::vector<std::string> get_tables(sqlite3 *db) {
    const char *query_sql = "select name from sqlite_master where type='table'";
    sqlite3_stmt *query_stmt;
    if (sqlite3_prepare_v2(db, query_sql, -1, &query_stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(query_stmt);
        sqlite3_close(db);
        return {};
    }
    std::vector<std::string> res = {};
    while (sqlite3_step(query_stmt) == SQLITE_ROW) {
        std::string table_name = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 0));
        if (table_name != "tea_info") {
            res.emplace_back(table_name);
        }
    }
    sqlite3_finalize(query_stmt);
    return res;
}

std::vector<Sample> query_db(const char *query_file, const char *db_path, int64_t pre_cnt) {
    std::vector<Sample> top10_similar_samples = {};
    std::ifstream fin;
    std::string temp;
    std::vector<uint64_t> query_similarity(SUFMAX, -1);
    std::string query_similarity_string;
    query_similarity_string = "";
    fin.open(query_file);
    if (fin) {
        while (getline(fin, temp)) {
            std::vector<int64_t> rec = get_jac(temp);
            if (rec.size() != 2) {
                continue;
            }
            query_similarity[rec[0]] = rec[1];
        }
        fin.close();
    } else {
        return {};
    }
    for (int i = 0; i < query_similarity.size(); ++i) {
        query_similarity_string += std::to_string(query_similarity[i]);
        if (i < query_similarity.size() - 1) {
            query_similarity_string += ",";
        }
    }

    sqlite3 *db = nullptr;
    int res = sqlite3_open_v2(db_path, &db, SQLITE_OPEN_READONLY, nullptr);
    if (res == SQLITE_OK) {
        std::vector<std::string> tables = get_tables(db);
        std::priority_queue<Record, std::vector<Record>, std::greater<Record>> pq;
        for (std::string &table: tables) {
            std::string query_sql = "select id, name, similarity from " + table;
            sqlite3_stmt *query_stmt;
            if (sqlite3_prepare_v2(db, query_sql.c_str(), -1, &query_stmt, nullptr) != SQLITE_OK) {
                sqlite3_finalize(query_stmt);
                sqlite3_close(db);
                return {};
            }

            while (sqlite3_step(query_stmt) == SQLITE_ROW) {
                Record rec;
                rec.id = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 0));
                rec.name = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 1));
                std::string ref_similarity = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 2));
                rec.similarity = get_similarity(ref_similarity, query_similarity_string, pre_cnt);
                if (pq.empty() || pq.size() < 10) {
                    pq.push(rec);
                } else {
                    if (rec.similarity > pq.top().similarity) {
                        pq.pop();
                        pq.push(rec);
                    }
                }
            }
            sqlite3_finalize(query_stmt);
        }
        std::vector<Record> result = {};
        while (!pq.empty()) {
            Record rec = pq.top();
            result.emplace_back(rec);
            pq.pop();
        }
        sort(result.begin(), result.end(), [](Record &x, Record &y) { return x.similarity > y.similarity; });

        for (Record &rec: result) {
            std::string englishname;
            std::string species;
            std::string group;
            std::string country;
            std::string area;

            std::string query_sql = "select englishname, species, groups, country, area from tea_info where id=\'" + rec.id + "\';";
            sqlite3_stmt *query_stmt;
            if (sqlite3_prepare_v2(db, query_sql.c_str(), -1, &query_stmt, nullptr) != SQLITE_OK) {
                sqlite3_finalize(query_stmt);
                sqlite3_close(db);
                return {};
            }
            while (sqlite3_step(query_stmt) == SQLITE_ROW) {
                englishname = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 0));
                species = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 1));
                group = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 2));
                country = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 3));
                area = reinterpret_cast<const char *>(sqlite3_column_text(query_stmt, 4));
            }
            Sample smp;
            smp.id = rec.id;
            smp.name = rec.name;
            smp.englishname = englishname;
            smp.species = species;
            smp.group = group;
            smp.country = country;
            smp.area = area;
            smp.similarity = std::to_string(rec.similarity);
            top10_similar_samples.emplace_back(smp);
            sqlite3_finalize(query_stmt);
        }
        sqlite3_close(db);
    } else {
        return {};
    }
    return top10_similar_samples;
}
