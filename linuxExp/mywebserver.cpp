//
// Created by looper on 18-10-2.
//
/* webserv.c - a minimal web server (version 0.2)
* usage: ws portnumber
* features: supports the GET command only
* runs in the current directory
* forks a new child to handle each request
* has MAJOR security holes, for demo purposes only
* has many other weaknesses, but is a good start
* build: cc webserv.c socklib.c -o webserv
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <cstdlib>
#include <dnet.h>
#include <zconf.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define HOSTLEN 256
#define BACKLOG 1

int make_server_socket(int);

int make_server_socket_q(int, int);

int connect_to_server(char *, int);

void read_til_crnl(FILE *);

void process_rq(char *, int);

void header(FILE *, char *);

void cannot_do(int);

void do_404(char *, int);

bool isadir(char *);

bool not_exist(char *);

void do_ls(char *, int );

char *file_type(char *);

bool ends_in_cgi(char *);

void do_exec(char *, int );

void do_cat(char *, int );

int main(int ac, char *av[]) {
    int sock, fd;
    FILE *fpin;
    char request[BUFSIZ];
    if (ac == 1) {
        fprintf(stderr, "usage: ws portnum\n");
        exit(1);
    }
    sock = make_server_socket(atoi(av[1]));
    if (sock == -1) exit(2);
/* main loop here */
    while (1) {
/* take a call and buffer it */
        fd = accept(sock, NULL, NULL);
        fpin = fdopen(fd, "rf");
/* read request */
        fgets(request, BUFSIZ, fpin);
        printf("got a call: request = %s", request);
        read_til_crnl(fpin);
/* do what client asks */
        process_rq(request, fd);
        fclose(fpin);
    }
}

int make_server_socket(int portnum) {
    return make_server_socket_q(portnum, BACKLOG);
}

int make_server_socket_q(int portnum, int backlog) {
    struct sockaddr_in saddr{}; /* build our address here */
    struct hostent *hp; /* this is part of our */
    char hostname[HOSTLEN]; /* address */
    int sock_id; /* the socket */
    sock_id = socket(PF_INET, SOCK_STREAM, 0); /* get a socket */
    if (sock_id == -1)
        return -1;
/** build address and bind it to socket **/
    bzero((void *) &saddr, sizeof(saddr)); /* clear out struct */
    gethostname(hostname, HOSTLEN); /* where am I ? */
    hp = gethostbyname(hostname); /* get info about host */
/* fill in host part */
    bcopy((void *) hp->h_addr, (void *) &saddr.sin_addr, hp->h_length);
    saddr.sin_port = htons(portnum); /* fill in socket port */
    saddr.sin_family = AF_INET; /* fill in addr family */
    if (bind(sock_id, (struct sockaddr *) &saddr, sizeof(saddr)) != 0)
        return -1;
/** arrange for incoming calls **/
    if (listen(sock_id, backlog) != 0)
        return -1;
    return sock_id;
}

int connect_to_server(char *host, int portnum) {
    int sock;
    struct sockaddr_in servadd{}; /* the number to call */
    struct hostent *hp; /* used to get number */
/** Step 1: Get a socket **/
    sock = socket(AF_INET, SOCK_STREAM, 0); /* get a line */
    if (sock == -1)
        return -1;
/** Step 2: connect to server **/
    bzero(&servadd, sizeof(servadd)); /* zero the address */
    hp = gethostbyname(host); /* lookup host's ip # */
    if (hp == nullptr)
        return -1;
    bcopy(hp->h_addr, (struct sockaddr *) &servadd.sin_addr, hp->h_length);
    servadd.sin_port = htons(portnum); /* fill in port number */
    servadd.sin_family = AF_INET; /* fill in socket type */
    if (connect(sock, (struct sockaddr *) &servadd, sizeof(servadd)) != 0)
        return -1;
    return sock;
}

/* ------------------------------------------------------ *
read_til_crnl(FILE *)
skip over all request info until a CRNL is seen
------------------------------------------------------ */
void read_til_crnl(FILE *fp) {
    char buf[BUFSIZ];
    while (fgets(buf, BUFSIZ, fp) != nullptr && strcmp(buf, "\r\n") != 0);
}

/* ------------------------------------------------------ *
process_rq( char *rq, int fd )
do what the request asks for and write reply to fd
handles request in a new process
rq is HTTP command: GET /foo/bar.html HTTP/1.0
------------------------------------------------------ */
void process_rq(char *rq, int fd) {
    char cmd[BUFSIZ], arg[BUFSIZ];
/* create a new process and return if not the child */
    if (fork() != 0)
        return;
    strcpy(arg, "./"); /* precede args with ./ */
    if (sscanf(rq, "%s%s", cmd, arg + 2) != 2)
        return;
    if (strcmp(cmd, "GET") != 0)
        cannot_do(fd);
    else if (not_exist(arg))
        do_404(arg, fd);
    else if (isadir(arg))
        do_ls(arg, fd);
    else if (ends_in_cgi(arg))
        do_exec(arg, fd);
    else
        do_cat(arg, fd);
}

/* ------------------------------------------------------ *
the reply header thing: all functions need one
if content_type is NULL then don't send content type
------------------------------------------------------ */
void header(FILE *fp, char *content_type) {
    fprintf(fp, "HTTP/1.0 200 OK\r\n");
    if (content_type)
        fprintf(fp, "Content-type: %s;charset=utf-8\r\n", content_type);
}

/* ------------------------------------------------------ *
simple functions first:
cannot_do(fd) unimplemented HTTP command
and do_404(item,fd) no such object
------------------------------------------------------ */
void cannot_do(int fd) {
    FILE *fp = fdopen(fd, "w");
    fprintf(fp, "HTTP/1.0 501 Not Implemented\r\n");
    fprintf(fp, "Content-type: text/plain;charset=utf-8\r\n");
    fprintf(fp, "\r\n");
    fprintf(fp, "That command is not yet implemented\r\n");
    fclose(fp);
}

void do_404(char *item, int fd) {
    FILE *fp = fdopen(fd, "w");
    fprintf(fp, "HTTP/1.0 404 Not Found\r\n");
    fprintf(fp, "Content-type: text/plain;charset=utf-8\r\n");
    fprintf(fp, "\r\n");
    fprintf(fp, "The item you requested: %s\r\nis not found\r\n",
            item);
    fclose(fp);
}

/* ------------------------------------------------------ *
the directory listing section
isadir() uses stat, not_exist() uses stat
do_ls runs ls. It should not
------------------------------------------------------ */
bool isadir(char *f) {
    struct stat info{};
    return (stat(f, &info) != -1 && S_ISDIR(info.st_mode));
}

bool not_exist(char *f) {
    struct stat info{};
    return (stat(f, &info) == -1);
}

void do_ls(char *dir, int fd) {
    FILE *fp;
    fp = fdopen(fd, "w");
    header(fp, "text/plain;charset=utf-8");
    fprintf(fp, "\r\n");
    fflush(fp);
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);
    execlp("ls", "ls", "-l", dir, NULL);
    perror(dir);
    exit(1);
}

/* ------------------------------------------------------ *
the cgi stuff. function to check extension and
one to run the program.
------------------------------------------------------ */
char *file_type(char *f) {/* returns 'extension' of file */
    char *cp;
    if ((cp = strrchr(f, '.')) != nullptr)
        return cp + 1;
    return "";
}

bool ends_in_cgi(char *f) {
    return (strcmp(file_type(f), "cgi") == 0);
}

void do_exec(char *prog, int fd) {
    FILE *fp;
    fp = fdopen(fd, "w");
    header(fp, nullptr);
    fflush(fp);
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);
    execl(prog, prog, NULL);
    perror(prog);
}

/* ------------------------------------------------------ *
do_cat(filename,fd)
sends back contents after a header
------------------------------------------------------ */
void do_cat(char *f, int fd) {
    char *extension = file_type(f);
    char *content = "text/plain;charset=utf-8";
    FILE *fpsock, *fpfile;
    int c;
    if (strcmp(extension, "html") == 0)
        content = "text/html;charset=utf-8";
    else if (strcmp(extension, "gif") == 0)
        content = "image/gif";
    else if (strcmp(extension, "jpg") == 0)
        content = "image/jpeg";
    else if (strcmp(extension, "jpeg") == 0)
        content = "image/jpeg";
    fpsock = fdopen(fd, "w");
    fpfile = fopen(f, "r");
    if (fpsock != NULL && fpfile != NULL) {
        header(fpsock, content);
        fprintf(fpsock, "\r\n");
        while ((c = getc(fpfile)) != EOF)
            putc(c, fpsock);
        fclose(fpfile);
        fclose(fpsock);
    }
    exit(0);
}

