#include "mongoose.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        (void) fn_data;

        if (mg_http_match_uri(hm, "/api/run") && mg_strcmp(hm->method, mg_str("OPTIONS")) == 0) {
            mg_http_reply(c, 200, 
                "Access-Control-Allow-Origin: *\r\n"
                "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
                "Access-Control-Allow-Headers: Content-Type\r\n", 
                "");
            return;
        }

        if (mg_http_match_uri(hm, "/api/algorithms")) {
            char json_response[2048] = "[";
            DIR *d = opendir("../algorithms/build");
            if (d) {
                struct dirent *dir; 
                int first = 1;
                while ((dir = readdir(d)) != NULL) {
                    if (strcmp(dir->d_name, ".") != 0 && 
                        strcmp(dir->d_name, "..") != 0 && 
                        dir->d_name[0] != '.') {
                        if (!first) strcat(json_response, ",");
                        strcat(json_response, "\""); 
                        strcat(json_response, dir->d_name); 
                        strcat(json_response, "\"");
                        first = 0;
                    }
                }
                closedir(d);
            }
            strcat(json_response, "]");
            mg_http_reply(c, 200, 
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n", 
                "%s", json_response);
            return;
        } 
        
        if (mg_http_match_uri(hm, "/api/run")) {
            char *algo_name_ptr = mg_json_get_str(hm->body, "$.algorithm");
            char *processes_str_ptr = mg_json_get_str(hm->body, "$.processes");
            
            if (algo_name_ptr == NULL || processes_str_ptr == NULL) {
                mg_http_reply(c, 400, "", "{\"error\":\"Missing 'algorithm' or 'processes'\"}");
                if (algo_name_ptr) free(algo_name_ptr);
                if (processes_str_ptr) free(processes_str_ptr);
                return;
            }

            char command[4096];
            
            if (strcmp(algo_name_ptr, "RR") == 0 || 
                strcmp(algo_name_ptr, "multilevel_static") == 0 || 
                strcmp(algo_name_ptr, "multilevel_dynamic") == 0) {
                
                double quantum_d;
                if (mg_json_get_num(hm->body, "$.quantum", &quantum_d)) {
                    int quantum = (int)quantum_d;
                    snprintf(command, sizeof(command), 
                             "../algorithms/build/%s \"%s\" %d", 
                             algo_name_ptr, processes_str_ptr, quantum);
                } else {
                    snprintf(command, sizeof(command), 
                             "../algorithms/build/%s \"%s\" %d", 
                             algo_name_ptr, processes_str_ptr, 2);
                }
            } else {
                snprintf(command, sizeof(command), 
                         "../algorithms/build/%s \"%s\"", 
                         algo_name_ptr, processes_str_ptr);
            }
            
            FILE *fp = popen(command, "r");
            
            free(algo_name_ptr);
            free(processes_str_ptr);
            
            if (fp == NULL) {
                mg_http_reply(c, 500, "", "{\"error\":\"Failed to execute algorithm binary\"}");
                return;
            }

            char result[8192] = "";
            char buffer[512];
            while (fgets(buffer, sizeof(buffer), fp) != NULL) {
                if (strlen(result) + strlen(buffer) < sizeof(result) - 1) {
                    strcat(result, buffer);
                }
            }
            pclose(fp);
            
            if (strlen(result) == 0) {
                mg_http_reply(c, 500, "", "{\"error\":\"Algorithm produced no output\"}");
            } else {
                mg_http_reply(c, 200, 
                    "Content-Type: application/json\r\n"
                    "Access-Control-Allow-Origin: *\r\n", 
                    "%s", result);
            }
            return;
        } 
        
        mg_http_reply(c, 404, "", "{\"error\":\"Endpoint not found\"}");
    }
}

int main(void) {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    printf("Starting C Scheduler Backend on http://localhost:3001\n");
    mg_http_listen(&mgr, "http://localhost:3001", fn, NULL);
    for (;;) mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
    return 0;
}