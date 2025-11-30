/* VERY simplified: post text to PHP endpoint */
#include <stdio.h>
#include <curl/curl.h>

int post_sdp(const char *url, const char *json_payload){
    CURL *c = curl_easy_init();
    if(!c) return 0;
    struct curl_slist *hs = NULL;
    hs = curl_slist_append(hs, "Content-Type: application/json");
    curl_easy_setopt(c, CURLOPT_HTTPHEADER, hs);
    curl_easy_setopt(c, CURLOPT_URL, url);
    curl_easy_setopt(c, CURLOPT_POSTFIELDS, json_payload);
    CURLcode res = curl_easy_perform(c);
    curl_slist_free_all(hs);
    curl_easy_cleanup(c);
    return res == CURLE_OK;
}

