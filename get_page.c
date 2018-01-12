#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>

static void
print_cookies(CURL *curl)
{
    CURLcode res;
    struct curl_slist *cookies;
    struct curl_slist *nc;
    int i;

    printf("Cookies, curl knows:\n");
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if (res != CURLE_OK) {
        fprintf(stderr, "Curl curl_easy_getingo failed: %s\n",
                curl_easy_strerror(res));
        exit(1);
    }
    nc = cookies;
    i = 1;
    while (nc) {
        printf("[%d]: %s\n", i, nc->data);
        nc = nc->next;
        i++;
    }
    if (i == 1)
        printf("(none)\n");
    curl_slist_free_all(cookies);
}

int main(void)
{
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        char nline[256];

        curl_easy_setopt(curl, CURLOPT_URL, "");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
            return 1;
        }

        print_cookies(curl);

        printf("Erasing curl's knowledge of cookies!\n");
        curl_easy_setopt(curl, CURLOPT_COOKIELIST, "ALL");

        print_cookies(curl);

        printf("-----------------------------------------------\n"
               "Setting a cookie \"PREF\" via cookie interface:\n");

        /* Netscape format cookie */ 
        snprintf(nline, sizeof(nline), "%s\t%s\t%s\t%s\t%lu\t%s\t%s",
                 ".example.com", "TRUE", "/", "FALSE",
                 (unsigned long)time(NULL) + 31337UL,
                 "PREF", "hello example, i like you very much!");
        res = curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);
        if(res != CURLE_OK) {
            fprintf(stderr, "Curl curl_easy_setopt failed: %s\n",
                    curl_easy_strerror(res));
            return 1;
        }

        /* HTTP-header style cookie. If you use the Set-Cookie format and don't
           specify a domain then the cookie is sent for any domain and will not be
           modified, likely not what you intended. Starting in 7.43.0 any-domain
           cookies will not be exported either. For more information refer to the
           CURLOPT_COOKIELIST documentation.
        */ 
        snprintf(nline, sizeof(nline),
                 "Set-Cookie: OLD_PREF=3d141414bf4209321; "
                 "expires=Sun, 17-Jan-2038 19:14:07 GMT; path=/; domain=.example.com");
        res = curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);
        if(res != CURLE_OK) {
            fprintf(stderr, "Curl curl_easy_setopt failed: %s\n",
                    curl_easy_strerror(res));
            return 1;
        }
 
        print_cookies(curl);
 
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
            return 1;
        }
 
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Curl init failed!\n");
        return 1;
    }

    curl_global_cleanup();
    return 0;
}

/*#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	struct sockaddr_in servaddr;
	struct hostent *hp;
	int sock_id;
	char message[1024*1024];
	char msglen;
	char request[] = "GET /auth/login HTTP/1.1\n"
        "Host: test.com\n"
        "Authorization: name:pass\n\n";
	
	//Get a socket	
	if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            fprintf(stderr,"Couldn't get a socket.\n"); exit(EXIT_FAILURE);
	}
	else {
            fprintf(stderr,"Got a socket.\n");
	}
	
	//book uses bzero which my man pages say is deprecated
	//the man page said to use memset instead. :-)
	memset(&servaddr,0,sizeof(servaddr));
	
	//get address for google.com
	if((hp = gethostbyname(argv[1])) == NULL) {
            fprintf(stderr,"Couldn't get an address.\n"); exit(EXIT_FAILURE);
	}
	else {
            fprintf(stderr,"Got an address.\n");
	}
	
	//bcopy is deprecated also, using memcpy instead
	memcpy((char *)&servaddr.sin_addr.s_addr, (char *)hp->h_addr, hp->h_length);
	
	//fill int port number and type
	servaddr.sin_port = htons(80);
	servaddr.sin_family = AF_INET;
	
	//make the connection
	if(connect(sock_id, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
            fprintf(stderr, "Couldn't connect.\n");
	}
	else {
            fprintf(stderr,"Got a connection!!!\n");
	}
	
	//NOW THE HTTP PART!!!
	
	//send the request
	write(sock_id,request,strlen(request));
	
	//read the response
	msglen = read(sock_id,message,1024*1024);
	
	//print the reasponse
	fprintf(stdout,"%s\n",message);
	
	return 0;
}
*/
