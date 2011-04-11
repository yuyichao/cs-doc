int main(int argv,char **argc) {
        char buf[256];
        printf("%p\n",buf);
        strcpy(buf,argc[1]);
}
