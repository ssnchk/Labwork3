#include "SandParse.h"

int main(int argc,char* argv[]){
    Args args=Parse(argc,argv);
    Field pole;
    tsv_args tsv=Tsv_Parse(args.input);
    pole = tsv_to_field(tsv);
    Sandpile pile(pole);
    pile.all_field_skattering(args.max_iter,args.freq,args.output);
}
