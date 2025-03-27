#!/usr/bin/env python3
import sqlite3
import os
import argparse


def get_opts():
    group = argparse.ArgumentParser()
    group.add_argument("-l", "--list", help="Input infomation list", required=True)
    group.add_argument("-i", "--input", help="Input jac directory", required=True)
    group.add_argument("-o", "--output", help="Output db file", required=True)
    return group.parse_args()


def create_db(jac_dir, info_list, out_db):
    print("Preparing")
    db = out_db
    if os.path.exists(db):
        os.remove(db)

    conn = sqlite3.connect(db)
    cur = conn.cursor()

    group_db = {}
    group_set = set()
    name_db = {}

    print("Create information table")
    sql = "CREATE TABLE tea_info(id text primary key not null, name text no null, englishname text not null, species text not null, groups text not null, country text not null, area text not null);"
    cur.execute(sql)

    print("Inserting sample informations")
    with open(info_list, "r") as fin:
        for line in fin:
            data = line.strip().split("\t")
            if len(data) != 7:
                continue
            group_db[data[0]] = data[4]
            name_db[data[0]] = data[1]
            group_set.add(data[4])
            insert_data = ["'%s'" % _ for _ in data]
            if insert_data:
                sql = (
                    "INSERT INTO tea_info(id, name, englishname, species, groups, country, area) VALUES(%s);"
                    % (",".join(insert_data))
                )
                cur.execute(sql)

    print("Creating tables by group")
    for group in group_set:
        sql = (
            "CREATE TABLE %s(id text primary key not null,name text not null,similarity not null,foreign key(id) references tea(id));"
            % (group.replace(".", "_"))
        )
        cur.execute(sql)

    print("Inserting data by groups")
    for fn in os.listdir(jac_dir):
        if fn.endswith(".jac"):
            smp = fn.split(".")[0]
            print("\tReading %s" % smp)
            if smp not in group_db:
                continue
            group = group_db[smp].replace(".", "_")
            full_fn = os.path.join(jac_dir, fn)
            suff_list = [-1 for _ in range(4**10)]
            with open(full_fn, "r") as fin:
                for line in fin:
                    data = line.strip().split()
                    if not data:
                        continue
                    if data[0].isdigit():
                        pre = int(data[0])
                        suf = int(data[1])
                        suff_list[pre] = suf
            sql = "INSERT INTO %s (id, name, similarity) VALUES('%s', '%s', '%s');" % (
                group,
                smp,
                name_db[smp] if smp in name_db else "--",
                ",".join(map(str, suff_list)),
            )
            cur.execute(sql)

    print("Saving database")
    conn.commit()
    conn.close()
    print("Finished")


if __name__ == "__main__":
    opts = get_opts()
    info_list = opts.list
    jac_dir = opts.input
    out_db = opts.output
    create_db(jac_dir, info_list, out_db)
