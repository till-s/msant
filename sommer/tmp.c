main()

{
char name[30];
gethostname(name,30);
printf("%s %x\n",name,gethostid());

}
