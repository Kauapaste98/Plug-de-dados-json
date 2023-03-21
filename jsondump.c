#include <stdio.h>
#include <sqlite3.h>
#include <cjson/cJSON.h>

int main(int argc, char** argv) {
    sqlite3* db;
    int rc = sqlite3_open("mydatabase.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    cJSON* json = cJSON_ParseFile("data.json");

    if (json == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo JSON\n");
        sqlite3_close(db);
        return 1;
    }

    cJSON* item = NULL;
    cJSON_ArrayForEach(item, json) {
        const char* name = cJSON_GetObjectItem(item, "name")->valuestring;
        const char* email = cJSON_GetObjectItem(item, "email")->valuestring;

        char* sql = sqlite3_mprintf("INSERT INTO users(name, email) VALUES('%q', '%q')", name, email);

        rc = sqlite3_exec(db, sql, NULL, NULL, NULL);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "Erro ao inserir registro: %s\n", sqlite3_errmsg(db));
            sqlite3_free(sql);
            sqlite3_close(db);
            return 1;
        }

        sqlite3_free(sql);
    }

    cJSON_Delete(json);
    sqlite3_close(db);

    return 0;
}
