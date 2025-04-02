typedef struct kommon_{
	float a,b;
	int   c;
	double d;
} kommon;
extern kommon global_;

extern void subsub_();

main()
{
global_.a=0.5;
global_.b=0.6E-13;
global_.c=-20;
global_.d=1.23456789E18;
subsub_();
}
